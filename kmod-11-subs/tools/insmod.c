/*
 * kmod-insmod - insert modules into linux kernel using libkmod.
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
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include "libkmod.h"

#include "kmod.h"

static const char cmdopts_s[] = "psfVh";
static const struct option cmdopts[] = {
	{"version", no_argument, 0, 'V'},
	{"help", no_argument, 0, 'h'},
	{NULL, 0, 0, 0}
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
 * @brief  @brief 负责打印该命令的帮助提示信息，通过 -h 参数可以显示
 */
static void help(void)
{
	printf("Usage:\n"
		"\t%s [options] filename [args]\n"
		"Options:\n"
		"\t-V, --version     show version\n"
		"\t-h, --help        show this help\n",
		program_invocation_short_name);
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
 * @brief  根据传入 err 的值，打印出错信息
 */
static const char *mod_strerror(int err)
{
	switch (err) {
	case ENOEXEC:
		return "Invalid module format";
	case ENOENT:
		return "Unknown symbol in module";
	case ESRCH:
		return "Module has wrong symbol version";
	case EINVAL:
		return "Invalid parameters";
	default:
		return strerror(err);
	}
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
 * @brief  insmod 函数的真正实现，通过 getopt_long 分析传入参数，
 * 
 * 通过调用 libkmod 的接口，实现 insmod 命令
 * do_insmod() 的实现可以分为5个步骤
 *	创建模块的上下文 struct kmod_ctx ctx
 *	通过 filename 和 ctx 获得模块 struct kmod_module mod
 *	将 mod 插入到当前模块列表中, 通过 kmod_module_insert_module 完成真正的插入内核功能
 *	释放 mod 
 *	释放 ctx
 *
 * 涉及到两个模块的5个接口，两个模块是
 * libkmod/libkmod.c
 *	 - kmod_new() 
 *	 - kmod_unref()
 * libkmod/libkmod-module.c
 *	 - kmod_module_new_from_path()
 *	 - kmod_module_insert_module()
 *	 - kmod_module_unref()
 */ 
static int do_insmod(int argc, char *argv[])
{
	struct kmod_ctx *ctx;
	struct kmod_module *mod;
	const char *filename;
	char *opts = NULL;
	size_t optslen = 0;
	int i, err;
	const char *null_config = NULL;

	printf("begin to parse para\n");
	for (;;) {
		int c, idx = 0;
		c = getopt_long(argc, argv, cmdopts_s, cmdopts, &idx);
		printf("c = %d\n", c);
		if (c == -1)
			break;
		switch (c) {
		case 'p':
		case 's':
		case 'f':
			/* ignored, for compatibility only */
			break;
		case 'h':
			help();
			return EXIT_SUCCESS;
		case 'V':
			puts(PACKAGE " version " VERSION);
			return EXIT_SUCCESS;
		case '?':
			return EXIT_FAILURE;
		default:
			printf("no this para\n");
			ERR("unexpected getopt_long() value '%c'.\n",
				c);
			return EXIT_FAILURE;
		}
	}

	if (optind >= argc) {
		ERR("missing filename.\n");
		return EXIT_FAILURE;
	}

	filename = argv[optind];
	if (strcmp(filename, "-") == 0) {
		ERR("this tool does not support loading from stdin!\n");
		return EXIT_FAILURE;
	}

	for (i = optind + 1; i < argc; i++) {
		size_t len = strlen(argv[i]);
		void *tmp = realloc(opts, optslen + len + 2);
		if (tmp == NULL) {
			ERR("out of memory\n");
			free(opts);
			return EXIT_FAILURE;
		}
		opts = tmp;
		if (optslen > 0) {
			opts[optslen] = ' ';
			optslen++;
		}
		memcpy(opts + optslen, argv[i], len);
		optslen += len;
		opts[optslen] = '\0';
	}

	ctx = kmod_new(NULL, &null_config);
	if (!ctx) {
		ERR("kmod_new() failed!\n");
		free(opts);
		return EXIT_FAILURE;
	}

	err = kmod_module_new_from_path(ctx, filename, &mod);
	if (err < 0) {
		ERR("could not load module %s: %s\n", filename,
		    strerror(-err));
		goto end;
	}

	err = kmod_module_insert_module(mod, 0, opts);
	if (err < 0) {
		ERR("could not insert module %s: %s\n", filename,
		    mod_strerror(-err));
	}
	kmod_module_unref(mod);

end:
	kmod_unref(ctx);
	free(opts);
	return err >= 0 ? EXIT_SUCCESS : EXIT_FAILURE;
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
 * @brief  用于实现该命令的结构体, 在 kmod.c 中有关于这个结构体的指针数组
 * 通过用户输入命令的字符串和结构体的 name 成员相比较，确定是哪个命令
 * 这个结构体中还有一个 cmd 成员，是一个函数指针，
 * 通过这个函数指针，可以调用到真正的命令实现函数 do_xxx
 */
const struct kmod_cmd kmod_cmd_compat_insmod = {
	.name = "insmod",
	.cmd = do_insmod,
	.help = "compat insmod command",
};
