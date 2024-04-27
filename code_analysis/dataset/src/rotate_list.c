#include <stdio.h>
#include <stdlib.h>


struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* rotateRight(struct ListNode* head, int k)
{
    if (head == NULL) {
        return head;
    }

    int len = 0;
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *tail = &dummy;
    while (tail->next != NULL) {
        tail = tail->next;
        len++;
    }

    struct ListNode *prev = &dummy;
    struct ListNode *p = head;
    len = len - (k % len);
    while (len-- > 0) {
        prev = p;
        p = p->next;
    }

    if (p != NULL) {
        /* deletion */
        prev->next = NULL;
        /* insertion */
        tail->next = head;
        head = p;
    }

    return head;
}

int main(int argc, char **argv)
{
    // if (argc < 2) {
    //     fprintf(stderr, "Usage: ./test k n1 n2...\n");
    //     exit(-1);
    // }

    int i;
    struct ListNode *p, *prev, dummy, *list;
    char arg_v[] = {'1','2','3','4','5'};
    dummy.next = NULL;
    prev = &dummy;
    for (i = 2; i < argc; i++) {
        p = malloc(sizeof(*p));
        char temp_str[2] = {arg_v[i], '\0'};  // Create a temporary string
        int n = atoi(temp_str);
        p->val = n;
        p->next = NULL;
        prev->next = p;
        prev = p;
    }

    list = rotateRight(dummy.next, 3);
    for (p = list; p != NULL; p = p->next) {
        printf("%d ", p->val);
    }
    putchar('\n');

    return 0;
}
