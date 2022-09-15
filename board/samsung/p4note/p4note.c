//SPDX-License-Identifier: GPL-2.0+
/*
 * Samsung p4note board file
 * Based on midas board file from Simon Shields <simon@lineageos.org>
 *
 * Copyright (c) 2021 Walter Lütjens <lutjens.pol@gmail.com>
 */
#include <common.h>
#include <env.h>
#include <console.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/power.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/global_data.h>
#include <asm/gpio.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
#include <power/pmic.h>
#include <power/regulator.h>
#include <power/max77686_pmic.h>
#include <usb.h>
#include <usb/dwc2_udc.h>

DECLARE_GLOBAL_DATA_PTR;

int get_board_rev(void)
{
	return 0;
}

#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	int ret;

	/* we don't want the OS to think we're running under secure firmware */

	int offs = fdt_node_offset_by_compatible(blob, 0, "samsung,secure-firmware");
	if (offs < 0) {
		if (offs == -FDT_ERR_NOTFOUND) {
			printf("%s: no secure firmware node!\n", __func__);
			return 0;
		}
		printf("%s: failed to find secure firmware node: %d\n", __func__, offs);
		return -EINVAL;
	}

	/* delete the node */

	ret = fdt_del_node(blob, offs);
	if (ret < 0) {
		printf("%s: failed to remove secure firmware node: %d\n", __func__, ret);
		return -EINVAL;
	}

	printf("FDT set up for OS %p\n", blob);

	return 0;
}
#endif

int exynos_early_init_f(void)
{
	return 0;
}

static int p4note_phy_control(int on)
{
	struct udevice *dev;
	int ret;

	ret = regulator_get_by_platname("VUOTG_3.0V", &dev);
	if (ret) {
		pr_err("Regulator get error: %d\n", ret);
		return ret;
	}

	if (on)
		return regulator_set_mode(dev, OPMODE_ON);
	else
		return regulator_set_mode(dev, OPMODE_LPM);
}

struct dwc2_plat_otg_data exynos4_otg_data = {
	.phy_control = p4note_phy_control,
	.regs_phy = EXYNOS4X12_USBPHY_BASE,
	.regs_otg = EXYNOS4X12_USBOTG_BASE,
	.usb_phy_ctrl = EXYNOS4X12_USBPHY_CONTROL,
	.usb_flags = PHY0_SLEEP,
};

int board_usb_init(int index, enum usb_init_type init)
{
	pr_info("Board usb init! %d %d\n", index, init);
	return dwc2_udc_probe(&exynos4_otg_data);
}

int exynos_init(void)
{
  int ret;
  struct udevice *vddadc;

  /*
   * we have to enable ADC regulator to prepare device, otherwise
   * we'll get stmpe811 init error (also breaking up relying drivers
	 * on ADC) in Linux kernel as it initializes before PMIC driver
	 * configures regulator
	 */
  ret = regulator_get_by_platname("VDD_ADC_3.3V", &vddadc);
	if (ret) {
		pr_err("Failed to get VDD_ADC_3.3V: %d\n", ret);
		return -1;
	}

  ret = regulator_set_enable(vddadc, true);
  if (ret) {
    pr_err("Failed to set VDD_ADC_3.3V to ON: %d\n", ret);
    return -1;
  }

	return 0;
}

int exynos_late_init(void)
{
	printf("Booting...\n");

	return 0;
}
