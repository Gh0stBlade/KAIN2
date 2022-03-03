#ifndef THISDUST_H
#define THISDUST_H

#define null 0

#include "kain2.h"


static unsigned long __hblankEvent; // offset 0x800D18E8

static struct _G2AppDataVM_Type _appDataVM; // offset 0x800D517C

static struct _G2AnimChanStatusBlockPool_Type _chanStatusBlockPool; // offset 0x800D49CC

static struct _G2AnimControllerPool_Type _controllerPool; // offset 0x800D515C

static int _indexTable[16]; // offset 0x800CAC2C

static struct _G2AnimInterpStateBlockPool_Type _interpStateBlockPool; // offset 0x800D516C

extern int _ramsize; // offset 0x800CE558

static struct _G2AnimSegValue_Type _segValues[80]; // offset 0x800D49DC

extern int _stacksize; // offset 0x800CE55C

static int _stepSizeTable[64]; // offset 0x800CAC6C

static struct AadProgramAtr aadDummySfxProgram; // offset 0x800CEF48

extern unsigned long aadGp; // offset 0x800CA910

static unsigned short aadHblanksPerUpdate[4]; // offset 0x800CECC0

extern unsigned short aadPitchTable[85]; // offset 0x800CEF50

static unsigned long aadReverbModeSize[10]; // offset 0x800CED5C

extern unsigned long aadStepsPerCent[85]; // offset 0x800CEFFC

extern short aadStepsPerSemitone[85]; // offset 0x800CF150

static unsigned long aadUpdateRate[4]; // offset 0x800CECC8

static long ACE_amount; // offset 0x800CFE80

extern struct DebugMenuLine AIMenu[1]; // offset 0x800CFB94

static int AlgoControlFlag; // offset 0x800CF858

static short align_flag; // offset 0x800CE358

extern int AutoFaceAngle; // offset 0x800D59A8

extern short AXIS_LENGTH; // offset 0x800CDE5C

extern struct _ButtonTexture BexitLine[2]; // offset 0x800D0E20

static int blast_range; // offset 0x800D48CC

static int BlockCount; // offset 0x800CF880

extern struct _ButtonTexture BmenuLines[3][2]; // offset 0x800D0F24

extern struct DebugMenuLine BossAreasMenu[1]; // offset 0x800CFB7C

static struct SavedBasic (*bufferSavedIntroArray[64]); // offset 0x800D59D0

static struct burntTuneType burntTest; // offset 0x800CF674

static short Camera_lookDist; // offset 0x800CFE8E

static short Camera_lookHeight; // offset 0x800CFE8C

extern long camera_modeToIndex[15]; // offset 0x800CDF38

static struct _SVector camera_plane; // offset 0x800CFE64

static struct _SVector camera_shakeOffset[16]; // offset 0x800CFDE4

static short camera_still; // offset 0x800CDF78

extern int CameraCenterDelay; // offset 0x800D035C

static short CameraLookStickyFlag; // offset 0x800CDF96

extern struct DebugMenuLine cameraMenu[1]; // offset 0x800CFCE0

extern long cameraMode; // offset 0x800CE2A4

extern long campoint_hh; // offset 0x800D0358

extern long campoint_hw; // offset 0x800D0354

extern struct __CannedSound cannedSound[4]; // offset 0x800CF808

static int cem_cursor_width; // offset 0x800CDCEC

static int cem_item_leading; // offset 0x800CDCF8

static int cem_line_leading; // offset 0x800CDCF4

static int cem_line_width; // offset 0x800CDCF0

static int cem_x_base; // offset 0x800CDCE4

static int cem_y_base; // offset 0x800CDCE8

static short CenterFlag; // offset 0x800CFDDC

extern char charMap[92][3]; // offset 0x800C8620

static long cheatCodeLastCommand; // offset 0x800CDB44

static long CheatKeyMap[10]; // offset 0x800CDB48

static char circBuf[4096]; // offset 0x800D2E2C

static void *circWhere; // offset 0x800CF5B0

extern struct BLK_FILL clearRect[2]; // offset 0x800D0F88

extern long collide_acceptAttr; // offset 0x800CE168

extern long collide_ignoreAttr; // offset 0x800CE164

extern struct _SVector collide_localPoint; // offset 0x800D0370

extern struct MATRIX *collide_lwTransform; // offset 0x800D0378

extern struct _SVector *collide_normal0; // offset 0x800D037C

extern struct _SVector *collide_normal1; // offset 0x800D0380

extern struct _SVector *collide_point0; // offset 0x800D0360

extern struct _SVector *collide_point1; // offset 0x800D0364

extern long collide_t0; // offset 0x800D0368

extern long collide_t1; // offset 0x800D036C

static short combat_cam_angle; // offset 0x800CFDD6

static short combat_cam_debounce; // offset 0x800CFDDA

static short combat_cam_distance; // offset 0x800CFDD4

static short combat_cam_weight; // offset 0x800CFDD8

static struct __FitPoint constrictData[32]; // offset 0x800D5804

extern int ControlFlag; // offset 0x800D59AC

extern struct __razController controllerList[24]; // offset 0x800CF7A8

extern unsigned char controllerType[2]; // offset 0x800D0CC8

extern long *controlsScreen; // offset 0x800CE6CC

static unsigned short cowlList[19]; // offset 0x800CF780

static short current_rain_fade; // offset 0x800CEB78

static long current_roll_amount; // offset 0x800CDF80

static struct ScriptPCode *currentActionScript; // offset 0x800D2738

static struct Event *currentEventInstance; // offset 0x800D2734

static long CurrentEventLine; // offset 0x800CF53C

extern int currentIdleCount; // offset 0x800CF87C

extern struct __Idle **currentIdleList; // offset 0x800CF874

extern int currentIdleListCount; // offset 0x800CF878

static struct Level *CurrentPuzzleLevel; // offset 0x800D27D0

extern long CurrentWarpNumber; // offset 0x800CF1FC

static struct MATRIX cwTransform2X; // offset 0x800CFDB4

extern long debug_cameraMode; // offset 0x800CDAE8

static struct debug_dispatch_t debug_dispatch_table[5]; // offset 0x800C84D8

extern long debugFogBlu; // offset 0x800CDB08

extern long debugFogFar; // offset 0x800CDAF4

extern long debugFogGrn; // offset 0x800CDB04

extern long debugFogNear; // offset 0x800CDAF8

extern long debugFogRed; // offset 0x800CDB00

extern struct DebugMenuLine debugForgedAbilitiesMenu[1]; // offset 0x800CFBF4

extern long debugGameTime; // offset 0x800CDAFC

extern struct DebugMenuLine debugGlyphAbilitiesMenu[1]; // offset 0x800CFBC4

extern long debugHealthLevel; // offset 0x800CDB0C

extern struct DebugMenuLine debugHealthSystemMenu[1]; // offset 0x800CFC3C

extern struct DebugMenuLine debugImbueSoulReaverMenu[1]; // offset 0x800CFD28

extern long debugManaLevel; // offset 0x800CDB10

static long debugMenuChoice; // offset 0x800CDB40

extern unsigned char debugOrgFogBlu; // offset 0x800CFC90

extern short debugOrgFogFar; // offset 0x800CFC94

extern unsigned char debugOrgFogGrn; // offset 0x800CFC8C

extern short debugOrgFogNear; // offset 0x800CFC54

extern unsigned char debugOrgFogRed; // offset 0x800CFC88

extern unsigned long debugRazielFlags1; // offset 0x800CFD40

extern unsigned long debugRazielFlags2; // offset 0x800CFD44

extern unsigned long debugRazielFlags3; // offset 0x800CFD48

extern struct DebugMenuLine debugRazielMenu[1]; // offset 0x800CFD4C

extern struct DebugMenuLine debugSoundMenu[1]; // offset 0x800CFBAC

extern struct DebugMenuLine debugSpecialAbilitiesMenu[1]; // offset 0x800CFC0C

extern unsigned long debugTimeMult; // offset 0x800CDAF0

extern long debugVoiceIndex; // offset 0x800CDAEC

static struct LightGroup default_lightgroup; // offset 0x800CE460

static struct _MonsterFunctionTable DefaultFunctionTable; // offset 0x800CF68C

static struct _MonsterState DefaultStateTable[31]; // offset 0x800CAA20

static struct _fth directionList[6]; // offset 0x800CF75C

extern struct DISPENV disp[2]; // offset 0x800D0E40

extern long DoMainMenu; // offset 0x800CE6C0

static int Down; // offset 0x800CF590

extern struct DRAWENV draw[2]; // offset 0x800D0E6C

extern long draw_belowSplit; // offset 0x800CE19C

extern unsigned char *draw_stack; // offset 0x800D0384

static int dualShock; // offset 0x800CE35C

extern unsigned char dualshock_align[6]; // offset 0x800C8760

extern unsigned char dualshock_motors[2]; // offset 0x800C875C

static short dualshock_onflag; // offset 0x800CE35A

static long dualshock0_time; // offset 0x800D0C68

static long dualshock1_time; // offset 0x800D0C6C

extern long dyna_clddyna[8]; // offset 0x800CE104

extern long dyna_cldstat[8]; // offset 0x800CE144

extern short ecostable[3072]; // offset 0x800C9110

static struct __EngagedInstance EngagedList[15]; // offset 0x800D5924

static short *EventAbortedPosition; // offset 0x800D273C

static short EventAbortLine; // offset 0x800CF534

static long EventCurrentEventIndex; // offset 0x800CF540

static short EventJustRecievedTimer; // offset 0x800CF536

extern struct _Instance (*eventListArray[20][10]); // offset 0x800D2B0C

extern long eventListNumInstances[20]; // offset 0x800D2A1C

extern struct DebugMenuLine eventMenu[1]; // offset 0x800CFB64

static struct __Force *ExternalForces; // offset 0x800D51D4

static struct __Force ExternalForces[4]; // offset 0x800D58C4

static struct _G2SVector3_Type *ExtraRot; // offset 0x800D58B4

static struct _G2SVector3_Type ExtraRotData; // offset 0x800D58AC

extern struct UVTYPE *face_uv0; // offset 0x800CE1B8

extern struct UVTYPE *face_uv01; // offset 0x800CE1C4

extern struct UVTYPE *face_uv1; // offset 0x800CE1BC

extern struct UVTYPE *face_uv12; // offset 0x800CE1C8

extern struct UVTYPE *face_uv2; // offset 0x800CE1C0

extern struct UVTYPE *face_uv20; // offset 0x800CE1CC

extern SVECTOR*face_v0; // offset 0x800CE1A0

extern SVECTOR*face_v01; // offset 0x800CE1AC

extern SVECTOR*face_v1; // offset 0x800CE1A4

extern SVECTOR*face_v12; // offset 0x800CE1B0

extern SVECTOR*face_v2; // offset 0x800CE1A8

extern SVECTOR*face_v20; // offset 0x800CE1B4


static struct _ButtonTexture *FaceButtons; // offset 0x800D5C04

static short FireReaverFlag; // offset 0x800D48B8

extern struct DebugMenuLine fogMenu[1]; // offset 0x800CFC58

extern struct FontPos fontPos[72]; // offset 0x800C8500

extern struct Object *fontsObject; // offset 0x800CE2AC

extern struct FontTracker fontTracker; // offset 0x800D05EC

extern unsigned char fontTransTable[128]; // offset 0x800CE224

extern short forceDrawInFront; // offset 0x800CE198

static char fp_str[512]; // offset 0x800D03E4

extern long FRAC_BITS; // offset 0x800CEB3C

static struct MONTABLE_207fake functionChoiceTable[6]; // offset 0x800CA9F0

extern struct _FXBlastringEffect *fx_blastring; // offset 0x800CF5D0

extern long FX_ColorArray[6]; // offset 0x800CEBAC

static struct _Instance *FX_ConstrictInstance; // offset 0x800CEB70

static struct _Position FX_ConstrictPosition; // offset 0x800D1594

static struct _Position *FX_ConstrictPositionPtr; // offset 0x800D159C

static struct _FXRibbon *FX_ConstrictRibbon; // offset 0x800CEB68

static short FX_ConstrictStage; // offset 0x800CEB6C

static short FX_Frames; // offset 0x800D15A4

static struct _FXGeneralEffect *FX_GeneralEffectTracker; // offset 0x800CEB64

static short fx_going; // offset 0x800CF5CC

static struct _FX_PRIM *FX_LastUsedPrim; // offset 0x800D15A0

static int fx_radius_old; // offset 0x800D48D0

static struct _Instance *FX_reaver_instance; // offset 0x800CEB7C

static short FX_TimeCount; // offset 0x800D15A6

extern struct _FXTracker *fxTracker; // offset 0x800D0C58

static struct _G2AnimAlphaTable_Type (*G2AlphaTables[7]); // offset 0x800CF594

static struct GameCheat GameCheats[23]; // offset 0x800CDB70

extern int gamePadControllerOut; // offset 0x800D0D70

static struct GameTracker *gameTracker; // offset 0x800D58A4

extern struct GameTracker gameTrackerX; // offset 0x800D0FAC

static int gDefragRequest; // offset 0x800CECDC

extern struct _FXTracker *gFXT; // offset 0x800D0C44

extern struct HintSystemStruct gHintSystem; // offset 0x800D2AFC

extern struct _GlobalSaveTracker *GlobalSave; // offset 0x800CF8A8

static int glowdeg; // offset 0x800D48F8

static short glyph_cost; // offset 0x800D48C4

static short glyph_time; // offset 0x800D48C0

static short glyph_trigger; // offset 0x800D48C2

static struct MEMCARD_216fake gMcmenu; // offset 0x800D8FD4

extern struct memcard_t gMemcard; // offset 0x800D91D4

extern struct _Normal gNormalList[244]; // offset 0x800C8970

static int gNumMaterialMonsters; // offset 0x800CF634

static int gNumMonsters; // offset 0x800CF62C

static int gNumSpectralMonsters; // offset 0x800CF630

extern struct DebugMenuLine goodiesMenu[1]; // offset 0x800CFBDC

static struct _PolytopeList *gPolytopeList; // offset 0x800D0C1C

static int gpSaved; // offset 0x800CE3A4

static int hack_attract; // offset 0x800CFA70

static int hack_attract_movie; // offset 0x800CFA74

static int hack_initialized; // offset 0x800CFB3C

static int hack_reset_attract; // offset 0x800CFA68

extern unsigned long **hackOT; // offset 0x800D0C50

extern char HashExtensions[7][4]; // offset 0x800CE4D0

static short hitline_rot; // offset 0x800CFE7C

static struct _SVector HUD_Cap_Pos; // offset 0x800D48E4

static struct _SVector HUD_Cap_Vel; // offset 0x800D48EC

static short HUD_Captured; // offset 0x800CF5D4

static short HUD_Count; // offset 0x800D48C6

static short HUD_Count_Overall; // offset 0x800D48C8

static short HUD_Pos_vel; // offset 0x800D48DE

static short HUD_Position; // offset 0x800D48DC

static short HUD_Rot_vel; // offset 0x800D48DA

static short HUD_Rotation; // offset 0x800D48D8

static short HUD_State; // offset 0x800D48E0

static short HUD_Wait; // offset 0x800D48E2

extern int hud_warp_arrow_flash; // offset 0x800D48FC

extern struct _MonsterFunctionTable HUMAN_FunctionTable; // offset 0x800CF5E0

extern struct _MonsterStateChoice HUMAN_StateChoiceTable[6]; // offset 0x800CA978

extern short identityMatrix[3][3]; // offset 0x800CE870

static int ignoreFind; // offset 0x800D0C64

extern int InStreamUnit; // offset 0x800D0C48

static int ITEMSKIP; // offset 0x800CFA64

static int LastBlock; // offset 0x800CF884

extern unsigned short lastData[2]; // offset 0x800CE3A0

static int LastRC; // offset 0x800D59A4

static int lastSenseFrame; // offset 0x800D49C8

static struct _SVector left_point; // offset 0x800CFE6C

extern struct DebugMenuLine level2SelectMenu[1]; // offset 0x800CFD10

extern struct DebugMenuLine levelSelectMenu[1]; // offset 0x800CFCF8

extern void *light_light; // offset 0x800D0D78

extern struct LightInfo *light_lightInfo; // offset 0x800D0D80

extern struct _Terrain *light_terrain; // offset 0x800D0D7C

static int LINESKIP; // offset 0x800CFA60

static int loadCanFail; // offset 0x800CF4E4

extern int LoopCounter; // offset 0x800CF774

extern short M_TrackClutUpdate; // offset 0x800D4804

static int MAIN_WIDTH; // offset 0x800CFA54

static int MAIN_XPOS; // offset 0x800CFA50

static int MAIN_YPOS; // offset 0x800CFA48

static char mainFormatString[20]; // offset 0x800CDACC

extern short mainMenuFadeInTime; // offset 0x800CE6D0

extern short mainMenuFading; // offset 0x800CE6D2

static int MANNA_Pickup_Time; // offset 0x800D48D4

static short MANNA_Pos_vel; // offset 0x800D48BE

static short MANNA_Position; // offset 0x800D48BC

extern struct DebugMenuLine mcardMenu[1]; // offset 0x800CFC24

static struct menuface_t MenuFaces[8]; // offset 0x800CFABC

extern long menuPauseSel; // offset 0x800CDAE4

static char midiDataByteCount[8]; // offset 0x800CED84

static struct MissileGraphics MISSILE_objectTable[4]; // offset 0x800CA9C0

extern long modelDQP; // offset 0x800D0388

static struct _MonsterVars (*monsterSenseArray[40]); // offset 0x800D4900

static char monsterSensed[40]; // offset 0x800D49A0

extern char monVersion[12]; // offset 0x800CF95C

extern short MORPH_Component[3]; // offset 0x800D1914

extern struct _TFace *MORPH_SavedFace; // offset 0x800D1D20

extern struct Level *MORPH_SavedLevel; // offset 0x800D1910

extern short MORPH_Track[2]; // offset 0x800D191C

static long MoveCodeStreamExtra; // offset 0x800CF538

static struct NewMemTracker newMemTracker; // offset 0x800D18D4

extern int nomusic; // offset 0x800CE56C

static struct _Normal Norm; // offset 0x800D599C

extern int nosound; // offset 0x800CE568

static long numbufferedIntros; // offset 0x800CF8AC

extern long NumSignalsToReset; // offset 0x800CF548

extern struct ObjectAccess objectAccess[28]; // offset 0x800C877C

extern struct ObjectFunc objectFunc[7]; // offset 0x800C885C

static long objectOneShotTriggerTbl[3]; // offset 0x800CEA90

static long overrideCommand[2][5]; // offset 0x800D0CA0

static long overrideData[2][5]; // offset 0x800D0C70

static long *PadData; // offset 0x800D58A8

static short panic_count; // offset 0x800CDF9A

static short pause_redraw_flag; // offset 0x800D0C3C

static void *pause_redraw_prim; // offset 0x800D0C40

static int PAUSE_WIDTH; // offset 0x800CFA5C

static int PAUSE_XPOS; // offset 0x800CFA58

static int PAUSE_YPOS; // offset 0x800CFA4C

extern struct _ButtonTexture PauseButtons[4][2]; // offset 0x800D122C

static char pauseFormatString[20]; // offset 0x800CDAB8

extern struct DebugMenuLine pauseMenu[1]; // offset 0x800CFCC8

extern int pcverify; // offset 0x800CE564

static int Pending; // offset 0x800CF588

static int PhysicsMode; // offset 0x800D5914

static unsigned char PickupList[4]; // offset 0x800CF77C

extern long playerCameraMode; // offset 0x800CE2A8

static struct RazielData *PlayerData; // offset 0x800D57D0

extern struct PoolManagementData *poolManagementData; // offset 0x800D51D0

static struct PoolManagementData poolManagementDataStorage; // offset 0x800D5184

extern char *primBase; // offset 0x800D0C5C

extern unsigned short psxData[2]; // offset 0x800D0CCC

static struct RadarInfo radarDir[8]; // offset 0x800CF638

static unsigned char radarDirIndex[8]; // offset 0x800CF668

static short rain_amount; // offset 0x800CEB76

static struct __Player Raziel; // offset 0x800D5214

extern long RazielCommands[10]; // offset 0x800CF560

static struct _G2AnimInterpInfo_Type razInterpInfo[3]; // offset 0x800D57D4

extern short RENDER_currentStreamUnitID; // offset 0x800D0C4C

extern struct SignalResetStruct ResetSignalArray[16]; // offset 0x800D2A74

static struct _SVector right_point; // offset 0x800CFE74

static int roll_inc; // offset 0x800CDF84

static long roll_target; // offset 0x800CDF7C

static int s_zval; // offset 0x800D18F8

extern long SaveArraySize[10]; // offset 0x800CF8B4

static struct SavedInfoTracker savedInfoTracker; // offset 0x800D59C0

extern struct DebugMenuLine saveMenu[1]; // offset 0x800CFC70

extern struct LightInfo *sbsp_lightInfo; // offset 0x800D12AC

static struct UW_ScreenXY *ScreenMorphArray; // offset 0x800CF894

extern char (*selectMenuText[5]); // offset 0x800C8768

extern SVECTORshadow_vertices[11]; // offset 0x800D038C

static short shorten_count; // offset 0x800CDF74

static short shorten_flag; // offset 0x800CDF76

extern long SignalEndGame; // offset 0x800CDFBC

extern struct SignalInfo signalInfoList[27]; // offset 0x800CDFC0

extern struct _MonsterFunctionTable SLUAGH_FunctionTable; // offset 0x800CF6AC

extern struct _MonsterStateChoice SLUAGH_StateChoiceTable[4]; // offset 0x800CAB18

static unsigned long smfBytesLeft; // offset 0x800D18F0

static unsigned char *smfDataPtr; // offset 0x800D18EC

static struct AadDynamicSfxLoadInfo *smfInfo; // offset 0x800D18F4

static short snow_amount; // offset 0x800CEB74

extern struct _MonsterFunctionTable SOUL_FunctionTable; // offset 0x800CF6D4

extern struct _MonsterStateChoice SOUL_StateChoiceTable[7]; // offset 0x800CAB48

extern char soundBuffer[13256]; // offset 0x800D5C08

extern struct SoundEffectChannel soundEffectChannelTbl[16]; // offset 0x800D12C0

extern unsigned short SpecialFogClut; // offset 0x800D05E8

static struct DVECTOR Spiral_Array[65]; // offset 0x800D15B4

static long Spiral_Current; // offset 0x800CEBC8

static short Spiral_Degrees; // offset 0x800CEBC6

static int Spiral_Glow_Size; // offset 0x800D18CC

static int Spiral_Glow_X; // offset 0x800D18C4

static int Spiral_Glow_Y; // offset 0x800D18C8

static long Spiral_Max; // offset 0x800CEBCC

static int Spiral_Mod; // offset 0x800D18D0

static short Spiral_Number; // offset 0x800CEBC4

static struct DVECTOR Spiral_OffsetM[64]; // offset 0x800D17C4

static struct DVECTOR Spiral_OffsetP[64]; // offset 0x800D16C4

static struct _Rotation splinecam_helprot; // offset 0x800CFE84

extern unsigned long StackSave; // offset 0x800D0C54

static int StartGameFading; // offset 0x800CFA6C

extern long stat_clddyna[8]; // offset 0x800CE124

extern struct DebugMenuLine statsMenu[1]; // offset 0x800CFC98

extern int StCdIntrFlag; // offset 0x800D8FD0

static struct MATRIX TempMat; // offset 0x800D51E4

static struct MATRIX TempMat; // offset 0x800D5884

extern struct _BlockVramEntry TerrainVramBlock; // offset 0x800D480C

static char (*the_attract_movies[4]); // offset 0x800CADC8

static struct cinema_fn_table_t *the_cine_table; // offset 0x800CAD90

static struct _ObjectTracker *the_cine_tracker; // offset 0x800CAD94

static struct font_color_t the_font_color_table[5]; // offset 0x800CE1D0

static char *the_format_string; // offset 0x800CDAE0

static int the_header_size; // offset 0x800CF8B0

extern struct mcmenu_table_t the_mcmenu_table; // offset 0x24

static struct menu_sound_entry_t the_menu_sounds[6]; // offset 0x800CAD98

extern struct DebugMenuLine *the_previous_menu; // offset 0x800CDCE0

extern struct Camera theCamera; // offset 0x800CFE90

static long timerArray[10]; // offset 0x800D51A4

static long timerIndex; // offset 0x800D51CC

static int Up; // offset 0x800CF58C

static long UW_angle; // offset 0x800D59B0

static long UW_angleInc; // offset 0x800D59BC

static long UW_scalex; // offset 0x800D59B4

static long UW_scalexInc; // offset 0x800D59B8

static long vmClock; // offset 0x800CEBD8

static long vmRealClock; // offset 0x800CEBD4

extern long VRAM_NeedToUpdateMorph; // offset 0x800CF5B4

extern struct _MonsterFunctionTable VWRAITH_FunctionTable; // offset 0x800CF6F4

extern struct _MonsterStateChoice VWRAITH_StateChoiceTable[7]; // offset 0x800CAB9C

static int warpDraw; // offset 0x800D48F4

extern struct STREAM_294fake WarpGateLoadInfo; // offset 0x800D18FC

extern struct WarpRoom WarpRoomArray[14]; // offset 0x800CF200

static int WaterStatus; // offset 0x800CF778

static struct MATRIX wcTransform2X; // offset 0x800CFD94

static struct MATRIX wcTransformX; // offset 0x800CFD74

static short wind_deg; // offset 0x800CEBA8

static short wind_speed; // offset 0x800CEBAA

static short windx; // offset 0x800CEBA4

static short windy; // offset 0x800CEBA6

extern struct _MonsterFunctionTable WORSHIP_FunctionTable; // offset 0x800CF714

