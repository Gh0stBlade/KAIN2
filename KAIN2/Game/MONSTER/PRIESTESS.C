/* ========================================== */
/* Unused code from the PC version, it's from */
/* the prototype priestess and barely does a  */
/* thing in actuality.                        */
/* ========================================== */
#include <stdlib.h>
#include "Game/CORE.H"

void PRIESTS_Init(struct _Instance* instance)
{
	UNIMPLEMENTED();
}
void  PRIESTS_CleanUp(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

u_long  PRIESTS_Query(struct _Instance* instance, unsigned long data)
{
	UNIMPLEMENTED();
	return 0;
}

void  PRIESTS_Message(struct _Instance* instance, unsigned int message, unsigned int data)
{
	UNIMPLEMENTED();
}

void  PRIESTS_IdleEntry(struct _Instance* instance)
{
	UNIMPLEMENTED();
}
// TODO: fill me
void  PRIESTS_Idle(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

void  PRIESTS_PursueEntry(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

void  PRIESTS_Pursue(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

void  PRIESTS_FleeEntry(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

void  PRIESTS_Flee(struct _Instance* instance)
{
	UNIMPLEMENTED();
}

// ------------------ unlinked code ------------------
struct _Instance*  PRIESTS_InstanceToPossess(struct _Instance* instance)
{
	UNIMPLEMENTED();
	return NULL;
}

void  PRIESTS_DoAttackAnim(_Instance* instance, int a2)
{
	UNIMPLEMENTED();
}

extern void HUMAN_DeadEntry(struct _Instance* instance);
extern void HUMAN_Dead(struct _Instance* instance);