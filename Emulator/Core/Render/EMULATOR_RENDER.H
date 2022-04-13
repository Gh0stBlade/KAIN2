#ifndef EMULATOR_RENDER_H
#define EMULATOR_RENDER_H

#include "EMULATOR.H"

struct VertexBufferSplit
{
	TextureID      textureId;
	unsigned short vIndex;
	unsigned short vCount;
	BlendMode      blendMode;
	TexFormat      texFormat;
};

extern void Emulator_ClearVBO();
extern void Emulator_ResetPolyState();
extern void Emulator_AggregatePTAGsToSplits(u_long* p, bool singlePrimitive);
extern void Emulator_DrawSplit(const VertexBufferSplit& split);
extern void Emulator_DrawAggregatedSplits();

#endif