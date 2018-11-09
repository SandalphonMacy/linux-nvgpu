/*
 * Copyright (c) 2016-2018, NVIDIA CORPORATION.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef NVGPU_CTRLCLKAVFS_H
#define NVGPU_CTRLCLKAVFS_H

/*!
 * Valid global VIN ID values
 */
#define	CTRL_CLK_VIN_ID_SYS		0x00000000U
#define	CTRL_CLK_VIN_ID_LTC		0x00000001U
#define	CTRL_CLK_VIN_ID_XBAR		0x00000002U
#define	CTRL_CLK_VIN_ID_GPC0		0x00000003U
#define	CTRL_CLK_VIN_ID_GPC1		0x00000004U
#define	CTRL_CLK_VIN_ID_GPC2		0x00000005U
#define	CTRL_CLK_VIN_ID_GPC3		0x00000006U
#define	CTRL_CLK_VIN_ID_GPC4		0x00000007U
#define	CTRL_CLK_VIN_ID_GPC5		0x00000008U
#define	CTRL_CLK_VIN_ID_GPCS		0x00000009U
#define	CTRL_CLK_VIN_ID_SRAM		0x0000000AU
#define	CTRL_CLK_VIN_ID_UNDEFINED	0x000000FFU

#define	CTRL_CLK_VIN_TYPE_DISABLED 0x00000000U
#define CTRL_CLK_VIN_TYPE_V10      0x00000001U
#define CTRL_CLK_VIN_TYPE_V20      0x00000002U

/*!
 * Various types of VIN calibration that the GPU can support
 */
#define CTRL_CLK_VIN_CAL_TYPE_V10           (0x00000000U)
#define CTRL_CLK_VIN_CAL_TYPE_V20           (0x00000001U)

/*!
 * Mask of all GPC VIN IDs supported by RM
 */
#define CTRL_CLK_LUT_NUM_ENTRIES_MAX   (128U)
#define CTRL_CLK_LUT_NUM_ENTRIES_GV10x (128U)
#define CTRL_CLK_LUT_NUM_ENTRIES_GP10x (100U)
#define CTRL_CLK_VIN_STEP_SIZE_UV (10000U)
#define CTRL_CLK_LUT_MIN_VOLTAGE_UV (450000U)
#define CTRL_CLK_FLL_TYPE_DISABLED 0U

#define CTRL_CLK_FLL_REGIME_ID_INVALID                     ((u8)0x00000000)
#define CTRL_CLK_FLL_REGIME_ID_FFR                         ((u8)0x00000001)
#define CTRL_CLK_FLL_REGIME_ID_FR                          ((u8)0x00000002)

#define CTRL_CLK_FLL_LUT_VSELECT_LOGIC  		   (0x00000000U)
#define CTRL_CLK_FLL_LUT_VSELECT_MIN  	  		   (0x00000001U)
#define CTRL_CLK_FLL_LUT_VSELECT_SRAM   		   (0x00000002U)
#endif /* NVGPU_CTRLCLKAVFS_H */
