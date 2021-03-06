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
 * insmod 函数的真正实现，通过 getopt_long 分析传入参数，
 * 通过调用 libkmod 的接口，实现 insmod 命令
 * 主要使用的接口包括
 * - kmod_new()
 * - kmod_module_new_from_path()
 * - kmod_module_insert_module()
 * - kmod_module_unref()
 * - kmod_unref()
 */ 
