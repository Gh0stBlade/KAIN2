#ifndef HEALTH_H
#define HEALTH_H


extern void InitHealthSystem(); // 0x800A45C8

extern void GainHealth(int data); // 0x800A4668

extern void LoseHealth(int amount); // 0x800A46F4

extern void DrainHealth(int amount); // 0x800A47F0

extern void BumpUpHealth(); // 0x800A490C

extern int GetMaxHealth(); // 0x800A4950

extern void ProcessHealth(struct _Instance *instance); // 0x800A49A0

extern int HealthCheckForLowHealth(); // 0x800A4E4C

extern void DrainMana(int amount); // 0x800A4EB8

extern void SetMana(int amount); // 0x800A4EFC

extern void HealthInstantDeath(struct _Instance *instance); // 0x800A4F24

extern void RAZIEL_DebugHealthSetScale(long healthScale); // 0x800A4FA4

extern void RAZIEL_DebugManaSetMax(long manaScale); // 0x800A4FFC

extern void RAZIEL_DebugHealthFillUp(); // 0x800A5024

extern void RAZIEL_DebugManaFillUp(); // 0x800A5084

extern void razPlayUnderworldSounds(struct _Instance *instance); // 0x800A50A4


#endif
