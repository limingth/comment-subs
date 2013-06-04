/*
 * Copyright (C) 2012  Lucas De Marchi <lucas.de.marchi@gmail.com
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

#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mkdir.h"
#include "testsuite.h"

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
/* 通过调用系统函数 stat, mkdir 实现 mkdir 功能 */
TS_EXPORT int mkdir_p(const char *path, mode_t mode)
{
	char *start = strdupa(path);
	int len = strlen(path);
	char *end = start + len;
	struct stat st;

	/*
	 * scan backwards, replacing '/' with '\0' while the component doesn't
	 * exist
	 */
	for (;;) {
		if (stat(start, &st) >= 0) {
			if (S_ISDIR(st.st_mode))
				break;
			return -ENOTDIR;
		}

		/* Find the next component, backwards, discarding extra '/'*/
		for (; end != start && *end != '/'; end--)
			;

		for (; end != start - 1 && *end == '/'; end--)
			;

		end++;
		if (end == start)
			break;

		*end = '\0';
	}

	if (end == start + len)
		return 0;

	for (; end < start + len;) {
		*end = '/';
		end += strlen(end);

		if (mkdir(start, mode) < 0)
			return -errno;
	}

	return 0;
}
