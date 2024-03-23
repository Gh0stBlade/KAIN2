#include "Game/CORE.H"
#include "Game/PLAYER.H"
#include "RAZIEL.H"
#include "Game/GAMELOOP.H"
#include "Game/OBTABLE.H"
#include "Game/COLLIDE.H"
#include "Game/STATE.H"
#include "Game/G2/ANMG2ILF.H"
#include "RAZLIB.H"
#include "HEALTH.H"
#include "Game/DEBUG.H"
#include "CONTROL.H"
#include "Game/CAMERA.H"
#include "Game/STREAM.H"
#include "Game/SAVEINFO.H"
#include "Game/EVENT.H"
#include "RAZDBUG.H"
#include "Game/RAZCNTRL.H"
#include "Game/GLYPH.H"
#include "Game/SOUND.H"
#include "Game/PHYSICS.H"
#include "Game/FX.H"
#include "Game/G2/ANMCTRLR.H"
#include "Game/GAMEPAD.H"
#include "Game/TIMER.H"
#include "Game/RAZIEL/STEERING.H"
#include "Game/RAZIEL/ALGOCTRL.H"
#include "Game/RAZIEL/PUPPET.H"
#include "Game/RAZIEL/ATTACK.H"
#include "Game/RAZIEL/SWIM.H"
#include "Game/COLLIDE.H"
#include "Game/GAMEPAD.H"
#include "SENSES.H"
#include <Game/MATH3D.H>
#include <Game/RAZIEL/SPIDER.H>
#include "Game/GENERIC.H"

struct RazielData* PlayerData;
struct _G2AnimInterpInfo_Type razInterpInfo[3];
struct __EngagedInstance EngagedList[15];
struct __FitPoint constrictData[32];
int ControlFlag;
struct _G2SVector3_Type* ExtraRot;
struct _G2SVector3_Type ExtraRotData;
int WaterStatus;
_Normal Norm;
struct __Force ExternalForces[4];
int LoopCounter;
int AutoFaceAngle;
struct __CannedSound cannedSound[4];
static int BlockCount; 
static int LastBlock;

void InitStates(struct _Instance* PlayerInstance)
{
	unsigned char i;
	struct _G2AnimSection_Type* animSection;

	if (Raziel.footPrint == 0)
	{
		Raziel.footPrint = (uintptr_t)PlayerInstance->object;
	}

	PlayerInstance->InstanceState = &Raziel.State.CharacterInstance;
	Raziel.State.CharacterInstance = PlayerInstance;
	PlayerData = (struct RazielData*)PlayerInstance->data;

	G2EmulationSetTotalSections(&Raziel.State, 3);
	G2EmulationSetStartAndEndSegment(&Raziel.State, 0, 0, 13);
	G2EmulationSetStartAndEndSegment(&Raziel.State, 1, 14, 49);
	G2EmulationSetStartAndEndSegment(&Raziel.State, 2, 50, 65);
	
	for (i = 0; i < 3; i++)
	{
		animSection = &PlayerInstance->anim.section[i];
		animSection->callback = (unsigned long (*)(struct _G2Anim_Type*, int, enum _G2AnimCallbackMsg_Enum, long, long, struct _Instance*)) & RazielAnimCallback;
		animSection->callbackData = NULL;
	
		Raziel.State.SectionList[i].Process = &StateHandlerIdle;

		InitMessageQueue(&Raziel.State.SectionList[i].Event);
		InitMessageQueue(&Raziel.State.SectionList[i].Defer);
	}

	G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[0], &razInterpInfo[0]);
	G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[1], &razInterpInfo[1]);
	G2AnimSection_SetInterpInfo(&PlayerInstance->anim.section[2], &razInterpInfo[2]);

	Raziel.Mode = 1;

	gameTrackerX.debugFlags2 |= 0x800;

	InitHealthSystem();

	debugRazielFlags1 = 0x1000000;

	Raziel.GlyphManaBalls = 0;
	Raziel.GlyphManaMax = 0;
	Raziel.Abilities = 0;
	Raziel.RotationSegment = 0;
	Raziel.extraRot.x = 0;
	Raziel.extraRot.y = 0;
	Raziel.extraRot.z = 0;

	PlayerInstance->rotation.x = 0;
	PlayerInstance->rotation.y = 0;
	PlayerInstance->rotation.z = 0;

	Raziel.Senses.EngagedList = &EngagedList[0];
	Raziel.constrictData = &constrictData[0];
	Raziel.dropOffHeight = 256;
	Raziel.Senses.EngagedMask = 0;
	Raziel.idleInstance = NULL;
	Raziel.soulReaver = NULL;
	Raziel.fallZVelocity = -32;

	G2EmulationSetAnimation(&Raziel.State, 0, 0, 0, 0);
	G2EmulationSetMode(&Raziel.State, 0, 2);

	G2EmulationSetAnimation(&Raziel.State, 1, 0, 0, 0);
	G2EmulationSetMode(&Raziel.State, 1, 2);

	G2EmulationSetAnimation(&Raziel.State, 2, 0, 0, 0);
	G2EmulationSetMode(&Raziel.State, 2, 2);

	ControlFlag = 0x519;

	PlayerInstance->maxXVel = 256;
	PlayerInstance->maxYVel = 256;
	PlayerInstance->maxZVel = 256;

	razSetCowlNoDraw(1);

	InitExternalForces(&ExternalForces[0], 4);

	ResetPhysics(PlayerInstance, -16);

	ExtraRot = NULL;

	CAMERA_ChangeToOutOfWater(&theCamera, PlayerInstance);

	razAttachControllers();

	INSTANCE_Post(PlayerInstance, 0x100006, 0);

	PlayerInstance->checkMask = 0x803E002E;
	PlayerInstance->flags2 |= 0x400;

	G2EmulationInit();

	InitGlyphSystem(PlayerInstance);

	Raziel.slipSlope = 0xB50;
	Raziel.terminator = -1;

	gameTrackerX.raziel_collide_override = 0;
}

void StateInitIdle(struct __CharacterState* In, int CurrentSection, int Ptr)//Matching - 99.73%
{
	struct evControlInitIdleData* data;
	struct _Instance* linkWeapon;

	data = (struct evControlInitIdleData*)Ptr;

	if (data == NULL)
	{
		data = (struct evControlInitIdleData*)SetControlInitIdleData(0, 0, 3);
	}

	linkWeapon = razGetHeldWeapon();

	if (data->mode == 2)
	{
		if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 96, -1, -1) != 0)
		{
			G2EmulationSwitchAnimation(In, CurrentSection, 140, 0, 0, 1);
		}
	}
	else if (data->mode == 3)
	{
		if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 100, -1, -1) != 0)
		{
			G2EmulationSwitchAnimation(In, CurrentSection, 141, 0, 0, 1);
		}
	}
	else if (data->mode == 4)
	{
		if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 104, -1, -1) != 0)
		{
			G2EmulationSwitchAnimation(In, CurrentSection, 142, 0, 0, 1);
		}
	}
	else
	{
		if (linkWeapon == NULL || CurrentSection != 1)
		{
			if (data->mode == 0)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, data->frames, 2);
			}
			else if (data->mode == 1)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 55, 0, data->frames, 2);
			}
		}
		else
		{
			if ((unsigned)(data->mode - 2) < 3)
			{
				data->mode = 0;
			}

			switch (Raziel.Senses.heldClass)
			{
			case 0:
				break;
			case 1:
			{
				if (data->mode == 0)
				{
					G2EmulationSwitchAnimation(In, 1, 50, 0, data->frames, 2);
				}
				else
				{
					G2EmulationSwitchAnimation(In, 1, 127, 0, data->frames, 2);
				}
				break;
			}
			case 0x1000:
			{
				if (data->mode == 0)
				{
					G2EmulationSwitchAnimation(In, 1, 117, 0, data->frames, 2);

					razReaverScale(2800);
				}
				else
				{
					G2EmulationSwitchAnimation(In, 1, 137, 0, data->frames, 2);
				}
				break;
			}
			case 2:
			{
				if (data->mode == 0)
				{
					G2EmulationSwitchAnimation(In, 1, 84, 0, data->frames, 2);
				}
				else
				{
					G2EmulationSwitchAnimation(In, 1, 126, 0, data->frames, 2);
				}
				break;
			}

			case 3:
			{
				G2EmulationSwitchAnimation(In, 1, 98, 0, data->frames, 2);
				break;
			}
			default:
			{
				if (Raziel.Senses.heldClass != CurrentSection && Raziel.Senses.heldClass == CurrentSection)
				{
					if (data->mode == 0)
					{
						G2EmulationSwitchAnimation(In, 1, 50, 0, data->frames, 2);
					}
					else
					{
						G2EmulationSwitchAnimation(In, 1, 127, 0, data->frames, 2);
					}
				}
				break;
			}
			}
		}
	}
}

void StateHandlerIdle(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;
	int Anim;
	int blockForwardMotion;
	struct _Instance* heldInst;
	struct evPhysicsEdgeData* EdgeData;

	blockForwardMotion = 0;

	ControlFlag &= ~0x10;

	G2EmulationQueryFrame(In, CurrentSection);

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)))
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 0x1;

				Raziel.idleCount = 0;

				ControlFlag = 0x2A109;

				PhysicsMode = 0x3;

				SteerSwitchMode(In->CharacterInstance, 0);

				Raziel.movementMinRate = 0;
			}

			StateInitIdle(In, CurrentSection, Ptr->Data);
			break;
		case 0x100004:
			if (CurrentSection == 0)
			{
				razReaverScale(4096);
				razResetPauseTranslation(In->CharacterInstance);
				COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
			}

			break;
		case 0x8000000:
			if (Anim == 215)
			{
				ControlFlag &= ~0x800000;
			}

			if (Anim == 214)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 215, 0, 3, 1);
			}
			else
			{
				StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));

				if (CurrentSection == 0)
				{
					if (!(Raziel.playerEventHistory & 0x10000))
					{
						ControlFlag &= ~0x8000;
					}

					COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
				}
			}

			break;
		case 0x80000000:
			if (!(Raziel.Senses.Flags & 0x80))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerAttack2, 0);
			}

			break;
		case 0x80000002:
			if (CurrentSection == 0)
			{
				if ((PadData[0] & RazielCommands[1]))
				{
					Raziel.returnState = NULL;

					StateSwitchStateCharacterData(In, &StateHandlerSoulSuck, 0);
				}
			}

			break;
		case 0x4010401:
			blockForwardMotion = 1;
			break;
		case 0x4010080:
			if (CurrentSection == 0)
			{
				if (Ptr->Data != 0)
				{
					razResetPauseTranslation(In->CharacterInstance);
				}
				else
				{
					razSetPauseTranslation(In->CharacterInstance);
				}
			}

			break;
		case 0x10000000:
			if ((PadData[0] & RazielCommands[7]))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
			}
			else
			{
				if (Raziel.Bearing < -512)
				{
					if (CurrentSection == 1)
					{
						if (razGetHeldWeapon() == NULL)
						{
							G2EmulationSwitchAnimation(In, CurrentSection, 54, 0, 2, 1);
						}
					}
					else
					{
						G2EmulationSwitchAnimation(In, CurrentSection, 54, 0, 2, 1);
					}

					StateSwitchStateData(In, CurrentSection, &StateHandlerStartTurn, 0);

					In->SectionList[CurrentSection].Data1 = 52;
				}
				else if (Raziel.Bearing >= 513)
				{
					if (CurrentSection == 1)
					{
						if (In->CharacterInstance->LinkChild == NULL)
						{
							G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
						}
					}
					else
					{
						G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
					}

					StateSwitchStateData(In, CurrentSection, &StateHandlerStartTurn, 0);

					In->SectionList[CurrentSection].Data1 = 51;
				}
				else if (blockForwardMotion == 0)
				{
					if (Raziel.Magnitude < 4096)
					{
						StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
					}
					else
					{
						StateSwitchStateData(In, CurrentSection, &StateHandlerStartMove, 0);
					}
				}
			}

			break;
		case 0x80000001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 0x10;
				ControlFlag |= 0x10;

				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 16, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 34, 0, 2, 1);
				}

				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
			}

			break;
		case 0x2000000:
			if ((Raziel.Senses.EngagedMask & 0x1) && (Raziel.Senses.heldClass != 0x3))
			{
				Raziel.Mode = 0x200;

				In->SectionList[CurrentSection].Data1 = 0;

				if (CurrentSection == 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 21, 0, 6, 1);

					StateSwitchStateCharacterData(In, &StateHandlerPushObject, 0);
				}

				ControlFlag &= ~0x1;
			}
			else
			{
				if ((Raziel.Senses.EngagedMask & 0x8) && (Raziel.Senses.heldClass != 0x3))
				{
					if (CurrentSection == 0)
					{
						StateSwitchStateCharacterData(In, &StateHandlerPullSwitch, 0);
					}
				}
				else
				{
					if ((Raziel.Senses.EngagedMask & 0x2010))
					{
						if (CurrentSection == 0)
						{
							if (Raziel.Senses.heldClass == 0x3)
							{
								heldInst = razGetHeldItem();

								if (heldInst != NULL)
								{
									if ((INSTANCE_Query(heldInst, 2) & 0x20))
									{
										StateSwitchStateCharacterData(In, &StateHandlerBreakOff, 0);
									}
								}
							}
							else
							{
								StateSwitchStateCharacterData(In, &StateHandlerBreakOff, 0);
							}
						}
					}
					else
					{
						if ((Raziel.Senses.EngagedMask & 0x800))
						{
							if (CurrentSection == 1)
							{
								razReaverPickup(In->CharacterInstance, Raziel.Senses.EngagedList[11].instance);
							}
						}
						else
						{
							if ((Raziel.Senses.EngagedMask & 0x4000))
							{
								if (CurrentSection == 0)
								{
									StateSwitchStateCharacterData(In, &StateHandlerWarpGate, 0);
								}
							}
							else
							{
								if ((razPickupAndGrab(In, CurrentSection) != 0) && (CurrentSection == 0) && (!(Raziel.Senses.Flags & 0x80)))
								{
									StateSwitchStateCharacterData(In, &StateHandlerAttack2, 0);
								}
							}
						}
					}
				}
			}

			break;
		case 0x4010400:
			EdgeData = (struct evPhysicsEdgeData*)Ptr->Data;

			if ((EdgeData->rc & 0x20000) && (Raziel.Abilities & 0x1) && (Raziel.CurrentPlane == 2))
			{
				Raziel.playerEvent |= 0x10000;
			}

			break;
		case 0x80000010:
			if (CurrentSection == 0)
			{
				if ((Raziel.Senses.Flags & 0x4))
				{
					if ((Raziel.Senses.Flags & 0x80) || (gameTrackerX.streamFlags & 0x4))
					{
						EnMessageQueueData(&In->SectionList[0].Defer, 0x80000010, 0);
					}
					else
					{
						StateSwitchStateCharacterData(In, &StateHandlerGlyphs, 0);
					}
				}
			}

			break;
		case 0x2000001:
		case 0x2000002:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}

	if ((CurrentSection == 0) && (CheckHolding(In->CharacterInstance) == 0) && (Anim != 55) && (Anim != 214)
		&& (Raziel.idleCount++, Raziel.idleCount >= 301) && (Raziel.idleCount == (Raziel.idleCount / 900) * 900))
	{
		if (Raziel.idleInstance != NULL)
		{
			G2EmulationInstanceToInstanceSwitchAnimationCharacter(In->CharacterInstance, Raziel.idleInstance, 1, 0, 3, 1);
		}
		else
		{
			G2EmulationSwitchAnimationCharacter(In, 24, 0, 3, 1);

			COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);
		}
	}
}


void StateHandlerLookAround(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;
	struct _Instance* instance;
	int message;
	int messageData;
	struct _G2Anim_Type* anim; // not from SYMDUMP

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x100001:
				if (CurrentSection == 1)
				{
					anim = &In->CharacterInstance->anim;

					G2Anim_EnableController(anim, 17, 14);
					G2Anim_EnableController(anim, 16, 14);
					G2Anim_EnableController(anim, 14, 14);

					ControlFlag = 0x8001008;
				}

				if (G2EmulationQueryAnimation(In, CurrentSection) == 24)
				{
					StateInitIdle(In, CurrentSection, 0);
				}

				break;
			case 0x100004:
				if (CurrentSection == 1)
				{
					instance = In->CharacterInstance;

					G2Anim_InterpDisableController(&instance->anim, 17, 14, 300);
					G2Anim_InterpDisableController(&instance->anim, 16, 14, 300);
					G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);
				}

				break;
			case 0x80000020:
				if (StateHandlerDecodeHold(&message, &messageData) != 0)
				{
					if ((message == 0x80000) && (CurrentSection == 0))
					{
						razLaunchForce(In->CharacterInstance);

						StateSwitchStateData(In, 0, &StateHandlerThrow2, 0);
					}

					StateSwitchStateData(In, CurrentSection, &StateHandlerThrow2, 0);

					if (Raziel.Senses.heldClass != 0x1000)
					{
						if (Raziel.Senses.heldClass != 0x8)
						{
							razSetFadeEffect(In->CharacterInstance->fadeValue, PlayerData->throwFadeValue,

								PlayerData->throwFadeInRate);
						}
					}

					Raziel.returnState = &StateHandlerIdle;

					Raziel.throwMode = 0x2;
				}

				break;
			case 0x100000:
				StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
				break;
			case 0x1000000:
				if (CurrentSection == 0)
				{
					CAMERA_ForceEndLookaroundMode(&theCamera);
				}

				StateSwitchStateData(In, CurrentSection, &StateHandlerHitReaction, Ptr->Data);
				break;
			case 0x40005:
			case 0x40025:
				StateSwitchStateData(In, CurrentSection, &StateHandlerStumble, Ptr->Data);
				break;
			}
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}

	if ((!(PadData[0] & RazielCommands[5])) || (!(PadData[0] & RazielCommands[4])))
	{
		EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
	}
}