extern struct _MonsterStateChoice WORSHIP_StateChoiceTable[5]; // offset 0x800CABF0

static int ZoneDelta; // offset 0x800CF85C

struct TextureFT3 // hashcode: 0x2D2801D6 (dec: 757596630)
{
	unsigned char u0; // size=0, offset=0
	unsigned char v0; // size=0, offset=1
	unsigned short clut; // size=0, offset=2
	unsigned char u1; // size=0, offset=4
	unsigned char v1; // size=0, offset=5
	unsigned short tpage; // size=0, offset=6
	unsigned char u2; // size=0, offset=8
	unsigned char v2; // size=0, offset=9
	unsigned short attr; // size=0, offset=10
};

struct TextureMT3 // hashcode: 0xC9D0A5B9 (dec: -909072967)
{
	unsigned char u0; // size=0, offset=0
	unsigned char v0; // size=0, offset=1
	unsigned short clut; // size=0, offset=2
	unsigned char u1; // size=0, offset=4
	unsigned char v1; // size=0, offset=5
	unsigned short tpage; // size=0, offset=6
	unsigned char u2; // size=0, offset=8
	unsigned char v2; // size=0, offset=9
	char pad1; // size=0, offset=10
	char sortPush; // size=0, offset=11
	long color; // size=0, offset=12
};

struct AniTexInfo // hashcode: 0x907C013D (dec: -1870921411)
{
	struct TextureMT3 *texture; // size=16, offset=0
	long numFrames; // size=0, offset=4
	long speed; // size=0, offset=8
};

struct AniTex // hashcode: 0xE4DA2729 (dec: -455465175)
{
	long numAniTextues; // size=0, offset=0
	struct AniTexInfo aniTexInfo; // size=12, offset=4
};

struct DrMoveAniTexSrcInfo // hashcode: 0x4AD6E543 (dec: 1255597379)
{
	short pixSrcX; // size=0, offset=0
	short pixSrcY; // size=0, offset=2
	short clutSrcX; // size=0, offset=4
	short clutSrcY; // size=0, offset=6
};

struct DrMoveAniTexDestInfo // hashcode: 0xF3A982EE (dec: -206994706)
{
	short pixDstX; // size=0, offset=0
	short pixDstY; // size=0, offset=2
	short pixW; // size=0, offset=4
	short pixH; // size=0, offset=6
	short clutDstX; // size=0, offset=8
	short clutDstY; // size=0, offset=10
	short clutW; // size=0, offset=12
	short clutH; // size=0, offset=14
	short pixCurrentX; // size=0, offset=16
	short pixCurrentY; // size=0, offset=18
	short clutCurrentX; // size=0, offset=20
	short clutCurrentY; // size=0, offset=22
	long numFrames; // size=0, offset=24
	long speed; // size=0, offset=28
	struct DrMoveAniTexSrcInfo frame; // size=8, offset=32
};

struct DrMoveAniTex // hashcode: 0x0820A5D4 (dec: 136357332)
{
	long numAniTextues; // size=0, offset=0
	struct DrMoveAniTexDestInfo *aniTexInfo; // size=40, offset=4
};

union PLAYER_76fake // Hashcode: 0x35EA4737 (dec: 904546103)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: player.o, player.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: player.o, player.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: player.o, player.o
};

struct _Model // hashcode: 0x9BCE1D24 (dec: -1680990940)
{
	long numVertices; // size=0, offset=0
	struct _MVertex *vertexList; // size=8, offset=4
	long numNormals; // size=0, offset=8
	struct _SVectorNoPad *normalList; // size=6, offset=12
	long numFaces; // size=0, offset=16
	struct _MFace *faceList; // size=12, offset=20
	long numSegments; // size=0, offset=24
	struct _Segment *segmentList; // size=24, offset=28
	struct AniTex *aniTextures; // size=16, offset=32
	short maxRad; // size=0, offset=36
	short pad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	struct MultiSpline *multiSpline; // size=0, offset=44
	struct TextureMT3 *startTextures; // size=16, offset=48
	struct TextureMT3 *endTextures; // size=16, offset=52
};

union PLAYER_77fake // Hashcode: 0xBB3AB3AC (dec: -1153780820)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct _PVertex // hashcode: 0x4AC0ECE8 (dec: 1254157544)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	long otz; // size=0, offset=4
};

struct _VertexPool // hashcode: 0x98F8C96E (dec: -1728525970)
{
	struct _PVertex vertex[528]; // size=4224, offset=0
	struct CVECTOR color[528]; // size=2112, offset=4224
};

struct _PrimPool // hashcode: 0x9529B283 (dec: -1792429437)
{
	long numPrims; // size=0, offset=0
	unsigned long *nextPrim; // size=0, offset=4
	unsigned long *lastPrim; // size=0, offset=8
	unsigned long prim[24000]; // size=96000, offset=12
};


struct _CameraKey // hashcode: 0xB9E6B724 (dec: -1176062172)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short id; // size=0, offset=6
	short rx; // size=0, offset=8
	short ry; // size=0, offset=10
	short rz; // size=0, offset=12
	short flags; // size=0, offset=14
	short tx; // size=0, offset=16
	short ty; // size=0, offset=18
	short tz; // size=0, offset=20
	short pad; // size=0, offset=22
};

struct Object // hashcode: 0xEC12E9AC (dec: -334304852)
{
	long oflags; // size=0, offset=0
	short id; // size=0, offset=4
	short sfxFileHandle; // size=0, offset=6
	short numModels; // size=0, offset=8
	short numAnims; // size=0, offset=10
	struct _Model **modelList; // size=56, offset=12
	struct _G2AnimKeylist_Type **animList; // size=20, offset=16
	short introDist; // size=0, offset=20
	short vvIntroDist; // size=0, offset=22
	short removeDist; // size=0, offset=24
	short vvRemoveDist; // size=0, offset=26
	void *data; // size=0, offset=28
	char *script; // size=0, offset=32
	char *name; // size=0, offset=36
	unsigned char *soundData; // size=0, offset=40
	long oflags2; // size=0, offset=44
	short sectionA; // size=0, offset=48
	short sectionB; // size=0, offset=50
	short sectionC; // size=0, offset=52
	short numberOfEffects; // size=0, offset=54
	struct ObjectEffect *effectList; // size=4, offset=56
	unsigned long *relocList; // size=0, offset=60
	void *relocModule; // size=0, offset=64
	struct VramSize vramSize; // size=8, offset=68
};

struct _BVertex // hashcode: 0xADD3BD2F (dec: -1378632401)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _BFace // hashcode: 0x69100F99 (dec: 1762660249)
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	short pad; // size=0, offset=6
	struct TextureFT3 *texture; // size=12, offset=8
};

struct _BGObject // hashcode: 0xEC69FB30 (dec: -328598736)
{
	short flags; // size=0, offset=0
	short numFaces; // size=0, offset=2
	struct _BVertex *vertexList; // size=8, offset=4
	struct _BFace *faceList; // size=12, offset=8
	short x; // size=0, offset=12
	short y; // size=0, offset=14
	short z; // size=0, offset=16
	short radius; // size=0, offset=18
	long numVertices; // size=0, offset=20
};

struct _HFace // hashcode: 0x30CA5CA6 (dec: 818568358)
{
	short v0; // size=0, offset=0
	short v1; // size=0, offset=2
	short v2; // size=0, offset=4
	unsigned char attr; // size=0, offset=6
	char pad; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short n0; // size=0, offset=10
};

struct _HSphere // hashcode: 0x8E7B7191 (dec: -1904512623)
{
	long attr; // size=0, offset=0
	unsigned char id; // size=0, offset=4
	unsigned char rank; // size=0, offset=5
	short pad; // size=0, offset=6
	struct _Position position; // size=6, offset=8
	unsigned short radius; // size=0, offset=14
	unsigned long radiusSquared; // size=0, offset=16
};

struct _HBox // hashcode: 0x47CD8BDA (dec: 1204653018)
{
	short flags; // size=0, offset=0
	unsigned char id; // size=0, offset=2
	unsigned char rank; // size=0, offset=3
	short minX; // size=0, offset=4
	short minY; // size=0, offset=6
	short minZ; // size=0, offset=8
	short maxX; // size=0, offset=10
	short maxY; // size=0, offset=12
	short maxZ; // size=0, offset=14
	short refMinX; // size=0, offset=16
	short refMinY; // size=0, offset=18
	short refMinZ; // size=0, offset=20
	short refMaxX; // size=0, offset=22
	short refMaxY; // size=0, offset=24
	short refMaxZ; // size=0, offset=26
};

struct _HPrim // hashcode: 0x988F5E11 (dec: -1735434735)
{
	unsigned char hpFlags; // size=0, offset=0
	unsigned char withFlags; // size=0, offset=1
	unsigned char type; // size=0, offset=2
	unsigned char segment; // size=0, offset=3
	union {
		struct _HSphere *hsphere; // size=20, offset=0
		struct _HBox *hbox; // size=28, offset=0
		struct _HFace *hface; // size=12, offset=0
	} data; // size=4, offset=4, found in object files: player.o, player.o
};

struct _HModel // hashcode: 0xBC680356 (dec: -1134034090)
{
	struct _HPrim *hPrimList; // size=8, offset=0
	long numHPrims; // size=0, offset=4
};

struct _HInfo // hashcode: 0x6AC9807F (dec: 1791590527)
{
	long numHFaces; // size=0, offset=0
	struct _HFace *hfaceList; // size=12, offset=4
	long numHSpheres; // size=0, offset=8
	struct _HSphere *hsphereList; // size=20, offset=12
	long numHBoxes; // size=0, offset=16
	struct _HBox *hboxList; // size=28, offset=20
};

struct _Segment // hashcode: 0x2F06BF28 (dec: 788971304)
{
	long flags; // size=0, offset=0
	short firstTri; // size=0, offset=4
	short lastTri; // size=0, offset=6
	short firstVertex; // size=0, offset=8
	short lastVertex; // size=0, offset=10
	short px; // size=0, offset=12
	short py; // size=0, offset=14
	short pz; // size=0, offset=16
	short parent; // size=0, offset=18
	struct _HInfo *hInfo; // size=24, offset=20
};



struct ObjectEffect // hashcode: 0x2892438D (dec: 680674189)
{
	unsigned char effectNumber; // size=0, offset=0
	unsigned char modifierList[3]; // size=3, offset=1
};

struct _TFace // hashcode: 0x85FE430F (dec: -2046934257)
{
	struct _Face face; // size=6, offset=0
	unsigned char attr; // size=0, offset=6
	char sortPush; // size=0, offset=7
	unsigned short normal; // size=0, offset=8
	unsigned short textoff; // size=0, offset=10
};

struct _TVertex // hashcode: 0x0883ABD5 (dec: 142846933)
{
	struct _Vertex vertex; // size=6, offset=0
	unsigned short rgb15; // size=0, offset=6
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
};

struct Intro // hashcode: 0x796E766D (dec: 2037282413)
{
	char name[16]; // size=16, offset=0
	long intronum; // size=0, offset=16
	long UniqueID; // size=0, offset=20
	struct _Rotation rotation; // size=8, offset=24
	struct _Position position; // size=6, offset=32
	short maxRad; // size=0, offset=38
	long maxRadSq; // size=0, offset=40
	long flags; // size=0, offset=44
	void *data; // size=0, offset=48
	struct _Instance *instance; // size=616, offset=52
	struct MultiSpline *multiSpline; // size=0, offset=56
	void *dsignal; // size=0, offset=60
	short specturalLightGroup; // size=0, offset=64
	short meshColor; // size=0, offset=66
	struct _Position spectralPosition; // size=6, offset=68
	short spad; // size=0, offset=74
};

struct _Terrain // hashcode: 0x5D541B4E (dec: 1565793102)
{
	short UnitChangeFlags; // size=0, offset=0
	short spad; // size=0, offset=2
	long lpad2; // size=0, offset=4
	long numIntros; // size=0, offset=8
	struct Intro *introList; // size=76, offset=12
	long numVertices; // size=0, offset=16
	long numFaces; // size=0, offset=20
	long numNormals; // size=0, offset=24
	struct _TVertex *vertexList; // size=12, offset=28
	struct _TFace *faceList; // size=12, offset=32
	struct _Normal *normalList; // size=8, offset=36
	struct DrMoveAniTex *aniList; // size=8, offset=40
	long pad; // size=0, offset=44
	void *StreamUnits; // size=0, offset=48
	struct TextureFT3 *StartTextureList; // size=12, offset=52
	struct TextureFT3 *EndTextureList; // size=12, offset=56
	struct _MorphVertex *MorphDiffList; // size=14, offset=60
	struct _MorphColor *MorphColorList; // size=2, offset=64
	long numBSPTrees; // size=0, offset=68
	struct BSPTree *BSPTreeArray; // size=36, offset=72
	short *morphNormalIdx; // size=0, offset=76
	struct _MultiSignal *signals; // size=904, offset=80
};

struct CDLight // hashcode: 0xD90757B8 (dec: -653830216)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Sphere sphere; // size=12, offset=12
};

struct _MorphVertex // hashcode: 0xAAB8DABE (dec: -1430725954)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short vindex; // size=0, offset=6
	short hx; // size=0, offset=8
	short hy; // size=0, offset=10
	short hz; // size=0, offset=12
};

struct _MorphColor // hashcode: 0xA6BA0326 (dec: -1497758938)
{
	short morphColor15; // size=0, offset=0
};

struct _SavedCinematic // hashcode: 0x1E29151B (dec: 506008859)
{
	struct _Position position; // size=6, offset=0
	struct _Position focusPoint; // size=6, offset=6
	struct _Position base; // size=6, offset=12
	struct _Position targetPos; // size=6, offset=18
	struct _Position targetFocusPoint; // size=6, offset=24
	struct _Position targetBase; // size=6, offset=30
	struct _Rotation baseRot; // size=8, offset=36
	struct _Rotation focusRotation; // size=8, offset=44
	struct _Rotation targetFocusRotation; // size=8, offset=52
	short focusDistance; // size=0, offset=60
	short targetFocusDistance; // size=0, offset=62
	struct _SVector focusPointVel; // size=8, offset=64
	struct _SVector focusPointAccl; // size=8, offset=72
	struct _SVector baseVel; // size=8, offset=80
	struct _SVector baseAccl; // size=8, offset=88
	long maxVel; // size=0, offset=96
	struct MultiSpline *posSpline; // size=0, offset=100
	struct MultiSpline *targetSpline; // size=0, offset=104
	struct Level *level; // size=0, offset=108
};



struct LightGroup // hashcode: 0xBB5FF847 (dec: -1151338425)
{
	struct MATRIX lightMatrix; // size=32, offset=0
	struct MATRIX colorMatrix; // size=32, offset=32
};

struct LightList // hashcode: 0xC8448BB7 (dec: -935031881)
{
	struct CVECTOR ambient; // size=4, offset=0
	long numLightGroups; // size=0, offset=4
	struct LightGroup *lightGroupList; // size=64, offset=8
};

struct PointLight // hashcode: 0xFEFDD2F2 (dec: -16919822)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Position position; // size=6, offset=12
	short radius; // size=0, offset=18
	long radiusSquared; // size=0, offset=20
};

struct SpotLight // hashcode: 0xEF8B4C2E (dec: -276083666)
{
	struct NodeType node; // size=8, offset=0
	unsigned char r; // size=0, offset=8
	unsigned char g; // size=0, offset=9
	unsigned char b; // size=0, offset=10
	unsigned char flags; // size=0, offset=11
	struct _Position centroid; // size=6, offset=12
	short radius; // size=0, offset=18
	long radiusSquared; // size=0, offset=20
	struct _Position position; // size=6, offset=24
	struct _Position direction; // size=6, offset=30
	short cosFalloffAngle; // size=0, offset=36
	short attenuationScale; // size=0, offset=38
};

struct LightInfo // hashcode: 0x2A95530E (dec: 714429198)
{
	long numPointLights; // size=0, offset=0
	struct PointLight pointLightPool[32]; // size=768, offset=4
	struct NodeType usedPointLightList; // size=8, offset=772
	struct NodeType freePointLightList; // size=8, offset=780
	struct NodeType activePointLightList; // size=8, offset=788
	long numSpotLights; // size=0, offset=796
	struct SpotLight spotLightPool[8]; // size=320, offset=800
	struct NodeType usedSpotLightList; // size=8, offset=1120
	struct NodeType freeSpotLightList; // size=8, offset=1128
	struct NodeType activeSpotLightList; // size=8, offset=1136
	long numSavedColors; // size=0, offset=1144
};

struct _VMOffset // hashcode: 0x680AC445 (dec: 1745536069)
{
	short dx; // size=0, offset=0
	short dy; // size=0, offset=2
	short dz; // size=0, offset=4
};

struct _VMColorOffset // hashcode: 0xFD3FB530 (dec: -46156496)
{
	char dr; // size=0, offset=0
	char dg; // size=0, offset=1
	char db; // size=0, offset=2
};

struct _VMInterpolated // hashcode: 0x3A40B5C0 (dec: 977319360)
{
	short tvIdx; // size=0, offset=0
	short startIdx; // size=0, offset=2
	short endIdx; // size=0, offset=4
	short time; // size=0, offset=6
};

struct _VMOffsetTable // hashcode: 0xE4A13881 (dec: -459196287)
{
	long numVMOffsets; // size=0, offset=0
	union {
		struct _VMOffset moveOffsets; // size=6, offset=0
		struct _VMColorOffset colorOffsets; // size=3, offset=0
	} offsets; // size=6, offset=4, found in object files: player.o, player.o
};

struct _VMObject // hashcode: 0x436E75EB (dec: 1131312619)
{
	unsigned short flags; // size=0, offset=0
	short bspIdx; // size=0, offset=2
	short materialIdx; // size=0, offset=4
	short spectralIdx; // size=0, offset=6
	short currentIdx; // size=0, offset=8
	short timeScale; // size=0, offset=10
	long timer; // size=0, offset=12
	struct _Position position; // size=6, offset=16
	short radius; // size=0, offset=22
	long radiusSquared; // size=0, offset=24
	long numVMOffsetTables; // size=0, offset=28
	struct _VMOffsetTable **vmoffsetTableList; // size=12, offset=32
	struct _VMOffsetTable *curVMOffsetTable; // size=12, offset=36
	long numVMVertices; // size=0, offset=40
	void *vmvertexList; // size=0, offset=44
	long numVMInterpolated; // size=0, offset=48
	struct _VMInterpolated *vminterpolatedList; // size=8, offset=52
	char *name; // size=0, offset=56
};

struct _VGroup // hashcode: 0x4421FB39 (dec: 1143077689)
{
	long id; // size=0, offset=0
	long numVertices; // size=0, offset=4
	struct _TVertex **vertexList; // size=12, offset=8
};

struct _HotSpot // hashcode: 0x6EC4F776 (dec: 1858402166)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short id; // size=0, offset=6
};

struct _PlanMkr // hashcode: 0xBC0DD043 (dec: -1139945405)
{
	struct _Position pos; // size=6, offset=0
	short id; // size=0, offset=6
};

struct _SFXMkr // hashcode: 0x74BB4848 (dec: 1958430792)
{
	unsigned char *soundData; // size=0, offset=0
	long uniqueID; // size=0, offset=4
	struct SoundInstance sfxTbl[4]; // size=12, offset=8
	struct _Position pos; // size=6, offset=20
	short pad; // size=0, offset=26
	long livesInOnePlace; // size=0, offset=28
	long inSpectral; // size=0, offset=32
};

struct TClassAttr // hashcode: 0x4DC02369 (dec: 1304437609)
{
	short flags; // size=0, offset=0
	unsigned short sound; // size=0, offset=2
};

struct PLAYER_114fake // hashcode: 0x5A1E67AD (dec: 1511942061)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: player.o, player.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: player.o, player.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: player.o, player.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: player.o, player.o
};

struct SpuVolume // hashcode: 0x1BD13981 (dec: 466696577)
{
	short left; // size=0, offset=0
	short right; // size=0, offset=2
};

struct SpuVoiceAttr // hashcode: 0x819E0A50 (dec: -2120349104)
{
	unsigned long voice; // size=0, offset=0
	unsigned long mask; // size=0, offset=4
	struct SpuVolume volume; // size=4, offset=8
	struct SpuVolume volmode; // size=4, offset=12
	struct SpuVolume volumex; // size=4, offset=16
	unsigned short pitch; // size=0, offset=20
	unsigned short note; // size=0, offset=22
	unsigned short sample_note; // size=0, offset=24
	short envx; // size=0, offset=26
	unsigned long addr; // size=0, offset=28
	unsigned long loop_addr; // size=0, offset=32
	long a_mode; // size=0, offset=36
	long s_mode; // size=0, offset=40
	long r_mode; // size=0, offset=44
	unsigned short ar; // size=0, offset=48
	unsigned short dr; // size=0, offset=50
	unsigned short sr; // size=0, offset=52
	unsigned short rr; // size=0, offset=54
	unsigned short sl; // size=0, offset=56
	unsigned short adsr1; // size=0, offset=58
	unsigned short adsr2; // size=0, offset=60
};

struct SpuExtAttr // hashcode: 0x58B0F474 (dec: 1487991924)
{
	struct SpuVolume volume; // size=4, offset=0
	long reverb; // size=0, offset=4
	long mix; // size=0, offset=8
};

struct SpuStVoiceAttr // hashcode: 0xAFDFBA7E (dec: -1344292226)
{
	char status; // size=0, offset=0
	char pad1; // size=0, offset=1
	char pad2; // size=0, offset=2
	char pad3; // size=0, offset=3
	long last_size; // size=0, offset=4
	unsigned long buf_addr; // size=0, offset=8
	unsigned long data_addr; // size=0, offset=12
};


struct GameTracker // hashcode: 0x4EB3EFC5 (dec: 1320415173)
{
	struct _GameData_Type gameData; // size=32, offset=0
	struct menu_t *menu; // size=0, offset=32
	struct memcard_t *memcard; // size=0, offset=36
	struct Level *level; // size=392, offset=40
	struct _Instance *playerInstance; // size=616, offset=44
	long drawPage; // size=0, offset=48
	struct _InstanceList *instanceList; // size=268, offset=52
	struct _InstancePool *instancePool; // size=38204, offset=56
	struct _VertexPool *vertexPool; // size=6336, offset=60
	struct _PrimPool *primPool; // size=96012, offset=64
	struct _ObjectTracker *GlobalObjects; // size=36, offset=68
	long controlCommand[2][5]; // size=40, offset=72
	long controlData[2][5]; // size=40, offset=112
	long overrideData[2][5]; // size=40, offset=152
	long debugFlags; // size=0, offset=192
	long debugFlags2; // size=0, offset=196
	struct CVECTOR wipeColor; // size=4, offset=200
	short wipeTime; // size=0, offset=204
	short maxWipeTime; // size=0, offset=206
	short wipeType; // size=0, offset=208
	short numGSignals; // size=0, offset=210
	void (*gSignal[16]); // size=64, offset=212
	struct LightInfo *lightInfo; // size=1148, offset=276
	void *reqDisp; // size=0, offset=280
	long *drawTimerReturn; // size=0, offset=284
	long usecsStartDraw; // size=0, offset=288
	void *disp; // size=0, offset=292
	unsigned long displayFrameCount; // size=0, offset=296
	unsigned long frameCount; // size=0, offset=300
	unsigned long fps30Count; // size=0, offset=304
	unsigned long vblFrames; // size=0, offset=308
	unsigned long vblCount; // size=0, offset=312
	long numMatrices; // size=0, offset=316
	long gameFlags; // size=0, offset=320
	long streamFlags; // size=0, offset=324
	void *drawNonAnimatedSegmentFunc; // size=0, offset=328
	void *drawAnimatedModelFunc; // size=0, offset=332
	void *drawDisplayPolytopeListFunc; // size=0, offset=336
	void *drawBgFunc; // size=0, offset=340
	struct Level *mainDrawUnit; // size=392, offset=344
	char baseAreaName[16]; // size=16, offset=348
	short levelDone; // size=0, offset=364
	short levelChange; // size=0, offset=366
	short hideBG; // size=0, offset=368
	short gameMode; // size=0, offset=370
	long currentHotSpot; // size=0, offset=372
	long StreamUnitID; // size=0, offset=376
	short SwitchToNewStreamUnit; // size=0, offset=380
	short SwitchToNewWarpIndex; // size=0, offset=382
	char S_baseAreaName[16]; // size=16, offset=384
	short toSignal; // size=0, offset=400
	short fromSignal; // size=0, offset=402
	char LastSignal; // size=0, offset=404
	short StreamNormalA; // size=0, offset=406
	short StreamNormalB; // size=0, offset=408
	short StreamNormalC; // size=0, offset=410
	long StreamNormalD; // size=0, offset=412
	long moveRazielToStreamID; // size=0, offset=416
	struct _ColorType animObjLine; // size=4, offset=420
	struct _ColorType animObjShade; // size=4, offset=424
	long maxIntroFXTime; // size=0, offset=428
	struct gSoundData sound; // size=20, offset=432
	short controllerMode; // size=0, offset=452
	unsigned char plan_collide_override; // size=0, offset=454
	unsigned char cheatMode; // size=0, offset=455
	char currentLvl; // size=0, offset=456
	char lastLvl; // size=0, offset=457
	struct Object *introFX; // size=76, offset=460
	struct Intro *introFXIntro; // size=76, offset=464
	unsigned long **drawOT; // size=0, offset=468
	unsigned long **dispOT; // size=0, offset=472
	struct P_TAG *savedOTStart; // size=8, offset=476
	struct P_TAG *savedOTEnd; // size=8, offset=480
	long introWaitTime; // size=0, offset=484
	long mirrorZPush; // size=0, offset=488
	long defVVRemoveDist; // size=0, offset=492
	long defRemoveDist; // size=0, offset=496
	struct _Position forcedStartPosition; // size=6, offset=500
	short hudCollDisplay; // size=0, offset=506
	long primMemUsed; // size=0, offset=508
	long cheatTimerCount; // size=0, offset=512
	long playerCheatFlags; // size=0, offset=516
	long savedPlayerCameraMode; // size=0, offset=520
	long debugDrawFlags; // size=0, offset=524
	void *planningPool; // size=0, offset=528
	void *enemyPlanPool; // size=0, offset=532
	unsigned char block_collide_override; // size=0, offset=536
	unsigned char raziel_collide_override; // size=0, offset=537
	short timeOfDay; // size=0, offset=538
	long decoupleGame; // size=0, offset=540
	long multGameTime; // size=0, offset=544
	short spectral_fadeValue; // size=0, offset=548
	short material_fadeValue; // size=0, offset=550
	unsigned long drawTime; // size=0, offset=552
	unsigned long currentTime; // size=0, offset=556
	unsigned long currentMaterialTime; // size=0, offset=560
	unsigned long currentSpectralTime; // size=0, offset=564
	unsigned long currentTimeOfDayTime; // size=0, offset=568
	unsigned long lastLoopTime; // size=0, offset=572
	unsigned long timeMult; // size=0, offset=576
	unsigned long globalTimeMult; // size=0, offset=580
	unsigned long spectralTimeMult; // size=0, offset=584
	unsigned long materialTimeMult; // size=0, offset=588
	unsigned long currentTicks; // size=0, offset=592
	unsigned long totalTime; // size=0, offset=596
	unsigned long idleTime; // size=0, offset=600
	long visibleInstances; // size=0, offset=604
	int gameFramePassed; // size=0, offset=608
	unsigned long timeSinceLastGameFrame; // size=0, offset=612
	long frameRateLock; // size=0, offset=616
	short frameRate24fps; // size=0, offset=620
	char monster_collide_override; // size=0, offset=622
	char pad; // size=0, offset=623
};



