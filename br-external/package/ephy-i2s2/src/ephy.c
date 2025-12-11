
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
/*
0x03009804[0] = 1'b1 (rg_ephy_apb_rw_sel=1, use apb interface)
0x03009808[4:0] = 5'b00001 (rg_ephy_pll_stable_cnt[4:0] = 5'd1 (10us)
0x03009800[2] = 0x0905 (rg_ephy_dig_rst_n=1, reset release, other keep default)
wait 10us
0x0300907C[12:8]= 5'b00101 (page_sel_mode0 = page 5)
0x03009078[11:0] = 0xF00 (set to gpio from top)
0x03009074[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
0x03009070[10:9 2:1]= 0x606 (set ephy rxp&rxm input&output enable)
*/
#define REG_EPHY_TOP_WRAP 0x03009800
#define REG_EPHY_BASE 0x03009000
enum ephy_fields {
    F_PAGE_SEL_MODE0,
    F_GPIO_SOMETHING,
    F_EPHY_RXP_RXM1,
    F_EPHY_RXP_RXM2,
    F_EPHY_RXP_RXM3,
    F_EPHY_RXP_RXM4,
    F_EPHY_MAX_ID,

};
enum ephy_wrap_fields {
    F_RG_EPHY_APB_RW_SEL,
    F_RG_EPHY_PLL_STABLE_CNT,
    F_RG_EPHY_DIG_RST_N,
    F_TOP_MAX_ID,
};

static const struct reg_field ephy_fields[] = {
    [F_PAGE_SEL_MODE0] = REG_FIELD(0x7c, 8, 12),
    [F_GPIO_SOMETHING] = REG_FIELD(0x78, 0, 11),
    [F_EPHY_RXP_RXM1] = REG_FIELD(0x74, 9, 10),
    [F_EPHY_RXP_RXM2] = REG_FIELD(0x74, 1, 2),
    [F_EPHY_RXP_RXM3] = REG_FIELD(0x70, 9, 10),
    [F_EPHY_RXP_RXM4] = REG_FIELD(0x70, 1, 2),

};
static const struct reg_field ephy_wrap_fields[] = {
    [F_RG_EPHY_APB_RW_SEL] = REG_FIELD(0x4, 0, 0),
    [F_RG_EPHY_PLL_STABLE_CNT] = REG_FIELD(0x8, 0, 4),
    [F_RG_EPHY_DIG_RST_N] = REG_FIELD(0x0, 2, 2),
};

static int sg2002_ephy_init2(struct device* dev) {
    void __iomem *top_base, *base;
    struct regmap *top_map, *base_map;
    struct regmap_field* fields[F_EPHY_MAX_ID];
    struct regmap_field* fields_wrap[F_TOP_MAX_ID];
    static const struct regmap_config ephy_regmap_cfg = {
        .name = "ephy",
        .reg_bits = 32,
        .reg_stride = 4,
        .val_bits = 32,
        .max_register = 0xfff,
    };
    static const struct regmap_config ephy_regmap_wrap_cfg = {
        .name = "ephy_wrap",
        .reg_bits = 32,
        .reg_stride = 4,
        .val_bits = 32,
        .max_register = 0xfff,
    };

    /* ioremap fixed bases */
    top_base = devm_ioremap(dev, REG_EPHY_TOP_WRAP, 0x1000);
    if (!top_base) return -ENOMEM;

    base = devm_ioremap(dev, REG_EPHY_BASE, 0x1000);
    if (!base) return -ENOMEM;

    top_map = devm_regmap_init_mmio(dev, top_base, &ephy_regmap_wrap_cfg);
    if (IS_ERR(top_map)) return PTR_ERR(top_map);

    base_map = devm_regmap_init_mmio(dev, base, &ephy_regmap_cfg);
    if (IS_ERR(base_map)) return PTR_ERR(base_map);

    int i;
    for (i = 0; i < F_EPHY_MAX_ID; i++) {
        fields[i] = devm_regmap_field_alloc(dev, base_map, ephy_fields[i]);
        if (IS_ERR(fields[i])) return PTR_ERR(fields[i]);
    }
    for (i = 0; i < F_TOP_MAX_ID; i++) {
        fields_wrap[i] =
            devm_regmap_field_alloc(dev, top_map, ephy_wrap_fields[i]);
        if (IS_ERR(fields_wrap[i])) return PTR_ERR(fields_wrap[i]);
    }

    /* 0x03009804[0] = 1 (rg_ephy_apb_rw_sel=1) */
    regmap_field_write(fields_wrap[F_RG_EPHY_APB_RW_SEL], 1);
    /* 0x03009808[4:0] = 1 (10us) */
    regmap_field_write(fields_wrap[F_RG_EPHY_PLL_STABLE_CNT], 1);
    /* 0x03009800[2] = 1 (rg_ephy_dig_rst_n=1) */
    regmap_field_write(fields_wrap[F_RG_EPHY_DIG_RST_N], 1);
    udelay(10);

    /* 0x0300907C[12:8] = 5 (page_sel_mode0 = page 5) */
    regmap_field_write(fields[F_PAGE_SEL_MODE0], 5);
    /* 0x03009078[11:0] = 0xF00 (set to gpio from top) */
    regmap_field_write(fields[F_GPIO_SOMETHING], 0xF00);

    /* 0x03009074[10:9 2:1] = 0x606 -> hi = 3, lo = 3 */
    regmap_field_write(fields[F_EPHY_RXP_RXM1], 3);
    regmap_field_write(fields[F_EPHY_RXP_RXM2], 3);
    /* 0x03009070[10:9 2:1] = 0x606 -> hi = 3, lo = 3 */
    regmap_field_write(fields[F_EPHY_RXP_RXM3], 3);
    regmap_field_write(fields[F_EPHY_RXP_RXM4], 3);
    return 0;
}

static int sg2002_ephy_probe(struct platform_device* pdev) {
    int ret;

    dev_info(&pdev->dev, "SG2002 EPHY init\n");

    ret = sg2002_ephy_init2(&pdev->dev);
    if (ret) dev_err(&pdev->dev, "ephy init failed: %d\n", ret);

    return ret;
}

static void sg2002_ephy_remove(struct platform_device* pdev) {}

static const struct of_device_id sg2002_ephy_of_match[] = {
    {.compatible = "sophgo,sg2002-ephy"}, {/* sentinel */}};
MODULE_DEVICE_TABLE(of, sg2002_ephy_of_match);

static struct platform_driver sg2002_ephy_driver = {
    .probe = sg2002_ephy_probe,
    .remove = sg2002_ephy_remove,
    .driver =
        {
            .name = "sg2002-ephy",
            .of_match_table = sg2002_ephy_of_match,
        },
};
module_platform_driver(sg2002_ephy_driver);

MODULE_AUTHOR("Anton D. Stavinsky");
MODULE_DESCRIPTION("SG2002 EPHY primitive init driver");
MODULE_LICENSE("GPL");