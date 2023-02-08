#pragma once

struct _GlyphTuneData 
{
	short glyph_size;
	short glyph_darkness;
	char glyph_costs[8];
	short glyph_range[8];
	long color_array[28];
};