struct vecS // hashcode: 0xEADB8F3A (dec: -354709702)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

struct vecL // hashcode: 0x7575CB14 (dec: 1970653972)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
};

struct __MessageQueue // hashcode: 0x473062B5 (dec: 1194353333)
{
	int Head; // size=0, offset=0
	int Tail; // size=0, offset=4
	struct __Event Queue[16]; // size=128, offset=8
};

struct __State // hashcode: 0xA27CC1F7 (dec: -1568882185)
{
	void (*Process)(); // size=0, offset=0
	struct __MessageQueue Event; // size=136, offset=4
	struct __MessageQueue Defer; // size=136, offset=140
	int Data1; // size=0, offset=276
	int Data2; // size=0, offset=280
};

union PLAYER_208fake // Hashcode: 0xD87F8A52 (dec: -662730158)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct POLY_F4 // hashcode: 0xE2AC295D (dec: -492033699)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
	short x2; // size=0, offset=16
	short y2; // size=0, offset=18
	short x3; // size=0, offset=20
	short y3; // size=0, offset=22
};

union DEBUG_118fake // Hashcode: 0x841D74D1 (dec: -2078444335)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: debug.o, debug.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: debug.o, debug.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: debug.o, debug.o
};

struct _SmallRotation // hashcode: 0xB9CA5EE3 (dec: -1177919773)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
};

union DEBUG_119fake // Hashcode: 0xED3B4C32 (dec: -314880974)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct EventPointers // hashcode: 0x3787E212 (dec: 931652114)
{
	long numPuzzles; // size=0, offset=0
	struct Event (*eventInstances[1]); // size=4, offset=4
};

struct _ButtonTexture // hashcode: 0x9E750D8C (dec: -1636495988)
{
	short tpage; // size=0, offset=0
	short clut; // size=0, offset=2
	short textureW; // size=0, offset=4
	short textureH; // size=0, offset=6
	struct _BlockVramEntry *vramBlock; // size=0, offset=8
	long xshift; // size=0, offset=12
};

struct FontChar // hashcode: 0x5A2F9F5B (dec: 1513070427)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	char c; // size=0, offset=4
	char color; // size=0, offset=5
};

struct _BigFileEntry // hashcode: 0xA904436B (dec: -1459338389)
{
	long fileHash; // size=0, offset=0
	long fileLen; // size=0, offset=4
	long filePos; // size=0, offset=8
	long checkSumFull; // size=0, offset=12
};

struct _BigFileDirEntry // hashcode: 0x7D70C603 (dec: 2104542723)
{
	short streamUnitID; // size=0, offset=0
	short numFiles; // size=0, offset=2
	long subDirOffset; // size=0, offset=4
};

struct _BigFileDir // hashcode: 0x4E39D3E2 (dec: 1312412642)
{
	long numFiles; // size=0, offset=0
	struct _BigFileEntry fileList[0]; // size=0, offset=4
};

struct _BigFile // hashcode: 0x50C273F3 (dec: 1354920947)
{
	long bigfileBaseOffset; // size=0, offset=0
	struct _BigFileDir *rootDir; // size=4, offset=4
	struct _BigFileDir *currentDir; // size=4, offset=8
	struct _BigFileDir *cachedDir; // size=4, offset=12
	long currentDirID; // size=0, offset=16
	long cachedDirID; // size=0, offset=20
	long searchDirID; // size=0, offset=24
	long numSubDirs; // size=0, offset=28
	struct _BigFileDirEntry *subDirList; // size=8, offset=32
};

struct _NonBlockLoadEntry // hashcode: 0x722059B4 (dec: 1914722740)
{
#ifdef PSX_VERSION
	long fileHash; // size=0, offset=0
	long dirHash; // size=0, offset=4
	long filePos; // size=0, offset=8
	long* loadAddr; // size=0, offset=12
	long loadSize; // size=0, offset=16
	long checksum; // size=0, offset=20
	long checksumType; // size=0, offset=24
	long memType; // size=0, offset=28
	long posInFile; // size=0, offset=32
	void* retFunc; // size=0, offset=36
	void* retData; // size=0, offset=40
	void* retData2; // size=0, offset=44
	void** retPointer; // size=0, offset=48
	char fileName[48]; // size=48, offset=52
#else
	long fileHash; // size=0, offset=0
	long dirHash; // size=0, offset=4
	long filePos; // size=0, offset=8
	long *loadAddr; // size=0, offset=12
	long loadSize; // size=0, offset=16
	long checksum; // size=0, offset=20
	long checksumType; // size=0, offset=24
	long memType; // size=0, offset=28
	long posInFile; // size=0, offset=32
	long mallocFailOk; // size=0, offset=36
	void *retFunc; // size=0, offset=40
	void *retData; // size=0, offset=44
	void *retData2; // size=0, offset=48
	void **retPointer; // size=0, offset=52
#endif
};

struct _ReadQueueEntry // hashcode: 0x5360DCDA (dec: 1398856922)
{
	long readStartPos; // size=0, offset=0
	long readSize; // size=0, offset=4
	long readCurSize; // size=0, offset=8
	void *readStartDest; // size=0, offset=12
	void *readCurDest; // size=0, offset=16
	long readStatus; // size=0, offset=20
	long checksum; // size=0, offset=24
	long checksumType; // size=0, offset=28
	void *retFunc; // size=0, offset=32
	void *retData; // size=0, offset=36
	void *retData2; // size=0, offset=40
};

struct BASIC_LINE_G2 // hashcode: 0x684545B2 (dec: 1749370290)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	unsigned char r1; // size=0, offset=8
	unsigned char g1; // size=0, offset=9
	unsigned char b1; // size=0, offset=10
	unsigned char p1; // size=0, offset=11
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
};

struct _BASIC_G4 // hashcode: 0xC15AD9EF (dec: -1051010577)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	unsigned char r1; // size=0, offset=8
	unsigned char g1; // size=0, offset=9
	unsigned char b1; // size=0, offset=10
	unsigned char pad1; // size=0, offset=11
	short x1; // size=0, offset=12
	short y1; // size=0, offset=14
	unsigned char r2; // size=0, offset=16
	unsigned char g2; // size=0, offset=17
	unsigned char b2; // size=0, offset=18
	unsigned char pad2; // size=0, offset=19
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	unsigned char r3; // size=0, offset=24
	unsigned char g3; // size=0, offset=25
	unsigned char b3; // size=0, offset=26
	unsigned char pad3; // size=0, offset=27
	short x3; // size=0, offset=28
	short y3; // size=0, offset=30
};

struct _BASIC_F4 // hashcode: 0x2871008E (dec: 678494350)
{
	unsigned char r0; // size=0, offset=0
	unsigned char g0; // size=0, offset=1
	unsigned char b0; // size=0, offset=2
	unsigned char code; // size=0, offset=3
	short x0; // size=0, offset=4
	short y0; // size=0, offset=6
	short x1; // size=0, offset=8
	short y1; // size=0, offset=10
	short x2; // size=0, offset=12
	short y2; // size=0, offset=14
	short x3; // size=0, offset=16
	short y3; // size=0, offset=18
};

struct __BubbleParams // hashcode: 0xE483C494 (dec: -461126508)
{
	short DisperseFrames; // size=0, offset=0
	short KillScale; // size=0, offset=2
	short MinSplashSize; // size=0, offset=4
	short MaxSpeed; // size=0, offset=6
	short MaxSpeedRange; // size=0, offset=8
	short ScaleRate; // size=0, offset=10
	short ScaleRateRange; // size=0, offset=12
	short StartScale; // size=0, offset=14
	short StartScaleRange; // size=0, offset=16
	short UniqueBubbles; // size=0, offset=18
};

struct _GenericParticleParams // hashcode: 0x7A6370AD (dec: 2053337261)
{
	short size; // size=0, offset=0
	unsigned char StartOnInit; // size=0, offset=2
	unsigned char type; // size=0, offset=3
	short birthRadius; // size=0, offset=4
	char startSegment; // size=0, offset=6
	char endSegment; // size=0, offset=7
	struct _Position direction; // size=6, offset=8
	unsigned char spectral_colorize; // size=0, offset=14
	unsigned char absolute_direction; // size=0, offset=15
	char accx; // size=0, offset=16
	char accy; // size=0, offset=17
	char accz; // size=0, offset=18
	char useInstanceModel; // size=0, offset=19
	unsigned char startColor_red; // size=0, offset=20
	unsigned char startColor_green; // size=0, offset=21
	unsigned char startColor_blue; // size=0, offset=22
	char model; // size=0, offset=23
	unsigned char endColor_red; // size=0, offset=24
	unsigned char endColor_green; // size=0, offset=25
	unsigned char endColor_blue; // size=0, offset=26
	char texture; // size=0, offset=27
	short lifeTime; // size=0, offset=28
	unsigned char primLifeTime; // size=0, offset=30
	char use_child; // size=0, offset=31
	short startFadeValue; // size=0, offset=32
	short fadeStep; // size=0, offset=34
	char numberBirthParticles; // size=0, offset=36
	char z_undulation_mode; // size=0, offset=37
	short scaleSpeed; // size=0, offset=38
	struct _Position offset; // size=6, offset=40
	short startScale; // size=0, offset=46
};

struct _GenericRibbonParams // hashcode: 0xD243E3C9 (dec: -767302711)
{
	char id; // size=0, offset=0
	char StartOnInit; // size=0, offset=1
	char startSegment; // size=0, offset=2
	char endSegment; // size=0, offset=3
	char type; // size=0, offset=4
	char use_child; // size=0, offset=5
	short ribbonLifeTime; // size=0, offset=6
	short faceLifeTime; // size=0, offset=8
	short startFadeValue; // size=0, offset=10
	long startColor; // size=0, offset=12
	long endColor; // size=0, offset=16
};

struct _GenericGlowParams // hashcode: 0xF67692B8 (dec: -160001352)
{
	char StartOnInit; // size=0, offset=0
	unsigned char segment; // size=0, offset=1
	unsigned char segmentEnd; // size=0, offset=2
	char numSegments; // size=0, offset=3
	unsigned char color_num; // size=0, offset=4
	char use_child; // size=0, offset=5
	unsigned char numColors; // size=0, offset=6
	char id; // size=0, offset=7
	long atuColorCycleRate; // size=0, offset=8
	short width; // size=0, offset=12
	short height; // size=0, offset=14
	long lifetime; // size=0, offset=16
	short fadein_time; // size=0, offset=20
	short fadeout_time; // size=0, offset=22
};

struct _GenericLightningParams // hashcode: 0x226C68CF (dec: 577530063)
{
	char type; // size=0, offset=0
	char use_child; // size=0, offset=1
	short lifeTime; // size=0, offset=2
	short deg; // size=0, offset=4
	short deg_inc; // size=0, offset=6
	struct _Position start_offset; // size=6, offset=8
	char startSeg; // size=0, offset=14
	char endSeg; // size=0, offset=15
	struct _Position end_offset; // size=6, offset=16
	char matrixSeg; // size=0, offset=22
	unsigned char segs; // size=0, offset=23
	short width; // size=0, offset=24
	short small_width; // size=0, offset=26
	short sine_size; // size=0, offset=28
	short variation; // size=0, offset=30
	long color; // size=0, offset=32
	long glow_color; // size=0, offset=36
};

struct _GenericBlastringParams // hashcode: 0xAB3B2462 (dec: -1422187422)
{
	char type; // size=0, offset=0
	char use_child; // size=0, offset=1
	short lifeTime; // size=0, offset=2
	struct _Position offset; // size=6, offset=4
	short matrixSeg; // size=0, offset=10
	char segment; // size=0, offset=12
	char sortInWorld; // size=0, offset=13
	short radius; // size=0, offset=14
	short size1; // size=0, offset=16
	short size2; // size=0, offset=18
	short endRadius; // size=0, offset=20
	short colorchange_radius; // size=0, offset=22
	int vel; // size=0, offset=24
	int accl; // size=0, offset=28
	short height1; // size=0, offset=32
	short height2; // size=0, offset=34
	short height3; // size=0, offset=36
	char predator_offset; // size=0, offset=38
	char stay_in_place; // size=0, offset=39
	long startColor; // size=0, offset=40
	long endColor; // size=0, offset=44
};

struct _GenericFlashParams // hashcode: 0x06D8DB60 (dec: 114875232)
{
	short type; // size=0, offset=0
	short timeToColor; // size=0, offset=2
	long color; // size=0, offset=4
	short timeAtColor; // size=0, offset=8
	short timeFromColor; // size=0, offset=10
};

union DEBUG_224fake // Hashcode: 0x0131C19B (dec: 20038043)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct SavedBasic // hashcode: 0x6198E1EF (dec: 1637409263)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
};

struct _SavedIntro // hashcode: 0x8389D216 (dec: -2088119786)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	unsigned char lightGroup; // size=0, offset=2
	unsigned char specturalLightGroup; // size=0, offset=3
	char name[8]; // size=8, offset=4
	short introUniqueID; // size=0, offset=12
	short streamUnitID; // size=0, offset=14
	short birthUnitID; // size=0, offset=16
	unsigned short attachedUniqueID; // size=0, offset=18
	struct _SmallRotation smallRotation; // size=6, offset=20
	struct _Position position; // size=6, offset=26
	long flags; // size=0, offset=32
	long flags2; // size=0, offset=36
};

struct SavedEvent // hashcode: 0x429B6172 (dec: 1117479282)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short eventNumber; // size=0, offset=4
	short eventVariables[5]; // size=10, offset=6
};

struct SavedBspTree // hashcode: 0x7AE7209C (dec: 2061967516)
{
	struct _Position localOffset; // size=6, offset=0
	unsigned char importantFlagsSaved; // size=0, offset=6
	unsigned char bspIndex; // size=0, offset=7
};





struct EventWildCardObject // hashcode: 0x24FF3630 (dec: 620705328)
{
	short id; // size=0, offset=0
	short spad; // size=0, offset=2
	long unitID; // size=0, offset=4
	char *objectName; // size=0, offset=8
};

struct Vector3d // hashcode: 0x10BF06FC (dec: 280954620)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
	short vz; // size=0, offset=4
	short pad1; // size=0, offset=6
	short errorx; // size=0, offset=8
	short errory; // size=0, offset=10
	short errorz; // size=0, offset=12
	short pad2; // size=0, offset=14
	long streamUnitID; // size=0, offset=16
};

struct Rotation3d // hashcode: 0x646160C4 (dec: 1684103364)
{
	short vx; // size=0, offset=0
	short vy; // size=0, offset=2
	short vz; // size=0, offset=4
	short pad1; // size=0, offset=6
	short errorx; // size=0, offset=8
	short errory; // size=0, offset=10
	short errorz; // size=0, offset=12
	short pad2; // size=0, offset=14
	long attribute; // size=0, offset=16
};

struct InstanceObject // hashcode: 0xA445CB1D (dec: -1538929891)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct EventObject // hashcode: 0x38B0B53D (dec: 951104829)
{
	struct Event *event; // size=28, offset=0
	long attribute; // size=0, offset=4
};

struct SavedEventObject // hashcode: 0xC69C23B2 (dec: -962845774)
{
	struct SavedBasic *savedEvent; // size=2, offset=0
	long attribute; // size=0, offset=4
	long areaID; // size=0, offset=8
	long eventNumber; // size=0, offset=12
};

struct GameObject // hashcode: 0xFA1A30A9 (dec: -98946903)
{
	long attribute; // size=0, offset=0
};

struct SignalObject // hashcode: 0x42CFA9E2 (dec: 1120905698)
{
	struct _MultiSignal *msignal; // size=904, offset=0
	long attribute; // size=0, offset=4
};

struct IntroObject // hashcode: 0x13CCBC8C (dec: 332184716)
{
	struct Intro *intro; // size=76, offset=0
	long attribute; // size=0, offset=4
};

struct InstanceAnimate // hashcode: 0x200AA21A (dec: 537567770)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct InstanceAnimateTexture // hashcode: 0x5266C0BB (dec: 1382465723)
{
	struct _Instance *instance; // size=616, offset=0
	struct AniTex *aniTextures; // size=16, offset=4
	long attribute; // size=0, offset=8
};

struct ListObject // hashcode: 0x1323C7B3 (dec: 321111987)
{
	struct EventWildCardObject *eventInstance; // size=12, offset=0
	long attribute[5]; // size=20, offset=4
	long numberOfAttributes; // size=0, offset=24
	long lineID; // size=0, offset=28
};

struct SubListObject // hashcode: 0xFDCC28E5 (dec: -36951835)
{
	struct _Instance **instanceList; // size=616, offset=0
	long numberOfInstances; // size=0, offset=4
	long numberOfAttributes; // size=0, offset=8
	long attribute[5]; // size=20, offset=12
};

struct TGroupObject // hashcode: 0x669B4B67 (dec: 1721453415)
{
	struct BSPTree *bspTree; // size=36, offset=0
	struct _StreamUnit *streamUnit; // size=64, offset=4
	long attribute; // size=0, offset=8
	short arg[2]; // size=4, offset=12
};

struct ConstrictInfo // hashcode: 0xA586BA8A (dec: -1517897078)
{
	struct _Instance *instance; // size=616, offset=0
	long attribute; // size=0, offset=4
};

struct CameraObject // hashcode: 0xD3BF3ACE (dec: -742442290)
{
	struct Camera *camera; // size=1220, offset=0
	long frames; // size=0, offset=4
	long attribute; // size=0, offset=8
};

struct EventVmObject // hashcode: 0x9DE5D8F8 (dec: -1645881096)
{
	struct _VMObject *vmObjectPtr; // size=60, offset=0
	struct Level *level; // size=392, offset=4
	long attribute; // size=0, offset=8
};

struct AreaObject // hashcode: 0x38AC5A77 (dec: 950819447)
{
	struct _StreamUnit *streamUnit; // size=64, offset=0
	long attribute; // size=0, offset=4
	long unitID; // size=0, offset=8
};

struct SoundObject // hashcode: 0x9DA5ECE7 (dec: -1650070297)
{
	short flags; // size=0, offset=0
	short attribute; // size=0, offset=2
	long soundNumber; // size=0, offset=4
	long value; // size=0, offset=8
	long duration; // size=0, offset=12
	union {
		struct _Instance *instance; // size=616, offset=0
		struct _SFXMkr *sfxMarker; // size=36, offset=0
	} data; // size=4, offset=16, found in object files: debug.o, debug.o
};

struct StackType // hashcode: 0x9CE4C465 (dec: -1662729115)
{
	long id; // size=0, offset=0
	union {
		struct {
			long instanceNumber; // size=0, offset=0
			long attribute; // size=0, offset=4
		} Object; // size=8, offset=0, found in object files: debug.o, debug.o
		struct {
			struct _SavedIntro *intro; // size=40, offset=0
			long attribute; // size=0, offset=4
		} SavedIntroObject; // size=8, offset=0, found in object files: debug.o, debug.o
		struct InstanceSpline instanceSpline; // size=24, offset=0
		struct {
			long currentNumber; // size=0, offset=0
			long error; // size=0, offset=4
			short flags; // size=0, offset=8
		} Number; // size=12, offset=0, found in object files: debug.o, debug.o
		struct {
			short vx; // size=0, offset=0
			short vy; // size=0, offset=2
		} Vector2d; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			char *pointer; // size=0, offset=0
		} CharPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			short *pointer; // size=0, offset=0
		} ShortPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct {
			long *pointer; // size=0, offset=0
		} LongPointer; // size=4, offset=0, found in object files: debug.o, debug.o
		struct AreaObject areaObject; // size=12, offset=0
		struct InstanceObject instanceObject; // size=8, offset=0
		struct Vector3d vector3d; // size=20, offset=0
		struct Rotation3d rotation3d; // size=20, offset=0
		struct SignalObject signalObject; // size=8, offset=0
		struct EventObject eventObject; // size=8, offset=0
		struct GameObject gameObject; // size=4, offset=0
		struct ListObject listObject; // size=32, offset=0
		struct IntroObject introObject; // size=8, offset=0
		struct InstanceAnimate instanceAnimate; // size=8, offset=0
		struct InstanceAnimateTexture instanceAniTexture; // size=12, offset=0
		struct SavedEventObject savedEventObject; // size=16, offset=0
		struct SubListObject subListObject; // size=32, offset=0
		struct TGroupObject terrainGroup; // size=16, offset=0
		struct ConstrictInfo constrictInfo; // size=8, offset=0
		struct CameraObject cameraObject; // size=12, offset=0
		struct EventVmObject vmObject; // size=12, offset=0
		struct SoundObject soundObject; // size=20, offset=0
	} data; // size=32, offset=4, found in object files: debug.o, debug.o
};



enum option_ctrl_t // Hashcode: 0xEBCEABAC (dec: -338777172)
{
	option_ctrl_bogus = 0,
	option_ctrl_none = 1,
	option_ctrl_select = 2,
	option_ctrl_left = 3,
	option_ctrl_right = 4,
	option_ctrl_num_states = 5,
};

typedef struct POLY_F4 POLY_F4;

typedef struct _SmallRotation SmallRotation;

typedef struct Camera Camera;

typedef struct DebugMenuLine DebugMenuLine;

typedef struct _ButtonTexture ButtonTexture;

typedef struct FontChar FontChar;

typedef struct _BigFileEntry BigFileEntry;

typedef struct _BigFileDirEntry BigFileDirEntry;

typedef struct _BigFileDir BigFileDir;

typedef struct _BigFile BigFile;

typedef struct _NonBlockLoadEntry NonBlockLoadEntry;

typedef struct _ReadQueueEntry ReadQueueEntry;

typedef struct _FX_MATRIX FX_MATRIX;

typedef struct _FX_PRIM FX_PRIM;

typedef struct BASIC_LINE_G2 BASIC_LINE_G2;

typedef struct _BASIC_G4 BASIC_G4;

typedef struct _BASIC_F4 BASIC_F4;

typedef struct __BubbleParams BubbleParams;

typedef struct _GenericParticleParams GenericParticleParams;

typedef struct _GenericRibbonParams GenericRibbonParams;

typedef struct _GenericGlowParams GenericGlowParams;

typedef struct _GenericLightningParams GenericLightningParams;

typedef struct _GenericBlastringParams GenericBlastringParams;

typedef struct _GenericFlashParams GenericFlashParams;

typedef struct _StreamUnit StreamUnit;

typedef struct XAFileInfo XAFileInfo;

typedef struct CdCommand CdCommand;

typedef struct VoiceCommand VoiceCommand;

typedef struct _LoadQueueEntry LoadQueueEntry;

typedef struct _BlockVramEntry BlockVramEntry;

typedef struct SavedBasic SavedBasic;

typedef struct _SavedIntro SavedIntro;

typedef struct SavedEvent SavedEvent;

typedef struct SavedBspTree SavedBspTree;

typedef struct EventBasicObject EventBasicObject;

typedef struct EventWildCardObject EventWildCardObject;

typedef struct ScriptPCode ScriptPCode;

typedef struct Event Event;

typedef struct EventPointers EventPointers;

typedef struct InstanceSpline InstanceSpline;

typedef struct Vector3d Vector3d;

typedef struct Rotation3d Rotation3d;

typedef struct InstanceObject InstanceObject;

typedef struct EventObject EventObject;

typedef struct SavedEventObject SavedEventObject;

typedef struct GameObject GameObject;

typedef struct SignalObject SignalObject;

typedef struct IntroObject IntroObject;

typedef struct InstanceAnimate InstanceAnimate;

typedef struct InstanceAnimateTexture InstanceAnimateTexture;

typedef struct ListObject ListObject;

typedef struct SubListObject SubListObject;

typedef struct TGroupObject TGroupObject;

typedef struct ConstrictInfo ConstrictInfo;

typedef struct CameraObject CameraObject;

typedef struct EventVmObject EventVmObject;

typedef struct AreaObject AreaObject;

