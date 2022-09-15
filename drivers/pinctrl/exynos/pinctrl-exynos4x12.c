// SPDX-License-Identifier: GPL-2.0+
/*
 * Exynos4x12 pinctrl driver.
 *
 * Copyright (c) 2020 Dzmitry Sankouski (dsankouski@gmail.com)
 *
 * based on drivers/pinctrl/samsung/pinctrl-exynos-arm.c :
 * Copyright (C) 2016 Samsung Electronics
 * Thomas Abraham <thomas.ab@samsung.com>
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <dm/pinctrl.h>
#include <dm/root.h>
#include <fdtdec.h>
#include <asm/arch/pinmux.h>
#include "pinctrl-exynos.h"

static const struct pinctrl_ops exynos4x12_pinctrl_ops = {
	.set_state	= exynos_pinctrl_set_state
};

/* pin banks of exynos4x12 pin-controller 0 */
static const struct samsung_pin_bank_data exynos4x12_pin_banks0[] = {
	EXYNOS_PIN_BANK(6, 0x000, "etc0"),
	EXYNOS_PIN_BANK(3, 0x020, "etc1"),
	EXYNOS_PIN_BANK(8, 0x040, "gpa0"),
	EXYNOS_PIN_BANK(8, 0x060, "gpa1"),
	EXYNOS_PIN_BANK(8, 0x080, "gpa2"),
	EXYNOS_PIN_BANK(5, 0x0a0, "gpa3"),
	EXYNOS_PIN_BANK(2, 0x0c0, "gpq0"),
	
	EXYNOS_PIN_BANK(8, 0x000, "gpa0"),
	EXYNOS_PIN_BANK(6, 0x020, "gpa1"),
	EXYNOS_PIN_BANK(8, 0x040, "gpb"),
	EXYNOS_PIN_BANK(5, 0x060, "gpc0"),
	EXYNOS_PIN_BANK(5, 0x080, "gpc1"),
	EXYNOS_PIN_BANK(4, 0x0A0, "gpd0"),
	EXYNOS_PIN_BANK(4, 0x0C0, "gpd1"),
	EXYNOS_PIN_BANK(8, 0x180, "gpf0"),
	EXYNOS_PIN_BANK(8, 0x1A0, "gpf1"),
	EXYNOS_PIN_BANK(8, 0x1C0, "gpf2"),
	EXYNOS_PIN_BANK(6, 0x1E0, "gpf3"),
	EXYNOS_PIN_BANK(8, 0x240, "gpj0"),
	EXYNOS_PIN_BANK(5, 0x260, "gpj1"),
};

/* pin banks of exynos4x12 pin-controller 1 */
static const struct samsung_pin_bank_data exynos4x12_pin_banks1[] = {
	EXYNOS_PIN_BANK(2, 0x000, "gpm0"),
	
	EXYNOS_PIN_BANK(7, 0x040, "gpk0"),
	EXYNOS_PIN_BANK(7, 0x060, "gpk1"),
	EXYNOS_PIN_BANK(7, 0x080, "gpk2"),
	EXYNOS_PIN_BANK(7, 0x0A0, "gpk3"),
	EXYNOS_PIN_BANK(7, 0x0C0, "gpl0"),
	EXYNOS_PIN_BANK(2, 0x0E0, "gpl1"),
	EXYNOS_PIN_BANK(8, 0x100, "gpl2"),
	EXYNOS_PIN_BANK(8, 0x260, "gpm0"),
	EXYNOS_PIN_BANK(7, 0x280, "gpm1"),
	EXYNOS_PIN_BANK(5, 0x2A0, "gpm2"),
	EXYNOS_PIN_BANK(8, 0x2C0, "gpm3"),
	EXYNOS_PIN_BANK(8, 0x2E0, "gpm4"),
	EXYNOS_PIN_BANK(6, 0x120, "gpy0"),
	EXYNOS_PIN_BANK(4, 0x140, "gpy1"),
	EXYNOS_PIN_BANK(6, 0x160, "gpy2"),
	EXYNOS_PIN_BANK(8, 0x180, "gpy3"),
	EXYNOS_PIN_BANK(8, 0x1A0, "gpy4"),
	EXYNOS_PIN_BANK(8, 0x1C0, "gpy5"),
	EXYNOS_PIN_BANK(8, 0x1E0, "gpy6"),
	EXYNOS_PIN_BANK(8, 0xC00, "gpx0"),
	EXYNOS_PIN_BANK(8, 0xC20, "gpx1"),
	EXYNOS_PIN_BANK(8, 0xC40, "gpx2"),
	EXYNOS_PIN_BANK(8, 0xC60, "gpx3"),
};

/* pin banks of exynos4x12 pin-controller 2 */
static const struct samsung_pin_bank_data exynos4x12_pin_banks2[] = {
	EXYNOS_PIN_BANK(7, 0x000, "gpz"),
};

/* pin banks of exynos4x12 pin-controller 3 */
static const struct samsung_pin_bank_data exynos4x12_pin_banks3[] = {
	EXYNOS_PIN_BANK(8, 0x000, "gpv0"),
	EXYNOS_PIN_BANK(8, 0x020, "gpv1"),
	EXYNOS_PIN_BANK(8, 0x040, "gpv2"),
	EXYNOS_PIN_BANK(8, 0x060, "gpv3"),
	EXYNOS_PIN_BANK(2, 0x080, "gpv4"),
};

const struct samsung_pin_ctrl exynos4x12_pin_ctrl[] = {
	{
		/* pin-controller instance 0 */
		.pin_banks	= exynos4x12_pin_banks0,
		.nr_banks	= ARRAY_SIZE(exynos4x12_pin_banks0),
	}, {
		/* pin-controller instance 1 */
		.pin_banks	= exynos4x12_pin_banks1,
		.nr_banks	= ARRAY_SIZE(exynos4x12_pin_banks1),
	}, {
		/* pin-controller instance 2 */
		.pin_banks	= exynos4x12_pin_banks2,
		.nr_banks	= ARRAY_SIZE(exynos4x12_pin_banks2),
	}, {
		/* pin-controller instance 3 */
		.pin_banks	= exynos4x12_pin_banks3,
		.nr_banks	= ARRAY_SIZE(exynos4x12_pin_banks3),
	},
	{/* list terminator */}
};

static const struct udevice_id exynos4x12_pinctrl_ids[] = {
	{ .compatible = "samsung,exynos4x12-pinctrl",
		.data = (ulong)exynos4x12_pin_ctrl },
	{ }
};

U_BOOT_DRIVER(pinctrl_exynos4x12) = {
	.name		= "pinctrl_exynos4x12",
	.id		= UCLASS_PINCTRL,
	.of_match	= exynos4x12_pinctrl_ids,
	.priv_auto = sizeof(struct exynos_pinctrl_priv),
	.ops		= &exynos4x12_pinctrl_ops,
	.probe		= exynos_pinctrl_probe,
};
