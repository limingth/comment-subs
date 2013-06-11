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

#include <errno.h>
#include <dlfcn.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "testsuite.h"


/**
 * @topic 本注释得到了"核高基"科技重大专项2012年课题“开源操作系统内核分析和安全性评估
 *（课题编号：2012ZX01039-004）”的资助。
 *
 * @group 注释添加单位：清华大学——03任务（Linux内核相关通用基础软件包分析）承担单位
 *
 * @author 注释添加人员： 李明
 *
 * @details 注释详细内容:
 * 
 * 自行设计实现的 uname 函数，标准c库中的函数 
 * 通过调用 dlopen, dlsym 动态库接口函数
 * 获得 libc.so.6 中的 uname 函数入口地址
 * 调用其中的 uname() 函数, 完成相关数据 S_TC_UNAME 的赋值
 */
TS_EXPORT int uname(struct utsname *u)
{
	static void *nextlib = NULL;
	static int (*nextlib_uname)(struct utsname *u);
	const char *release = getenv(S_TC_UNAME_R);
	int err;
	size_t sz;

	if (release == NULL) {
		fprintf(stderr, "TRAP uname(): missing export %s?\n",
							S_TC_UNAME_R);
		errno = EFAULT;
		return -1;
	}

	if (nextlib == NULL) {
#ifdef RTLD_NEXT
		nextlib = RTLD_NEXT;
#else
		nextlib = dlopen("libc.so.6", RTLD_LAZY);
#endif
		nextlib_uname = dlsym(nextlib, "uname");
	}

	err = nextlib_uname(u);
	if (err < 0)
		return err;

	sz = strlen(release) + 1;
	if (sz > sizeof(u->release)) {
		fprintf(stderr, "uname(): sizeof release (%s) "
				"is greater than available space: %zu",
				release, sizeof(u->release));
		errno = -EFAULT;
		return -1;
	}

	memcpy(u->release, release, sz);
	return 0;
}
