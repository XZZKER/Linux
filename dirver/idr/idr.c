// SPDX-License-Identifier: GPL-2.0
/*
 * This is a idr test code.
 * Xzzker
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/idr.h>

static int __init idr_test_init(void)
{
	struct idr idr;
	int id1, id2, id3, rid;
	char *s1 = "xzzker", *s2 = "jiangjy", *s3 = "yuyang", *res;

	idr_init(&idr);

	id1 = idr_alloc(&idr, s1, 0, 100, GFP_KERNEL);
	if (id1 == -ENOMEM || id1 == -ENOSPC)
		goto FAIL;

	id2 = idr_alloc(&idr, s2, 0, 100, GFP_KERNEL);
	if (id1 == -ENOMEM || id1 == -ENOSPC)
		goto FAIL;

	id3 = idr_alloc(&idr, s3, 0, 100, GFP_KERNEL);
	if (id1 == -ENOMEM || id1 == -ENOSPC)
		goto FAIL;

	res = idr_find(&idr, id1);
	pr_info("res1 = %s\n", res);

	res = idr_find(&idr, id2);
	pr_info("res2 = %s\n", res);

	res = idr_find(&idr, id3);
	pr_info("res3 = %s\n", res);

	res = idr_remove(&idr, id1);
	pr_info("res4 = %s\n", res);

	idr_replace(&idr, s2, id3);
	res = idr_find(&idr, id3);
	pr_info("res5 = %s\n", res);

	if (idr_is_empty(&idr))
		pr_info("The idr is empty.\n");
	else
		pr_info("The idr is not empty.\n");

	idr_for_each_entry(&idr, res, rid)
		pr_info("id = %d, data = %s\n", rid, res);

FAIL:

	idr_destroy(&idr);

	return 0;
}

static void __exit idr_test_exit(void)
{
}

MODULE_AUTHOR("xzzker");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is a idr test code");

module_init(idr_test_init);
module_exit(idr_test_exit);

