#ifndef MONMSG_H
#define MONMSG_H

#include "MONSTER.H"

struct evMonsterAlarmData 
{
	struct _Instance* sender; // size=668, offset=0
	struct _Position position; // size=6, offset=4
	short type; // size=0, offset=10
};

extern int MON_GroundMoveQueueHandler(struct _Instance *instance); // 0x800847B8

extern void MON_PupateMessageHandler(struct _Instance *instance, struct __Event *message); // 0x80084840

extern void MON_IdleMessageHandler(struct _Instance *instance, struct __Event *message); // 0x80084A84

extern void MON_DefaultMessageHandler(struct _Instance *instance, struct __Event *message); // 0x80084BC8

extern void MON_PupateQueueHandler(struct _Instance *instance); // 0x800859C0

extern void MON_IdleQueueHandler(struct _Instance *instance); // 0x80085A0C

extern void MON_DefaultQueueHandler(struct _Instance *instance); // 0x80085A58


#endif
