#include "CORE.H"
#include "DEBUG.H"
#include "GAMELOOP.H"
#include "MENU/MENU.H"
#include "PSX/MAIN.H"
#include "FONT.H"

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

unsigned long debugRazielFlags1;
unsigned long debugRazielFlags2;
unsigned long debugRazielFlags3;

struct DebugMenuLine debugHealthSystemMenu[7];
struct DebugMenuLine cameraMenu[1];
struct DebugMenuLine fogMenu[1];

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
		(long*)debugRazielFlags1,
		0x3f
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"PASS THROUGH BARRIERS",
		(long*)debugRazielFlags1,
		0x1
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"WALL CRAWLING",
		(long*)debugRazielFlags1,
		0xb
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"FORCE",
		(long*)debugRazielFlags1,
		0xf
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SOUL REAVER",
		(long*)debugRazielFlags1,
		0x9
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"SWIM",
		(long*)debugRazielFlags1,
		0x1f
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"CONSTRICT",
		(long*)debugRazielFlags1,
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
		(long*)debugRazielFlags1,
		0x3fc00
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)debugRazielFlags1,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)debugRazielFlags1,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)debugRazielFlags1,
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
		(long*)debugRazielFlags1,
		0x3fc00
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)debugRazielFlags1,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)debugRazielFlags1,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)debugRazielFlags1,
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
		(long*)debugRazielFlags1,
		0x8
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Spectral Reaver",
		(long*)debugRazielFlags2,
		0x400
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Material Reaver",
		(long*)debugRazielFlags2,
		0x800
	},
	{
		DEBUG_LINE_TYPE_BIT,
		0,
		0,
		"Fire Reaver",
		(long*)debugRazielFlags2,
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
struct DebugMenuLine pauseMenu[7]; // offset 0x800c9ef0

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

}


