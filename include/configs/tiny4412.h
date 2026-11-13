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

/* select serial console configuration */

/* Console configuration */
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC0,115200n8\0"

#define CONFIG_SYS_MONITOR_BASE	0x00000000

/* Power Down Modes */
#define S5P_CHECK_SLEEP			0x00000BAD
#define S5P_CHECK_DIDLE			0xBAD00000
#define S5P_CHECK_LPA			0xABAD0000

/* MMC SPL */
#define COPY_BL2_FNPTR_ADDR	0x02020030

#define CFG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x40008000\0" \
	"rdaddr=0x48000000\0" \
	"kerneladdr=0x40008000\0" \
	"ramdiskaddr=0x48000000\0" \
	"console=ttySAC0,115200n8\0" \
	"sdkernelboff=0x691\0" \
	"eMMCkernelboff=0x0\0" \
	"kernelbsize=0x2480\0" \
	"bootargs=root=/dev/mmcblk0p1 rootfstype=ext4 console=ttySAC0,115200 init=/linuxrc ctp=2 skipcali=y ethmac=1C:6F:65:34:51:7E\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
        "loadbootscript=load mmc ${mmcdev} ${loadaddr} boot.scr\0" \
        "bootscript=echo Running bootscript from mmc${mmcdev} ...; " \
                "source ${loadaddr}\0"


#define CONFIG_CLK_1000_400_200

/* MIU (Memory Interleaving Unit) */
#define CONFIG_MIU_2BIT_21_7_INTERLEAVED

#define RESERVE_BLOCK_SIZE		(512)
#define BL1_SIZE			(8 << 10) /*8 K reserved for BL1*/
#define BL2_SIZE			(16 << 10) /*16 K reserved for BL2*/
#define TINY4412_ENV_SIZE			(16 << 10) /*16 K reserved for ENV*/

#ifdef CONFIG_SPL_BUILD
#define SYS_INIT_SP_ADDR		0x02040000
#else
#define SYS_INIT_SP_ADDR		0x42E00000
#endif

#endif	/* __CONFIG_TINY4412_H */
