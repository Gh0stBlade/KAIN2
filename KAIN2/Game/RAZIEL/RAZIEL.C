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

struct RazielData* PlayerData;
struct _G2AnimInterpInfo_Type razInterpInfo[3];
struct __EngagedInstance EngagedList[15];
struct __FitPoint constrictData[32];
int ControlFlag;
struct _G2SVector3_Type* ExtraRot;
int WaterStatus;
_Normal Norm;
struct __Force ExternalForces[4];
int LoopCounter;
int AutoFaceAngle;
struct __CannedSound cannedSound[4];

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

void StateHandlerIdle(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $s0
	int Anim; // $s5
	int blockForwardMotion; // $s7
	struct _Instance* heldInst; // $v0
	struct evPhysicsEdgeData* EdgeData;// $v0

	//s1 = In
	//s2 = CurrentSection
	//fp = Data

	blockForwardMotion = 0;

	ControlFlag &= 0xFFFFFFEF;

	G2EmulationQueryFrame(In, CurrentSection);

	//s3 = 1
	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	//s4 = &In->SectionList[CurrentSection];
	//a0 = &In->SectionList[CurrentSection].Event;

	//loc_800A8560
	

	//v0 = 0x2000000
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x2000000:
		{
			//loc_800A8A18
			if ((Raziel.Senses.EngagedMask & 0x1) && Raziel.Senses.heldClass != 3)
			{
				Raziel.Mode = 0x200;

				if (CurrentSection == 0)
				{
					Raziel.State.SectionList[CurrentSection].Data1 = 0;

					G2EmulationSwitchAnimationCharacter(In, 21, 0, 6, 1);

					StateSwitchStateCharacterData(In, &StateHandlerPushObject, 0);
				}
				
				//loc_800A8A74
				ControlFlag &= 0xFFFFFFFE;
			}
			else
			{
				//loc_800A8A8C
				if ((Raziel.Senses.EngagedMask & 0x8) && Raziel.Senses.heldClass != 3)
				{
					if (CurrentSection == 0)
					{
						StateSwitchStateCharacterData(In, &StateHandlerPullSwitch, 0);
					}
					//loc_800A8C88
				}
				else
				{
					//loc_800A8AC8

					if ((Raziel.Senses.EngagedMask & 0x2010))
					{
						if (CurrentSection == 0)
						{
							if (Raziel.Senses.heldClass == 3)
							{
								heldInst = razGetHeldItem();

								if (heldInst != NULL)
								{
									if ((INSTANCE_Query(heldInst, 0x2) & 0x20))
									{
										StateSwitchStateCharacterData(In, &StateHandlerBreakOff, 0);
									}
									//loc_800A8C88
								}
								//loc_800A8C88
							}
							else
							{
								//loc_800A8B28
								StateSwitchStateCharacterData(In, &StateHandlerBreakOff, 0);
							}
						}
						//loc_800A8C88
					}
					else
					{
						//loc_800A8B34
						if ((Raziel.Senses.EngagedMask & 0x800))
						{
							if (CurrentSection == 1)
							{
								razReaverPickup(In->CharacterInstance, Raziel.Senses.EngagedList[11].instance);
							}
							//loc_800A8C88
						}
						else
						{
							//loc_800A8B60
							if((Raziel.Senses.EngagedMask & 0x4000))
							{
								if (CurrentSection == 0)
								{
									StateSwitchStateCharacterData(In, &StateHandlerWarpGate, 0);
								}
							}
							else
							{
								//loc_800A8B80
								if (razPickupAndGrab(In, CurrentSection) != 0 && CurrentSection == 0 && !(Raziel.Senses.Flags & 0x80))
								{
									StateSwitchStateCharacterData(In, &StateHandlerAttack2, 0);
								}
								//loc_800A8C88
							}
						}
					}
				}
			}
	
			//loc_800A8C88
			//a0 = s6 + s1
			break;
		}
		case 0x80000002:
		{
			if (CurrentSection == 0)
			{
				if ((PadData[0] & RazielCommands[1]))
				{
					StateSwitchStateCharacterData(In, &StateHandlerSoulSuck, 0);
				}
			}

			break;
		}
		case 0x80000000:
		{
			//loc_800A8794
			if (!(Raziel.Senses.Flags & 0x80))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerAttack2,0);
			}
			break;
		}
		case 0x80000001:
		{
			if (CurrentSection == 0)
			{
				Raziel.Mode = 16;

				ControlFlag |= 0x10;

				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 16, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 34, 0, 2, 1);
				}

				StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
			}
			break;
		}
		case 0x100001:
		{
			if (CurrentSection == 0)
			{
				Raziel.Mode = 1;

				Raziel.idleCount = 0;

				ControlFlag = 0x2A109;

				PhysicsMode = 3;

				SteerSwitchMode(In->CharacterInstance, 0);

				Raziel.movementMinRate = 0;
			}
			
			StateInitIdle(In, CurrentSection, Ptr->Data);

			break;
		}
		case 0x100004:
		{
			//loc_800A86C0
			if (CurrentSection == 0)
			{
				razReaverScale(4096);

				razResetPauseTranslation(In->CharacterInstance);

				COLLIDE_SegmentCollisionOn(In->CharacterInstance, 1);
			}

			break;
		}
		case 0x10000000:
		{
			//loc_800A8838
			//v0 = PadData[0];
			//v1 = RazielCommands[7];

			//a1 = CurrentSection
			if (!(PadData[0] & RazielCommands[7]))
			{
				//v1 = Raziel.Bearing

				//s1 = In
				//s2 = CurrentSection
				//fp = Data

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

					//loc_800A88B8
					StateSwitchStateData(In, CurrentSection, &StateHandlerStartTurn, 0);

					In->SectionList[CurrentSection].Data1 = 52;

					//j loc_800A8C84
				}
				else
				{
					//loc_800A88DC
					if (Raziel.Bearing >= 513)
					{
						//a0 = In
						if (CurrentSection == 1)
						{
							//a1 = CurrentSection
							if (In->CharacterInstance->LinkChild == NULL)
							{
								G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
							}
							//loc_800A894C
						}
						else
						{
							//loc_800A8924
							G2EmulationSwitchAnimation(In, CurrentSection, 53, 0, 2, 1);
						}

						//loc_800A894C
						StateSwitchStateData(In, CurrentSection, &StateHandlerStartTurn, 0);

						In->SectionList[CurrentSection].Data1 = 51;
					}
					else
					{
						//loc_800A8968

						//a0 = s6 + s1
						if (blockForwardMotion == 0)
						{
							if (Raziel.Magnitude < 0x1000)
							{
								StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
							}
							else
							{
								//loc_800A89A0
								StateSwitchStateData(In, CurrentSection, &StateHandlerStartMove, 0);
							}
						}
						//loc_800A8C88
					}
				}
			}
			else
			{
				//loc_800A8988
				StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 3);
			}

			break;
		}
		case 0x80000010:
		{
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
			//loc_800A8C88
			break;
		}
		default:
			DefaultStateHandler(In, CurrentSection, Data);
			break;
		}

		//loc_800A8C88
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}

	//loc_800A8C98
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerLookAround(struct __CharacterState *In /*$s2*/, int CurrentSection /*$s1*/, int Data /*$a2*/)
void StateHandlerLookAround(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 902, offset 0x800a8f28
	/* begin block 1 */
		// Start line: 903
		// Start offset: 0x800A8F28
		// Variables:
			struct __Event *Ptr; // $s0

		/* begin block 1.1 */
			// Start line: 910
			// Start offset: 0x800A9018
			// Variables:
				struct _Instance *instance; // $s0
		/* end block 1.1 */
		// End offset: 0x800A905C
		// End Line: 918

		/* begin block 1.2 */
			// Start line: 925
			// Start offset: 0x800A9094
			// Variables:
				//struct _Instance *instance; // $s0
		/* end block 1.2 */
		// End offset: 0x800A9094
		// End Line: 926

		/* begin block 1.3 */
			// Start line: 938
			// Start offset: 0x800A90DC
			// Variables:
				int message; // stack offset -32
				int messageData; // stack offset -28
		/* end block 1.3 */
		// End offset: 0x800A9190
		// End Line: 956
	/* end block 1 */
	// End offset: 0x800A9254
	// End Line: 979

	/* begin block 2 */
		// Start line: 1925
	/* end block 2 */
	// End Line: 1926
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerCrouch(struct __CharacterState *In /*$s3*/, int CurrentSection /*$s4*/, int Data /*stack 8*/)
void StateHandlerCrouch(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 982, offset 0x800a9270
	/* begin block 1 */
		// Start line: 983
		// Start offset: 0x800A9270
		// Variables:
			struct __Event *Ptr; // $s1
			int Anim; // $s0
			struct _Instance *heldInst; // $s5
			int DropThisFrame; // stack offset -48

		/* begin block 1.1 */
			// Start line: 1097
			// Start offset: 0x800A96A0
			// Variables:
				struct evObjectData *data; // $s0
				int i; // $s2
		/* end block 1.1 */
		// End offset: 0x800A9788
		// End Line: 1124
	/* end block 1 */
	// End offset: 0x800A99B8
	// End Line: 1186

	/* begin block 2 */
		// Start line: 2095
	/* end block 2 */
	// End Line: 2096
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerDropAction(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s0*/, int Data /*$s4*/)
void StateHandlerDropAction(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 1189, offset 0x800a99e8
	/* begin block 1 */
		// Start line: 1190
		// Start offset: 0x800A99E8
		// Variables:
			struct __Event *Ptr; // $v0
	/* end block 1 */
	// End offset: 0x800A9C48
	// End Line: 1260

	/* begin block 2 */
		// Start line: 2528
	/* end block 2 */
	// End Line: 2529
			UNIMPLEMENTED();
}

void StateHandlerSoulSuck(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $a0
	int Anim; // $s3
	struct evPhysicsSwimData* SwimData; // $v0

	//v0 = LoopCounter
	//s1 = In
	//s0 = CurrentSection
	//s5 = Data

	if (!(LoopCounter & 0x3))
	{
		FX_MakeSoulDust(In->CharacterInstance, 16);
	}

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	//s4 = &In->SectionList[CurrentSection];
	//s2 = 1

	//loc_800A9B54
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x1000006:
		{
			//loc_800A9D8C
			break;
		}
		case 0x100001:
		{
			//loc_800A9C20
			//v0 = 2
			if (CurrentSection == 0)
			{
				ControlFlag = 9;

				//v0 = Raziel.Senses.EngagedMask

				PhysicsMode = 3;

				//a0 = In
				if ((Raziel.Senses.EngagedMask & 0x1000) && Raziel.Senses.heldClass != 3)
				{
					razAlignYRotMove(Raziel.Senses.EngagedList[12].instance, 220, &In->CharacterInstance->position, &In->CharacterInstance->rotation, 0);

					INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0x1);
				}
				//loc_800A9C90

				G2EmulationSwitchAnimationAlpha(In, CurrentSection, 78, 0, 4, 1, 4);
			}
			//loc_800A9CB4
			if (CurrentSection == 2)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, 4, 2);
			}

			//loc_800A9CD8
			if (CurrentSection == 1)
			{
				if (razGetHeldWeapon() != NULL || (Raziel.Senses.EngagedMask & 0x1000))
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 94, 0, 4, 1);
				}
				else
				{
					//loc_800A9D04
					G2EmulationSwitchAnimation(In, CurrentSection, 78, 0, 4, 1);
				}

				Raziel.Mode |= 0x10000000;
			}
			//loc_800A9FE4
			break;
		}
		case 0x80000010:
		{
			//loc_800A9FE4
			break;
		}
		case 0x100004:
		{
			//loc_800A9D40
			break;
		}
		case 0x1000001:
		{
			//loc_800A9FE4
			break;
		}
		case 0x4020000:
		{
			//loc_800A9FA8
			break;
		}
		case 0x1000009:
		{
			//loc_800A9F80
			break;
		}
		case 0x1000016:
		{
			//loc_800A9F48
			break;
		}
		case 0x8000000:
		{
			//loc_800A9E40
			if (Anim == 47)
			{
				if (Raziel.returnState)
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
				if ((PadData[0] & 0x40))
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 79, 0, 8, 1);
				}

				if (Raziel.returnState)
				{
					StateSwitchStateData(In, CurrentSection, Raziel.returnState, 0);
				}
				else
				{
					StateSwitchStateData(In, CurrentSection,  &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
			break;
		}
		case 0x20000002:
		{
			//loc_800A9D8C
			if (Anim == 79)
			{
				G2EmulationSwitchAnimation(In, CurrentSection, 47, 0, 3, 1);
				PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
			}
			else
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
			if ((Raziel.Senses.EngagedMask & 0x1000) != 0 && CurrentSection == 0)
			{
				INSTANCE_Post(Raziel.Senses.EngagedList[12].instance, 0x1000014, 0);
			}
			break;
		}
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	//loc_800A9FF4
#if 0
		

		loc_800A9C90 :
	move    $a1, $s0
		li      $a2, 0x4E  # 'N'
		move    $a3, $zero
		li      $v0, 4
		sw      $v0, 0x20 + var_10($sp)
		sw      $s2, 0x20 + var_C($sp)
		jal     sub_800720C4
		sw      $v0, 0x20 + var_8($sp)
		li      $v0, 2

		loc_800A9CB4:
	bne     $s0, $v0, loc_800A9CD8
		move    $a0, $s1
		move    $a1, $v0
		move    $a2, $zero
		move    $a3, $a2
		li      $v0, 4
		sw      $v0, 0x20 + var_10($sp)
		jal     sub_80072080
		sw      $s0, 0x20 + var_C($sp)

		loc_800A9CD8:
	bne     $s0, $s2, loc_800A9FE4
		nop
		jal     sub_800A5CBC
		nop
		bnez    $v0, loc_800A9D04
		move    $a0, $s1
		lw      $v0, -0x5F4($gp)
		nop
		andi    $v0, 0x1000
		beqz    $v0, loc_800A9D10
		li      $a1, 1

		loc_800A9D04:
	li      $a1, 1
		j       loc_800A9D14
		li      $a2, 0x5E  # '^'

		loc_800A9D10 :
		li      $a2, 0x4E  # 'N'

		loc_800A9D14 :
		move    $a3, $zero
		li      $v0, 4
		sw      $v0, 0x20 + var_10($sp)
		jal     sub_80072080
		sw      $s0, 0x20 + var_C($sp)
		lw      $v0, -0x670($gp)
		lui     $v1, 0x1000
		or $v0, $v1
		sw      $v0, -0x670($gp)
		j       loc_800A9FE4
		nop

		loc_800A9D40 :
	bne     $s0, $s2, loc_800A9FE4
		nop
		jal     sub_800A76C4
		li      $a0, 1
		li      $a0, 0xEFFFFFFF
		lw      $v0, -0x238($gp)
		li      $v1, 0xFFFFFFBF
		and $v0, $v1
		sw      $v0, -0x238($gp)
		lw      $v0, -0x670($gp)
		lw      $v1, -0x5F4($gp)
		and $v0, $a0
		andi    $v1, 0x1000
		sw      $v0, -0x670($gp)
		beqz    $v1, loc_800A9FE4
		lui     $a1, 0x100
		j       loc_800A9E24
		nop

		loc_800A9D8C :
	li      $v0, 0x4F  # 'O'
		bne     $s3, $v0, loc_800A9DC4
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, 0x2F  # '/'
		move    $a3, $zero
		li      $v0, 3
		sw      $v0, 0x20 + var_10($sp)
		jal     sub_80072080
		sw      $s2, 0x20 + var_C($sp)
		jal     sub_80070CD8
		addiu   $a0, $s4, 4
		j       loc_800A9E08
		nop

		loc_800A9DC4 :
	lw      $a2, -0x4C8($gp)
		nop
		bnez    $a2, loc_800A9DFC
		move    $a1, $s0
		move    $a0, $zero
		move    $a1, $a0
		jal     sub_8007193C
		li      $a2, 3
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, sub_800A84E0
		j       loc_800A9E00
		move    $a3, $v0

		loc_800A9DFC :
	move    $a3, $zero

		loc_800A9E00 :
	jal     sub_80072B04
		nop

		loc_800A9E08 :
	lw      $v0, -0x5F4($gp)
		nop
		andi    $v0, 0x1000
		beqz    $v0, loc_800A9FE4
		nop
		bnez    $s0, loc_800A9FE4
		lui     $a1, 0x100

		loc_800A9E24:
	lw      $v0, -0x5F8($gp)
		li      $a1, 0x1000014
		lw      $a0, 0x60($v0)
		jal     sub_80034684
		move    $a2, $zero
		j       loc_800A9FE4
		nop

		loc_800A9E40 :
	li      $v0, 0x2F  # '/'
		bne     $s3, $v0, loc_800A9E98
		li      $v0, 0x4E  # 'N'
		lw      $a2, -0x4C8($gp)
		nop
		beqz    $a2, loc_800A9E68
		move    $a0, $s1
		move    $a1, $s0
		j       loc_800A9E8C
		move    $a3, $zero

		loc_800A9E68 :
	move    $a0, $zero
		move    $a1, $a0
		jal     sub_8007193C
		li      $a2, 3
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, sub_800A84E0
		move    $a3, $v0

		loc_800A9E8C :
	jal     sub_80072B04
		nop
		li      $v0, 0x4E  # 'N'

		loc_800A9E98 :
		bne     $s3, $v0, loc_800A9EC0
		li      $v0, 0x50  # 'P'
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, 0x4F  # 'O'
		move    $a3, $zero
		sw      $zero, 0x20 + var_10($sp)
		jal     sub_80072080
		sw      $s2, 0x20 + var_C($sp)
		li      $v0, 0x50  # 'P'

		loc_800A9EC0:
	bne     $s3, $v0, loc_800A9FE4
		nop
		lw      $v0, -0x33C($gp)
		lw      $v1, -0x5B78($gp)
		lw      $v0, 0($v0)
		nop
		and $v0, $v1
		beqz    $v0, loc_800A9EF8
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, 0x4F  # 'O'
		move    $a3, $zero
		j       loc_800A9F94
		li      $v0, 8

		loc_800A9EF8:
	lw      $a2, -0x4C8($gp)
		nop
		beqz    $a2, loc_800A9F18
		move    $a1, $s0
		jal     sub_80072B04
		move    $a3, $zero
		j       loc_800A9FE4
		nop

		loc_800A9F18 :
	move    $a0, $zero
		move    $a1, $a0
		jal     sub_8007193C
		li      $a2, 3
		move    $a0, $s1
		move    $a1, $s0
		li      $a2, sub_800A84E0
		jal     sub_80072B04
		move    $a3, $v0
		j       loc_800A9FE4
		nop

		loc_800A9F48 :
	lw      $v0, -0x4174($gp)
		nop
		andi    $v0, 0x800
		beqz    $v0, loc_800A9FE4
		nop
		bne     $s0, $s2, loc_800A9FE4
		nop
		lw      $a0, 4($a0)
		jal     sub_800A4540
		nop
		jal     sub_800A7B20
		li      $a0, 0x1000
		j       loc_800A9FE4
		nop

		loc_800A9F80 :
	move    $a0, $s1
		move    $a1, $s0
		li      $a2, 0x50  # 'P'
		move    $a3, $zero
		li      $v0, 2

		loc_800A9F94 :
		sw      $v0, 0x20 + var_10($sp)
		jal     sub_80072080
		sw      $s2, 0x20 + var_C($sp)
		j       loc_800A9FE4
		nop

		loc_800A9FA8 :
	lw      $v0, 4($a0)
		nop
		lhu     $v0, 0x10($v0)
		nop
		andi    $v0, 0x40
		beqz    $v0, loc_800A9FE4
		move    $a0, $s1
		move    $a1, $s0
		jal     sub_8009EE2C
		move    $a2, $zero
		j       loc_800A9FE4
		nop

		loc_800A9FD8 :
	move    $a1, $s0

		loc_800A9FDC :
	jal     sub_800AFEE4
		move    $a2, $s5

		loc_800A9FE4 :
	jal     sub_80070C9C
		addiu   $a0, $s4, 4
		j       loc_800A9B54
		nop

		loc_800A9FF4 :
	lw      $ra, 0x20 + var_s18($sp)
		lw      $s5, 0x20 + var_s14($sp)
		lw      $s4, 0x20 + var_s10($sp)
		lw      $s3, 0x20 + var_sC($sp)
		lw      $s2, 0x20 + var_s8($sp)
		lw      $s1, 0x20 + var_s4($sp)
		lw      $s0, 0x20 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x40
#endif
}

