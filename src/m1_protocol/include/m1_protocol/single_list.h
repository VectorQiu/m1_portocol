/**
 * \file            single_list.h
 * \brief           Single List
 * \date            2024-12-07
 */

/*
 * Copyright (c) 2024 Vector Qiu
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of the m1 protocol library.
 *
 * Author:          Vector Qiu <vetor.qiu@gmail.com>
 * Version:         V0.0.1
 */
#ifndef __SINGLE_LIST_H__
#define __SINGLE_LIST_H__

/* includes ----------------------------------------------------------------- */
#include <stddef.h>
#include "rtcompiler.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        single list manager
 * \brief           single list manager
 * \{
 */
/* public define ------------------------------------------------------------ */
#ifndef rt_container_of
/**
 * \brief           Retrieve the pointer to the container structure from a
 *                  member
 *
 * \param[in]       ptr: Pointer to the member
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the member in the container structure
 * \return          Pointer to the container structure
 *
 * \note            用于从链表节点指针反推包含该节点的结构体指针。
 */
#define rt_container_of(ptr, type, member)                                     \
    ((type*)((char*)(ptr) - (uintptr_t)(&((type*)0)->member)))
#endif

/**
 * \brief           Macro to initialize a single list object
 *
 * \param[in]       object: The single list object to initialize
 */
#define SINGLE_LIST_OBJECT_INIT(object) {NULL}

/* public typedef struct ---------------------------------------------------- */
/**
 * \brief           Single linked list node structure
 *
 * \details         This structure represents a node in a single linked list.
 *                  Each node contains a pointer to the next node.
 */
struct single_list_node {
    struct single_list_node* next; /*!< Pointer to the next node */
};
typedef struct single_list_node single_list_t;

/* public functions --------------------------------------------------------- */
static inline void single_list_init(single_list_t* l) {
    l->next = NULL;
}

/**
 * \brief           Append a node to the end of the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \param[in]       n: Pointer to the new node to append
 *
 * \details         遍历链表找到尾节点，将新节点追加到链表尾部。
 */
static inline void single_list_append(single_list_t* l, single_list_t* n) {
    struct single_list_node* node;

    node = l;
    while (node->next) {
        node = node->next;
    }

    /* append the node to the tail */
    node->next = n;
    n->next = NULL;
}

/**
 * \brief           Insert a node after the head of the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \param[in]       n: Pointer to the new node to insert
 *
 * \details         将新节点插入到头节点后，链表其余部分保持不变。
 */
static inline void single_list_insert(single_list_t* l, single_list_t* n) {
    n->next = l->next;
    l->next = n;
}

/**
 * \brief           Calculate the length of the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \return          The number of nodes in the list
 *
 * \details         从头节点开始遍历，统计节点个数。
 */
static inline unsigned int single_list_len(const single_list_t* l) {
    unsigned int len = 0;
    const single_list_t* list = l->next;
    while (list != NULL) {
        list = list->next;
        len++;
    }

    return len;
}

/**
 * \brief           Remove a node from the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \param[in]       n: Pointer to the node to remove
 * \return          Pointer to the head of the list
 *
 * \details         从链表中移除指定节点，调整前驱节点的 `next` 指针。
 */
static inline single_list_t* single_list_remove(single_list_t* l,
                                                single_list_t* n) {
    /* remove single list head */
    struct single_list_node* node = l;
    while (node->next && node->next != n) {
        node = node->next;
    }

    /* remove node */
    if (node->next != (single_list_t*)0) {
        node->next = node->next->next;
    }

    return l;
}

/**
 * \brief           Get the first node in the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \return          Pointer to the first node, or NULL if the list is empty
 */
static inline single_list_t* single_list_first(single_list_t* l) {
    return l->next;
}

/**
 * \brief           Get the last node in the single linked list
 *
 * \param[in]       l: Pointer to the head of the list
 * \return          Pointer to the last node
 */
static inline single_list_t* single_list_tail(single_list_t* l) {
    while (l->next) {
        l = l->next;
    }

    return l;
}

/**
 * \brief           Get the next node in the single linked list
 *
 * \param[in]       n: Pointer to the current node
 * \return          Pointer to the next node, or NULL if it does not exist
 */
static inline single_list_t* single_list_next(single_list_t* n) {
    return n->next;
}

/**
 * \brief           Check if the single linked list is empty
 *
 * \param[in]       l: Pointer to the head of the list
 * \return          1 if the list is empty, 0 otherwise
 */
static inline int single_list_isempty(single_list_t* l) {
    return (l->next == NULL);
}

/* public define ------------------------------------------------------------ */
/**
 * \brief           Retrieve the container structure from a single list node
 *
 * \param[in]       node: Pointer to the single list node
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the single list member in the structure
 * \return          Pointer to the container structure
 */
#define single_list_entry(node, type, member)                                  \
    rt_container_of(node, type, member)

/**
 * \brief           Iterate over a single linked list
 *
 * \param[in]       pos: Pointer to the current node (used as a loop variable)
 * \param[in]       head: Pointer to the head of the list
 *
 * \details         遍历链表中的所有节点。
 */
#define single_list_for_each(pos, head)                                        \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * \brief           Iterate over a single linked list of a given type
 *
 * \param[in]       pos: Pointer to the current container structure
 * \param[in]       head: Pointer to the head of the list
 * \param[in]       member: Name of the single list member in the structure
 *
 * \details         遍历链表中的所有节点，并通过链表节点获取其所属的结构体。
 */
#define single_list_for_each_entry(pos, head, member)                          \
    for (pos =                                                                 \
             ((head)->next == (NULL)                                           \
                  ? (NULL)                                                     \
                  : single_list_entry((head)->next, rt_typeof(*pos), member)); \
         (pos != (NULL)) && (&pos->member != (NULL));                          \
         pos = (pos->member.next == (NULL)                                     \
                    ? (NULL)                                                   \
                    : single_list_entry(pos->member.next, rt_typeof(*pos),     \
                                        member)))

/**
 * \brief           Get the first entry of a single linked list
 *
 * \param[in]       ptr: Pointer to the head of the list
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the single list member in the structure
 * \return          Pointer to the first container structure
 */
#define single_list_first_entry(ptr, type, member)                             \
    single_list_entry((ptr)->next, type, member)

/**
 * \brief           Get the last entry of a single linked list
 *
 * \param[in]       ptr: Pointer to the head of the list
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the single list member in the structure
 * \return          Pointer to the last container structure
 */
#define single_list_tail_entry(ptr, type, member)                              \
    single_list_entry(single_list_tail(ptr), type, member)
/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SINGLE_LIST_H__ */

/* ----------------------------- end of file -------------------------------- */
