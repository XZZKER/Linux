// SPDX-License-Identifier: GPL-2.0
/*
 * This is a link list test code.
 * Xzzker
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>

struct element {
#define NAME_LEN 64
	struct list_head list;
	char name[NAME_LEN];
};

static struct element *alloc_element(char *name)
{
	struct element *ele;

	ele = kmalloc(sizeof(struct element), GFP_KERNEL);
	if (!ele)
		return NULL;
	strcpy(ele->name, name);
	return ele;
}

static void free_element(struct element *ele)
{
	kfree(ele);
}

static int __init link_list_init(void)
{
	struct element head;
	struct element *ele1, *ele2, *ele3, *ele, *ele_tmp;

	INIT_LIST_HEAD(&head.list);

	ele1 = alloc_element("ele1");
	if (!ele1)
		return -ENOMEM;
	list_add_tail(&ele1->list, &head.list);

	ele2 = alloc_element("ele2");
	if (!ele2) {
		free_element(ele1);
		return -ENOMEM;
	}
	list_add_tail(&ele2->list, &head.list);

	ele3 = alloc_element("ele3");
	if (!ele3) {
		free_element(ele1);
		free_element(ele2);
		return -ENOMEM;
	}
	list_add_tail(&ele3->list, &head.list);

	pr_info("start delete element.\n");
	list_for_each_entry_safe(ele, ele_tmp, &head.list, list)
		pr_info("element->name = %s\n", ele->name);

	pr_info("start delete all element.\n");
	list_for_each_entry_safe(ele, ele_tmp, &head.list, list) {
		list_del(&ele->list);
		free_element(ele);
	}

	if (list_empty(&head.list))
		pr_info("list is empty.");
	else
		pr_info("list is not empty.");

	return 0;
}

static void __exit link_list_exit(void)
{
}

MODULE_AUTHOR("xzzker");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a link list test code");

module_init(link_list_init);
module_exit(link_list_exit);
