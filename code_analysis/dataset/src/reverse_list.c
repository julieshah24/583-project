#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

static struct ListNode* reverseBetween(struct ListNode* head, int m, int n)
{
    int i;
    struct ListNode dummy;
    struct ListNode *prev = &dummy;
    prev->next = head;

    for (i = 1; i < m; i++) {
        prev = prev->next;
    }

    struct ListNode *p = prev->next;
    for (i = m; i < n; i++) {
        struct ListNode *q = p->next;
        /* deletion */
        p->next = q->next;
        /* insertion */
        q->next = prev->next;
        prev->next = q;
    }

    return dummy.next;
}

int main(int argc, char **argv)
{
    // if (argc < 3) {
    //     fprintf(stderr, "Usage: ./test m n 1 2 3...\n");
    //     exit(-1);
    // }
    int i, count = 2;
    struct ListNode dummy;
    struct ListNode *prev = &dummy;
    struct ListNode *p;
    for (i = 0; i < count; i++) {
        p = malloc(sizeof(*p));
        p->val = i;
        p->next = NULL;
        prev->next = p;
        prev = p;
    }
    int m = 1;
    int n = 2;
    struct ListNode *head = reverseBetween(dummy.next, m, n);
    for (p = head; p != NULL; p = p->next) {
        printf("%d ", p->val);
    }
    printf("\n");
    return 0;
}