void StateHandlerStartTurn(struct __CharacterState* In, int CurrentSection, int Data)
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
		case 0x8000000:
		case 0x100000:
		{
			//loc_800AA12C
			if (CurrentSection == 0)
			{
				if (!(PadData[0] & 0x8000000F))
				{
					StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
				else
				{
					StateSwitchStateCharacterData(In, &StateHandlerStartMove, 0);
				}
			}
			break;
		}
		case 0x80000001:
		{
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
		}
		case 0x100001:
		{
			if (CurrentSection == 0)
			{
				Raziel.Mode = 0x4000;

				ControlFlag = 0x20109;

				PhysicsMode = 3;

				SteerSwitchMode(In->CharacterInstance, 1);
			}
			break;
		}
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

void StateHandlerStartMove(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $a1
	int mode; // $a0

	//s1 = In
	//s2 = CurrentSection
	//s4 = Data
	//s3 = In + CurrentSection

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x4010080:
		{
#if 0
			if (CurrentSection == 2)
			{
				//loc_800AA804
				if (Ptr->Data != 0)
				{
					G2EmulationSetMode(In, CurrentSection, 1);
				}
				else
				{
					//loc_800AA4C0
					G2EmulationSetMode(In, CurrentSection, 0);
				}
			}
			//loc_800AA4D0

			//v1 = 0x80000000
			if ((Raziel.passedMask & 0x2))
			{
				//v0 = PadData[0];

				if ((PadData[0] & 0x8000000F))
				{
					G2EmulationSetMode(In, CurrentSection, 2);
				}
				//loc_800AA804
			}
			//loc_800AA804
#endif
			break;
		}
		case 0x100001:
		{
			//loc_800AA420
			StateInitStartMove(In, CurrentSection, Ptr->Data);

			In->SectionList[CurrentSection].Data2 = 1;

			if (CurrentSection == 0)
			{
				Raziel.Mode &= 0x200800;

				Raziel.Mode |= 0x4;

				ControlFlag = 0x2A119;

				PhysicsMode = 3;

				SteerSwitchMode(In->CharacterInstance, 2);

				Raziel.movementMinRate = 3276;

				Raziel.movementMinAnalog = 1024;

				Raziel.movementMaxAnalog = 4096;

				Raziel.passedMask = 0;

				//j loc_800AA808
			}

			break;
		}
		case 0x08000000:
		{
			//loc_800AA6D0
			StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 0);
			break;
		}
		//case 0x80000000:
		{
			//break;
		}
		default:
			//loc_800AA7FC
			//DefaultStateHandler(In, CurrentSection, Data);
			break;
		}


		DeMessageQueue(&In->SectionList[CurrentSection].Event);

	}
	//loc_800AA818
	//v0 = PadData[0];

	if ((PadData[0] & 0x8000000F) && In->SectionList[CurrentSection].Data2 != 0)
	{
		if (++In->SectionList[CurrentSection].Data2 >= 8)
		{
			G2EmulationSetMode(In, CurrentSection, 1);

			ControlFlag &= 0xFFFFDFFF;

			In->SectionList[CurrentSection].Data2 = 0;
		}
	}

	if (G2EmulationQueryFrame(In, CurrentSection) >= 11 && CurrentSection == 0 && CheckHolding(In->CharacterInstance) != 0)
	{
		StateSwitchStateData(In, 1, &StateHandlerMove, 11);

		if ((void*)In->SectionList[1].Data1 == (void*)&StateHandlerStartMove)
		{
			StateHandlerMove(In, 2, 11);
		}
	}

	if (Raziel.Magnitude != 0 && Raziel.Magnitude < 4096)
	{
		if ((Raziel.passedMask & 0x1))
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

void StateHandlerMove(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $s0
	int Anim; // $s6
	int data; // $a2

	//s1 = In
	//s2 = CurrentSection
	//s7 = Data

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

	//s5 = CurrentSection <<
	//s3 = &In[CurrentSection];
	In->SectionList[CurrentSection].Data1++;

	//s4 = 2
	//loc_800AAC18
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
		{
			In->SectionList[CurrentSection].Data2 = -1;

			StateInitMove(In, CurrentSection, Ptr->Data);

			Raziel.constrictFlag = 1;

			SteerSwitchMode(In->CharacterInstance, 2);

			In->SectionList[CurrentSection].Data1 = 0;

			Raziel.passedMask |= 0x1000;
			break;
		}
		case 0x100004:
		{
			//loc_800AAD84
			FX_EndConstrict(0, NULL);

			In->SectionList[CurrentSection].Data1 = 0;

			break;
		}
		case 0x10000000:
		{
			//loc_800AAFA8
			if (Raziel.Magnitude < 0x1000)
			{
				StateInitMove(In, CurrentSection, 3);
			}
			else
			{
				//loc_800AAFCC
				StateInitMove(In, CurrentSection, 0);
			}
			break;
		}
		case 0:
		{//loc_800AAE54
			if (CurrentSection == 0)
			{
				if (Raziel.steeringMode == 9 || Raziel.steeringMode == 14 || Raziel.steeringMode == 15)
				{
					razApplyMotion(In, CurrentSection);
				}
				else
				{
					printf("%x\n", PadData[0]);
					//loc_800AAE90
					if (Raziel.Mode == 2 || Anim == 123 || Anim == 124)
					{
						StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
					}
					else if (Raziel.Mode == 0x1000000)
					{
						StateSwitchStateCharacterData(In, &StateHandlerCrouch, 0);
					}
					else if (Ptr->Data < 4 && (ControlFlag & 0x800000))
					{
						ControlFlag |= 0x2000;

						if (!(PadData[0] & 0x8000000F))
						{
							EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0, Ptr->Data + 1);

							razApplyMotion(In, 0);
						}
					}
					else
					{
						//loc_800AAF60
						data = 0;

						if ((Raziel.passedMask & 0x2000))
						{
							data = 30;
						}
						else if ((Raziel.passedMask & 0x1000))
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
		}
		case 0x08000000:
			break;
		case 0x08000003:
			break;
		default:
		{
			break;
		}
		}
		//loc_800AB21C
		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}

	//loc_800AB22C
	if(CurrentSection == 0 && In->SectionList[CurrentSection].Process != &StateHandlerMove)
	{
		razResetMotion(In->CharacterInstance);
	}
}

void StateHandlerStopMove(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $a1
	
	//s0 = In
	//s1 = CurrentSection
	//s4 = Data

	//v0 = CurrentSection + 8
	//s2 = &In->SectionList[CurrentSection]
	//s3 = -1

	//loc_800AB2C4
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100004:
		{
			break;
		}
		case 0x8000000:
		{
			//800AB4E4
			StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 5));
			break;
		}
		case 0x100001:
		{
			//loc_800AB380
			if (CurrentSection == 0)
			{
				Raziel.Mode = 4;

				if ((ControlFlag & 0x800000))
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
			//loc_800AB3C4
			//v0 = 0x1E
			if (Ptr->Data == 60)
			{
				if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 88, -1, -1) != 0)
				{
					razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 2, -1, -1);
				}
			}
			else if(Ptr->Data == 30)
			{
				//loc_800AB3FC
				if (razSwitchVAnimGroup(In->CharacterInstance, CurrentSection, 92, -1, -1) != 0)
				{
					razSwitchVAnimSingle(In->CharacterInstance, CurrentSection, 3, -1, -1);
				}
			}
			else
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 6));
			}
			//loc_800AB57C

			break;
		}
		case 0x10000000:
		{
			StateSwitchStateData(In, CurrentSection, &StateHandlerStartMove, 0);
			break;
		}
		case 0:
			break;
		default:
		{
			printf("STATE STOP MOVE: %x\n", Ptr->ID);
			break;
		}

		}
		DeMessageQueue(&In->SectionList[CurrentSection].Event);

	}
	//loc_800AB58C
