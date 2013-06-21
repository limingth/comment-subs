/*
 * libkmod - interface to kernel module operations
 *
 * Copyright (C) 2011-2012  ProFUSION embedded systems
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "libkmod.h"
#include "libkmod-array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* basic pointer array growing in steps */

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
 * @brief  array 结构体初始化
 */
void array_init(struct array *array, size_t step)
{
	assert(step > 0);
	array->array = NULL;
	array->count = 0;
	array->total = 0;
	array->step = step;
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
 * @brief  array 结构体添加元素
 */
int array_append(struct array *array, const void *element)
{
	size_t idx;

	if (array->count + 1 >= array->total) {
		size_t new_total = array->total + array->step;
		void *tmp = realloc(array->array, sizeof(void *) * new_total);
		assert(array->step > 0);
		if (tmp == NULL)
			return -ENOMEM;
		array->array = tmp;
		array->total = new_total;
	}
	idx = array->count;
	array->array[idx] = (void *)element;
	array->count++;
	return idx;
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
 * @brief  array 结构体添加新元素，确保之前不存在才执行添加操作
 */
int array_append_unique(struct array *array, const void *element)
{
	void **itr = array->array;
	void **itr_end = itr + array->count;
	for (; itr < itr_end; itr++)
		if (*itr == element)
			return -EEXIST;
	return array_append(array, element);
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
 * @brief  array 出栈操作
 */
void array_pop(struct array *array) {
	array->count--;
	if (array->count + array->step < array->total) {
		size_t new_total = array->total - array->step;
		void *tmp = realloc(array->array, sizeof(void *) * new_total);
		assert(array->step > 0);
		if (tmp == NULL)
			return;
		array->array = tmp;
		array->total = new_total;
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
 * @brief  array 释放内层空间操作
 */
void array_free_array(struct array *array) {
	free(array->array);
	array->count = 0;
	array->total = 0;
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
 * @brief  array 元素的排序操作，采用 qsort 快速排序
 */
void array_sort(struct array *array, int (*cmp)(const void *a, const void *b))
{
	qsort(array->array, array->count, sizeof(void *), cmp);
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
 * @brief  array 删除节点操作，可以删除在指定位置 pos 的元素
 */
int array_remove_at(struct array *array, unsigned int pos)
{
	if (array->count <= pos)
		return -ENOENT;

	array->count--;
	if (pos < array->count)
		memmove(array->array + pos, array->array + pos + 1,
			sizeof(void *) * (array->count - pos));

	if (array->count + array->step < array->total) {
		size_t new_total = array->total - array->step;
		void *tmp = realloc(array->array, sizeof(void *) * new_total);
		assert(array->step > 0);
		if (tmp == NULL)
			return 0;
		array->array = tmp;
		array->total = new_total;
	}

	return 0;
}
