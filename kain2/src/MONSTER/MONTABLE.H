#ifndef MONTABLE_H
#define MONTABLE_H

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern void MONTABLE_SetupTablePointer(struct Object *object); // 0x8008CE88
extern struct _MonsterState * MONTABLE_GetStateFuncs(struct _Instance *instance, int state); // 0x8008CED8
extern TDRFuncPtr_MONTABLE_GetDamageEffectFunc MONTABLE_GetDamageEffectFunc(struct _Instance *instance); // 0x8008CF48
extern TDRFuncPtr_MONTABLE_GetInitFunc MONTABLE_GetInitFunc(struct _Instance *instance); // 0x8008CF7C
extern TDRFuncPtr_MONTABLE_GetCleanUpFunc MONTABLE_GetCleanUpFunc(struct _Instance *instance); // 0x8008CFB0
extern void MONTABLE_SetQueryFunc(struct _Instance *instance); // 0x8008CFE4
extern void MONTABLE_SetMessageFunc(struct _Instance *instance); // 0x8008D018

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