#if 0
		lui     $v0, 0x200
		lw      $v1, 0($a1)
		nop
		beq     $v1, $v0, loc_800AB4CC
		slt     $v0, $v1
		bnez    $v0, loc_800AB33C
		lui     $v0, 0x800
		li      $v0, unk_80000002
		beq     $v1, $v0, loc_800AB4CC
		slt     $v0, $v1
		bnez    $v0, loc_800AB31C
		lui     $v0, 0x8000
		li      $v0, unk_80000001
		beq     $v1, $v0, loc_800AB518
		move    $a0, $s0
		j       loc_800AB574
		move    $a1, $s1

		loc_800AB31C :
	li      $v0, unk_80000010
		beq     $v1, $v0, loc_800AB4CC
		lui     $v0, 0x10
		li      $v0, 0x100001
		beq     $v1, $v0, loc_800AB380
		move    $a0, $s0
		j       loc_800AB574
		move    $a1, $s1

		loc_800AB33C :
	li      $v0, 0x8000001
		slt     $v0, $v1
		bnez    $v0, loc_800AB370
		lui     $v0, 0x1000
		lui     $v0, 0x800
		slt     $v0, $v1, $v0
		beqz    $v0, loc_800AB4E4
		lui     $v0, 0x401
		li      $v0, 0x4010080
		beq     $v1, $v0, loc_800AB468
		move    $a0, $s0
		j       loc_800AB574
		move    $a1, $s1

		loc_800AB370 :
	beq     $v1, $v0, loc_800AB44C
		move    $a0, $s0
		j       loc_800AB574
		move    $a1, $s1


		loc_800AB3FC:
	bne     $a1, $v0, loc_800AB440
		move    $a0, $zero
		move    $a1, $s1
		li      $a2, 0x5C  # '\'
		sw      $s3, 0x18 + var_8($sp)
		lw      $a0, 0($s0)
		jal     sub_800A7178
		li      $a3, 0xFFFFFFFF
		beqz    $v0, loc_800AB57C
		move    $a1, $s1
		li      $a2, 3

		loc_800AB428:
	sw      $s3, 0x18 + var_8($sp)
		lw      $a0, 0($s0)
		jal     sub_800A728C
		li      $a3, 0xFFFFFFFF
		j       loc_800AB57C
		nop

		loc_800AB440 :
	move    $a1, $a0
		j       loc_800AB4F0
		li      $a2, 6

		loc_800AB44C :
		move    $a1, $s1
		li      $a2, sub_800AA2C4
		jal     sub_80072B04
		move    $a3, $zero
		j       loc_800AB57C
		nop

		loc_800AB468 :
	bnez    $s1, loc_800AB57C
		nop
		lw      $v0, 4($a1)
		nop
		bnez    $v0, loc_800AB4B8
		nop
		lw      $a0, 0($s0)
		jal     sub_800A6380
		nop
		move    $a0, $zero
		move    $a1, $a0
		jal     sub_8007193C
		li      $a2, 5
		move    $a0, $s0
		li      $a1, sub_800A84E0
		jal     sub_80072BD0
		move    $a2, $v0
		j       loc_800AB57C
		nop

		loc_800AB4B8 :
	lw      $a0, 0($s0)
		jal     sub_800A63F4
		nop
		j       loc_800AB57C
		nop

		loc_800AB4CC :
	addiu   $a0, $s2, 0x8C
		lw      $a1, 0($a1)
		jal     sub_80070D38
		move    $a2, $zero
		j       loc_800AB57C
		nop

		loc_800AB4E4 :
	move    $a0, $zero
		move    $a1, $a0
		li      $a2, 5

		loc_800AB4F0 :
		jal     sub_8007193C
		nop
		move    $a0, $s0
		move    $a1, $s1
		li      $a2, sub_800A84E0
		jal     sub_80072B04
		move    $a3, $v0
		j       loc_800AB57C
		nop

		loc_800AB518 :
	bnez    $s1, loc_800AB57C
		move    $a1, $zero
		move    $a2, $a1
		li      $v0, 0x10
		sw      $v0, -0x670($gp)
		lw      $a0, 0($s0)
		jal     sub_800A70BC
		move    $a3, $a1
		beqz    $v0, loc_800AB558
		li      $v0, 1
		sw      $v0, 0x18 + var_8($sp)
		move    $a0, $s0
		li      $a1, 0x1A
		move    $a2, $zero
		jal     sub_800723F0
		move    $a3, $a2

		loc_800AB558 :
	move    $a0, $s0
		li      $a1, sub_800AB5AC
		jal     sub_80072BD0
		move    $a2, $zero
		j       loc_800AB57C
		nop

		loc_800AB574 :
	jal     sub_800AFEE4
		move    $a2, $s4

		loc_800AB57C :
	jal     sub_80070C9C
		addiu   $a0, $s2, 4
		j       loc_800AB2C4
		nop

		loc_800AB58C :
	lw      $ra, 0x18 + var_s14($sp)
		lw      $s4, 0x18 + var_s10($sp)
		lw      $s3, 0x18 + var_sC($sp)
		lw      $s2, 0x18 + var_s8($sp)
		lw      $s1, 0x18 + var_s4($sp)
		lw      $s0, 0x18 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x30
