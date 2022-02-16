#ifndef EVENT_H
#define EVENT_H

struct __Event // hashcode: 0xBC16D673 (dec: -1139353997)
{
	int ID; // size=0, offset=0
	int Data; // size=0, offset=4
};

struct EventBasicObject // hashcode: 0x9FC1DB2A (dec: -1614685398)
{
	short id; // size=0, offset=0
};

struct ScriptPCode // hashcode: 0x5E69DEB9 (dec: 1583996601)
{
	unsigned short sizeOfPcodeStream; // size=0, offset=0
	unsigned short conditionBits; // size=0, offset=2
	short* data; // size=0, offset=4
};

struct Event // hashcode: 0x68F4AD26 (dec: 1760865574)
{
	short eventNumber; // size=0, offset=0
	short numInstances; // size=0, offset=2
	unsigned char numActions; // size=0, offset=4
	unsigned char processingPuppetShow; // size=0, offset=5
	short eventVariables[5]; // size=10, offset=6
	struct EventBasicObject** instanceList; // size=2, offset=16
	struct ScriptPCode** conditionalList; // size=8, offset=20
	struct ScriptPCode** actionList; // size=8, offset=24
};

struct EventTimer // hashcode: 0xAA55BC81 (dec: -1437221759)
{
	long flags : 1; // offset=0.0
	long time : 31; // offset=0.1
	struct Event* event; // size=28, offset=4
	struct ScriptPCode* actionScript; // size=8, offset=8
	short* scriptPos; // size=0, offset=12
	struct Level* level; // size=392, offset=16
	long nextEventIndex; // size=0, offset=20
};

struct EventInstanceObject // hashcode: 0xC22B6786 (dec: -1037342842)
{
	short id; // size=0, offset=0
	short flags; // size=0, offset=2
	long unitID; // size=0, offset=4
	long introUniqueID; // size=0, offset=8
	struct _Instance* instance; // size=616, offset=12
	union {
		struct Intro* intro; // size=76, offset=0
		struct _SFXMkr* sfxMarker; // size=36, offset=0
	} data; // size=4, offset=16, found in object files: event.o, event.o
};

struct EventEventObject // hashcode: 0x59DD7B24 (dec: 1507687204)
{
	short id; // size=0, offset=0
	short eventNumber; // size=0, offset=2
	long unitID; // size=0, offset=4
	struct Event* event; // size=0, offset=8
};

struct EventTGroupObject // hashcode: 0x9ED92E4E (dec: -1629934002)
{
	short id; // size=0, offset=0
	short tgroupNumber; // size=0, offset=2
	long unitID; // size=0, offset=4
	struct BSPTree* bspTree; // size=36, offset=8
	struct _StreamUnit* stream; // size=0, offset=12
};

extern void EVENT_UpdateResetSignalArrayAndWaterMovement(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel); // 0x800605D0

extern void EVENT_ResetAllOneTimeVariables(); // 0x800606AC

extern void EVENT_AddSignalToReset(struct _MultiSignal *mSignal); // 0x80060714

extern void HINT_ResetHint(); // 0x80060770

extern void HINT_StartHint(short hintNumber); // 0x80060798

extern void HINT_StopHint(); // 0x800607FC

extern void HINT_KillSpecificHint(short hintNumber); // 0x80060860

extern long HINT_GetCurrentHint(); // 0x80060890

extern void EVENT_ProcessTimers(); // 0x800608B0

extern void EVENT_ProcessHints(); // 0x800609CC

extern struct EventTimer * EVENT_GetNextTimer(); // 0x80060B8C

extern void EVENT_RemoveTimer(struct EventTimer *timer); // 0x80060BF8

extern void EVENT_InitTimers(); // 0x80060C28

extern void EVENT_InitTerrainMovement(); // 0x80060C54

extern void EVENT_Init(); // 0x80060CA4

extern struct WaterLevelProcess * EVENT_GetNextTerrainMove(); // 0x80060CF0

extern void EVENT_ProcessMovingWater(); // 0x80060D28

extern void EVENT_DoProcess(); // 0x80060E58

extern void EVENT_BSPProcess(struct _StreamUnit *streamUnit); // 0x80060E80

extern void EVENT_Process(struct Event *eventInstance, long startIndex); // 0x800610C0

extern void EVENT_ProcessPuppetShow(struct Event *eventInstance, long startIndex); // 0x80061220

extern void EVENT_ProcessEvents(struct EventPointers *eventPointers, struct Level *level); // 0x800613F8

extern long EVENT_DoAction(struct Event *eventInstance, struct ScriptPCode *actionScript, short *scriptData); // 0x800614BC

extern long EVENT_IsConditionTrue(struct Event *eventInstance, struct ScriptPCode *conditionScript); // 0x80061634

extern long EVENT_WriteEventObject(struct StackType *stackEntry, long areaID, struct Event *event, long number); // 0x80061724

