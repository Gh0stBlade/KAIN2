#ifndef LOCALSTR_H
#define LOCALSTR_H


extern enum language_t localstr_get_language(); // 0x800B7978

extern void localstr_set_language(enum language_t lang); // 0x800B7984

extern char * localstr_get(enum localstr_t id); // 0x800B7A14


#endif
