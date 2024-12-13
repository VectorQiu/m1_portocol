/**
 * \file            single_list_ex.h
 * \brief           single list expand
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
#ifndef __SINGLE_LIST_EX_H__
#define __SINGLE_LIST_EX_H__

/* includes ----------------------------------------------------------------- */
#include "./m1_protocol/single_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        single list ex manager
 * \brief           single list ex manager
 * \{
 */

/* public define ------------------------------------------------------------ */
/**
 * \brief           Macro to initialize a single list expand object
 *
 * \param[in]       object: The single list expand object to initialize
 */
#define SINGLE_LIST_EX_OBJECT_INIT(object) {NULL, NULL}

/* public typedef struct ---------------------------------------------------- */
/**
 * \brief           Extended single linked list node structure
 */
struct single_list_node_ex {
    struct single_list_node* head; /*!< Pointer to the head of the list */
    struct single_list_node* tail; /*!< Pointer to the tail of the list */
};
typedef struct single_list_node_ex single_list_ex_t;

/* public functions --------------------------------------------------------- */
/**
 * \brief           Initialize an extended single linked list
 *
 * \param[in]       l: Pointer to the extended list structure to initialize
 */
static inline void single_list_ex_init(single_list_ex_t* l) {
    l->head = NULL;
    l->tail = NULL;
}

/**
 * \brief           Append a node to the end of the extended single linked list
 *
 * \param[in,out]   l: Pointer to the extended list structure
 * \param[in]       n: Pointer to the node to append
 * \note            The node \p n is expected to have its `next` pointer as
 *                  NULL.
 */
static inline void single_list_ex_append(single_list_ex_t* l,
                                         single_list_t* n) {
    if (l->tail) {
        l->tail->next = n;
    } else {
        l->head = n;
    }
    l->tail = n;
    n->next = NULL;
}

/**
 * \brief           Insert a node at the beginning of the extended single linked
 *                  list
 *
 * \param[in,out]   l: Pointer to the extended list structure
 * \param[in]       n: Pointer to the node to insert
 * \note            If the list was empty, \p n
 *                  will also become the tail.
 */
static inline void single_list_ex_insert(single_list_ex_t* l,
                                         single_list_t* n) {
    n->next = l->head;
    l->head = n;
    if (l->tail == NULL) {
        l->tail = n;
    }
}

/**
 * \brief           Calculate the length of the extended single linked list
 *
 * \param[in]       l: Pointer to the extended list structure
 * \return          The length of the list
 */
static inline unsigned int single_list_ex_len(const single_list_ex_t* l) {
    unsigned int len = 0;
    const single_list_t* node = l->head;
    while (node) {
        node = node->next;
        len++;
    }
    return len;
}

/**
 * \brief           Remove a specific node from the extended single linked list
 *
 * \param[in,out]   l: Pointer to the extended list structure
 * \param[in]       n: Pointer to the node to remove
 * \return          Pointer to the removed node, or NULL if not found
 */
static inline single_list_t* single_list_ex_remove(single_list_ex_t* l,
                                                   single_list_t* n) {
    single_list_t* prev = NULL;
    single_list_t* current = l->head;

    while (current) {
        if (current == n) {
            if (prev) {
                prev->next = current->next;
            } else {
                l->head = current->next;
            }

            if (current == l->tail) {
                l->tail = prev;
            }

            current->next = NULL;
            return current;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

/**
 * \brief           Check if the extended single linked list is empty
 *
 * \param[in]       l: Pointer to the extended list structure
 * \return          1 if the list is empty, 0 otherwise
 */
static inline int single_list_ex_isempty(single_list_ex_t* l) {
    return (l->head == NULL);
}

/**
 * \brief           Get the first node in the extended single linked list
 *
 * \param[in]       l: Pointer to the extended list structure
 * \return          Pointer to the first node, or NULL if the list is empty
 */
static inline single_list_t* single_list_ex_first(single_list_ex_t* l) {
    return l->head;
}

/**
 * \brief           Get the last node in the extended single linked list
 *
 * \param[in]       l: Pointer to the extended list structure
 * \return          Pointer to the last node, or NULL if the list is empty
 */
static inline single_list_t* single_list_ex_tail(single_list_ex_t* l) {
    return l->tail;
}

/**
 * \brief           Get the next node in the single linked list
 *
 * \param[in]       n: Pointer to the current node
 * \return          Pointer to the next node, or NULL if at the end of the list
 */
static inline single_list_t* single_list_ex_next(single_list_t* n) {
    return n->next;
}

/* public define ------------------------------------------------------------ */
/**
 * \brief           Iterate over each node in the extended single linked list
 *
 * \param[in]       pos: Loop variable to hold the current node
 * \param[in]       list: Pointer to the extended list structure
 */
#define single_list_ex_for_each(pos, list)                                     \
    for (pos = (list)->head; pos != NULL; pos = pos->next)

/**
 * \brief           Iterate over each entry in the extended single linked list
 *
 * \param[in]       pos: Loop variable to hold the current container structure
 * \param[in]       list: Pointer to the extended list structure
 * \param[in]       member: Name of the single list member in the container
 *                  structure
 */
#define single_list_ex_for_each_entry(pos, list, member)                       \
    for (pos =                                                                 \
             ((list)->head == (NULL)                                           \
                  ? (NULL)                                                     \
                  : single_list_entry((list)->head, rt_typeof(*pos), member)); \
         (pos != (NULL)) && (&pos->member != NULL);                            \
         pos = (pos->member.next == (NULL)                                     \
                    ? (NULL)                                                   \
                    : single_list_entry(pos->member.next, rt_typeof(*pos),     \
                                        member)))

/**
 * \brief           Get the first entry in the extended single linked list
 *
 * \param[in]       ptr: Pointer to the extended list structure
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the single list member in the container
 *                  structure
 * \return          Pointer to the first container structure
 */
#define single_list_ex_first_entry(ptr, type, member)                          \
    single_list_entry((ptr)->head, type, member)

/**
 * \brief           Get the last entry in the extended single linked list
 *
 * \param[in]       ptr: Pointer to the extended list structure
 * \param[in]       type: Type of the container structure
 * \param[in]       member: Name of the single list member in the container
 *                  structure
 * \return          Pointer to the last container structure
 */
#define single_list_ex_tail_entry(ptr, type, member)                           \
    single_list_entry((ptr)->tail, type, member)

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SINGLE_LIST_EX_H__ */

/* ----------------------------- end of file -------------------------------- */
