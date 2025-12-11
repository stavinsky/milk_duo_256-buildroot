EPHY_I2S2_SITE            = $(BR2_EXTERNAL_br_ext_PATH)/package/ephy-i2s2/src
EPHY_I2S2_SITE_METHOD     = local
EPHY_I2S2_LICENSE         = GPL-2.0
EPHY_I2S2_LICENSE_FILES   =
EPHY_I2S2_INSTALL_TARGET = YES

EPHY_I2S2_MODULE_SUBDIRS  = .

$(eval $(kernel-module))
$(eval $(generic-package))