typedef struct SoundObject SoundObject;

typedef struct StackType StackType;

typedef struct __ReaverSaveDat ReaverSaveData;

typedef enum option_ctrl_t option_ctrl_t;

typedef struct GameCheat GameCheat;

typedef struct debug_dispatch_t debug_dispatch_t;

typedef struct debug_format_t debug_format_t;

typedef void FX_PRIM_MODIFY_PROCESS();

typedef void FX_PROCESS();

typedef void FX_PARTICLE_PROCESS();

typedef void FX_GENERAL_PROCESS();

typedef void FX_SETUP();

typedef long TFract;

typedef void (*LoadReturnProc)();

typedef void (*BufferedLoadReturnProc)();

typedef void option_fn_t();

typedef int menu_fn_t();

typedef unsigned char CheatKey;

typedef void debug_dispatch_fn_t();

union SIGNAL_71fake // Hashcode: 0x60202F31 (dec: 1612721969)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: signal.o, signal.o
};

union SIGNAL_72fake // Hashcode: 0x4AF92C48 (dec: 1257843784)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SIGNAL_109fake // hashcode: 0xD12486F0 (dec: -786135312)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: signal.o, signal.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: signal.o, signal.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: signal.o, signal.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: signal.o, signal.o
};

union SIGNAL_206fake // Hashcode: 0x2E97F42B (dec: 781710379)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct SignalInfo // hashcode: 0x5899EF0F (dec: 1486483215)
{
	long (*signalHandleFunc)(struct _Instance* instance, struct Signal* signal); // size=0, offset=0
	short length; // size=0, offset=4
	short onlyPlayer; // size=0, offset=6
	void (*signalRelocateFunc)(struct Signal* signal, int offset); // size=0, offset=8
};

typedef struct _FXTracker FXTracker;

typedef struct SignalInfo SignalInfo;

typedef void BSPLeafFunc();

typedef void SignalGlobalValueFunc();

typedef long HandleSignalFunc();

typedef long SignalHandleFunc();

typedef void RelocateSignalFunc();

union COLLIDE_76fake // Hashcode: 0x3AA65B79 (dec: 983980921)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: collide.o, collide.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: collide.o, collide.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: collide.o, collide.o
};

struct _Vector // hashcode: 0x5DEB6D24 (dec: 1575709988)
{
	long x; // size=0, offset=0
	long y; // size=0, offset=4
	long z; // size=0, offset=8
};

struct _Triangle2D // hashcode: 0xD422B451 (dec: -735923119)
{
	short x0; // size=0, offset=0
	short y0; // size=0, offset=2
	short x1; // size=0, offset=4
	short y1; // size=0, offset=6
	short x2; // size=0, offset=8
	short y2; // size=0, offset=10
};

union COLLIDE_77fake // Hashcode: 0xCA00C1AF (dec: -905920081)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct _PCollideInfo // hashcode: 0x32613C4B (dec: 845233227)
{
	short type; // size=0, offset=0
	short segment; // size=0, offset=2
	void *prim; // size=0, offset=4
	struct _Instance *inst; // size=0, offset=8
	SVECTORwNormal; // size=8, offset=12
	SVECTOR*newPoint; // size=8, offset=20
	SVECTOR*oldPoint; // size=8, offset=24
	SVECTORcldPoint; // size=8, offset=28
	long collideType; // size=0, offset=36
	struct _Instance *instance; // size=0, offset=40
};

struct _HNormal // hashcode: 0xC53FB2BB (dec: -985681221)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

struct _HFaceInfo // hashcode: 0x0E79FF8C (dec: 242876300)
{
	long flags; // size=0, offset=0
	long attr; // size=0, offset=4
	struct _HFace *hface; // size=12, offset=8
	struct _SVector normal; // size=8, offset=12
	struct _HVertex *vertex0; // size=12, offset=20
	struct _HVertex *vertex1; // size=12, offset=24
	struct _HVertex *vertex2; // size=12, offset=28
};

struct _CollideInfo // hashcode: 0xCA2DF5E9 (dec: -902957591)
{
	void *level; // size=0, offset=0
	char flags; // size=0, offset=4
	char segment; // size=0, offset=5
	char type0; // size=0, offset=6
	char type1; // size=0, offset=7
	void *prim0; // size=0, offset=8
	void *prim1; // size=0, offset=12
	void *inst0; // size=0, offset=16
	void *inst1; // size=0, offset=20
	SVECTORpoint0; // size=8, offset=24
	SVECTORpoint1; // size=8, offset=32
	struct _Position offset; // size=6, offset=40
	short bspID; // size=0, offset=46
};

struct _LCollideInfo // hashcode: 0xCC1DB336 (dec: -870468810)
{
	struct _TFace *tface; // size=12, offset=0
	struct _Terrain *terrain; // size=84, offset=4
	int curTree; // size=0, offset=8
};

struct SCollideInfo // hashcode: 0x0DEAD21F (dec: 233493023)
{
	struct _Sphere *sphere; // size=12, offset=0
	SVECTOR*oldPos; // size=8, offset=4
	void (*collideFunc)(); // size=0, offset=8
	long id; // size=0, offset=12
	long segment; // size=0, offset=16
	struct _Instance *instance; // size=616, offset=20
	void *prim; // size=0, offset=24
};

union COLLIDE_218fake // Hashcode: 0x489C3F73 (dec: 1218199411)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union COLLIDE_246fake // Hashcode: 0x095377A5 (dec: 156465061)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

struct PandTScratch // hashcode: 0x53B2A743 (dec: 1404217155)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _SVector normal; // size=8, offset=32
	struct _Vector dpv; // size=12, offset=40
	struct _SVector planePoint; // size=8, offset=52
	long ignore_attr; // size=0, offset=60
	long accept_attr; // size=0, offset=64
	short *backface_flag; // size=0, offset=68
	struct _SVector newPos; // size=8, offset=72
	struct _SVector oldPos; // size=8, offset=80
	int flags; // size=0, offset=88
	int i; // size=0, offset=92
	int slack; // size=0, offset=96
	long in_spectral; // size=0, offset=100
	short *normalList; // size=0, offset=104
	struct _TVertex *vertexList; // size=12, offset=108
	struct _TFace *result; // size=12, offset=112
	struct _SVector line; // size=8, offset=116
};

struct COLLIDE_258fake // hashcode: 0x1D802093 (dec: 494936211)
{
	long m[3]; // size=12, offset=0
};

struct SandHFScratch // hashcode: 0x31C4E20D (dec: 834986509)
{
	struct _SVector normal; // size=8, offset=0
	struct MATRIX posMatrix; // size=32, offset=8
	struct _Vector dpv; // size=12, offset=40
	struct _SVector triPoint; // size=8, offset=52
	struct _SVector planePoint; // size=8, offset=60
};

struct SandTScratch // hashcode: 0x2773448F (dec: 661865615)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _Vector dpv; // size=12, offset=32
	struct _HNormal *normalList; // size=8, offset=44
	struct _CollideInfo collideInfo; // size=48, offset=48
	struct _HFaceInfo hfaceInfo; // size=32, offset=96
	struct _SVector midPoint; // size=8, offset=128
	struct _SVector spherePos; // size=8, offset=136
	void (*collideFunc)(); // size=0, offset=144
	struct _Instance *instance; // size=616, offset=148
	long edge; // size=0, offset=152
	long in_spectral; // size=0, offset=156
	struct _SVector oldPos; // size=8, offset=160
	void *prim; // size=0, offset=168
	struct _SVector normal; // size=8, offset=172
	struct _Sphere sphere; // size=12, offset=180
	short result; // size=0, offset=192
	short i; // size=0, offset=194
	long collide_ignoreAttr; // size=0, offset=196
	long collide_acceptAttr; // size=0, offset=200
	long midRadius; // size=0, offset=204
	struct _TVertex *vertexList; // size=12, offset=208
};

struct IandTScratch // hashcode: 0xE0F1425D (dec: -521059747)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _SVector normal; // size=8, offset=32
	struct _Vector dpv; // size=12, offset=40
	struct _SVector planePoint; // size=8, offset=52
	struct _SVector line; // size=8, offset=60
	struct _SVector oldPos; // size=8, offset=68
	struct _SVector newPos; // size=8, offset=76
	int i; // size=0, offset=84
	long in_spectral; // size=0, offset=88
	short *normalList; // size=0, offset=92
	struct _TVertex *vertexList; // size=12, offset=96
};

struct PandTFScratch // hashcode: 0x218F0B73 (dec: 563022707)
{
	struct MATRIX posMatrix; // size=32, offset=0
	struct _SVector planePoint; // size=8, offset=32
	struct _SVector newPos; // size=8, offset=40
	struct _SVector oldPos; // size=8, offset=48
	struct _SVector normal; // size=8, offset=56
	struct _Vector dpv; // size=12, offset=64
};

typedef struct _Vector Vector;

typedef struct _Triangle2D Triangle2D;

typedef struct _PCollideInfo PCollideInfo;

typedef struct _HNormal HNormal;

typedef struct _HFaceInfo HFaceInfo;

typedef struct _CollideInfo CollideInfo;

typedef struct _LCollideInfo LCollideInfo;

typedef struct SCollideInfo SCollideInfo;

typedef long CollideBoundFunc();

union CAMERA_71fake // Hashcode: 0x0F6F6CC4 (dec: 258960580)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: camera.o, camera.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: camera.o, camera.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: camera.o, camera.o
};

union CAMERA_72fake // Hashcode: 0x598765BC (dec: 1502045628)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct CameraCollisionInfo // hashcode: 0x76C89E59 (dec: 1992859225)
{
	struct _TFace (*tfaceList[5]); // size=20, offset=0
	struct _Terrain (*tfaceTerrain[5]); // size=20, offset=20
	short bspTree[6]; // size=12, offset=40
	short lengthList[6]; // size=12, offset=52
	long flags; // size=0, offset=64
	long line; // size=0, offset=68
	long numCollided; // size=0, offset=72
	long lenCenterToExtend; // size=0, offset=76
	struct _Sphere *start; // size=12, offset=80
	struct _Sphere *end; // size=12, offset=84
	long cldLines; // size=0, offset=88
};

union CAMERA_206fake // Hashcode: 0x6CD0C7C3 (dec: 1825621955)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

typedef struct CameraCollisionInfo CameraCollisionInfo;

struct POLY_GT3 // hashcode: 0x405DA246 (dec: 1079878214)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char p1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char u1; // size=0, offset=24
	unsigned char v1; // size=0, offset=25
	unsigned short tpage; // size=0, offset=26
	unsigned char r2; // size=0, offset=28
	unsigned char g2; // size=0, offset=29
	unsigned char b2; // size=0, offset=30
	unsigned char p2; // size=0, offset=31
	short x2; // size=0, offset=32
	short y2; // size=0, offset=34
	unsigned char u2; // size=0, offset=36
	unsigned char v2; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
};

struct POLY_GT4 // hashcode: 0x596BAA44 (dec: 1500228164)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char p1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char u1; // size=0, offset=24
	unsigned char v1; // size=0, offset=25
	unsigned short tpage; // size=0, offset=26
	unsigned char r2; // size=0, offset=28
	unsigned char g2; // size=0, offset=29
	unsigned char b2; // size=0, offset=30
	unsigned char p2; // size=0, offset=31
	short x2; // size=0, offset=32
	short y2; // size=0, offset=34
	unsigned char u2; // size=0, offset=36
	unsigned char v2; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
	unsigned char r3; // size=0, offset=40
	unsigned char g3; // size=0, offset=41
	unsigned char b3; // size=0, offset=42
	unsigned char p3; // size=0, offset=43
	short x3; // size=0, offset=44
	short y3; // size=0, offset=46
	unsigned char u3; // size=0, offset=48
	unsigned char v3; // size=0, offset=49
	unsigned short pad3; // size=0, offset=50
};

union DRAW_71fake // Hashcode: 0x10652FFA (dec: 275066874)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: draw.o, draw.o
};

union DRAW_72fake // Hashcode: 0x5A7FA683 (dec: 1518315139)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct _PFace // hashcode: 0xD6E8EAC4 (dec: -689378620)
{
	unsigned long *tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	short pad1; // size=0, offset=30
	long pad2; // size=0, offset=32
	long pad3; // size=0, offset=36
};

struct _PolytopeList // hashcode: 0xAC7B9901 (dec: -1401186047)
{
	long numPolytopes; // size=0, offset=0
	void (*polytope[1024]); // size=4096, offset=4
};

struct DRAW_109fake // hashcode: 0x13B53E5F (dec: 330645087)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: draw.o, draw.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: draw.o, draw.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: draw.o, draw.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: draw.o, draw.o
};

struct UVTYPE // hashcode: 0x7D142F71 (dec: 2098474865)
{
	unsigned char u; // size=0, offset=0
	unsigned char v; // size=0, offset=1
	short pad; // size=0, offset=2
};

struct POLY_G3_SEMITRANS // hashcode: 0x323808D7 (dec: 842533079)
{
	unsigned long tag; // size=0, offset=0
	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	unsigned char r1; // size=0, offset=16
	unsigned char g1; // size=0, offset=17
	unsigned char b1; // size=0, offset=18
	unsigned char pad1; // size=0, offset=19
	short x1; // size=0, offset=20
	short y1; // size=0, offset=22
	unsigned char r2; // size=0, offset=24
	unsigned char g2; // size=0, offset=25
	unsigned char b2; // size=0, offset=26
	unsigned char pad2; // size=0, offset=27
	short x2; // size=0, offset=28
	short y2; // size=0, offset=30
};

struct _POLY_NG4 // hashcode: 0x65B704BE (dec: 1706493118)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
};

union DRAW_212fake // Hashcode: 0xA8C66167 (dec: -1463393945)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _GT3ZVertexD3D_Type // hashcode: 0xBD55FA10 (dec: -1118438896)
{
	float x; // size=0, offset=0
	float y; // size=0, offset=4
	float z; // size=0, offset=8
	float rhw; // size=0, offset=12
	unsigned int rgba; // size=0, offset=16
	float u; // size=0, offset=20
	float v; // size=0, offset=24
};

struct SP_SUBDIV_STRUCT // hashcode: 0x4C4B55E8 (dec: 1280005608)
{
	struct _SVector vertex0; // size=8, offset=0
	struct _SVector vertex1; // size=8, offset=8
	struct _SVector vertex2; // size=8, offset=16
	SVECTORface_v0; // size=8, offset=24
	SVECTORface_v1; // size=8, offset=32
	SVECTORface_v2; // size=8, offset=40
	SVECTORface_v01; // size=8, offset=48
	SVECTORface_v12; // size=8, offset=56
	SVECTORface_v20; // size=8, offset=64
	struct UVTYPE face_uv0; // size=4, offset=72
	struct UVTYPE face_uv1; // size=4, offset=76
	struct UVTYPE face_uv2; // size=4, offset=80
	struct UVTYPE face_uv01; // size=4, offset=84
	struct UVTYPE face_uv12; // size=4, offset=88
	struct UVTYPE face_uv20; // size=4, offset=92
	struct _ColorType color0; // size=4, offset=96
	struct _ColorType color1; // size=4, offset=100
	struct _ColorType color2; // size=4, offset=104
	struct _ColorType color01; // size=4, offset=108
	struct _ColorType color12; // size=4, offset=112
	struct _ColorType color20; // size=4, offset=116
	struct _Vector out[4]; // size=48, offset=120
	long color[4]; // size=16, offset=168
	long texinfo[3]; // size=12, offset=184
	struct DVECTOR sxy[4]; // size=16, offset=196
};

typedef struct POLY_GT3 POLY_GT3;

typedef struct POLY_GT4 POLY_GT4;

typedef struct _PFace PFace;

typedef struct _PolytopeList PolytopeList;

typedef struct UVTYPE UVTYPE;

typedef struct POLY_G3_SEMITRANS POLY_G3_SEMITRANS;

typedef struct _POLY_NG4 POLY_NG4;

typedef struct _GT3ZVertexD3D_Type GT3ZVertexD3D;

typedef struct SP_SUBDIV_STRUCT SP_SUBDIV_STRUCT;

typedef void G2AppData;

union LIGHT3D_71fake // Hashcode: 0xEEF7B666 (dec: -285755802)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: light3d.o, light3d.o
};

union LIGHT3D_72fake // Hashcode: 0xA2C28EAD (dec: -1564307795)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct LIGHT3D_109fake // hashcode: 0xCB7DE4CA (dec: -880941878)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: light3d.o, light3d.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: light3d.o, light3d.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: light3d.o, light3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: light3d.o, light3d.o
};

union LIGHT3D_209fake // Hashcode: 0xF250A22A (dec: -229596630)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

typedef long LightFunc();

struct CdlFILE // hashcode: 0x2EE126E0 (dec: 786507488)
{
	struct CdlLOC pos; // size=4, offset=0
	unsigned long size; // size=0, offset=4
	char name[16]; // size=16, offset=8
};

union LOAD3D_76fake // Hashcode: 0x8D4B684D (dec: -1924437939)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: load3d.o, load3d.o
};

union LOAD3D_77fake // Hashcode: 0xD696E2BF (dec: -694754625)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct LOAD3D_114fake // hashcode: 0x1C723D51 (dec: 477248849)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: load3d.o, load3d.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: load3d.o, load3d.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: load3d.o, load3d.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: load3d.o, load3d.o
};

struct _LoadStatus // hashcode: 0x3D15BA2C (dec: 1024834092)
{
	struct _ReadQueueEntry currentQueueFile; // size=44, offset=0
	long state; // size=0, offset=44
	long bytesTransferred; // size=0, offset=48
	long lastCheckPos; // size=0, offset=52
	long cdWaitTime; // size=0, offset=56
	unsigned char *compSrcAddr; // size=0, offset=60
	unsigned char *compDstAddr; // size=0, offset=64
	long compControlbits; // size=0, offset=68
	long compControl; // size=0, offset=72
	long compEndBytes; // size=0, offset=76
	long decompressLen; // size=0, offset=80
	long checksum; // size=0, offset=84
	long *checkAddr; // size=0, offset=88
	struct _BigFile bigFile; // size=36, offset=92
	void *buffer1; // size=0, offset=128
	void *buffer2; // size=0, offset=132
	int changeDir; // size=0, offset=136
	long endLoadTime; // size=0, offset=140
	long currentDirLoading; // size=0, offset=144
	long currentSector; // size=0, offset=148
	long sectorTime; // size=0, offset=152
	long seekTime; // size=0, offset=156
};

struct RedirectList // hashcode: 0x4C185B3B (dec: 1276664635)
{
	long numPointers; // size=0, offset=0
	long *data; // size=0, offset=4
};

typedef struct CdlFILE CdlFILE;

typedef struct _LoadStatus LoadStatus;

typedef struct RedirectList RedirectList;

struct DRAWENV // hashcode: 0x1D93A150 (dec: 496214352)
{
	struct PSX_RECT clip; // size=8, offset=0
	short ofs[2]; // size=4, offset=8
	struct PSX_RECT tw; // size=8, offset=12
	unsigned short tpage; // size=0, offset=20
	unsigned char dtd; // size=0, offset=22
	unsigned char dfe; // size=0, offset=23
	unsigned char isbg; // size=0, offset=24
	unsigned char r0; // size=0, offset=25
	unsigned char g0; // size=0, offset=26
	unsigned char b0; // size=0, offset=27
	struct DR_ENV dr_env; // size=64, offset=28
};

struct DISPENV // hashcode: 0x286FCE3D (dec: 678415933)
{
	struct PSX_RECT disp; // size=8, offset=0
	struct PSX_RECT screen; // size=8, offset=8
	unsigned char isinter; // size=0, offset=16
	unsigned char isrgb24; // size=0, offset=17
	unsigned char pad0; // size=0, offset=18
	unsigned char pad1; // size=0, offset=19
};

struct BLK_FILL // hashcode: 0x2490546A (dec: 613438570)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	unsigned short x0; // size=0, offset=8
	unsigned short y0; // size=0, offset=10
	unsigned short w; // size=0, offset=12
	unsigned short h; // size=0, offset=14
};

union MAIN_76fake // Hashcode: 0x1DB2E0D5 (dec: 498262229)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: main.o, main.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: main.o, main.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: main.o, main.o
};

union MAIN_77fake // Hashcode: 0x7C857C0F (dec: 2089122831)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MAIN_114fake // hashcode: 0x6BBD5265 (dec: 1807569509)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: main.o, main.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: main.o, main.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: main.o, main.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: main.o, main.o
};

union MAIN_213fake // Hashcode: 0xFBB8D531 (dec: -71772879)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct AnalogueData // hashcode: 0xD8EB0F12 (dec: -655683822)
{
	unsigned short pad; // size=0, offset=0
	unsigned char xR; // size=0, offset=2
	unsigned char yR; // size=0, offset=3
	unsigned char xL; // size=0, offset=4
	unsigned char yL; // size=0, offset=5
};

union MAIN_243fake // Hashcode: 0xD076C382 (dec: -797523070)
{
	unsigned short pad; // size=0, offset=0
	unsigned short multitap[16]; // size=32, offset=0
	unsigned short mouse[2]; // size=4, offset=0
	unsigned short negcon[3]; // size=6, offset=0
	struct AnalogueData analogue; // size=6, offset=0
};

struct POLY_F4_SEMITRANS // hashcode: 0xE3D17AA2 (dec: -472810846)
{
	unsigned long tag; // size=0, offset=0
	unsigned long dr_tpage; // size=0, offset=4
	unsigned char r0; // size=0, offset=8
	unsigned char g0; // size=0, offset=9
	unsigned char b0; // size=0, offset=10
	unsigned char code; // size=0, offset=11
	short x0; // size=0, offset=12
	short y0; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	short x2; // size=0, offset=20
	short y2; // size=0, offset=22
	short x3; // size=0, offset=24
	short y3; // size=0, offset=26
};

struct PhysObProperties // hashcode: 0xD5487B33 (dec: -716670157)
{
	unsigned short version; // size=0, offset=0
	unsigned short family; // size=0, offset=2
	unsigned short ID; // size=0, offset=4
	unsigned short Type; // size=0, offset=6
};

struct PhysObSplinter // hashcode: 0x933D3752 (dec: -1824704686)
{
	long numSplintData; // size=0, offset=0
	void *splinterData; // size=0, offset=4
};

struct PhysObWeaponAttributes // hashcode: 0x6EB0170A (dec: 1857033994)
{
	long Damage; // size=0, offset=0
	long AltDamage; // size=0, offset=4
	short knockBackDistance; // size=0, offset=8
	char knockBackFrames; // size=0, offset=10
	char dropSound; // size=0, offset=11
	char Class; // size=0, offset=12
	char ThrowSphere; // size=0, offset=13
	char LeftHandSphere; // size=0, offset=14
	char RightHandSphere; // size=0, offset=15
	struct _PhysObLight *Light; // size=0, offset=16
	struct PhysObSplinter splinter; // size=8, offset=20
};

struct _PhysObLight // hashcode: 0x873DE727 (dec: -2025986265)
{
	unsigned char length; // size=0, offset=0
	unsigned char segment; // size=0, offset=1
	short speed; // size=0, offset=2
	struct LightTableEntry *lightTable; // size=8, offset=4
};

struct __PhysObProjectileData // hashcode: 0x12820E13 (dec: 310513171)
{
	char model; // size=0, offset=0
	char startAnim; // size=0, offset=1
	char loopAnim; // size=0, offset=2
	char endAnim; // size=0, offset=3
	long flags; // size=0, offset=4
	struct PhysObWeaponAttributes *weapon; // size=28, offset=8
};

struct SwitchData // hashcode: 0x34EE19D8 (dec: 888019416)
{
	short state; // size=0, offset=0
	short accumulator; // size=0, offset=2
};

struct LightTableEntry // hashcode: 0x952494E4 (dec: -1792764700)
{
	short r; // size=0, offset=0
	short g; // size=0, offset=2
	short b; // size=0, offset=4
	short radius; // size=0, offset=6
};

typedef struct DRAWENV DRAWENV;

typedef struct DISPENV DISPENV;

typedef struct BLK_FILL BLK_FILL;

typedef struct MainTracker MainTracker;

typedef struct AnalogueData AnalogueData;

typedef struct POLY_F4_SEMITRANS POLY_F4_SEMITRANS;

typedef struct PhysObProperties PhysObProperties;

typedef struct PhysObSplinter PhysObSplinter;

typedef struct PhysObWeaponAttributes PhysObWeaponAttributes;

typedef struct __PhysObProjectileData PhysObProjData;

typedef struct SwitchData SwitchData;

typedef struct LightTableEntry LightTableEntry;

typedef struct _PhysObLight PhysObLight;

typedef struct InterfaceItem InterfaceItem;

struct POLY_FT4 // hashcode: 0xAD2AA0E4 (dec: -1389715228)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	unsigned short pad1; // size=0, offset=30
	short x3; // size=0, offset=32
	short y3; // size=0, offset=34
	unsigned char u3; // size=0, offset=36
	unsigned char v3; // size=0, offset=37
	unsigned short pad2; // size=0, offset=38
};



union GAMELOOP_76fake // Hashcode: 0x8976D081 (dec: -1988702079)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gameloop.o, gameloop.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gameloop.o, gameloop.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gameloop.o, gameloop.o
};

union GAMELOOP_77fake // Hashcode: 0x5136BCA3 (dec: 1362541731)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union GAMELOOP_192fake // Hashcode: 0x2687240D (dec: 646390797)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

union GAMELOOP_236fake // Hashcode: 0x41C73D01 (dec: 1103576321)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};



typedef void GlyphProcessFunction();

union GAMEPAD_76fake // Hashcode: 0xCC53CC9A (dec: -866923366)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gamepad.o, gamepad.o
};

