#include "Game/CORE.H"
#include <Game/STATE.H>
#include "PUPPET.H"
#include "RAZIEL.H"
#include "CONTROL.H"
#include "ALGOCTRL.H"
#include "STEERING.H"
#include <Game/G2/ANMG2ILF.H>
#include <Game/GAMEPAD.H>
#include <Game/STRMLOAD.H>
#include <Game/MATH3D.H>
#include "Game/GENERIC.H"


void StateHandlerPuppetShow(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 100%
{
	struct __Event* Ptr;

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x04000001:
				break;
			case 0x1000001:
				break;
			case 0x100001:
				if (Ptr->Data != 0)
				{
					G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, 3, 2);
				}

				Raziel.Mode = 1;
				Raziel.idleCount = 0;

				ControlFlag = 0x1020008;

				PhysicsMode = 0;

				SteerSwitchMode(In->CharacterInstance, 0);

				ResetPhysics(In->CharacterInstance, -16);

				Raziel.movementMinRate = 0;

				if (CurrentSection == 0)
				{
					Raziel.constrictGoodCircle = 0;
				}

				break;
			case 0x40003:
				if (CurrentSection == 0)
				{
					struct evActionPlayHostAnimationData* data1;

					data1 = (struct evActionPlayHostAnimationData*)Ptr->Data;

					G2EmulationInstanceToInstanceSwitchAnimationCharacter(data1->instance, data1->host, data1->newAnim, data1->newFrame, data1->frames, data1->mode);
				}

				break;
			case 0x40014:
				G2EmulationSwitchAnimation(In, CurrentSection, 0, 0, 3, 2);
				break;
			case 0x8000008:
				if (CurrentSection == 0)
				{
					struct evAnimationInstanceSwitchData* data2;

					data2 = (struct evAnimationInstanceSwitchData*)Ptr->Data;

					G2EmulationSwitchAnimationCharacter(In, data2->anim, data2->frame, data2->frames, data2->mode);
				}

				break;
			case 0x40016:
				G2EmulationSwitchAnimation(In, CurrentSection, 123, 0, 4, 2);
			case 0x4000C:
				StateSwitchStateData(In, CurrentSection, StateHandlerMoveToPosition, Ptr->Data);
				break;
			case 0x4000D:
				if (CurrentSection == 0)
				{
					struct evPositionData* data3;

					data3 = (struct evPositionData*)Ptr->Data;

					Raziel.puppetRotToPoint.z = data3->z;

					SteerSwitchMode(In->CharacterInstance, 13);
				}

				break;
			case 0x40021:
				Raziel.steeringVelocity = Ptr->Data;
				break;
			case 0x40018:
				if (CurrentSection == 0)
				{
					struct evPositionData* data4;
					short _x1;
					short _y1;
					short _z1;
					struct _Position* _v0;

					data4 = (struct evPositionData*)Ptr->Data;

					_x1 = data4->x;
					_y1 = data4->y;
					_z1 = data4->z;

					_v0 = &Raziel.puppetRotToPoint;

					_v0->x = _x1;
					_v0->y = _y1;
					_v0->z = _z1;

					SteerSwitchMode(In->CharacterInstance, 12);
				}

				break;
			case 0x40010:
				Raziel.Senses.Flags &= ~0x10;

				ControlFlag &= 0xFFFDFFFF;
				break;
			default:
				DefaultPuppetStateHandler(In, CurrentSection, Data);
				break;
			}

			DeMessageQueue(&In->SectionList[CurrentSection].Event);
		}
	}

	razApplyMotion(In, CurrentSection);
}


