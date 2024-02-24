#include "CORE.H"
#include "LIST.H"

void LIST_InsertFunc(struct NodeType *list, struct NodeType *node) //Matching - 99.55%
{
	node->prev = list;

	node->next = list->next;

	if (list->next != NULL)
	{
		list->prev = node;
	}

	list->next = node;
}

void LIST_DeleteFunc(struct NodeType* node)//Matching - 100%
{
    if (node->prev)
    {
        if (node->next)
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = NULL;
            node->prev = NULL;
            return;
        }
    }
    else if (node->next)
    {
        node->next->prev = NULL;
        node->next = NULL;
        node->prev = NULL;

        return;
    }
    if (node->prev)
    {
        node->prev->next = NULL;
        node->next = NULL;
    }
    else
    {
        node->next = NULL;
    }
    node->prev = NULL;
}

struct NodeType* LIST_GetFunc(struct NodeType* list)  // Matching - 100%
{
    struct NodeType* temp;  // not from SYMDUMP

    temp = list->next;

    if (temp != NULL)
    {
        LIST_DeleteFunc(temp);

        return temp;
    }

    return NULL;
}