extern struct _MultiSignal * EVENT_ResolveObjectSignal(struct _StreamUnit *stream, long signalNumber); // 0x800617D4

extern struct Intro * EVENT_ResolveObjectIntro(struct EventInstanceObject *instanceObject); // 0x8006186C

extern struct _SFXMkr * EVENT_ResolveSFXMarker(struct _StreamUnit *stream, struct EventInstanceObject *instanceObject); // 0x80061898

extern void EVENT_AddGameObjectToStack(struct _PCodeStack *stack); // 0x800618F8

extern void EVENT_AddPlayerObjectToStack(struct _PCodeStack *stack); // 0x80061944

extern void EVENT_AddObjectToStack(struct _PCodeStack *stack, long item); // 0x8006199C

extern void EVENT_AddCharPointerToStack(struct _PCodeStack *stack, char *pointer); // 0x80061BD0

extern void EVENT_AddShortPointerToStack(struct _PCodeStack *stack, short *pointer); // 0x80061C18

extern void EVENT_AddNumberToStack(struct _PCodeStack *stack, long item, long flags); // 0x80061C60

extern void EVENT_ChangeOperandToNumber(struct StackType *operand, long item, long flags); // 0x80061CB0

extern void EVENT_ChangeOperandVector3d(struct StackType *operand, short x, short y, short z, long streamUnitID); // 0x80061CC8

extern void EVENT_Addvector3dToStack(struct _PCodeStack *stack, short x, short y, short z, long streamUnitID); // 0x80061CF8

extern void EVENT_ChangeOperandRotation3d(struct StackType *operand, struct Rotation3d *rotation); // 0x80061D70

extern long EVENT_AddSubListObjectToStack(struct _PCodeStack *stack, long listNumber); // 0x80061DC8

extern void EVENT_StackDuplicate(struct _PCodeStack *stack); // 0x80061E54

extern long EVENT_TransformTGroupAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80061EEC

extern long EVENT_TransformConstrictAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item); // 0x80061F7C

extern long EVENT_TransformInstanceAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80061FE8

extern long EVENT_TransformSoundObjectAttribute(struct _PCodeStack *stack, struct SoundObject *soundObject, long item, short *codeStream); // 0x8006264C

extern long EVENT_GetGameValue(struct GameObject *gameObject); // 0x80062764

extern long EVENT_TransformGameAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80062800

extern long EVENT_TransformAreaAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80062C80

extern long EVENT_TransformEventAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80062D5C

extern long EVENT_TransformSavedEventAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80062DEC

extern long EVENT_TransformSubListObjectAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item); // 0x80062EA8

extern long EVENT_TransformListObjectAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item); // 0x80062EDC

extern long EVENT_TransformCameraObjectAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80062F10

extern long EVENT_TransformSplineAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x8006303C

extern long EVENT_TransformIntroAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item); // 0x80063140

extern long EVENT_ParseOperand2(struct StackType *operand2, long *error, long *trueValue); // 0x8006325C

extern long EVENT_DoVMObjectAction(struct EventVmObject *vmobject, struct StackType *operand2); // 0x800632B4

extern long EVENT_GetVMObjectValue(struct EventVmObject *vmobject); // 0x80063474

extern long EVENT_DoGameAction(struct GameObject *gameObject, struct StackType *operand2); // 0x8006353C

extern long EVENT_DoSignalAction(struct SignalObject *signalObject, struct StackType *operand2); // 0x80063830

extern long EVENT_TransformSignalAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item); // 0x800638AC

extern long EVENT_TransformRotation3dAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x80063904

extern long EVENT_TransformVector3dAttribute(struct _PCodeStack *stack, struct StackType *stackObject, long item, short *codeStream); // 0x8006393C

extern void EVENT_ModifyObjectToStackWithAttribute(struct _PCodeStack *stack, long item, short *codeStream); // 0x80063A38

extern long EVENT_DoInstanceAnimateTextureAction(struct InstanceAnimateTexture *instanceAniTexture, struct StackType *operand2); // 0x80063A68

extern void EVENT_ResetAllSplineFlags(struct MultiSpline *spline); // 0x80063B70

extern void EVENT_SetSplineLoop(struct MultiSpline *spline); // 0x80063BF8

extern long EVENT_DoSplineAction(struct InstanceSpline *instanceSpline, struct StackType *operand2); // 0x80063C80

extern long EVENT_DoAnimateAction(struct InstanceAnimate *instanceAnimate, struct StackType *operand2); // 0x8006404C

extern long EVENT_DoInstanceAction(struct InstanceObject *instanceObject, struct StackType *operand2, short *codeStream); // 0x80064320

extern long EVENT_GetTGroupValue(struct TGroupObject *terrainGroup); // 0x80064DC4