#endif
}


void StateHandlerCompression(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x8000000:
		{
			if (Raziel.Mode == 8)
			{
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 4, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 27, 0, 0, 1);
					}

					SetPhysics(In->CharacterInstance, -16, 0, 83, 154);
				}
			}
			else if (Raziel.Mode == 32)
			{
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 36, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 39, 0, 0, 1);
					}

					SetPhysics(In->CharacterInstance, -16, 0, 21, 195);
					
					In->CharacterInstance->yVel = 0;
				}
			}
			else
			{
				if (CurrentSection == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 20, 0, 0))
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
		}
		case 0x20000001:
		{
			if (CurrentSection == 0)
			{
				if (Raziel.Mode == 16)
				{
					In->SectionList[CurrentSection].Data1 = 1;
				}
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
			}
			break;
		}
		case 0x8000001:
		{
			EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
			break;
		}
		case 0x100001:
		{
			if (CurrentSection == 0)
			{
				ControlFlag = 273;

				SetExternalForce(&ExternalForces[2], 0, 0, 0, 1, 0);
				
				In->SectionList[0].Data1 = 0;
				
				PhysicsMode = 3;
				
				Raziel.movementMinRate = 0;

				if (Raziel.Mode == 16)
				{
					SteerSwitchMode(In->CharacterInstance, 4);
					
					Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;
					
					In->CharacterInstance->yVel = 40;

					if (!G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14))
					{
						G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
					}
				}
				else if (Raziel.Mode >= 17 && Raziel.Mode == 32)
				{
					SteerSwitchMode(In->CharacterInstance, 4);

					Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;

					In->CharacterInstance->yVel = 21;

					if (!G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14))
					{
						G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
					}
				}
				else
				{
					if (Raziel.Mode >= 17 && Raziel.Mode == 32)
					{
						SteerSwitchMode(In->CharacterInstance, 4);

						Raziel.steeringLockRotation = In->CharacterInstance->rotation.z;

						In->CharacterInstance->yVel = 21;

						if (!G2Anim_IsControllerActive(&In->CharacterInstance->anim, 1, 14))
						{
							G2Anim_EnableController(&In->CharacterInstance->anim, 1, 14);
						}
					}
				}
			}
			break;
		}
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerJump(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x8000000:
		{
			if (CurrentSection == 0)
			{
				if (Raziel.Mode == 16)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
					}
				}
				else if (Raziel.Mode == 32)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 40, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 40, 0, 10, 1);
					}
				}
				else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, 0, 0))
				{
					G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);
				}
			}
			StateSwitchStateData(In, 0, &StateHandlerFall, 0);

			if (!(PadData[0] & 0x80))
			{
				In->SectionList[CurrentSection].Data2 = 1;
			}

			break;
		}
		case 0x4020000:
		{
			break;
		}		
		case 0x4010008:
		{
			break;
		}
		case 0x2000000:
		{
			razPickupAndGrab(In, CurrentSection);

			break;
		}
		case 0x8000004:
		{
			ControlFlag |= 0x8;
			break;
		}
		case 0x20000001:
		{
			In->SectionList[CurrentSection].Data2 = 1;

			if (CurrentSection == 0)
			{
				if (Raziel.Mode != 16 && Raziel.Mode != 32)
				{
					if (In->SectionList[CurrentSection].Data1 != 0 || (In->SectionList[CurrentSection].Data1 = G2EmulationQueryFrame(In, CurrentSection) + 4) != 0 && In->SectionList[CurrentSection].Data1 < G2EmulationQueryFrame(In, CurrentSection))
					{
						SetDropPhysics(In->CharacterInstance, &Raziel);

						if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 8, NULL, NULL) != 0)
						{
							G2EmulationSwitchAnimationCharacter(In, 28, 0, 7, 1);

							StateSwitchStateCharacterData(In, &StateHandlerFall, In->SectionList[CurrentSection].Data2);
						}
					}
					else
					{
						EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x20000001, 0);
					}
				}
			}
			break;
		}
		case 0x100001:
		{

			if (CurrentSection == 0)
			{
				ControlFlag = 1297;
				Raziel.alarmTable = 200;
				PhysicsMode = 0;
				In->CharacterInstance->anim.section[CurrentSection].swAlarmTable = &Raziel.alarmTable;
			}

			In->SectionList[CurrentSection].Data2 = 0;

			break;
		}
		default:
		{
			DefaultStateHandler(In, CurrentSection, Data);
		}
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}

void StateHandlerFall(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $s0
	int Moving; // $s7
	int DeferFlag; // $s6
	struct evPhysicsSwimData* SwimData; // $s1

	Moving = 0;
	DeferFlag = 1;

	if (CurrentSection != 0 && (STREAM_GetLevelWithID(In->CharacterInstance->currentStreamUnitID)->unitFlags & 0x1000))
	{
		EnMessageQueueData(&In->SectionList[CurrentSection].Event, 16, Moving);
	}

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
			case 0x4020000:
			{
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
			}

			case 0x10000000:
			{
				Moving = 1;

				break;
			}
			case 0x20000001:
			{
				if (Raziel.Mode != 16 && Raziel.Mode != 32 && CurrentSection == 0)
				{
					SetDropPhysics(In->CharacterInstance, &Raziel);
				}
				break;
			}
			case 0x2000000:
			{
				razPickupAndGrab(In, CurrentSection);
				break;
			}
			case 0x4010008:
			{
				if (DeferFlag)
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x4010008, 0);
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
			}
			case 0x100000:
			{
				StateSwitchStateCharacterData(In, &StateHandlerForcedGlide, 0);
				break;
			}
			case 0x100001:
			{
				if (CurrentSection == 0)
				{
					ControlFlag = 281;

					if (Raziel.Mode != 256)
					{
						ControlFlag = 1305;
					}

					In->SectionList[CurrentSection].Data1 = 0;

					Raziel.movementMinRate = 0;

					PhysicsMode = 0;
				}

				if (Ptr->Data && (PadData[0] & 0x80))
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
				}

				break;
			}
			case 0x40005:
			{
				if (Raziel.HitPoints <= 99999)
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerStumble, 0);
				}
				break;
			}
			case 0x80000001:
			{
				if (Raziel.Senses.heldClass != 3 && !(ControlFlag & 0x2000000) && CurrentSection == 0)
				{
					StateSwitchStateCharacterData(In,&StateHandlerGlide, 3);
				}
				break;
			}
			default:
			{
				DefaultStateHandler(In, CurrentSection, Data);
				break;
			}

		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerSlide(struct __CharacterState *In /*$s0*/, int CurrentSection /*$s1*/, int Data /*$s3*/)
