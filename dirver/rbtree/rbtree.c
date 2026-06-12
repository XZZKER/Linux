// SPDX-License-Identifier: GPL-2.0
/*
 * This is a rbtree test code.
 * Xzzker
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/slab.h>

struct par {
	u32		id;
	char		*val;
	struct rb_node	node;
};

/*
 * root		: 红黑树根节点
 * id		: 查找目标 id
 * retval	: 若树上有对应 id 的节点，返回节点指针；否则返回 NULL
 */
static struct par *rbtree_find_by_id(struct rb_root *root, u32 id)
{
	struct rb_node *node;
	struct par *par;

	node = root->rb_node;
	while (node) {
		par = rb_entry(node, struct par, node);
		if (par->id < id)
			node = node->rb_right;
		else if (par->id > id)
			node = node->rb_left;
		else
			return par;
	}
	return NULL;
}

/*
 * root		: 红黑树根节点
 * par		: 要插入的 par 节点
 * retval	: 无
 */
static void rbtree_insert(struct rb_root *root, struct par *par)
{
	struct rb_node **new_node, *parent_node = NULL;
	struct par *cur_par;

	new_node = &(root->rb_node);
	while (*new_node) {
		parent_node = *new_node;
		cur_par = rb_entry(parent_node, struct par, node);
		if (cur_par->id > par->id)
			new_node = &((*new_node)->rb_left);
		else if (cur_par->id < par->id)
			new_node = &((*new_node)->rb_right);
		else {
			pr_info("%u already in tree\n", par->id);
			return;
		}
	}

	rb_link_node(&par->node, parent_node, new_node);
	rb_insert_color(&par->node, root);
}

/*
 * root		: 红黑树根节点
 * par		: 要删除的 par 节点
 * retval	: 无
 */
static void rbtree_delete(struct rb_root *root, struct par *par)
{
	rb_erase(&par->node, root);
}

/*
 * root		: 红黑树根节点
 * retval	: 无
 */
static void rbtree_delete_all(struct rb_root *root)
{
	struct rb_node *node, *next;
	struct par *cur_par;

	node = rb_first(root);
	while (node) {
		next = rb_next(node);
		cur_par = rb_entry(node, struct par, node);
		rbtree_delete(root, cur_par);
		node = next;
	}
}

static int __init rbtree_init(void)
{
	struct rb_root root = RB_ROOT;
	struct par par0 = {
		.id = 0,
		.val = "test0"
	}, par1 = {
		.id = 1,
		.val = "test1"
	}, par2 = {
		.id = 2,
		.val = "test2"
	};

	rbtree_insert(&root, &par0);
	rbtree_insert(&root, &par1);
	rbtree_insert(&root, &par2);

	pr_info("find id = %u, val = %s\n", 0, rbtree_find_by_id(&root, 0)->val);
	pr_info("find id = %u, val = %s\n", 1, rbtree_find_by_id(&root, 1)->val);
	pr_info("find id = %u, val = %s\n", 2, rbtree_find_by_id(&root, 2)->val);

	rbtree_delete_all(&root);

	return 0;
}

static void __exit rbtree_exit(void)
{
}

MODULE_AUTHOR("xzzker");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is rbtree code");

module_init(rbtree_init);
module_exit(rbtree_exit);
