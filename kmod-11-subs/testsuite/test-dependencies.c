/*
 * Copyright (C) 2011-2012  ProFUSION embedded systems
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
#include <libkmod.h>

#include "testsuite.h"
#define TEST_UNAME "4.0.20-kmod"

/**
 * @brief 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
 *
 * @note 注释详细内容:
 * 
 * 测试模块的 kmod_module_get_dependencies 是否工作正确
 * 其中需要调用到 libkmod 模块中的以下接口
 *	- kmod_new()
 *	- kmod_module_new_from_name()
 *	- kmod_module_get_dependencies()
 *	- kmod_list_foreach()
 *	- kmod_module_get_module()
 *	- kmod_module_get_name()
 *	- kmod_module_unref_list()
 *	- kmod_module_unref()
 *	- kmod_unref()
 */
static int test_dependencies(const struct test *t)
{
	struct kmod_ctx *ctx;
	struct kmod_module *mod;
	struct kmod_list *list, *l;
	int err;
	size_t len = 0;
	int crc16 = 0, mbcache = 0, jbd2 = 0;

	ctx = kmod_new(NULL, NULL);
	if (ctx == NULL)
		return EXIT_FAILURE;

	err = kmod_module_new_from_name(ctx, "ext4", &mod);
	if (err < 0) {
		kmod_unref(ctx);
		return EXIT_FAILURE;
	}

	list = kmod_module_get_dependencies(mod);

	kmod_list_foreach(l, list) {
		struct kmod_module *m = kmod_module_get_module(l);
		const char *name = kmod_module_get_name(m);

		if (strcmp(name, "crc16") == 0)
			crc16 = 1;
		if (strcmp(name, "mbcache") == 0)
			mbcache = 1;
		else if (strcmp(name, "jbd2") == 0)
			jbd2 = 1;

		kmod_module_unref(m);
		len++;
	}

	/* crc16, mbcache, jbd2 */
	if (len != 3 || !crc16 || !mbcache || !jbd2)
		return EXIT_FAILURE;

	kmod_module_unref_list(list);
	kmod_module_unref(mod);
	kmod_unref(ctx);

	return EXIT_SUCCESS;
}
static const struct test stest_dependencies = {
	.name = "test_dependencies",
	.description = "test if kmod_module_get_dependencies works",
	.func = test_dependencies,
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-dependencies/",
		[TC_UNAME_R] = TEST_UNAME,
	},
	.need_spawn = true,
};

static const struct test *tests[] = {
	&stest_dependencies,
	NULL,
};

TESTSUITE_MAIN(tests);