void StateHandlerCrouch(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.73%
{
	struct __Event* Ptr;
	int Anim;
	struct _Instance* heldInst;
	int DropThisFrame;
	struct evObjectData* data;
	int i;

	DropThisFrame = 0;
	heldInst = razGetHeldItem();
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		Anim = G2EmulationQueryAnimation(In, CurrentSection);
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 64;
				ControlFlag = 0x8109;
				PhysicsMode = 3;
				SteerSwitchMode(In->CharacterInstance, 0);
				if (Ptr->Data != 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 72, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 3, 0, 3, 1);
					}
				}
				else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 76, NULL, NULL))
				{
					G2EmulationSwitchAnimationCharacter(In, 4, 0, 8, 2);
				}
				if (heldInst != NULL)
				{
					INSTANCE_Post(heldInst, 0x80002C, 0);
				}
			}
			if (Ptr->Data != 0)
			{
				In->SectionList[CurrentSection].Data2 = 72;
			}
			else
			{
				In->SectionList[CurrentSection].Data2 = 0;
			}
			break;
		case 0x100004:
			if (CurrentSection == 0)
			{
				COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
				if (heldInst != NULL)
				{
					INSTANCE_Post(heldInst, 0x200003, 7);
					INSTANCE_Post(heldInst, 0x80002B, 0);
				}
			}
			break;
		case 0x8000000:
			if ((Anim == 5) || (Anim == 85))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				ControlFlag &= 0xF7FFEFFF;
				In->SectionList[CurrentSection].Data1 = 1;
				break;
			}
			if (CurrentSection == 1)
			{
				if (((In->SectionList[CurrentSection].Data2 == 80) && (heldInst != NULL)) && (DropThisFrame == 0))
				{
					INSTANCE_Post(heldInst, 0x800008, 0);
					razReaverOn();
				}
				In->SectionList[0].Data2 = 76;
				In->SectionList[CurrentSection].Data2 = 76;
				In->SectionList[2].Data2 = 76;
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 76, NULL, NULL))
				{
					G2EmulationSwitchAnimationCharacter(In, 4, 0, 8, 2);
				}
			}
			break;
		case 0x100000:
			StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
			break;
		case 0x2000000:
			if (((Raziel.Senses.EngagedMask & 4) != 0) && (Raziel.Senses.heldClass != 3) && (Anim != 5) && (Anim != 85))
			{
				ControlFlag = (ControlFlag | 0x8041000) & -9;
				if (CurrentSection == 0)
				{
					for (i = 0; i < 3; i++)
					{
						In->SectionList[i].Data1 = 0;
						PurgeMessageQueue(&In->SectionList[i].Event);
					}
					razCenterWithBlock(In->CharacterInstance, Raziel.Senses.EngagedList[2].instance, -141);
					data = (struct evObjectData*)SetObjectData(-(int)Raziel.Senses.ForwardNormal.x,
						-(int)Raziel.Senses.ForwardNormal.y, 0, NULL, 0);
					INSTANCE_Post(Raziel.Senses.EngagedList[2].instance, 0x800001, (int)data);
					if ((data->rc & 1) == 0)
					{
						INSTANCE_Post(In->CharacterInstance, 0x100000, 0);
						break;
					}
					COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);
					if ((data->rc & 8) != 0)
					{
						G2EmulationSwitchAnimationCharacter(In, 85, 0, 0, 1);
					}
					else
					{
						G2EmulationSwitchAnimationCharacter(In, 5, 0, 0, 1);
					}
					razSetPlayerEventHistory(2);
				}
			}
			break;
		case 0x80000001:
			if ((CurrentSection == 0) && (Anim != 5) && (Anim != 85))
			{
				Raziel.Mode = 32;
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 32, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 38, 0, 1, 1);
				}
				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
			}
			break;
		case 0x80000010:
			if (CurrentSection == 0)
			{
				if (In->CharacterInstance->tface != NULL)
				{
					EnMessageQueueData(&In->SectionList[0].Defer, 0x80000010, 0);
					ControlFlag |= 0x800000;
				}
			}
			if ((Anim != 5) && (Anim != 85))
			{
				if (In->SectionList[CurrentSection].Data2 != 80)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
					Raziel.Mode = 0x1000000;
				}
			}
			break;
		case 0x20000008:
			if ((Anim != 5) && (Anim != 85))
			{
				if (In->SectionList[CurrentSection].Data2 != 80)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
					Raziel.Mode = 0x1000000;
				}
			}
			break;
		case 0x80000000:
			if (heldInst != NULL)
			{
				DropThisFrame = 1;
				razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 80, -1, -1);
				In->SectionList[CurrentSection].Data2 = 80;
				INSTANCE_Post(heldInst, 0x200005, 0);
			}
			break;
		case 0x10000000:
			if ((Raziel.Senses.heldClass != 3) && (Anim != 5) && (Anim != 85))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
			}
			break;
		case 0x80000020:
			if ((Anim != 5) && (Anim != 85))
			{
				DefaultStateHandler(In, CurrentSection, Data);
			}
			break;
		case 0x80000008:
			break;
		case 0x80000004:
			break;
		case 0x4020000:
			break;
		case 0x4010400:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	if (((PadData[0] & RazielCommands[6]) == 0) && (In->SectionList[CurrentSection].Process == &StateHandlerCrouch))
	{
		EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x20000008, 0);
	}
}


void StateHandlerDropAction(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x100001:
				if ((razGetHeldItem() != NULL) && (CurrentSection == 1))
				{
					switch (Raziel.Senses.heldClass)
					{
					case 0:
						break;
					case 1:
						G2EmulationSwitchAnimation(In, 1, 83, 0, 3, 1);
						break;
					case 0x1000:
						break;
					case 2:
						G2EmulationSwitchAnimation(In, 1, 105, 0, 3, 1);
						break;
					case 3:
						G2EmulationSwitchAnimation(In, 1, 99, 0, 3, 1);
						break;
					}

					INSTANCE_Post(razGetHeldItem(), 0x80002C, 0);
				}
				else if (CurrentSection == 2)
				{
					G2EmulationSwitchAnimationAlpha(In, 2, 69, 0, 3, 1, 1);
				}
				else if (CurrentSection == 0)
				{
					G2EmulationSwitchAnimationAlpha(In, 0, 3, 0, 3, 1, 1);

					Raziel.Mode = 0x40;
				}

				break;
			case 0x8000000:
				if (CurrentSection == 1)
				{
					if (razGetHeldItem() != NULL)
					{
						INSTANCE_Post(razGetHeldItem(), 0x800008, 0);

						razReaverOn();
					}

					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}

				break;
			case 0x10000000:
				break;
			case 0x80000001:
				break;
			case 0x20000008:
				break;
			default:
				DefaultStateHandler(In, CurrentSection, Data);
			}
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerSoulSuck(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.61%
{
	struct __Event* Ptr;
	int Anim;
	struct evPhysicsSwimData* SwimData;

	if ((LoopCounter & 3) == 0)
	{
		FX_MakeSoulDust(In->CharacterInstance, 16);
	}
	Anim = G2EmulationQueryAnimation(In, CurrentSection);
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				ControlFlag = 9;
				PhysicsMode = 3;
				if ((Raziel.Senses.EngagedMask & 0x1000) && Raziel.Senses.heldClass != 3)
				{
					razAlignYRotMove(Raziel.Senses.EngagedList[12].instance, 220, &In->CharacterInstance->position, &In->CharacterInstance->rotation, 0);
					INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 1);
				}
				G2EmulationSwitchAnimationAlpha(In, CurrentSection, 78, 0, 4, 1, 4);
			}
			if (CurrentSection == 2)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, 4, 2);
			}
			if (CurrentSection == 1)
			{
				if (razGetHeldWeapon() != NULL || (Raziel.Senses.EngagedMask & 0x1000))
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 94, 0, 4, 1);
				}
				else
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 78, 0, 4, 1);
				}
				Raziel.Mode |= 0x10000000;
			}
			break;
		case 0x100004:
			if (CurrentSection == 1)
			{
				razSetCowlNoDraw(1);
				ControlFlag &= 0xFFFFFFBF;
				Raziel.Mode &= 0xEFFFFFFF;
				if (Raziel.Senses.EngagedMask & 0x1000)
				{
					INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0);
				}
			}
			break;
		case 0x1000006:
		case 0x20000002:
			if (Anim == 79)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 47, 0, 3, 1);
				PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
			}
			else
			{
				if (Raziel.returnState == NULL)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
				}
			}
			if (((Raziel.Senses.EngagedMask & 0x1000) != 0) && (CurrentSection == 0))
			{
				INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0);
			}
			break;
		case 0x8000000:
			if (Anim == 47)
			{
				if (Raziel.returnState != NULL)
				{
					StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
			if (Anim == 78)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 79, 0, 0, 1);
			}
			if (Anim == 80)
			{
				if ((PadData[0] & RazielCommands[1]))
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 79, 0, 8, 1);
					break;
				}
				if (Raziel.returnState != NULL)
				{
					StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
			break;
		case 0x1000016:
			if ((gameTrackerX.debugFlags2 & 0x800) && (CurrentSection == 1))
			{
				GainHealth(Ptr->Data);
				razSetPlayerEventHistory(4096);
			}
			break;
		case 0x1000009:
			G2EmulationSwitchAnimation(In, CurrentSection, 80, 0, 2, 1);
			break;
		case 0x4020000:
			SwimData = (struct evPhysicsSwimData*)Ptr->Data;
			if (SwimData->rc & 64)
			{
				StateHandlerInitSwim(In, CurrentSection, 0);
			}
			break;
		case 0x80000010:
			break;
		case 0x1000001:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerStartTurn(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.65%
{
	struct __Event* Ptr;

	G2EmulationQueryFrame(In, CurrentSection);
	if (Raziel.Bearing == 0)
	{
		EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
	}
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 0x4000;
				ControlFlag = 0x20109;
				PhysicsMode = 3;
				SteerSwitchMode(In->CharacterInstance, 1);
			}
			break;
		case 0x100000:
		case 0x8000000:
		case 0:
			if (CurrentSection == 0)
			{
				if ((PadData[0] & 0x8000000F) == 0)
				{
					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
				else
				{
					StateSwitchStateCharacterData(In, &StateHandlerStartMove, 0);
				}
			}
			break;
		case 0x80000001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 8;
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
				}
				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
			}
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateInitStartMove(struct __CharacterState* In, int CurrentSection, int Frame)//Matching - 92.02%
{
	if ((PadData[0] & RazielCommands[7]))
	{
		StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
	}

	if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 48, Frame, -1) != 0)
	{
		razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 0, Frame, -1);
	}

	ControlFlag |= 0x2000;
}

void StateHandlerStartMove(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.23%
{
	struct __Event* Ptr;
	int mode;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			StateInitStartMove(In, CurrentSection, Ptr->Data);
			In->SectionList[CurrentSection].Data2 = 1;
			if (CurrentSection == 0)
			{
				ControlFlag = 0x2A119;
				Raziel.Mode &= 0x200800;
				Raziel.Mode |= 4;
				PhysicsMode = 3;
				SteerSwitchMode(In->CharacterInstance, 2);
				Raziel.movementMinRate = 3276;
				Raziel.movementMinAnalog = 1024;
				Raziel.movementMaxAnalog = 4096;
				Raziel.passedMask = 0;
			}
			break;
		case 0x8000000:
		case 0x8000001:
			StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 0);
			break;
		case 0x4010080:
			if (CurrentSection != 2)
			{
				if (Ptr->Data != 0)
				{
					G2EmulationSetMode(In, CurrentSection, 1);
				}
				else
				{
					G2EmulationSetMode(In, CurrentSection, 0);
				}
			}
			if ((Raziel.passedMask & 2) != 0)
			{
				if ((PadData[0] & 0x8000000F) != 0)
				{
					G2EmulationSetMode(In, CurrentSection, 2);
					ControlFlag &= -0x2001;
				}
			}
			break;
		case 0x2000000:
			if ((Raziel.Senses.EngagedMask & 32) != 0)
			{
				razPickupAndGrab(In, CurrentSection);
				break;
			}
		case 0x80000002:
		case 0x80000010:
			if (CurrentSection == 0)
			{
				if (In->CharacterInstance->tface != NULL)
				{
					EnMessageQueueData(&In->SectionList[0].Defer, Ptr->ID, 0);
					ControlFlag |= 0x800000;
				}
			}
		case 0:
			if (CurrentSection == 0)
			{
				if (((Raziel.passedMask & 7) != 0) || ((G2EmulationQueryMode(In, 0)) == 0))
				{
					mode = Raziel.passedMask;
					if ((Raziel.passedMask & 1) != 0)
					{
						mode = 2;
					}
					else if ((Raziel.passedMask & 2) != 0)
					{
						mode = 3;
					}
					else
					{
						mode &= 4;
					}
					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(mode, 5, 5));
					ControlFlag &= -0x2001;
				}
				ControlFlag |= 0x2000;
			}
			break;
		case 0x80000001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 8;
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
				}
				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
				ControlFlag &= -0x2001;
			}
			break;
		case 0x80000000:
			if ((Raziel.passedMask & 4) != 0)
			{
				if (CurrentSection == 1)
				{
					if ((Raziel.Senses.Flags & 128) == 0)
					{
						StateSwitchStateData(In, 1, &StateHandlerAttack2, 10);
					}
					break;
				}
				StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 0);
				break;
			}
			if ((CurrentSection == 0) && (Raziel.Senses.Flags & 128) == 0)
			{
				StateSwitchStateCharacterData(In, &StateHandlerAttack2, 0);
			}
			break;
		case 0x80000004:
			StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
			break;
		case 0x4000001:
			if (CurrentSection == 0)
			{
				if ((G2EmulationQueryFrame(In, 0) < 7) == 0)
				{
					PhysicsMode = 0;
					SetDropPhysics(In->CharacterInstance, &Raziel);
					if ((In->CharacterInstance->zVel < -32) != 0)
					{
						if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
						{
							G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
						}
						StateSwitchStateCharacterData(In, &StateHandlerFall, 0);
					}
				}
			}
			break;
		case 0x4010401:
			StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 5, 5));
			break;
		case 0x10000000:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	if (((PadData[0] & 0x8000000F) != 0) && (In->SectionList[CurrentSection].Data2 != 0))
	{
		if ((++In->SectionList[CurrentSection].Data2 >= 8) != 0)
		{
			G2EmulationSetMode(In, CurrentSection, 1);
			ControlFlag &= 0xFFFFDFFF;
			In->SectionList[CurrentSection].Data2 = 0;
		}
	}
	if ((G2EmulationQueryFrame(In, CurrentSection) >= 11) && (CurrentSection == 0) && (CheckHolding(In->CharacterInstance) != 0))
	{
		if (In->SectionList[1].Process == &StateHandlerStartMove)
		{
			StateSwitchStateData(In, 1, &StateHandlerMove, 11);
		}
		if (In->SectionList[2].Process == &StateHandlerStartMove)
		{
			StateSwitchStateData(In, 2, &StateHandlerMove, 11);
		}
	}
	if ((Raziel.Magnitude != 0) && ((Raziel.Magnitude < 4096) != 0))
	{
		if ((Raziel.passedMask & 1))
		{
			StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 0);
		}
	}
}

void StateInitMove(struct __CharacterState* In, int CurrentSection, int Frames)//Matching - 97.69%
{
	if ((PadData[0] & RazielCommands[7]))
	{
		Raziel.Mode = 2;

		if ((ControlFlag & 0x800000))
		{
			ControlFlag = 0x800000;
		}
		else
		{
			ControlFlag = 0;
		}

		ControlFlag |= 0x22119;

		if (In->SectionList[CurrentSection].Data2 != 52)
		{
			razResetMotion(In->CharacterInstance);

			if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 52, -1, -1) != 0)
			{
				razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 1, -1, -1);
			}

			In->SectionList[CurrentSection].Data2 = 52;
		}
	}
	else
	{
		if ((PadData[0] & RazielCommands[6]))
		{
			Raziel.Mode = 0x1000000;

			if ((ControlFlag & 0x800000))
			{
				ControlFlag = 0x800000;
			}
			else
			{
				ControlFlag = 0;
			}

			ControlFlag |= 0x2119;

			if (In->SectionList[CurrentSection].Data2 != 56)
			{
				razResetMotion(In->CharacterInstance);

				if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 56, -1, -1) != 0)
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 74, 0, 3, 2);
				}

				In->SectionList[CurrentSection].Data2 = 56;
			}
		}
		else
		{
			Raziel.Mode &= 0x200800;

			Raziel.Mode |= 0x4;

			ControlFlag |= 0x28119;

			if ((Raziel.Abilities & 0x20))
			{
				ControlFlag |= 0x200000;
			}

			razSelectMotionAnim(In, CurrentSection, Frames, &In->SectionList[CurrentSection].Data2);
		}
	}
}

