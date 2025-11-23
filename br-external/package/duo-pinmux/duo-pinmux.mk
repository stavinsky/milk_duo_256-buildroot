################################################################################
#
# duo-pinmux
#
################################################################################
DUO_PINMUX_VERSION = 49a1d4ae2ccc18286e8c6dd60490189e8b85a9b8
DUO_PINMUX_SITE    = $(call github,milkv-duo,duo-pinmux,$(DUO_PINMUX_VERSION))
DUO_PINMUX_INSTALL_STAGING = NO
DUO_PINMUX_INSTALL_TARGET = YES
# DUO_PINMUX_PATCH = "patch1.patch"

define DUO_PINMUX_BUILD_CMDS
	$(MAKE) -C $(@D)/duo256m \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define DUO_PINMUX_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 \
		$(@D)/duo256m/duo-pinmux \
		$(TARGET_DIR)/usr/bin/duo-pinmux
endef


$(eval $(generic-package))
