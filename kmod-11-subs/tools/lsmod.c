/*
 * kmod-lsmod - list modules from linux kernel using libkmod.
 *
 * Copyright (C) 2011-2012  ProFUSION embedded systems
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "libkmod.h"

#include "kmod.h"

static int do_lsmod(int argc, char *argv[])
{
	struct kmod_ctx *ctx;
	const char *null_config = NULL;
	struct kmod_list *list, *itr;
	int err;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s\n", argv[0]);
		return EXIT_FAILURE;
	}

	ctx = kmod_new(NULL, &null_config);
	if (ctx == NULL) {
		fputs("Error: kmod_new() failed!\n", stderr);
		return EXIT_FAILURE;
	}

	err = kmod_module_new_from_loaded(ctx, &list);
	if (err < 0) {
		fprintf(stderr, "Error: could not get list of modules: %s\n",
			strerror(-err));
		kmod_unref(ctx);
		return EXIT_FAILURE;
	}

	puts("Module                  Size  Used by");

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

const struct kmod_cmd kmod_cmd_compat_lsmod = {
	.name = "lsmod",
	.cmd = do_lsmod,
	.help = "compat lsmod command",
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
 * 用于实现该命令的结构体, 在 kmod.c 中有关于这个结构体的指针数组
 * 通过用户输入命令的字符串和结构体的 name 成员相比较，确定是哪个命令
 * 这个结构体中还有一个 cmd 成员，是一个函数指针，
 * 通过这个函数指针，可以调用到真正的命令实现函数 do_xxx
 */
const struct kmod_cmd kmod_cmd_list = {
	.name = "list",
	.cmd = do_lsmod,
	.help = "list currently loaded modules",
};
