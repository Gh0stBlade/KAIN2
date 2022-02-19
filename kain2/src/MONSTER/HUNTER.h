#ifndef __HUNTER_H
#define __HUNTER_H

extern void FX_MakeHitFlame(_Position* pos, __int16 a2, int a3, int a4, int a5);
extern void HUNTER_InitFlamethrow(struct _Instance* instance);
extern int HUNTER_Flamethrow(struct _Instance* instance, int damage, int a3, int segment);
extern void HUNTER_Init(struct _Instance* instance);
extern void HUNTER_CleanUp(struct _Instance* instance);
extern void HUNTER_ProjectileEntry(struct _Instance* instance);
extern void HUNTER_Projectile(struct _Instance* instance);

#endif