void StateHandlerMove(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.54%
{
	struct __Event* Ptr;
	int Anim;
	int data;

	Anim = G2EmulationQueryAnimation(In, CurrentSection);
	In->SectionList[CurrentSection].Data1++;
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			In->SectionList[CurrentSection].Data2 = -1;
			StateInitMove(In, CurrentSection, Ptr->Data);
			Raziel.constrictFlag = 1;
			SteerSwitchMode(In->CharacterInstance, 2);
			In->SectionList[CurrentSection].Data1 = 0;
			Raziel.passedMask |= 0x1000;
			break;
		case 0x100004:
			FX_EndConstrict(0, NULL);
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80000008:
			break;
		case 0x4010080:
			if (CurrentSection == 0)
			{
				if (Ptr->Data != 0)
				{
					razResetPauseTranslation(In->CharacterInstance);
				}
				else
				{
					razSetPauseTranslation(In->CharacterInstance);
				}
			}
			break;
		case 0x2000000:
			if ((Raziel.Senses.EngagedMask & 32) != 0)
			{
				razPickupAndGrab(In, CurrentSection);
				break;
			}
		case 0x80000002:
			if ((PadData[0] & RazielCommands[6]) != 0)  // double check command index
			{
				break;
			}
		case 0x80000010:
			if (CurrentSection != 0)
			{
				break;
			}
			if (In->CharacterInstance->tface != NULL)
			{
				EnMessageQueueData(&In->SectionList[0].Defer, Ptr->ID, 0);
				ControlFlag |= 0x800000;
			}
		case 0:
			if (CurrentSection == 0)
			{
				if ((Raziel.steeringMode == 9) || (Raziel.steeringMode == 14) || (Raziel.steeringMode == 15))
				{
					razApplyMotion(In, CurrentSection);
				}
				else
				{
					if ((Raziel.Mode == 2) || (Anim == 123) || (Anim == 124))
					{
						StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
					}
					else if (Raziel.Mode == 0x1000000)
					{
						StateSwitchStateCharacterData(In, &StateHandlerCrouch, 0);
					}
					else if ((Ptr->Data < 4) && (ControlFlag & 0x800000) == 0)
					{
						ControlFlag |= 0x2000;
						if ((PadData[0] & 0x8000000F) == 0)
						{
							EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0, Ptr->Data + 1);
							razApplyMotion(In, 0);
						}
					}
					else
					{
						data = 0;
						if ((Raziel.passedMask & 0x2000) != 0)
						{
							data = 30;
						}
						if ((Raziel.passedMask & 0x1000) != 0)
						{
							data = 60;
						}
						if (CurrentSection == 0)
						{
							StateSwitchStateCharacterData(In, &StateHandlerStopMove, data);
						}
					}
				}
			}
			break;
		case 0x10000000:
			if ((Raziel.Magnitude < 0x1000) != 0)
			{
				StateInitMove(In, CurrentSection, 3);
			}
			else
			{
				StateInitMove(In, CurrentSection, 0);
			}
			break;
		case 0x80000004:
			if ((Raziel.Mode & 0x200000) == 0)
			{
				if (CurrentSection == 2)
				{
					G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
				}
				else
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 1, 0, 3, 2);
				}
				Raziel.Mode = 2;
				ControlFlag |= 0x2000;
			}
			break;
		case 0x20000004:
		case 0x20000008:
			Raziel.Mode = 4;
			ControlFlag &= ~0x2000;
			break;
		case 0x80000001:
			if (CurrentSection == 0)
			{
				if ((PadData[0] & RazielCommands[6]) != 0)  // double check command index
				{
					Raziel.Mode = 32;
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 32, NULL, NULL) != 0)
					{
						G2EmulationSwitchAnimationCharacter(In, 26, 0, 1, 1);
					}
				}
				else
				{
					Raziel.Mode = 8;
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
					{
						G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
					}
				}
				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
				ControlFlag &= ~0x2000;
			}
			break;
		case 0x80000000:
			if ((PadData[0] & RazielCommands[7]) == 0)  // double check command index
			{
				if (In->SectionList[CurrentSection].Data2 == 68)
				{
					if ((CurrentSection == 1) && (Raziel.Senses.Flags & 0x80) == 0)
					{
						StateSwitchStateData(In, 1, &StateHandlerAttack2, 10);
					}
					Raziel.dropOffHeight = 200;
					Raziel.fallZVelocity = -96;
					break;
				}
				else if (In->SectionList[CurrentSection].Data2 == 56)
				{
					if ((razGetHeldItem() != NULL) && (CurrentSection == 0))
					{
						StateSwitchStateCharacterData(In, &StateHandlerDropAction, 0);
					}
				}
				else
				{
					if ((Raziel.Senses.Flags & 0x80) == 0)
					{
						StateSwitchStateData(In, CurrentSection, &StateHandlerAttack2, 0);
					}
				}
			}
			else
			{
				if ((Raziel.Senses.Flags & 0x80) == 0)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerAttack2, 0);
				}
			}
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	if ((CurrentSection == 0) && (In->SectionList[CurrentSection].Process != &StateHandlerMove))
	{
		razResetMotion(In->CharacterInstance);
	}
}

void StateHandlerStopMove(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.71%
{
	struct __Event* Ptr;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 4;
				if ((ControlFlag & 0x800000) != 0)
				{
					ControlFlag = 0x800000;
				}
				else
				{
					ControlFlag = 0;
				}
				ControlFlag |= 0x2119;
				PhysicsMode = 3;
			}
			if (Ptr->Data == 60)
			{
				if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 88, -1, -1) != 0)
				{
					razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 2, -1, -1);
				}
			}
			else if (Ptr->Data == 30)
			{
				if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 92, -1, -1) != 0)
				{
					razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 3, -1, -1);
				}
			}
			else
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 6));
			}
			break;
		case 0x10000000:
			StateSwitchStateData(In, CurrentSection, &StateHandlerStartMove, 0);
			break;
		case 0x4010080:
			if (CurrentSection == 0)
			{
				if (Ptr->Data == 0)
				{
					razSetPauseTranslation(In->CharacterInstance);
					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 5));
				}
				else
				{
					razResetPauseTranslation(In->CharacterInstance);
				}
			}
			break;
		case 0x2000000:
		case 0x80000002:
		case 0x80000010:
			EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, 0);
			break;
		case 0x8000000:
		case 0x8000001:
			StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 5));
			break;
		case 0x80000001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 16;
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
				}
				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
			}
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


void StateHandlerCompression(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.36%
{
	struct __Event* Ptr;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				ControlFlag = 273;
				SetExternalForce(&ExternalForces[2], 0, 0, 0, 1, 0);
				In->SectionList[0].Data1 = 0;
				PhysicsMode = 3;
				Raziel.movementMinRate = 0;
				switch (Raziel.Mode)
				{
				case 32:
					SteerSwitchMode(In->CharacterInstance, 4);
					Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;
					In->CharacterInstance->yVel = 21;
					if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) == G2FALSE)
					{
						G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
					}
					break;
				case 15:
					break;
				case 16:
					SteerSwitchMode(In->CharacterInstance, 4);
					Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;
					In->CharacterInstance->yVel = 40;
					if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) == G2FALSE)
					{
						G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
					}
					break;
				}
			}
			break;
		case 0x8000000:
			switch (Raziel.Mode)
			{
			case 8:
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 4, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 27, 0, 0, 1);
					}
					SetPhysics(In->CharacterInstance, -16, 0, 83, 154);
				}
				break;
			case 32:
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 36, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 39, 0, 0, 1);
					}
					SetPhysics(In->CharacterInstance, -16, 0, 21, 195);
					In->CharacterInstance->yVel = 0;
				}
				break;
			default:
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 20, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 35, 0, 0, 1);
					}
					SetPhysics(In->CharacterInstance, -16, 0, 40, 154);
					In->CharacterInstance->yVel = 0;
					if (In->SectionList[CurrentSection].Data1 == 1)
					{
						In->CharacterInstance->zVel = 2 * In->CharacterInstance->zVel / 3;
					}
				}
			}
			StateSwitchStateData(In, CurrentSection, &StateHandlerJump, 0);
			break;
		case 0x20000001:
			if (CurrentSection == 0)
			{
				if (Raziel.Mode == 16)
				{
					In->SectionList[CurrentSection].Data1 = 1;
				}
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
			}
			break;
		case 0x80000001:
			EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
			break;
		case 0x4000001:
			break;
		case 0x4020000:
			break;
		case 0x80000000:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerJump(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 98.94%
{
	struct __Event* Ptr;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				ControlFlag = 1297;
				Raziel.alarmTable = 200;
				PhysicsMode = 0;
				In->CharacterInstance->anim.section[CurrentSection].swAlarmTable = &Raziel.alarmTable;
			}
			In->SectionList[CurrentSection].Data2 = 0;
			break;
		case 0x8000004:
			ControlFlag |= 8;
			break;
		case 0x8000000:
			if (CurrentSection == 0)
			{
				if (Raziel.Mode == 16)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
					}
				}
				else if (Raziel.Mode == 32)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 40, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 40, 0, 10, 1);
					}
				}
				else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, NULL, NULL))
				{
					G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);
				}
			}
			StateSwitchStateData(In, CurrentSection, &StateHandlerFall, 0);
			if ((PadData[0] & RazielCommands[3]) == 0)
			{
				In->SectionList[CurrentSection].Data2 = 1;
			}
			break;
		case 0x20000001:
			In->SectionList[CurrentSection].Data2 = 1;
			if (CurrentSection == 0)
			{
				if ((Raziel.Mode == 16) || (Raziel.Mode == 32))
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
				}
				else if (((In->SectionList[CurrentSection].Data1 != 0)
					|| ((In->SectionList[CurrentSection].Data1 = G2EmulationQueryFrame(In, CurrentSection) + 4) != 0))
					&& (In->SectionList[CurrentSection].Data1 < G2EmulationQueryFrame(In, CurrentSection)))
				{
					SetDropPhysics(In->CharacterInstance, &Raziel);
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, NULL, NULL) != 0)
					{
						G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);
					}
					StateSwitchStateCharacterData(In, &StateHandlerFall, In->SectionList[CurrentSection].Data2);
				}
				else
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
				}
			}
			break;
		case 0x80000001:
			if (G2EmulationQueryFrame(In, CurrentSection) >= 2)
			{
				if ((Raziel.Senses.heldClass != 3) && (CurrentSection == 0))
				{
					StateSwitchStateCharacterData(In, &StateHandlerGlide, 0);
				}
			}
			else
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
			}
			break;
		case 0x2000000:
			razPickupAndGrab(In, CurrentSection);
			break;
		case 0x40005:
			break;
		case 0x1000001:
			break;
		case 0x4000001:
			break;
		case 0x4010008:
			break;
		case 0x4020000:
			break;
		case 0x80000000:
			break;
		case 0x80000008:
			break;
		case 0x80000020:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerFall(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.23%
{
	struct __Event* Ptr;
	int Moving;
	int DeferFlag;
	struct evPhysicsSwimData* SwimData;

	Moving = 0;
	DeferFlag = 1;
	if (CurrentSection == 0 && (STREAM_GetLevelWithID(In->CharacterInstance->currentStreamUnitID)->unitFlags & 0x1000))
	{
		EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, Moving);
	}
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				ControlFlag = 0x119;
				if (Raziel.Mode != 256)
				{
					ControlFlag = 0x519;
				}
				In->SectionList[CurrentSection].Data1 = 0;
				Raziel.movementMinRate = 0;
				PhysicsMode = 0;
			}
			if (Ptr->Data && (PadData[0] & RazielCommands[3]))
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
			}
			break;
		case 0x10000000:
			Moving = 1;
			break;
		case 0x100000:
			StateSwitchStateCharacterData(In, &StateHandlerForcedGlide, 0);
			break;
		case 0x4010008:
			if (DeferFlag)
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x4010008, 0);
				DeferFlag = 0;
			}
			else
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerDeCompression, Moving);
			}
			In->SectionList[CurrentSection].Data2 = 2;
			PhysicsMode = 3;
			ResetPhysics(In->CharacterInstance, -16);
			break;
		case 0x4020000:
			SwimData = (struct evPhysicsSwimData*)Ptr->Data;
			if (SwimData->Depth < 0)
			{
				if (!In->CharacterInstance->zVel && In->CharacterInstance->zAccl >= 0)
				{
					ResetPhysics(In->CharacterInstance, -16);
				}
			}
			Raziel.Mode &= ~0x40000;
			razEnterWater(In, CurrentSection, (struct evPhysicsSwimData*)Ptr->Data);
			if (SwimData->WaterDepth < 0 && SwimData->WaterDepth != -32767 && Raziel.CurrentPlane == 1)
			{
				ControlFlag |= 0x2000000;
			}
			break;
		case 0x20000001:
			if (Raziel.Mode != 16 && Raziel.Mode != 32 && CurrentSection == 0)
			{
				SetDropPhysics(In->CharacterInstance, &Raziel);
			}
			break;
		case 0x80000000:
			break;
		case 0x80000001:
			if (Raziel.Senses.heldClass != 3 && !(ControlFlag & 0x2000000) && CurrentSection == 0)
			{
				StateSwitchStateCharacterData(In, &StateHandlerGlide, 3);
			}
			break;
		case 0x2000000:
			razPickupAndGrab(In, CurrentSection);
			break;
		case 0x40005:
			if (Raziel.HitPoints <= 99999)
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerStumble, 0);
			}
			break;
		case 0x1000001:
			break;
		case 0x80000020:
			break;
		case 0x80000008:
			break;
		case 0x4000001:
			break;
		case 0x8000000:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


void StateHandlerSlide(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.71%
{
	struct __Event* Ptr;

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x100001:
				if (CurrentSection == 0)
				{
					Raziel.Mode = 0x400000;
					ControlFlag = 0x509;
					PhysicsMode = 0;
					ResetPhysics(In->CharacterInstance, -16);
					Raziel.soundHandle = SOUND_Play3dSound(&In->CharacterInstance->position, 30, 0, 60, 3500);
				}
				break;
			case 0x100004:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
					Raziel.soundHandle = 0;
				}
				break;
			case 0x100000:
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				break;
			case 0x4010008:
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				break;
			case 0x4010200:
				break;
			case 0x20000001:
				break;
			case 0x80000000:
			case 0x80000001:
				break;
			case 0x80000008:
				break;
			case 0x80000020:
				break;
			default:
				DefaultStateHandler(In, CurrentSection, Data);
				break;
			}
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


void StateHandlerBlock(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;
	int Anim;

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	if (CurrentSection == 0)
	{
		BlockCount += 1;
	}

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				Raziel.Mode = 0x800000;

				ControlFlag = 0x8009;

				PhysicsMode = 0x3;

				SteerSwitchMode(In->CharacterInstance, 0);

				G2EmulationSwitchAnimationCharacter(In, 81, 0, 10, 1);
			}

			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x100004:
			COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);

			BlockCount = 0;

			FX_EndPassthruFX(In->CharacterInstance);
			break;
		case 0x8000000:
			if (CurrentSection == 0)
			{
				if ((Anim == 82) || (Anim == 108))
				{
					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

					if (Anim == 108)
					{
						razSetPlayerEventHistory(0x10000);
					}
				}
				else if (In->SectionList[CurrentSection].Data1 != 0)
				{
					struct _Rotation rot;

					G2EmulationSwitchAnimationCharacter(In, 108, 0, 5, 1);

					COLLIDE_SegmentCollisionOff(In->CharacterInstance, 1);

					razGetRotFromNormal(&Raziel.Senses.ForwardNormal, &rot);

					In->CharacterInstance->rotation.z = rot.z;
				}
				else
				{
					G2EmulationSwitchAnimationCharacter(In, 82, 0, 3, 1);

					Raziel.alarmTable = 1600;

					In->CharacterInstance->anim.section->swAlarmTable = &Raziel.alarmTable;
				}
			}

			break;
		case 0x8000004:
			if ((Raziel.Senses.EngagedMask & 1))
			{
				razGetForwardNormal(In->CharacterInstance, Raziel.Senses.EngagedList->instance);

				SetupReaction(In->CharacterInstance, Raziel.Senses.EngagedList->instance);

				INSTANCE_Post(Raziel.Senses.EngagedList->instance, 0x800000, SetObjectData(-Raziel.Senses.ForwardNormal.x, -Raziel.Senses.ForwardNormal.y, 6, NULL, 0));
			}

			break;
		case 0x0:
			if ((CurrentSection == 0) && (Anim != 108))
			{
				StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
			}

			break;
		case 0x4000001:
			break;
		case 0x4010200:
			break;
		case 0x4010400:
		{
			struct evPhysicsEdgeData* EdgeData;

			EdgeData = (struct evPhysicsEdgeData*)Ptr->Data;

			if (((EdgeData->rc & 0x20000)) && ((Raziel.Abilities & 1)) && (Raziel.CurrentPlane == 2))
			{
				SVECTOR startVec;
				SVECTOR endVec;

				PHYSICS_GenericLineCheckSetup(0, 0, 0, &startVec);

				PHYSICS_GenericLineCheckSetup(0, -320, 0, &endVec);

				if ((PHYSICS_CheckForObjectCollide(gameTrackerX.playerInstance, &startVec, &endVec, 1) == 0) && (In->SectionList[CurrentSection].Data1 == 0))
				{
					FX_StartPassthruFX(In->CharacterInstance, EdgeData->Normal1, EdgeData->Delta);

					In->SectionList[CurrentSection].Data1 = 1;
				}
			}

			break;
		}
		case 0x80000000:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
			break;
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


void StateHandlerDeCompression(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.70%
{
	struct __Event* Ptr;
	short Anim;  // not in symdump

	In->CharacterInstance->cachedTFace = -1;
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				In->SectionList[CurrentSection].Data1 = Raziel.Mode;
				ControlFlag = 281;
				Raziel.Mode |= 0x1000;
				PhysicsMode = 3;
				SteerSwitchMode(In->CharacterInstance, 0);
			}
			else
			{
				In->SectionList[CurrentSection].Data1 = In->SectionList[CurrentSection - 1].Data1;
			}
			if (CurrentSection == 0)
			{
				Anim = G2EmulationQueryAnimation(In, 0);
				if ((Raziel.Mode & 0x20) || Anim == 17 || Anim == 18 || Anim == 19 || (In->CharacterInstance->zVel < -256))
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 44, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 41, 0, 1, 1);
					}
				}
				else if ((Raziel.Mode & 0x10) == 0)
				{
					if (Ptr->Data != 0)
					{
						if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 12, NULL, NULL))
						{
							G2EmulationSwitchAnimationCharacter(In, 29, 0, 0, 1);
						}
					}
					else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 28, NULL, NULL))
					{
						G2EmulationSwitchAnimationCharacter(In, 37, 0, 1, 1);
					}
				}
				else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 28, NULL, NULL))
				{
					G2EmulationSwitchAnimationCharacter(In, 37, 0, 1, 1);
				}
			}
			break;
		case 0x100004:
			SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
			break;
		case 0x8000000:
			if ((PadData[0] & RazielCommands[6]))
			{
				if (CurrentSection == 0)
				{
					StateSwitchStateCharacterData(In, &StateHandlerCrouch, 0);
				}
			}
			else if ((PadData[0] & 0x8000000F))
			{
				if (CurrentSection == 0)
				{
					if (G2EmulationQueryAnimation(In, 0) == 29)
					{
						StateSwitchStateCharacterData(In, &StateHandlerMove, 0);
					}
					else
					{
						StateSwitchStateCharacterData(In, &StateHandlerStartMove, 5);
					}
				}
			}
			else
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 4));
				Raziel.Mode = 1;
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80000000:
			break;
		case 0x80000020:
			break;
		case 0x10000000:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


