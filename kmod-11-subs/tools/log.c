/*
 * kmod - log infrastructure
 *
 * Copyright (C) 2012  ProFUSION embedded systems
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "libkmod.h"
#include "kmod.h"

static bool log_use_syslog;
static int log_priority = LOG_ERR;

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 *
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
 */
/* 
 * 将优先级 prio 整型数转换成相应的字符串 
 * 优先级的分类包括
 *	- FATAL 最高级别 
 *	- ERROR
 *	- WARNING
 *	- NOTICE
 *	- INFO
 *	- DEBUG
 * 详见 /usr/include/i386-linux-gnu/sys/syslog.h

 #define LOG_EMERG       0       /* system is unusable */
 #define LOG_ALERT       1       /* action must be taken immediately */
 #define LOG_CRIT        2       /* critical conditions */
 #define LOG_ERR         3       /* error conditions */
 #define LOG_WARNING     4       /* warning conditions */
 #define LOG_NOTICE      5       /* normal but significant condition */
 #define LOG_INFO        6       /* informational */
 #define LOG_DEBUG       7       /* debug-level messages */

 */
static _always_inline_ const char *prio_to_str(int prio)
{
	const char *prioname;
	char buf[32];

	switch (prio) {
	case LOG_CRIT:
		prioname = "FATAL";
		break;
	case LOG_ERR:
		prioname = "ERROR";
		break;
	case LOG_WARNING:
		prioname = "WARNING";
		break;
	case LOG_NOTICE:
		prioname = "NOTICE";
		break;
	case LOG_INFO:
		prioname = "INFO";
		break;
	case LOG_DEBUG:
		prioname = "DEBUG";
		break;
	default:
		snprintf(buf, sizeof(buf), "LOG-%03d", prio);
		prioname = buf;
	}

	return prioname;
}

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 *
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
 */
/* 核心日志记录函数，调用了系统的 syslog() 函数，增加了优先级信息打印 */
static void log_kmod(void *data, int priority, const char *file, int line,
		     const char *fn, const char *format, va_list args)
{
	const char *prioname = prio_to_str(priority);
	char *str;

	if (vasprintf(&str, format, args) < 0)
		return;

	if (log_use_syslog) {
#ifdef ENABLE_DEBUG
		syslog(priority, "%s: %s:%d %s() %s", prioname, file, line,
		       fn, str);
#else
		syslog(priority, "%s: %s", prioname, str);
#endif
	} else {
#ifdef ENABLE_DEBUG
		fprintf(stderr, "%s: %s: %s:%d %s() %s",
			program_invocation_short_name, prioname, file, line,
			fn, str);
#else
		fprintf(stderr, "%s: %s: %s", program_invocation_short_name,
			prioname, str);
#endif
	}

	free(str);
	(void)data;
}

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 *
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
 */
/* 打开日志文件, 调用了系统的 openlog() */
void log_open(bool use_syslog)
{
	log_use_syslog = use_syslog;

	if (log_use_syslog)
		openlog(program_invocation_short_name, LOG_CONS, LOG_DAEMON);
}

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 *
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
 */
/* 关闭日志文件，调用 closelog() */
void log_close(void)
{
	if (log_use_syslog)
		closelog();
}

/* 
 * 本注释得到了“核高基”科技重大专项2012年课题的资助
 * 课题名称“开源操作系统内核分析和安全性评估”
 * 课题编号“2012ZX01039-004”
 *
 * 注释添加单位 清华大学--03任务
 * Linux 内核相关通用基础软件包分析 承担单位
 * 注释添加人 李明
 * 注释日期 2013年5月4日
 */
/* 带有优先级 prio 的日志打印函数，可以设定日志打印的优先级 */
void log_printf(int prio, const char *fmt, ...)
{
	const char *prioname;
	char *msg;
	va_list args;

	if (prio > log_priority)
		return;

	va_start(args, fmt);
	if (vasprintf(&msg, fmt, args) < 0)
		msg = NULL;
	va_end(args);
	if (msg == NULL)
		return;

	prioname = prio_to_str(prio);

	if (log_use_syslog)
		syslog(prio, "%s: %s", prioname, msg);
	else
		fprintf(stderr, "%s: %s: %s", program_invocation_short_name,
			prioname, msg);
	free(msg);

	if (prio <= LOG_CRIT)
		exit(EXIT_FAILURE);
}

void log_setup_kmod_log(struct kmod_ctx *ctx, int priority)
{
	log_priority = priority;

	kmod_set_log_priority(ctx, log_priority);
	kmod_set_log_fn(ctx, log_kmod, NULL);
}