void StateHandlerSlide(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 2621, offset 0x800ac3f8
	/* begin block 1 */
		// Start line: 2622
		// Start offset: 0x800AC3F8
		// Variables:
			struct __Event *Ptr; // $v0
	/* end block 1 */
	// End offset: 0x800AC5BC
	// End Line: 2678

	/* begin block 2 */
		// Start line: 5560
	/* end block 2 */
	// End Line: 5561
			UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerBlock(struct __CharacterState *In /*$s2*/, int CurrentSection /*$s4*/, int Data /*$s7*/)
void StateHandlerBlock(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 2688, offset 0x800ac5d8
	/* begin block 1 */
		// Start line: 2689
		// Start offset: 0x800AC5D8
		// Variables:
			struct __Event *Ptr; // $a0
			int Anim; // $s3

		/* begin block 1.1 */
			// Start line: 2727
			// Start offset: 0x800AC7E0
			// Variables:
				struct _Rotation rot; // stack offset -56
		/* end block 1.1 */
		// End offset: 0x800AC7E0
		// End Line: 2728

		/* begin block 1.2 */
			// Start line: 2743
			// Start offset: 0x800AC868
		/* end block 1.2 */
		// End offset: 0x800AC868
		// End Line: 2744

		/* begin block 1.3 */
			// Start line: 2757
			// Start offset: 0x800AC90C
			// Variables:
				struct evPhysicsEdgeData *EdgeData; // $s1

			/* begin block 1.3.1 */
				// Start line: 2762
				// Start offset: 0x800AC94C
				// Variables:
					//SVECTOR startVec; // stack offset -56
					///SVECTOR endVec; // stack offset -48
			/* end block 1.3.1 */
			// End offset: 0x800AC9A4
			// End Line: 2770
		/* end block 1.3 */
		// End offset: 0x800AC9F8
		// End Line: 2779
	/* end block 1 */
	// End offset: 0x800ACA18
	// End Line: 2792

	/* begin block 2 */
		// Start line: 5703
	/* end block 2 */
	// End Line: 5704
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerDeCompression(struct __CharacterState *In /*$s0*/, int CurrentSection /*$s2*/, int Data /*$s5*/)
void StateHandlerDeCompression(struct __CharacterState *In, int CurrentSection, int Data)
{
	struct __Event* Ptr;

	In->CharacterInstance->cachedTFace = -1;

	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100001:
		{
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
				if ((Raziel.Mode & 0x20) || (G2EmulationQueryAnimation(In, 0) == 17) || G2EmulationQueryAnimation(In, 0) == 18 || G2EmulationQueryAnimation(In, 0) == 19 || (In->CharacterInstance->zVel < -256))
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 44, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 41, 0, 1, 1);
					}
				}
				else if ((Raziel.Mode & 0x10) != 0 || Ptr->Data == 0)
				{
					if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 28, 0, 0))
					{
						G2EmulationSwitchAnimationCharacter(In, 37, 0, 1, 1);
					}
				}
				else if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 12, 0, 0))
				{
					G2EmulationSwitchAnimationCharacter(In, 29, 0, 0, 1);
				}
			}
		}
		case 0x8000000:
		{
			if ((PadData[0] & 0x100))
			{
				if (!CurrentSection)
				{
					StateSwitchStateCharacterData(In, &StateHandlerCrouch, 0);
					In->SectionList[CurrentSection].Data1 = 0;
				}
				else
				{
					In->SectionList[CurrentSection].Data1 = 0;
				}
			}
			else if (!(PadData[0] & 0x8000000F))
			{
				StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 4));
				Raziel.Mode = 1;
				In->SectionList[CurrentSection].Data1 = 0;
			}
			else if (CurrentSection)
			{
				In->SectionList[CurrentSection].Data1 = 0;
			}
			else if (G2EmulationQueryAnimation(In, 0) == 29)
			{
				StateSwitchStateCharacterData(In, &StateHandlerMove, 0);
				In->SectionList[CurrentSection].Data1 = 0;
			}
			else
			{
				StateSwitchStateCharacterData(In, &StateHandlerStartMove, 5);
				In->SectionList[CurrentSection].Data1 = 0;
			}

			break;
		}
		default:
		{
			DefaultStateHandler(In, CurrentSection, Data);
			break;
		}

		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerGlide(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s3*/, int Data /*$fp*/)
void StateHandlerGlide(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 2889, offset 0x800acdb4
	/* begin block 1 */
		// Start line: 2890
		// Start offset: 0x800ACDB4
		// Variables:
			struct __Event *Ptr; // $s0
			int Frame; // $s7
			int Anim; // $s5
			int moving; // $s6

		/* begin block 1.1 */
			// Start line: 3009
			// Start offset: 0x800AD32C
			// Variables:
				struct evObjectDraftData *draft; // $a2
		/* end block 1.1 */
		// End offset: 0x800AD384
		// End Line: 3016

		/* begin block 1.2 */
			// Start line: 3021
			// Start offset: 0x800AD3AC
			// Variables:
				struct evPhysicsSwimData *SwimData; // $v0
		/* end block 1.2 */
		// End offset: 0x800AD43C
		// End Line: 3034
	/* end block 1 */
	// End offset: 0x800AD4F0
	// End Line: 3087

	/* begin block 2 */
		// Start line: 6140
	/* end block 2 */
	// End Line: 6141
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerHang(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s2*/, int Data /*$fp*/)
void StateHandlerHang(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 3090, offset 0x800ad520
	/* begin block 1 */
		// Start line: 3091
		// Start offset: 0x800AD520
		// Variables:
			struct __Event *Ptr; // $a0
			int Anim; // $s7

		/* begin block 1.1 */
			// Start line: 3103
			// Start offset: 0x800AD6E0
			// Variables:
				struct evControlInitHangData *data; // $s0
		/* end block 1.1 */
		// End offset: 0x800AD7F4
		// End Line: 3133
	/* end block 1 */
	// End offset: 0x800ADA94
	// End Line: 3244

	/* begin block 2 */
		// Start line: 6555
	/* end block 2 */
	// End Line: 6556
				UNIMPLEMENTED();
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

void StateHandlerGlyphs(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event* Ptr; // $s0
	int hitPosted; // $s5
	struct evActionPlayHostAnimationData* ptr; // $v0
	struct evMonsterHitData* data; // $v1

	//s1 = In
	//s2 = CurrentSection
	//s6 = Data
	hitPosted = 0;

	G2EmulationQueryAnimation(In, CurrentSection);

	Raziel.invincibleTimer = 12288;
	//s4 = In->SectionList[CurrentSection]
	//s3 = In + [CurrentSection]

	//loc_800AF6B0
	while ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x80007:
		{
			//loc_800AFC64
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					razPrepGlyph();

					razSwitchStringAnimation(In->CharacterInstance, 0);
				}
				//loc_800AFC90
				
				In->SectionList[CurrentSection].Data1 = 0;

				if (CurrentSection == 0)
				{
					razPlaneShift(In->CharacterInstance);

					if ((Raziel.Mode & 0x40000))
					{
						CAMERA_ChangeToOutOfWater(&theCamera, In->CharacterInstance);

						SteerSwitchMode(In->CharacterInstance, 6);
					}
					//loc_800AFCD4

					SteerSwitchMode(In->CharacterInstance, 0);

					if ((Raziel.playerEvent & 0x2000))
					{
						razSetPlayerEventHistory(0x2000);

						HINT_KillSpecificHint(0x28);
					}
				}
				//loc_800AFD04
				Raziel.invincibleTimer = 122880;
			}
			//loc_800AFEA0
			break;
		}
		case 0x100004:
		{
			if (CurrentSection == 0)
			{
				razReaverOn();

				if (GlyphIsGlyphOpen(Raziel.GlyphSystem))
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
				}
			}

			Raziel.invincibleTimer = 0;
			break;
		}
		case 0x10000000:
		{
			//loc_800AFD4C
			if (CurrentSection == 0)
			{
				if ((PadData[0] & 0x4))
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x10000004, Ptr->Data);
				}
				//loc_800AFD84

				if ((PadData[0] & 0x8))
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x10000002, Ptr->Data);
				}
			}
			//loc_800AFEA0
			break;
		}
		case 0x80000010:
		{
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					In->SectionList[CurrentSection].Data1 = 0;

					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
				}
				
				if ((Raziel.Mode & 0x40000))
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerSwim, 0);
				}
				else
				{
					StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				}
			}
			//loc_800AFEA0
			break;
		}
		case 0x100001:
		{
			//loc_800AF8A4

			//v0 = 1
			if (CurrentSection == 0)
			{
				//v1 - 0x1100000
				if (Ptr->Data == 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000010, (intptr_t)In->CharacterInstance);
				}
				//loc_800AF8D4

				Raziel.Mode |= 0x1;

				ControlFlag = 0x1100008;

				SteerSwitchMode(In->CharacterInstance, 16);

				razResetMotion(In->CharacterInstance);
			}
			//loc_800AF904
			In->SectionList[CurrentSection].Data1 = 1;

			PhysicsMode = 3;

			if (Ptr->Data != 0)
			{
				EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80007, 0);
			}
			//loc_800AFEA0	
			break;
		}
		case 0x80000000:
		{
			if (In->SectionList[CurrentSection].Data1 != 0)
			{
				if (CurrentSection == 0)
				{
					INSTANCE_Post(Raziel.GlyphSystem, 0x80000000, 0);

					PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
				}
				//loc_800AFEA0
			}
			//loc_800AFEA0
			break;
		}
		case 0x8000003:
		case 0x8000000:
		{
			if (CurrentSection == 0)
			{
				CheckStringAnimation(In->CharacterInstance, Ptr->ID);
			}
			break;
		}
		case 0x4000001:
		{
			if (!(Raziel.Mode & 0x40000))
			{
				PhysicsMode = 0;

				SetDropPhysics(In->CharacterInstance, &Raziel);
			}
			break;
		}
		case 0:
		{
			break;
		}
		case 0x100000:
		{
			if (In->SectionList[CurrentSection].Data1 == 0)
			{
				if ((Raziel.Mode & 0x40000) != 0 && Raziel.CurrentPlane == 1)
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
				//loc_800AFA28
				if (!(Raziel.Mode & 0x40000) && In->SectionList[CurrentSection].Data1 != 2)
				{
					StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));

					In->SectionList[CurrentSection].Data1 = 2;
				}
			}
			break;
		}
		default:
			printf("%x\n SHG ", Ptr->ID);
			//DefaultStateHandler(In, CurrentSection, Data);
			break;
		}

		DeMessageQueue(&In->SectionList[CurrentSection].Event);
	}
	//loc_800AFEB0
	In->CharacterInstance->cachedTFace = -1;
}