extern long EVENT_DoTGroupAction(struct TGroupObject *terrainGroup, struct StackType *operand2); // 0x80064F30

extern long EVENT_DoCameraAction(struct CameraObject *cameraObject, struct StackType *operand2, short *codeStream); // 0x80065238

extern long EVENT_DoObjectSoundAction(struct SoundObject *soundObject, struct StackType *operand2); // 0x800655B8

extern long EVENT_DoSoundMarkerAction(struct SoundObject *soundObject, struct StackType *operand2); // 0x800656EC

extern long EVENT_GetSoundValue(struct SoundObject *soundObject); // 0x80065820

extern long EVENT_DoAreaAction(struct AreaObject *areaObject, struct StackType *operand2); // 0x80065828

extern long EVENT_DoIntroAction(struct IntroObject *introObject, struct StackType *operand2); // 0x800658D4

extern void EVENT_DoStackOperationEquals(struct _PCodeStack *stack, short *codeStream); // 0x80065AE4

extern long EVENT_CompareVector3d(struct Vector3d *vector1, struct Vector3d *vector2); // 0x80065BD0

extern long EVENT_CompareRotationVectors(struct Rotation3d *rot1, struct Rotation3d *rot2, long trueValue); // 0x80065D8C

extern long EVENT_CompareListWithOperation(struct _PCodeStack *stack, struct ListObject *listObject, struct StackType *operand2, long operation); // 0x80065F84

extern long EVENT_CompareSubListWithOperation(struct _PCodeStack *stack, struct SubListObject *subListObject, struct StackType *operand2, long operation); // 0x80066178

extern long EVENT_CompareOperandsWithOperation(struct _PCodeStack *stack, struct StackType *operand1, struct StackType *operand2, long operation); // 0x80066314

extern void EVENT_DoStackMathOperation(struct _PCodeStack *stack, long operation); // 0x8006664C

extern short * EVENT_ParseOpcode(struct _PCodeStack *stack, short *codeStream, long *operateOnStack); // 0x80066834

extern long EVENT_GetIntroValue(struct IntroObject *introObject); // 0x80066AB0

extern void EVENT_DoSubListAction(struct SubListObject *subListObject, struct StackType *operand2, struct _PCodeStack *stack, short *codeStream); // 0x80066B44

extern long EVENT_GetInstanceValue(struct InstanceObject *instanceObject); // 0x80066C3C

extern long EVENT_GetSplineFrameNumber(struct InstanceSpline *instanceSpline); // 0x80066DB4

extern long EVENT_GetSplineValue(struct InstanceSpline *instanceSpline); // 0x80066DF8

extern long EVENT_GetAnimateTextureValue(struct InstanceAnimateTexture *instanceAniTexture); // 0x80066EF4

extern long EVENT_GetAnimateValue(struct InstanceAnimate *instanceAnimate); // 0x80066FA4

extern long EVENT_TransformObjectOnStack(struct _PCodeStack *stack, long item, short *codeStream); // 0x8006701C

extern void EVENT_UpdatePuzzlePointers(struct EventPointers *events, long offset); // 0x80067070

extern void EVENT_SaveEventsFromLevel(long levelID, struct Level *level); // 0x8006724C

extern void EVENT_LoadEventsForLevel(long levelID, struct Level *level); // 0x800673C8

extern struct SavedBasic * EVENT_CreateSaveEvent(long levelID, long eventNumber); // 0x80067504

extern void EVENT_RemoveInstanceFromInstanceList(struct _Instance *instance); // 0x80067570

extern void EVENT_UpdatePuzzleWithInstance(struct EventPointers *puzzle, struct _Instance *instance); // 0x80067658

extern void EVENT_AddInstanceToInstanceList(struct _Instance *instance); // 0x800676FC

extern struct _VMObject * EVENT_FindVMObject(struct _StreamUnit *stream, char *vmoName); // 0x800677E8

extern void EVENT_FixPuzzlesForStream(struct _StreamUnit *stream); // 0x80067888

extern void EVENT_AddStreamToInstanceList(struct _StreamUnit *stream); // 0x80067B6C

extern void EVENT_RemoveStreamToInstanceList(struct _StreamUnit *stream); // 0x80067E54

extern void EVENT_RelocateInstanceList(struct Level *oldLevel, struct Level *newLevel, long sizeOfLevel); // 0x80068018

extern void EVENT_PrintVars(); // 0x80068218

extern void EVENT_ExecuteActionCommand(struct StackType *operand1, struct StackType *operand2, struct _PCodeStack *stack, short *codeStream); // 0x80068220

extern long EVENT_GetScalerValueFromOperand(struct StackType *operand, long *error, short *flags); // 0x800683E8

extern long EVENT_TransformOperand(struct StackType *stackObject, struct _PCodeStack *stack, long item, short *codeStream); // 0x80068558


#endif
