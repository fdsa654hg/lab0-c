#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "harness.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *cur = q->head;
    while (q->head) {
        q->head = q->head->next;
        cur->next = NULL;
        free(cur->value);
        free(cur);
        cur = q->head;
    }

    free(q);
}


/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *new_head;
    new_head = malloc(sizeof(list_ele_t));
    if (!new_head)
        return false;

    new_head->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_head->value) {
        free(new_head);
        return false;
    }

    q->size += 1;

    snprintf(new_head->value, (size_t) strlen(s) + 1, "%s", s);
    new_head->next = q->head;
    q->head = new_head;

    if (!q->tail) {
        list_ele_t **tmp = &q->head;
        while ((*tmp)->next)
            tmp = &(*tmp)->next;
        q->tail = *tmp;
        q->tail->next = NULL;
    }
    return true;
}



/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *new_tail;
    new_tail = malloc(sizeof(list_ele_t));
    if (!new_tail)
        return false;

    new_tail->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_tail->value) {
        free(new_tail);
        return false;
    }

    q->size += 1;

    snprintf(new_tail->value, (size_t) strlen(s) + 1, "%s", s);
    if (!q->tail) {
        q->tail = q->head = new_tail;
        q->tail->next = q->head->next = NULL;
        return true;
    }

    q->tail->next = new_tail;
    q->tail = new_tail;
    q->tail->next = NULL;

    return true;
}


/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    if (sp)
        snprintf(sp, bufsize, "%s", q->head->value);

    q->size -= 1;

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;

    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head)
        return 0;

    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head)
        return;

    list_ele_t *old_head = q->head;
    list_ele_t *prev = q->head, *next, *cur = q->head->next;
    q->head->next = NULL;

    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    q->head = prev;
    q->tail = old_head;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head)
        return;

    merge_sort(&q->head);
    while ((q->tail->next))
        q->tail = q->tail->next;
}

void merge_sort(list_ele_t **head)
{
    if (!(*head) || !((*head)->next))
        return;

    list_ele_t *l1 = *head;
    list_ele_t *l2 = (*head)->next;

    while (l2 && l2->next) {
        l1 = l1->next;
        l2 = l2->next->next;
    }
    l2 = l1->next;
    l1->next = NULL;
    l1 = *head;

    merge_sort(&l1);
    merge_sort(&l2);

    list_ele_t **tmp = head;

    while (l1 && l2) {
        if (strncmp(l1->value, l2->value, strlen(l2->value)) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    if (l1)
        *tmp = l1;
    else
        *tmp = l2;
}