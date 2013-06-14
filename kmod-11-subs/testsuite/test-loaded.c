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
#include <libkmod.h>

#include "testsuite.h"

/**
 * @brief  本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
 *
 * @note 注释详细内容:
 * 
 * @brief  测试 list 模块功能 
 * 其中用到了如下 libkmod 函数
 *	- kmod_new()
 *	- kmod_module_new_from_loaded()
 *	- kmod_list_foreach()
 *	- kmod_module_get_module()
 *	- kmod_module_get_name()
 *	- kmod_module_get_refcnt()
 *	- kmod_module_get_size()
 *	- kmod_module_get_holders()
 *	- kmod_module_unref_list()
 *	- kmod_module_unref()
 *	- kmod_unref()
 */
static int loaded_1(const struct test *t)
{
	struct kmod_ctx *ctx;
	const char *null_config = NULL;
	struct kmod_list *list, *itr;
	int err;

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	err = kmod_module_new_from_loaded(ctx, &list);
	if (err < 0) {
		fprintf(stderr, "%s\n", strerror(-err));
		kmod_unref(ctx);
		exit(EXIT_FAILURE);
	}

	printf("Module                  Size  Used by\n");

	kmod_list_foreach(itr, list) {
		struct kmod_module *mod = kmod_module_get_module(itr);
		const char *name = kmod_module_get_name(mod);
		int use_count = kmod_module_get_refcnt(mod);
		long size = kmod_module_get_size(mod);
		struct kmod_list *holders, *hitr;
		int first = 1;

		printf("%-19s %8ld  %d ", name, size, use_count);
		holders = kmod_module_get_holders(mod);
		kmod_list_foreach(hitr, holders) {
			struct kmod_module *hm = kmod_module_get_module(hitr);

			if (!first)
				putchar(',');
			else
				first = 0;

			fputs(kmod_module_get_name(hm), stdout);
			kmod_module_unref(hm);
		}
		putchar('\n');
		kmod_module_unref_list(holders);
		kmod_module_unref(mod);
	}
	kmod_module_unref_list(list);

	kmod_unref(ctx);

	return EXIT_SUCCESS;
}
static DEFINE_TEST(loaded_1,
	.description = "check if list of module is created",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-loaded/",
	},
	.need_spawn = true,
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-loaded/correct.txt",
	});

static const struct test *tests[] = {
	&sloaded_1,
	NULL,
};

/**
 * @brief  本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
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