void StateHandlerMoveToPosition(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 100%
{
	struct __Event* Ptr;
	long distance;
	int motion;
	int applyMotion;

	applyMotion = 1;

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x1000000:
				break;
			case 0x100001:
				if (CurrentSection == 0)
				{
					struct evPositionData* moveToPoint;
					short _x1;
					short _y1;
					short _z1;
					struct _Position* _v0;

					moveToPoint = (struct evPositionData*)Ptr->Data;

					_x1 = moveToPoint->x;
					_y1 = moveToPoint->y;
					_z1 = moveToPoint->z;

					_v0 = &Raziel.puppetMoveToPoint;

					_v0->x = _x1;
					_v0->y = _y1;
					_v0->z = _z1;

					razAlignYRotInterp(In->CharacterInstance, &Raziel.puppetMoveToPoint, 0, 4);

					PhysicsMode = 0;

					SteerSwitchMode(In->CharacterInstance, 0);
				}

				break;
			case 0x100000:
				StateSwitchStateData(In, CurrentSection, &StateHandlerPuppetShow, SetControlInitIdleData(0, 0, 3));

				applyMotion = 0;
				break;
			case 0x40016:
				G2EmulationSwitchAnimation(In, CurrentSection, 123, 0, 4, 2);
			case 0x4000C:
			{
				struct evPositionData* moveToPoint;
				short _x1;
				short _y1;
				short _z1;
				struct _Position* _v0; // not from SYMDUMP

				moveToPoint = (struct evPositionData*)Ptr->Data;

				_x1 = moveToPoint->x;
				_y1 = moveToPoint->y;
				_z1 = moveToPoint->z;

				_v0 = &Raziel.puppetMoveToPoint;

				_v0->x = _x1;
				_v0->y = _y1;
				_v0->z = _z1;

				break;
			}
			case 0x4020000:
				break;
			case 0x4010400:
				break;
			case 0x4000001:
				PhysicsMode = 0;

				SetDropPhysics(In->CharacterInstance, &Raziel);
				break;
			default:
				DefaultPuppetStateHandler(In, CurrentSection, Data);
				break;
			}

			DeMessageQueue(&In->SectionList[CurrentSection].Event);
		}
	}

	if (applyMotion != 0)
	{
		if (CurrentSection == 0)
		{
			razAlignYRotInterp(In->CharacterInstance, &Raziel.puppetMoveToPoint, 0, 4);
		}

		distance = MATH3D_LengthXYZ(In->CharacterInstance->position.x - Raziel.puppetMoveToPoint.x, In->CharacterInstance->position.y - Raziel.puppetMoveToPoint.y, In->CharacterInstance->position.z - Raziel.puppetMoveToPoint.z);

		motion = razApplyMotion(In, CurrentSection) * 2;

		if ((CurrentSection == 0) && (distance < motion))
		{
			INSTANCE_Post(In->CharacterInstance, 0x100000, 0);
		}
	}
}

void DefaultPuppetStateHandler(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 98.99%
{
	struct __Event* Ptr;
	struct evPositionData* data;
	long _x1;
	long _y1;
	long _z1;
	struct _Vector* _v0;

	Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event);
	if (Ptr != NULL)
	{
		switch (Ptr->ID)
		{
		case 0x100004:
			razResetMotion(gameTrackerX.playerInstance);
			break;
		case 0x4000A:
			STREAM_SetInstancePosition(gameTrackerX.playerInstance, (struct evPositionData*)Ptr->Data);
			break;
		case 0x4000B:
			if (CurrentSection == 0)
			{
				data = (struct evPositionData*)Ptr->Data;
				gameTrackerX.playerInstance->rotation.z = data->z;
			}
			break;
		case 0x4000F:
			_v0 = &Raziel.Senses.lookAtPoint;
			_x1 = ((struct evPositionData*)Ptr->Data)->x;
			_y1 = ((struct evPositionData*)Ptr->Data)->y;
			_z1 = ((struct evPositionData*)Ptr->Data)->z;
			_v0->x = _x1;
			_v0->y = _y1;
			_v0->z = _z1;
			Raziel.Senses.Flags |= 16;
			ControlFlag |= 0x20000;
			break;
		case 0x40020:
			if (CurrentSection == 0)
			{
				G2Anim_SetSpeedAdjustment(&gameTrackerX.playerInstance->anim, Ptr->Data);
			}
			break;
		case 0x800027:
			if (Data != 0)
			{
				ControlFlag |= 8;
				break;
			}
			ControlFlag &= 0xFFFFFFF7;
			break;
		case 0x10002002:
			razMaterialShift();
			break;
		case 0x10002001:
			razSpectralShift();
			break;
		case 0x8000000:
			break;
		case 0x1000001:
			break;
		case 0x80000020:
			break;
		case 0x80000008:
			break;
		case 0x80000010:
			break;
		case 0x80000000:
			break;
		case 0x100009:
			break;
		case 0x4010200:
			break;
		default:
			DefaultStateHandler(In, CurrentSection, Data);
		}
	}
}