void StateHandlerGlide(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 98.44%
{
	struct __Event* Ptr;
	int Frame;
	int Anim;
	int moving;
	struct evObjectDraftData* draft;
	struct evPhysicsSwimData* SwimData;

	moving = 0;
	Anim = G2EmulationQueryAnimation(In, CurrentSection);
	Frame = G2EmulationQueryFrame(In, CurrentSection);
	if (CurrentSection == 0)
	{
		In->SectionList[CurrentSection].Data2 &= -3;
	}
	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x100001:
				if (CurrentSection == 0)
				{
					Raziel.Mode = 0x2000;
					ControlFlag = 0x518;
					PhysicsMode = 0;
					SteerSwitchMode(In->CharacterInstance, 8);
					DeInitAlgorithmicWings(In->CharacterInstance);
					if (In->CharacterInstance->zVel < 0)
					{
						SetPhysics(In->CharacterInstance, 0, 0, 52, -24);
					}
				}
				In->SectionList[CurrentSection].Data1 = 1;
				In->SectionList[CurrentSection].Data2 = 0;
				G2EmulationSwitchAnimation(In, CurrentSection, 16, Ptr->Data, 5, 1);
				break;
			case 0x100004:
				if (CurrentSection == 0)
				{
					InitAlgorithmicWings(In->CharacterInstance);
					In->SectionList[CurrentSection].Data2 = 0;
				}
				break;
			case 0x8000000:
				if (In->SectionList[CurrentSection].Data1 != 0)
				{
					G2EmulationSwitchAnimationAlpha(In, CurrentSection, 18, 0, 5, 2, 4);
					In->SectionList[CurrentSection].Data1 = 0;
				}
				break;
			case 0x8000003:
				if (Anim == 16)
				{
					SetPhysics(In->CharacterInstance, 0, 0, 52, -24);
				}
				break;
			case 0x4010008:
				StateSwitchStateData(In, CurrentSection, &StateHandlerDeCompression, 0);
				break;
			case 0x20000001:
				if (((Frame >= 13) || (Anim != 16)) && ((PadData[0] & RazielCommands[3]) == 0))
				{
					if (CurrentSection == 0)
					{
						SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
						if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL))
						{
							G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
						}
					}
					StateSwitchStateCharacterData(In, &StateHandlerFall, 0);
				}
				else
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
				}
				break;
			case 0x20000004:
				if (CurrentSection == 0)
				{
					SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 4, 0, 52, -24);
				}
				G2EmulationSwitchAnimation(In, CurrentSection, 18, 0, 5, 2);
				In->SectionList[CurrentSection].Data1 = 0;
				break;
			case 0x80000004:
				if (CurrentSection == 1)
				{
					SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 4, 0, 24, -24);
				}
				G2EmulationSwitchAnimation(In, CurrentSection, 17, 0, 5, 2);
				In->SectionList[CurrentSection].Data1 = 0;
				if ((In->SectionList[CurrentSection].Data2 & 1) != 0)
				{
					razSetPlayerEventHistory(0x4000);
				}
				break;
			case 0x10000000:
				if (Anim != 16)
				{
					if (Raziel.Bearing > 0)
					{
						G2EmulationSwitchAnimation(In, CurrentSection, 43, 0, 6, 1);
						In->SectionList[CurrentSection].Data1 = 2;
					}
					if (Raziel.Bearing < 0)
					{
						G2EmulationSwitchAnimation(In, CurrentSection, 44, 0, 6, 1);
						In->SectionList[CurrentSection].Data1 = 2;
					}
					if (Raziel.Bearing == 0)
					{
						moving = 1;
						if (In->SectionList[CurrentSection].Data1 == 2)
						{
							G2EmulationSwitchAnimation(In, CurrentSection, 18, 0, 5, 2);
							In->SectionList[CurrentSection].Data1 = 0;
						}
					}
				}
				moving = 1;
				break;
			case 0x4000007:
				if (CurrentSection == 0)
				{
					draft = (struct evObjectDraftData*)Ptr->Data;
					if (draft->maxVelocity < In->CharacterInstance->zVel)
					{
						SetExternalForce(&ExternalForces[1], 0, 0, 0, 0, 0);  // double check array index
					}
					else
					{
						SetExternalForce(&ExternalForces[1], 0, 0, draft->force, 0, 4092);  // double check array index
					}
					In->SectionList[CurrentSection].Data2 |= 3;
					Raziel.playerEvent |= 0x4000;
				}
				break;
			case 0x4020000:
				SwimData = (struct evPhysicsSwimData*)Ptr->Data;
				if (((SwimData->WaterDepth < 0) && (SwimData->WaterDepth != -0x7FFF) && (Raziel.CurrentPlane == 1)))
				{
					if (CurrentSection == 0)
					{
						SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
					}
					G2EmulationSwitchAnimation(In, CurrentSection, 19, 0, 4, 1);
					StateSwitchStateData(In, CurrentSection, &StateHandlerFall, 0);
				}
				break;
			case 0x1000001:
				break;
			case 0x40005:
				break;
			case 0x80000008:
				break;
			case 0x80000000:
				break;
			case 0x80000020:
				break;
			case 0x04000001:
				break;
			default:
				DefaultStateHandler(In, CurrentSection, Data);
			}
			DeMessageQueue(&In->SectionList[CurrentSection].Event);
		}
	}
	if ((CurrentSection == 0) && (In->SectionList[CurrentSection].Data2 & 1) != 0)
	{
		if ((In->SectionList[CurrentSection].Data2 & 2) == 0)
		{
			if (moving != 0)
			{
				SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 10, 0, 52, -24);
			}
			else
			{
				SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 10, 0, 0, -24);
			}
			In->SectionList[CurrentSection].Data2 = 0;
		}
		else
		{
			if (moving == 0)
			{
				In->CharacterInstance->yVel = 0;
			}
			else
			{
				In->CharacterInstance->yVel = 52;
			}
		}
	}
}


void StateHandlerHang(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 100%
{
	struct __Event* Ptr;
	int Anim;

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		switch (Ptr->ID)
		{
		case 0x80000000:
		case 0x80000008:
		case 0x80000020:
		case 0x4010200:
		case 0x4000011:
		case 0x4000001:
		case 0x4010010:
		case 0x1000000:
		case 0x1000001:
			break;
		case 0x100001:
		{
			struct evControlInitHangData* data;

			data = (struct evControlInitHangData*)Ptr->Data;

			if (CurrentSection == 0)
			{
				if (Raziel.Mode & 0x40000)
				{
					CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
				}

				In->SectionList[CurrentSection].Data2 = In->CharacterInstance->attachedID;

				ControlFlag = 0x8001501;

				if (In->CharacterInstance->attachedID == 0)
				{
					ControlFlag = 0xC001501;
				}

				In->CharacterInstance->attachedID = 0;

				Raziel.Mode = 0x100;

				if (data->instance != NULL)
				{
					Raziel.attachedPlatform = data->instance;
				}

				SteerSwitchMode(In->CharacterInstance, 0);
			}

			if (Raziel.iVelocity.z < 0)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 7, 4, data->frames, 1);
			}
			else
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 7, 0, data->frames, 1);
			}

			In->SectionList[CurrentSection].Data1 = 0;

			PhysicsMode = 3;

			SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

			In->CharacterInstance->flags2 &= ~0x40;

			break;
		}
		case 0x100004:
			if (CurrentSection == 0)
			{
				Raziel.attachedPlatform = NULL;

				ControlFlag &= ~0x400;

				In->CharacterInstance->attachedID = 0;
			}

			In->CharacterInstance->flags2 |= 0x40;
			break;
		case 0x10000000:
			if ((Ptr->Data == 0x10000003) && (Anim == 6))
			{
				if (CurrentSection == 1)
				{
					SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
					{
						G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, CurrentSection);
					}

					StateSwitchStateCharacterData(In, StateHandlerFall, 0);
				}
			}
			else if ((Ptr->Data == 0x10000001) || (PadData[0] & 1))
			{
				if (In->SectionList[CurrentSection].Data1 == 0)
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 8, 0, 2, 1);

					PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
				}

				In->SectionList[CurrentSection].Data1 = 1;
			}

			break;
		case 0x8000001:
			if (In->SectionList[CurrentSection].Data1 == 1)
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
			}

			break;
		case 0x8000003:
			if (CurrentSection == 0)
			{
				ControlFlag &= ~0x400;

				if (In->SectionList[CurrentSection].Data2 != 0)
				{
					ControlFlag |= 0x4000000;
				}
			}

			break;
		case 0x8000000:
			if (In->SectionList[CurrentSection].Data1 == 1)
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);

				In->CharacterInstance->rotation.x = 0;
				In->CharacterInstance->rotation.y = 0;
			}
			else
			{
				if (CurrentSection == 2)
				{
					G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
				}
				else
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 6, 0, 3, 0);
				}
			}

			if (CurrentSection == 0)
			{
				ControlFlag &= ~0x400;
			}

			break;
		case 0x100000:
			if (PadData[0] & 0x8000000F)
			{
				StateSwitchStateData(In, CurrentSection, StateHandlerStartMove, 10);

				In->SectionList[CurrentSection].Data1 = 0;
			}
			else
			{
				StateSwitchStateData(In, CurrentSection, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

				Raziel.Mode = 1;

				In->SectionList[CurrentSection].Data1 = 0;
			}

			break;
		case 0x100014:
			if (CurrentSection == 1)
			{
				SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, CurrentSection);
				}

				StateSwitchStateCharacterData(In, StateHandlerFall, 0);
			}

			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
			break;
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerPushObject(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s2*/, int Data /*$s5*/)
void StateHandlerPushObject(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3247, offset 0x800adac4
	/* begin block 1 */
		// Start line: 3248
		// Start offset: 0x800ADAC4
		// Variables:
			struct __Event *Ptr; // $v0

		/* begin block 1.1 */
			// Start line: 3300
			// Start offset: 0x800ADD58
			// Variables:
				struct evObjectData *data; // $s0
		/* end block 1.1 */
		// End offset: 0x800ADDB0
		// End Line: 3309
	/* end block 1 */
	// End offset: 0x800ADDCC
	// End Line: 3319

	/* begin block 2 */
		// Start line: 6879
	/* end block 2 */
	// End Line: 6880

	/* begin block 3 */
		// Start line: 6882
	/* end block 3 */
	// End Line: 6883
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerBreakOff(struct __CharacterState *In /*$s3*/, int CurrentSection /*$s4*/, int Data /*$s7*/)
void StateHandlerBreakOff(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3322, offset 0x800addf0
	/* begin block 1 */
		// Start line: 3323
		// Start offset: 0x800ADDF0
		// Variables:
			struct __Event *Ptr; // $v0

		/* begin block 1.1 */
			// Start line: 3334
			// Start offset: 0x800ADEF4
			// Variables:
				struct PhysObInteractProperties *interactProp; // $s0
				struct _Instance *Inst; // $s1
				int anim; // $a2
		/* end block 1.1 */
		// End offset: 0x800AE07C
		// End Line: 3383

		/* begin block 1.2 */
			// Start line: 3397
			// Start offset: 0x800AE0D4
			// Variables:
				//struct PhysObInteractProperties *interactProp; // $v0
				int action; // $s1
				int condition; // $v0
				//struct _Instance *Inst; // $s0
		/* end block 1.2 */
		// End offset: 0x800AE17C
		// End Line: 3428
	/* end block 1 */
	// End offset: 0x800AE1C8
	// End Line: 3443

	/* begin block 2 */
		// Start line: 7040
	/* end block 2 */
	// End Line: 7041
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerPullSwitch(struct __CharacterState *In /*$s4*/, int CurrentSection /*$s5*/, int Data /*stack 8*/)
void StateHandlerPullSwitch(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3446, offset 0x800ae1f4
	/* begin block 1 */
		// Start line: 3447
		// Start offset: 0x800AE1F4
		// Variables:
			struct __Event *Ptr; // $s2
			int switchDone; // stack offset -56
			int hitPosted; // stack offset -52

		/* begin block 1.1 */
			// Start line: 3459
			// Start offset: 0x800AE32C
			// Variables:
				struct PhysObSwitchProperties *switchData; // $fp

			/* begin block 1.1.1 */
				// Start line: 3465
				// Start offset: 0x800AE35C
				// Variables:
					int switchStatus; // $s1
					int switchSuccess; // $s3
					struct _Instance *inst; // $s2
					struct PhysObSwitchProperties *switchProperties; // $s0
					int extraZ; // $s6

				/* begin block 1.1.1.1 */
					// Start line: 3478
					// Start offset: 0x800AE3B0
				/* end block 1.1.1.1 */
				// End offset: 0x800AE408
				// End Line: 3490
			/* end block 1.1.1 */
			// End offset: 0x800AE534
			// End Line: 3539
		/* end block 1.1 */
		// End offset: 0x800AE534
		// End Line: 3539

		/* begin block 1.2 */
			// Start line: 3545
			// Start offset: 0x800AE544
			// Variables:
				//int switchStatus; // $v1
				//struct _Instance *inst; // $s0
				//struct PhysObSwitchProperties *switchProperties; // $s0
				int switchClass; // $s1
		/* end block 1.2 */
		// End offset: 0x800AE6C8
		// End Line: 3586

		/* begin block 1.3 */
			// Start line: 3598
			// Start offset: 0x800AE740
			// Variables:
				struct evMonsterHitData *data; // $s0
		/* end block 1.3 */
		// End offset: 0x800AE7A8
		// End Line: 3609
	/* end block 1 */
	// End offset: 0x800AE7D4
	// End Line: 3629

	/* begin block 2 */
		// Start line: 7314
	/* end block 2 */
	// End Line: 7315
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerDragObject(struct __CharacterState *In /*$s2*/, int CurrentSection /*$s4*/, int Data /*$fp*/)
void StateHandlerDragObject(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3632, offset 0x800ae804
	/* begin block 1 */
		// Start line: 3633
		// Start offset: 0x800AE804
		// Variables:
			struct __Event *Ptr; // $s1
			struct _Instance *instance; // $s3
			int anim; // $s0
			int hitPosted; // $s7

		/* begin block 1.1 */
			// Start line: 3676
			// Start offset: 0x800AEA10
			// Variables:
				struct evPhysicsSlideData *slideData; // stack offset -48

			/* begin block 1.1.1 */
				// Start line: 3681
				// Start offset: 0x800AEA2C
				// Variables:
					struct evObjectData *data; // $s0
			/* end block 1.1.1 */
			// End offset: 0x800AED18
			// End Line: 3738
		/* end block 1.1 */
		// End offset: 0x800AED18
		// End Line: 3739

		/* begin block 1.2 */
			// Start line: 3795
			// Start offset: 0x800AEEA0
			// Variables:
				//struct evMonsterHitData *data; // $v0
		/* end block 1.2 */
		// End offset: 0x800AEEA0
		// End Line: 3796
	/* end block 1 */
	// End offset: 0x800AEEFC
	// End Line: 3810

	/* begin block 2 */
		// Start line: 7694
	/* end block 2 */
	// End Line: 7695

	/* begin block 3 */
		// Start line: 7700
	/* end block 3 */
	// End Line: 7701
					UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerPickupObject(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s0*/, int Data /*$s4*/)
void StateHandlerPickupObject(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3813, offset 0x800aef2c
	/* begin block 1 */
		// Start line: 3814
		// Start offset: 0x800AEF2C
		// Variables:
			struct __Event *Ptr; // $v0

		/* begin block 1.1 */
			// Start line: 3822
			// Start offset: 0x800AEFE4
			// Variables:
				long colorArray[1]; // stack offset -32
		/* end block 1.1 */
		// End offset: 0x800AEFE4
		// End Line: 3822
	/* end block 1 */
	// End offset: 0x800AF128
	// End Line: 3870

	/* begin block 2 */
		// Start line: 8073
	/* end block 2 */
	// End Line: 8074
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerAutoFace(struct __CharacterState *In /*$s0*/, int CurrentSection /*$s1*/, int Data /*stack 8*/)
void StateHandlerAutoFace(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3870, offset 0x800af148
	/* begin block 1 */
		// Start line: 3871
		// Start offset: 0x800AF148
		// Variables:
			struct __Event *Ptr; // $a0
			int Frames; // $s3
			int Anim; // $s2
	/* end block 1 */
	// End offset: 0x800AF858
	// End Line: 4077

	/* begin block 2 */
		// Start line: 8216
	/* end block 2 */
	// End Line: 8217
			UNIMPLEMENTED();
}

void StateHandlerGlyphs(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 98.87%
{
	struct __Event* Ptr;
	int hitPosted;
	struct evActionPlayHostAnimationData* ptr;
	struct evMonsterHitData* data;

	hitPosted = 0;
	G2EmulationQueryAnimation(In, CurrentSection);
	Raziel.invincibleTimer = 12288;
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
			if (CurrentSection == 0)
			{
				if (Ptr->Data == 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (uintptr_t)In->CharacterInstance);
				}
				Raziel.Mode |= 1;
				ControlFlag = 0x1100008;
				SteerSwitchMode(In->CharacterInstance, 16);
				razResetMotion(In->CharacterInstance);
			}
			In->SectionList[CurrentSection].Data1 = 1;
			PhysicsMode = 3;
			if (Ptr->Data != 0)
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80007, 0);
			}
			break;
		case 0x100004:
			if (CurrentSection == 0)
			{
				razReaverOn();
				if (GlyphIsGlyphOpen(Raziel.GlyphSystem) != 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (uintptr_t)In->CharacterInstance);
				}
			}
			Raziel.invincibleTimer = 0;
			break;
		case 0x8000003:
		case 0x8000000:
		case 0x100015:
			if (CurrentSection == 0)
			{
				CheckStringAnimation(In->CharacterInstance, Ptr->ID);
			}
			break;
		case 0x100000:
			if (In->SectionList[CurrentSection].Data1 == 0)
			{
				if (((Raziel.Mode & 0x40000) != 0) && (Raziel.CurrentPlane == 1))
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerSwim, 0);
					if (CurrentSection == 0)
					{
						razSetFadeEffect(4096, 0, 256);
					}
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
					if (CurrentSection == 0)
					{
						razSetFadeEffect(4096, 0, 256);
					}
				}
			}
			else
			{
				if (((Raziel.Mode & 0x40000) == 0) && (In->SectionList[CurrentSection].Data1 != 2))
				{
					StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));
					In->SectionList[CurrentSection].Data1 = 2;
				}
			}
			break;
		case 0x80000010:
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					In->SectionList[CurrentSection].Data1 = 0;
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (uintptr_t)In->CharacterInstance);
				}
				if ((Raziel.Mode & 0x40000) != 0)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerSwim, 0);
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
			break;
		case 0x80006:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 6);
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80004:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 5);
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80003:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 4);
				Raziel.effectsFlags |= 4;
				razSetupSoundRamp(In->CharacterInstance, (struct _SoundRamp*)&Raziel.soundHandle, 19, 600, 750, 60, 120, 0x32000, 3500);
				Raziel.soundTimerNext = 0x32000;
				Raziel.soundTimerData = 1;
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80005:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 3);
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80002:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 2);
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80001:
			if (CurrentSection == 0)
			{
				razPrepGlyph();
				razSwitchStringAnimation(In->CharacterInstance, 1);
				Raziel.effectsFlags |= 4;
				razSetupSoundRamp(In->CharacterInstance, (struct _SoundRamp*)&Raziel.soundHandle, 12, 0, 125, 80, 80, 0x4D000, 3500);
				Raziel.soundTimerNext = 0x4D000;
				Raziel.soundTimerData = 3;
			}
			In->SectionList[CurrentSection].Data1 = 0;
			break;
		case 0x80007:
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					razPrepGlyph();
					razSwitchStringAnimation(In->CharacterInstance, 0);
				}
				In->SectionList[CurrentSection].Data1 = 0;
				if (CurrentSection == 0)
				{
					razPlaneShift(In->CharacterInstance);
					if ((Raziel.Mode & 0x40000) != 0)
					{
						CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);
						SteerSwitchMode(In->CharacterInstance, 6);
					}
					SteerSwitchMode(In->CharacterInstance, 0);
					if ((Raziel.playerEvent & 0x2000) != 0)
					{
						razSetPlayerEventHistory(0x2000);
						HINT_KillSpecificHint(40);
					}
				}
				Raziel.invincibleTimer = 122880;
			}
			break;
		case 0x2000000:
		case 0x80000000:
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000000, 0);
					PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
				}
			}
			break;
		case 0x10000000:
			if (CurrentSection == 0)
			{
				if ((PadData[0] & 4) != 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x10000004, Ptr->Data);
				}
				if ((PadData[0] & 8) != 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x10000002, Ptr->Data);
				}
			}
			break;
		case 0x4000001:
			if ((Raziel.Mode & 0x40000) == 0)
			{
				PhysicsMode = 0;
				SetDropPhysics(In->CharacterInstance, &Raziel);
			}
			break;
		case 0x40003:
			ptr = (struct evActionPlayHostAnimationData*)Ptr->Data;
			EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, SetActionPlayHostAnimationData(ptr->instance, ptr->host, ptr->newAnim, ptr->newFrame, ptr->frames, ptr->mode));
			break;
		case 0x1000000:
			data = (struct evMonsterHitData*)Ptr->Data;
			if ((hitPosted == 0) && (Raziel.invincibleTimer == 0))
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x1000000, SetMonsterHitData(data->sender, data->lastHit, data->power, data->knockBackDistance, data->knockBackDuration));
				hitPosted = 1;
			}
			break;
		case 0x40025:
		case 0x40005:
			EnMessageQueueData(&In->SectionList[CurrentSection].Defer, Ptr->ID, Ptr->Data);
			break;
		case 0x80000008:
			break;
		case 0x80000020:
			break;
		case 0x1000001:
			break;
		case 0x4020000:
			break;
		case 0x4010200:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	In->CharacterInstance->cachedTFace = -1;
}