union GAMEPAD_77fake // Hashcode: 0xD392046D (dec: -745405331)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct GAMEPAD_114fake // hashcode: 0xA495E8C3 (dec: -1533679421)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: gamepad.o, gamepad.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: gamepad.o, gamepad.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: gamepad.o, gamepad.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: gamepad.o, gamepad.o
};

union GAMEPAD_200fake // Hashcode: 0xE2465038 (dec: -498708424)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct JoypadMappingStruct // hashcode: 0x312F2BE8 (dec: 825175016)
{
	unsigned short psxData; // size=0, offset=0
	unsigned short translation; // size=0, offset=2
};

typedef struct JoypadMappingStruct JoypadMapping;

struct _G2EulerAngles_Type // hashcode: 0xB3709AC5 (dec: -1284465979)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short order; // size=0, offset=6
};

enum _G2AnimCallbackMsg_Enum // Hashcode: 0xA8F4A89C (dec: -1460361060)
{
	G2ANIM_MSG_DONE = 1,
	G2ANIM_MSG_LOOPPOINT = 2,
	G2ANIM_MSG_SECTION_INTERPDONE = 3,
	G2ANIM_MSG_SEGCTRLR_INTERPDONE = 4,
	G2ANIM_MSG_SWALARMSET = 5,
	G2ANIM_MSG_PLAYEFFECT = 6,
};





struct _AnimSoundData_Type // hashcode: 0x5367FF66 (dec: 1399324518)
{
	short sfxToneID; // size=0, offset=0
	short volume; // size=0, offset=2
	short pitch; // size=0, offset=4
	short minVolDistance; // size=0, offset=6
};

struct _InstanceHandle // hashcode: 0xDC7D3685 (dec: -595773819)
{
	struct _Instance *instance; // size=616, offset=0
	long instanceID; // size=0, offset=4
};

struct evCollideInstanceStatsData // hashcode: 0xD8198216 (dec: -669416938)
{
	struct _Instance *instance; // size=616, offset=0
	unsigned long distance; // size=0, offset=4
	struct _SVector relativePosition; // size=8, offset=8
	long xyDistance; // size=0, offset=16
	long zDelta; // size=0, offset=20
};

union INSTANCE_204fake // Hashcode: 0x3FD2E4BE (dec: 1070785726)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct INICommand // hashcode: 0x04B02DE1 (dec: 78654945)
{
	short command; // size=0, offset=0
	short numParameters; // size=0, offset=2
	long parameter[0]; // size=0, offset=4
};

struct SavedIntroSmall // hashcode: 0x42C7682C (dec: 1120364588)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
};

struct SavedIntroSpline // hashcode: 0x018B74DF (dec: 25916639)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short introUniqueID; // size=0, offset=2
	short splineFlags; // size=0, offset=4
	short splineKeyFrame; // size=0, offset=6
	short splineClipBeg; // size=0, offset=8
	short splineClipEnd; // size=0, offset=10
};

struct _SavedIntroWithIntro // hashcode: 0xF234DCD6 (dec: -231416618)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	unsigned short attachedUniqueID; // size=0, offset=2
	short introOffset; // size=0, offset=4
	short birthUnitID; // size=0, offset=6
	short introUniqueID; // size=0, offset=8
	unsigned char lightGroup; // size=0, offset=10
	unsigned char specturalLightGroup; // size=0, offset=11
	long flags; // size=0, offset=12
	long flags2; // size=0, offset=16
	struct _SmallRotation smallRotation; // size=6, offset=20
	struct _Position position; // size=6, offset=26
};

typedef enum _G2AnimCallbackMsg_Enum G2AnimCallbackMsg;

typedef struct _AnimSoundData_Type AnimSoundData;

typedef struct _InstanceHandle InstanceHandle;

typedef struct evCollideInstanceStatsData evCollideInstanceStatsData;

typedef struct INICommand INICommand;

typedef struct SavedIntroSmall SavedIntroSmall;

typedef struct SavedIntroSpline SavedIntroSpline;

typedef struct _SavedIntroWithIntro SavedIntroWithIntro;

typedef struct _MonsterAnim MonsterAnim;

typedef struct _MonsterIdle MonsterIdle;

typedef struct _MonsterBehavior MonsterBehavior;

typedef struct _MonsterSenses MonsterSenses;

typedef struct _MonsterAllegiances MonsterAllegiances;

typedef struct _MonsterAttackAttributes MonsterAttackAttributes;

typedef struct _MonsterMissile MonsterMissile;

typedef struct _MonsterCombatAttributes MonsterCombatAttributes;

typedef struct _MonsterSubAttributes MonsterSubAttributes;

typedef void (*processVert_t)();

union SOUND_76fake // Hashcode: 0xA905F6AB (dec: -1459226965)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: sound.o, sound.o
};

union SOUND_77fake // Hashcode: 0xFD6718B0 (dec: -43575120)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SOUND_114fake // hashcode: 0xEE28F284 (dec: -299306364)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: sound.o, sound.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: sound.o, sound.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: sound.o, sound.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: sound.o, sound.o
};

struct ObjectPeriodicSound // hashcode: 0x931DBADB (dec: -1826768165)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
	unsigned char onTime; // size=0, offset=14
	unsigned char onTimeVariation; // size=0, offset=15
	unsigned char offTime; // size=0, offset=16
	unsigned char offTimeVariation; // size=0, offset=17
};

struct ObjectEventSound // hashcode: 0x02927CD2 (dec: 43154642)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct ObjectOneShotSound // hashcode: 0xF16FD213 (dec: -244329965)
{
	unsigned char type; // size=0, offset=0
	unsigned char numSfxIDs; // size=0, offset=1
	unsigned char soundInst; // size=0, offset=2
	unsigned char flags; // size=0, offset=3
	unsigned short minVolDistance; // size=0, offset=4
	short pitch; // size=0, offset=6
	unsigned short pitchVariation; // size=0, offset=8
	unsigned char maxVolume; // size=0, offset=10
	unsigned char maxVolVariation; // size=0, offset=11
	unsigned char initialDelay; // size=0, offset=12
	unsigned char initialDelayVariation; // size=0, offset=13
};

struct SoundEffectChannel // hashcode: 0xA19502FD (dec: -1584069891)
{
	unsigned char inUse; // size=0, offset=0
	unsigned char unused; // size=0, offset=1
	unsigned short unused1; // size=0, offset=2
	unsigned long handle; // size=0, offset=4
	short volume; // size=0, offset=8
	short volumeChangeTime; // size=0, offset=10
	short volumeChangeTimeSave; // size=0, offset=12
	short volumeChangePerUpdate; // size=0, offset=14
	short volumeChangeSign; // size=0, offset=16
	short volumeChangeErrPerUpdate; // size=0, offset=18
	short volumeChangeError; // size=0, offset=20
	short pitch; // size=0, offset=22
	short pitchChangeTime; // size=0, offset=24
	short pitchChangeTimeSave; // size=0, offset=26
	short pitchChangePerUpdate; // size=0, offset=28
	short pitchChangeSign; // size=0, offset=30
	short pitchChangeErrPerUpdate; // size=0, offset=32
	short pitchChangeError; // size=0, offset=34
};

union SOUND_205fake // Hashcode: 0x2C15C540 (dec: 739624256)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

typedef struct AadInitAttr AadInitAttr;

typedef struct MusicLoadInfo MusicLoadInfo;

typedef struct ObjectPeriodicSound ObjectPeriodicSound;

typedef struct ObjectEventSound ObjectEventSound;

typedef struct ObjectOneShotSound ObjectOneShotSound;

typedef struct SoundEffectChannel SoundEffectChannel;

typedef void asm_fn();

typedef long hook_fn_1();

typedef long hook_fn_2();

typedef long hook_fn_3();

typedef long hook_fn_4();

typedef long hook_fn_5();

typedef long hook_fn_6();

typedef long hook_fn_7();

typedef long hook_fn_8();





typedef struct evAnimationInstanceSwitchData evAnimationInstanceSwitchData;

typedef struct evControlSaveDataData evControlSaveDataData;

typedef struct GenericTune GenericTune;

struct POLY_FT3 // hashcode: 0x1FB59984 (dec: 531995012)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char u0; // size=0, offset=12
	unsigned char v0; // size=0, offset=13
	unsigned short clut; // size=0, offset=14
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char u1; // size=0, offset=20
	unsigned char v1; // size=0, offset=21
	unsigned short tpage; // size=0, offset=22
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char u2; // size=0, offset=28
	unsigned char v2; // size=0, offset=29
	unsigned short pad1; // size=0, offset=30
};

struct TransformAnimatedInstanceVerticesWork_t // hashcode: 0x7584B74F (dec: 1971631951)
{
	struct MATRIX scTransform; // size=32, offset=0
	struct MATRIX mirrorMatrix; // size=32, offset=32
	struct MATRIX llm; // size=32, offset=64
	struct _Vector vector; // size=12, offset=96
};

typedef struct POLY_FT3 POLY_FT3;

typedef struct _Mirror Mirror;

typedef struct _FXHalvePlane FXHalvePlane;

typedef struct _FXGlowEffect FXGlowEffect;

typedef struct TransformAnimatedInstanceVerticesWork_t TransformAnimatedInstanceVerticesWork_t;

union SCRIPT_76fake // Hashcode: 0x07CCD3E7 (dec: 130864103)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: script.o, script.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: script.o, script.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: script.o, script.o
};

union SCRIPT_77fake // Hashcode: 0xA89A9D98 (dec: -1466262120)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SCRIPT_114fake // hashcode: 0x918B5BD9 (dec: -1853137959)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: script.o, script.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: script.o, script.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: script.o, script.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: script.o, script.o
};

union SCRIPT_202fake // Hashcode: 0x28E126E1 (dec: 685844193)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct FXSplinter // hashcode: 0x3E763456 (dec: 1047934038)
{
	short flags; // size=0, offset=0
	short soundFx; // size=0, offset=2
	short chunkVelXY; // size=0, offset=4
	short chunkVelZ; // size=0, offset=6
	short chunkVelRng; // size=0, offset=8
	short triVelRng; // size=0, offset=10
	short lifetime; // size=0, offset=12
	short faceLimit; // size=0, offset=14
	short rotRateRng; // size=0, offset=16
	short gravityZ; // size=0, offset=18
};

typedef struct FXSplinter FXSplinter;

union TIMER_71fake // Hashcode: 0x3CBEC8D9 (dec: 1019136217)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: timer.o, timer.o
};

union TIMER_72fake // Hashcode: 0x2BECCC5B (dec: 736939099)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct TIMER_109fake // hashcode: 0x38B8D1E3 (dec: 951636451)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: timer.o, timer.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: timer.o, timer.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: timer.o, timer.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: timer.o, timer.o
};

union SPLINE_71fake // Hashcode: 0x4CDE9A19 (dec: 1289656857)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: spline.o, spline.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: spline.o, spline.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: spline.o, spline.o
};

union SPLINE_72fake // Hashcode: 0x9E0EEC67 (dec: -1643189145)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct DR_TPAGE // hashcode: 0xA93EABEF (dec: -1455510545)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[1]; // size=4, offset=4
};

union FX_71fake // Hashcode: 0xB3324C63 (dec: -1288549277)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: fx.o, fx.o
};

union FX_72fake // Hashcode: 0x20C14C3E (dec: 549538878)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct FX_109fake // hashcode: 0x76D46C98 (dec: 1993632920)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: fx.o, fx.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: fx.o, fx.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: fx.o, fx.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: fx.o, fx.o
};

struct _POLY_SG4 // hashcode: 0xA49EA007 (dec: -1533108217)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
	unsigned long drawTPage2; // size=0, offset=40
	struct _BASIC_F4 p2; // size=20, offset=44
};

struct evObjectDraftData // hashcode: 0x73C6643E (dec: 1942381630)
{
	short force; // size=0, offset=0
	unsigned short radius; // size=0, offset=2
	unsigned short radiusCoef; // size=0, offset=4
	unsigned short height; // size=0, offset=6
	unsigned short maxVelocity; // size=0, offset=8
};

struct _POLY_TF4 // hashcode: 0x871FC08E (dec: -2027962226)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage; // size=0, offset=4
	struct _BASIC_F4 p1; // size=20, offset=8
};

struct _POLY_2G4 // hashcode: 0x3FC1C531 (dec: 1069663537)
{
	unsigned long tag; // size=0, offset=0
	struct _BASIC_G4 p1; // size=32, offset=4
	struct _BASIC_G4 p2; // size=32, offset=36
};

struct _POLY_2G4T // hashcode: 0xA357E01D (dec: -1554522083)
{
	unsigned long tag; // size=0, offset=0
	unsigned long drawTPage1; // size=0, offset=4
	struct _BASIC_G4 p1; // size=32, offset=8
	struct _BASIC_G4 p2; // size=32, offset=40
};

typedef struct DR_TPAGE DR_TPAGE;

typedef struct _POLY_SG4 POLY_SG4;

typedef struct _FXGeneralEffect FXGeneralEffect;

typedef struct _FXRibbon FXRibbon;

typedef struct _FXParticle FXParticle;

typedef struct _FXLightBeam FXLightBeam;

typedef struct _FXBlastringEffect FXBlastringEffect;

typedef struct _FXForceFieldEffect FXForceFieldEffect;

typedef struct _FXLightning FXLightning;

typedef struct _FXFlash FXFlash;

typedef struct GenericFXObject GenericFXObject;

typedef struct evObjectDraftData evObjectDraftData;

typedef struct _GlyphTuneData GlyphTuneData;

typedef struct __ReaverData ReaverData;

typedef struct __ReaverTuneData ReaverTuneData;

typedef struct _POLY_TF4 POLY_TF4;

typedef struct _POLY_2G4 POLY_2G4;

typedef struct _POLY_2G4T POLY_2G4T;

union TABLES_71fake // Hashcode: 0xBB5F5001 (dec: -1151381503)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: tables.o, tables.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: tables.o, tables.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: tables.o, tables.o
};

union TABLES_72fake // Hashcode: 0x42364A63 (dec: 1110854243)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union VM_71fake // Hashcode: 0x577C0C3A (dec: 1467747386)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vm.o, vm.o
};

union VM_72fake // Hashcode: 0x27CA656B (dec: 667575659)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct VM_109fake // hashcode: 0x94EF070A (dec: -1796274422)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vm.o, vm.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vm.o, vm.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vm.o, vm.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vm.o, vm.o
};

union VM_199fake // Hashcode: 0x1A8628B8 (dec: 444999864)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct NewMemTracker // hashcode: 0x5C68280F (dec: 1550329871)
{
	struct MemHeader *rootNode; // size=8, offset=0
	unsigned long totalMemory; // size=0, offset=4
	unsigned long currentMemoryUsed; // size=0, offset=8
	char *lastMemoryAddress; // size=0, offset=12
	long doingGarbageCollection; // size=0, offset=16
};

union MEMPACK_71fake // Hashcode: 0x13120B58 (dec: 319949656)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: mempack.o, mempack.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: mempack.o, mempack.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: mempack.o, mempack.o
};

union MEMPACK_72fake // Hashcode: 0x11956E16 (dec: 295005718)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MEMPACK_188fake // Hashcode: 0x583B4A58 (dec: 1480280664)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MEMPACK_243fake // Hashcode: 0xFF7DC21B (dec: -8535525)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

typedef struct NewMemTracker NewMemTracker;

union GXASSERT_76fake // Hashcode: 0x6E0FED62 (dec: 1846537570)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: gxassert.o, gxassert.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: gxassert.o, gxassert.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: gxassert.o, gxassert.o
};

union GXASSERT_77fake // Hashcode: 0xB6DAB56F (dec: -1227180689)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct AadVolume // hashcode: 0xDE43B1D6 (dec: -565988906)
{
	short left; // size=0, offset=0
	short right; // size=0, offset=2
};

typedef struct AadVolume AadVolume;

struct AadSequenceHdr // hashcode: 0x09B09BCF (dec: 162569167)
{
	unsigned long sequenceID; // size=0, offset=0
	unsigned long quarterNoteTime; // size=0, offset=4
	unsigned short ppqn; // size=0, offset=8
	unsigned short version; // size=0, offset=10
	unsigned short numTracks; // size=0, offset=12
	unsigned short unused; // size=0, offset=14
};

struct AadTempo // hashcode: 0x0AB3CE4B (dec: 179555915)
{
	unsigned long quarterNoteTime; // size=0, offset=0
	int ppqn; // size=0, offset=4
};

typedef struct AadSequenceHdr AadSequenceHdr;

typedef struct AadTempo AadTempo;

union AADASSRT_118fake // Hashcode: 0x94794D33 (dec: -1803989709)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: aadassrt.o, aadassrt.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: aadassrt.o, aadassrt.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: aadassrt.o, aadassrt.o
};

union AADASSRT_119fake // Hashcode: 0x8D49167A (dec: -1924589958)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct AadSlotAttributes // hashcode: 0x7DE97E8C (dec: 2112454284)
{
	int sequenceNumberAssigned; // size=0, offset=0
	int sequenceAssignedDynamicBank; // size=0, offset=4
	int playingStatus; // size=0, offset=8
	int channelMute; // size=0, offset=12
	int slotVolume; // size=0, offset=16
	int slotPan; // size=0, offset=20
	int eventsInQueue[16]; // size=64, offset=24
};

union AADLIB_115fake // Hashcode: 0xBBFB2637 (dec: -1141168585)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: aadlib.o, aadlib.o
};

union AADLIB_116fake // Hashcode: 0x1F25FD4D (dec: 522583373)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct AADLIB_153fake // hashcode: 0x95DB25F7 (dec: -1780800009)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: aadlib.o, aadlib.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: aadlib.o, aadlib.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: aadlib.o, aadlib.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: aadlib.o, aadlib.o
};

union AADLIB_190fake // Hashcode: 0x9F6EB891 (dec: -1620133743)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

typedef struct AadSlotAttributes AadSlotAttributes;

struct POLY_G4 // hashcode: 0x82B947DC (dec: -2101786660)
{
	unsigned long tag; // size=0, offset=0
	unsigned char r0; // size=0, offset=4
	unsigned char g0; // size=0, offset=5
	unsigned char b0; // size=0, offset=6
	unsigned char code; // size=0, offset=7
	short x0; // size=0, offset=8
	short y0; // size=0, offset=10
	unsigned char r1; // size=0, offset=12
	unsigned char g1; // size=0, offset=13
	unsigned char b1; // size=0, offset=14
	unsigned char pad1; // size=0, offset=15
	short x1; // size=0, offset=16
	short y1; // size=0, offset=18
	unsigned char r2; // size=0, offset=20
	unsigned char g2; // size=0, offset=21
	unsigned char b2; // size=0, offset=22
	unsigned char pad2; // size=0, offset=23
	short x2; // size=0, offset=24
	short y2; // size=0, offset=26
	unsigned char r3; // size=0, offset=28
	unsigned char g3; // size=0, offset=29
	unsigned char b3; // size=0, offset=30
	unsigned char pad3; // size=0, offset=31
	short x3; // size=0, offset=32
	short y3; // size=0, offset=34
};

struct DR_AREA // hashcode: 0xE0CED39B (dec: -523316325)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[2]; // size=8, offset=4
};

union STREAM_76fake // Hashcode: 0x4C037A0C (dec: 1275296268)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: stream.o, stream.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: stream.o, stream.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: stream.o, stream.o
};

union STREAM_77fake // Hashcode: 0x616584A0 (dec: 1634043040)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct evPositionData // hashcode: 0x853C21E0 (dec: -2059656736)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short z; // size=0, offset=4
	short pad; // size=0, offset=6
};

union STREAM_210fake // Hashcode: 0x3DCA0E3B (dec: 1036652091)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct FaceExtents // hashcode: 0x22E8BE79 (dec: 585678457)
{
	long smallx; // size=0, offset=0
	long bigx; // size=0, offset=4
	long smally; // size=0, offset=8
	long bigy; // size=0, offset=12
	long smallz; // size=0, offset=16
	long bigz; // size=0, offset=20
};

struct WarpRoom // hashcode: 0x34E00E66 (dec: 887098982)
{
	char name[16]; // size=16, offset=0
	struct _StreamUnit *streamUnit; // size=64, offset=16
};



union STREAM_261fake // Hashcode: 0x825DBD24 (dec: -2107785948)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

struct VramBuffer // hashcode: 0x13A80E34 (dec: 329780788)
{
	long flags; // size=0, offset=0
	short x; // size=0, offset=4
	short y; // size=0, offset=6
	short w; // size=0, offset=8
	short h; // size=0, offset=10
	short yOffset; // size=0, offset=12
	short lengthOfLeftOverData; // size=0, offset=14
	short *lineOverFlow; // size=0, offset=16
};

struct STREAM_294fake // hashcode: 0x4C00FE3C (dec: 1275133500)
{
	int warpgate_in_use; // size=0, offset=0
	char loading; // size=0, offset=4
	char blocked; // size=0, offset=5
	short fadeValue; // size=0, offset=6
	long curTime; // size=0, offset=8
	long maxTime; // size=0, offset=12
	struct _Instance *warpFaceInstance; // size=616, offset=16
};

typedef struct POLY_G4 POLY_G4;

typedef struct DR_AREA DR_AREA;

typedef struct evPositionData evPositionData;

typedef struct STracker STracker;

typedef struct FaceExtents FaceExtents;

typedef struct WarpRoom WarpRoom;

typedef struct PlanningNode PlanningNode;

typedef struct VramBuffer VramBuffer;

union PHYSOBS_71fake // Hashcode: 0xF2195964 (dec: -233219740)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: PhysObs.o, PhysObs.o
};

union PHYSOBS_72fake // Hashcode: 0x97A84143 (dec: -1750580925)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PHYSOBS_109fake // hashcode: 0x64BC6ACF (dec: 1690069711)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: PhysObs.o, PhysObs.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: PhysObs.o, PhysObs.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: PhysObs.o, PhysObs.o
};

struct evObjectData // hashcode: 0xE261F276 (dec: -496897418)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
	short PathNumber; // size=0, offset=4
	short LinkNode; // size=0, offset=6
	struct _Instance *Force; // size=616, offset=8
	int rc; // size=0, offset=12
};

struct evPhysicsGravityData // hashcode: 0xD13BDF0D (dec: -784605427)
{
	short UpperOffset; // size=0, offset=0
	short LowerOffset; // size=0, offset=2
	short x; // size=0, offset=4
	short y; // size=0, offset=6
	short z; // size=0, offset=8
	short slipSlope; // size=0, offset=10
};

struct evPhysicsSlideData // hashcode: 0x42A65F7B (dec: 1118199675)
{
	struct _SVector ForwardVector; // size=8, offset=0
	struct _SVector ForwardNormal; // size=8, offset=8
	struct _SVector DropNormal; // size=8, offset=16
	short Segment; // size=0, offset=24
	short DropOffset; // size=0, offset=26
	short UpperOffset; // size=0, offset=28
	short Height; // size=0, offset=30
	struct _Instance *UpperInstance; // size=616, offset=32
	int Dot; // size=0, offset=36
};

struct evObjectThrowData // hashcode: 0x293A2507 (dec: 691676423)
{
	short type; // size=0, offset=0
	short spinType; // size=0, offset=2
	union {
		struct _Instance *target; // size=616, offset=0
		struct _Position position; // size=6, offset=0
		struct _Rotation direction; // size=8, offset=0
		struct _Position throwVector; // size=6, offset=0
	} data; // size=8, offset=4, found in object files: PhysObs.o, PhysObs.o
	struct _SVector angularVel; // size=8, offset=12
	unsigned short speed; // size=0, offset=20
	short gravity; // size=0, offset=22
	short initialXRot; // size=0, offset=24
	short zVel; // size=0, offset=26
};

struct evObjectBirthProjectileData // hashcode: 0xAA355F00 (dec: -1439342848)
{
	struct _Instance *instance; // size=616, offset=0
	int joint; // size=0, offset=4
	int type; // size=0, offset=8
	struct _Instance *birthInstance; // size=616, offset=12
};

struct PhysObWeaponProperties // hashcode: 0xC3612955 (dec: -1017042603)
{
	struct PhysObProperties Properties; // size=8, offset=0
	struct PhysObWeaponAttributes WeaponAttributes; // size=28, offset=8
};

struct PhysObInteractProperties // hashcode: 0x668EE420 (dec: 1720640544)
{
	struct PhysObProperties Properties; // size=8, offset=0
	unsigned short conditions; // size=0, offset=8
	unsigned short auxConditions; // size=0, offset=10
	unsigned short action; // size=0, offset=12
	unsigned short auxAction; // size=0, offset=14
	unsigned char startAnim; // size=0, offset=16
	unsigned char endAnim; // size=0, offset=17
	unsigned char razielAnim; // size=0, offset=18
	unsigned char razielAuxAnim; // size=0, offset=19
	unsigned char frame; // size=0, offset=20
	unsigned char startAnimMode; // size=0, offset=21
	unsigned short distance; // size=0, offset=22
	unsigned short newType; // size=0, offset=24
	unsigned short newClass; // size=0, offset=26
	unsigned short mode; // size=0, offset=28
	unsigned short engageXYDistance; // size=0, offset=30
	short engageZMinDelta; // size=0, offset=32
	short engageZMaxDelta; // size=0, offset=34
	unsigned char engageYCone; // size=0, offset=36
	unsigned char engageZCone; // size=0, offset=37
	unsigned short pad; // size=0, offset=38
	struct PhysObWeaponAttributes *weapon; // size=28, offset=40
};

struct __PhysObProjectileProperties // hashcode: 0x0F1971B2 (dec: 253325746)
{
	struct PhysObProperties Properties; // size=8, offset=0
	long totalProjectiles; // size=0, offset=8
	struct __PhysObProjectileData *data; // size=12, offset=12
};

