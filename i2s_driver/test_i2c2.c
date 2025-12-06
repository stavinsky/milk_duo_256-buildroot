
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

#include "i2s_driver.h"

// vendor undocumented mambo jumbo
// code from
// https://community.milkv.io/t/i2s-milk-v-duo-speaker-max98357a/668/46
// https://community.milkv.io/u/emeb
#define ETH_BASE_ADDRESS 0x03009000  // Base address of ETH registers on Duo

// Offsets for ETH registers
#define ETH_070 0x070
#define ETH_074 0x074
#define ETH_078 0x078
#define ETH_07C 0x07C
#define ETH_800 0x800
#define ETH_804 0x804
#define ETH_808 0x806
// word access macro
#define word(x) (x / sizeof(unsigned int))
static int mambo_jumbo() {
    int mem_fd;
    void* reg_map;

    // Open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("can't open /dev/mem");
        return 1;
    }

    // Map registers to user space memory
    reg_map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
                   ETH_BASE_ADDRESS);

    if (reg_map == MAP_FAILED) {
        perror("mmap error");
        close(mem_fd);
        return 1;
    }

    // Access ETH registers
    volatile unsigned int* eth_reg = (volatile unsigned int*)reg_map;
    unsigned int temp;

    // 0x03009804[0] = 1'b1 (rg_ephy_apb_rw_sel=1, use apb interface)
    temp = eth_reg[word(ETH_804)];
    printf("READ 804: 0x%08X\n", temp);
    temp |= 0x1;
    eth_reg[word(ETH_804)] = temp;
    printf("WRITE 804: 0x%08X\n", temp);

    // 0x03009808[4:0] = 5'b00001 (rg_ephy_pll_stable_cnt[4:0] = 5'd1 (10us))
    temp = eth_reg[word(ETH_808)];
    printf("READ 808: 0x%08X\n", temp);
    temp &= ~0x1F;
    temp |= 0x01;
    eth_reg[word(ETH_808)] = temp;
    printf("WRITE 808: 0x%08X\n", temp);

    // 0x03009800[2] = 0x0905 (rg_ephy_dig_rst_n=1, reset release, other keep
    // default)
    temp = eth_reg[word(ETH_800)];
    printf("READ 800: 0x%08X\n", temp);
    temp = 0x0905;
    eth_reg[word(ETH_800)] = temp;
    printf("WRITE 808: 0x%08X\n", temp);

    // Delay 10us
    usleep(10);

    // 0x0300907C[12:8]= 5'b00101 (page_sel_mode0 = page 5)
    temp = eth_reg[word(ETH_07C)];
    printf("READ 07C: 0x%08X\n", temp);
    temp &= ~(0x1F << 8);
    temp |= (0x05 << 8);
    eth_reg[word(ETH_07C)] = temp;
    printf("WRITE 07C: 0x%08X\n", temp);

    // 0x03009078[11:0] = 0xF00 (set to gpio from top)
    temp = eth_reg[word(ETH_078)];
    printf("READ 078: 0x%08X\n", temp);
    temp &= ~0xFFF;
    temp |= 0xF00;
    eth_reg[word(ETH_078)] = temp;
    printf("WRITE 078: 0x%08X\n", temp);

    // 0x03009074[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
    temp = eth_reg[word(ETH_074)];
    printf("READ 074: 0x%08X\n", temp);
    temp |= 0x606;
    eth_reg[word(ETH_074)] = temp;
    printf("WRITE 074: 0x%08X\n", temp);

    // 0x03009070[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
    temp = eth_reg[word(ETH_070)];
    printf("READ 070: 0x%08X\n", temp);
    temp |= 0x606;
    eth_reg[word(ETH_070)] = temp;
    printf("WRITE 070: 0x%08X\n", temp);

    // Unmap memory
    munmap(reg_map, 4096);

    // Close /dev/mem
    close(mem_fd);

    return 0;
}

static inline int update_reg(volatile uint32_t* regs, uint32_t register_offset,
                             uint32_t value, uint32_t left, uint32_t right) {
    printf("update offset %X\n", register_offset);
    if (left >= 32 || right >= 32 || left < right) return -1;
    unsigned width = left - right + 1;
    uint32_t field_mask = ((1u << width) - 1u) << right;
    value &= (1u << width) - 1u;
    uint32_t idx = register_offset / 4;
    uint32_t reg = regs[idx];
    reg = (reg & ~field_mask) | (value << right);
    regs[idx] = reg;
    return 0;
}

#define i2s_tdm_2_sdi_in_sel_mask (0b111 << 8)

