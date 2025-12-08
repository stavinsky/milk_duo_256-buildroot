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

const struct regmap_config aiao_regmap_cfg = {
    .name = "tdm",
    .reg_bits = 32,
    .reg_stride = 4,
    .val_bits = 32,
};
