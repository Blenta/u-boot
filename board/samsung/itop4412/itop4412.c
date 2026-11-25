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
#include <mmc.h>
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

static void itop4412_i2c1_pinmux(void)
{
	/* GPD1_2: I2C_1_SDA (func 0x2), GPD1_3: I2C_1_SCL (func 0x2) */
	gpio_cfg_pin(EXYNOS4X12_GPIO_D12, S5P_GPIO_FUNC(0x2));
	gpio_cfg_pin(EXYNOS4X12_GPIO_D13, S5P_GPIO_FUNC(0x2));
	gpio_set_pull(EXYNOS4X12_GPIO_D12, S5P_GPIO_PULL_NONE);
	gpio_set_pull(EXYNOS4X12_GPIO_D13, S5P_GPIO_PULL_NONE);
}

int exynos_init(void)
{
	itop4412_i2c1_pinmux();

	usb_init();

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
	// usb_stop();
	// usb_init();

	if (env_get_ulong("fastboot_mmc_dev", 10, 0) == 2) {// MMC2: SD Card
		env_set("fastboot_raw_partition_bl1", "0x1 0x10"); // Reserved: 1 block, BL1: 8K
		env_set("fastboot_raw_partition_bl2", "0x11 0x20");// BL2: 16K
		env_set("fastboot_raw_partition_bootloader", "0x31 0x800");// Bootloader: 1024K
		env_set("fastboot_partition_alias_system", "2:2");
		env_set("fastboot_partition_alias_userdata", "2:3");
		env_set("fastboot_partition_alias_cache", "2:4");
	} else {// MMC0: eMMC
		env_set("fastboot_raw_partition_bl1", "0x0 0x10 mmcpart 1");
		env_set("fastboot_raw_partition_bl2", "0x10 0x20 mmcpart 1");
		env_set("fastboot_raw_partition_bootloader", "0x30 0x800 mmcpart 1");
		env_set("fastboot_partition_alias_system", "0.0:2");
		env_set("fastboot_partition_alias_userdata", "0.0:3");
		env_set("fastboot_partition_alias_cache", "0.0:4");
	}

	/* CLK_DIV_FSYS3 = 0x00000004 */
	writel(0x00000004, 0x1003c54c);

	/* Prevent Reset*/
	writel(readl(0x11000c08) & ~0x30, 0x11000c08);

	return 0;
}
#endif

void arch_preboot_os(void)
{
	usb_stop();
	/* Isolate USB Host PHY so Linux 3.0.15 runs full usb_host_phy_init() */
	writel(readl(0x10020708) & ~0x1, 0x10020708);
}
