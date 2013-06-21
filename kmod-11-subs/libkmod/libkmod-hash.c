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
#include "libkmod-hash.h"

#include "libkmod-util.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
 * @brief  hash entry 表项的数据结构，仅包含 key 和 value 
 * 其中 key 是 字符串指针
 * value 是无类型 void * 指针
 */

struct hash_entry {
	const char *key;
	const void *value;
};

struct hash_bucket {
	struct hash_entry *entries;
	unsigned int used;
	unsigned int total;
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
 * @brief  hash 表的数据结构，包含 count, step, n_buckets, buckets 指针
 */

struct hash {
	unsigned int count;
	unsigned int step;
	unsigned int n_buckets;
	void (*free_value)(void *value);
	struct hash_bucket buckets[];
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
 * @brief  hash 表的创建，给定 n_buckets 个
 */
struct hash *hash_new(unsigned int n_buckets,
					void (*free_value)(void *value))
{
	struct hash *hash = calloc(1, sizeof(struct hash) +
				n_buckets * sizeof(struct hash_bucket));
	if (hash == NULL)
		return NULL;
	hash->n_buckets = n_buckets;
	hash->free_value = free_value;
	hash->step = n_buckets / 32;
	if (hash->step == 0)
		hash->step = 4;
	else if (hash->step > 64)
		hash->step = 64;
	return hash;
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
 * @brief  hash 表的销毁
 */
void hash_free(struct hash *hash)
{
	struct hash_bucket *bucket, *bucket_end;

	if (hash == NULL)
		return;

	bucket = hash->buckets;
	bucket_end = bucket + hash->n_buckets;
	for (; bucket < bucket_end; bucket++) {
		if (hash->free_value) {
			struct hash_entry *entry, *entry_end;
			entry = bucket->entries;
			entry_end = entry + bucket->used;
			for (; entry < entry_end; entry++)
				hash->free_value((void *)entry->value);
		}
		free(bucket->entries);
	}
	free(hash);
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
 * @brief  计算 hash 值，传入 key 和 key 的长度，返回 hashval
 */
static inline unsigned int hash_superfast(const char *key, unsigned int len)
{
	/* Paul Hsieh (http://www.azillionmonkeys.com/qed/hash.html)
	 * used by WebCore (http://webkit.org/blog/8/hashtables-part-2/)
	 * EFL's eina and possible others.
	 */
	unsigned int tmp, hash = len, rem = len & 3;

	len /= 4;

	/* Main loop */
	for (; len > 0; len--) {
		hash += get_unaligned((uint16_t *) key);
		tmp = (get_unaligned((uint16_t *)(key + 2)) << 11) ^ hash;
		hash = (hash << 16) ^ tmp;
		key += 4;
		hash += hash >> 11;
	}

	/* Handle end cases */
	switch (rem) {
	case 3:
		hash += get_unaligned((uint16_t *) key);
		hash ^= hash << 16;
		hash ^= key[2] << 18;
		hash += hash >> 11;
		break;

	case 2:
		hash += get_unaligned((uint16_t *) key);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;

	case 1:
		hash += *key;
		hash ^= hash << 10;
		hash += hash >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}

/*
 * add or replace key in hash map.
 *
 * none of key or value are copied, just references are remembered as is,
 * make sure they are live while pair exists in hash!
 */
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
 * @brief  从当前的 hash 表 添加一个 key
 */
int hash_add(struct hash *hash, const char *key, const void *value)
{
	unsigned int keylen = strlen(key);
	unsigned int hashval = hash_superfast(key, keylen);
	unsigned int pos = hashval % hash->n_buckets;
	struct hash_bucket *bucket = hash->buckets + pos;
	struct hash_entry *entry, *entry_end;

	if (bucket->used + 1 >= bucket->total) {
		unsigned new_total = bucket->total + hash->step;
		size_t size = new_total * sizeof(struct hash_entry);
		struct hash_entry *tmp = realloc(bucket->entries, size);
		if (tmp == NULL)
			return -errno;
		bucket->entries = tmp;
		bucket->total = new_total;
	}

	entry = bucket->entries;
	entry_end = entry + bucket->used;
	for (; entry < entry_end; entry++) {
		int c = strcmp(key, entry->key);
		if (c == 0) {
			if (hash->free_value)
				hash->free_value((void *)entry->value);
			entry->value = value;
			return 0;
		} else if (c < 0) {
			memmove(entry + 1, entry,
				(entry_end - entry) * sizeof(struct hash_entry));
			break;
		}
	}

	entry->key = key;
	entry->value = value;
	bucket->used++;
	hash->count++;
	return 0;
}

/* similar to hash_add(), but fails if key already exists */
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
 * @brief  添加一个 key 和 value 进入当前的 hash 表, 如果已经存在则返回失败
 */
int hash_add_unique(struct hash *hash, const char *key, const void *value)
{
	unsigned int keylen = strlen(key);
	unsigned int hashval = hash_superfast(key, keylen);
	unsigned int pos = hashval % hash->n_buckets;
	struct hash_bucket *bucket = hash->buckets + pos;
	struct hash_entry *entry, *entry_end;

	if (bucket->used + 1 >= bucket->total) {
		unsigned new_total = bucket->total + hash->step;
		size_t size = new_total * sizeof(struct hash_entry);
		struct hash_entry *tmp = realloc(bucket->entries, size);
		if (tmp == NULL)
			return -errno;
		bucket->entries = tmp;
		bucket->total = new_total;
	}

	entry = bucket->entries;
	entry_end = entry + bucket->used;
	for (; entry < entry_end; entry++) {
		int c = strcmp(key, entry->key);
		if (c == 0)
			return -EEXIST;
		else if (c < 0) {
			memmove(entry + 1, entry,
				(entry_end - entry) * sizeof(struct hash_entry));
			break;
		}
	}

	entry->key = key;
	entry->value = value;
	bucket->used++;
	hash->count++;
	return 0;
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
 * @brief  hash 比较，基于字符串 strcmp 的比较 key 
 */
static int hash_entry_cmp(const void *pa, const void *pb)
{
	const struct hash_entry *a = pa;
	const struct hash_entry *b = pb;
	return strcmp(a->key, b->key);
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
 * @brief  hash 查找
 * 从 hash 指针所指向的 hash 表中，找到元素 key 
 * 采用二分查找法
 */
void *hash_find(const struct hash *hash, const char *key)
{
	unsigned int keylen = strlen(key);
	unsigned int hashval = hash_superfast(key, keylen);
	unsigned int pos = hashval % hash->n_buckets;
	const struct hash_bucket *bucket = hash->buckets + pos;
	const struct hash_entry se = {
		.key = key,
		.value = NULL
	};
	const struct hash_entry *entry = bsearch(
		&se, bucket->entries, bucket->used,
		sizeof(struct hash_entry), hash_entry_cmp);
	if (entry == NULL)
		return NULL;
	return (void *)entry->value;
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
 * @brief  从当前的 hash 表 删除一个 key
 */

int hash_del(struct hash *hash, const char *key)
{
	unsigned int keylen = strlen(key);
	unsigned int hashval = hash_superfast(key, keylen);
	unsigned int pos = hashval % hash->n_buckets;
	unsigned int steps_used, steps_total;
	struct hash_bucket *bucket = hash->buckets + pos;
	struct hash_entry *entry, *entry_end;
	const struct hash_entry se = {
		.key = key,
		.value = NULL
	};

	entry = bsearch(&se, bucket->entries, bucket->used,
		sizeof(struct hash_entry), hash_entry_cmp);
	if (entry == NULL)
		return -ENOENT;

	if (hash->free_value)
		hash->free_value((void *)entry->value);

	entry_end = bucket->entries + bucket->used;
	memmove(entry, entry + 1,
		(entry_end - entry) * sizeof(struct hash_entry));

	bucket->used--;
	hash->count--;

	steps_used = bucket->used / hash->step;
	steps_total = bucket->total / hash->step;
	if (steps_used + 1 < steps_total) {
		size_t size = (steps_used + 1) *
			hash->step * sizeof(struct hash_entry);
		struct hash_entry *tmp = realloc(bucket->entries, size);
		if (tmp) {
			bucket->entries = tmp;
			bucket->total = (steps_used + 1) * hash->step;
		}
	}

	return 0;
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
 * @brief  hash 获得表项数量, 返回 hash->count
 */
unsigned int hash_get_count(const struct hash *hash)
{
	return hash->count;
}

void hash_iter_init(const struct hash *hash, struct hash_iter *iter)
{
	iter->hash = hash;
	iter->bucket = 0;
	iter->entry = -1;
}

bool hash_iter_next(struct hash_iter *iter, const char **key,
							const void **value)
{
	const struct hash_bucket *b = iter->hash->buckets + iter->bucket;
	const struct hash_entry *e;

	iter->entry++;

	if (iter->entry >= b->used) {
		iter->entry = 0;

		for (iter->bucket++; iter->bucket < iter->hash->n_buckets;
							iter->bucket++) {
			b = iter->hash->buckets + iter->bucket;

			if (b->used > 0)
				break;
		}

		if (iter->bucket >= iter->hash->n_buckets)
			return false;
	}

	e = b->entries + iter->entry;

	if (value != NULL)
		*value = e->value;
	if (key != NULL)
		*key = e->key;

	return true;
}
