#
# Copyright (C) 2010 Samsung Electronics Co., Ltd.
#              http://www.samsung.com/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
####################################
if [ -z $1 ]
then
    echo "usage: ./create_4412_sd_image.sh <target board> <output file>"
    exit 0
fi

block_size=512
#sizes are multiplied by block_size
#512B empty header
reserved_size=1
#8kB
fwbl1_size=16
#16kB
bl2_size=32
#1024kB
uboot_size=2048
#160kB
tzsw_size=320
#16kB
env_size=32

reserved_position=0
fwbl1_pos=$(($reserved_position + $reserved_size))
bl2_pos=$(($fwbl1_pos + $fwbl1_size))
uboot_pos=$(($bl2_pos + $bl2_size))
tzsw_pos=$(($uboot_pos + $uboot_size))
env_pos=$(($tzsw_pos + $tzsw_size))
#env_pos=$(($uboot_pos + $uboot_size))

####################################
echo "Exynos4412 FWBL1 fusing"
dd if=./p4412_s_fwbl1.bin of=$2 bs=$block_size seek=$fwbl1_pos
####################################
echo "Exynos4412 BL2 fusing"
dd if=spl/$1-spl.bin of=$2 bs=$block_size seek=$bl2_pos
####################################
echo "Exynos4412 bootloader fusing"
dd if=./u-boot.bin of=$2 bs=$block_size seek=$uboot_pos
####################################
echo "Exynos4412 tzsw fusing"
#dd if=./gcam_tzsw.bin of=$1 bs=$block_size seek=$tzsw_pos
####################################
echo "Filling env with zeroes.."
dd if=/dev/zero of=$2 bs=$block_size count=$env_size seek=$env_pos
####################################

sync
#<Message Display>
echo "$2 - Exynos4412 U-Boot SD card image is ready."
