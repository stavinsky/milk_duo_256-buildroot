#include <linux/bitfield.h>
#include <linux/bits.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/regmap.h>

/// mambo jumbo

/* whole ETH control block */
#define SG2002_ETH_BASE 0x03009000
#define SG2002_ETH_SIZE 0x1000

/* offsets from 0x03009000 */
#define ETH_800 0x800 /* 0x03009800 */
#define ETH_804 0x804 /* 0x03009804 */
#define ETH_808 0x808 /* 0x03009808 */

#define ETH_070 0x070 /* 0x03009070 */
#define ETH_074 0x074 /* 0x03009074 */
#define ETH_078 0x078 /* 0x03009078 */
#define ETH_07C 0x07C /* 0x0300907C */

const struct regmap_config aiao_regmap_cfg = {
    .name = "tdm",
    .reg_bits = 32,
    .reg_stride = 4,
    .val_bits = 32,
};
// struct aiao_regmap_variant {
//     struct reg_field i2s_tdm_0_sdi_in_sel;
//     struct reg_field i2s_tdm_1_sdi_in_sel;
//     struct reg_field i2s_tdm_2_sdi_in_sel;
//     struct reg_field i2s_tdm_3_sdi_in_sel;

//     struct reg_field i2s_tdm_0_sdo_out_sel;
//     struct reg_field i2s_tdm_1_sdo_out_sel;
//     struct reg_field i2s_tdm_2_sdo_out_sel;
//     struct reg_field i2s_tdm_3_sdo_out_sel;

//     struct reg_field audio_pdm_sel_i2s1;
// };
// struct aiao_regmap_variant aiao_layout = {

// };

/* I2S/TDM register offsets */
