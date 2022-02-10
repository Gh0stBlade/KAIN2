#ifndef VWRAITH_H
#define VWRAITH_H


extern void VWRAITH_MoveVertical(struct _Instance *instance, long targetZ, int velocity); // 0x8008E8F4

extern void VWRAITH_Init(struct _Instance *instance); // 0x8008E94C

extern int VWRAITH_ShouldISwoop(struct _Instance *instance); // 0x8008EA00

extern void VWRAITH_PursueEntry(struct _Instance *instance); // 0x8008EA84

extern void VWRAITH_Pursue(struct _Instance *instance); // 0x8008EB1C

extern void VWRAITH_VerticalMove(struct _Instance *instance); // 0x8008ECEC

extern void VWRAITH_CombatEntry(struct _Instance *instance); // 0x8008ED70

extern void VWRAITH_Combat(struct _Instance *instance); // 0x8008ED90

extern void VWRAITH_EmbraceEntry(struct _Instance *instance); // 0x8008EDBC

extern void VWRAITH_Embrace(struct _Instance *instance); // 0x8008EDDC


#endif
