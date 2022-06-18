#include "CORE.H"
#include "DEBUG.H"
#include "GAMELOOP.H"
#include "MENU/MENU.H"
#include "PSX/MAIN.H"
#include "FONT.H"
#include "SOUND.H"

#ifdef PC_VERSION
#pragma warning(disable: 4101)
#endif

#if defined(PSXPC_VERSION)
#include <assert.h>
#endif

void DEBUG_FillUpHealth(long* var);
void DEBUG_FogLoad();
void DEBUG_SetViewVram();
void DEBUG_ReloadCurrentLevel();
void DEBUG_LevelSelectNew();

struct debug_dispatch_t debug_dispatch_table[5] = { 
	{ DEBUG_LINE_TYPE_BIT, handle_line_type_bit },
	{ DEBUG_LINE_TYPE_LONG, handle_line_type_long },
	{ DEBUG_LINE_TYPE_ACTION, handle_line_type_action },
	{ DEBUG_LINE_TYPE_ACTION_WITH_LINE, handle_line_type_action_with_line },
	{ DEBUG_LINE_TYPE_MENU, handle_line_type_menu }
};

char* the_format_string = mainMenu[0].text;
int cem_x_base = 40; // offset 0x800d014c
int cem_y_base = 20; // offset 0x800d0150
int cem_cursor_width = 20; // offset 0x800d0154
int cem_line_width = 240; // offset 0x800d0158
int cem_line_leading; // offset 0x800d015c
int cem_item_leading; // offset 0x800d0160

unsigned long debugRazielFlags1;
unsigned long debugRazielFlags2;
unsigned long debugRazielFlags3;

struct DebugMenuLine debugHealthSystemMenu[7];
struct DebugMenuLine cameraMenu[1];
struct DebugMenuLine fogMenu[1];
struct DebugMenuLine debugSoundMenu[1];

char pauseFormatString[20];