void DefaultStateHandler(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;
	typedef void NewProcess(struct __CharacterState* In, int CurrentSection, int Data);  // not from SYMDUMP

	if (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		switch (Ptr->ID)
		{
		case 0x8000000:
			StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
			break;
		case 0x80000000:
			if ((!(Raziel.Mode & 0x40000)) && (!(Raziel.Senses.Flags & 0x80)))
			{
				StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerAttack2, 0);
			}

			break;
		case 0x80000020:
		{
			int message;
			int messageData;

			if ((CurrentSection == 0) && (!(Raziel.Senses.Flags & 0x80)) && (!(PadData[1] & RazielCommands[2]))
				&& (StateHandlerDecodeHold(&message, &messageData) != 0))
			{
				if (message == 0x800010)
				{
					StateSwitchStateCharacterDataDefault(In, &StateHandlerThrow2, 0);
				}
				else if (message == 0x1000002)
				{
					StateSwitchStateCharacterDataDefault(In, &StateHandlerGrab, messageData);
				}
				else if (message == 0x80000)
				{
					Raziel.playerEvent |= 0x400;

					razSetPlayerEventHistory(0x400);

					razLaunchForce(In->CharacterInstance);

					StateSwitchStateCharacterDataDefault(In, &StateHandlerThrow2, 0);
				}
				else
				{
					int i;

					StateSwitchStateCharacterDataDefault(In, &StateHandlerAttack2, 0);

					for (i = 0; i < 3; i++)
					{
						EnMessageQueueData((struct __MessageQueue*)&In->SectionList[i].Event.Queue[16].ID, 0x80000020, 0);
					}
				}
			}

			break;
		}
		case 0x80000004:
			Raziel.Mode |= 0x2;
			break;
		case 0x20000004:
			Raziel.Mode = (Raziel.Mode | 0x1) & ~0x2;
			break;
		case 0x80000008:
			if (CurrentSection == 0)
			{
				StateSwitchStateCharacterDataDefault(In, &StateHandlerCrouch, 1);
			}

			break;
		case 0x4000001:
			PhysicsMode = 0;

			SetDropPhysics(In->CharacterInstance, &Raziel);

			if ((In->CharacterInstance->zVel < Raziel.fallZVelocity) && (CurrentSection == 0))
			{
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
				}

				StateSwitchStateCharacterDataDefault(In, &StateHandlerFall, 0);
			}

			break;
		case 0x4010008:
			if (PhysicsMode != 0x3)
			{
				PhysicsMode = 0x3;

				SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
			}

			break;
		// @fixme might be triggered abruptly when being out of water, perhaps StateHandlerInitSwim needs implementing first
		/*case 0x4020000:
			razEnterWater(In, CurrentSection, (struct evPhysicsSwimData*)Ptr->Data);
			break;*/
		case 0x1000001:
			StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerAutoFace, 0);
			break;
		case 0x1000000:
			if ((CurrentSection == 0) && (!(ControlFlag & 0x4000)) && (Raziel.invincibleTimer == 0))
			{
				StateSwitchStateCharacterDataDefault(In, &StateHandlerHitReaction, Ptr->Data);

				CAMERA_ForceEndLookaroundMode(&theCamera);
			}

			break;
		// @fixme sliding can be triggered abruptly, uncomment if 3D collision improves
		/*case 0x4010200:
		{
			struct evPhysicsGravityData* ptr;
			short zRot;

			ptr = (struct evPhysicsGravityData*)Ptr->Data;

			if (CurrentSection == 0)
			{
				struct _Position pos1;
				struct _Position pos2;
				short diff;  // changed SYMDUMP type (int to short)

				pos1.x = 0;
				pos1.y = 0;
				pos2.z = 0;  // bug in the original code?

				pos2.x = ptr->x;
				pos2.y = ptr->y;
				pos2.z = ptr->z;

				diff = MATH3D_AngleFromPosToPos(&pos1, &pos2);

				if (G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14) != G2FALSE)
				{
					G2Anim_DisableController(&In->CharacterInstance->anim, 1, 14);

					if (ExtraRot != 0)
					{
						In->CharacterInstance->rotation.z += ExtraRot->z;
					}

					ExtraRot = 0;
				}

				zRot = In->CharacterInstance->rotation.z;

				if (((zRot - diff) + 1023U) < 2047)
				{
					In->CharacterInstance->rotation.z = diff;

					G2EmulationSwitchAnimationCharacter(&Raziel.State, 73, 0, 6, 2);

					StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerSlide, 0);
				}
				else
				{
					In->CharacterInstance->rotation.z = diff + 2048;

					G2EmulationSwitchAnimationCharacter(&Raziel.State, 77, 0, 6, 2);

					StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerSlide, 0);
				}
			}

			break;
		}*/
		case 0x4010400:
			if (CurrentSection == 0)
			{
				if ((LastBlock + 2) < LoopCounter)
				{
					BlockCount = 0;
				}

				LastBlock = LoopCounter;

				BlockCount++;
			}

			if (BlockCount >= 16)
			{
				StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerBlock, 0);
			}

			break;
		case 0x400000:
			if (CurrentSection == 0)
			{
				struct evFXHitData* BloodData;
				struct _SVector Accel;

				memset(&Accel, 0, 8);

				BloodData = (struct evFXHitData*)Ptr->Data;

				FX_Blood2(&BloodData->location, &BloodData->velocity, &Accel, 64, 0xFF8010, 0xFF8010);
			}

			break;
		case 0x4000011:
			if (((Raziel.Abilities & 0x2)) && (Raziel.Senses.heldClass != 0x3) && (Raziel.CurrentPlane == 1) && (CurrentSection == 0))
			{
				if ((razSideMoveSpiderCheck(In->CharacterInstance, -128) == 0) && (razSideMoveSpiderCheck(In->CharacterInstance, 128)) == 0)
				{
					StateSwitchStateCharacterDataDefault(In, &StateHandlerWallGrab, 0);
				}
			}

			break;
		case 0x40000:
			if (CurrentSection == 2)
			{
				G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
			}
			else
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 23, 0, 3, 1);
			}

			break;
		case 0x100005:
			if (Ptr->Data == 1)
			{
				if (In->CharacterInstance->LinkChild != NULL)
				{
					G2EmulationSwitchAnimation(In, Ptr->Data, 50, 0, 3, 2);

					StateSwitchStateDataDefault(In, Ptr->Data, In->SectionList[CurrentSection].Process, 0);
				}
				else
				{
					G2EmulationSwitchAnimationSync(In, Ptr->Data, 0, 3);
				}
			}
			else
			{
				G2EmulationSwitchAnimationSync(In, Ptr->Data, 0, 3);
			}

			break;
		case 0x100000:
		{
			if (Ptr->Data != 0)
			{
				StateSwitchStateDataDefault(In, CurrentSection, (NewProcess*)Ptr->Data, 0);
			}

			break;
		}
		case 0x100006:
			InitAlgorithmicWings(In->CharacterInstance);
			break;
		case 0x4010010:
		{
			struct evPhysicsEdgeData* data;

			data = (struct evPhysicsEdgeData*)Ptr->Data;

			StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerHang, SetControlInitHangData(data->instance, 0, 3));

			In->CharacterInstance->rotation.z = data->zRot;
			break;
		}
		case 0x100009:
			if (Ptr->Data != 0)
			{
				if (CurrentSection == 0)
				{
					Raziel.returnState = In->SectionList[0].Process;

					if (Raziel.returnState == &StateHandlerSoulSuck)
					{
						Raziel.returnState = &StateHandlerIdle;
					}

					In->SectionList[0].Data1 = Raziel.Mode;

					Raziel.Mode = 0x80000;
				}

				StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerLookAround, 0);
				break;
			}

			if (Raziel.returnState == NULL)
			{
				Raziel.returnState = &StateHandlerIdle;
			}

			StateSwitchStateDataDefault(In, CurrentSection, Raziel.returnState, 0);
			break;
		case 0x40005:
		case 0x40025:
			StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerStumble, Ptr->Data);

			In->SectionList[CurrentSection].Data1 = Ptr->Data * 30;
			break;
		case 0x10002001:
		case 0x10002002:
			StateSwitchStateDataDefault(In, CurrentSection, &StateHandlerGlyphs, 1);
			break;
		case 0x4000004:
			break;
		}
	}
}

long RazielAnimCallback(struct _G2Anim_Type* anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void* data)
{
	struct __State* pSection; // $a0
	struct _G2AnimSection_Type* animSection; // $a2
	struct evAnimationControllerDoneData* ControllerData; // $v1
	//struct __AlarmData* data; // $s0
	struct _Instance* inst; // $a0
	int test; // $a0
	struct _SoundRamp* soundRamp; // $t0
	struct _Instance* heldInstance;
	int result;
	int v21;
	int v22;
	struct __CannedSound* v24;

	pSection = &Raziel.State.SectionList[sectionID];
	animSection = &anim->section[sectionID];

	switch (message)
	{
	case 1:
	{
		EnMessageQueueData(&pSection->Event, 0x8000000, (unsigned short)animSection->keylistID);
		return messageDataA;
	}
	case 2:
	{
		EnMessageQueueData(&pSection->Event, 0x8000001, (unsigned short)animSection->keylistID);
		return messageDataA;
	}
	case 3:
	{
		EnMessageQueueData(&pSection->Event, 0x8000003, (unsigned short)animSection->keylistID);
		return messageDataA;
	}
	case 4:
		ControllerData = (struct evAnimationControllerDoneData*)SetAnimationControllerDoneData( Raziel.State.CharacterInstance, messageDataB, messageDataA, (int)data);

		if (ControllerData->data == 2)
		{
			AlgorithmicWings(Raziel.State.CharacterInstance, ControllerData);
		}
		else if (ControllerData->data)
		{
			if (ControllerData->data == 4)
			{
				G2Anim_DisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type);
			}
		}
		else
		{
			G2Anim_InterpDisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type, 300);
		}

		return messageDataA;
	case 5:
		*(int*)animSection->swAlarmTable = 0;
		EnMessageQueueData(&pSection->Event, 134217732, 0);
		return messageDataA;
	case 6:
		if (messageDataA != 2)
		{
			if (*(short*)messageDataB == 45)
			{
				result = 0;
				if ((ControlFlag & 0x100000) != 0)
					return result;
				heldInstance = razGetHeldWeapon();
				if (!heldInstance)
					return messageDataA;
				if ((INSTANCE_Query(heldInstance, 2) & 0x20) == 0)
					return messageDataA;
				if ((INSTANCE_Query(heldInstance, 3) & 0x10000) == 0)
					return messageDataA;
			}
			INSTANCE_DefaultAnimCallback(anim, sectionID, message, messageDataA, messageDataB, Raziel.State.CharacterInstance);
			return messageDataA;
		}
		inst = razGetHeldWeapon();
		switch (*(short*)messageDataB)
		{
		case 1:
			if (inst)
				INSTANCE_Post(inst, 0x200002, *(short*)(messageDataB + 2));
			else
				EnableWristCollision(gameTrackerX.playerInstance, *(short*)(messageDataB + 2));
			return 2;
		case 2:
			if (inst)
				INSTANCE_Post(inst, 0x200003, *(short*)(messageDataB + 2));
			else
				DisableWristCollision(gameTrackerX.playerInstance, *(short*)(messageDataB + 2));
			ControlFlag |= 0x10000000u;
			return 2;
		case 3:
			razSetCowlNoDraw(0);
			ControlFlag |= 0x40u;
			return 2;
		case 4:
			razSetCowlNoDraw(1);
			ControlFlag &= ~0x40u;
			return 2;
		case 5:
			if (*(short*)(messageDataB + 2) < 32)
				Raziel.passedMask |= 1 << *(short*)(messageDataB + 2);
			else
				Raziel.passedMask = -1;
			return 2;
		case 6:
			if (*(short*)(messageDataB + 2) < 32)
				Raziel.passedMask &= ~(1 << *(short*)(messageDataB + 2));
			else
				Raziel.passedMask = 0;
			return 2;
		case 7:
			Raziel.effectsFadeSource = gameTrackerX.playerInstance->fadeValue;
			Raziel.effectsFlags |= 1u;
			Raziel.effectsFadeDest = *(short*)(messageDataB + 2);
			return 2;
		case 8:
			Raziel.effectsFadeSteps = 0;
			Raziel.effectsFlags |= 1u;
			Raziel.effectsFadeStep = *(short*)(messageDataB + 2);
			return 2;
		case 9:
			if (!inst)
				return messageDataA;
			INSTANCE_Post(inst, 2097157, *(short*)(messageDataB + 2));
			return 2;
		case 0xA:
			if (!inst)
				return messageDataA;
			INSTANCE_Post(inst, 2097158, *(short*)(messageDataB + 2));
			return 2;
		case 0xB:
			v21 = 0;
			if (*(short*)(messageDataB + 2) == 2)
			{
				v22 = 1;
			LABEL_46:
				if (Raziel.currentSoulReaver == v22)
					v21 = 1;
				goto LABEL_48;
			}
			if (*(short*)(messageDataB + 2) >= 3)
			{
				v22 = 6;
				if (*(short*)(messageDataB + 2) != 3)
					goto LABEL_48;
				goto LABEL_46;
			}
			if (*(short*)(messageDataB + 2) >= 0)
				v21 = 1;
		LABEL_48:
			result = 2;
			if (v21)
			{
				v24 = &cannedSound[5 * *(short*)(messageDataB + 2)];
				razSetupSoundRamp(
					gameTrackerX.playerInstance,
					(struct _SoundRamp*)&Raziel.soundHandle[v24],
					*((short*)v24 + 1),
					*((short*)v24 + 2),
					*((short*)v24 + 3),
					*((short*)v24 + 4),
					*((short*)v24 + 5),
					v24->time,
					v24->distance);
				if (!cannedSound[5 * *(short*)(messageDataB + 2)].bank)
				{
					Raziel.effectsFlags |= 4u;
				}
				result = 2;
				if (cannedSound[4 * *(short*)(messageDataB + 2)].bank == 1)
				{
					Raziel.effectsFlags |= 8u;
					result = 2;
				}
			}
			break;
		default:
			return messageDataA;
		}
		return result;
	default:
		return messageDataA;
	}
}

long RazielAnimCallbackDuringPause(struct _G2Anim_Type* anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void* data) // Matching - 100%
{
	struct evAnimationControllerDoneData* ControllerData;

	if (message == G2ANIM_MSG_SEGCTRLR_INTERPDONE)
	{
		ControllerData = (struct evAnimationControllerDoneData*)SetAnimationControllerDoneData(Raziel.State.CharacterInstance, messageDataB, messageDataA, (int)data);

		if (ControllerData->data == 2)
		{
			AlgorithmicWings(Raziel.State.CharacterInstance, ControllerData);
		}
		else if (ControllerData->data == 0)
		{
			G2Anim_InterpDisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type, 300);
		}
		else if (ControllerData->data == 4)
		{
			G2Anim_DisableController(&ControllerData->instance->anim, ControllerData->segment, ControllerData->type);
		}
	}

	return messageDataA;
}

