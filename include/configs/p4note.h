#ifndef __P4NOTE_CONFIG_H__
#define __P4NOTE_CONFIG_H__

#define CONFIG_EXYNOS4
#define CONFIG_EXYNOS_FB

#include <configs/exynos4-common.h>
#ifndef CONFIG_SPL_BUILD
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 2) \
	func(MMC, mmc, 0)
#include <config_distro_bootcmd.h>
#else
#define BOOTENV
#endif


/* Console configuration */
#define EXYNOS_DEVICE_SETTINGS \
		"stdin=serial\0" \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

/* Cache disabled */
#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

#define CONFIG_MISC_COMMON
#define CONFIG_MISC_INIT_R
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_REVISION_TAG

/* 2GB of DRAM */
#define CONFIG_SYS_SDRAM_BASE 0x40000000
#define SDRAM_BANK_SIZE (256 << 20) /*0x40000000 * 1GB */ /* 256MB */
#define PHYS_SDRAM_1 CONFIG_SYS_SDRAM_BASE

/* Enable SPL */
#define CONFIG_EXYNOS_SPL
#define CONFIG_SPL_STACK 0x02050000
#define CONFIG_IRAM_TOP 0x02050000
#define CONFIG_SYS_INIT_SP_ADDR 0x02050000
#define CONFIG_SPL_MAX_FOOTPRINT ((14 << 10) - 4)

/*
 * SD boot: note that this is not used when booting off the eMMC.
 * 512b reserved
 * 8K BL1
 * 14K SPL, padded to 16K
 * 1024K u-boot (?)
 */
#define CONFIG_RES_BLOCK_SIZE (512)
#define CONFIG_BL1_SIZE (8 << 10) /* 8K for BL1 */
#define CONFIG_SPL_SIZE (16 << 10) /* 14K for SPL (BL2). 4 byte checksum. Padded to 16K */
#define CONFIG_BL2_SIZE (1024 << 10) /* 1024K u-boot */

#define CONFIG_BL2_OFFSET (CONFIG_RES_BLOCK_SIZE + CONFIG_BL1_SIZE + CONFIG_SPL_SIZE)

#define BL2_START_OFFSET (CONFIG_BL2_OFFSET/512)
#define BL2_SIZE_BLOC_COUNT (CONFIG_BL2_SIZE/512)

/* Boot off EMMC */
#define CONFIG_SUPPORT_EMMC_BOOT

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadbootscript=load mmc ${mmcbootdev}:${mmcbootpart} ${scriptaddr} " \
		"boot/boot.scr\0" \
	"loadkernel=load mmc ${mmcbootdev}:${mmcbootpart} ${kernel_addr_r} " \
		"${kernelname}\0" \
	"loadinitrd=load mmc ${mmcbootdev}:${mmcbootpart} ${ramdisk_addr_r} " \
		"${initrdname}\0" \
	"loaddtb=load mmc ${mmcbootdev}:${mmcbootpart} ${fdt_addr_r} " \
		"${fdtfile}\0" \
	"check_ramdisk=" \
		"if run loadinitrd; then " \
			"setenv initrd_addr ${ramdisk_addr_r};" \
		"else " \
			"setenv initrd_addr -;" \
		"fi;\0" \
	"check_dtb=" \
		"if run loaddtb; then " \
			"setenv fdt_addr ${fdt_addr_r};" \
		"else " \
			"setenv fdt_addr;" \
		"fi;\0" \
	"kernel_args=" \
		"setenv bootargs console=ttySAC2,115200n8 root=/dev/mmcblk${mmcrootdev}p${mmcrootpart}" \
		" rootwait ${console} ${opts}\0" \
	"boot_script=" \
		"run loadbootscript;" \
		"source ${scriptaddr}\0" \
	"boot_zimg=" \
		"setenv kernelname zImage;" \
		"run check_dtb;" \
		"run check_ramdisk;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootz ${kernel_addr_r} ${initrd_addr} ${fdt_addr};\0" \
	"autoboot=" \
		"if test -e mmc 1 boot.scr; then; " \
			"run boot_script; " \
		"elif test -e mmc 1 zImage; then; " \
			"run boot_zimg;" \
		"fi;\0" \
	"mmcbootdev=2\0" \
	"mmcbootpart=1\0" \
	"mmcrootdev=1\0" \
	"mmcrootpart=1\0" \
	"dfu_alt_info=Please reset the board\0" \
	"consoleon=set console console=ttySAC2,115200n8; save; reset\0" \
	"initrdname=uInitrd\0" \
    "console=console=ttySAC2,115200n8\0" \
	"ramdisk_addr_r=0x42000000\0" \
	"scriptaddr=0x42000000\0" \
	"fdt_addr_r=0x40800000\0" \
	"kernel_addr_r=0x41000000\0" \
	"fdt_high=0x49000000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	EXYNOS_DEVICE_SETTINGS \
	BOOTENV

#include <linux/sizes.h>

#define CONFIG_BAUDRATE		115200

#endif
