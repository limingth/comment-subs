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
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <libkmod.h>

#include "testsuite.h"


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
 * 测试 initlib 模块功能 
 * 其中用到了如下 libkmod 函数
 *	- kmod_new()
 *	- kmod_unref()
 */
static __noreturn int test_initlib(const struct test *t)
{
	struct kmod_ctx *ctx;
	const char *null_config = NULL;

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	kmod_unref(ctx);

	exit(EXIT_SUCCESS);
}
static DEFINE_TEST(test_initlib,
		.description = "test if libkmod's init function work");


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
 * 测试 insert 模块功能 
 * 其中用到了如下 libkmod 函数
 *	- kmod_new()
 *	- kmod_module_new_from_path()
 *	- kmod_module_insert_module()
 *	- kmod_unref()
 */
static __noreturn int test_insert(const struct test *t)
{
	struct kmod_ctx *ctx;
	struct kmod_module *mod;
	const char *null_config = NULL;
	int err;

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	err = kmod_module_new_from_path(ctx, "/ext4-x86_64.ko", &mod);
	if (err != 0) {
		ERR("could not create module from path: %m\n");
		exit(EXIT_FAILURE);
	}

	err = kmod_module_insert_module(mod, 0, NULL);
	if (err != 0) {
		ERR("could not insert module: %m\n");
		exit(EXIT_FAILURE);
	}
	kmod_unref(ctx);

	exit(EXIT_SUCCESS);
}
static DEFINE_TEST(test_insert,
	.description = "test if libkmod's insert_module returns ok",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-init/",
		[TC_INIT_MODULE_RETCODES] = "bla:1:20",
	},
	.need_spawn = true);


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
 * 测试 remove 模块功能 
 * 其中用到了如下 libkmod 函数
 *	- kmod_new()
 *	- kmod_module_new_from_name()
 *	- kmod_module_remove_module()
 *	- kmod_unref()
 */
static __noreturn int test_remove(const struct test *t)
{
	struct kmod_ctx *ctx;
	struct kmod_module *mod;
	const char *null_config = NULL;
	int err;

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL)
		exit(EXIT_FAILURE);

	err = kmod_module_new_from_name(ctx, "ext4", &mod);
	if (err != 0) {
		ERR("could not create module from name: %m\n");
		exit(EXIT_FAILURE);
	}

	err = kmod_module_remove_module(mod, 0);
	if (err != 0) {
		ERR("could not remove module: %m\n");
		exit(EXIT_FAILURE);
	}
	kmod_unref(ctx);

	exit(EXIT_SUCCESS);
}
static DEFINE_TEST(test_remove,
	.description = "test if libkmod's remove_module returns ok",
	.config = {
		[TC_ROOTFS] = TESTSUITE_ROOTFS "test-remove/",
		[TC_DELETE_MODULE_RETCODES] = "bla:1:20",
	},
	.need_spawn = true);

static const struct test *tests[] = {
	&stest_initlib,
	&stest_insert,
	&stest_remove,
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
