// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2011 Samsung Electronics
 */

#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/cpu.h>
#include <asm/arch/mmc.h>
#include <asm/arch/periph.h>
#include <asm/arch/pinmux.h>
#include <env.h>
#include <usb.h>
#include <usb/dwc2_udc.h>

u32 get_board_rev(void)
{
	return 0;
}

int exynos_power_init(void)
{
	return 0;
}

int exynos_init(void)
{
#ifdef CONFIG_USB_ETHER_DM9601
	debug("---> ready to init usb3503\n");
	/* USB3503A Connect */
	gpio_request(EXYNOS4X12_GPIO_M33, "USB3503A Connect");
	/* USB3503A Reset */
	gpio_request(EXYNOS4X12_GPIO_M24, "USB3503A Reset");
	/* USB3503A Disconnect, Reset, Connect */
	gpio_direction_output(EXYNOS4X12_GPIO_M33, 0);
	gpio_direction_output(EXYNOS4X12_GPIO_M24, 0);
	gpio_direction_output(EXYNOS4X12_GPIO_M24, 1);
	gpio_direction_output(EXYNOS4X12_GPIO_M33, 1);
#endif

	/* PS-Hold high */
	writel(readl(0x1002330c) | 0x300, 0x1002330c);
	writel(0x0, 0x11000c08);

	return 0;
}

#ifdef CONFIG_USB_GADGET
static int exynos4412_phy_control(int on)
{
	return 0;
}

struct dwc2_plat_otg_data exynos4412_otg_data = {
	.phy_control	= exynos4412_phy_control,
	.regs_phy	= EXYNOS4X12_USBPHY_BASE,
	.regs_otg	= EXYNOS4X12_USBOTG_BASE,
	.usb_phy_ctrl	= EXYNOS4X12_USBPHY_CONTROL,
	.usb_flags	= PHY0_SLEEP,
};
#endif

int board_usb_init(int index, enum usb_init_type init)
{
#ifdef CONFIG_USB_GADGET
	debug("USB_udc_probe\n");
	return dwc2_udc_probe(&exynos4412_otg_data);
#else
	return 0;
#endif
}

#ifdef CONFIG_BOARD_EARLY_INIT_F
int exynos_early_init_f(void)
{
	return 0;
}
#endif

#if defined(CONFIG_BOARD_LATE_INIT)
int exynos_late_init(void)
{
	if (env_get_ulong("fastboot_mmc_dev", 10, 0) == 2) {// MMC2: SD Card
		env_set("fastboot_raw_partition_bl1", "0x1 0x10"); // Reserved: 1 block, BL1: 8K
		env_set("fastboot_raw_partition_bl2", "0x11 0x20");// BL2: 16K
		env_set("fastboot_raw_partition_bootloader", "0x31 0x800");// Bootloader: 1024K
	} else {// MMC0: eMMC
		env_set("fastboot_raw_partition_bl1", "0x0 0x10 mmcpart 1");
		env_set("fastboot_raw_partition_bl2", "0x10 0x20 mmcpart 1");
		env_set("fastboot_raw_partition_bootloader", "0x30 0x800 mmcpart 1");
	}
	return 0;
}
#endif
