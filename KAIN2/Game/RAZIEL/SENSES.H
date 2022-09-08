#ifndef SENSES_H
#define SENSES_H

#include "PLAYER.H"

extern int SetEngagedInstance(struct __Player *player, struct evCollideInstanceStatsData *Ptr, unsigned long Attribute); // 0x800A3394

extern void UpdateEngagementList(struct evCollideInstanceStatsData *Ptr, struct __Player *player); // 0x800A342C

extern long UpdateAutoFaceAngle(struct _Instance *player, struct _Instance *instance); // 0x800A4588


#endif
