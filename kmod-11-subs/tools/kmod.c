/*
 * kmod - one tool to rule them all
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
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <libkmod.h>
#include "kmod.h"


static const char options_s[] = "+hV";
static const struct option options[] = {
	{ "help", no_argument, NULL, 'h' },
	{ "version", no_argument, NULL, 'V' },
	{}
};

static const struct kmod_cmd kmod_cmd_help;

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
 * @brief  kmod 命令的主程序
 */
static const struct kmod_cmd *kmod_cmds[] = {
	&kmod_cmd_help,
	&kmod_cmd_list,
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
 * @brief  kmod 支持的compat命令 lsmod/rmmod/insmod/modinfo/modprobe/depmod
 */
static const struct kmod_cmd *kmod_compat_cmds[] = {
	&kmod_cmd_compat_lsmod,
	&kmod_cmd_compat_rmmod,
	&kmod_cmd_compat_insmod,
	&kmod_cmd_compat_modinfo,
	&kmod_cmd_compat_modprobe,
	&kmod_cmd_compat_depmod,
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
 * @brief  kmod 的帮助提示，运行 kmod help 时显示信息
 */
static int kmod_help(int argc, char *argv[])
{
	size_t i;

	printf("kmod - Manage kernel modules: list, load, unload, etc\n"
			"Usage:\n"
			"\t%s [options] command [command_options]\n\n"
			"Options:\n"
			"\t-V, --version     show version\n"
			"\t-h, --help        show this help\n\n"
			"Commands:\n", basename(argv[0]));

	for (i = 0; i < ARRAY_SIZE(kmod_cmds); i++) {
		if (kmod_cmds[i]->help != NULL) {
			printf("  %-12s %s\n", kmod_cmds[i]->name,
							kmod_cmds[i]->help);
		}
	}

	puts("\nkmod also handles gracefully if called from following symlinks:");

	for (i = 0; i < ARRAY_SIZE(kmod_compat_cmds); i++) {
		if (kmod_compat_cmds[i]->help != NULL) {
			printf("  %-12s %s\n", kmod_compat_cmds[i]->name,
						kmod_compat_cmds[i]->help);
		}
	}

	return EXIT_SUCCESS;
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
 * @brief  kmod help 帮助命令的数据结构
 */
static const struct kmod_cmd kmod_cmd_help = {
	.name = "help",
	.cmd = kmod_help,
	.help = "Show help message",
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
 * @brief  解释 kmod -h -V -? 等命令参数
 */
static int handle_kmod_commands(int argc, char *argv[])
{
	const char *cmd;
	int err = 0;
	size_t i;

	for (;;) {
		int c;

		c = getopt_long(argc, argv, options_s, options, NULL);
		if (c == -1)
			break;

		switch (c) {
		case 'h':
			kmod_help(argc, argv);
			return EXIT_SUCCESS;
		case 'V':
			puts("kmod version " VERSION);
			return EXIT_SUCCESS;
		case '?':
			return EXIT_FAILURE;
		default:
			fprintf(stderr, "Error: unexpected getopt_long() value '%c'.\n", c);
			return EXIT_FAILURE;
		}
	}

	if (optind >= argc) {
		fputs("missing command\n", stderr);
		goto fail;
	}

	cmd = argv[optind];

	for (i = 0, err = -EINVAL; i < ARRAY_SIZE(kmod_cmds); i++) {
		if (strcmp(kmod_cmds[i]->name, cmd) != 0)
			continue;

		err = kmod_cmds[i]->cmd(--argc, ++argv);
	}

	if (err < 0) {
		fprintf(stderr, "invalid command '%s'\n", cmd);
		goto fail;
	}

	return err;

fail:
	kmod_help(argc, argv);
	return EXIT_FAILURE;
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
 * @brief  解释 kmod compat 命令
 */
static int handle_kmod_compat_commands(int argc, char *argv[])
{
	const char *cmd;
	size_t i;

	cmd = basename(argv[0]);

	for (i = 0; i < ARRAY_SIZE(kmod_compat_cmds); i++) {
		if (strcmp(kmod_compat_cmds[i]->name, cmd) == 0)
			return kmod_compat_cmds[i]->cmd(argc, argv);
	}

	return -ENOENT;
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
 * @brief  所有tools下实现的命令的主程序
 */
int main(int argc, char *argv[])
{
	int err;

	printf("program_invocation_short_name = %s\n", program_invocation_short_name);
	printf("program_invocation_name = %s\n", program_invocation_name);
	printf("program_invocation_short_name = %x\n", program_invocation_short_name);
	if (strcmp(program_invocation_short_name, "kmod") == 0)
		err = handle_kmod_commands(argc, argv);
	else
		err = handle_kmod_compat_commands(argc, argv);

	return err;
}