void StateHandlerWarpGate(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
{
	struct __Event* Ptr;
	int anim;
	struct _Instance* heldInst;

	anim = G2EmulationQueryAnimation(In, CurrentSection);

	while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
	{
		if (Ptr != NULL)
		{
			switch (Ptr->ID)
			{
			case 0x100001:
				StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));

				ControlFlag = 0x20008;

				Raziel.Mode = 0x80000000;

				PhysicsMode = 3;

				Raziel.idleCount = 0;

				if (CurrentSection == 0)
				{
					Raziel.puppetRotToPoint.z = ((Raziel.Senses.EngagedList[14].instance)->rotation).z;

					SteerSwitchMode(In->CharacterInstance, 13);

					In->CharacterInstance->position = (Raziel.Senses.EngagedList[14].instance)->position;

					razSetPlayerEventHistory(0x800);

					WARPGATE_StartUsingWarpgate();
				}

				break;
			case 0x80000000:
			case 0x2000000:
				if (anim != 123)
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);

					WARPGATE_EndUsingWarpgate();
				}

				break;
			case 0x10000000:
				if ((CurrentSection == 0) && (anim != 123))
				{
					if ((PadData[0] & 4))
					{
						WARPGATE_IncrementIndex();
					}

					if ((PadData[0] & 8))
					{
						WARPGATE_DecrementIndex();
					}

					if ((PadData[0] & 1))
					{
						if (WARPGATE_IsWarpgateUsable() != 0)
						{
							SetTimer(75);

							heldInst = razGetHeldItem();

							if (heldInst != NULL)
							{
								INSTANCE_Post(heldInst, 0x800008, 0);
							}

							G2EmulationSwitchAnimationCharacter(In, 123, 0, 6, 2);

							if (WARPGATE_IsWarpgateSpectral() != 0)
							{
								razSpectralShift();
							}

							break;
						}

						if ((!(Raziel.playerEventHistory & 0x20000)) && (WARPGATE_IsWarpgateReady() != 0))
						{
							LOAD_PlayXA(239);

							razSetPlayerEventHistory(0x20000);
						}
					}
				}

				break;
			case 0x100000:
				if (WARPGATE_IsWarpgateActive() != 0)
				{
					EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100000, 0);
					break;
				}

				StateSwitchStateCharacterData(In, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				break;
			case 0x100015:
				if (CurrentSection == 0)
				{
					EnMessageQueueData(&In->SectionList[0].Defer, 0x100000, 0);

					WARPGATE_EndUsingWarpgate();
				}

				StateInitIdle(In, CurrentSection, SetControlInitIdleData(0, 0, 3));
				break;
			case 0x100004:
				break;
			case 0x80000020:
				break;
			case 0x80000008:
				break;
			case 0x80000010:
				break;
			case 0x40005:
				break;
			case 0x100014:
				break;
			case 0x1000001:
				break;
			case 0x8000000:
				break;
			case 0x10002002:
				break;
			case 0x10002001:
				break;
			default:
				DefaultStateHandler(In, CurrentSection, Data);
			}

			DeMessageQueue(&In->SectionList[CurrentSection].Event);
		}
	}

	if (CurrentSection == 0)
	{
		razApplyMotion(In, 0);
	}
}

void StateHandlerForcedGlide(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 98.77%
{
	struct __Event* Ptr;
	int Anim;
	int extraProcess;

	extraProcess = 0;

	Anim = G2EmulationQueryAnimation(In, CurrentSection);

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

					ControlFlag = 0;

					PhysicsMode = 0;

					SteerSwitchMode(In->CharacterInstance, 0);

					DeInitAlgorithmicWings(In->CharacterInstance);

					SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 4, 0, 24, -24);

					gameTrackerX.wipeType = 11;
					gameTrackerX.wipeTime = -10;
					gameTrackerX.maxWipeTime = 10;

					if (Raziel.forcedGlideSpeed >= -23)
					{
						Raziel.forcedGlideSpeed = -24;
					}
				}

				G2EmulationSwitchAnimation(In, CurrentSection, 16, Ptr->Data, 5, 1);
				break;
			case 0x100004:
				if (CurrentSection == 0)
				{
					InitAlgorithmicWings(In->CharacterInstance);

					gameTrackerX.wipeType = 11;
					gameTrackerX.wipeTime = 10;
					gameTrackerX.maxWipeTime = 10;

					Raziel.forcedGlideSpeed = ~0x17;
				}

				break;
			case 0x100000:
				SetPhysics(In->CharacterInstance, -16, 0, 0, 0);

				if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 24, NULL, NULL) != 0)
				{
					G2EmulationSwitchAnimationCharacter(In, 36, 0, 4, 1);
				}

				extraProcess = 1;

				StateSwitchStateCharacterData(In, &StateHandlerFall, 0);
				break;
			case 0x8000000:
				if (Anim == 16)
				{
					G2EmulationSwitchAnimationAlpha(In, CurrentSection, 17, 0, 5, 1, 4);

					SetExternalTransitionForce((struct __Force*)&ExternalForces, In->CharacterInstance, 4, 0, 0, Raziel.forcedGlideSpeed);
				}

				break;
			case 0x4010008:
				StateSwitchStateData(In, CurrentSection, &StateHandlerDeCompression, 0);
				break;
			case 0x1000001:
				break;
			case 0x80000020:
				break;
			case 0x80000004:
				break;
			case 0x80000000:
				break;
			case 0x80000008:
				break;
			case 0x4020000:
				break;
			case 0x4000007:
				break;
			case 0x4000001:
				break;
			case 0x10000000:
				break;
			case 0x20000001:
				break;
			case 0x20000004:
				break;
			default:
				DefaultStateHandler(In, CurrentSection, Data);
			}

			DeMessageQueue(&In->SectionList[CurrentSection].Event);
		}
	}

	if ((extraProcess == 0) && (CurrentSection == 0) && (!(STREAM_GetLevelWithID(In->CharacterInstance->currentStreamUnitID)->unitFlags & 0x1000)))
	{
		EnMessageQueueData(&In->SectionList[0].Event, 0x100000, 0);
	}
}
