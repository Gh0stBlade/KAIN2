#ifndef ANMDECMP_H
#define ANMDECMP_H

#include "gex2.h"
#include "ANIMG2.H"

extern void _G2Anim_DecompressChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status); // 0x800900F4
extern void _G2Anim_DecompressChannel_Linear(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status); // 0x80090220
extern void _G2Anim_InitializeChannel_AdaptiveDelta(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status); // 0x800902B4
extern void _G2Anim_InitializeChannel_Linear(struct _G2AnimDecompressChannelInfo_Type *dcInfo, struct _G2AnimChanStatus_Type *status); // 0x800902EC

#endif
