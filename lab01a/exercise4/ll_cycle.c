#include <stddef.h>
#include "ll_cycle.h"
#include <stdio.h>

int ll_has_cycle(node *head)
{
    /* TODO: Implement ll_has_cycle */
    if (!head)
        return 0;
    node *slow = head;
    node *fast = head;
    int counter = 0;
    while (fast->next != NULL)
    {
        fast = fast->next;
        if (slow == fast)
            return 1;
        if (counter++%2)
            slow = slow->next;
    }
    return 0;
}