unsigned long RazielQuery(struct _Instance *instance, unsigned long Query)
{
	struct _Normal* Ptr;
	unsigned long ability;
	struct PlayerSaveData data;
	struct evShadowSegmentData* shadowData;
	
	switch (Query)
	{
	case 1:
	{
		return 1;
		break;
	}
	case 9:
	{
		return WaterStatus;
		break;
	}
	case 6:
	{
		return SetPositionData(instance->position.x, instance->position.y, instance->position.z);

		break;
	}
	case 7:
	{
		if (Raziel.steeringMode == 4)
		{
			return SetPositionData(instance->rotation.x, instance->rotation.y, Raziel.steeringLockRotation);
		}
		else
		{
			return SetPositionData(instance->rotation.x, instance->rotation.y, instance->rotation.z);
		}

		break;
	}
	case 8:
	{
		if (ExtraRot != NULL)
		{
			return (unsigned long)ExtraRot;
		}
		else
		{
			return NULL;
		}
		break;
	}
	case 10:
	{
		return Raziel.Mode;

		break;
	}
	case 11:
	{
		return Raziel.CurrentPlane;
	
		break;
	}
	case 12:
	{
		if (instance->oldMatrix != NULL)
		{
			return (unsigned long)&instance->oldMatrix[15];
		}
		else
		{
			return NULL;
		}
		break;
	}
	case 13:
	{
		if (instance->oldMatrix != NULL)
		{
			return (unsigned long)&instance->oldMatrix[0];
		}
		else
		{
			return NULL;
		}

		break;
	}
	case 16:
	{
		Ptr = (struct _Normal*)CIRC_Alloc(sizeof(_Normal));

		if (Raziel.Mode == 0x40000)
		{
			Ptr->x = instance->matrix[1].m[0][2];
			Ptr->y = instance->matrix[1].m[1][2];
			Ptr->z = instance->matrix[1].m[2][2];
		}
		else
		{
			Ptr->x = -instance->matrix[0].m[0][1];
			Ptr->y = -instance->matrix[0].m[1][1];
			Ptr->z = -instance->matrix[0].m[2][1];
		}

		return (unsigned long)Ptr;

		break;
	}
	case 19:
	{
		ability = Raziel.Abilities & 0x1FC0000;

		if ((STREAM_GetLevelWithID(instance->currentStreamUnitID)->unitFlags & 0x800) || RAZIEL_OkToShift() == 0)
		{
			ability &= 0xFEFFFFFF;
		}

		if ((Raziel.Mode & 0x40000) || Raziel.CurrentPlane == 2)
		{
			ability &= 0x1000000;
		}

		return ability;

		break;
	}
	case 24:
	{
		int ret = SetControlSaveDataData(sizeof(struct PlayerSaveData), &data);

		data.abilities = Raziel.Abilities;
		data.currentPlane = Raziel.CurrentPlane;
		data.healthScale = Raziel.HealthScale;
		data.healthBalls = Raziel.HealthBalls;
		data.manaBalls = Raziel.GlyphManaBalls;
		data.manaMax = Raziel.GlyphManaMax;
		data.playerEventHistory = Raziel.playerEventHistory;

		return ret;

		break;
	}
	case 31:
	{

		return Raziel.HealthBalls;

		break;
	}
	case 32:
	{
		return Raziel.GlyphManaBalls;

		break;
	}
	case 34:
	{
		if ((Raziel.Senses.EngagedMask & 0x40))
		{
			return ((unsigned long*)Raziel.Senses.EngagedList)[12];
		}
		else
		{
			return 0;
		}

		break;
	}
	case 36:
	{
		return Raziel.Abilities;
		break;
	}
	case 38:
	{
		shadowData = (struct evShadowSegmentData*)SetShadowSegmentData(2);
		shadowData->shadowSegments[0] = 12;
		shadowData->shadowSegments[1] = 8;

		break;
	}
	case 39:
	{
		return Raziel.Senses.EngagedMask;

		break;
	}
	case 41:
	{
		return Raziel.playerEvent;

		break;
	}
	case 42:
	{
		return Raziel.playerEventHistory;

		break;
	}
	case 43:
	{
		return (Raziel.HitPoints ^ GetMaxHealth()) < 1;

		break;
	}
	case 44:
	{
		return (unsigned long)razGetHeldItem();

		break;
	}
	case 45:
	{
		return Raziel.GlyphManaMax;

		break;
	}
	case 46:
	{
		return Raziel.invincibleTimer;

		break;
	}
	}
	
	return 0;
}

void RazielPost(struct _Instance* instance, unsigned long Message, unsigned long Data) // Matching - 100%
{
	int i;
	typedef unsigned long fn(struct _G2Anim_Type*, int, enum _G2AnimCallbackMsg_Enum, long, long, struct _Instance*);

	switch (Message)
	{
	case 0x200001:
		if (!((ControlFlag & 0x40000)) && (HealthCheckForLowHealth() == 0))
		{
			UpdateEngagementList((struct evCollideInstanceStatsData*)Data, &Raziel);
		}

		break;
	case 0x200000:
		if (!(ControlFlag & 0x40000))
		{
			Raziel.Senses.EngagedMask = 0;
		}

		Raziel.Senses.Flags &= ~0x20;
		Raziel.Senses.Flags &= ~0x40;
		break;
	case 0x200004:
		if ((ControlFlag & 0x40000000))
		{
			instance->collideInfo = (struct _CollideInfo*)Data;

			((struct _CollideInfo*)Data)->offset.z = 0;

			RazielCollide(instance, &gameTrackerX);

			COLLIDE_UpdateAllTransforms(razGetHeldItem(), (SVECTOR*)&((struct _CollideInfo*)instance->collideInfo)->offset);
		}

		break;
	case 0x100007:
	{
		struct PlayerSaveData* data;

		data = (struct PlayerSaveData*)((struct evControlSaveDataData*)Data)->data;

		Raziel.Abilities = data->abilities;

		debugRazielFlags1 = Raziel.Abilities;

		if ((razInBaseArea("under", 5)) != 0)
		{
			Raziel.CurrentPlane = 2;
		}
		else
		{
			Raziel.CurrentPlane = data->currentPlane;
		}

		Raziel.HealthScale = (short)data->healthScale;
		Raziel.HealthBalls = (short)data->healthBalls;

		HUD_Setup_Chit_Count(data->healthBalls);

		Raziel.GlyphManaBalls = data->manaBalls;
		Raziel.GlyphManaMax = data->manaMax;

		Raziel.soulReaver = NULL;

		if ((Raziel.Abilities & 0x8))
		{
			debugRazielFlags2 = 0;
		}

		Raziel.playerEventHistory = data->playerEventHistory;

		if (!(Raziel.playerEventHistory & 0x1000))
		{
			Raziel.HitPoints = 100;
		}
		else
		{
			Raziel.HitPoints = GetMaxHealth();
		}

		break;
	}
	case 0x4000005:
		Raziel.slipSlope = Data;
		break;
	case 0x4000006:
		Raziel.slipSlope = 2896;
		break;
	case 0x4000001:
		if (((ControlFlag & 8)) && (!(Raziel.Senses.Flags & 2)))
		{
			for (i = 0; i < 3; i++)
			{
				EnMessageQueueData(&Raziel.State.SectionList[i].Defer, Message, Data);
			}
		}

		break;
	case 0x40001:
		instance->currentStreamUnitID = Data;
		instance->tface = NULL;

		Raziel.GlyphSystem->currentStreamUnitID = Data;
		Raziel.GlyphSystem->tface = NULL;

		if (Raziel.soulReaver != NULL)
		{
			Raziel.soulReaver->currentStreamUnitID = Data;
			Raziel.soulReaver->tface = NULL;
		}

		break;
	case 0x800024:
		Raziel.idleInstance = ((struct evObjectIdleData*)Data)->instance;
		break;
	case 0x40006:
		DrainHealth(Data);
		break;
	case 0x40008:
		DrainMana(Data);
		break;
	case 0x40019:
		SetMana(Data);
		break;
	case 0x40004:
		G2EmulationSwitchAnimationCharacter(&Raziel.State, 128, 0, 3, 1);

		StateSwitchStateCharacterData(&Raziel.State, &StateHandlerCannedReaction, 0);
		break;
	case 0x100008:
		if ((ControlFlag & 0x200000))
		{
			RelocateConstrict((struct _SVector*)Data);
		}

		Raziel.puppetMoveToPoint.x += ((struct _SVector*)Data)->x;
		Raziel.puppetMoveToPoint.y += ((struct _SVector*)Data)->y;
		Raziel.puppetMoveToPoint.z += ((struct _SVector*)Data)->z;

		STREAM_MORPH_Relocate();
		break;
	case 0x10000A:
		if (Data != 0)
		{
			struct _G2AnimSection_Type* animSection;

			for (i = 0; i < 3; i++)
			{
				animSection = &instance->anim.section[i];

				animSection->callback = (fn*)&RazielAnimCallbackDuringPause;
				animSection->callbackData = NULL;
			}

			DeInitAlgorithmicWings(instance);

			razResetPauseTranslation(instance);
		}
		else
		{
			struct _G2AnimSection_Type* animSection;

			for (i = 0; i < 3; i++)
			{
				animSection = &instance->anim.section[i];

				animSection->callback = (fn*)&RazielAnimCallback;
				animSection->callbackData = NULL;
			}

			InitAlgorithmicWings(instance);
		}

		break;
	case 0x100010:
		if (Data != 0)
		{
			if (!(Raziel.Mode & 0x40000000))
			{
				Raziel.Mode = 0x40000000;

				ResetPhysics(instance, -16);

				for (i = 0; i < 3; i++)
				{
					StateSwitchStateData(&Raziel.State, i, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
		}
		else if ((Raziel.Mode & 0x40000000))
		{
			Raziel.Mode &= ~0x40000000;
		}

		break;
	case 0x40011:
		HealthInstantDeath(instance);
		break;
	case 0x40012:
		StateSwitchStateCharacterData(&Raziel.State, &StateHandlerDropAction, 0);
		break;
	case 0x10002000:
		razPlaneShift(instance);
		break;
	case 0x40015:
		Raziel.Abilities = Raziel.Abilities | Data;

		debugRazielFlags1 = Raziel.Abilities;

		RAZIEL_DebugHealthFillUp();

		if ((Data & 0x3FC00) && (Raziel.soulReaver != NULL))
		{
			razReaverOn();

			razReaverImbue(razGetReaverFromMask(Data));
		}

		break;
	case 0x100011:
		EnMessageQueueData(&Raziel.State.SectionList[0].Defer, 0x100011, Data);
		break;
	case 0x100012:
		Raziel.Senses.heldClass = INSTANCE_Query((struct _Instance*)Data, 4);

		if (Raziel.Senses.heldClass != 0x8)
		{
			razReaverBladeOff();
		}

		Raziel.Mode &= ~0x800;

		Raziel.Senses.Flags &= ~0x80;
		break;
	case 0x100013:
		if (Data != (int)Raziel.soulReaver)
		{
			if (razReaverOn() == 0)
			{
				Raziel.Senses.heldClass = 0;
			}

			razReaverBladeOn();
		}

		Raziel.Senses.Flags &= ~0x80;
		break;
	case 0x40022:
		Raziel.forcedGlideSpeed = Data;
		break;
	case 0x40024:
		if (SndTypeIsPlayingOrRequested(1) == 0)
		{
			SOUND_Play3dSound(&gameTrackerX.playerInstance->position, 1, 0, 75, 3500);
		}

		break;
	case 0x4000E:
		if (Data != 0)
		{
			int i;
			struct _Instance* heldWeapon;

			Raziel.returnState = StateHandlerPuppetShow;

			StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerPuppetShow, 0);

			InitAlgorithmicWings(instance);

			for (i = 0; i < 3; i++)
			{
				PurgeMessageQueue(&Raziel.State.SectionList[i].Event);

				PurgeMessageQueue(&Raziel.State.SectionList[i].Defer);
			}

			GAMELOOP_Reset24FPS();

			heldWeapon = razGetHeldWeapon();

			if ((heldWeapon != NULL) && (heldWeapon != Raziel.soulReaver))
			{
				razSetFadeEffect(0, 4096, 10);
			}
		}
		else
		{
			struct _Instance* heldWeapon;

			Raziel.Senses.Flags &= ~0x10;

			ControlFlag &= ~0x20000;

			StateSwitchStateCharacterDataDefault(&Raziel.State, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

			for (i = 0; i < 3; i++)
			{
				PurgeMessageQueue(&Raziel.State.SectionList[i].Event);

				PurgeMessageQueue(&Raziel.State.SectionList[i].Defer);
			}

			GAMELOOP_Set24FPS();

			heldWeapon = razGetHeldWeapon();

			if ((heldWeapon != NULL) && (heldWeapon != Raziel.soulReaver))
			{
				razSetFadeEffect(4096, 0, 10);
			}
		}

		break;
	case 0x100016:
		razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 51, -200, -200, 120, 120, Data << 12, 3500);

		Raziel.soundTimerNext = Data << 12;
		Raziel.soundTimerData = 0;
		break;
	default:
		for (i = 0; i < 3; i++)
		{
			EnMessageQueueData(&Raziel.State.SectionList[i].Defer, Message, Data);
		}
	}
}

int SetStates(struct _Instance* instance, struct GameTracker* GT, long* controlCommand, int AnalogLength)//Matching - 92.36%
{
	int i;
	int Event;
	int Data1;
	static unsigned long LastTime;
	struct __Event* Ptr;
	struct __Event* message;

	Data1 = 0;

#if defined(PSXPC_VERSION)
	LastTime = (unsigned long)(Emulator_GetPerformanceCounter() / (Emulator_GetPerformanceFrequency() / 1000000));
#else
	LastTime = (GetRCnt(0xF2000000) & 0xFFFF) | (gameTimer << 16);
#endif


	PadData = controlCommand;

	gameTracker = GT;

	LoopCounter++;

	for(i = 0; i < 3;)
	{
		Ptr = DeMessageQueue(&Raziel.State.SectionList[i].Defer);

		if (Ptr != NULL)
		{
			EnMessageQueue(&Raziel.State.SectionList[i].Event, Ptr);
			continue;
		}

		i++;
	}

	if (instance->offset.x != 0 || instance->offset.y != 0 || instance->offset.z != 0)
	{
		INSTANCE_Post(instance, 0x4000004, LoopCounter);

		SetImpulsePhysics(instance, &Raziel);
	}

	if (Raziel.Senses.HitMonster != NULL)
	{
		INSTANCE_Post(instance, 0x2000002, SetMonsterHitData(Raziel.Senses.HitMonster, NULL, 4096, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));
		
		DisableWristCollision(instance, 1);

		DisableWristCollision(instance, 2);
	}

	if (!(gameTrackerX.playerInstance->flags & 0x100))
	{
		Event = 0;

		if ((ControlFlag & 0x800000))
		{
			PadData = &Raziel.blankPad;

			for (i = 0; i < 3; i++)
			{
				EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
			}
		}
		else
		{
			GetControllerMessages(controlCommand);

			while ((message = DeMessageQueue(&Raziel.padCommands)) != NULL)
			{
				Event = message->ID;

				if (Event == 0x80000000)
				{
					if (!(Raziel.Senses.EngagedMask & 0x681F))
					{
						if ((Raziel.Senses.EngagedMask & 0x20))
						{
							if (razGetHeldItem() == NULL)
							{
								Event = 0x2000000;
							}
						}
					}
					else
					{
						Event = 0x2000000;
					}
				}

				if (Event == 0)
				{
					continue;
				}

				for (i = 0; i < 3; i++)
				{
					EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
				}
			}

			Data1 = ProcessMovement(instance, controlCommand, GT);

			if (Data1 != 0)
			{
				if ((Data1 & 0x1000))
				{
					Event = 0x1000000;
				}
				else
				{
					Event = 0x10000000;
				}
			}
			else
			{
				Event = 0;
			}

			for (i = 0; i < 3; i++)
			{
				EnMessageQueueData(&Raziel.State.SectionList[i].Event, Event, Data1);
			}
		}
	}

	razSetPlayerEvent();

	G2EmulatePlayAnimation(&Raziel.State);

	for (i = 0; i < 3; i++)
	{
		Raziel.State.SectionList[i].Process(&Raziel.State, i, 1);
	}

	if (gameTracker->cheatMode == 1)
	{
		PhysicsMode = 3;
	}

#if !defined(EDITOR)
	ProcessPhysics(&Raziel, &Raziel.State, 0, PhysicsMode);
#endif

	if((PadData[0] & RazielCommands[7]))
	{
		Raziel.nothingCounter = 0;

		if ((Raziel.Senses.EngagedMask & 0x40))
		{
			if (!(Raziel.Mode & 0x2000000))
			{
				for (i = 0; i < 3; i++)
				{
					EnMessageQueueData(&Raziel.State.SectionList[i].Event, 0x1000001, 0);
				}
			}
		}

		ControlFlag |= 0x4;

		if ((Raziel.Senses.EngagedMask & 0x40))
		{
			Raziel.Senses.CurrentAutoFace = Raziel.Senses.EngagedList[6].instance;
		}
		else
		{
			Raziel.Senses.CurrentAutoFace = NULL;
		}
	}
	else
	{
		if (++Raziel.nothingCounter < 6)
		{
			if ((Raziel.Senses.EngagedMask & 0x40) && !(Raziel.Mode & 0x2000000))
			{
				for (i = 0; i < 3; i++)
				{
					EnMessageQueueData(&Raziel.State.SectionList[i].Event, 0x1000001, 0);
				}
			}
		}
		else
		{
			if (Raziel.nothingCounter == 6)
			{
				Raziel.Senses.LastAutoFace = NULL;
				Raziel.Senses.CurrentAutoFace = NULL;
			}
		}

		ControlFlag &= 0xFFFFFFFB;
	}
	
	AutoFaceAngle = -1;

	Raziel.iVelocity.x = instance->position.x - instance->oldPos.x - instance->offset.x;
	Raziel.iVelocity.y = instance->position.y - instance->oldPos.y - instance->offset.y;
	Raziel.iVelocity.z = instance->position.z - instance->oldPos.z - instance->offset.z;

	StateGovernState(&Raziel.State, 3);

	if ((gameTrackerX.debugFlags2 & 0x800) && !(ControlFlag & 0x1000000))
	{
		ProcessHealth(instance);
	}
	
	if ((ControlFlag & 0x20000))
	{
		if ((Raziel.Senses.EngagedMask & 0x400))
		{
			AlgorithmicNeck(instance, Raziel.Senses.EngagedList[10].instance);
		}
		else
		{
			if ((Raziel.Senses.Flags & 0x10))
			{
				AlgorithmicNeck(instance, NULL);
			}
		}
	}

	if ((ControlFlag & 0x200000) && gameTrackerX.gameFramePassed != 0)
	{
		ProcessConstrict();
	}

	if (Raziel.effectsFlags != 0)
	{
		ProcessEffects(instance);
	}

	ProcessInteractiveMusic(instance);

	ProcessSpecialAbilities(instance);
	
	if ((Raziel.Senses.Flags & 0x40) && RAZIEL_OkToShift() != 0)
	{
		if (INSTANCE_Query(Raziel.Senses.Portal, 0x11) != 1)
		{
			FX_EndInstanceParticleEffects(Raziel.Senses.Portal);

			INSTANCE_Post(Raziel.Senses.Portal, 0x8000008, SetAnimationInstanceSwitchData(Raziel.Senses.Portal, 1, 0, 0, 2));
		}
	}
	else
	{
		if (Raziel.Senses.Portal != NULL)
		{
			if (INSTANCE_Query(Raziel.Senses.Portal, 0x11) != 0)
			{
				FX_EndInstanceParticleEffects(Raziel.Senses.Portal);

				INSTANCE_Post(Raziel.Senses.Portal, 0x8000008, SetAnimationInstanceSwitchData(Raziel.Senses.Portal, 0, 0, 0, 2));
			}
		}

		Raziel.Senses.Portal = NULL;
	}

	return 1;
}


void ProcessConstrict()  // Matching - 100%
{
	if ((Raziel.constrictFlag & 1))
	{
		Raziel.constrictIndex = 0;
		Raziel.constrictFlag = (Raziel.constrictFlag & ~1) | 4;
		Raziel.constrictWaitIndex = 0;
		Raziel.constrictXTotal = 0;
		Raziel.constrictYTotal = 0;
		Raziel.constrictGoodCircle = 0;
	}

	Raziel.constrictXTotal += Raziel.State.CharacterInstance->position.x;
	Raziel.constrictYTotal += Raziel.State.CharacterInstance->position.y;

	Raziel.constrictData[Raziel.constrictIndex].x = Raziel.State.CharacterInstance->position.x;
	Raziel.constrictData[Raziel.constrictIndex].y = Raziel.State.CharacterInstance->position.y;

	Raziel.constrictIndex++;

	if (Raziel.constrictIndex >= 32)
	{
		Raziel.constrictIndex = 0;
	}

	if ((Raziel.constrictFlag & 4))
	{
		if (Raziel.constrictWaitIndex == Raziel.constrictIndex)
		{
			Raziel.constrictFlag |= 0x22;
		}
		else
		{
			Raziel.constrictFlag &= ~0x20;
		}
	}

	if ((Raziel.constrictFlag & 2))
	{
		int i;

		Raziel.constrictCenter.x = (short)(Raziel.constrictXTotal / 32);
		Raziel.constrictCenter.y = (short)(Raziel.constrictYTotal / 32);
		Raziel.constrictCenter.z = Raziel.State.CharacterInstance->position.z + 256;

		Raziel.constrictXTotal -= Raziel.constrictData[Raziel.constrictIndex].x;
		Raziel.constrictYTotal -= Raziel.constrictData[Raziel.constrictIndex].y;

		Raziel.constrictGoodCircle = 1;

		for (i = 0; i < 32; i++)
		{
			if ((MATH3D_SquareLength(Raziel.constrictData[i].x - Raziel.constrictCenter.x, Raziel.constrictData[i].y - Raziel.constrictCenter.y, 0) - 1) > 819198)
			{
				Raziel.constrictGoodCircle = 0;
			}
		}

		if (Raziel.constrictGoodCircle != 0)
		{
			int thisIndex;
			int nextIndex;

			thisIndex = Raziel.constrictIndex - 1;
			nextIndex = Raziel.constrictIndex;

			if (thisIndex < 0)
			{
				thisIndex = 31;
			}

			if (MATH3D_SquareLength(Raziel.constrictData[thisIndex].x - Raziel.constrictData[nextIndex].x, Raziel.constrictData[thisIndex].y - Raziel.constrictData[nextIndex].y, 0) > 1440000)
			{
				Raziel.constrictGoodCircle = 0;
			}

			if (Raziel.constrictGoodCircle != 0)
			{
				Raziel.constrictXTotal = 0;
				Raziel.constrictYTotal = 0;
				Raziel.constrictFlag = (Raziel.constrictFlag & 0xFFFD) | 0x1C;
				Raziel.constrictWaitIndex = Raziel.constrictIndex;

				if (Raziel.constrictGoodCircle >= 2)
				{
					gameTrackerX.streamFlags |= 4;

					FX_EndConstrict(1, NULL);

					Raziel.constrictFlag = 1;
				}

				Raziel.constrictGoodCircle++;
				return;
			}
		}

		Raziel.constrictGoodCircle = 1;

		Raziel.constrictFlag &= 0xFFEF;

		FX_EndConstrict(0, NULL);
	}
}


// autogenerated function stub: 
// void /*$ra*/ RelocateConstrict(struct _SVector *offset /*$a3*/)
void RelocateConstrict(struct _SVector *offset)
{ // line 6068, offset 0x800b285c
	/* begin block 1 */
		// Start line: 6069
		// Start offset: 0x800B285C
		// Variables:
			int i; // $a2
	/* end block 1 */
	// End offset: 0x800B29B4
	// End Line: 6097

	/* begin block 2 */
		// Start line: 12827
	/* end block 2 */
	// End Line: 12828

	/* begin block 3 */
		// Start line: 12832
	/* end block 3 */
	// End Line: 12833
			UNIMPLEMENTED();
}


void ProcessEffects(struct _Instance* instance)//Matching - 95.41%
{
	struct _Instance* heldInst;
	int step;

	heldInst = razGetHeldItem();

	if ((Raziel.effectsFlags & 0x4) && !razUpdateSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle))
	{
		SndEndLoop(Raziel.soundHandle);
		Raziel.soundHandle = 0;
		Raziel.effectsFlags &= ~0x4;
	}

	if ((Raziel.effectsFlags & 0x8) && !razUpdateSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2))
	{
		SndEndLoop(Raziel.soundHandle2);
		Raziel.soundHandle2 = 0;
		Raziel.effectsFlags &= ~0x8;
	}

	if ((Raziel.effectsFlags & 0x2))
	{
		if (Raziel.throwInstance)
		{
			instance = Raziel.throwInstance;
		}
		else if (heldInst)
		{
			if ((unsigned int)(Raziel.Senses.heldClass - 1) < 3)
			{
				instance = heldInst;
			}
			else
			{
				Raziel.effectsFlags &= ~0x2;
				Raziel.throwInstance = 0;
			}
		}
	}

	if ((Raziel.effectsFlags & 0x1))
	{
		Raziel.effectsFadeSteps += Raziel.effectsFadeStep * gameTrackerX.timeMult;
		step = Raziel.effectsFadeSteps / 4096;
		instance->fadeValue = instance->fadeValue + step;

		if (step > 0)
		{
			if (instance->fadeValue > Raziel.effectsFadeDest)
			{
				instance->fadeValue = Raziel.effectsFadeDest;
				Raziel.effectsFlags &= ~0x1;

				if (instance == Raziel.throwInstance)
				{
					Raziel.effectsFlags &= 0xFFFFFFFD;
				}
			}
		}
		else
		{
			if (step < 0)
			{
				if (Raziel.effectsFadeDest > instance->fadeValue)
				{
					instance->fadeValue = Raziel.effectsFadeDest;
					Raziel.effectsFlags &= ~0x1;

					if (instance == Raziel.throwInstance)
					{
						Raziel.effectsFlags &= 0xFFFFFFFD;
						Raziel.throwInstance = 0;

					}
				}
			}
		}
	}
}