struct PhysObDraftProperties // hashcode: 0x734F01FC (dec: 1934557692)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short force; // size=0, offset=8
	unsigned short radius; // size=0, offset=10
	unsigned short radiusCoef; // size=0, offset=12
	short maxVelocity; // size=0, offset=14
	unsigned long height; // size=0, offset=16
};

struct PhysObSwitchProperties // hashcode: 0xF10C9EBB (dec: -250831173)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short Distance; // size=0, offset=8
	short Class; // size=0, offset=10
	unsigned char onAnim; // size=0, offset=12
	unsigned char offAnim; // size=0, offset=13
	unsigned char failedOnAnim; // size=0, offset=14
	unsigned char enableAnim; // size=0, offset=15
	unsigned char shutAnim; // size=0, offset=16
	unsigned char onWalkTimer; // size=0, offset=17
	unsigned char offWalkTimer; // size=0, offset=18
	unsigned char walkMode; // size=0, offset=19
	unsigned char razielOnAnim; // size=0, offset=20
	unsigned char razielOffAnim; // size=0, offset=21
	unsigned char razielFailedOnAnim; // size=0, offset=22
	unsigned char razielEnableAnim; // size=0, offset=23
	unsigned char startMode; // size=0, offset=24
	unsigned char startAnim; // size=0, offset=25
	unsigned short engageXYDistance; // size=0, offset=26
	char engageZMinDelta; // size=0, offset=28
	char engageZMaxDelta; // size=0, offset=29
	unsigned char engageYCone; // size=0, offset=30
	unsigned char engageZCone; // size=0, offset=31
};

struct PhysObCollectibleProperties // hashcode: 0x80B1E807 (dec: -2135824377)
{
	struct PhysObProperties Properties; // size=8, offset=0
	short collectClass; // size=0, offset=8
	short collectValue; // size=0, offset=10
	unsigned long distance; // size=0, offset=12
	unsigned char idleAnim; // size=0, offset=16
	unsigned char collectAnim; // size=0, offset=17
	unsigned short spatialNode; // size=0, offset=18
};

struct PhysObAnimatedProperties // hashcode: 0xC40C87E4 (dec: -1005811740)
{
	struct PhysObProperties Properties; // size=8, offset=0
	unsigned short flags; // size=0, offset=8
	unsigned short pad; // size=0, offset=10
};

struct PhysObData // hashcode: 0x95351304 (dec: -1791683836)
{
	long Mode; // size=0, offset=0
	short xForce; // size=0, offset=4
	short yForce; // size=0, offset=6
	struct _Instance *Force; // size=616, offset=8
	short LinkNode; // size=0, offset=12
	short px; // size=0, offset=14
	short py; // size=0, offset=16
	short pz; // size=0, offset=18
	short rx1; // size=0, offset=20
	short ry1; // size=0, offset=22
	short rz1; // size=0, offset=24
	short Segment1; // size=0, offset=26
	short rx2; // size=0, offset=28
	short ry2; // size=0, offset=30
	short rz2; // size=0, offset=32
	short Segment2; // size=0, offset=34
	short Steps; // size=0, offset=36
	short Step; // size=0, offset=38
	short PathNumber; // size=0, offset=40
	short Pad1; // size=0, offset=42
	struct _HInfo *RightCollision; // size=24, offset=44
	struct _HInfo *LeftCollision; // size=24, offset=48
	short burnAmpl; // size=0, offset=52
	short throwFlags; // size=0, offset=54
	short initialXRot; // size=0, offset=56
	short xRotVel; // size=0, offset=58
	short yRotVel; // size=0, offset=60
	short zRotVel; // size=0, offset=62
	struct _Instance *throwingInstance; // size=616, offset=64
	int physObTimer; // size=0, offset=68
};

struct BreakOffData // hashcode: 0x47B171A7 (dec: 1202811303)
{
	short NewType; // size=0, offset=0
	short NewClass; // size=0, offset=2
};

struct ProjectileData // hashcode: 0x05689657 (dec: 90740311)
{
	int Type; // size=0, offset=0
};

struct _PhysObSaveData // hashcode: 0xA353E0E7 (dec: -1554784025)
{
	struct SwitchData switchData; // size=4, offset=0
	long Mode; // size=0, offset=4
};

typedef struct evObjectData evObjectData;

typedef struct evPhysicsGravityData evPhysicsGravityData;

typedef struct evPhysicsSlideData evPhysicsSlideData;

typedef struct evObjectThrowData evObjectThrowData;

typedef struct evObjectBirthProjectileData evObjectBirthProjectileData;

typedef struct PhysObWeaponProperties PhysObWeaponProperties;

typedef struct PhysObInteractProperties PhysObInteractProperties;

typedef struct __PhysObProjectileProperties PhysObProjectileProperties;

typedef struct PhysObDraftProperties PhysObDraftProperties;

typedef struct PhysObSwitchProperties PhysObSwitchProperties;

typedef struct PhysObCollectibleProperties PhysObCollectibleProperties;

typedef struct PhysObAnimatedProperties PhysObAnimatedProperties;

typedef struct PhysObData PhysObData;

typedef struct BreakOffData BreakOffData;

typedef struct ProjectileData ProjectileData;

typedef struct _PhysObSaveData PhysObSaveData;

union RAZCNTRL_71fake // Hashcode: 0x71649135 (dec: 1902416181)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: razcntrl.o, razcntrl.o
};

union RAZCNTRL_72fake // Hashcode: 0xF95E971E (dec: -111241442)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct RAZCNTRL_109fake // hashcode: 0x79D2E6E3 (dec: 2043864803)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: razcntrl.o, razcntrl.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: razcntrl.o, razcntrl.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: razcntrl.o, razcntrl.o
};

union STATE_71fake // Hashcode: 0xAA58AF71 (dec: -1437028495)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: state.o, state.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: state.o, state.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: state.o, state.o
};

union STATE_72fake // Hashcode: 0x5FE524E4 (dec: 1608852708)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct STATE_109fake // hashcode: 0x5CC5AB24 (dec: 1556458276)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: state.o, state.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: state.o, state.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: state.o, state.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: state.o, state.o
};

struct evMonsterHitData // hashcode: 0x5BC7AF7A (dec: 1539813242)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Instance *lastHit; // size=616, offset=4
	short knockBackDistance; // size=0, offset=8
	short knockBackDuration; // size=0, offset=10
	long power; // size=0, offset=12
};

struct evMonsterThrownData // hashcode: 0x1A3E9058 (dec: 440307800)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Rotation direction; // size=8, offset=4
	short power; // size=0, offset=12
};

struct evMonsterAlarmData // hashcode: 0x5CC4F325 (dec: 1556411173)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Position position; // size=6, offset=4
	short type; // size=0, offset=10
};

struct evMonsterSoulSuckData // hashcode: 0x24FA0E9A (dec: 620367514)
{
	struct _Instance *sender; // size=616, offset=0
	struct _Position Destination; // size=6, offset=4
};

struct evFXHitData // hashcode: 0x3D609E61 (dec: 1029742177)
{
	struct _SVector location; // size=8, offset=0
	struct _SVector velocity; // size=8, offset=8
	short amount; // size=0, offset=16
	short type; // size=0, offset=18
};

struct evAnimationControllerDoneData // hashcode: 0x06BA7BE4 (dec: 112884708)
{
	struct _Instance *instance; // size=616, offset=0
	long segment; // size=0, offset=4
	long type; // size=0, offset=8
	int data; // size=0, offset=12
};

struct evMonsterImpaleData // hashcode: 0x6537B50B (dec: 1698149643)
{
	struct _Instance *weapon; // size=616, offset=0
	struct _Rotation direction; // size=8, offset=4
	struct _Position position; // size=6, offset=12
	short distance; // size=0, offset=18
};

struct evObjectBreakOffData // hashcode: 0xE7DCD1A1 (dec: -404958815)
{
	struct _Instance *force; // size=616, offset=0
	unsigned short node; // size=0, offset=4
	short distance; // size=0, offset=6
	unsigned short animation; // size=0, offset=8
	unsigned short frame; // size=0, offset=10
	unsigned short type; // size=0, offset=12
	unsigned short action; // size=0, offset=14
};

struct evObjectAbsorbData // hashcode: 0x1379C145 (dec: 326746437)
{
	struct _Instance *force; // size=616, offset=0
	unsigned short node; // size=0, offset=4
	unsigned short steps; // size=0, offset=6
};

struct evObjectIdleData // hashcode: 0x7C5C48CC (dec: 2086422732)
{
	long mode; // size=0, offset=0
	struct _Instance *instance; // size=616, offset=4
};

struct evControlInitHangData // hashcode: 0xA248117B (dec: -1572335237)
{
	struct _Instance *instance; // size=616, offset=0
	int frame; // size=0, offset=4
	int frames; // size=0, offset=8
};

struct evControlInitIdleData // hashcode: 0xB37CA3C6 (dec: -1283677242)
{
	int mode; // size=0, offset=0
	int frame; // size=0, offset=4
	int frames; // size=0, offset=8
};

struct evActionLookAroundData // hashcode: 0x27BD6BB6 (dec: 666725302)
{
	short *rotx; // size=0, offset=0
	short *rotz; // size=0, offset=4
	short minx; // size=0, offset=8
	short maxx; // size=0, offset=10
	short minz; // size=0, offset=12
	short maxz; // size=0, offset=14
};

struct evActionPlayHostAnimationData // hashcode: 0x89FEF84B (dec: -1979778997)
{
	struct _Instance *instance; // size=616, offset=0
	struct _Instance *host; // size=616, offset=4
	int newAnim; // size=0, offset=8
	int newFrame; // size=0, offset=12
	int frames; // size=0, offset=16
	int mode; // size=0, offset=20
};

struct evShadowSegmentData // hashcode: 0xE814E960 (dec: -401282720)
{
	unsigned long totalShadowSegments; // size=0, offset=0
	unsigned short shadowSegments[4]; // size=8, offset=4
};

typedef struct __CharacterState CharacterState;

typedef struct evMonsterHitData evMonsterHitData;

typedef struct evMonsterThrownData evMonsterThrownData;

typedef struct evMonsterAlarmData evMonsterAlarmData;

typedef struct evMonsterSoulSuckData evMonsterSoulSuckData;

typedef struct evPhysicsDropOffData evPhysicsDropOffData;

typedef struct evPhysicsEdgeData evPhysicsEdgeData;

typedef struct evPhysicsWallCrawlData evPhysicsWallCrawlData;

typedef struct evPhysicsSwimData evPhysicsSwimData;

typedef struct evPhysicsDropHeightData evPhysicsDropHeightData;

typedef struct evPhysicsLinkedMoveData evPhysicsLinkedMoveData;

typedef struct evFXHitData evFXHitData;

typedef struct evAnimationControllerDoneData evAnimationControllerDoneData;

typedef struct evMonsterImpaleData evMonsterImpaleData;

typedef struct evObjectBreakOffData evObjectBreakOffData;

typedef struct evObjectAbsorbData evObjectAbsorbData;

typedef struct evObjectIdleData evObjectIdleData;

typedef struct evControlInitHangData evControlInitHangData;

typedef struct evControlInitIdleData evControlInitIdleData;

typedef struct evActionLookAroundData evActionLookAroundData;

typedef struct evActionPlayHostAnimationData evActionPlayHostAnimationData;

typedef struct evShadowSegmentData evShadowSegmentData;

union STRMLOAD_76fake // Hashcode: 0x11D0D844 (dec: 298899524)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: strmload.o, strmload.o
};

union STRMLOAD_77fake // Hashcode: 0xBBC25C92 (dec: -1144890222)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct STRMLOAD_116fake // hashcode: 0x9FFC6FB9 (dec: -1610846279)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: strmload.o, strmload.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: strmload.o, strmload.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: strmload.o, strmload.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: strmload.o, strmload.o
};

union SCRIPT2_71fake // Hashcode: 0x96CD1A76 (dec: -1764943242)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: script2.o, script2.o
};

union SCRIPT2_72fake // Hashcode: 0x9E503CEA (dec: -1638908694)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SCRIPT2_114fake // hashcode: 0x48B4C9D2 (dec: 1219807698)
{
	struct {
		struct MultiSpline *posSpline; // size=80, offset=0
		struct MultiSpline *targetSpline; // size=80, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: script2.o, script2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: script2.o, script2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: script2.o, script2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: script2.o, script2.o
};

union SCRIPT2_203fake // Hashcode: 0xA02D75C0 (dec: -1607633472)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct LitShaftProperties // hashcode: 0x6DFDA06C (dec: 1845338220)
{
	short fadeValue; // size=0, offset=0
	short pad; // size=0, offset=2
};

typedef struct LitShaftProperties LitShaftProperties;

union EVENT_76fake // Hashcode: 0x5ECA26A1 (dec: 1590306465)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: event.o, event.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: event.o, event.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: event.o, event.o
};

union EVENT_77fake // Hashcode: 0xCEC39DA9 (dec: -826040919)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SavedEventSmallVars // hashcode: 0xE3137261 (dec: -485264799)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	char eventNumber; // size=0, offset=4
	char eventVariables[5]; // size=5, offset=5
};

struct EventInstanceObject // hashcode: 0xC22B6786 (dec: -1037342842)
{
	short id; // size=0, offset=0
	short flags; // size=0, offset=2
	long unitID; // size=0, offset=4
	long introUniqueID; // size=0, offset=8
	struct _Instance *instance; // size=616, offset=12
	union {
		struct Intro *intro; // size=76, offset=0
		struct _SFXMkr *sfxMarker; // size=36, offset=0
	} data; // size=4, offset=16, found in object files: event.o, event.o
};

struct EventEventObject // hashcode: 0x59DD7B24 (dec: 1507687204)
{
	short id; // size=0, offset=0
	short eventNumber; // size=0, offset=2
	long unitID; // size=0, offset=4
	struct Event *event; // size=0, offset=8
};

struct EventTGroupObject // hashcode: 0x9ED92E4E (dec: -1629934002)
{
	short id; // size=0, offset=0
	short tgroupNumber; // size=0, offset=2
	long unitID; // size=0, offset=4
	struct BSPTree *bspTree; // size=36, offset=8
	struct _StreamUnit *stream; // size=0, offset=12
};

struct _PCodeStack // hashcode: 0x88A5C96B (dec: -2002400917)
{
	long topOfStack; // size=0, offset=0
	struct StackType stack[32]; // size=1152, offset=4
};

union EVENT_219fake // Hashcode: 0xED7CCDD7 (dec: -310587945)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct EventTimer // hashcode: 0xAA55BC81 (dec: -1437221759)
{
#ifdef PSX_VERSION
	long flags : 1; // offset=0.0
	long time : 31; // offset=0.1
#else
	long flags; // size=1, offset=0
	long time; // size=31, offset=1
#endif
	struct Event *event; // size=28, offset=4
	struct ScriptPCode *actionScript; // size=8, offset=8
	short *scriptPos; // size=0, offset=12
	struct Level *level; // size=392, offset=16
	long nextEventIndex; // size=0, offset=20
};

struct SignalResetStruct // hashcode: 0x8D08210C (dec: -1928847092)
{
	struct _MultiSignal *mSignal; // size=904, offset=0
	long timeLeft; // size=0, offset=4
};

struct HintSystemStruct // hashcode: 0xAD521037 (dec: -1387130825)
{
	short flags; // size=0, offset=0
	short hintNumber; // size=0, offset=2
	short stringNumber; // size=0, offset=4
	short pad; // size=0, offset=6
	long fadeTimer; // size=0, offset=8
	long spawningUnitID; // size=0, offset=12
};

typedef struct SavedEventSmallVars SavedEventSmallVars;

typedef struct WaterLevelProcess WaterLevelProcess;

typedef struct EventInstanceObject EventInstanceObject;

typedef struct EventEventObject EventEventObject;

typedef struct EventTGroupObject EventTGroupObject;

typedef struct _PCodeStack PCodeStack;

typedef struct EventTimer EventTimer;

typedef struct SignalResetStruct SignalResetStruct;

typedef struct HintSystemStruct HintSystemStruct;

union MONMSG_76fake // Hashcode: 0xE9C1A94D (dec: -373184179)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monmsg.o, monmsg.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monmsg.o, monmsg.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monmsg.o, monmsg.o
};

union MONMSG_77fake // Hashcode: 0x1D317893 (dec: 489781395)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONMSG_114fake // hashcode: 0x79F33A82 (dec: 2045983362)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: monmsg.o, monmsg.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: monmsg.o, monmsg.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: monmsg.o, monmsg.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: monmsg.o, monmsg.o
};

union MONMSG_196fake // Hashcode: 0x160BF936 (dec: 369883446)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _MonsterAttributes // hashcode: 0x9EE88812 (dec: -1628927982)
{
	unsigned long magicnum; // size=0, offset=0
	void *tunData; // size=0, offset=4
	char *auxAnimList; // size=0, offset=8
	char *ambientAnimList; // size=0, offset=12
	long whatAmI; // size=0, offset=16
	char numAuxAnims; // size=0, offset=20
	char numAmbientAnims; // size=0, offset=21
	unsigned char defaultAge; // size=0, offset=22
	char pupateObject; // size=0, offset=23
	unsigned char damageFXSegment; // size=0, offset=24
	unsigned char headSegment; // size=0, offset=25
	unsigned char neckSegment; // size=0, offset=26
	unsigned char leftShoulderSegment; // size=0, offset=27
	unsigned char rightShoulderSegment; // size=0, offset=28
	unsigned char waistSegment; // size=0, offset=29
	unsigned char leftKneeSegment; // size=0, offset=30
	unsigned char rightKneeSegment; // size=0, offset=31
	unsigned char leftFootSegment; // size=0, offset=32
	unsigned char rightFootSegment; // size=0, offset=33
	unsigned char spineSegment; // size=0, offset=34
	unsigned char leftWeaponSegment; // size=0, offset=35
	unsigned char rightWeaponSegment; // size=0, offset=36
	unsigned char grabSegment; // size=0, offset=37
	unsigned char bloodImpaleFrame; // size=0, offset=38
	unsigned char bloodConeFrame; // size=0, offset=39
	char numSubAttributes; // size=0, offset=40
	char numCombatAttributes; // size=0, offset=41
	char numAttackAttributes; // size=0, offset=42
	char numMissiles; // size=0, offset=43
	char numAnims; // size=0, offset=44
	char numIdles; // size=0, offset=45
	char numBehaviors; // size=0, offset=46
	char numShatters; // size=0, offset=47
	struct _MonsterSubAttributes **subAttributesList; // size=84, offset=48
	struct _MonsterCombatAttributes **combatAttributesList; // size=46, offset=52
	struct _MonsterAttackAttributes *attackAttributesList; // size=32, offset=56
	struct _MonsterMissile *missileList; // size=16, offset=60
	struct _MonsterAnim *animList; // size=16, offset=64
	struct _MonsterIdle *idleList; // size=4, offset=68
	struct _MonsterBehavior *behaviorList; // size=8, offset=72
	void *shatterList; // size=0, offset=76
};

struct burntTuneType // hashcode: 0x127E6332 (dec: 310272818)
{
	int burntDist; // size=0, offset=0
	int fadeDelta; // size=0, offset=4
};

struct _MonsterState // hashcode: 0x1B0172BB (dec: 453079739)
{
	void (*entryFunction)(struct _Instance* instance); // size=0, offset=0
	void (*stateFunction)(struct _Instance* instance); // size=0, offset=4
};

struct _MonsterStateChoice // hashcode: 0xE0AABD5C (dec: -525681316)
{
	int state; // size=0, offset=0
	struct _MonsterState functions; // size=8, offset=4
};

typedef struct _MonsterAttributes MonsterAttributes;

typedef struct _MonsterVars MonsterVars;

typedef struct _MonsterIR MonsterIR;

typedef struct burntTuneType burntTuneType;

typedef struct _MonsterState MonsterState;

typedef struct _MonsterStateChoice MonsterStateChoice;

typedef void (*MonsterFunc)();

typedef void (*MonsterDamageFunc)();

union VRAM_76fake // Hashcode: 0xFB5FC5A7 (dec: -77609561)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vram.o, vram.o
};

union VRAM_77fake // Hashcode: 0xE4B92D7A (dec: -457626246)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct VRAM_114fake // hashcode: 0xBB89CDD4 (dec: -1148596780)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vram.o, vram.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vram.o, vram.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vram.o, vram.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vram.o, vram.o
};

union VRAM_210fake // Hashcode: 0x69CCA192 (dec: 1775018386)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PHYSICS_71fake // Hashcode: 0x71AB8CEE (dec: 1907068142)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: physics.o, physics.o
};

union PHYSICS_72fake // Hashcode: 0x4F67EB4B (dec: 1332210507)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PHYSICS_109fake // hashcode: 0xC9F7DBFE (dec: -906503170)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: physics.o, physics.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: physics.o, physics.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: physics.o, physics.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: physics.o, physics.o
};

union PHYSICS_203fake // Hashcode: 0x0B2D1FCE (dec: 187506638)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union REAVER_76fake // Hashcode: 0x4667F1DF (dec: 1181217247)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: reaver.o, reaver.o
};

union REAVER_77fake // Hashcode: 0x1E75973B (dec: 511022907)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct REAVER_114fake // hashcode: 0x25B74FFD (dec: 632770557)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: reaver.o, reaver.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: reaver.o, reaver.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: reaver.o, reaver.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: reaver.o, reaver.o
};

union REAVER_213fake // Hashcode: 0x859607D9 (dec: -2053765159)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union GLYPH_76fake // Hashcode: 0xA7755406 (dec: -1485483002)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: glyph.o, glyph.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: glyph.o, glyph.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: glyph.o, glyph.o
};

union GLYPH_77fake // Hashcode: 0x56F23318 (dec: 1458713368)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct GLYPH_114fake // hashcode: 0x249332FF (dec: 613626623)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: glyph.o, glyph.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: glyph.o, glyph.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: glyph.o, glyph.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: glyph.o, glyph.o
};

union GLYPH_213fake // Hashcode: 0x655BB5FA (dec: 1700509178)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct __GlyphData // hashcode: 0x9C88C1F1 (dec: -1668759055)
{
	void (*process)(); // size=0, offset=0
	struct __MessageQueue messages; // size=136, offset=4
	short selectedGlyph; // size=0, offset=140
	short glyph_open; // size=0, offset=142
	short glyph_time; // size=0, offset=144
	short glyph_radius; // size=0, offset=146
	short glyph_scale; // size=0, offset=148
	short glyph_rotation; // size=0, offset=150
	short target_glyph_rotation; // size=0, offset=152
	short glyph_movement; // size=0, offset=154
};

typedef struct __GlyphData GlyphData;

union HUMAN_76fake // Hashcode: 0xF9B1F2DA (dec: -105778470)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: human.o, human.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: human.o, human.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: human.o, human.o
};

union HUMAN_77fake // Hashcode: 0x55AD237E (dec: 1437410174)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct HUMAN_114fake // hashcode: 0xC7C43F22 (dec: -943440094)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: human.o, human.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: human.o, human.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: human.o, human.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: human.o, human.o
};

union HUMAN_193fake // Hashcode: 0x24019F16 (dec: 604086038)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _MonsterFunctionTable // hashcode: 0x82983F8D (dec: -2103951475)
{
	void (*initFunc)(); // size=0, offset=0
	void (*cleanUpFunc)(); // size=0, offset=4
	void (*damageEffectFunc)(); // size=0, offset=8
	unsigned long (*queryFunc)(); // size=0, offset=12
	void (*messageFunc)(); // size=0, offset=16
	struct _MonsterStateChoice *stateFuncs; // size=12, offset=20
	char *versionID; // size=0, offset=24
	char *localVersionID; // size=0, offset=28
};

typedef struct _MonsterFunctionTable MonsterFunctionTable;

union MISSILE_76fake // Hashcode: 0x770AB642 (dec: 1997190722)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: missile.o, missile.o
};

union MISSILE_77fake // Hashcode: 0x00D3FAB3 (dec: 13892275)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MISSILE_114fake // hashcode: 0xA1EFD7AB (dec: -1578117205)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: missile.o, missile.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: missile.o, missile.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: missile.o, missile.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: missile.o, missile.o
};

union MISSILE_196fake // Hashcode: 0xC6BB167F (dec: -960817537)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct MissileGraphics // hashcode: 0xD28383EB (dec: -763132949)
{
	unsigned char object; // size=0, offset=0
	unsigned char model; // size=0, offset=1
};

typedef struct MissileGraphics MissileGraphics;

union MONAPI_76fake // Hashcode: 0x56316C41 (dec: 1446079553)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monapi.o, monapi.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monapi.o, monapi.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monapi.o, monapi.o
};

union MONAPI_77fake // Hashcode: 0x8FFC132D (dec: -1879305427)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONAPI_114fake // hashcode: 0x26233514 (dec: 639841556)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: monapi.o, monapi.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: monapi.o, monapi.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: monapi.o, monapi.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: monapi.o, monapi.o
};

struct evMonsterHitTerrainData // hashcode: 0x4CD2E030 (dec: 1288888368)
{
	struct _TFace *tface; // size=12, offset=0
	struct _Normal normal; // size=8, offset=4
	short faceFlags; // size=0, offset=12
	short bspFlags; // size=0, offset=14
};

struct evMonsterHitObjectData // hashcode: 0xB7192556 (dec: -1223088810)
{
	struct _Instance *instance; // size=616, offset=0
	int hitType; // size=0, offset=4
};

union MONAPI_201fake // Hashcode: 0x68ED3057 (dec: 1760374871)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _MonsterSaveInfo // hashcode: 0x3088BC75 (dec: 814267509)
{
	long mvFlags; // size=0, offset=0
	long auxFlags; // size=0, offset=4
	short soulID; // size=0, offset=8
	short soulJuice; // size=0, offset=10
	unsigned int age; // size=3, offset=96
	unsigned int state; // size=6, offset=99
	unsigned int behaviorState; // size=5, offset=105
	unsigned int anim; // size=6, offset=110
	unsigned int animLooping; // size=1, offset=116
	unsigned int causeOfDeath; // size=3, offset=117
};

