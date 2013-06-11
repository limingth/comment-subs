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

static __noreturn int modinfo_jonsmodules(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modinfo";
	const char *const args[] = {
		progname,
		"/ext4-i686.ko", "/ext4-ppc64.ko", "/ext4-s390x.ko",
		"/ext4-x86_64.ko",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modinfo_jonsmodules,
	.description = "check if output for modinfo is correct for i686, ppc64, s390x and x86_64",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modinfo/",
	},
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-modinfo/correct.txt",
	});

static const struct test *tests[] = {
	&smodinfo_jonsmodules,
	NULL,
};


/**
* @topic 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
*（课题编号：2012ZX01039-004）”的资助。
*
* @group 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
*
* @author 注释添加人员： 李明

* @details 注释详细内容:
* 
*/

/*
 * 这个宏解决了所有 testsuite 里的文件所需要的 main 函数 
 * 基本的实现原理是通过宏定义，调用了如下函数
 *	- test_init()
 *	- test_find()
 *	- test_run()
 *	- exit()
 */
TESTSUITE_MAIN(tests);