void DefaultStateHandler(struct __CharacterState* In, int CurrentSection, int Data)
{
	struct __Event *Ptr; // $s0
	int message; // stack offset -32
	int messageData; // stack offset -28
	int i; // $s1
	struct evPhysicsGravityData *ptr; // $v1
	short zRot; // $s0
	struct _Position pos1; // stack offset -48
	struct _Position pos2; // stack offset -40
	int diff; // $v0
	struct evFXHitData *BloodData; // $a0
	struct _SVector Accel; // stack offset -48
	struct evPhysicsEdgeData *data; // $s0
	
	
	if ((Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event)) != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100006:
		{
			InitAlgorithmicWings(In->CharacterInstance);
			break;
		}
		case 0x4000001:
		{
			PhysicsMode = 0;
			SetDropPhysics(In->CharacterInstance, &Raziel);
			if (In->CharacterInstance->zVel < Raziel.fallZVelocity && CurrentSection == 0)
			{
				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, 0, 0))
				{
					G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
				}

				StateSwitchStateCharacterDataDefault(In, &StateHandlerFall, 0);
			}
			break;
		}
		case 0x4010008:
		{
			if (PhysicsMode != 3)
			{
				PhysicsMode = 3;

				SetPhysics(In->CharacterInstance, -16, 0, 0, 0);
			}
			break;
		}
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

