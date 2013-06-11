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

static __noreturn int modprobe_show_depends(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modprobe";
	const char *const args[] = {
		progname,
		"--show-depends", "btusb",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modprobe_show_depends,
	.description = "check if output for modprobe --show-depends is correct for loaded modules",
	.config = {
		[TC_UNAME_R] = "4.4.4",
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modprobe/show-depends",
	},
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-modprobe/show-depends/correct.txt",
	});

static __noreturn int modprobe_show_depends2(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modprobe";
	const char *const args[] = {
		progname,
		"--show-depends", "psmouse",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modprobe_show_depends2,
	.description = "check if output for modprobe --show-depends is correct",
	.config = {
		[TC_UNAME_R] = "4.4.4",
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modprobe/show-depends",
	},
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-modprobe/show-depends/correct-psmouse.txt",
	});

static __noreturn int modprobe_builtin(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modprobe";
	const char *const args[] = {
		progname,
		"unix",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modprobe_builtin,
	.description = "check if modprobe return 0 for builtin",
	.config = {
		[TC_UNAME_R] = "4.4.4",
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modprobe/builtin",
	});

static __noreturn int modprobe_softdep_loop(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modprobe";
	const char *const args[] = {
		progname,
		"bluetooth",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modprobe_softdep_loop,
	.description = "check if modprobe breaks softdep loop",
	.config = {
		[TC_UNAME_R] = "4.4.4",
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modprobe/softdep-loop",
		[TC_INIT_MODULE_RETCODES] = "",
	});

static __noreturn int modprobe_install_cmd_loop(const struct test *t)
{
	const char *progname = ABS_TOP_BUILDDIR "/tools/modprobe";
	const char *const args[] = {
		progname,
		"snd-pcm",
		NULL,
	};

	test_spawn_prog(progname, args);
	exit(EXIT_FAILURE);
}
static DEFINE_TEST(modprobe_install_cmd_loop,
	.description = "check if modprobe breaks install-commands loop",
	.config = {
		[TC_UNAME_R] = "4.4.4",
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-modprobe/install-cmd-loop",
		[TC_INIT_MODULE_RETCODES] = "",
	},
	.env_vars = (const struct keyval[]) {
		{ "MODPROBE", ABS_TOP_BUILDDIR "/tools/modprobe" },
		{ }
		},
	);

static const struct test *tests[] = {
	&smodprobe_show_depends,
	&smodprobe_show_depends2,
	&smodprobe_builtin,
	&smodprobe_softdep_loop,
	&smodprobe_install_cmd_loop,
	NULL,
};


/**
 * @topic 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。
 *
 * @group 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 *
 * @author 注释添加人员： 李明
 *
 * @details 注释详细内容:
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