void ProcessHints()
{
	long hint;

	hint = HINT_GetCurrentHint();

	if (!(Raziel.playerEventHistory & 0x2000))
	{
		if ((Raziel.playerEvent & 0x2000))
		{
			if (hint == -1)
			{
				HINT_StartHint(12);
			}

			if (hint == 12 && Raziel.State.SectionList[0].Process == &StateHandlerGlyphs)
			{
				HINT_KillSpecificHint(12);

				HINT_StartHint(40);
			}

			if (hint == 40 && Raziel.State.SectionList[0].Process != &StateHandlerGlyphs)
			{
				HINT_KillSpecificHint(40);

				HINT_StartHint(12);
			}
		}
		else
		{
			if (hint == 12)
			{
				HINT_KillSpecificHint(12);
			}
		}
	}
	else
	{
		if (hint == 12 || hint == 40)
		{
			HINT_KillSpecificHint(12);
		}
	}

	if (!(Raziel.playerEventHistory & 0x10000))
	{
		if ((Raziel.playerEvent & 0x10000))
		{
			if (hint == -1)
			{
				HINT_StartHint(32);
			}
		}
		else if (hint == 32)
		{
			HINT_KillSpecificHint(32);
		}
	}
}

void ProcessInteractiveMusic(struct _Instance* instance)
{
	struct Level* level;

	level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

	RAZIEL_SetInteractiveMusic(6, level->unitFlags & 0x2);

	RAZIEL_SetInteractiveMusic(9, level->unitFlags & 0x200);

	RAZIEL_SetInteractiveMusic(10, level->unitFlags & 0x100);

	RAZIEL_SetInteractiveMusic(7, level->unitFlags & 0x40);

	RAZIEL_SetInteractiveMusic(8, level->unitFlags & 0x80);

	RAZIEL_SetInteractiveMusic(11, level->unitFlags & 0x400);

	RAZIEL_SetInteractiveMusic(14, level->unitFlags & 0x4000);

	if ((level->unitFlags & 0x2) && !(level->unitFlags & 0xC0))
	{
		RAZIEL_SetInteractiveMusic(7, 0 < (GAMELOOP_GetTimeOfDay() ^ 0x76C));

		RAZIEL_SetInteractiveMusic(7, (GAMELOOP_GetTimeOfDay() < 1));
	}

	RAZIEL_SetInteractiveMusic(12, (Raziel.CurrentPlane ^ 2) < 1);

	RAZIEL_SetInteractiveMusic(5, (Raziel.Mode & 0x40000));

	if ((level->unitFlags & 0x10) || (Raziel.Mode & 0x2000000))
	{
		RAZIEL_SetInteractiveMusic(3, 1);
	}
	else if ((level->unitFlags & 0x8) || (Raziel.Senses.Flags & 0x20))
	{
		RAZIEL_SetInteractiveMusic(2, 1);
	}
	else if ((level->unitFlags & 0x4))
	{
		RAZIEL_SetInteractiveMusic(1, 1);
	}
	else if ((level->unitFlags & 0x20))
	{
		Raziel.soundModifier &= 0xFFFFFFE1;

		RAZIEL_SetInteractiveMusic(4, 1);
	}
	else
	{
		Raziel.soundModifier &= 0xFFFFFFE1;

		SOUND_SetMusicModifier(0);
	}
}

void ProcessTimers(struct _Instance* instance)  // Matching - 100%
{
	if (Raziel.timeAccumulator > 0)
	{
		Raziel.timeAccumulator -= gameTrackerX.timeMult;
		if (Raziel.timeAccumulator <= 0)
		{
			INSTANCE_Post(instance, 0x100015, -Raziel.timeAccumulator);
			Raziel.timeAccumulator = 0;
		}
	}
	if (Raziel.soundTimerNext > 0)
	{
		Raziel.soundTimerNext -= gameTrackerX.timeMult;
		if (Raziel.soundTimerNext <= 0)
		{
			Raziel.soundTimerNext = 0;
			switch (Raziel.soundTimerData)
			{
			case 1:
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 19, 1000, 1000, 120, 120, 4096, 3500);
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 20, 1030, 1030, 120, 120, 4096, 3500);
				Raziel.soundTimerNext = 122880;
				Raziel.soundTimerData = 2;
				Raziel.effectsFlags |= 8;
				break;
			case 2:
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 19, 1000, 1000, 120, 0, 245760, 3500);
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 20, 1030, 1030, 120, 0, 245760, 3500);
				Raziel.soundTimerNext = 0;
				Raziel.soundTimerData = 0;
				break;
			case 3:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
				Raziel.soundHandle = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 13, 0, 0, 100, 100, 4096, 3500);
				Raziel.soundTimerNext = 40960;
				Raziel.soundTimerData = 4;
				break;
			case 4:
				if (Raziel.soundHandle2 != 0)
				{
					SndEndLoop(Raziel.soundHandle2);
				}
				Raziel.soundHandle2 = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 13, 50, 50, 95, 95, 4096, 3500);
				Raziel.soundTimerNext = 28672;
				Raziel.soundTimerData = 5;
				Raziel.effectsFlags |= 8;
				break;
			case 5:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
				Raziel.soundHandle = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 13, -20, -20, 80, 80, 4096, 3500);
				Raziel.soundTimerNext = 32768;
				Raziel.soundTimerData = 6;
				break;
			case 6:
				if (Raziel.soundHandle2 != 0)
				{
					SndEndLoop(Raziel.soundHandle2);
				}
				Raziel.soundHandle2 = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 13, 100, 100, 75, 75, 4096, 3500);
				Raziel.soundTimerNext = 40960;
				Raziel.soundTimerData = 7;
				break;
			case 7:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
				Raziel.soundHandle = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 13, -100, -100, 65, 65, 4096, 3500);
				Raziel.soundTimerNext = 20480;
				Raziel.soundTimerData = 8;
				break;
			case 8:
				if (Raziel.soundHandle2 != 0)
				{
					SndEndLoop(Raziel.soundHandle2);
				}
				Raziel.soundHandle2 = 0;
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 13, 30, 30, 60, 60, 4096, 3500);
				Raziel.soundTimerNext = 40960;
				Raziel.soundTimerData = 9;
				break;
			case 9:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
				Raziel.soundHandle = 0;
				if (Raziel.soundHandle2 != 0)
				{
					SndEndLoop(Raziel.soundHandle2);
				}
				Raziel.soundHandle2 = 0;
				Raziel.effectsFlags &= ~4;
				Raziel.effectsFlags &= ~8;
				break;
			case 10:
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 51, -200, -200, 120, 0, 245760, 3500);
				Raziel.soundTimerNext = 0;
				Raziel.soundTimerData = 0;
				break;
			default:
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
			}
		}
	}
}

void SetTimer(int ticks)
{
	Raziel.timeAccumulator = ticks >> 12;
}


void ProcessSpecialAbilities(struct _Instance* instance) // Matching - 100%
{
	unsigned long reaver;
	unsigned long temp;
	struct Object* soulReaverOb;
	struct Level* level;

	if ((Raziel.Abilities & 0x8))
	{
		if (debugRazielFlags2 != 0)
		{
			temp = 1 << (Raziel.currentSoulReaver + 9);

			if (temp != debugRazielFlags2)
			{
				temp = debugRazielFlags2 & ~temp;
				debugRazielFlags2 = temp;
			}

			reaver = razGetReaverFromMask(temp);
		}
		else
		{
			if (Raziel.CurrentPlane == 1)
			{
				reaver = 2;
				debugRazielFlags2 = 0x800;
			}
			else
			{
				reaver = 1;
				debugRazielFlags2 = 0x400;
			}
		}

		if (Raziel.soulReaver == NULL)
		{
			soulReaverOb = (struct Object*)objectAccess[22].object;
			if (soulReaverOb != NULL)
			{
				razReaverPickup(instance, INSTANCE_BirthObject(instance, soulReaverOb, 0));
				RAZIEL_DebugHealthFillUp();
			}
		}
		else
		{
			if (Raziel.currentSoulReaver != reaver)
			{
				INSTANCE_Post(Raziel.soulReaver, 0x800103, reaver);
				Raziel.currentSoulReaver = reaver;
			}
			else
			{
				if (reaver == 6)
				{
					level = STREAM_GetLevelWithID(instance->currentStreamUnitID);

					if (instance->waterFace != NULL)
					{
						if (instance->matrix == NULL)
						{
							return;
						}

						if (instance->matrix[41].t[2] < instance->splitPoint.z)
						{
							razReaverImbue(2);
						}
					}
					else
					{
						if (instance->matrix == NULL)
						{
							return;
						}

						if (instance->matrix[41].t[2] < level->waterZLevel)
						{
							razReaverImbue(2);
						}
					}
				}
			}
		}
	}
	else
	{
		if (Raziel.soulReaver != NULL)
		{
			INSTANCE_UnlinkFromParent(Raziel.soulReaver);
			INSTANCE_KillInstance(Raziel.soulReaver);
			Raziel.soulReaver = NULL;
			Raziel.currentSoulReaver = 0;
			debugRazielFlags2 = 0;
			Raziel.Senses.heldClass = 0;
		}
	}
}


int GetControllerMessages(long* controlCommand)
{
	if ((controlCommand[1] & RazielCommands[2]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000000, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[2]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000000, 0);

		}
	}

	if ((controlCommand[1] & RazielCommands[1]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000002, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[1]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000002, 0);
		}
	}

	if ((controlCommand[1] & RazielCommands[3]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000001, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[3]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000001, 0);
		}
	}

	if ((controlCommand[1] & RazielCommands[7]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000004, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[7]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000004, 0);
		}
	}

	if ((controlCommand[1] & RazielCommands[6]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000008, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[6]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000008, 0);
		}
	}

	if ((controlCommand[1] & RazielCommands[9]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000020, 0);
	}
	else
	{
		if ((controlCommand[2] & RazielCommands[9]))
		{
			EnMessageQueueData(&Raziel.padCommands, 0x20000020, 0);
		}
	}

	if ((controlCommand[1] & RazielCommands[0]))
	{
		EnMessageQueueData(&Raziel.padCommands, 0x80000010, 0);
	}
	
	return 0;
}

