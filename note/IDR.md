# IDR

ID到指针/对象的双向映射。

不过内核社区已逐步弃用旧的IDR接口，并推荐使用更新的XArray替代。

## API

1. 结构体

    ```c++
    struct idr {
        struct radix_tree_root idr_rt;
        unsigned int idr_base;
        unsigned int idr_next;
    };
    /*
     * idt_rt	: 基于基数树实现
     * idr_base	: 起始ID偏移量
     * idr_next	: 下一个待分配的ID
     */
    ```

    

2. 初始化与销毁

    ``````c++
    void idr_init(struct idr *idr);
    /*
     * idr : 指向要初始化的 IDR 结构体的指针
     * retval : 无
     * warning : 在调用任何其他 IDR 函数之前，必须先初始化 IDR
     */
    
    void idr_destroy(struct idr *idr);
    /*
     * idr		: 目标idr
     * warning	: 若内核版本低于v3.9需要先手动调用idr_remove_all
     */
    ``````

    

3. 分配

    ```c++
    int idr_alloc(struct idr *idr, void *ptr, int start, int end, gfp_t gfp_mask);
    /*
     * idr      : 目标 idr
     * ptr      : 要关联的指针（不能为 NULL）
     * start    : 最小允许 ID（包含）
     * end      : 最大允许 ID（包含），若为 0 则表示 INT_MAX
     * gfp_mask : 内存分配标志
     * retval   : 成功返回分配的 ID（≥0），失败返回负错误码（-ENOSPC 无空闲 ID，-ENOMEM 内存不足）
     * warning  : 如果 start > end，函数会自动交换两者；ptr 不能为 NULL
     */
    
    int idr_alloc_u32(struct idr *idr, void *ptr, u32 *next_id, unsigned long max, gfp_t gfp_mask);
    /*
     * idr      : 目标 idr
     * ptr      : 要关联的指针（不能为 NULL）
     * next_id  : 输入/输出参数，调用时指定搜索起始 ID，成功后存放实际分配的 ID
     * max      : 最大允许 ID（必须 ≤ UINT32_MAX）
     * gfp_mask : 内存分配标志
     * retval   : 成功返回 0，失败返回负错误码（-ENOSPC 无可用 ID，-ENOMEM 内存不足）
     * warning  : 成功时分配的 ID 通过 next_id 返回；强制 ID 不超过 32 位
     */
    
    int idr_alloc_cyclic(struct idr *idr, void *ptr, int start, int end, gfp_t gfp_mask);
    /*
     * idr      : 目标 idr
     * ptr      : 要关联的指针（不能为 NULL）
     * start    : 搜索范围的起始 ID（包含）
     * end      : 搜索范围的结束 ID（包含），若为 0 则表示 INT_MAX
     * gfp_mask : 内存分配标志
     * retval   : 成功返回分配的 ID（≥0），失败返回负错误码（-ENOSPC, -ENOMEM）
     * warning  : 从 idr->idr_next 开始搜索，到达 end 后回绕到 start；成功后更新 idr->idr_next
     */
    ```

    

4. 查找

    ```c++
    void *idr_find(const struct idr *idr, unsigned long id);
    /*
     * idr : 目标 idr
     * id  : 要查找的 ID
     * retval : 如果 ID 存在，返回关联的指针；如果 ID 不存在或已被移除，返回 NULL
     * warning : NULL 既表示“未找到”，也可能是用户存储的合法 NULL 指针，无法区分这两种情况
     */
    ```

    

5. 替换

    ```c++
    void *idr_replace(struct idr *idr, void *ptr, unsigned long id);
    /*
     * idr : 目标 idr
     * ptr : 新的指针（不能为 NULL）
     * id  : 已分配的 ID（必须存在）
     * retval : 成功返回被替换的旧指针；如果 id 尚未分配，返回 ERR_PTR(-ENOENT)
     * warning : 需要用 IS_ERR() 检查返回值是否为错误；调用前必须确保 id 已分配
     */
    ```

    

6. 移除

    ```c++
    void idr_remove(struct idr *idr, unsigned long id);
    /*
     * idr : 目标 idr
     * id  : 要移除的 ID
     * retval : 无
     * warning : 不会释放 ptr 指向的内存，调用者需自行 kfree；重复移除同一 id 安全
     */
    ```

    

7. 遍历

    ```c++
    idr_for_each_entry(struct idr *idr, void *entry, int id);
    /*
     * idr   : 目标 idr
     * entry : 循环变量，每次迭代指向当前条目的指针
     * id    : 整型变量，每次迭代存放当前条目的 ID
     * warning : 遍历时可以安全删除当前条目，但不能插入或删除其他条目；需要删除所有条目建议直接用 idr_destroy
     */
    ```

    

8. 辅助

    ```c++
    int idr_empty(const struct idr *idr);
    /*
     * idr		: 目标 idr
     * retval	: idr 为空返回非 0，否则返回 0
     */
    ```

    