void DEBUG_FogLoad(void)
{
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


// autogenerated function stub: 
// void /*$ra*/ DEBUG_Draw(struct GameTracker *gameTracker /*$a0*/, unsigned long **ot /*$a1*/)
void DEBUG_Draw(struct GameTracker *gameTracker, unsigned long **ot)
{ // line 1676, offset 0x800133a4
	/* begin block 1 */
		// Start line: 3352
	/* end block 1 */
	// End Line: 3353

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

}


// autogenerated function stub: 
// struct DebugMenuLine * /*$ra*/ get_last_menu_line(struct DebugMenuLine *line /*$a0*/)
struct DebugMenuLine * get_last_menu_line(struct DebugMenuLine *line)
{ // line 1749, offset 0x800134b0
	/* begin block 1 */
		// Start line: 3365
	/* end block 1 */
	// End Line: 3366

	/* begin block 2 */
		// Start line: 3366
	/* end block 2 */
	// End Line: 3367

	return null;
}


// autogenerated function stub: 
// int /*$ra*/ num_menu_items(struct DebugMenuLine *menu /*$a0*/)
int num_menu_items(struct DebugMenuLine *menu)
{ // line 1757, offset 0x800134e0
	/* begin block 1 */
		// Start line: 1759
		// Start offset: 0x800134E0
		// Variables:
			int nitems; // $a1
	/* end block 1 */
	// End offset: 0x80013508
	// End Line: 1763

	/* begin block 2 */
		// Start line: 3381
	/* end block 2 */
	// End Line: 3382

	/* begin block 3 */
		// Start line: 3382
	/* end block 3 */
	// End Line: 3383

	/* begin block 4 */
		// Start line: 3383
	/* end block 4 */
	// End Line: 3384

	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ maybe_change_menu_choice(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *menu /*$s2*/)
void maybe_change_menu_choice(struct GameTracker *gt, struct DebugMenuLine *menu)
{ // line 1766, offset 0x80013510
	/* begin block 1 */
		// Start line: 1767
		// Start offset: 0x80013510
		// Variables:
			long *command; // $s0
			int choice; // $s1
			int nitems; // $a0
			int incr; // $v1
	/* end block 1 */
	// End offset: 0x800135C0
	// End Line: 1785

	/* begin block 2 */
		// Start line: 3399
	/* end block 2 */
	// End Line: 3400

}


// autogenerated function stub: 
// void /*$ra*/ handle_line_type_long(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *line /*$a1*/)
void handle_line_type_long(struct GameTracker *gt, struct DebugMenuLine *line)
{ // line 1789, offset 0x800135d8
	/* begin block 1 */
		// Start line: 1790
		// Start offset: 0x800135D8
		// Variables:
			long *command; // $v1

		/* begin block 1.1 */
			// Start line: 1794
			// Start offset: 0x800135F8
			// Variables:
				int incr; // $a2
		/* end block 1.1 */
		// End offset: 0x800136B4
		// End Line: 1811
	/* end block 1 */
	// End offset: 0x800136B4
	// End Line: 1812

	/* begin block 2 */
		// Start line: 3450
	/* end block 2 */
	// End Line: 3451

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

}


// autogenerated function stub: 
// void /*$ra*/ handle_line_type_action(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *line /*$a1*/)
void handle_line_type_action(struct GameTracker *gt, struct DebugMenuLine *line)
{ // line 1825, offset 0x8001371c
	/* begin block 1 */
		// Start line: 1826
		// Start offset: 0x8001371C
		// Variables:
			int ok; // $v0
	/* end block 1 */
	// End offset: 0x80013748
	// End Line: 1835

	/* begin block 2 */
		// Start line: 3523
	/* end block 2 */
	// End Line: 3524

}


// autogenerated function stub: 
// void /*$ra*/ handle_line_type_action_with_line(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *line /*$a1*/)
void handle_line_type_action_with_line(struct GameTracker *gt, struct DebugMenuLine *line)
{ // line 1837, offset 0x80013758
	/* begin block 1 */
		// Start line: 1838
		// Start offset: 0x80013758
		// Variables:
			enum option_ctrl_t ctrl; // $a2
	/* end block 1 */
	// End offset: 0x800137AC
	// End Line: 1849

	/* begin block 2 */
		// Start line: 3547
	/* end block 2 */
	// End Line: 3548

}


// autogenerated function stub: 
// void /*$ra*/ handle_line_type_menu(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *line /*$s0*/)
void handle_line_type_menu(struct GameTracker *gt, struct DebugMenuLine *line)
{ // line 1853, offset 0x800137bc
	/* begin block 1 */
		// Start line: 1854
		// Start offset: 0x800137BC
		// Variables:
			int ok; // $v0
	/* end block 1 */
	// End offset: 0x8001382C
	// End Line: 1870

	/* begin block 2 */
		// Start line: 3579
	/* end block 2 */
	// End Line: 3580

}


// autogenerated function stub: 
// void /*$ra*/ process_menu_line(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *menu /*$a1*/)
void process_menu_line(struct GameTracker *gt, struct DebugMenuLine *menu)
{ // line 1889, offset 0x8001383c
	/* begin block 1 */
		// Start line: 1890
		// Start offset: 0x8001383C
		// Variables:
			struct DebugMenuLine *line; // $a1

		/* begin block 1.1 */
			// Start line: 1893
			// Start offset: 0x8001386C
			// Variables:
				struct debug_dispatch_t *dispatch; // $v1
		/* end block 1.1 */
		// End offset: 0x80013890
		// End Line: 1898
	/* end block 1 */
	// End offset: 0x80013890
	// End Line: 1899

	/* begin block 2 */
		// Start line: 3652
	/* end block 2 */
	// End Line: 3653

	/* begin block 3 */
		// Start line: 3653
	/* end block 3 */
	// End Line: 3654

}


// autogenerated function stub: 
// int /*$ra*/ pre_process_functions(struct GameTracker *gt /*$a0*/, struct DebugMenuLine *menu /*$a1*/)
int pre_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu)
{ // line 1903, offset 0x800138a0
	/* begin block 1 */
		// Start line: 3681
	/* end block 1 */
	// End Line: 3682

	/* begin block 2 */
		// Start line: 3682
	/* end block 2 */
	// End Line: 3683

	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ post_process_functions(struct GameTracker *gt /*$s0*/, struct DebugMenuLine *menu /*$a1*/)
void post_process_functions(struct GameTracker *gt, struct DebugMenuLine *menu)
{ // line 1911, offset 0x800138c8
	/* begin block 1 */
		// Start line: 3697
	/* end block 1 */
	// End Line: 3698

}


// autogenerated function stub: 
// void /*$ra*/ set_debug_leading()
void set_debug_leading()
{ // line 1966, offset 0x8001396c
	/* begin block 1 */
		// Start line: 3807
	/* end block 1 */
	// End Line: 3808

	/* begin block 2 */
		// Start line: 3808
	/* end block 2 */
	// End Line: 3809

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

}


// autogenerated function stub: 
// char * /*$ra*/ find_eol(char *text /*$v0*/)
char * find_eol(char *text)
{ // line 2027, offset 0x80013b54
	/* begin block 1 */
		// Start line: 3956
	/* end block 1 */
	// End Line: 3957

	return null;
}


// autogenerated function stub: 
// void /*$ra*/ draw_menu_item(struct GameTracker *gt /*$a0*/, struct debug_format_t *fmt /*$s2*/, char *text /*$s0*/)
void draw_menu_item(struct GameTracker *gt, struct debug_format_t *fmt, char *text)
{ // line 2035, offset 0x80013b8c
	/* begin block 1 */
		// Start line: 2037
		// Start offset: 0x80013BAC

		/* begin block 1.1 */
			// Start line: 2038
			// Start offset: 0x80013BAC
			// Variables:
				char *eol; // $s1
				char c; // $s3

			/* begin block 1.1.1 */
				// Start line: 2042
				// Start offset: 0x80013BD0
			/* end block 1.1.1 */
			// End offset: 0x80013BD0
			// End Line: 2042
		/* end block 1.1 */
		// End offset: 0x80013C40
		// End Line: 2058
	/* end block 1 */
	// End offset: 0x80013C5C
	// End Line: 2062

	/* begin block 2 */
		// Start line: 3972
	/* end block 2 */
	// End Line: 3973

}


// autogenerated function stub: 
// void /*$ra*/ draw_menu(struct GameTracker *gt /*$s4*/, struct DebugMenuLine *menu /*$s0*/)
void draw_menu(struct GameTracker *gt, struct DebugMenuLine *menu)
{ // line 2065, offset 0x80013c8c
	/* begin block 1 */
		// Start line: 2066
		// Start offset: 0x80013C8C
		// Variables:
			struct debug_format_t fmt; // stack offset -56
			int i; // $s3

		/* begin block 1.1 */
			// Start line: 2079
			// Start offset: 0x80013D0C
			// Variables:
				int xpos; // $s2
				int ypos; // $s1
		/* end block 1.1 */
		// End offset: 0x80013E60
		// End Line: 2123
	/* end block 1 */
	// End offset: 0x80013E78
	// End Line: 2124

	/* begin block 2 */
		// Start line: 4055
	/* end block 2 */
	// End Line: 4056

	/* begin block 3 */
		// Start line: 4056
	/* end block 3 */
	// End Line: 4057

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
			if (menu[debugMenuChoice++].type < DEBUG_LINE_TYPE_FORMAT)
			{
				do
				{
				} while (menu[debugMenuChoice++].type >= DEBUG_LINE_TYPE_FORMAT);
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

}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_ContinueGame()
void DEBUG_ContinueGame()
{ // line 3736, offset 0x8001421c
	/* begin block 1 */
		// Start line: 7472
	/* end block 1 */
	// End Line: 7473

	/* begin block 2 */
		// Start line: 6331
	/* end block 2 */
	// End Line: 6332

}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_ExitGame()
void DEBUG_ExitGame()
{ // line 3757, offset 0x80014234
	/* begin block 1 */
		// Start line: 6371
	/* end block 1 */
	// End Line: 6372

}

void DEBUG_ReloadCurrentLevel()
{
}

void DEBUG_LevelSelectNew()
{
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

}


// autogenerated function stub: 
// void /*$ra*/ DEBUG_ViewVram(struct GameTracker *gameTracker /*$s1*/)
void DEBUG_ViewVram(struct GameTracker *gameTracker)
{ // line 3946, offset 0x800142c0
	/* begin block 1 */
		// Start line: 3947
		// Start offset: 0x800142C0
		// Variables:
			long *controlCommand; // $a0
			static int xPos; // offset 0x478
			static int yPos; // offset 0x47c
	/* end block 1 */
	// End offset: 0x80014380
	// End Line: 3973

	/* begin block 2 */
		// Start line: 6577
	/* end block 2 */
	// End Line: 6578

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
	assert(0);
#endif
#endif
}

void DEBUG_ProcessSecondController(struct GameTracker *gameTracker)
{
	// Intentionally empty
}

// autogenerated function stub: 
// void /*$ra*/ DEBUG_ProcessCheat(struct GameTracker *gameTracker /*$s3*/)
void DEBUG_ProcessCheat(struct GameTracker *gameTracker)
{ // line 4538, offset 0x8001454c
	/* begin block 1 */
		// Start line: 4539
		// Start offset: 0x8001454C
		// Variables:
			long angleRelCamera; // $s2

		/* begin block 1.1 */
			// Start line: 4565
			// Start offset: 0x80014620
			// Variables:
				SVECTOR v; // stack offset -80
				VECTOR dv; // stack offset -72
				//MATRIX rotate_mat; // stack offset -56
		/* end block 1.1 */
		// End offset: 0x800146A4
		// End Line: 4575
	/* end block 1 */
	// End offset: 0x800146A4
	// End Line: 4576

	/* begin block 2 */
		// Start line: 7762
	/* end block 2 */
	// End Line: 7763

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

}