// autogenerated function stub: 
// long /*$ra*/ RazielAnimCallbackDuringPause(struct _G2Anim_Type *anim /*$a0*/, int sectionID /*$a1*/, enum _G2AnimCallbackMsg_Enum message /*$s0*/, long messageDataA /*$s1*/, long messageDataB /*stack 16*/, void *data /*stack 20*/)
long RazielAnimCallbackDuringPause(struct _G2Anim_Type *anim, int sectionID, enum _G2AnimCallbackMsg_Enum message, long messageDataA, long messageDataB, void *data)
{ // line 4961, offset 0x800b10b0
	/* begin block 1 */
		// Start line: 4962
		// Start offset: 0x800B10B0

		/* begin block 1.1 */
			// Start line: 4971
			// Start offset: 0x800B10D0
			// Variables:
				struct evAnimationControllerDoneData *ControllerData; // $v1
		/* end block 1.1 */
		// End offset: 0x800B114C
		// End Line: 4987
	/* end block 1 */
	// End offset: 0x800B114C
	// End Line: 4992

	/* begin block 2 */
		// Start line: 10454
	/* end block 2 */
	// End Line: 10455
				UNIMPLEMENTED();
	return 0;
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

void RazielPost(struct _Instance* instance, unsigned long Message, unsigned long Data)
{
	int i; // $s0
	struct PlayerSaveData* data; // $s0
	struct _G2AnimSection_Type* animSection; // $v0
	struct _Instance* heldWeapon; // $v1

	//s1 = instance
	//s2 = Message
	//s3 = Data

	//v0 = 0x10000A
	switch (Message)
	{
    case 0:
            break;
	default:
	{
		for (i = 0; i < 3; i++)
		{
			EnMessageQueueData(&Raziel.State.SectionList[i].Defer, Message, Data);;
		}
		break;
	}
	}

	//loc_800B17BC
	GAMELOOP_Reset24FPS();
#if 0
		loc_800B14CC :
		lw      $v0, -0x238($gp)
		lui     $v1, 4
		and $v0, $v1
		bnez    $v0, loc_800B1BF4
		nop
		jal     sub_800A4D0C
		nop
		bnez    $v0, loc_800B1BF4
		nop
		addiu   $a1, $gp, -0x9D0
		jal     sub_800A335C
		move    $a0, $s3
		j       loc_800B1BF4
		nop

		loc_800B1504 :
	lw      $v0, -0x238($gp)
		lui     $v1, 4
		and $v0, $v1
		bnez    $v0, loc_800B151C
		nop
		sw      $zero, Raziel.Senses.EngagedMask

		loc_800B151C:
	lw      $v0, -0x634($gp)
		li      $v1, 0xFFFFFFDF
		and $v0, $v1
		j       loc_800B19F8
		li      $v1, 0xFFFFFFBF

		loc_800B1530 :
		lw      $v0, -0x238($gp)
		lui     $v1, 0x4000
		and $v0, $v1
		beqz    $v0, loc_800B1BF4
		move    $a0, $s1
		addiu   $a1, $gp, -0x4238
		sw      $s3, 0xC0($s1)
		jal     sub_800B3EDC
		sh      $zero, 0x2C($s3)
		jal     sub_800A5C84
		nop
		lw      $a1, 0xC0($s1)
		move    $a0, $v0
		jal     sub_8001ED78
		addiu   $a1, 0x28  # '('
		j       loc_800B1BF4
		nop

		loc_800B1574 :
	lui     $a0, 0x800D
		lw      $s0, 4($s3)
		li      $a0, aUnder      # "under"
		lw      $v0, 0($s0)
		nop
		sw      $v0, -0x5C8($gp)
		sw      $v0, -0x54A8($gp)
		jal     sub_800A7EA8
		li      $a1, 5
		bnez    $v0, loc_800B15A4
		li      $v0, 2
		lw      $v0, 4($s0)

		loc_800B15A4:
	nop
		sw      $v0, -0x598($gp)
		lhu     $v0, 8($s0)
		nop
		sh      $v0, -0x5D8($gp)
		lhu     $v0, 0xC($s0)
		nop
		sh      $v0, -0x5D6($gp)
		lw      $a0, 0xC($s0)
		jal     sub_8007C484
		nop
		lhu     $v0, 0x10($s0)
		nop
		sh      $v0, -0x5C0($gp)
		lhu     $v1, 0x12($s0)
		lw      $v0, -0x5C8($gp)
		sw      $zero, -0x5A0($gp)
		andi    $v0, 8
		sh      $v1, -0x5BE($gp)
		beqz    $v0, loc_800B15FC
		nop
		sw      $zero, -0x54A4($gp)

		loc_800B15FC:
	lw      $v0, 0x14($s0)
		nop
		sw      $v0, -0x420($gp)
		andi    $v0, 0x1000
		bnez    $v0, loc_800B1620
		li      $v0, 0x64  # 'd'
		sw      $v0, -0x5D4($gp)
		j       loc_800B1BF4
		nop

		loc_800B1620 :
	jal     sub_800A4810
		nop
		sw      $v0, -0x5D4($gp)
		j       loc_800B1BF4
		nop

		loc_800B1634 :
	sw      $s3, -0x594($gp)
		j       loc_800B1BF4
		nop

		loc_800B1640 :
	li      $v0, 0xB50
		sw      $v0, -0x594($gp)
		j       loc_800B1BF4
		nop

		loc_800B1650 :
	lw      $v0, -0x238($gp)
		nop
		andi    $v0, 8
		beqz    $v0, loc_800B1BF4
		nop
		lw      $v0, -0x634($gp)
		nop
		andi    $v0, 2
		bnez    $v0, loc_800B1BF4
		move    $s0, $zero
		addiu   $s1, $gp, -0x938

		loc_800B167C:
	move    $a0, $s1
		move    $a1, $s2
		jal     sub_80070D38
		move    $a2, $s3
		addiu   $s0, 1
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B167C
		addiu   $s1, 0x11C
		j       loc_800B1BF4
		nop

		loc_800B16A4 :
	sw      $s3, 0x38($s1)
		sw      $zero, 0xB4($s1)
		lw      $v0, -0x5C4($gp)
		nop
		sw      $s3, 0x38($v0)
		lw      $v0, -0x5C4($gp)
		nop
		sw      $zero, 0xB4($v0)
		lw      $v0, -0x5A0($gp)
		nop
		beqz    $v0, loc_800B1BF4
		nop
		sw      $s3, 0x38($v0)
		lw      $v0, -0x5A0($gp)
		j       loc_800B1BF4
		sw      $zero, 0xB4($v0)

		loc_800B16E4:
	lw      $v0, 4($s3)
		nop
		sw      $v0, -0x58C($gp)
		j       loc_800B1BF4
		nop

		loc_800B16F8 :
	jal     sub_800A46C8
		move    $a0, $s3
		j       loc_800B1BF4
		nop

		loc_800B1708 :
	jal     sub_800A4D78
		move    $a0, $s3
		j       loc_800B1BF4
		nop

		loc_800B1718 :
	jal     sub_800A4DBC
		move    $a0, $s3
		j       loc_800B1BF4
		nop

		loc_800B1728 :
	li      $v0, 1
		sw      $v0, 0x28 + var_18($sp)
		addiu   $a0, $gp, -0x9CC
		li      $a1, 0x80
		move    $a2, $zero
		jal     sub_800723F0
		li      $a3, 3
		addiu   $a0, $gp, -0x9CC
		lui     $a1, 0x800A
		j       loc_800B18EC
		li      $a1, sub_8009CEB4

		loc_800B1754 :
	lw      $v0, -0x238($gp)
		lui     $v1, 0x20  # ' '
		and $v0, $v1
		beqz    $v0, loc_800B1770
		nop
		jal     sub_800B2574
		move    $a0, $s3

		loc_800B1770 :
	lhu     $v0, -0x484($gp)
		lhu     $v1, 0($s3)
		nop
		addu    $v0, $v1
		sh      $v0, -0x484($gp)
		lhu     $v0, -0x482($gp)
		lhu     $v1, 2($s3)
		nop
		addu    $v0, $v1
		sh      $v0, -0x482($gp)
		lhu     $v0, -0x480($gp)
		lhu     $v1, 4($s3)
		nop
		addu    $v0, $v1
		sh      $v0, -0x480($gp)
		jal     sub_8005E498
		nop
		j       loc_800B1BF4
		nop

		loc_800B17BC :
	beqz    $s3, loc_800B1804
		move    $s0, $zero
		lui     $v0, 0x800B
		addiu   $a0, $v0, (sub_800B0E80 - 0x800B0000)
		li      $v1, 0x1EC

		loc_800B17D0 :
		addu    $v0, $s1, $v1
		addiu   $s0, 1
		sw      $a0, 0x1C($v0)
		sw      $zero, 0x20($v0)
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B17D0
		addiu   $v1, 0x30  # '0'
		jal     sub_800A1CEC
		move    $a0, $s1
		jal     sub_800A63F4
		move    $a0, $s1
		j       loc_800B1BF4
		nop

		loc_800B1804 :
	lui     $v0, 0x800B
		addiu   $a0, $v0, (sub_800B087C - 0x800B0000)
		li      $v1, 0x1EC

		loc_800B1810 :
		addu    $v0, $s1, $v1
		addiu   $s0, 1
		sw      $a0, 0x1C($v0)
		sw      $zero, 0x20($v0)
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B1810
		addiu   $v1, 0x30  # '0'
		jal     sub_800A1B54
		move    $a0, $s1
		j       loc_800B1BF4
		nop

		loc_800B183C :
	beqz    $s3, loc_800B18A8
		lui     $v1, 0x4000
		lw      $v0, -0x670($gp)
		nop
		and $v0, $v1
		bnez    $v0, loc_800B1BF4
		move    $a0, $s1
		sw      $v1, -0x670($gp)
		jal     sub_8009A1AC
		li      $a1, 0xFFFFFFF0
		move    $s0, $zero
		lui     $s1, 0x800B
		move    $a0, $zero

		loc_800B1870 :
	move    $a1, $a0
		jal     sub_8007193C
		li      $a2, 3
		addiu   $a0, $gp, -0x9CC
		move    $a1, $s0
		addiu   $a2, $s1, -0x7B20
		jal     sub_80072B04
		move    $a3, $v0
		addiu   $s0, 1
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B1870
		move    $a0, $zero
		j       loc_800B1BF4
		nop

		loc_800B18A8 :
	lw      $v1, -0x670($gp)
		lui     $v0, 0x4000
		and $v0, $v1, $v0
		beqz    $v0, loc_800B1BF4
		lui     $v0, 0xBFFF
		li      $v0, 0xBFFFFFFF
		and $v0, $v1, $v0
		sw      $v0, -0x670($gp)
		j       loc_800B1BF4
		nop

		loc_800B18D0 :
	jal     sub_800A4DE4
		move    $a0, $s1
		j       loc_800B1BF4
		nop

		loc_800B18E0 :
	addiu   $a0, $gp, -0x9CC
		li      $a1, sub_800A9860

		loc_800B18EC :
	jal     sub_80072BD0
		move    $a2, $zero
		j       loc_800B1BF4
		nop

		loc_800B18FC :
	jal     sub_800A5620
		move    $a0, $s1
		j       loc_800B1BF4
		nop

		loc_800B190C :
	lw      $v0, -0x5C8($gp)
		nop
		or $v0, $s3
		sw      $v0, -0x5C8($gp)
		sw      $v0, -0x54A8($gp)
		jal     sub_800A4ED0
		nop
		li      $v0, 0x3FC00
		and $v0, $s3, $v0
		beqz    $v0, loc_800B1BF4
		nop
		lw      $v0, -0x5A0($gp)
		nop
		beqz    $v0, loc_800B1BF4
		nop
		jal     sub_800A5E14
		nop
		jal     sub_800A5FF4
		move    $a0, $s3
		jal     sub_800A5FB4
		move    $a0, $v0
		j       loc_800B1BF4
		nop

		loc_800B196C :
	addiu   $a0, $gp, -0x938
		li      $a1, 0x100011
		jal     sub_80070D38
		move    $a2, $s3
		j       loc_800B1BF4
		nop

		loc_800B1988 :
	move    $a0, $s3
		jal     sub_80034648
		li      $a1, 4
		sw      $v0, Raziel.Senses.heldClass
		li      $v1, 8
		beq     $v0, $v1, loc_800B19AC
		nop
		jal     sub_800A5D1C
		nop

		loc_800B19AC :
	lw      $v0, -0x670($gp)
		li      $v1, 0xFFFFF7FF
		and $v0, $v1
		sw      $v0, -0x670($gp)
		j       loc_800B19F0
		nop

		loc_800B19C4 :
	lw      $v0, -0x5A0($gp)
		nop
		beq     $s3, $v0, loc_800B19F0
		nop
		jal     sub_800A5E14
		nop
		bnez    $v0, loc_800B19E8
		nop
		sw      $zero, Raziel.Senses.heldClass

		loc_800B19E8:
	jal     sub_800A5D4C
		nop

		loc_800B19F0 :
	lw      $v0, -0x634($gp)
		li      $v1, 0xFFFFFF7F

		loc_800B19F8 :
		and $v0, $v1
		sw      $v0, -0x634($gp)
		j       loc_800B1BF4
		nop

		loc_800B1A08 :
	sw      $s3, -0x434($gp)
		j       loc_800B1BF4
		nop

		loc_800B1A14 :
	jal     sub_80040818
		li      $a0, 1
		bnez    $v0, loc_800B1BF4
		li      $a1, 1
		move    $a2, $zero
		li      $a3, 0x4B  # 'K'
		lw      $a0, -0x420C($gp)
		li      $v0, 0xDAC
		sw      $v0, 0x28 + var_18($sp)
		jal     sub_8004004C
		addiu   $a0, 0x5C  # '\'
		j       loc_800B1BF4
		nop

		loc_800B1A48 :
	beqz    $s3, loc_800B1AD0
		lui     $a1, 0x800A
		li      $a1, sub_8009A90C
		sw      $a1, -0x4C8($gp)
		addiu   $a0, $gp, -0x9CC
		jal     sub_80072A8C
		move    $a2, $zero
		jal     sub_800A1B54
		move    $a0, $s1
		move    $s2, $zero
		addiu   $s1, $gp, -0x938
		addiu   $s0, $gp, -0x9C0

		loc_800B1A78:
	jal     sub_80070CD8
		move    $a0, $s0
		jal     sub_80070CD8
		move    $a0, $s1
		addiu   $s1, 0x11C
		addiu   $s2, 1
		slti    $v0, $s2, 3
		bnez    $v0, loc_800B1A78
		addiu   $s0, 0x11C
		jal     sub_800303B8
		nop
		jal     sub_800A5CBC
		nop
		move    $v1, $v0
		beqz    $v1, loc_800B1BF4
		nop
		lw      $v0, -0x5A0($gp)
		nop
		beq     $v1, $v0, loc_800B1BF4
		move    $a0, $zero
		j       loc_800B1B78
		li      $a1, 0x1000

		loc_800B1AD0:
	li      $t0, 0xFFFDFFFF
		move    $a0, $zero
		move    $a1, $a0
		li      $a2, 3
		addiu   $s2, $gp, -0x938
		addiu   $s1, $gp, -0x9C0
		li      $a3, 0xFFFFFFEF
		lw      $v0, -0x634($gp)
		lw      $v1, -0x238($gp)
		and $v0, $a3
		and $v1, $t0
		sw      $v0, -0x634($gp)
		sw      $v1, -0x238($gp)
		jal     sub_8007193C
		move    $s0, $a0
		addiu   $a0, $gp, -0x9CC
		li      $a1, sub_800A84E0
		jal     sub_80072A8C
		move    $a2, $v0

		loc_800B1B24 :
	jal     sub_80070CD8
		move    $a0, $s1
		jal     sub_80070CD8
		move    $a0, $s2
		addiu   $s2, 0x11C
		addiu   $s0, 1
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B1B24
		addiu   $s1, 0x11C
		jal     sub_800303A8
		nop
		jal     sub_800A5CBC
		nop
		move    $v1, $v0
		beqz    $v1, loc_800B1BF4
		nop
		lw      $v0, -0x5A0($gp)
		nop
		beq     $v1, $v0, loc_800B1BF4
		li      $a0, 0x1000
		move    $a1, $zero

		loc_800B1B78 :
	jal     sub_800A55FC
		li      $a2, 0xA
		j       loc_800B1BF4
		nop

		loc_800B1B88 :
	move    $a0, $s1
		addiu   $a1, $gp, -0x46C
		li      $a2, 0x33  # '3'
		li      $a3, 0xFFFFFF38
		move    $v0, $a3
		sw      $v0, 0x28 + var_18($sp)
		li      $v0, 0x78  # 'x'
		sll     $s0, $s3, 12
		sw      $v0, 0x28 + var_14($sp)
		sw      $v0, 0x28 + var_10($sp)
		li      $v0, 0xDAC
		sw      $s0, 0x28 + var_C($sp)
		jal     sub_800A7D40
		sw      $v0, 0x28 + var_8($sp)
		sw      $s0, -0x43C($gp)
		sw      $zero, -0x438($gp)
		j       loc_800B1BF4
		nop

		loc_800B1BD0 :
	addiu   $s1, $gp, -0x938

		loc_800B1BD4 :
		move    $a0, $s1
		move    $a1, $s2
		jal     sub_80070D38
		move    $a2, $s3
		addiu   $s0, 1
		slti    $v0, $s0, 3
		bnez    $v0, loc_800B1BD4
		addiu   $s1, 0x11C

		loc_800B1BF4:
	lw      $ra, 0x28 + var_s10($sp)
		lw      $s3, 0x28 + var_sC($sp)
		lw      $s2, 0x28 + var_s8($sp)
		lw      $s1, 0x28 + var_s4($sp)
		lw      $s0, 0x28 + var_s0($sp)
		jr      $ra
		addiu   $sp, 0x40
#endif
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


// autogenerated function stub: 
// void /*$ra*/ ProcessConstrict()
void ProcessConstrict()
{ // line 5900, offset 0x800b2588
	/* begin block 1 */
		// Start line: 5901
		// Start offset: 0x800B2588

		/* begin block 1.1 */
			// Start line: 5942
			// Start offset: 0x800B268C
			// Variables:
				int i; // $s0

			/* begin block 1.1.1 */
				// Start line: 5990
				// Start offset: 0x800B2754
				// Variables:
					int thisIndex; // $v1
					int nextIndex; // $v0
			/* end block 1.1.1 */
			// End offset: 0x800B27B8
			// End Line: 6011
		/* end block 1.1 */
		// End offset: 0x800B284C
		// End Line: 6034
	/* end block 1 */
	// End offset: 0x800B284C
	// End Line: 6065

	/* begin block 2 */
		// Start line: 12443
	/* end block 2 */
	// End Line: 12444

	/* begin block 3 */
		// Start line: 12446
	/* end block 3 */
	// End Line: 12447
					UNIMPLEMENTED();
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

void ProcessTimers(struct _Instance* instance)
{
	//a2 = Raziel.timeAccumulator
	//s3 = instance

	if (Raziel.timeAccumulator > 0)
	{
		Raziel.timeAccumulator -= gameTrackerX.timeMult;

		if (Raziel.timeAccumulator <= 0)
		{
			INSTANCE_Post(instance, 0x100015, -Raziel.timeAccumulator);
		}
	}
	//loc_800B2C1C
	if (Raziel.soundTimerNext > 0)
	{
		Raziel.soundTimerNext -= gameTrackerX.timeMult;

		if (Raziel.soundTimerNext <= 0)
		{
			//v0 = 
			Raziel.soundTimerNext = 0;

			switch (Raziel.soundTimerData)
			{
			case 1:
			{
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 19, 1000, 1000, 120, 120, 4096, 3500);
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 20, 1030, 1030, 120, 120, 4096, 3500);

				Raziel.soundTimerNext = 122880;

				//v0 = Raziel.effectsFlags
				//v1 = 2
				break;
			}
			case 2:
			{
				//s2 = 0x3C000
				//a0 = instance
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 19, 1000, 1000, 120, 0, 245760, 3500);
				razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle2, 20, 1030, 1030, 120, 0, 245760, 3500);

				//j loc_800B3078

				break;
			case 3:
			{
				if (Raziel.soundHandle != 0)
				{
					SndEndLoop(Raziel.soundHandle);
				}
				break;
			}
			}
			}
			//loc_800B30A8
		}
		//loc_800B30A8
		UNIMPLEMENTED();
#if 0

		loc_800B2D74 :
		move    $a0, $s3
			addiu   $a1, $gp, -0x46C
			li      $a2, 0xD
			move    $a3, $zero
			li      $v0, 0x64  # 'd'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x46C($gp)
			sw      $zero, 0x28 + var_18($sp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0xA000
			sw      $v0, -0x43C($gp)
			li      $v0, 4
			sw      $v0, -0x438($gp)
			j       loc_800B30A8
			nop

			loc_800B2DC4 : # jumptable 800B2C70 case 4
			lw      $a0, -0x454($gp)
			nop
			beqz    $a0, loc_800B2DDC
			nop
			jal     sub_80040870
			nop

			loc_800B2DDC :
		move    $a0, $s3
			addiu   $a1, $gp, -0x454
			li      $a2, 0xD
			li      $a3, 0x32  # '2'
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x5F  # '_'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x454($gp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0x7000
			sw      $v0, -0x43C($gp)
			lw      $v0, -0x4B4($gp)
			li      $v1, 5

			loc_800B2E28:
		sw      $v1, -0x438($gp)
			ori     $v0, 8
			sw      $v0, -0x4B4($gp)
			j       loc_800B30A8
			nop

			loc_800B2E3C : # jumptable 800B2C70 case 5
			lw      $a0, -0x46C($gp)
			nop
			beqz    $a0, loc_800B2E54
			nop
			jal     sub_80040870
			nop

			loc_800B2E54 :
		move    $a0, $s3
			addiu   $a1, $gp, -0x46C
			li      $a2, 0xD
			li      $a3, 0xFFFFFFEC
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x50  # 'P'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x46C($gp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0x8000
			sw      $v0, -0x43C($gp)
			li      $v0, 6
			sw      $v0, -0x438($gp)
			j       loc_800B30A8
			nop

			loc_800B2EA8 : # jumptable 800B2C70 case 6
			lw      $a0, -0x454($gp)
			nop
			beqz    $a0, loc_800B2EC0
			nop
			jal     sub_80040870
			nop

			loc_800B2EC0 :
		move    $a0, $s3
			addiu   $a1, $gp, -0x454
			li      $a2, 0xD
			li      $a3, 0x64  # 'd'
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x4B  # 'K'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x454($gp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0xA000
			sw      $v0, -0x43C($gp)
			li      $v0, 7
			sw      $v0, -0x438($gp)
			j       loc_800B30A8
			nop

			loc_800B2F14 : # jumptable 800B2C70 case 7
			lw      $a0, -0x46C($gp)
			nop
			beqz    $a0, loc_800B2F2C
			nop
			jal     sub_80040870
			nop

			loc_800B2F2C :
		move    $a0, $s3
			addiu   $a1, $gp, -0x46C
			li      $a2, 0xD
			li      $a3, 0xFFFFFF9C
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x41  # 'A'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x46C($gp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0x5000
			sw      $v0, -0x43C($gp)
			li      $v0, 8
			sw      $v0, -0x438($gp)
			j       loc_800B30A8
			nop

			loc_800B2F80 : # jumptable 800B2C70 case 8
			lw      $a0, -0x454($gp)
			nop
			beqz    $a0, loc_800B2F98
			nop
			jal     sub_80040870
			nop

			loc_800B2F98 :
		move    $a0, $s3
			addiu   $a1, $gp, -0x454
			li      $a2, 0xD
			li      $a3, 0x1E
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x3C  # '<'
			sw      $v0, 0x28 + var_14($sp)
			sw      $v0, 0x28 + var_10($sp)
			li      $v0, 0x1000
			sw      $v0, 0x28 + var_C($sp)
			li      $v0, 0xDAC
			sw      $zero, -0x454($gp)
			jal     sub_800A7D40
			sw      $v0, 0x28 + var_8($sp)
			li      $v0, 0xA000
			sw      $v0, -0x43C($gp)
			li      $v0, 9
			sw      $v0, -0x438($gp)
			j       loc_800B30A8
			nop

			loc_800B2FEC : # jumptable 800B2C70 case 9
			lw      $a0, -0x46C($gp)
			nop
			beqz    $a0, loc_800B3004
			nop
			jal     sub_80040870
			nop

			loc_800B3004 :
		lw      $a0, -0x454($gp)
			sw      $zero, -0x46C($gp)
			beqz    $a0, loc_800B301C
			nop
			jal     sub_80040870
			nop

			loc_800B301C :
		lw      $v0, -0x4B4($gp)
			li      $v1, 0xFFFFFFFB
			sw      $zero, -0x454($gp)
			and $v0, $v1
			li      $v1, 0xFFFFFFF7
			and $v0, $v1
			sw      $v0, -0x4B4($gp)
			j       loc_800B30A8
			nop

			loc_800B3040 : # jumptable 800B2C70 case 10
			li      $v1, 0x3C000
			move    $a0, $s3
			addiu   $a1, $gp, -0x46C
			li      $a2, 0x33  # '3'
			li      $a3, 0xFFFFFF38
			move    $v0, $a3
			sw      $v0, 0x28 + var_18($sp)
			li      $v0, 0x78  # 'x'
			sw      $v0, 0x28 + var_14($sp)
			li      $v0, 0xDAC
			sw      $zero, 0x28 + var_10($sp)
			sw      $v1, 0x28 + var_C($sp)
			sw      $v0, 0x28 + var_8($sp)

loc_800B3078:
jal     sub_800A7D40
nop
sw      $zero, -0x43C($gp)
sw      $zero, -0x438($gp)
j       loc_800B30A8
nop

def_800B2C70 : # jumptable 800B2C70 default case
lw      $a0, -0x46C($gp)
nop
beqz    $a0, loc_800B30A8
nop
jal     sub_80040870
nop

loc_800B30A8 :
lw      $ra, 0x28 + var_s10($sp)
lw      $s3, 0x28 + var_sC($sp)
lw      $s2, 0x28 + var_s8($sp)
lw      $s1, 0x28 + var_s4($sp)
lw      $s0, 0x28 + var_s0($sp)
jr      $ra
addiu   $sp, 0x40
#endif
	}
}