int main(void) {
    // mambo_jumbo();
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    /// pinmux
    void* pinmux =
        mmap(NULL, PINMUX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PINMUX);
    if (pinmux == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    volatile uint32_t* pinmux_regs = (volatile uint32_t*)pinmux;
    update_reg(pinmux_regs, 0x128, 7, 3, 0);  // BCLK PIN
    update_reg(pinmux_regs, 0x130, 7, 3, 0);  // DI PIN
    update_reg(pinmux_regs, 0x12C, 7, 3, 0);  // DO PIN
    update_reg(pinmux_regs, 0x124, 7, 3, 0);  // LRCK

    /// clkgen
    void* clk_gen =
        mmap(NULL, CLKGEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CLKGEN);
    if (clk_gen == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    volatile uint32_t* clk_gen_regs = (volatile uint32_t*)clk_gen;
    // update_reg(clk_gen_regs, clk_en_0, 1, 26, 26);  // clk_axi4_eth0
    // update_reg(clk_gen_regs, clk_en_0, 1, 29, 29);  // clk_apb_gpio
    update_reg(clk_gen_regs, clk_en_1, 1, 26, 26);  // clk_apb_i2s2
    // update_reg(clk_gen_regs, clk_en_2, 1, 1, 1);    // clk_axi4
    // update_reg(clk_gen_regs, clk_en_4, 1, 1, 1);  //
    // update_reg(clk_gen_regs, clk_en_4, 1, 2, 2);  // clk_audsrc
    // update_reg(clk_gen_regs, clk_en_4, 1, 4, 4);  //  clk_apb_audsrc

    void* i2s_global = mmap(NULL, I2S_GLOBAL_SIZE, PROT_READ | PROT_WRITE,
                            MAP_SHARED, fd, I2S_GLOBAL);
    if (i2s_global == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    volatile uint32_t* global_regs = (volatile uint32_t*)i2s_global;
    // global I2S config
    printf("global\n");
    update_reg(global_regs, I2S_TDM_SDI_IN_SEL, 0b110, 10, 8);
    update_reg(global_regs, I2S_TDM_SDO_OUT_SEL, 0b110, 10, 8);
    global_regs[AUDIO_PDM_CTRL / 4] = 0;

    // tdm2 config
    printf("tdm2\n");

    void* i2s2 =
        mmap(NULL, I2S2_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, I2S2);
    if (i2s2 == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    volatile uint32_t* tdm2_regs = (volatile uint32_t*)i2s2;

    /// not sure
    update_reg(tdm2_regs, I2S_BLK_MODE_SETTING, 1, 1, 1);  // master mode

    ///
    update_reg(tdm2_regs, I2S_CLK_CTRL1, 1, 15, 0);   // mclk_div
    update_reg(tdm2_regs, I2S_CLK_CTRL1, 2, 31, 16);  // bclk_div

    update_reg(tdm2_regs, I2S_CLK_CTRL0, 0, 0,
               0);                                  // aud_clk_sel from internal pll
    update_reg(tdm2_regs, I2S_CLK_CTRL0, 1, 6, 6);  // bclk_out_clk_force_en
    update_reg(tdm2_regs, I2S_CLK_CTRL0, 1, 7, 7);  // mclk_out_en
    update_reg(tdm2_regs, I2S_CLK_CTRL0, 1, 8, 8);  // aud_en

    // i2s reset
    printf("i2s reset\n");
    tdm2_regs[I2S_I2S_RESET / 4] = 0b11;  // i2s_reset_rx and i2s_reset_tx
    usleep(100 * 1000);
    tdm2_regs[I2S_I2S_RESET / 4] = 0b00;  // i2s_reset_rx and i2s_reset_tx

    // fifo reset
    printf("fifo reset\n");
    tdm2_regs[I2S_FIFO_RESET / 4] = 0b11;  // rx_fifo_reset and tx_fifo_reset
    usleep(100 * 1000);
    tdm2_regs[I2S_FIFO_RESET / 4] = 0b00;  // rx_fifo_reset and tx_fifo_reset

    // i2s enable

    printf("i2s enable \n");
    tdm2_regs[I2S_I2S_ENABLE / 4] = 0b1;  // i2s_enable

    printf("random start\n");
    update_reg(tdm2_regs, I2S_BLK_CFG, 1, 8, 8);  // rx_blk_clk_force_en
    update_reg(tdm2_regs, I2S_BLK_CFG, 1, 9, 9);  // rx_fifo_dma_clk_force_en

    munmap(i2s_global, I2S_GLOBAL_SIZE);
    munmap(i2s2, I2S2_SIZE);
    close(fd);
    return 0;
}