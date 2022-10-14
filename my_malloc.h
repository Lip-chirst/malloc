#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#define BLOCK_HEADER_SIZE sizeof(block_meta)
typedef struct block_header {
    ssize_t size;
    int  is_free;
    struct block_header *next;
} block_meta;

block_meta* list_head, *list_tail;

block_meta* find_free_block(ssize_t size) {
    block_meta *p = list_head;
    while (p) {
        if (p->size >= size && p->is_free) {
            return p;
        } else {
            p = p->next;
        }
    }

    return NULL;
}

block_meta* request_for_block(ssize_t size) {
    void* p = sbrk(size + BLOCK_HEADER_SIZE);
    if (p == (void *)-1) {
        return NULL;
    }
    block_meta *tail = p;
    tail->is_free = 1;
    tail->size = size;
    tail->next = NULL;
    if (list_head == NULL) {
        list_head = tail;
        list_tail = tail;
    } else {
        list_tail->next = tail;
        list_tail = tail;
    }

    return tail;
}

void* my_malloc(ssize_t size) {
    block_meta * p = find_free_block(size);
    if (p) {
        p->is_free = 0;
        return p;
    } else {
        p = request_for_block(size);
        if (p) {
            p->is_free = 0;
            return p;
        }

        return NULL;
    }
}

void my_free(void * p) {
    block_meta *cur = (block_meta*)p - 1;
    cur->is_free = 1;
}