typedef struct evMonsterHitTerrainData evMonsterHitTerrainData;

typedef struct evMonsterHitObjectData evMonsterHitObjectData;

typedef struct _MonsterSaveInfo MonsterSaveInfo;

union MONLIB_76fake // Hashcode: 0x270AB0E0 (dec: 655012064)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monlib.o, monlib.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monlib.o, monlib.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monlib.o, monlib.o
};

union MONLIB_77fake // Hashcode: 0x3370E575 (dec: 863036789)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONLIB_114fake // hashcode: 0x0EFC9F39 (dec: 251436857)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: monlib.o, monlib.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: monlib.o, monlib.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: monlib.o, monlib.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: monlib.o, monlib.o
};

struct evPhysicsLOSData // hashcode: 0x3AEAF56B (dec: 988476779)
{
	struct _Position destination; // size=6, offset=0
	short arc; // size=0, offset=6
	struct _Position origin; // size=6, offset=8
	short flags; // size=0, offset=14
	short result; // size=0, offset=16
};

union MONLIB_208fake // Hashcode: 0x7EC2D3DE (dec: 2126697438)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

enum MonsterAnim // Hashcode: 0x13BE1EAB (dec: 331226795)
{
	MONSTER_ANIM_HIT1 = 0,
	MONSTER_ANIM_HIT2 = 1,
	MONSTER_ANIM_WALK = 2,
	MONSTER_ANIM_RUN = 3,
	MONSTER_ANIM_GRABBED = 4,
	MONSTER_ANIM_STUNNED = 5,
	MONSTER_ANIM_JUMPFORWARD = 6,
	MONSTER_ANIM_JUMPBACK = 7,
	MONSTER_ANIM_JUMPLEFT = 8,
	MONSTER_ANIM_JUMPRIGHT = 9,
	MONSTER_ANIM_STANCE_HEALTHY = 10,
	MONSTER_ANIM_STANCE_VULNERABLE = 11,
	MONSTER_ANIM_STUNNED_RECOVERY = 12,
	MONSTER_ANIM_IMPALED = 13,
	MONSTER_ANIM_STRUGGLE = 14,
	MONSTER_ANIM_BREAKHOLD = 15,
	MONSTER_ANIM_THROWN = 16,
	MONSTER_ANIM_IMPACT = 17,
	MONSTER_ANIM_LANDONFEET = 18,
	MONSTER_ANIM_FALL = 19,
	MONSTER_ANIM_BREAKFALL = 20,
	MONSTER_ANIM_SPINLAND = 21,
	MONSTER_ANIM_IMPALEDEATH = 22,
	MONSTER_ANIM_GETUP = 23,
	MONSTER_ANIM_GENERALDEATH = 24,
	MONSTER_ANIM_MISSILEIMPALEDEATH = 25,
	MONSTER_ANIM_SURPRISED = 26,
	MONSTER_ANIM_AGONY = 27,
	MONSTER_ANIM_PUPATE = 28,
	MONSTER_ANIM_NOTICE = 29,
	MONSTER_ANIM_SOULSUCK = 30,
	MONSTER_ANIM_IMPALEHORZ = 31,
	MONSTER_ANIM_IMPALEVERT = 32,
	MONSTER_ANIM_SOULOUTGRND = 33,
	MONSTER_ANIM_SOULOUTHORZ = 34,
	MONSTER_ANIM_SOULOUTVERT = 35,
	MONSTER_ANIM_FALLOVER = 36,
	MONSTER_ANIM_EMBRACE = 37,
	MONSTER_ANIM_SHOVE = 38,
	MONSTER_ANIM_FLIPSWITCH = 39,
	MONSTER_ANIM_EVADELEFT = 40,
	MONSTER_ANIM_EVADERIGHT = 41,
	MONSTER_ANIM_ALARM = 42,
	MONSTER_ANIM_DEAF = 43,
	MONSTER_ANIM_BLIND = 44,
	MONSTER_ANIM_SUCKED = 45,
	MONSTER_NUM_ANIMS = 46,
};

enum MonsterState // Hashcode: 0xCAD17293 (dec: -892243309)
{
	MONSTER_STATE_BIRTH = 0,
	MONSTER_STATE_PURSUE = 1,
	MONSTER_STATE_IDLE = 2,
	MONSTER_STATE_MISSILEHIT = 3,
	MONSTER_STATE_FALL = 4,
	MONSTER_STATE_WANDER = 5,
	MONSTER_STATE_ATTACK = 6,
	MONSTER_STATE_IMPALEDEATH = 7,
	MONSTER_STATE_HIT = 8,
	MONSTER_STATE_STUNNED = 9,
	MONSTER_STATE_GRABBED = 10,
	MONSTER_STATE_THROWN = 11,
	MONSTER_STATE_IMPACT = 12,
	MONSTER_STATE_COMBAT = 13,
	MONSTER_STATE_BREAKHOLD = 14,
	MONSTER_STATE_LANDONFEET = 15,
	MONSTER_STATE_GENERALDEATH = 16,
	MONSTER_STATE_ENVIRONMENTDAMAGE = 17,
	MONSTER_STATE_LANDINWATER = 18,
	MONSTER_STATE_FLEE = 19,
	MONSTER_STATE_HIDE = 20,
	MONSTER_STATE_SURPRISEATTACK = 21,
	MONSTER_STATE_PARRY = 22,
	MONSTER_STATE_DEAD = 23,
	MONSTER_STATE_SURPRISED = 24,
	MONSTER_STATE_NOTICE = 25,
	MONSTER_STATE_PUPATE = 26,
	MONSTER_STATE_EMBRACE = 27,
	MONSTER_STATE_PROJECTILE = 28,
	MONSTER_STATE_TERRAINIMPALEDEATH = 29,
	MONSTER_STATE_PETRIFIED = 30,
	MONSTER_NUM_STATES = 31,
};

typedef struct evPhysicsLOSData evPhysicsLOSData;

union MONTABLE_71fake // Hashcode: 0xD5F65CA7 (dec: -705274713)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: montable.o, montable.o
};

union MONTABLE_72fake // Hashcode: 0x14670472 (dec: 342295666)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONTABLE_109fake // hashcode: 0xD09BCEF5 (dec: -795095307)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: montable.o, montable.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: montable.o, montable.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: montable.o, montable.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: montable.o, montable.o
};

union MONTABLE_188fake // Hashcode: 0x19B6D4AD (dec: 431412397)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct MONTABLE_207fake // hashcode: 0x35A35A66 (dec: 899897958)
{
	long whatAmI; // size=0, offset=0
	struct _MonsterFunctionTable *table; // size=32, offset=4
};

union MORLOCK_76fake // Hashcode: 0x5ED080FA (dec: 1590722810)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: morlock.o, morlock.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: morlock.o, morlock.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: morlock.o, morlock.o
};

union MORLOCK_77fake // Hashcode: 0xAAD9422C (dec: -1428602324)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MORLOCK_114fake // hashcode: 0x427E42C9 (dec: 1115570889)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: morlock.o, morlock.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: morlock.o, morlock.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: morlock.o, morlock.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: morlock.o, morlock.o
};

union MORLOCK_193fake // Hashcode: 0xF8639DBE (dec: -127689282)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SLUAGH_76fake // Hashcode: 0x38C12C3D (dec: 952183869)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: sluagh.o, sluagh.o
};

union SLUAGH_77fake // Hashcode: 0x781A6F6B (dec: 2014998379)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SLUAGH_114fake // hashcode: 0x17BD51C9 (dec: 398283209)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: sluagh.o, sluagh.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: sluagh.o, sluagh.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: sluagh.o, sluagh.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: sluagh.o, sluagh.o
};

union SLUAGH_193fake // Hashcode: 0x537C56CA (dec: 1400657610)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union SOUL_76fake // Hashcode: 0xCC0D2587 (dec: -871553657)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: soul.o, soul.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: soul.o, soul.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: soul.o, soul.o
};

union SOUL_77fake // Hashcode: 0x7DA1E6FA (dec: 2107762426)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct SOUL_114fake // hashcode: 0x75056227 (dec: 1963287079)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: soul.o, soul.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: soul.o, soul.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: soul.o, soul.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: soul.o, soul.o
};

union SOUL_193fake // Hashcode: 0xC2BEE064 (dec: -1027678108)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MONSENSE_76fake // Hashcode: 0x2B30D328 (dec: 724620072)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monsense.o, monsense.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monsense.o, monsense.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monsense.o, monsense.o
};

union MONSENSE_77fake // Hashcode: 0x25544503 (dec: 626279683)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONSENSE_114fake // hashcode: 0x5589BD82 (dec: 1435090306)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: monsense.o, monsense.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: monsense.o, monsense.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: monsense.o, monsense.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: monsense.o, monsense.o
};

union MONSENSE_196fake // Hashcode: 0xA7338706 (dec: -1489795322)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct RadarInfo // hashcode: 0xB8957CDE (dec: -1198162722)
{
	short angle; // size=0, offset=0
	short x; // size=0, offset=2
	short y; // size=0, offset=4
};

typedef struct RadarInfo RadarInfo;

union MONSTER_76fake // Hashcode: 0x93E73915 (dec: -1813563115)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: monster.o, monster.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: monster.o, monster.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: monster.o, monster.o
};

union MONSTER_77fake // Hashcode: 0x2E7BD072 (dec: 779866226)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MONSTER_114fake // hashcode: 0xAC393E28 (dec: -1405534680)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: monster.o, monster.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: monster.o, monster.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: monster.o, monster.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: monster.o, monster.o
};

union MONSTER_212fake // Hashcode: 0xDED63187 (dec: -556387961)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct burntMessType // hashcode: 0x8C763516 (dec: -1938410218)
{
	int closestvert; // size=0, offset=0
	int closestdist; // size=0, offset=4
	int closestseg; // size=0, offset=8
	struct burntTuneType *burntTune; // size=8, offset=12
};

struct bloodyMessType // hashcode: 0x5F4737FF (dec: 1598502911)
{
	int closestvert; // size=0, offset=0
	int closestdist; // size=0, offset=4
	int closestseg; // size=0, offset=8
	int bloodiedAVert; // size=0, offset=12
	int bloodIntensity; // size=0, offset=16
};

typedef struct burntMessType burntMessType;

typedef struct bloodyMessType bloodyMessType;

struct _G2AnimController_Type // hashcode: 0x74838F3C (dec: 1954778940)
{
	unsigned short next; // size=0, offset=0
	unsigned char type; // size=0, offset=2
	unsigned char segNumber; // size=0, offset=3
	void *callbackData; // size=0, offset=4
	unsigned short flags; // size=0, offset=8
	short duration; // size=0, offset=10
	short elapsedTime; // size=0, offset=12
	struct _G2AnimAlphaTable_Type *alphaTable; // size=4, offset=16
	union {
		struct {
			unsigned long (*function)(); // size=0, offset=0
			void *fnData; // size=0, offset=4
		} callback; // size=8, offset=0, found in object files: AnimG2.o, AnimG2.o
		struct {
			struct _G2SVector3_Type base; // size=6, offset=0
			struct _G2SVector3_Type offset; // size=6, offset=6
		} vector; // size=12, offset=0, found in object files: AnimG2.o, AnimG2.o
		struct {
			struct _G2Quat_Type src; // size=8, offset=0
			struct _G2Quat_Type dest; // size=8, offset=8
		} quat; // size=16, offset=0, found in object files: AnimG2.o, AnimG2.o
	} data; // size=16, offset=20, found in object files: AnimG2.o, AnimG2.o
};

union ANIMG2_72fake // Hashcode: 0x67F203D2 (dec: 1743913938)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct ANIMG2_109fake // hashcode: 0x1C77A67E (dec: 477603454)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: AnimG2.o, AnimG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: AnimG2.o, AnimG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: AnimG2.o, AnimG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: AnimG2.o, AnimG2.o
};

union ANIMG2_199fake // Hashcode: 0x703D7969 (dec: 1883076969)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _G2AnimSegKeyflagInfo_Type // hashcode: 0x5786AAF7 (dec: 1468443383)
{
	unsigned long *stream; // size=0, offset=0
	unsigned long flags; // size=0, offset=4
	int bitCount; // size=0, offset=8
};

struct _G2AnimDecompressChannelInfo_Type // hashcode: 0x9B2AEA69 (dec: -1691686295)
{
	struct _G2AnimKeylist_Type *keylist; // size=20, offset=0
	unsigned short *chanData; // size=0, offset=4
	int storedKey; // size=0, offset=8
	int targetKey; // size=0, offset=12
};

struct _G2AnimSegValue_Type // hashcode: 0x862DA63B (dec: -2043828677)
{
	union {
		struct _G2SVector3_Type rot; // size=6, offset=0
		struct _G2Quat_Type quat; // size=8, offset=0
	} rotQuat; // size=8, offset=0, found in object files: AnimG2.o, AnimG2.o
	struct _G2SVector3_Type scale; // size=6, offset=8
	unsigned short pad; // size=0, offset=14
	struct _G2SVector3_Type trans; // size=6, offset=16
	unsigned short bIsQuat; // size=0, offset=22
};

struct _G2AnimChanStatusBlockPool_Type // hashcode: 0x795F01EF (dec: 2036269551)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short *stack; // size=0, offset=8
	struct _G2AnimChanStatusBlock_Type *blockPool; // size=36, offset=12
};

struct _G2AnimControllerPool_Type // hashcode: 0x7F1B4AF1 (dec: 2132495089)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short *stack; // size=0, offset=8
	struct _G2AnimController_Type *blockPool; // size=36, offset=12
};

struct _G2AnimInterpStateBlockPool_Type // hashcode: 0x71F99BDD (dec: 1912183773)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short *stack; // size=0, offset=8
	struct _G2AnimInterpStateBlock_Type *blockPool; // size=164, offset=12
};

typedef struct _G2AnimController_Type G2AnimController;

typedef struct _G2AnimSegKeyflagInfo_Type G2AnimSegKeyflagInfo;

typedef struct _G2AnimDecompressChannelInfo_Type G2AnimDecompressChannelInfo;

typedef struct _G2AnimSegValue_Type G2AnimSegValue;

typedef struct _G2AnimChanStatusBlockPool_Type G2AnimChanStatusBlockPool;

typedef struct _G2AnimControllerPool_Type G2AnimControllerPool;

typedef struct _G2AnimInterpStateBlockPool_Type G2AnimInterpStateBlockPool;

typedef void G2PoolMem;

typedef void (*G2AnimApplyControllerFn)();

union VWRAITH_76fake // Hashcode: 0x3BD14D9A (dec: 1003572634)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: vwraith.o, vwraith.o
};

union VWRAITH_77fake // Hashcode: 0x1DF0AFEB (dec: 502312939)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct VWRAITH_114fake // hashcode: 0x99946622 (dec: -1718327774)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: vwraith.o, vwraith.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: vwraith.o, vwraith.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: vwraith.o, vwraith.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: vwraith.o, vwraith.o
};

union VWRAITH_193fake // Hashcode: 0x3F7542F5 (dec: 1064649461)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union WORSHIP_76fake // Hashcode: 0x67B6B44E (dec: 1740026958)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: worship.o, worship.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: worship.o, worship.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: worship.o, worship.o
};

union WORSHIP_77fake // Hashcode: 0x11B9D34C (dec: 297390924)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct WORSHIP_114fake // hashcode: 0x92B54E7B (dec: -1833611653)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: worship.o, worship.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: worship.o, worship.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: worship.o, worship.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: worship.o, worship.o
};

union WORSHIP_193fake // Hashcode: 0xB9C6ED01 (dec: -1178145535)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union MBMISS_76fake // Hashcode: 0x99AC1F62 (dec: -1716773022)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: mbmiss.o, mbmiss.o
};

union MBMISS_77fake // Hashcode: 0x2A18F047 (dec: 706277447)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MBMISS_114fake // hashcode: 0x8D708B2D (dec: -1922004179)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: mbmiss.o, mbmiss.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: mbmiss.o, mbmiss.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: mbmiss.o, mbmiss.o
};

union MBMISS_212fake // Hashcode: 0xA059CF55 (dec: -1604726955)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _walbossAttributes // hashcode: 0x300B2C35 (dec: 806038581)
{
	unsigned char segmentToAttach; // size=0, offset=0
	unsigned char frameToBirth; // size=0, offset=1
	unsigned char frameToExpel; // size=0, offset=2
	unsigned char maxEggHits; // size=0, offset=3
	short maxAngle; // size=0, offset=4
	short timeForHitAgony; // size=0, offset=6
	short segmentForBodyTwist; // size=0, offset=8
	short timeToThornPrep; // size=0, offset=10
	short timeToThornPoke; // size=0, offset=12
	short timeToEggThrob; // size=0, offset=14
	short timeToEggExplode; // size=0, offset=16
	short razielStunTime; // size=0, offset=18
	short eggIgniteDist; // size=0, offset=20
	short angleForShortAttack; // size=0, offset=22
	short closeAttackStart; // size=0, offset=24
	short junk; // size=0, offset=26
	struct {
		short plusDelta; // size=0, offset=0
		short minusDelta; // size=0, offset=2
		short validAtHitPoint; // size=0, offset=4
	} attackDeltas; // size=0, offset=28, found in object files: mbmiss.o, mbmiss.o
};

typedef struct _walbossAttributes WalbossAttributes;

union PLANAPI_113fake // Hashcode: 0xC8F8AFA3 (dec: -923226205)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planapi.o, planapi.o
};

union PLANAPI_114fake // Hashcode: 0xCACF262B (dec: -892393941)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANAPI_151fake // hashcode: 0xCFA0CAB4 (dec: -811545932)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planapi.o, planapi.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planapi.o, planapi.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planapi.o, planapi.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planapi.o, planapi.o
};

union PLANAPI_211fake // Hashcode: 0xE4701505 (dec: -462416635)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct PoolManagementData // hashcode: 0xA005D5E0 (dec: -1610230304)
{
	char state; // size=0, offset=0
	unsigned char numNodesInPool; // size=0, offset=1
	struct _Position playerPosAtLastPlanMkrUpdate; // size=6, offset=2
	struct PlanningNode *expansionNode1; // size=28, offset=8
	struct PlanningNode *expansionNode2; // size=28, offset=12
	unsigned short (*distanceMatrix)[32]; // size=4, offset=16
	unsigned short pairType; // size=0, offset=20
	struct _Position peakPos; // size=6, offset=22
	unsigned char currentNode; // size=0, offset=28
	unsigned char currentBit; // size=0, offset=29
};

struct _PlanCollideInfo // hashcode: 0x3B1266CC (dec: 991061708)
{
	long StreamUnitID; // size=0, offset=0
	struct _TFace *tFace; // size=12, offset=4
	struct _Position collidePos; // size=6, offset=8
};

typedef struct PoolManagementData PoolManagementData;

typedef struct EnemyPlanData EnemyPlanData;

typedef struct _PlanCollideInfo PlanCollideInfo;

union PLANCOLL_76fake // Hashcode: 0x735BBCE0 (dec: 1935391968)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plancoll.o, plancoll.o
};

union PLANCOLL_77fake // Hashcode: 0x61131CC4 (dec: 1628642500)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANCOLL_114fake // hashcode: 0xB6BC32DF (dec: -1229180193)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plancoll.o, plancoll.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plancoll.o, plancoll.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plancoll.o, plancoll.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plancoll.o, plancoll.o
};

union PLANCOLL_216fake // Hashcode: 0x15330FC6 (dec: 355667910)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _fth // hashcode: 0xDEECEA3E (dec: -554898882)
{
	char x; // size=0, offset=0
	char y; // size=0, offset=1
	char z; // size=0, offset=2
	char nodeType; // size=0, offset=3
};

typedef struct _fth FindTerrainHit;

union PLANDRAW_76fake // Hashcode: 0x4C57B53A (dec: 1280816442)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plandraw.o, plandraw.o
};

union PLANDRAW_77fake // Hashcode: 0x62B7D6B4 (dec: 1656215220)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANDRAW_114fake // hashcode: 0x6D041DD0 (dec: 1828986320)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plandraw.o, plandraw.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plandraw.o, plandraw.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plandraw.o, plandraw.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plandraw.o, plandraw.o
};

union PLANDRAW_211fake // Hashcode: 0xCBD20DED (dec: -875426323)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANOPT_113fake // Hashcode: 0xCBAEC67C (dec: -877738372)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planopt.o, planopt.o
};

union PLANOPT_114fake // Hashcode: 0x15E0A7CA (dec: 367044554)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANOPT_151fake // hashcode: 0xD60D918D (dec: -703753843)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planopt.o, planopt.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planopt.o, planopt.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planopt.o, planopt.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planopt.o, planopt.o
};

union PLANOPT_211fake // Hashcode: 0xC37ABAC4 (dec: -1015366972)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

typedef struct _G2AppDataVM_Type G2AppDataVM;

union QUATVM_71fake // Hashcode: 0x3F855776 (dec: 1065703286)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: QuatVM.o, QuatVM.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: QuatVM.o, QuatVM.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: QuatVM.o, QuatVM.o
};

union QUATVM_72fake // Hashcode: 0x0E200AE9 (dec: 236980969)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union ENMYPLAN_76fake // Hashcode: 0xB9153BA5 (dec: -1189790811)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: enmyplan.o, enmyplan.o
};

union ENMYPLAN_77fake // Hashcode: 0x3C88A9DD (dec: 1015589341)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct ENMYPLAN_114fake // hashcode: 0xA6FD1322 (dec: -1493363934)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: enmyplan.o, enmyplan.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: enmyplan.o, enmyplan.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: enmyplan.o, enmyplan.o
};

union ENMYPLAN_216fake // Hashcode: 0xDAEB41F2 (dec: -622116366)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct EnemyPlanSlotData // hashcode: 0xA348D4B1 (dec: -1555508047)
{
	char slotInUse; // size=0, offset=0
	char state; // size=0, offset=1
	char wayPointBeingServoedTo; // size=0, offset=2
	struct _Position startPos; // size=6, offset=4
	struct _Position goalPos; // size=6, offset=10
	struct _Position oldCurrentPos; // size=6, offset=16
	struct EnemyPlanData planData; // size=68, offset=22
	char targetType; // size=0, offset=90
	short timer; // size=0, offset=92
};

typedef struct EnemyPlanSlotData EnemyPlanSlotData;

union PLAN_76fake // Hashcode: 0x1EE5CF14 (dec: 518377236)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plan.o, plan.o
};

union PLAN_77fake // Hashcode: 0xF5480003 (dec: -179830781)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLAN_114fake // hashcode: 0xA74547B8 (dec: -1488631880)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plan.o, plan.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plan.o, plan.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plan.o, plan.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plan.o, plan.o
};

union PLAN_216fake // Hashcode: 0xA458DD1A (dec: -1537680102)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union INSTNCG2_23fake // Hashcode: 0x7E6DF599 (dec: 2121135513)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: InstncG2.o, InstncG2.o
};

union INSTNCG2_72fake // Hashcode: 0x7883E999 (dec: 2021910937)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct INSTNCG2_109fake // hashcode: 0x507B5C94 (dec: 1350261908)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: InstncG2.o, InstncG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: InstncG2.o, InstncG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: InstncG2.o, InstncG2.o
};

union MAING2_71fake // Hashcode: 0xC6D143DA (dec: -959364134)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: MainG2.o, MainG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: MainG2.o, MainG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: MainG2.o, MainG2.o
};

union MAING2_72fake // Hashcode: 0x5E3F6632 (dec: 1581213234)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union MATRIXG2_71fake // Hashcode: 0xA0B77D4D (dec: -1598587571)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: MatrixG2.o, MatrixG2.o
};

union MATRIXG2_72fake // Hashcode: 0x83CA3EFF (dec: -2083897601)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct MATRIXG2_109fake // hashcode: 0x245522F0 (dec: 609559280)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: MatrixG2.o, MatrixG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: MatrixG2.o, MatrixG2.o
};

union MATRIXG2_199fake // Hashcode: 0x4ED53C11 (dec: 1322597393)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct _G2PoolMemPool_Type // hashcode: 0x987744BB (dec: -1737014085)
{
	unsigned short blockSize; // size=0, offset=0
	unsigned short stackTop; // size=0, offset=2
	unsigned short stackSize; // size=0, offset=4
	unsigned short *stack; // size=0, offset=8
	struct _G2PoolMem_Type *blockPool; // size=0, offset=12
};

typedef struct _G2PoolMemPool_Type G2PoolMemPool;

union QUATG2_71fake // Hashcode: 0x6D09A304 (dec: 1829348100)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: QuatG2.o, QuatG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: QuatG2.o, QuatG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: QuatG2.o, QuatG2.o
};

union QUATG2_72fake // Hashcode: 0xEADF8295 (dec: -354450795)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union TIMERG2_71fake // Hashcode: 0x8A15530F (dec: -1978313969)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: TimerG2.o, TimerG2.o
};

union TIMERG2_72fake // Hashcode: 0xD9819D78 (dec: -645816968)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct TIMERG2_109fake // hashcode: 0xDA8B298B (dec: -628414069)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: TimerG2.o, TimerG2.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: TimerG2.o, TimerG2.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: TimerG2.o, TimerG2.o
};

union PLANPOOL_113fake // Hashcode: 0x56DC200F (dec: 1457266703)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: planpool.o, planpool.o
};

