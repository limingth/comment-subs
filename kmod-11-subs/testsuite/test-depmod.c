/*
 * Copyright (C) 2012  ProFUSION embedded systems
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

#include "testsuite.h"

#define MODULES_ORDER_UNAME "3.5.4-1-ARCH"
#define MODULES_ORDER_ROOTFS TESTSUITE_ROOTFS "test-depmod/modules-order-compressed"
#define MODULES_ORDER_LIB_MODULES MODULES_ORDER_ROOTFS "/lib/modules/" MODULES_ORDER_UNAME
static __noreturn int depmod_modules_order_for_compressed(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/depmod";
	const char *const args[] = {
		progname,
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}

/**                                                                
 * @attention 本注释得到了"核高基"科技重大专项2012年课题
 *             “开源操作系统内核分析和安全性评估
 *            （课题编号：2012ZX01039-004）”的资助。
 *                                                                      
 * @copyright 注释添加单位：清华大学——03任务
 *            （Linux内核相关通用基础软件包分析）
 *                                                                        
 * @author 注释添加人员： 李明
 *             (电子邮件 <limingth@gmail.com>)
 *                                                                    
 * @date 注释添加日期： 2013-6-1
 *                                                                   
 * @note 注释详细内容:                                                
 *
 * @brief  配合 depmod_modules_order_for_compressed 测试函数的配置结构体
 */
static DEFINE_TEST(depmod_modules_order_for_compressed,
	.description = "check if depmod let aliases in right order when using compressed modules",
	.config = {
		[TC_UNAME_R] = MODULES_ORDER_UNAME,
		[TC_ROOTFS] = MODULES_ORDER_ROOTFS,
	},
	.output = {
		.files = (const struct keyval[]) {
			{ MODULES_ORDER_LIB_MODULES "/correct-modules.alias",
			  MODULES_ORDER_LIB_MODULES "/modules.alias" },
			{ }
		},
	});


static const struct test *tests[] = {
	&sdepmod_modules_order_for_compressed,
	NULL,
};

/**                                                                
 * @attention 本注释得到了"核高基"科技重大专项2012年课题
 *             “开源操作系统内核分析和安全性评估
 *            （课题编号：2012ZX01039-004）”的资助。
 *                                                                      
 * @copyright 注释添加单位：清华大学——03任务
 *            （Linux内核相关通用基础软件包分析）
 *                                                                        
 * @author 注释添加人员： 李明
 *             (电子邮件 <limingth@gmail.com>)
 *                                                                    
 * @date 注释添加日期： 2013-6-1
 *                                                                   
 * @note 注释详细内容:                                                
 *
 * @brief  这个宏解决了所有 testsuite 里的文件所需要的 main 函数 
 * 基本的实现原理是通过宏定义，调用了如下函数
 *	- test_init()
 *	- test_find()
 *	- test_run()
 *	- exit()
 */
TESTSUITE_MAIN(tests);
