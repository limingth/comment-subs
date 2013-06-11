/*
 * kmod-rmmod - remove modules from linux kernel using libkmod.
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libkmod.h"
#include "macro.h"

#include "kmod.h"

#define DEFAULT_VERBOSE LOG_ERR
static int verbose = DEFAULT_VERBOSE;
static int use_syslog;

static const char cmdopts_s[] = "fsvVwh";
static const struct option cmdopts[] = {
	{"force", no_argument, 0, 'f'},
	{"syslog", no_argument, 0, 's'},
	{"verbose", no_argument, 0, 'v'},
	{"version", no_argument, 0, 'V'},
	{"wait", no_argument, 0, 'w'},
	{"help", no_argument, 0, 'h'},
	{NULL, 0, 0, 0}
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
/* 负责打印该命令的帮助提示信息，通过 -h 参数可以显示 */
static void help(void)
{
	printf("Usage:\n"
		"\t%s [options] modulename ...\n"
		"Options:\n"
		"\t-f, --force       forces a module unload and may crash your\n"
		"\t                  machine. This requires Forced Module Removal\n"
		"\t                  option in your kernel. DANGEROUS\n"
		"\t-s, --syslog      print to syslog, not stderr\n"
		"\t-v, --verbose     enables more messages\n"
		"\t-V, --version     show version\n"
		"\t-h, --help        show this help\n",
		program_invocation_short_name);
}


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
 * 检查模块是否正在使用，通过引用计数来帮助判断是否真正需要卸载模块 
 * 主要用到的函数接口
 * - kmod_module_get_initstate()
 * - kmod_module_get_name()
 * - kmod_module_get_holders()
 * - kmod_list_foreach()
 * - kmod_module_get_module()
 * - kmod_module_unref()
 * - kmod_module_unref_list()
 * - kmod_module_get_refcnt()
 * 其中 最后通过 kmod_module_get_refcnt() 函数可以得到模块当前的引用计数
 */

static int check_module_inuse(struct kmod_module *mod) {
	struct kmod_list *holders;

	if (kmod_module_get_initstate(mod) == -ENOENT) {
		ERR("Module %s is not currently loaded\n",
				kmod_module_get_name(mod));
		return -ENOENT;
	}

	holders = kmod_module_get_holders(mod);
	if (holders != NULL) {
		struct kmod_list *itr;

		ERR("Module %s is in use by:", kmod_module_get_name(mod));

		kmod_list_foreach(itr, holders) {
			struct kmod_module *hm = kmod_module_get_module(itr);
			fprintf(stderr, " %s", kmod_module_get_name(hm));
			kmod_module_unref(hm);
		}
		fputc('\n', stderr);

		kmod_module_unref_list(holders);
		return -EBUSY;
	}

	if (kmod_module_get_refcnt(mod) != 0) {
		ERR("Module %s is in use\n", kmod_module_get_name(mod));
		return -EBUSY;
	}

	return 0;
}


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
 * rmmod 函数的真正实现，通过 getopt_long 分析传入参数，
 * 通过调用 libkmod 的接口，实现 rmmod 命令
 * 主要使用的接口包括
 * - log_open()
 * - kmod_new()
 * - log_setup_kmod_log()
 * - kmod_module_new_from_path()
 * - check_module_inuse()
 * - kmod_module_remove_module()
 * - kmod_module_unref()
 * - kmod_unref()
 * - log_close()
 */ 
static int do_rmmod(int argc, char *argv[])
{
	struct kmod_ctx *ctx;
	const char *null_config = NULL;
	int flags = KMOD_REMOVE_NOWAIT;
	int i, err, r = 0;

	for (;;) {
		int c, idx = 0;
		c = getopt_long(argc, argv, cmdopts_s, cmdopts, &idx);
		if (c == -1)
			break;
		switch (c) {
		case 'f':
			flags |= KMOD_REMOVE_FORCE;
			break;
		case 's':
			use_syslog = 1;
			break;
		case 'v':
			verbose++;
			break;
		case 'w':
			ERR("'Wait' behavior is targeted for removal from kernel.\nWe will now sleep for 10s, and then continue.\n");
			sleep(10);
			flags &= ~KMOD_REMOVE_NOWAIT;
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
			ERR("unexpected getopt_long() value '%c'.\n", c);
			return EXIT_FAILURE;
		}
	}

	log_open(use_syslog);

	if (optind >= argc) {
		ERR("missing module name.\n");
		r = EXIT_FAILURE;
		goto done;
	}

	ctx = kmod_new(NULL, &null_config);
	if (!ctx) {
		ERR("kmod_new() failed!\n");
		r = EXIT_FAILURE;
		goto done;
	}

	log_setup_kmod_log(ctx, verbose);

	for (i = optind; i < argc; i++) {
		struct kmod_module *mod;
		const char *arg = argv[i];
		struct stat st;
		if (stat(arg, &st) == 0)
			err = kmod_module_new_from_path(ctx, arg, &mod);
		else
			err = kmod_module_new_from_name(ctx, arg, &mod);

		if (err < 0) {
			ERR("could not use module %s: %s\n", arg,
			    strerror(-err));
			break;
		}

		if (!(flags & KMOD_REMOVE_FORCE) && (flags & KMOD_REMOVE_NOWAIT))
			if (check_module_inuse(mod) < 0) {
				r++;
				goto next;
			}

		err = kmod_module_remove_module(mod, flags);
		if (err < 0) {
			ERR("could not remove module %s: %s\n", arg,
			    strerror(-err));
			r++;
		}
next:
		kmod_module_unref(mod);
	}

	kmod_unref(ctx);

done:
	log_close();

	return r == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}


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
const struct kmod_cmd kmod_cmd_compat_rmmod = {
	.name = "rmmod",
	.cmd = do_rmmod,
	.help = "compat rmmod command",
};