struct DebugMenuLine standardMenu[12] =
{
	{
		DEBUG_LINE_TYPE_ACTION,
		0,
		0,
		"FILL ER UP",
		(long*)&DEBUG_FillUpHealth,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"keep info between loads",
		&gameTrackerX.streamFlags,
		0x200000
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"LEVELS",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SHORT SHORT STATS",
		&gameTrackerX.debugFlags,
		0x4000000
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"RAZIEL MENU...",
		(long*)&debugRazielMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"CAMERA MENU...",
		(long*)&cameraMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"FOG MENU...",
		(long*)&fogMenu,
		(long)DEBUG_FogLoad
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0,
		0,
		"VIEW VRAM",
		(long*)DEBUG_SetViewVram,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Show Warp Gate Info",
		&gameTrackerX.debugFlags2,
		0x1000000
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Activate All WarpGates",
		&gameTrackerX.streamFlags,
		0x400000
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"MUSIC ON",
		&gameTrackerX.debugFlags2,
		0x1000
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine debugRazielMenu[8] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"HEALTH SYSTEM    ...",
		(long*)&debugHealthSystemMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Special Abilities...",
		(long*)&debugSpecialAbilitiesMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Forged  Abilities...",
		(long*)&debugForgedAbilitiesMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Glyph   Abilities...",
		(long*)&debugGlyphAbilitiesMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"IMBUE SOUL REAVER...",
		(long*)&debugImbueSoulReaverMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SHIFT ANY TIME",
		(long*)&debugRazielFlags1,
		0x50
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine debugSpecialAbilitiesMenu[10] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"RAZIEL MENU...",
		(long*)&debugRazielMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"ALL",
		(long*)&debugRazielFlags1,
		0x3f
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"PASS THROUGH BARRIERS",
		(long*)&debugRazielFlags1,
		0x1
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"WALL CRAWLING",
		(long*)&debugRazielFlags1,
		0xb
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"FORCE",
		(long*)&debugRazielFlags1,
		0xf
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SOUL REAVER",
		(long*)&debugRazielFlags1,
		0x9
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SWIM",
		(long*)&debugRazielFlags1,
		0x1f
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"CONSTRICT",
		(long*)&debugRazielFlags1,
		0x3f
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine debugForgedAbilitiesMenu[7] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"RAZIEL MENU...",
		(long*)&debugRazielMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"ALL",
		(long*)&debugRazielFlags1,
		0x3fc00
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)&debugRazielFlags1,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)&debugRazielFlags1,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)&debugRazielFlags1,
		0x8000
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine debugGlyphAbilitiesMenu[11] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"RAZIEL MENU...",
		(long*)&debugRazielMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"ALL",
		(long*)&debugRazielFlags1,
		0x3fc00
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)&debugRazielFlags1,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)&debugRazielFlags1,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)&debugRazielFlags1,
		0x8000
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine debugImbueSoulReaverMenu[7] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"RAZIEL MENU...",
		(long*)&debugRazielMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Hold Soul Reaver",
		(long*)&debugRazielFlags1,
		0x8
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)&debugRazielFlags2,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)&debugRazielFlags2,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)&debugRazielFlags2,
		0x8000
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine levelSelectMenu[14] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"MAIN MENU...",
		(long*)&standardMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0,
		0,
		"RELOAD CURRENT LEVEL",
		(long*)&DEBUG_ReloadCurrentLevel,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Aluka...",
		(long*)&AlukaMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Ash Village...",
		(long*)&AshVillageMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Oracle's Cave...",
		(long*)&OracleMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Pillars...",
		(long*)&PillarsMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Silenced Cathedral...",
		(long*)&SilencedCathedralMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Skinner...",
		(long*)&SkinnerMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Stone Glyph...",
		(long*)&StoneMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Sunlight Glyph... ",
		(long*)&SunLightMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Tomb Of Seven...",
		(long*)&TombMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Water Glyph...",
		(long*)&WaterMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"More Areas..",
		(long*)&level2SelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine level2SelectMenu[11] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"City...",
		(long*)&CityMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Cliff...",
		(long*)&CliffMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Under...",
		(long*)&UnderMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Morlock...",
		(long*)&MorlockMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"HubA...",
		(long*)&HubAMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"HubB...",
		(long*)&HubBMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Training...",
		(long*)&TrainingMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Dark Eden...",
		(long*)&DarkEdenMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Boss Areas...",
		(long*)&BossAreasMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine AlukaMenu[10] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"ALUKA 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x4,
		0,
		"ALUKA 4",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x6,
		0,
		"ALUKA 6",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x8,
		0,
		"ALUKA 8",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xc,
		0,
		"ALUKA 12",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x13,
		0,
		"ALUKA 19",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1b,
		0,
		"ALUKA 27",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1d,
		0,
		"ALUKA 29",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine AshVillageMenu[7] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"NIGHTA 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2,
		0,
		"NIGHTA 2",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x4,
		0,
		"NIGHTA 4",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"NIGHTB 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x5,
		0,
		"NIGHTB 5",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine OracleMenu[10] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"ORACLE 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x5,
		0,
		"ORACLE 5",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xa,
		0,
		"ORACLE 10",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xd,
		0,
		"ORACLE 13",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xf,
		0,
		"ORACLE 15",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x11,
		0,
		"ORACLE 17",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x12,
		0,
		"ORACLE 18",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x16,
		0,
		"ORACLE 22",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine PillarsMenu[6] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"Pillars 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x4,
		0,
		"Pillars 4",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x9,
		0,
		"Pillars 9",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"Tompil 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine SilencedCathedralMenu[11] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Cathy 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"Cathy 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x5,
		0,
		"Cathy 5",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x8,
		0,
		"Cathy 8",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x13,
		0,
		"Cathy 19",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2a,
		0,
		"Cathy 42",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2f,
		0,
		"Cathy 47",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x31,
		0,
		"Cathy 49",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x44,
		0,
		"Cathy 68",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine SkinnerMenu[7] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"OUT 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x4,
		0,
		"OUT 4",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x7,
		0,
		"SKINNR 7",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"SKINNR 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x9,
		0,
		"SKINNR 9",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine StoneMenu[5] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"STONE 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x5,
		0,
		"STONE 5",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xa,
		0,
		"STONE 10",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine SunLightMenu[7] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"FILL 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"INTVALY 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"SUNRM 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"PISTON 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"HTORM 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine TombMenu[6] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2,
		0,
		"BOSS 2",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"TOMB 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"CONECTC 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Add 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine WaterMenu[4] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"TOWER 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x7,
		0,
		"TOWER 7",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine CityMenu[5] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2,
		0,
		"CITY 2",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x9,
		0,
		"CITY 9",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xe,
		0,
		"CITY 14",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine CliffMenu[3] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"CLIFF 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine UnderMenu[3] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"UNDER 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine MorlockMenu[3] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Mrlock 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine HubAMenu[5] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Huba 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x6,
		0,
		"Huba 6",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xa,
		0,
		"Huba 10",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine HubBMenu[3] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Hubb 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine TrainingMenu[5] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Train 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x7,
		0,
		"Train 7",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x9,
		0,
		"Train 9",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine DarkEdenMenu[3] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"AREA MENU...",
		(long*)&levelSelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"Fire 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine BossAreasMenu[9] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"More Areas Menu...",
		(long*)&level2SelectMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Anterooms...",
		(long*)&AnteRoomsMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x9,
		0,
		"Skinnr 9",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x37,
		0,
		"Cathy 55",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Pillars 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x6,
		0,
		"Aluka 6",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x3,
		0,
		"Nightb 3",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x1,
		0,
		"Chrono 1",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