union PLANPOOL_114fake // Hashcode: 0xC5FBA233 (dec: -973364685)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANPOOL_151fake // hashcode: 0x1F99F3B2 (dec: 530183090)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: planpool.o, planpool.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: planpool.o, planpool.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: planpool.o, planpool.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: planpool.o, planpool.o
};

union PLANPOOL_211fake // Hashcode: 0x03C6821C (dec: 63341084)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union PLANSRCH_113fake // Hashcode: 0x580A71C1 (dec: 1477079489)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: plansrch.o, plansrch.o
};

union PLANSRCH_114fake // Hashcode: 0x3FB13975 (dec: 1068579189)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct PLANSRCH_151fake // hashcode: 0x490AB669 (dec: 1225438825)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: plansrch.o, plansrch.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: plansrch.o, plansrch.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: plansrch.o, plansrch.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: plansrch.o, plansrch.o
};

union PLANSRCH_211fake // Hashcode: 0xDCCB3C3B (dec: -590660549)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union CONTROL_71fake // Hashcode: 0x043A48B4 (dec: 70928564)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: control.o, control.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: control.o, control.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: control.o, control.o
};

union CONTROL_72fake // Hashcode: 0xC9FAD521 (dec: -906308319)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct CONTROL_109fake // hashcode: 0x745D7876 (dec: 1952282742)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: control.o, control.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: control.o, control.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: control.o, control.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: control.o, control.o
};

union CONTROL_203fake // Hashcode: 0xA503E0F9 (dec: -1526472455)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct __Force // hashcode: 0x2CC8F311 (dec: 751366929)
{
	int Type; // size=0, offset=0
	struct _Vector LinearForce; // size=12, offset=4
	short Friction; // size=0, offset=16
};

struct __Idle // hashcode: 0xA4E3256E (dec: -1528617618)
{
	unsigned char anim; // size=0, offset=0
	unsigned char frame; // size=0, offset=1
	unsigned char frames; // size=0, offset=2
	unsigned char type; // size=0, offset=3
};

struct __VAnim // hashcode: 0xDC662191 (dec: -597286511)
{
	unsigned char anim0; // size=0, offset=0
	unsigned char anim1; // size=0, offset=1
	unsigned char anim2; // size=0, offset=2
	unsigned char frames; // size=0, offset=3
	unsigned char mode; // size=0, offset=4
	unsigned char alpha; // size=0, offset=5
	unsigned short frame; // size=0, offset=6
};

struct __SAnim // hashcode: 0xD19E5798 (dec: -778152040)
{
	struct __VAnim *anim; // size=8, offset=0
	struct __SAnim *nextAnim; // size=16, offset=4
	short mode; // size=0, offset=8
	short data; // size=0, offset=10
	short speedAdjust; // size=0, offset=12
	short pad; // size=0, offset=14
};

struct __AttackItem // hashcode: 0x6E34BA91 (dec: 1848949393)
{
	unsigned char anim; // size=0, offset=0
	unsigned char framesIn; // size=0, offset=1
	unsigned char alphaIn; // size=0, offset=2
	unsigned char framesOut; // size=0, offset=3
	unsigned char switchDelay; // size=0, offset=4
	unsigned char ignoreDelay; // size=0, offset=5
	unsigned char segmentToGlow; // size=0, offset=6
	unsigned char startCollisionFrame; // size=0, offset=7
	unsigned char handsToCollide; // size=0, offset=8
	unsigned char endCollisionFrame; // size=0, offset=9
	unsigned char ribbonStartFrame; // size=0, offset=10
	unsigned char ribbonStartSegment; // size=0, offset=11
	unsigned char ribbonEndSegment; // size=0, offset=12
	unsigned char ribbonLifeTime; // size=0, offset=13
	unsigned char ribbonFaceLifeTime; // size=0, offset=14
	unsigned char knockBackFrames; // size=0, offset=15
	unsigned char glowFadeInFrames; // size=0, offset=16
	unsigned char glowFadeOutFrames; // size=0, offset=17
	unsigned short ribbonStartOpacity; // size=0, offset=18
	unsigned long ribbonStartColor; // size=0, offset=20
	unsigned long ribbonEndColor; // size=0, offset=24
	unsigned long glowColor; // size=0, offset=28
	unsigned short knockBackDistance; // size=0, offset=32
	unsigned short hitPowerScale; // size=0, offset=34
};

struct __ThrowItem // hashcode: 0xABF5F7A3 (dec: -1409943645)
{
	unsigned char prepAnim; // size=0, offset=0
	unsigned char prepAnimFramesIn; // size=0, offset=1
	unsigned char prepAnimAlphaIn; // size=0, offset=2
	unsigned char coilAnim; // size=0, offset=3
	unsigned char coilAnimFramesIn; // size=0, offset=4
	unsigned char coilAnimAlphaIn; // size=0, offset=5
	unsigned char throwAnim; // size=0, offset=6
	unsigned char throwAnimFramesInFactor; // size=0, offset=7
	unsigned char throwFrameToLaunch; // size=0, offset=8
	unsigned char framesOut; // size=0, offset=9
	short launchPointX; // size=0, offset=10
	short launchPointY; // size=0, offset=12
	short launchPointZ; // size=0, offset=14
	short maxXRotation; // size=0, offset=16
	short minXRotation; // size=0, offset=18
	short maxZRotation; // size=0, offset=20
	short minZRotation; // size=0, offset=22
	short velocity; // size=0, offset=24
	short gravity; // size=0, offset=26
	short coilRot; // size=0, offset=28
	short selfAdjustingFlag; // size=0, offset=30
};

struct __EngagedInstance // hashcode: 0x504DFFAD (dec: 1347289005)
{
	struct _Instance *instance; // size=616, offset=0
	unsigned long distance; // size=0, offset=4
};

struct __FitPoint // hashcode: 0xFA3F31C2 (dec: -96521790)
{
	short x; // size=0, offset=0
	short y; // size=0, offset=2
};

typedef struct __Force Force;

typedef struct __Idle Idle;

typedef struct __VAnim VAnim;

typedef struct __SAnim SAnim;

typedef struct __AttackItem AttackItem;

typedef struct __ThrowItem ThrowItem;

typedef struct __EngagedInstance EngagedInstance;

typedef struct __FitPoint FitPoint;

typedef struct __SensoryData SensoryData;

typedef struct __Player Player;

union RAZIEL_71fake // Hashcode: 0xF5AF9DDD (dec: -173040163)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: raziel.o, raziel.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: raziel.o, raziel.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: raziel.o, raziel.o
};

union RAZIEL_72fake // Hashcode: 0x9204611F (dec: -1845206753)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union RAZIEL_203fake // Hashcode: 0x2DB038C4 (dec: 766523588)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union RAZIEL_257fake // Hashcode: 0x017E7D66 (dec: 25066854)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

struct __CannedSound // hashcode: 0x8A7588C2 (dec: -1972008766)
{
	short bank; // size=0, offset=0
	short sound; // size=0, offset=2
	short startPitch; // size=0, offset=4
	short endPitch; // size=0, offset=6
	short startVolume; // size=0, offset=8
	short endVolume; // size=0, offset=10
	long time; // size=0, offset=12
	long distance; // size=0, offset=16
};

struct __AlarmData // hashcode: 0x49AF2B39 (dec: 1236216633)
{
	short command; // size=0, offset=0
	short data; // size=0, offset=2
};

struct _SoundRamp // hashcode: 0xB5503009 (dec: -1253036023)
{
	unsigned long soundHandle; // size=0, offset=0
	long soundTimer; // size=0, offset=4
	long soundTotalTime; // size=0, offset=8
	unsigned long soundDistance; // size=0, offset=12
	short soundStartPitch; // size=0, offset=16
	short soundEndPitch; // size=0, offset=18
	short soundStartVolume; // size=0, offset=20
	short soundEndVolume; // size=0, offset=22
};

struct PlayerSaveData // hashcode: 0x12C2C060 (dec: 314753120)
{
	unsigned long abilities; // size=0, offset=0
	unsigned long currentPlane; // size=0, offset=4
	unsigned long healthScale; // size=0, offset=8
	unsigned long healthBalls; // size=0, offset=12
	unsigned short manaBalls; // size=0, offset=16
	unsigned short manaMax; // size=0, offset=18
	unsigned long playerEventHistory; // size=0, offset=20
};

struct RazielData // hashcode: 0x40AA7C96 (dec: 1084914838)
{
	unsigned long version; // size=0, offset=0
	unsigned long nonBurningRibbonStartColor; // size=0, offset=4
	unsigned long nonBurningRibbonEndColor; // size=0, offset=8
	struct __Idle ***idleList; // size=4, offset=12
	struct __AttackItem ***attackList; // size=36, offset=16
	struct __ThrowItem **throwList; // size=32, offset=20
	struct __VAnim *virtualAnimations; // size=8, offset=24
	struct __VAnim *virtualAnimSingle; // size=8, offset=28
	struct __SAnim **stringAnimations; // size=16, offset=32
	short throwFadeValue; // size=0, offset=36
	short throwFadeInRate; // size=0, offset=38
	int throwFadeOutRate; // size=0, offset=40
	int throwManualDistance; // size=0, offset=44
	short healthMaterialRate; // size=0, offset=48
	short healthSpectralRate; // size=0, offset=50
	short healthInvinciblePostHit; // size=0, offset=52
	short healthInvinciblePostShunt; // size=0, offset=54
	short forceMinPitch; // size=0, offset=56
	short forceMaxPitch; // size=0, offset=58
	short forceMinVolume; // size=0, offset=60
	short forceMaxVolume; // size=0, offset=62
	unsigned long forceRampTime; // size=0, offset=64
	int SwimPhysicsFallDamping; // size=0, offset=68
	int SwimPhysicsWaterFrequency; // size=0, offset=72
	int SwimPhysicsWaterAmplitude; // size=0, offset=76
	int SwimPhysicsUnderDeceleration; // size=0, offset=80
	int SwimPhysicsUnderKickVelocity; // size=0, offset=84
	int SwimPhysicsUnderKickAccel; // size=0, offset=88
	int SwimPhysicsUnderVelocity; // size=0, offset=92
	int SwimPhysicsUnderKickDecel; // size=0, offset=96
	int SwimPhysicsUnderStealthAdjust; // size=0, offset=100
	int SwimPhysicsCoilVelocity; // size=0, offset=104
	int SwimPhysicsCoilDecelerationIn; // size=0, offset=108
	int SwimPhysicsCoilDecelerationOut; // size=0, offset=112
	int SwimPhysicsShotVelocity; // size=0, offset=116
	int SwimPhysicsShotAccelerationIn; // size=0, offset=120
	int SwimPhysicsShotAccelerationOut; // size=0, offset=124
	int SwimPhysicsSurfVelocity; // size=0, offset=128
	int SwimPhysicsSurfAccelerationIn; // size=0, offset=132
	int SwimPhysicsSurfAccelerationOut; // size=0, offset=136
	int SwimPhysicsSurfKickVelocity; // size=0, offset=140
	int SwimPhysicsSurfKickAccel; // size=0, offset=144
	int SwimPhysicsSurfMinRotation; // size=0, offset=148
	int SwimPhysicsSurfMaxRotation; // size=0, offset=152
	int SwimPhysicsSurfKickDecel; // size=0, offset=156
};

struct __razController // hashcode: 0x694854B2 (dec: 1766347954)
{
	unsigned short segment; // size=0, offset=0
	unsigned short type; // size=0, offset=2
};

typedef struct __CannedSound CannedSound;

typedef struct __AlarmData AlarmData;

typedef struct _SoundRamp SoundRamp;

typedef struct PlayerSaveData PlayerSaveData;

typedef struct RazielData RazielData;

typedef struct __razController razController;

union SAVEINFO_76fake // Hashcode: 0xD1DD7996 (dec: -774014570)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: saveinfo.o, saveinfo.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: saveinfo.o, saveinfo.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: saveinfo.o, saveinfo.o
};

union SAVEINFO_77fake // Hashcode: 0xFB46A940 (dec: -79255232)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

union SAVEINFO_196fake // Hashcode: 0x37750CF1 (dec: 930417905)
{
	struct _Instance* target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

struct SavedLevel // hashcode: 0xABBBA745 (dec: -1413765307)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
};

struct ActualSavedLevel // hashcode: 0xE75849FD (dec: -413644291)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	short areaID; // size=0, offset=2
	short waterZ; // size=0, offset=4
	short numberBSPTreesSaved; // size=0, offset=6
	struct SavedBspTree bspTreeArray[1]; // size=8, offset=8
};

struct SavedDeadDeadBits // hashcode: 0x00B3140D (dec: 11736077)
{
	unsigned char savedID; // size=0, offset=0
	unsigned char shiftedSaveSize; // size=0, offset=1
	char deadDeadBits[832]; // size=832, offset=2
};

struct _GlobalSaveTracker // hashcode: 0x1972AA1A (dec: 426945050)
{
	short savedID; // size=0, offset=0
	short saveSize; // size=0, offset=2
	short flags; // size=0, offset=4
	short saveVersion; // size=0, offset=6
	unsigned long currentTime; // size=0, offset=8
	short humanOpinionOfRaziel; // size=0, offset=12
	char numRegens; // size=0, offset=14
	struct _MONAPI_Regenerator regenEntries[10]; // size=80, offset=16
	struct gSoundData sound; // size=20, offset=96
	short sizeUsedInBlock; // size=0, offset=116
	short CurrentBirthID; // size=0, offset=118
};

union SAVEINFO_223fake // Hashcode: 0x8FDE617F (dec: -1881251457)
{
	struct Intro *intro; // size=76, offset=0
	struct _SFXMkr *sfxMarker; // size=36, offset=0
};

typedef struct SavedLevel SavedLevel;

typedef struct ActualSavedLevel ActualSavedLevel;

typedef struct SavedDeadDeadBits SavedDeadDeadBits;

typedef struct _GlobalSaveTracker GlobalSaveTracker;

struct CdlFILTER // hashcode: 0x62FCDD39 (dec: 1660738873)
{
	unsigned char file; // size=0, offset=0
	unsigned char chan; // size=0, offset=1
	unsigned short pad; // size=0, offset=2
};

struct SpuCommonAttr // hashcode: 0xCB591BFF (dec: -883352577)
{
	unsigned long mask; // size=0, offset=0
	struct SpuVolume mvol; // size=4, offset=4
	struct SpuVolume mvolmode; // size=4, offset=8
	struct SpuVolume mvolx; // size=4, offset=12
	struct SpuExtAttr cd; // size=12, offset=16
	struct SpuExtAttr ext; // size=12, offset=28
};

union VOICEXA_86fake // Hashcode: 0x5344B4D5 (dec: 1397011669)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: voicexa.o, voicexa.o
};

union VOICEXA_87fake // Hashcode: 0xEAE9F72B (dec: -353765589)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct VOICEXA_175fake // hashcode: 0xD4103ED4 (dec: -737132844)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: voicexa.o, voicexa.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: voicexa.o, voicexa.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: voicexa.o, voicexa.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: voicexa.o, voicexa.o
};

typedef struct CdlFILTER CdlFILTER;

typedef struct SpuCommonAttr SpuCommonAttr;

typedef struct XAVoiceListEntry XAVoiceListEntry;

typedef struct XAVoiceTracker XAVoiceTracker;

typedef void (*VoiceCmdProc)();

union CINEPSX_71fake // Hashcode: 0xC9DD8A44 (dec: -908228028)
{
	struct {
		unsigned long (*function)(); // size=0, offset=0
		void *fnData; // size=0, offset=4
	} callback; // size=8, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _G2SVector3_Type base; // size=6, offset=0
		struct _G2SVector3_Type offset; // size=6, offset=6
	} vector; // size=12, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _G2Quat_Type src; // size=8, offset=0
		struct _G2Quat_Type dest; // size=8, offset=8
	} quat; // size=16, offset=0, found in object files: cinepsx.o, cinepsx.o
};

union CINEPSX_72fake // Hashcode: 0x52A4E0FA (dec: 1386537210)
{
	long color; // size=0, offset=0
	struct _RGBAColor rgba; // size=4, offset=0
};

struct CINEPSX_109fake // hashcode: 0xCA47EDB9 (dec: -901255751)
{
	struct {
		struct MultiSpline *posSpline; // size=0, offset=0
		struct MultiSpline *targetSpline; // size=0, offset=4
		struct _SVector lastSplinePos; // size=8, offset=8
		short splinePointAhead; // size=0, offset=16
		short cinema_done; // size=0, offset=18
		int splinecam_currkey; // size=0, offset=20
		int splinecam_helpkey; // size=0, offset=24
	} Cinematic; // size=28, offset=0, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _Position offset; // size=6, offset=0
		struct _Rotation rotation; // size=8, offset=6
		long focusDistance; // size=0, offset=16
	} Idle; // size=20, offset=28, found in object files: cinepsx.o, cinepsx.o
	struct {
		struct _TFace *tface; // size=12, offset=0
		struct _TFace *oldTFace; // size=12, offset=4
		struct _SVector oldTargetRot; // size=8, offset=8
		long timer; // size=0, offset=16
		struct _SVector oldFocusInstancePos; // size=8, offset=20
	} Behind; // size=28, offset=48, found in object files: cinepsx.o, cinepsx.o
	struct {
		long stopTimer; // size=0, offset=0
		long stopHit; // size=0, offset=4
		struct _TFace *tface; // size=12, offset=8
		struct _TFace *oldTFace; // size=12, offset=12
		long hit; // size=0, offset=16
	} Follow; // size=20, offset=76, found in object files: cinepsx.o, cinepsx.o
};

union CINEPSX_188fake // Hashcode: 0x6F4C9E8E (dec: 1867292302)
{
	struct _Instance *target; // size=616, offset=0
	struct _Position position; // size=6, offset=0
	struct _Rotation direction; // size=8, offset=0
	struct _Position throwVector; // size=6, offset=0
};

union CINEPSX_211fake // Hashcode: 0x8DD526F5 (dec: -1915410699)
{
	unsigned short pad; // size=0, offset=0
	unsigned short multitap[16]; // size=32, offset=0
	unsigned short mouse[2]; // size=4, offset=0
	unsigned short negcon[3]; // size=6, offset=0
	struct AnalogueData analogue; // size=6, offset=0
};

typedef struct cinema_fn_table_t cinema_fn_table_t;

typedef int PlayCinematic_t();

enum language_t // Hashcode: 0x2F13EDA4 (dec: 789835172)
{
	language_default = -1,
	language_english = 0,
	language_french = 1,
	language_german = 2,
	language_italian = 3,
	language_spanish = 4,
	language_num_states = 5,
};

enum localstr_t // Hashcode: 0xC51A2F36 (dec: -988139722)
{
	LOCALSTR_BOGUS = 0,
	LOCALSTR_format_card = 1,
	LOCALSTR_accessing_card = 2,
	LOCALSTR_back = 3,
	LOCALSTR_cancel = 4,
	LOCALSTR_choose_load_file = 5,
	LOCALSTR_choose_save_file = 6,
	LOCALSTR_confirm = 7,
	LOCALSTR_continue = 8,
	LOCALSTR_controller = 9,
	LOCALSTR_create_file = 10,
	LOCALSTR_creating = 11,
	LOCALSTR_demos = 12,
	LOCALSTR_do_not_remove_card = 13,
	LOCALSTR_done = 14,
	LOCALSTR_exit_game = 15,
	LOCALSTR_formatting = 16,
	LOCALSTR_go_back = 17,
	LOCALSTR_insert_card = 18,
	LOCALSTR_insert_card_free = 19,
	LOCALSTR_invalid_card = 20,
	LOCALSTR_load_failed = 21,
	LOCALSTR_load_game = 22,
	LOCALSTR_loading = 23,
	LOCALSTR_music = 24,
	LOCALSTR_new_game = 25,
	LOCALSTR_no = 26,
	LOCALSTR_no_card = 27,
	LOCALSTR_card_full = 28,
	LOCALSTR_card_not_enough_blocks = 29,
	LOCALSTR_ok = 30,
	LOCALSTR_options = 31,
	LOCALSTR_overwrite_game = 32,
	LOCALSTR_paused = 33,
	LOCALSTR_press_start = 34,
	LOCALSTR_query_format_card = 35,
	LOCALSTR_query_overwrite = 36,
	LOCALSTR_query_quit = 37,
	LOCALSTR_quit_game = 38,
	LOCALSTR_resume_game = 39,
	LOCALSTR_save_failed = 40,
	LOCALSTR_save_game = 41,
	LOCALSTR_save_ok = 42,
	LOCALSTR_saving = 43,
	LOCALSTR_sound = 44,
	LOCALSTR_start_game = 45,
	LOCALSTR_start_new_game = 46,
	LOCALSTR_try_card_again = 47,
	LOCALSTR_unformatted_card = 48,
	LOCALSTR_vibration_off = 49,
	LOCALSTR_vibration_on = 50,
	LOCALSTR_voice = 51,
	LOCALSTR_warning_one = 52,
	LOCALSTR_warning_two = 53,
	LOCALSTR_yes = 54,
	LOCALSTR_Hint0 = 55,
	LOCALSTR_Hint1 = 56,
	LOCALSTR_Hint2 = 57,
	LOCALSTR_Hint3 = 58,
	LOCALSTR_Hint4 = 59,
	LOCALSTR_Hint5 = 60,
	LOCALSTR_Hint6 = 61,
	LOCALSTR_Hint7 = 62,
	LOCALSTR_Hint8 = 63,
	LOCALSTR_Hint9 = 64,
	LOCALSTR_Hint10 = 65,
	LOCALSTR_Hint11 = 66,
	LOCALSTR_Hint12 = 67,
	LOCALSTR_Hint13 = 68,
	LOCALSTR_Hint14 = 69,
	LOCALSTR_Hint15 = 70,
	LOCALSTR_Hint16 = 71,
	LOCALSTR_Hint17 = 72,
	LOCALSTR_Hint18 = 73,
	LOCALSTR_Hint19 = 74,
	LOCALSTR_Hint20 = 75,
	LOCALSTR_Hint21 = 76,
	LOCALSTR_Hint22 = 77,
	LOCALSTR_Hint23 = 78,
	LOCALSTR_Hint24 = 79,
	LOCALSTR_Hint25 = 80,
	LOCALSTR_Hint26 = 81,
	LOCALSTR_Hint27 = 82,
	LOCALSTR_Hint28 = 83,
	LOCALSTR_Hint29 = 84,
	LOCALSTR_Hint30 = 85,
	LOCALSTR_Hint31 = 86,
	LOCALSTR_Hint32 = 87,
	LOCALSTR_Hint33 = 88,
	LOCALSTR_Hint34 = 89,
	LOCALSTR_Hint35 = 90,
	LOCALSTR_Hint36 = 91,
	LOCALSTR_Hint37 = 92,
	LOCALSTR_Hint38 = 93,
	LOCALSTR_Hint39 = 94,
	LOCALSTR_Hint40 = 95,
	LOCALSTR_Hint41 = 96,
	LOCALSTR_Hint42 = 97,
	LOCALSTR_Hint43 = 98,
	LOCALSTR_Hint44 = 99,
	LOCALSTR_Hint45 = 100,
	LOCALSTR_Hint46 = 101,
	LOCALSTR_Hint47 = 102,
	LOCALSTR_Hint48 = 103,
	LOCALSTR_Hint49 = 104,
	LOCALSTR_Hint50 = 105,
	LOCALSTR_Hint51 = 106,
	LOCALSTR_Hint52 = 107,
	LOCALSTR_EndOfHintStrings = 108,
	LOCALSTR_no_controller = 109,
	LOCALSTR_NUM_ITEMS = 110,
};

struct LocalizationHeader // hashcode: 0x40BDAC32 (dec: 1086172210)
{
	enum language_t language; // size=4, offset=0
	int numXAfiles; // size=0, offset=4
	int XATableOffset; // size=0, offset=8
	int numStrings; // size=0, offset=12
};

typedef enum language_t language_t;

typedef enum localstr_t localstr_t;

typedef struct LocalizationHeader LocalizationHeader;



struct Extents2d // hashcode: 0x49E3CF7D (dec: 1239666557)
{
	int xmin; // size=0, offset=0
	int xmax; // size=0, offset=4
	int ymin; // size=0, offset=8
	int ymax; // size=0, offset=12
};

typedef enum menu_ctrl_t menu_ctrl_t;

typedef enum menu_sound_t menu_sound_t;

typedef struct menu_t menu_t;

typedef struct menu_format_t menu_format_t;

typedef struct menu_stack_t menu_stack_t;

typedef struct menu_item_t menu_item_t;

typedef struct Extents2d Extents2d;

typedef int menu_item_fn_t();

typedef void menu_draw_fn_t();



enum sfx_t // Hashcode: 0xC1FB9866 (dec: -1040476058)
{
	sfx_sound = 0,
	sfx_music = 1,
	sfx_voice = 2,
	sfx_error = 3,
};

typedef enum sfx_t sfx_t;

typedef struct mcpsx_t mcpsx_t;

typedef enum mcpsx_err_t mcpsx_err_t;

typedef enum mcpsx_func_t mcpsx_func_t;

typedef enum mcpsx_sync_t mcpsx_sync_t;

typedef enum mcpsx_mode_t mcpsx_mode_t;

typedef struct mcard_t mcard_t;

typedef enum state_t fsmcard_t;

typedef enum mcard_status_t mcard_status_t;

typedef enum fsm_t fsm_t;

typedef struct mcmenu_t mcmenu_t;

typedef struct mcmenu_table_t mcmenu_table_t;

typedef struct memcard_t memcard_t;

typedef int mcard_filename_fn_t();

struct DR_STP // hashcode: 0x8C972396 (dec: -1936252010)
{
	unsigned long tag; // size=0, offset=0
	unsigned long code[2]; // size=8, offset=4
};



typedef struct DR_STP DR_STP;

typedef struct UW_ScreenXY UW_ScreenXY;


#endif
