#ifndef LIST_H
#define LIST_H

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern void LIST_InsertFunc(struct NodeType *list, struct NodeType *node); // 0x8004F568
extern void LIST_DeleteFunc(struct NodeType *node); // 0x8004F594
extern struct NodeType * LIST_GetFunc(struct NodeType *list); // 0x8004F600

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif
