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

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 */

/*
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
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
 * @brief 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
 *
 * @note 注释详细内容:
 *
 * 通过名字新建一个模块
 * 成功则返回 EXIT_SUCCESS
 * 失败则返回 EXIT_FAILURE
 */
static int from_name(const struct test *t)
{
	static const char *modnames[] = {
		"ext4",
		"balbalbalbbalbalbalbalbalbalbal",
		"snd-hda-intel",
		"snd-timer",
		"iTCO_wdt",
		NULL,
	};
	const char **p;
	struct kmod_ctx *ctx;
	struct kmod_module *mod;
	const char *null_config = NULL;
	int err;

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	for (p = modnames; p != NULL; p++) {
		err = kmod_module_new_from_name(ctx, *p, &mod);
		if (err < 0)
			exit(EXIT_SUCCESS);

		printf("modname: %s\n", kmod_module_get_name(mod));
		kmod_module_unref(mod);
	}

	kmod_unref(ctx);

	return EXIT_SUCCESS;
}
static DEFINE_TEST(from_name,
	.description = "check if module names are parsed correctly",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-new-module/from_name/",
	},
	.need_spawn = true,
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-new-module/from_name/correct.txt",
	});

/**
 * @brief 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
 *
 * @note 注释详细内容:
 *
 * 通过alias新建一个模块
 * 成功则返回 EXIT_SUCCESS
 * 失败则返回 EXIT_FAILURE
 */
static int from_alias(const struct test *t)
{
	static const char *modnames[] = {
		"ext4.*",
		NULL,
	};
	const char **p;
	struct kmod_ctx *ctx;
	int err;

	ctx = kmod_new(NULL, NULL);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	for (p = modnames; p != NULL; p++) {
		struct kmod_list *l, *list = NULL;

		err = kmod_module_new_from_lookup(ctx, *p, &list);
		if (err < 0)
			exit(EXIT_SUCCESS);

		kmod_list_foreach(l, list) {
			struct kmod_module *m;
			m = kmod_module_get_module(l);

			printf("modname: %s\n", kmod_module_get_name(m));
			kmod_module_unref(m);
		}
		kmod_module_unref_list(list);
	}

	kmod_unref(ctx);

	return EXIT_SUCCESS;
}
static DEFINE_TEST(from_alias,
	.description = "check if aliases are parsed correctly",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-new-module/from_alias/",
	},
	.need_spawn = true,
	.output = {
		.stdout = TESTSUITE_ROOTFS "test-new-module/from_alias/correct.txt",
	});

static const struct test *tests[] = {
	&sfrom_name,
	&sfrom_alias,
	NULL,
};

/**
 * @brief 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。

 * @copyright 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 * @author 注释添加人员： 李明
 * @date 2013-6-1
 *
 * @note 注释详细内容:
 *
 * 这个宏解决了所有 testsuite 里的文件所需要的 main 函数 
 * 基本的实现原理是通过宏定义，调用了如下函数
 *	- test_init()
 *	- test_find()
 *	- test_run()
 *	- exit()
 */
TESTSUITE_MAIN(tests);