void RazielAdditionalCollide(struct _Instance* instance, struct GameTracker* gameTracker)//Matching - 92.83%
{
	int rc;
	int Mode;
	short Height;
	struct evPhysicsEdgeData* Data;
	struct evPhysicsSwimData* swimData;
	struct _Instance* Inst;

	if ((ControlFlag & 0x8))
	{
		rc = 1;
	}
	else
	{
		rc = 0;
	}

	if ((ControlFlag & 0x2000))
	{
		PhysicsCheckDropOff(instance, SetPhysicsDropOffData(0, -96, Raziel.dropOffHeight, (short)Raziel.slipSlope, 256), 2);
	}

	if ((rc & 0x1))
	{
		if (gameTrackerX.cheatMode == 1)
		{
			Height = 64;
		}
		else
		{
			Height = 128;
		}

		if (Height < instance->oldPos.z - instance->position.z)
		{
			Height = instance->oldPos.z - instance->position.z;
		}

		if ((PhysicsCheckGravity(instance, SetPhysicsGravityData((short)instance->matrix[1].t[2] - (instance->matrix)->t[2], Height, 0, 0, 0, (short)Raziel.slipSlope), 7) & 0x1))
		{
			Raziel.Senses.Flags |= 0x4;
		}
		else
		{
			Raziel.Senses.Flags &= 0xFFFFFFFB;
		}

		FX_UpdateInstanceWaterSplit(instance);
	}
	else
	{
		instance->oldTFace = NULL;
		instance->tface = NULL;
		instance->tfaceLevel = NULL;
		instance->waterFace = NULL;
		instance->waterFaceTerrain = NULL;
	}

	if ((ControlFlag & 0x400))
	{
		if (Raziel.Senses.heldClass != 3)
		{
			Data = (struct evPhysicsEdgeData*)SetPhysicsEdgeData(400, -256, 144, 0, -196, 498, &Raziel.Senses.ForwardNormal, &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta);

			Mode = PhysicsCheckEdgeGrabbing(instance, gameTracker, (intptr_t)Data, 3);

			if ((Mode & 0x6) == 6)
			{
				SetPhysics(instance, 0, 0, 0, 0);

				Raziel.Senses.Flags |= 0x2;

				if (ExtraRot != NULL)
				{
					G2Anim_DisableController(&instance->anim, 1, 14);

					instance->rotation.z += ExtraRot->z;

					ExtraRot = NULL;
				}

				if (Data != NULL)
				{
					PhysicsDefaultEdgeGrabResponse(instance, Data, 0);
				}
			}
			else
			{
				Raziel.Senses.Flags &= 0xFFFFFFFD;
			}

			if ((Mode & 0x2))
			{
				Raziel.Senses.Flags |= 0x1;
			}
			else
			{
				Raziel.Senses.Flags &= 0xFFFFFFFE;
			}
		}
	}

	if ((ControlFlag & 0x100))
	{
		Inst = razGetHeldWeapon();

		swimData = (struct evPhysicsSwimData*)SetPhysicsSwimData((Raziel.Mode >> 18) & 0x1, &Raziel.iVelocity, 256, 416, 112);

		WaterStatus = PhysicsCheckSwim(instance, (intptr_t)swimData, 3);

		if ((swimData->rc & 0x10) && Inst != NULL)
		{
			if (INSTANCE_Query(Inst, 4) == 3)
			{
				G2Anim_SetSpeedAdjustment(&instance->anim, 2048);
			}
		}

		if ((swimData->rc & 0x20) && Inst != NULL)
		{
			if (INSTANCE_Query(Inst, 4) == 3)
			{
				G2Anim_SetSpeedAdjustment(&instance->anim, 4096);
			}
		}
	}
	else
	{
		WaterStatus = 32;

		FX_UpdateInstanceWaterSplit(instance);
	}

	if ((ControlFlag & 0x8000))
	{
		Mode = PhysicsCheckBlockers(instance, gameTracker, SetPhysicsEdgeData(256, -256, 80, 0, -104, 0, &Raziel.Senses.ForwardNormal, &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta), 3);

		if ((Mode & 0x4))
		{
			Raziel.Senses.Flags |= 0x2;
		}
		else
		{
			Raziel.Senses.Flags &= 0xFFFFFFFD;
		}

		if ((Mode & 0x2))
		{
			Raziel.Senses.Flags |= 0x1;
		}
		else
		{
			Raziel.Senses.Flags &= 0xFFFFFFFE;
		}
	}

	if ((ControlFlag & 0x80000))
	{
		PhysicsFollowWall(instance, gameTracker, SetPhysicsWallCrawlData(0, -448, 160, -186), 7);
	}

	if ((ControlFlag & 0x4000000))
	{
		if (Raziel.attachedPlatform != NULL)
		{
			if (INSTANCE_Query(Raziel.attachedPlatform, 2) & 0x8)
			{
				PhysicsCheckLinkedMove(instance, SetPhysicsLinkedMoveData(Raziel.attachedPlatform, 2, NULL, NULL), 5);
			}
			else
			{
				PhysicsCheckLinkedMove(instance, SetPhysicsLinkedMoveData(Raziel.attachedPlatform, 0, NULL, NULL), 5);
			}
		}
	}
}

int GetEngageEvent(struct _Instance* instance)//Matching - 91.46%
{
	int Ability;

	if (instance != NULL)
	{
		Ability = INSTANCE_Query(instance, 0x2);

		if ((Ability & 0x8) != 0)
		{
			return 0x2000000;
		}

		if ((Ability & 0x1) != 0)
		{
			return 0x2000001;
		}

		if ((Ability & 0x2) != 0)
		{
			return 0x2000004;
		}
	}

	return 0;
}

int SetupReaction(struct _Instance* player, struct _Instance* instance) // Matching - 94.42%
{
	int FaceAngle;
	int isEvent;

	player->yVel = 0;
	player->xVel = 0;
	*(struct _Instance**)&player->work3 = instance;
	isEvent = GetEngageEvent(instance) != 0x02000001;
	FaceAngle = player->position.z;
	if (isEvent)
	{
		PhysicsCheckEdgeGrabbing(player, gameTracker,
			SetPhysicsEdgeData(400, -256, 144, 0, -196, 498, &Raziel.Senses.ForwardNormal, &Raziel.Senses.AboveNormal, &Raziel.Senses.Delta),
			1);

		PhysicsDefaultEdgeGrabResponse(player,
			(struct evPhysicsEdgeData*)SetPhysicsEdgeData(0, 0, 0, 0, -141, 0, &Raziel.Senses.ForwardNormal, NULL, &Raziel.Senses.Delta),
			1);
	}
	player->position.z = FaceAngle;
	return FaceAngle;
}

int CheckHolding(struct _Instance* instance)//Matching - 100%
{
	return instance->LinkChild != NULL;
}

void DisableWristCollision(struct _Instance *instance, int Side)
{
	if ((Side & 0x1))
	{
		COLLIDE_SegmentCollisionOff(instance, 0x1F);
	}

	if ((Side & 0x2))
	{
		COLLIDE_SegmentCollisionOff(instance, 0x29);
	}
}

void EnableWristCollision(struct _Instance *instance, int Side)
{
	if ((Side & 0x1))
	{
		COLLIDE_SegmentCollisionOn(instance, 0x1F);
	}

	if ((Side & 0x2))
	{
		COLLIDE_SegmentCollisionOn(instance, 0x29);
	}
}

int GetCollisionType(struct _Instance* instance)//Matching - 89.86%
{
	struct _CollideInfo* collideInfo; // $s0
	struct _Instance* inst; // $v1

	collideInfo = (struct _CollideInfo*)instance->collideInfo;

	if (*(char*)((char*)collideInfo->prim0 + 4) == 9)
	{
		if (collideInfo->type1 != 3)
		{
			inst = (struct _Instance*)collideInfo->inst1;

			inst->flags |= 4u;
		}
		else
		{
			COLLIDE_SetBSPTreeFlag(collideInfo, 2048);
		}

		if (collideInfo->type1 != 1)
		{
			COLLIDE_SegmentCollisionOff(instance, (unsigned char)collideInfo->segment);
			if (Raziel.Senses.HitMonster)
				INSTANCE_Post(Raziel.Senses.HitMonster, 16777252, 0);
			Raziel.Senses.HitMonster = 0;
			return 1;
		}
	}
	if ((*(int*)&collideInfo->flags & 0xFFFF0000) == 16842752)
	{
		if ((ControlFlag & 0x1000) != 0)
		{
			return 1;
		}

		if (*(char*)((char*)collideInfo->prim0 + 4) == 9 && *(char*)((char*)collideInfo->prim1 + 4) == 8)
		{
			if (Raziel.Senses.HitMonster == 0)
			{
				Raziel.Senses.HitMonster = (struct _Instance*)collideInfo->inst1;
			}
			else
			{
				if (Raziel.Senses.HitMonster == (struct _Instance*)collideInfo->inst1)
					return 1;
				printf("MultiHit\n");
			}

			return 1;
		}
		else
		{
			if (collideInfo->type0 == 5 || collideInfo->type1 == 5 || collideInfo->type0 == 2 || collideInfo->type1 == 2)
			{
				if ((ControlFlag & 0x8000000) != 0)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	else
	{
		if (collideInfo->type0 == 5 || collideInfo->type1 == 5 || collideInfo->type0 == 2 || collideInfo->type1 == 2)
		{
			if ((ControlFlag & 0x8000000) != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}

	return 0;
}

void InitGlyphSystem(struct _Instance* instance)
{
	struct Object* GlyphOb;
	struct _Instance* iGlyph;
	
	GlyphOb = (struct Object*)objectAccess[20].object;
	
	if (GlyphOb != NULL)
	{
		iGlyph = INSTANCE_BirthObject(instance, GlyphOb, 0);

		if (iGlyph != NULL)
		{
			Raziel.GlyphSystem = iGlyph;
		}
	}
}

void mdRazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker, long *controlCommand)
{
	ProcessTimers(playerInstance);

	razClearPlayerEvent();

	ProcessRazControl(controlCommand);

	SetStates(playerInstance, gameTracker, controlCommand, -1);

	ProcessHints();

	CAMERA_Control(&theCamera, playerInstance);

	playerInstance->offset.x = 0;
	playerInstance->offset.y = 0;
	playerInstance->offset.z = 0;
	
	Raziel.collisionEdgeOffset.x = 0;
	Raziel.collisionEdgeOffset.y = 0;
	Raziel.collisionEdgeOffset.z = 0;

	Raziel.Senses.HitMonster = NULL;

	if (Raziel.GlyphSystem != NULL)
	{
		GlyphProcess(Raziel.GlyphSystem, gameTracker);
	}

	Raziel.Abilities = debugRazielFlags1;
	
	debugRazielFlags1 |= debugRazielFlags2;
}

void RazielProcess(struct _Instance *playerInstance, struct GameTracker *gameTracker)
{
	mdRazielProcess(gameTracker->playerInstance, gameTracker, &gameTrackerX.controlCommand[0][0]);

	DebugProcess(playerInstance , &Raziel);

	Norm.z = 0;
	Norm.y = 0;
	Norm.x = 0;
}

void RazielInit(struct _Instance *instance, struct GameTracker *gameTracker)
{ 
	instance->data = instance->object->data;
	gameTracker->playerInstance = instance;
	instance->intro = NULL;
	instance->zVel = 0;
	instance->zAccl = 0;
	instance->yVel = 0;
	instance->yAccl = 0;
	instance->work0 = 0;
	instance->work1 = 0;
	instance->offset.x = 0;
	instance->offset.y = 0;
	instance->offset.z = 0;
	instance->currentMainState = 0;
	instance->currentSubState = 0;
	instance->matrix = NULL;
	instance->maxZVel = 0;
	instance->work3 = 0;
	instance->lightGroup = 0;
	instance->flags |= 0x400;
	instance->flags &= 0xFFFFFEFF;
	instance->flags2 |= 0x84;

	OBTABLE_GetInstanceCollideFunc(instance);
	OBTABLE_GetInstanceProcessFunc(instance);
	DisableWristCollision(instance, 2);
	DisableWristCollision(instance, 1);
	InitStates(instance);
}

void RazielCollide(struct _Instance* instance, struct GameTracker* gameTracker)//Matching - 91.73%
{
	struct _CollideInfo* collideInfo; // $s2
	SVECTOR* offset; // $a1

	collideInfo = (struct _CollideInfo*)instance->collideInfo;

	if (GetCollisionType(instance) != 1)
	{
		if (collideInfo->type1 != 1)
		{
			if ((collideInfo->type0 == 4 || collideInfo->type0 == 1)
				&& (collideInfo->type1 == 3 && (((struct _TFace*)collideInfo->prim1)->attr & 8) == 0
					|| collideInfo->type1 == 5 && (((struct _TFace*)collideInfo->prim1)->face.v0 & 0x2000)
					|| collideInfo->type1 == 2))
			{
				offset = (SVECTOR*)&((struct _CollideInfo*)instance->collideInfo)->offset;

				if (*(unsigned int*)&((struct _CollideInfo*)instance->collideInfo)->flags & 0xA)
				{

					instance->offset.x += offset->vx;
					instance->offset.y += offset->vy;
					instance->offset.z += offset->vz;
					instance->position.x += offset->vx;
					instance->position.y += offset->vy;
					instance->position.z += offset->vz;

					COLLIDE_UpdateAllTransforms(instance, offset);
				}
				else
				{
					instance->position.x += offset->vx;
					instance->position.y += offset->vy;
					instance->position.z += offset->vz;
					COLLIDE_UpdateAllTransforms(instance, offset);
					Raziel.collisionEdgeOffset.x += offset->vx;
					Raziel.collisionEdgeOffset.y += offset->vy;
					Raziel.collisionEdgeOffset.z += offset->vz;
				}
			}
		}
		else
		{

			if ((*(unsigned int*)&(((struct _TFace*)collideInfo->prim1)->face.v0) & 0x2000) != 0)
			{
				offset = (SVECTOR*)&((struct _CollideInfo*)instance->collideInfo)->offset;
				offset->vz = 0;
				instance->offset.x += offset->vx;
				instance->offset.y += offset->vy;
				instance->position.x += offset->vx;
				instance->position.y += offset->vy;
				COLLIDE_UpdateAllTransforms(instance, offset);
			}
		}
	}
}

void RAZIEL_TurnHead(struct _Instance* instance, short* rotx, short* rotz, struct GameTracker* gameTracker)//Matching - 95.18%
{
	struct _Rotation rot;
	struct evActionLookAroundData data;
	short rx;

	if ((Raziel.Mode & 0x20000) && !(Raziel.throwMode & 0x4))
	{
		*rotx += (short)(gameTrackerX.controlData[0][4] / 8);
		*rotz -= (short)(gameTrackerX.controlData[0][3] / 6);

		if (Raziel.extraRot.x && *rotx)
		{
			*rotx -= Raziel.throwData->coilRot;
		}
		*rotx = Raziel.extraRot.x + *rotx - 4096;
		rot.x = *rotx;
		rot.y = 0;
		rot.z = *rotz;
		LimitRotation(&rot);
		*rotx = rot.x + 4096 - Raziel.extraRot.x;
		*rotz = rot.z;
		ThrowSetFocusPoint(instance, &rot);
		if (Raziel.extraRot.x)
		{
			CAMERA_SetLookRot(&theCamera, *rotx + Raziel.throwData->coilRot, *rotz);
		}
	}
	else
	{
		*rotx += (short)(gameTrackerX.controlData[0][4] / 4);
		*rotz -= (short)(gameTrackerX.controlData[0][3] / 3);

		rx = *rotx & 0xFFF;
		if (rx > 0x800)
		{
			rx |= 0xF000;
		}
		*rotx = rx;

		rx = *rotz & 0xFFF;
		if (rx > 0x800)
		{
			rx |= 0xF000;
		}
		*rotz = rx;

		data.minx = -768;
		data.maxx = 512;
		data.minz = -1024;
		data.rotx = rotx;
		data.rotz = rotz;
		data.maxz = 1024;
		razRotateUpperBody(instance, &data);
	}
}

void RAZIEL_SetLookAround(struct _Instance* instance)//Matching - 99%
{
	G2Anim_EnableController(&instance->anim, 17, 14);
	G2Anim_EnableController(&instance->anim, 16, 14);
	G2Anim_EnableController(&instance->anim, 14, 14);

	INSTANCE_Post(instance, 0x100009, 1);

	Raziel.throwXRot = 0;
	Raziel.throwZRot = 0;
}

void  RAZIEL_ResetLookAround(struct _Instance* instance)//Matching - 99.35%
{
	G2Anim_InterpDisableController(&instance->anim, 17, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 16, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);

	INSTANCE_Post(instance, 0x100009, 0);
}

long RAZIEL_OkToLookAround(struct _Instance* playerInstance)
{
	if ((Raziel.Senses.Flags & 0x4) && (Raziel.State.SectionList[0].Process == &StateHandlerIdle || Raziel.State.SectionList[0].Process == &StateHandlerThrow2))
	{
		return 1;
	}

	if (Raziel.State.SectionList[1].Process == &StateHandlerSwim)
	{
		return 1;
	}

	if (Raziel.State.SectionList[1].Process == &StateHandlerSwimTread)
	{
		return 1;
	}

	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ TrailWaterFX(struct _Instance *instance /*$s2*/, int Segment /*$s0*/, int Bubbles /*$s7*/, int Type /*$s6*/)
void TrailWaterFX(struct _Instance *instance, int Segment, int Bubbles, int Type)
{ // line 7714, offset 0x800b4810
	/* begin block 1 */
		// Start line: 7715
		// Start offset: 0x800B4810
		// Variables:
			struct _SVector Pos; // stack offset -88
			struct _SVector Vel; // stack offset -80
			struct _SVector Accl; // stack offset -72
			int i; // $s4
			int j; // $s1
			struct Level *level; // $s5
			//struct __BubbleParams BP; // stack offset -64
	/* end block 1 */
	// End offset: 0x800B4B64
	// End Line: 7761

	/* begin block 2 */
		// Start line: 16358
	/* end block 2 */
	// End Line: 16359
			UNIMPLEMENTED();
}




