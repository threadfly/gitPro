#ifndef _MY_LIST_H_
#define _MY_LIST_H_
struct list_head{
    struct list_head *next, *prev;
}
#define LIST_HEAD_INIT(name) \
        {&(name), &(name)}
// 初始化使得list_head的两个元素指向自己
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)
/* 初始化链表头节点
 * 感觉这方法跟前面的宏的功能一样啊
 *
 */
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}
/*
 * 在两个连续的节点之间插入一个新的节点
 *
 */
//#ifndef CONFIG_DEBUG_LIST
static inline void __list_add(struct list_head *new,
                             struct list_head *prev,
                             struct list_head *next)
{
    prev->next = new;
    new->next = next;
    new->prev = prev;
    next->prev = new;
}
//#else
extern void __list_add(struct list_head *new,
                       struct list_head *prev,
                       struct list_head *next);
//#endif
/* list_add - 新添加一个节点
 * @new: 一个新的需添加的节点
 * @head: 一个链表头节点用来添加到它后面
 *
 */
static inline void list_add(struct list_head *new,
                            struct list_head *head)
{
    __list_add(new, head, head->next);
}
/* list_add_tail - 新添加一个节点
 * @new
 *
 */
static inline void list_add_tail(struct list_head * new, struct list_head * head)
{
    __list_add(new, head->prev, head);
}
/* 
 * 删除一个链表节点通过节点前驱和后驱
 *
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    prev->next = next;
    next->prev =prev;
}
/*
 * list_del - 从链表中删除一个节点
 *
 */
//#ifndef CONFIG_DEBUG_LIST
static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);   
}
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}
extern void __list_del_entry(struct list_head *entry);
extern void list_del(struct list_head *entry);
/*
 * list_replace - 替换一个
 *
 */


#endif
