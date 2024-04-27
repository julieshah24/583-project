#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Definition for singly-linked list. */
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2)
{
    int carry = 0;
    struct ListNode dummy;
    struct ListNode *p = l1, *prev = &dummy;

    dummy.next = p;
    while (l1 != NULL || l2 != NULL) {
        int sum = 0;

        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }

        if (l2 != NULL) {
            if (p == NULL) {
                /* l2 longer than l1 */
                prev->next = l2;
                p = l2;
            }
            sum += l2->val;
            l2 = l2->next;
        }

        sum += carry;
        carry = sum / 10;
        p->val = sum % 10;
        prev = p;
        p = p->next;
    }

    if (carry) {
        p = malloc(sizeof(*p));
        p->val = carry;
        p->next = NULL;
        prev->next = p;
    }
    
    return dummy.next;
}

static struct ListNode *node_build(const char *digits)
{
    struct ListNode *res, *p, *prev;
    int first = 1;
    int len = strlen(digits);
    const char *c = digits + len - 1;
    prev = NULL;
    while (len-- > 0) {
        p = malloc(sizeof(*p));
        if (first) {
            first = 0;
            res = p;
        }
        p->val = *c-- - '0';
        p->next = NULL;
        if (prev != NULL) {
            prev->next = p;
        }
        prev = p;
    }

    return res;
}

static void show(struct ListNode *ln)
{
    int sum = 0, factor = 1;
    while (ln != NULL) {
        sum += ln->val * factor;
        factor *= 10;
        ln = ln->next;
    }
    printf("%d\n", sum);
}

int main(int argc, char **argv)
{
    // test case 1
    struct ListNode *l1 = node_build("234\n");
    struct ListNode *l2 = node_build("564\n");
    struct ListNode *res = addTwoNumbers(l1, l2);
    show(l1);
    show(l2);
    show(res);

    struct ListNode *l3 = node_build("0\n");
    struct ListNode *l4 = node_build("0\n");
    struct ListNode *res2 = addTwoNumbers(l3, l4);
    show(l4);
    show(l4);
    show(res);

    struct ListNode *l5 = node_build("9999999\n");
    struct ListNode *l6 = node_build("9999\n");
    struct ListNode *res3 = addTwoNumbers(l5, l6);
    show(l5);
    show(l6);
    show(res);
    return 0;
}