struct DebugMenuLine AnteRoomsMenu[8] =
{
	{
		DEBUG_LINE_TYPE_MENU,
		0,
		0,
		"Boss Menu...",
		(long*)&BossAreasMenu,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0xc,
		0,
		"Skinnr 12",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x36,
		0,
		"Cathy 54",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2,
		0,
		"Pillars 2",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2e,
		0,
		"Aluka 46",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x8,
		0,
		"Nightb 8",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ACTION,
		0x2,
		0,
		"Chrono 2",
		(long*)&DEBUG_LevelSelectNew,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		&gameTrackerX.debugFlags,
		0
	}
};

long debugMenuChoice;
struct DebugMenuLine* currentMenu; // offset 0x800CDB3C
struct DebugMenuLine* the_previous_menu;

struct DebugMenuLine pauseMenu[7] = {
	{
		DEBUG_LINE_TYPE_FORMAT,
		0,
		0,
		(char*)&pauseFormatString[0],
		NULL,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
		{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
	{
		DEBUG_LINE_TYPE_ENDLIST,
		0,
		0,
		(char*)0x800cf5f8, // Fix me
		NULL,
		0
	},
};

// autogenerated function stub: 
// void /*$ra*/ DEBUG_UpdateHealth(long *var /*$a0*/)
void DEBUG_UpdateHealth(long *var)
{ // line 1261, offset 0x80012ec0
	/* begin block 1 */
		// Start line: 2482
	/* end block 1 */
	// End Line: 2483

	/* begin block 2 */
		// Start line: 2522
	/* end block 2 */
	// End Line: 2523

	/* begin block 3 */
		// Start line: 2483
	/* end block 3 */
	// End Line: 2484
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_UpdateMana(long *var /*$a0*/)
void DEBUG_UpdateMana(long *var)
{ // line 1266, offset 0x80012ee4
	/* begin block 1 */
		// Start line: 2493
	/* end block 1 */
	// End Line: 2494

	/* begin block 2 */
		// Start line: 2494
	/* end block 2 */
	// End Line: 2495
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_FillUpHealth(long *var /*$a0*/)
void DEBUG_FillUpHealth(long *var)
{ // line 1275, offset 0x80012f08
	/* begin block 1 */
		// Start line: 1276
		// Start offset: 0x80012F08
	/* end block 1 */
	// End offset: 0x80012F08
	// End Line: 1276

	/* begin block 2 */
		// Start line: 2512
	/* end block 2 */
	// End Line: 2513
	UNIMPLEMENTED();
}


void DEBUG_FogLoad(void)
{
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ process_cheat_codes(struct GameTracker *gt /*$a0*/, long *ctrl /*$a1*/)
void process_cheat_codes(struct GameTracker *gt, long *ctrl)
{ // line 1474, offset 0x80012f30
	/* begin block 1 */
		// Start line: 1475
		// Start offset: 0x80012F30
		// Variables:
			int i; // $s0
			struct GameCheat *cheat; // $a0
			unsigned long padPress; // $s2
			unsigned long padOn; // $s1

		/* begin block 1.1 */
			// Start line: 1487
			// Start offset: 0x80012F70
			// Variables:
				int cheatTriggered; // $s3

			/* begin block 1.1.1 */
				// Start line: 1490
				// Start offset: 0x80012F84
				// Variables:
					unsigned long keyMask; // $v1

				/* begin block 1.1.1.1 */
					// Start line: 1510
					// Start offset: 0x80013010
				/* end block 1.1.1.1 */
				// End offset: 0x800131AC
				// End Line: 1584
			/* end block 1.1.1 */
			// End offset: 0x800131AC
			// End Line: 1586
		/* end block 1.1 */
		// End offset: 0x800131D4
		// End Line: 1592
	/* end block 1 */
	// End offset: 0x800131D4
	// End Line: 1594

	/* begin block 2 */
		// Start line: 2948
	/* end block 2 */
	// End Line: 2949
					UNIMPLEMENTED();
}

void DEBUG_Process(struct GameTracker *gameTracker)
{ 
	long *controlCommand;
	long oldFlags;
	long oldFlags2;
	
	oldFlags = gameTracker->debugFlags;
	oldFlags2 = gameTracker->debugFlags2;
	controlCommand = &gameTracker->controlCommand[0][0];

	if (gameTracker->gameMode == 0)
	{
		if ((oldFlags & 0x8))
		{
			DEBUG_ProcessSecondController(gameTracker);
		}

		if (gameTracker->cheatMode == 1)
		{
			DEBUG_ProcessCheat(gameTracker);
		}
	}
	else if (gameTracker->gameMode == 4)
	{
		DEBUG_Menu(gameTracker);

		if ((gameTracker->debugFlags2 & 0x40000))
		{
			if (!(oldFlags2 & 0x40000))
			{
				gameTracker->debugFlags |= 0x8;
			}

			if ((gameTracker->debugFlags2 & 0x40000))
			{
				if ((gameTracker->debugFlags & 0x8) && !(oldFlags & 0x8))
				{
#if 0//Enable me when camera is not reporting incomplete type.
					theCamera.core.debugPos = theCamera.core.Position;
					theCamera.core.debugRot = theCamera.core.Rotation;
#endif
				}
			}
		}
		
		if ((oldFlags2 & 0x40000))
		{
			gameTracker->debugFlags &= -9;
		}

		if ((gameTracker->debugFlags & 0x8) && !(oldFlags & 0x8))
		{
#if 0//Enable me when camera is not reporting incomplete type.
			theCamera.core.debugPos = theCamera.core.Position;
			theCamera.core.debugRot = theCamera.core.Rotation;
#endif
		}
	}
	else if (gameTracker->gameMode == 6)//v0 = 7
	{
		process_cheat_codes(gameTracker, controlCommand);
		DEBUG_Menu(gameTracker);
	}
	else if (gameTracker->gameMode == 7)
	{
		DEBUG_ViewVram(gameTracker);
	}

	if ((gameTracker->debugFlags & 0x4000) && (controlCommand[1] & 0x400))
	{
		DEBUG_CaptureScreen(gameTracker);
	}
}

void DEBUG_Draw(struct GameTracker *gameTracker, unsigned long **ot)
{
	if (gameTracker->gameMode == 0 || gameTracker->cheatMode == 1 || gameTracker->gameMode == 4)
	{
		DEBUG_DisplayStatus(gameTracker);
	}
}


// autogenerated function stub: 
// long /*$ra*/ DEBUG_MenuCountLength(struct DebugMenuLine *menu /*$a0*/)
long DEBUG_MenuCountLength(struct DebugMenuLine *menu)
{ // line 1699, offset 0x800133ec
	/* begin block 1 */
		// Start line: 1701
		// Start offset: 0x800133EC
		// Variables:
			int length; // $v1
			struct DebugMenuLine *curLine; // $a0
	/* end block 1 */
	// End offset: 0x80013410
	// End Line: 1710

	/* begin block 2 */
		// Start line: 3260
	/* end block 2 */
	// End Line: 3261

	/* begin block 3 */
		// Start line: 3261
	/* end block 3 */
	// End Line: 3262

	/* begin block 4 */
		// Start line: 3263
	/* end block 4 */
	// End Line: 3264
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_ExitMenus()
void DEBUG_ExitMenus()
{ // line 1713, offset 0x80013418
	/* begin block 1 */
		// Start line: 1714
		// Start offset: 0x80013418
	/* end block 1 */
	// End offset: 0x800134A0
	// End Line: 1743

	/* begin block 2 */
		// Start line: 3288
	/* end block 2 */
	// End Line: 3289

	/* begin block 3 */
		// Start line: 3293
	/* end block 3 */
	// End Line: 3294
	UNIMPLEMENTED();
}

struct DebugMenuLine* get_last_menu_line(struct DebugMenuLine *line)
{
	if (line->type != DEBUG_LINE_TYPE_ENDLIST)
	{
		do
		{
		
		} while ((++line)->type != DEBUG_LINE_TYPE_ENDLIST);
	
		line--;
	}

	return line;
}

int num_menu_items(struct DebugMenuLine *menu)
{
	int nitems;

	nitems = 0;

	while (menu->type != DEBUG_LINE_TYPE_ENDLIST)
	{
		nitems++;
		menu++;
	}

	return nitems;
}

void maybe_change_menu_choice(struct GameTracker *gt, struct DebugMenuLine *menu)
{
	long* command;
	int choice;
	int nitems;
	int incr;

	command = (long*)&gt->controlCommand;
	choice = debugMenuChoice;

	nitems = num_menu_items(menu);

	if (!(command[1] & 0x1))
	{
		incr = (command[1] >> 1) & 0x1;
	}
	else
	{
		incr = -1;
	}

	if (incr != 0)
	{
		if (choice >= 0)
		{
			while(1)
			{
				choice = (choice + nitems + incr) % nitems;

				if (choice != debugMenuChoice)
				{
					if ((menu + choice)->type >= DEBUG_LINE_TYPE_FORMAT)
					{
						continue;
					}

					if (choice != debugMenuChoice)
					{
						SndPlay(5);

						debugMenuChoice = choice;
					}
				}

				break;
			}
		}
	}
}

void handle_line_type_long(struct GameTracker *gt, struct DebugMenuLine *line)
{ 
	long* command;
	int incr;

	command = &gt->controlCommand[0][0];

	if ((command[1] & 0xC))
	{
		if ((gt->controlCommand[0][0] & 0x400))
		{
			incr = 10;
		}
		else if ((gt->controlCommand[0][0] & 0x800))
		{
			incr = 100;
		}
		else if ((gt->controlCommand[0][0] & 0x800))
		{
			incr = 1000;
		}
		else
		{
			incr = 1;
		}

		if ((command[1] & 0x4))
		{
			incr = -incr;
		}

		line->var_address[0] += incr;
	}
}


// autogenerated function stub: 
// void /*$ra*/ handle_line_type_bit(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *line /*$a1*/)
void handle_line_type_bit(struct GameTracker *gt, struct DebugMenuLine *line)
{ // line 1814, offset 0x800136c4
	/* begin block 1 */
		// Start line: 1816
		// Start offset: 0x800136C4
	/* end block 1 */
	// End offset: 0x80013714
	// End Line: 1823

	/* begin block 2 */
		// Start line: 3501
	/* end block 2 */
	// End Line: 3502

	/* begin block 3 */
		// Start line: 3502
	/* end block 3 */
	// End Line: 3503

	/* begin block 4 */
		// Start line: 3504
	/* end block 4 */
	// End Line: 3505
	UNIMPLEMENTED();
}

void handle_line_type_action(struct GameTracker *gt, struct DebugMenuLine *line)
{
	int ok;
	
	if ((gt->controlCommand[0][1] & 0x80))
	{
		typedef void* (*fptr)();
		((fptr)line->var_address)();
	}
}

void handle_line_type_action_with_line(struct GameTracker *gt, struct DebugMenuLine *line)
{
	enum option_ctrl_t ctrl; // $a2
	
	if ((gt->controlCommand[0][1] & 0x80))
	{
		ctrl = option_ctrl_select;
	}
	else if ((gt->controlCommand[0][1] & 0x4))
	{
		ctrl = option_ctrl_left;
	}
	else if ((gt->controlCommand[0][1] & 0x8))
	{
		ctrl = option_ctrl_right;
	}
	else
	{
		ctrl = option_ctrl_none;
	}

	if (ctrl != option_ctrl_none)
	{
		typedef void* (*fptr)();
		((fptr)line->var_address)();
	}
}

void handle_line_type_menu(struct GameTracker *gt, struct DebugMenuLine *line)
{
	typedef int (*fptr)();
	fptr ok;

	if ((gt->controlCommand[0][1] & 0x80))
	{
		ok = (fptr)line->bit_mask;
		if (ok != 0)
		{
			ok();

		}
		
		get_last_menu_line(line)->lower = debugMenuChoice;

		the_previous_menu = currentMenu;
		
		currentMenu = (struct DebugMenuLine*)line->var_address;

		debugMenuChoice = get_last_menu_line(currentMenu)->lower;
	}
}

void process_menu_line(struct GameTracker *gt, struct DebugMenuLine *menu)
{
	struct DebugMenuLine *line;
	struct debug_dispatch_t *dispatch;
	
	line = menu + debugMenuChoice;
	
	if (line->type < DEBUG_LINE_TYPE_ENDLIST)
	{
		dispatch = &debug_dispatch_table[line->type];

		if (dispatch->fn != NULL)
		{
			dispatch->fn(gt, line);
		}
	}
}

int pre_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu)
{
	if (gt->playerInstance != NULL)
	{
		gt->playerInstance->flags |= 0x100;
	}

	return 0;
}

void post_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu)
{
	if (menu == debugSoundMenu)
	{
		SOUND_SetMusicVolume(-1);

		SOUND_SetSfxVolume(-1);

		SOUND_SetVoiceVolume(-1);

		if ((gt->debugFlags & 0x80000))
		{
			gt->sound.gVoiceOn = 1;
		}
		else
		{
			gt->sound.gVoiceOn = 0;
		}

		if ((gt->debugFlags2 & 0x1000))
		{
			gt->sound.gMusicOn = 1;
		}
		else
		{
			gt->sound.gMusicOn = 0;
		}

		if ((gt->debugFlags2 & 0x2000))
		{
			gt->sound.gSfxOn = 1;
		}
		else
		{
			gt->sound.gSfxOn = 0;
		}
	}
}

void set_debug_leading()
{
	cem_line_leading = 10;
	cem_item_leading = 12;
}


// autogenerated function stub: 
// void /*$ra*/ set_user_leading()
void set_user_leading()
{ // line 1972, offset 0x80013984
	/* begin block 1 */
		// Start line: 3819
	/* end block 1 */
	// End Line: 3820

	/* begin block 2 */
		// Start line: 3821
	/* end block 2 */
	// End Line: 3822
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// int /*$ra*/ isdigit(char c /*$a0*/)
//int isdigit(char c)
//{ // line 1992, offset 0x8001399c
//	/* begin block 1 */
//		// Start line: 3859
//	/* end block 1 */
//	// End Line: 3860
//
//	/* begin block 2 */
//		// Start line: 3860
//	/* end block 2 */
//	// End Line: 3861
//
//	return 0;
//}


// autogenerated function stub: 
// void /*$ra*/ adjust_format(char *ctrl /*$s1*/, struct debug_format_t *fmt /*$s4*/)
void adjust_format(char *ctrl, struct debug_format_t *fmt)
{ // line 1997, offset 0x800139ac
	/* begin block 1 */
		// Start line: 1998
		// Start offset: 0x800139AC

		/* begin block 1.1 */
			// Start line: 2002
			// Start offset: 0x80013A14
			// Variables:
				char *p; // $s0
				int x; // $s2
				int y; // $s3
		/* end block 1.1 */
		// End offset: 0x80013AE0
		// End Line: 2016
	/* end block 1 */
	// End offset: 0x80013B30
	// End Line: 2025

	/* begin block 2 */
		// Start line: 3869
	/* end block 2 */
	// End Line: 3870
				UNIMPLEMENTED();
}

char* find_eol(char* text)
{ 
	if (*text != 0)
	{
		do
		{
			if (*text == '\n')
			{
				break;
			}
			
			text++;

		} while (*text != 0);
	}

	return text;
}

void draw_menu_item(struct GameTracker *gt, struct debug_format_t *fmt, char *text)
{
	char* eol;
	char c;

	while (1)
	{
		eol = find_eol(text);

		c = eol[0];

#if !defined(PSXPC_VERSION)//Shouldn't be required and causes a crash!
		eol[0] = 0;
#endif

		if (fmt->is_centered != 0)
		{
			FONT_SetCursor(fmt->xpos - (FONT_GetStringWidth(text) >> 1), fmt->ypos);
		}
		else
		{
			FONT_SetCursor(fmt->xpos, fmt->ypos);
		}

		if (currentMenu->type != DEBUG_LINE_TYPE_FORMAT)
		{
			FONT_Print(text);
		}
		else
		{
			FONT_Print2(text);
		}

		if (c == 0)
		{
			break;
		}

		text = eol + 1;
		eol[0] = c;
		fmt->ypos += cem_line_leading;
	}

	fmt->ypos += cem_item_leading;
}

void draw_menu(struct GameTracker* gt, struct DebugMenuLine* menu)
{
	struct debug_format_t fmt;
	int i;
	int xpos = 0;
	int ypos = 0;

	fmt.xpos = cem_x_base;
	fmt.ypos = cem_y_base;
	fmt.is_centered = 0;

	if (menu->type == DEBUG_LINE_TYPE_FORMAT)
	{
		menu->text = the_format_string;
	}
	else
	{
		set_debug_leading();
	}

	i = 0;

	while (menu->type != DEBUG_LINE_TYPE_ENDLIST)
	{
		xpos = fmt.xpos;
		ypos = fmt.ypos;

		if (menu->type == DEBUG_LINE_TYPE_FORMAT)
		{
			adjust_format(menu->text, &fmt);
			menu++;
			i++;
			continue;
		}
		else
		{
			if (debugMenuChoice == i)
			{
				if (fmt.is_centered != 0)
				{
					FONT_SetCursor((xpos - (cem_line_width >> 1)) - cem_cursor_width, ypos);
				}
				else
				{
					FONT_SetCursor(xpos - cem_cursor_width, ypos);
				}

				FONT_Print(">");
			}

			draw_menu_item(gt, &fmt, menu->text);

			if (fmt.is_centered != 0)
			{
				FONT_SetCursor(xpos + (cem_line_width >> 1), ypos);
			}
			else
			{
				FONT_SetCursor(xpos + cem_line_width, ypos);
			}

			if (menu->type != DEBUG_LINE_TYPE_BIT)
			{
				if (menu->type == DEBUG_LINE_TYPE_LONG)
				{
					FONT_Print("%d", menu->var_address[0]);
					menu++;
				}
				else
				{
					menu++;
				}
			}
			else
			{
				if ((menu->var_address[0] & menu->bit_mask) == menu->bit_mask)
				{
					FONT_Print("YES");
					menu++;
				}
				else
				{
					FONT_Print("NO");
					menu++;
				}
			}
			i++;
		}
	}
}

void DEBUG_Menu(struct GameTracker* gt)
{
	struct DebugMenuLine* menu;
	int choice;

	menu = currentMenu;
	choice = debugMenuChoice;

	if (menu == mainMenu || menu == pauseMenu)
	{
		menu_process(gt->menu);
	}
	else
	{
		if (pre_process_functions(gt, menu) == 0)
		{
			if (menu[choice].type >= DEBUG_LINE_TYPE_FORMAT)
			{
				do
				{
					choice++;
				} while (menu[choice].type < DEBUG_LINE_TYPE_FORMAT);
			}

			draw_menu(gt, menu);
			maybe_change_menu_choice(gt, menu);

			if (debugMenuChoice == choice)
			{
				process_menu_line(gt, menu);

				if (currentMenu == menu)
				{
					post_process_functions(gt, menu);
				}
			}
		}
	}
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_DisplayStatus(struct GameTracker *gameTracker /*$s1*/)
void DEBUG_DisplayStatus(struct GameTracker *gameTracker)
{ // line 2165, offset 0x80013fac
	/* begin block 1 */
		// Start line: 2166
		// Start offset: 0x80013FAC

		/* begin block 1.1 */
			// Start line: 2425
			// Start offset: 0x8001414C
			// Variables:
				int deg; // $a0
		/* end block 1.1 */
		// End offset: 0x800141D0
		// End Line: 2438

		/* begin block 1.2 */
			// Start line: 2444
			// Start offset: 0x800141E0
			// Variables:
				long numberInQueue; // stack offset -24
		/* end block 1.2 */
		// End offset: 0x80014200
		// End Line: 2451
	/* end block 1 */
	// End offset: 0x80014200
	// End Line: 2488

	/* begin block 2 */
		// Start line: 4317
	/* end block 2 */
	// End Line: 4318
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_DrawShrinkCels(unsigned long **polyAddr /*$a0*/)
void DEBUG_DrawShrinkCels(unsigned long **polyAddr)
{ // line 3564, offset 0x80014214
	/* begin block 1 */
		// Start line: 7128
	/* end block 1 */
	// End Line: 7129

	/* begin block 2 */
		// Start line: 6157
	/* end block 2 */
	// End Line: 6158
	UNIMPLEMENTED();
}

void DEBUG_ContinueGame()
{ 
	gameTrackerX.gameFlags |= 0x40000000;
}

void DEBUG_ExitGame()
{ 
	SOUND_StopAllSound();

	gameTrackerX.levelChange = 1;
	gameTrackerX.gameMode = 0;
	gameTrackerX.levelDone = 1;
	gameTrackerX.gameFlags |= 0x1;
}

void DEBUG_ReloadCurrentLevel()
{
	UNIMPLEMENTED();
}

void DEBUG_LevelSelectNew()
{
	UNIMPLEMENTED();
}

void DEBUG_SetViewVram()
{
	gameTrackerX.gameMode = 7;
	return;
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_EndViewVram(struct GameTracker *gameTracker /*$a0*/)
void DEBUG_EndViewVram(struct GameTracker *gameTracker)
{ // line 3940, offset 0x80014270
	/* begin block 1 */
		// Start line: 7880
	/* end block 1 */
	// End Line: 7881
	UNIMPLEMENTED();
}

void DEBUG_ViewVram(struct GameTracker *gameTracker)
{
	long* controlCommand;
	static int xPos;
	static int yPos;

	controlCommand = &gameTracker->controlCommand[0][0];

	if ((controlCommand[1] & 0x1))
	{
		if (xPos >= 0)
		{
			xPos -= 32;
		}
	}

	if ((controlCommand[1] & 0x2))
	{
		if (xPos < 272)
		{
			xPos += 32;
		}
	}

	if ((controlCommand[1] & 0x4))
	{
		if (yPos >= 0)
		{
			yPos -= 32;
		}
	}

	if ((controlCommand[1] & 0x8))
	{
		if (yPos < 512)
		{
			yPos += 32;
		}
	}

	SetDefDispEnv(&disp[0], yPos, xPos, SCREEN_WIDTH, SCREEN_HEIGHT);
	SetDefDispEnv(&disp[1], yPos, xPos, SCREEN_WIDTH, SCREEN_HEIGHT);

	gameTracker->playerInstance->flags |= 0x100;
}

void DEBUG_CaptureScreen(struct GameTracker *gameTracker)
{
	// Intentionally empty
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_PageFlip()
void DEBUG_PageFlip()
{ // line 4046, offset 0x800143e8
	/* begin block 1 */
		// Start line: 4047
		// Start offset: 0x800143E8
		// Variables:
			POLY_F4 poly; // stack offset -32
			unsigned long **drawot; // $s0
	/* end block 1 */
	// End offset: 0x800143E8
	// End Line: 4047

	/* begin block 2 */
		// Start line: 8086
	/* end block 2 */
	// End Line: 8087

	/* begin block 3 */
		// Start line: 6773
	/* end block 3 */
	// End Line: 6774
			UNIMPLEMENTED();
}

void DEBUG_FatalError(const char *fmt, ...)
{ 
	char msg[256];

	FONT_Flush();

	va_list ap;
	va_start(ap, fmt);
	vsprintf(msg, fmt, ap);
	FONT_Print(msg);

#if defined(PSX_VERSION)
	//break   1, 7
#if defined(PSXPC_VERSION)
	eprintf("%s", fmt);
	DrawOTag(NULL);
	assert(0);
#endif
#endif
}

void DEBUG_ProcessSecondController(struct GameTracker *gameTracker)
{
	// Intentionally empty
}

void DEBUG_ProcessCheat(struct GameTracker *gameTracker)
{ 
	long angleRelCamera; // $s2
	SVECTOR v; // stack offset -80
	VECTOR dv; // stack offset -72
	MATRIX rotate_mat; // stack offset -56

	//s3 = gameTracker
	if ((gameTrackerX.controlCommand[0][0] & 0xA01))
	{

	}
	UNIMPLEMENTED();
#if 0
		lw      $a0, 0x48($s3)
		li      $v0, 0xA01
		andi    $v1, $a0, 0xA01
		beq     $v1, $v0, loc_800149D4
		move    $s2, $zero
		andi    $v1, $a0, 0xA02
		li      $v0, 0xA02
		beq     $v1, $v0, loc_800149D4
		andi    $v1, $a0, 5
		li      $v0, 5
		bne     $v1, $v0, loc_8001495C
		andi    $v1, $a0, 9
		j       loc_800149D4
		li      $s2, 0xA00

		loc_8001495C:
	li      $v0, 9
		bne     $v1, $v0, loc_80014970
		andi    $v1, $a0, 6
		j       loc_800149D4
		li      $s2, 0x600

		loc_80014970 :
		li      $v0, 6
		bne     $v1, $v0, loc_80014984
		andi    $v1, $a0, 0xA
		j       loc_800149D4
		li      $s2, 0xE00

		loc_80014984 :
		li      $v0, 0xA
		bne     $v1, $v0, loc_80014998
		andi    $v0, $a0, 2
		j       loc_800149D4
		li      $s2, 0x200

		loc_80014998 :
		beqz    $v0, loc_800149A8
		andi    $v0, $a0, 4
		j       loc_800149D4
		li      $s2, 0x1000

		loc_800149A8 :
		beqz    $v0, loc_800149B8
		andi    $v0, $a0, 8
		j       loc_800149D4
		li      $s2, 0xC00

		loc_800149B8 :
		beqz    $v0, loc_800149C8
		andi    $v0, $a0, 1
		j       loc_800149D4
		li      $s2, 0x400

		loc_800149C8 :
		beqz    $v0, loc_800149D4
		nop
		li      $s2, 0x800

		loc_800149D4 :
		beqz    $s2, loc_80014A60
		addiu   $a0, $sp, 0x48 + var_38
		move    $a1, $zero
		jal     memset
		li      $a2, 8
		addiu   $s1, $sp, 0x48 + var_30
		move    $a0, $s1
		move    $a1, $zero
		jal     memset
		li      $a2, 0x10
		addiu   $s0, $sp, 0x48 + var_20
		move    $a0, $s0
		li      $v0, 0xFFFFFF00
		jal     sub_8003A130
		sh      $v0, 0x48 + var_36($sp)
		lh      $a0, -0x52BC($gp)
		move    $a1, $s0
		jal     sub_800790C8
		addu    $a0, $s2
		move    $a0, $s0
		addiu   $a1, $sp, 0x48 + var_38
		jal     ApplyMatrix
		move    $a2, $s1
		lw      $a0, 0x2C($s3)
		lhu     $v1, 0x48 + var_30($sp)
		lhu     $v0, 0x5C($a0)
		nop
		addu    $v0, $v1
		sh      $v0, 0x5C($a0)
		lw      $a0, 0x2C($s3)
		lhu     $v1, 0x48 + var_2C($sp)
		lhu     $v0, 0x5E($a0)
		nop
		addu    $v0, $v1
		sh      $v0, 0x5E($a0)

		loc_80014A60:
	lw      $ra, 0x48 + var_s10($sp)
		lw      $s3, 0x48 + var_sC($sp)
		lw      $s2, 0x48 + var_s8($sp)
		lw      $s1, 0x48 + var_s4($sp)
		lw      $s0, 0x48 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x60
		# End of function sub_80014908
#endif
}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_DoAreaProtection()
void DEBUG_DoAreaProtection()
{ // line 4593, offset 0x800146c0
	/* begin block 1 */
		// Start line: 4596
		// Start offset: 0x800146C8
	/* end block 1 */
	// End offset: 0x800146D8
	// End Line: 4603

	/* begin block 2 */
		// Start line: 7873
	/* end block 2 */
	// End Line: 7874
	UNIMPLEMENTED();
}