void SetTimer(int ticks)
{
	Raziel.timeAccumulator = ticks >> 12;
}


void ProcessSpecialAbilities(struct _Instance* instance)
{
	unsigned long reaver;
	unsigned long temp;
	struct Object* soulReaverOb;
	struct Level* level;

	if ((Raziel.Abilities & 0x00000008) != 0)
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

						if (instance->matrix[0x29].t[2] < instance->splitPoint.z)
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

						if (instance->matrix[0x29].t[2] < level->waterZLevel)
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


// autogenerated function stub: 
// int /*$ra*/ SetupReaction(struct _Instance *player /*$s2*/, struct _Instance *instance /*$a0*/)
int SetupReaction(struct _Instance *player, struct _Instance *instance)
{ // line 6997, offset 0x800b3cd0
	/* begin block 1 */
		// Start line: 6998
		// Start offset: 0x800B3CD0
		// Variables:
			int FaceAngle; // $s3

		/* begin block 1.1 */
			// Start line: 7014
			// Start offset: 0x800B3D14
		/* end block 1.1 */
		// End offset: 0x800B3D9C
		// End Line: 7023
	/* end block 1 */
	// End offset: 0x800B3D9C
	// End Line: 7024

	/* begin block 2 */
		// Start line: 14808
	/* end block 2 */
	// End Line: 14809
			UNIMPLEMENTED();
	return 0;
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




