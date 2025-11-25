
#define PINMUX 0x03001000
#define PINMUX_SIZE 0x1000

#define I2S_GLOBAL 0x04108000UL
#define I2S_GLOBAL_SIZE 0x10000

#define I2S2 0x04120000UL
#define I2S2_SIZE 0x10000

#define CLKGEN 0x03002000
#define CLKGEN_SIZE 0x1000

#define I2S_TDM_SCLK_IN_SEL 0x000  // looks like not needed in master mode.
#define I2S_TDM_FS_IN_SEL 0x004    // looks like not needed in master mode.
#define I2S_TDM_SDI_IN_SEL 0x008   //
#define I2S_TDM_SDO_OUT_SEL 0x00c
#define I2S_BCLK_OEN_SEL 0x030
#define AUDIO_PDM_CTRL 0x040
#define I2S_SYS_INT_EN 0x060
#define I2S_SYS_INTS 0x064


/* I2S/TDM register offsets */
#define I2S_BLK_MODE_SETTING 0x000
#define I2S_FRAME_SETTING 0x004
#define I2S_SLOT_SETTING1 0x008
#define I2S_SLOT_SETTING2 0x00C
#define I2S_DATA_FORMAT 0x010
#define I2S_BLK_CFG 0x014
#define I2S_I2S_ENABLE 0x018
#define I2S_I2S_RESET 0x01C
#define I2S_I2S_INT_EN 0x020
#define I2S_I2S_INT 0x024
#define I2S_FIFO_THRESHOLD 0x028
#define I2S_LRCK_MASTER 0x02C
#define I2S_FIFO_RESET 0x030
#define I2S_RX_STATUS 0x040
#define I2S_TX_STATUS 0x048
#define I2S_CLK_CTRL0 0x060
#define I2S_CLK_CTRL1 0x064
#define I2S_PCM_SYNTH 0x068
#define I2S_RX_RD_PORT 0x080
#define I2S_TX_WR_PORT 0x0C0

/// clkgen
#define clk_en_0 0x000
#define clk_en_1 0x004
#define clk_en_2 0x008
#define clk_en_4 0x010



/// mambo jumbo

/* whole ETH control block */
#define SG2002_ETH_BASE      0x03009000
#define SG2002_ETH_SIZE      0x1000

/* offsets from 0x03009000 */
#define ETH_800      0x800  /* 0x03009800 */
#define ETH_804      0x804  /* 0x03009804 */
#define ETH_808      0x808  /* 0x03009808 */

#define ETH_070      0x070  /* 0x03009070 */
#define ETH_074      0x074  /* 0x03009074 */
#define ETH_078      0x078  /* 0x03009078 */
#define ETH_07C      0x07C  /* 0x0300907C */

