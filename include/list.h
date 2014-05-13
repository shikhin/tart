#ifndef _LIST_H
#define _LIST_H

/* Linux inspired doubly linked-list implementation. */

#include <stdint.h>
#include <stdbool.h>

typedef struct list_node
{
    struct list_node *prev, *next;
} list_node_t;

#define get_offset(type, member)            (uintptr_t)&((type*)0)->(member))
#define get_container(entry, type, member)  (type*)((uintptr_t)&(entry) - get_offset(type, member))

/*
 * Internal implementation to add a new member.
 *     list_node_t *node -> node to add.
 *     list_node_t *prev -> prev node.
 *     list_node_t *next -> next node.
 */
static inline void __list_add(list_node_t *node, list_node_t *prev, list_node_t *next)
{
    prev->next = node;
    node->prev = prev;
    node->next = next;
    next->prev = node;
}

// Macros over add.
#define list_add_head(list, node) __list_add((node), (list), (list)->next)
#define list_add_tail(list, node) __list_add((node), (list)->prev, (list))
#define list_add_after(cur, node) list_add_head((cur), (node))
#define list_add_before(cur, node) list_add_tail((cur), (node))

/*
 * Internal implementation to remove a member.
 *     list_node_t *prev -> prev node.
 *     list_node_t *next -> next node.
 */
static inline void __list_rem(list_node_t *prev, list_node_t *next)
{
    prev->next = next;
    next->prev = prev;
}

/*
 * Delete a member.
 *     list_node_t *node -> node to delete.
 */
static inline void list_delete(list_node_t *node)
{
    __list_rem(node->prev, node->next);
    node->prev = node->next = 0;
}

/*
 * Delete the head of a list.
 *     list_node_t *list -> the list.
 *
 * Returns:
 *     list_node_t* -> the node deleted.
 */
static inline list_node_t* list_delete_head(list_node_t *list)
{
    list_node_t *node;
    if ((node = list->next) != list) {
        list_delete(node);
        return node;
    } else {
        return 0;
    }
}

/*
 * Delete the tail of a list.
 *     list_node_t *list -> the list.
 *
 * Returns:
 *     list_node_t* -> the node deleted.
 */
static inline list_node_t* list_delete_tail(list_node_t *list)
{
    list_node_t *node;
    if ((node = list->prev) != list) {
        list_delete(node);
        return node;
    } else {
        return 0;
    }
}

/*
 * Check if list is empty, or not.
 *     const list_node_t *list -> the list.
 *
 * Returns:
 *     bool -> true if empty.
 */
static inline bool list_is_emtpy(const list_node_t *list)
{
    return (list->next == list);
}

/*
 * Check if node is the last element in the list.
 *     const list_node_t *list -> the list.
 *     const list_node_t *node -> the node to check.
 *
 * Returns:
 *     bool -> true if last.
 */
static inline bool list_is_last(const list_node_t *list, const list_node_t *node)
{
    return (list->prev == node);
}

// Iterator.
#define list_foreach(list, type, member, iterator) for(\
                        (iterator) = get_container((list)->next, type, member); \
                        &(iterator)->member != (list); \
                        (iterator) = get_container((iterator)->(member).next, type, member))

// Safe iterator.
#define list_foreach_safe(list, type, member, iterator, temp_iter) for(\
                        (iterator) = get_container((list)->next, type, member), \
                        (temp_iter) = get_container((iterator)->(member).next, type, member); \
                        &(iterator)->member != (list); \
                        (iterator) = (temp_iter), \
                        (temp_iter) = get_container((iterator)->(member).next, type, member))

#endif /* _LIST_H */
