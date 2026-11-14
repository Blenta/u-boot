/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2011 Samsung Electronics
 *
 * Configuration settings for the SAMSUNG TINY4412 (EXYNOS4412) board.
 */

#ifndef __CONFIG_TINY4412_H
#define __CONFIG_TINY4412_H

#include <configs/exynos4-common.h>

/* TINY4412 has 4 bank of DRAM */
#define CFG_SYS_SDRAM_BASE		0x40000000
#define PHYS_SDRAM_1			CFG_SYS_SDRAM_BASE
#define SDRAM_BANK_SIZE			(256 << 20)	/* 256 MB */

#define CONFIG_MACH_TYPE		MACH_TYPE_TINY4412

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/* Power Down Modes */
#define S5P_CHECK_SLEEP			0x00000BAD
#define S5P_CHECK_DIDLE			0xBAD00000
#define S5P_CHECK_LPA			0xABAD0000

#define CFG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x41000000\0" \
	"sdkernelboff=0x851\0" \
	"eMMCkernelboff=0x850\0" \
	"kernelbsize=0x5000\0" \
	"fastboot_raw_partition_bl1=0x1 0x10\0" \
	"fastboot_raw_partition_bl2=0x11 0x20\0" \
	"fastboot_raw_partition_env=0x31 0x20\0" \
	"fastboot_raw_partition_bootloader=0x51 0x800\0"

#ifdef CONFIG_SPL_BUILD
#define SYS_INIT_SP_ADDR		0x02040000
#else
#define SYS_INIT_SP_ADDR		0x42E00000
#endif

#endif	/* __CONFIG_TINY4412_H */
