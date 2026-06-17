// SPDX-License-Identifier: GPL-2.0
/*
 * This is a bitmap test code.
 * Xzzker
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>

static int __init bitmap_init(void)
{
#define BITMAP_LEN 128
	DECLARE_BITMAP(test_bitmap1, BITMAP_LEN);
	DECLARE_BITMAP(test_bitmap2, BITMAP_LEN);
	int i, start, align_mask;

	for (i = 0; i < 10; i++)
		bitmap_set(test_bitmap1, i, 1);

	if (bitmap_full(test_bitmap1, BITMAP_LEN))
		pr_info("test_bitmap1 is full.\n");
	else
		pr_info("test_bitmap1 is not full.\n");

	bitmap_zero(test_bitmap1, BITMAP_LEN);

	bitmap_fill(test_bitmap2, BITMAP_LEN);

	bitmap_and(test_bitmap1, test_bitmap1, test_bitmap2, BITMAP_LEN);

	if (bitmap_empty(test_bitmap1, BITMAP_LEN))
		pr_info("test_bitmap1 is empty.\n");
	else
		pr_info("test_bitmap1 is not empty.\n");

	start = 0;
	align_mask = 0;
	i = bitmap_find_next_zero_area(test_bitmap1, BITMAP_LEN,
				       start, 2, align_mask);

	pr_info("idx = %d\n", i);

	return 0;
}

static void __exit bitmap_exit(void)
{
}

MODULE_AUTHOR("xzzker");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("This is bitmap code");

module_init(bitmap_init);
module_exit(bitmap_exit);
