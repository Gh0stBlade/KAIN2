#include "Game/CORE.H"
#include "STEERING.H"
#include "Game/RAZIEL/RAZIEL.H"
#include "Game/MATH3D.H"
#include "Game/RAZIEL/RAZLIB.H"
#include "Game/CAMERA.H"
#include "Game/G2/ANMCTRLR.H"
#include "Game/STATE.H"
#include "Game/GAMEPAD.H"
#include "Game/RAZCNTRL.H"
#include "Game/PSX/COLLIDES.H"

int ZoneDelta;
int LastRC;

int UpdateZoneDelta(int rc, int LastRC)
{
	if (LastRC != 0)
	{
		if (rc == LastRC)
		{
			ZoneDelta -= 4;

			if (ZoneDelta < 16)
			{
				ZoneDelta = 16;
			}
		}
		else
		{
			ZoneDelta = 256;
		}
	}
	
	return ZoneDelta;
}

int GetControllerInput(int* ZDirection, long* controlCommand)
{
	int rc;

	if ((gameTrackerX.playerInstance->flags & 0x100))
	{
		return 0;
	}

	if ((controlCommand[0] & 0x5) == 0x5)
	{
		rc = 0x10000010;

		ZDirection[0] = 2560;

		UpdateZoneDelta(0x10000010, LastRC);
	}
	else
	{
		if ((controlCommand[0] & 0x9) == 0x9)
		{
			rc = 0x10000020;

			ZDirection[0] = 1536;

			UpdateZoneDelta(0x10000020, LastRC);
		}
		else
		{
			if ((controlCommand[0] & 0x6) == 0x6)
			{
				rc = 0x10000040;

				ZDirection[0] = 3584;

				UpdateZoneDelta(0x10000040, LastRC);
			}
			else
			{
				if ((controlCommand[0] & 0xA) == 0xA)
				{
					rc = 0x10000030;

					ZDirection[0] = 512;

					UpdateZoneDelta(0x10000030, LastRC);
				}
				else
				{
					if ((controlCommand[0] & 0x1))
					{
						rc = 0x10000001;

						ZDirection[0] = 2048;

						UpdateZoneDelta(0x10000001, LastRC);
					}
					else
					{
						if ((controlCommand[0] & 0x2))
						{
							rc = 0x10000003;

							ZDirection[0] = 4096;

							UpdateZoneDelta(0x10000003, LastRC);
						}
						else
						{
							if ((controlCommand[0] & 0x8))
							{
								rc = 0x10000002;

								ZDirection[0] = 1024;

								UpdateZoneDelta(0x10000002, LastRC);
							}
							else
							{
								if ((controlCommand[0] & 0x4))
								{
									rc = 0x10000004;

									ZDirection[0] = 3072;

									UpdateZoneDelta(0x10000004, LastRC);
								}
								else
								{
									rc = 0x0;

									ZoneDelta = 16;

									ZDirection[0] = 0;
								}
							}
						}
					}
				}
			}
		}
	}

	LastRC = rc;

	return rc;
}

int DecodeDirection(int Source, int Destination, short* Difference, short* Zone)//Matching - 83.95%
{
	int rc; // $s1
	long diff;///@FIXME not in original, likely macro used.

	rc = 0;

	diff = AngleDiff((short)Destination, (short)Source);

	Zone[0] = diff & 0x1FF;

	if ((unsigned short)(diff & 0x1FF) < 0x3FF)
	{
		Difference[0] = (short)diff;

		rc = 0x10000001;
	}
	else
	{
		if ((unsigned short)(diff - 512) < 0x400)
		{
			Zone[0] = 1024;

			rc = 0x10000004;
		}
		else
		{
			if ((unsigned short)(diff + 0x5FF) < 0x400)
			{
				Zone[0] = -1024;

				rc = 0x10000002;
			}
			else
			{
				if ((short)diff >= 0x600)
				{
					Zone[0] = 2048;

					rc = 0x10000003;
				}
				else
				{
					if (diff < -0x5FF)
					{
						Zone[0] = -2048;

						rc = 0x10000003;
					}
				}
			}
		}
	}

	return rc;
}

int ProcessMovement(struct _Instance* instance, long* controlCommand, struct GameTracker* GT)
{
	int ZDirection; // stack offset -32
	int rc; // $s2
	int lag; // $v1
	short diff; // stack offset -28, -24
	short zone; // stack offset -26, -22
	struct _G2SVector3_Type rot; // stack offset -40
	short angle; // $s0

	//s1 = instance
	//s0 = controlCommand
	//v1 = Raziel.steeringMode
	//v0 = 9

	//a0 = &ZDirection
	if (Raziel.steeringMode == 9 || Raziel.steeringMode == 14 || Raziel.steeringMode == 15)
	{
		rc = 0;
	}
	else
	{
		//loc_800A23BC
		rc = GetControllerInput(&ZDirection, controlCommand);
	}

	//loc_800A23C8
	razZeroAxis(&controlCommand[3], &controlCommand[4],1024);

	if (controlCommand[3] == 128 || controlCommand[3] == -128 || controlCommand[4] == 128 || controlCommand[4] == -128)
	{
		Raziel.Magnitude = 4096;
	}
	else
	{
		//loc_800A2410
		Raziel.Magnitude = MATH3D_veclen2(controlCommand[3], controlCommand[4]) * 32;

		if (Raziel.Magnitude >= 4097)
		{
			Raziel.Magnitude = 4096;
		}

		//loc_800A2430

		if (Raziel.Magnitude != 0 && Raziel.Magnitude < 1024)
		{
			Raziel.Magnitude = 1024;
		}
	}
	//loc_800A244C
	//v0 = Raziel.input
	Raziel.lastInput = Raziel.input;

	Raziel.input = rc;

	//a0 = (controlCommand[4] * 4096) / 96;
	//a1 = ((controlCommand[3] * 4096) / 96) & 0xFFF;

	Raziel.ZDirection = (1024 - (ratan2(((controlCommand[4] * 4096) / 96), ((controlCommand[3] * 4096) / 96))) & 0xFFF);

	lag = theCamera.lagZ;

	if (rc != 0)
	{
		Raziel.LastBearing = Raziel.ZDirection + lag;
	}
	//loc_800A24CC

	Raziel.Bearing = AngleDiff(instance->rotation.z, Raziel.LastBearing);

	switch (Raziel.steeringMode)
	{

	case 0:
	case 16:
	{
		if (rc != 0)
		{
			rc = DecodeDirection(Raziel.Bearing, 0, &diff, &zone);
		}
		//def_800A2504
		break;
	}
	case 1:
	{
		Raziel.steeringVelocity = 256;

		SteerTurn(instance, rc);

		return rc;

		break;
	}
	case 2:
	{
		SteerMove(instance, rc);

		return rc;

		break;
	}
	case 4:
	{
		rot.y = 0;
		rot.x = 0;
		rot.z = Raziel.steeringLockRotation - Raziel.LastBearing;

		instance->rotation.z = Raziel.LastBearing;

		if (rc != 0)
		{
			if (Raziel.Mode == 16)
			{
				instance->yVel = 40;
			}
			else
			{
				instance->yVel = 21;
			}
		}
	
		G2Anim_SetController_Vector(&instance->anim, 1, 0xE, &rot);

		return rc;
	}
	case 5:
	case 9:
	{
		if (rc != 0)
		{
			rc = SteerAutoFace(instance, controlCommand);

			return rc;
		}
		else
		{
			if ((PadData[0] & RazielCommands[7]))
			{
				if ((Raziel.Senses.EngagedMask & 0x40))
				{
					SteerDisableAutoFace(instance);

					Raziel.steeringVelocity = 128;

					AngleMoveToward(&instance->rotation.z, MATH3D_AngleFromPosToPos(&instance->position, &Raziel.Senses.EngagedList[6].instance->position), (short)(gameTrackerX.timeMult >> 5));

				}
			}
		}

		return rc;

		break;
	}
	case 11:
	{
		if (G2Anim_IsControllerActive(&instance->anim, 1, 0xE) == 0)
		{
			G2Anim_EnableController(&instance->anim, 1, 0xE);
		}

		G2EmulationSetInterpController_Vector(instance, 1, 0xE, &Raziel.extraRot, 5, 0);
		
		Raziel.steeringVelocity = 256;
		
		SteerTurn(instance, rc);

		return rc;

		break;
	}
	default:
		break;
	}
#if 0
		loc_800A2578:            # jumptable 800A2504 case 18
		j       loc_800A2584
		li      $v0, 0x40  # '@'

		loc_800A2580:            # jumptable 800A2504 case 8
		li      $v0, 0x60  # '`'

				move    $a0, $s1
				jal     sub_800A28BC
				move    $a1, $s2
				j       loc_800A28A4
				move    $v0, $s2

				


				loc_800A2604 : # jumptable 800A2504 case 14
				lw      $v0, -0x33C($gp)
				lw      $v1, -0x5B60($gp)
				lw      $v0, 0($v0)
				nop
				and $v0, $v1
				beqz    $v0, loc_800A28A4
				move    $v0, $s2
				lw      $v0, -0x5F4($gp)
				nop
				andi    $v0, 0x40
				beqz    $v0, def_800A2504  # jumptable 800A2504 default case, case 3
				addiu   $a0, $s1, 0x5C  # '\'
				lw      $v0, -0x5F8($gp)
				nop
				lw      $a1, 0x30($v0)
				jal     sub_8003A21C
				addiu   $a1, 0x5C  # '\'
				j       def_800A2504     # jumptable 800A2504 default case, case 3
				sh      $v0, 0x78($s1)

				loc_800A2650:            # jumptable 800A2504 case 15
				bnez    $s2, loc_800A26DC
				move    $a0, $s1
				lw      $v0, -0x5F4($gp)
				nop
				andi    $v0, 0x40
				beqz    $v0, loc_800A28A4
				move    $v0, $s2
				jal     sub_800A2DB8
				move    $a0, $s1
				lw      $v0, -0x5F8($gp)
				nop
				lw      $a1, 0x30($v0)
				addiu   $a0, $s1, 0x5C  # '\'
				jal     sub_8003A21C
				addiu   $a1, 0x5C  # '\'
				lw      $v1, -0x5F8($gp)
				move    $a1, $s1
				lw      $a0, 0x30($v1)
				jal     sub_8008019C
				move    $s0, $v0
				andi    $v0, 0xFFF
				addiu   $v0, -0x2AB
				sltiu   $v0, 0xAAB
				bnez    $v0, loc_800A28A4
				move    $v0, $s2
				addiu   $a0, $s1, 0x78  # 'x'
				sll     $a1, $s0, 16
				sra     $a1, 16
				lw      $a2, -0x3FF8($gp)
				li      $v0, 0x80
				sh      $v0, -0x644($gp)
				j       loc_800A281C
				sll     $a2, 11

				

				loc_800A275C:            # jumptable 800A2504 cases 6, 17
				beqz    $s2, loc_800A2770
				addiu   $s0, $s1, 0x1C8
				jal     sub_800A2C94
				move    $a0, $s1
				addiu   $s0, $s1, 0x1C8

				loc_800A2770:
			move    $a0, $s0
				li      $a1, 1
				jal     sub_80090794
				li      $a2, 0xE
				bnez    $v0, loc_800A279C
				move    $a0, $s1
				move    $a0, $s0
				li      $a1, 1
				jal     sub_80090558
				li      $a2, 0xE
				move    $a0, $s1

				loc_800A279C :
			li      $a1, 1
				li      $a2, 0xE
				addiu   $a3, $gp, -0x4C0
				li      $v0, 4
				sw      $v0, 0x30 + var_20($sp)
				li      $v0, 3
				jal     sub_8007299C
				sw      $v0, 0x30 + var_1C($sp)
				j       loc_800A28A4
				move    $v0, $s2

				loc_800A27C4 : # jumptable 800A2504 case 7
				jal     sub_800A2D84
				move    $a0, $s1
				beqz    $s2, def_800A2504  # jumptable 800A2504 default case, case 3
				addiu   $a2, $sp, 0x30 + var_8
				lh      $a0, Raziel.Bearing
				move    $a1, $zero
				addiu   $a3, $sp, 0x30 + var_6

				loc_800A27E0 :
			jal     sub_800A2288
				nop
				j       def_800A2504     # jumptable 800A2504 default case, case 3
				move    $s2, $v0

				loc_800A27F0 : # jumptable 800A2504 case 10
				lw      $a1, -0x570($gp)
				addiu   $a0, $s1, 0x5C  # '\'
				jal     sub_8003A21C
				addiu   $a1, 0x5C  # '\'
				addiu   $a0, $s1, 0x78  # 'x'
				sll     $v0, 16
				sra     $a1, $v0, 16
				lw      $a2, -0x3FF8($gp)
				li      $v0, 0x100
				sh      $v0, -0x644($gp)
				sll     $a2, 12

				loc_800A281C:
			jal     sub_8003A14C
				sra     $a2, 16
				j       loc_800A28A4
				move    $v0, $s2

				loc_800A282C : # jumptable 800A2504 case 12
				addiu   $a1, $gp, -0x47E
				jal     sub_8003A21C
				addiu   $a0, $s1, 0x5C  # '\'
				addiu   $a0, $s1, 0x78  # 'x'
				sll     $s0, $v0, 16
				sra     $s0, 16
				lw      $a2, -0x3FF8($gp)
				li      $v0, 0x100
				sh      $v0, -0x644($gp)
				j       loc_800A2878
				move    $a1, $s0

				loc_800A2858 : # jumptable 800A2504 case 13
				addiu   $a0, $s1, 0x78  # 'x'
				lhu     $s0, -0x47A($gp)
				lw      $a2, -0x3FF8($gp)
				li      $v0, 0x100
				sh      $v0, -0x644($gp)
				sll     $s0, 16
				sra     $s0, 16
				move    $a1, $s0

				loc_800A2878 :
			sll     $a2, 12
				jal     sub_8003A14C
				sra     $a2, 16
				lh      $v0, 0x78($s1)
				nop
				bne     $s0, $v0, loc_800A28A4
				move    $v0, $s2
				move    $a0, $s1
				jal     sub_800A2E4C
				move    $a1, $zero

				def_800A2504 : # jumptable 800A2504 default case, case 3
				move    $v0, $s2

				loc_800A28A4 :
			lw      $ra, 0x30 + var_sC($sp)
				lw      $s2, 0x30 + var_s8($sp)
				lw      $s1, 0x30 + var_s4($sp)
				lw      $s0, 0x30 + var_s0($sp)
				jr      $ra
				addiu   $sp, 0x40
#endif

	return rc;
}

void SteerTurn(struct _Instance* instance, int rc)//Matching - 96.08%
{
	int rot;

	if (rc != 0 || Raziel.Bearing != 0)
	{
		if (Raziel.Bearing == 2048)
		{
			rot = instance->rotation.z - Raziel.LastBearing;

			if (rot <= 0)
			{
				rot = Raziel.LastBearing - 1;
			}
			else
			{
				rot = Raziel.LastBearing + 1;
			}

			Raziel.LastBearing = rot;
		}

		AngleMoveToward(&instance->rotation.z, Raziel.LastBearing, (short)((Raziel.steeringVelocity * gameTrackerX.timeMult) >> 12));
	}
	else
	{
		Raziel.steeringVelocity = 0;
	}
}

void SteerMove(struct _Instance* instance, int rc)
{
	if ((rc != 0) || (Raziel.Bearing != 0))
	{
		if (Raziel.Bearing < 0)
		{
			Raziel.steeringVelocity = -Raziel.Bearing;
		}
		else
		{
			Raziel.steeringVelocity = Raziel.Bearing;
		}

		if (Raziel.steeringVelocity >= 0x301)
		{
			Raziel.steeringVelocity = Raziel.steeringVelocity < 0 ? (((Raziel.steeringVelocity + 3) >> 2) + 0x70) : ((Raziel.steeringVelocity >> 2) + 0x7);
		}
		else
		{
			Raziel.steeringVelocity = 0x70;
		}

		AngleMoveToward(&instance->rotation.z, Raziel.LastBearing, (short)((Raziel.steeringVelocity * gameTrackerX.timeMult) >> 12));
	}
	else
	{
		Raziel.steeringVelocity = 0;
	}
}

int SteerAutoFace(struct _Instance* instance, long* controlCommand) { // Matching 99.21%
	short angle;
	int rc;
	struct _Instance* target;
	struct _G2SVector3_Type autoFaceRot;
	int diff;
	int predict;

	predict = NULL;
	angle = MATH3D_AngleFromPosToPos(&instance->position, &Raziel.Senses.EngagedList[6].instance->position);
	Raziel.autoFaceTrueAngle = angle;
	Raziel.autoFaceLastAnim = Raziel.autoFaceAnim;
	diff = AngleDiff(angle, Raziel.LastBearing);
	if (((unsigned int)diff + 383 < 767) != NULL)
	{
		predict = 1;
		Raziel.autoFaceAnim = NULL;
		Raziel.autoFaceRootAngle = NULL;
	}
	if (((unsigned int)diff + 1535 < 1152) != NULL)
	{
		predict = 2;
		Raziel.autoFaceAnim = 1;
		Raziel.autoFaceRootAngle = 1024;
	}
	if (((unsigned int)diff - 384 < 1152) != NULL)
	{
		predict = 4;
		Raziel.autoFaceAnim = 3;
		Raziel.autoFaceRootAngle = -1024;
	}
	if (((unsigned int)diff + 1535 < 3071) == NULL)
	{
		predict = 3;
		Raziel.autoFaceAnim = 2;
		Raziel.autoFaceRootAngle = 2048;
	}
	angle = Raziel.LastBearing;
	Raziel.steeringVelocity = 64;
	AngleMoveToward(&instance->rotation.z, angle, (short) gameTrackerX.timeMult >> 6);

	Raziel.autoFaceLastAnim = predict - 1;
	Raziel.autoFaceAngle = angle;
	rc = predict | 0x10001000;
	if (Raziel.autoFaceLastAnim != Raziel.autoFaceAnim)
	{

		instance->rotation.z = Raziel.LastBearing;
		autoFaceRot.x = 0;
		autoFaceRot.y = 0;
		autoFaceRot.z = Raziel.autoFaceRootAngle;
		if (G2Anim_IsControllerActive(&instance->anim, 1, 10) == G2FALSE)
		{
			G2Anim_EnableController(&instance->anim, 1, 10);
		}
		if (Raziel.input == Raziel.lastInput)
		{
			G2EmulationSetInterpController_Vector(instance, 1, 10, &autoFaceRot, 4, 3);
		}
		else
		{
			G2Anim_SetController_Vector(&instance->anim, 1, 10, &autoFaceRot);
		}
	}

	diff = AngleDiff(instance->rotation.z, Raziel.autoFaceTrueAngle);
	if (G2Anim_IsControllerInterpolating(&instance->anim, 1, 10) != G2FALSE)
	{
		if (G2Anim_IsControllerInterpolating(&instance->anim, 14, 14) == G2FALSE)
		{
			autoFaceRot.x = 0;
			autoFaceRot.y = 0;
			autoFaceRot.z = diff - Raziel.autoFaceRootAngle;
			G2EmulationSetInterpController_Vector(instance, 14, 14, &autoFaceRot, 4, 3);
		}
	}
	else
	{
		autoFaceRot.x = 0;
		autoFaceRot.y = 0;
		autoFaceRot.z = diff - Raziel.autoFaceRootAngle;
		if (G2Anim_IsControllerActive(&instance->anim, 14, 14) == G2FALSE)
		{
			G2Anim_EnableController(&instance->anim, 14, 14);
		}
		G2Anim_SetController_Vector(&instance->anim, 14, 14, &autoFaceRot);
	}
	return rc;
}

void SteerSwim(struct _Instance* instance) // Matching - 98.58%
{
	int step;
	int velocity;
	int temp;
	int temp1;

	temp = 0x20;
	temp1 = rsin(Raziel.ZDirection);
	step = (gameTrackerX.timeMult * temp) / 0x1000;
	instance->rotation.z -= (int)(step * temp1) / 0x1000;
	temp = 0x30;
	temp1 = rcos(Raziel.ZDirection);
	step = (gameTrackerX.timeMult * temp) / 0x1000;
	if ((Raziel.steeringMode != 0x11) || (temp1 < 0))
	{
		Raziel.extraRot.x -= (int)(step * temp1) / 0x1000;
	}
	if (Raziel.extraRot.x > 2048)
	{
		Raziel.extraRot.x = 2048;
	}
	if (Raziel.extraRot.x < 0)
	{
		Raziel.extraRot.x = 0;
	}
	Raziel.extraRot.y = 0;
	Raziel.extraRot.z = 0;
}

void SteerWallcrawling(struct _Instance* instance)
{
	Raziel.Bearing = AngleDiff(ExtraRot->y - 2048, Raziel.ZDirection);
}

void SteerDisableAutoFace(struct _Instance* instance) { // Matching 99.59%
	if (G2Anim_IsControllerActive(&instance->anim, 1, 10) != NULL) {
		G2Anim_DisableController(&instance->anim, 1, 10);
	}
	if (G2Anim_IsControllerActive(&instance->anim, 14, 14) != NULL) {
		G2Anim_DisableController(&instance->anim, 14, 14);
	}
	instance->rotation.z = instance->rotation.z + Raziel.autoFaceRootAngle;
	Raziel.autoFaceRootAngle = NULL;
	Raziel.autoFaceAnim = -1;
}

void SteerSwitchMode(struct _Instance* instance, int mode)
{
	int rotx;

	switch (Raziel.steeringMode)
	{
	case 0:
	case 1:
	case 2:
	case 4:
	case 8:
	case 18:
	{
		if (G2Anim_IsControllerActive(&instance->anim, 1, 0xE) != 0)
		{
			G2Anim_DisableController(&instance->anim, 1, 0xE);
		
			instance->rotation.z = Raziel.steeringLockRotation;

			Raziel.LastBearing = Raziel.steeringLockRotation;
		}
		break;
	}

	case 5:
	case 9:
	case 15:
	{
		SteerDisableAutoFace(instance);
		break;
	}
	case 6:
	case 17:
	{
		if (mode != 6 && mode != 11 && mode != 16 && mode != 17)
		{
			if ((G2Anim_IsControllerActive(&instance->anim, 1, 0xE)))
			{
				G2Anim_InterpDisableController(&instance->anim, 1, 0xE, 600);
			}

			Raziel.extraRot.x = 0;
		}
	}
	case 7:
	{
		if (mode != Raziel.steeringMode)
		{
			razDeinitWallCrawlSteering(instance);
		}
		break;
	}
	case 10:
	{
		Raziel.HitPoints = 0;
		break;
	}
	case 11:
	{
		CAMERA_EndSwimThrowMode(&theCamera);

		CAMERA_SetLookRot(&theCamera, 0, 0);

		Raziel.extraRot.x = Raziel.throwReturnRot;

		break;
	}
	}

	Raziel.RotationSegment = 0;

	switch (mode)
	{
	case 0:
	{
		Raziel.extraRot.z = 0;
		Raziel.extraRot.x = 0;

		Raziel.RotationSegment = 0;

		break;
	}
	case 1:
	case 2:
	case 4:
	case 5:
	case 8:
	case 9:
	case 10:
	case 15:
	case 18:
	{
		Raziel.RotationSegment = 0;
		break;
	}
	case 11:
	{
		rotx = (4096 - theCamera.core.rotation.x) & 0xFFF;

		Raziel.throwReturnRot = Raziel.extraRot.x;

		if (rotx < -1025)
		{
			rotx = 1024;
		}
		else if ((rotx - 2049) < 1023)
		{
			rotx = 3072;
		}

		Raziel.extraRot.x = rotx;

		CAMERA_StartSwimThrowMode(&theCamera);

		CAMERA_SetLookRot(&theCamera, 4096 - Raziel.extraRot.x, 0);
		break;
	}
	case 6:
	case 17:
	{
		Raziel.RotationSegment = 1;
		break;
	}

	case 7:
	{
		if (mode != Raziel.steeringMode)
		{
			razInitWallCrawlSteering(instance);
		}

		break;
	}
	case 14:
	{
		instance->rotation.z = MATH3D_AngleFromPosToPos(&instance->position, &Raziel.Senses.EngagedList[6].instance->position);

		break;
	}
	}
	Raziel.steeringMode = mode;
}

void razInitWallCrawlSteering(struct _Instance* instance) // Matching - 98.48%
{
	struct _G2SVector3_Type vec;

	G2Anim_EnableController(&instance->anim, 1, 38);
	vec.x = 0;
	vec.y = 0;
	vec.z = -318;
	instance->position.z += 318;
	instance->oldPos.z += 318;
	instance->matrix->t[2] += 318;
	instance->oldMatrix->t[2] += 318;
	G2Anim_SetController_Vector(&instance->anim, 1, 38, &vec);
	G2Anim_EnableController(&instance->anim, 0, 14);
	vec.x = instance->rotation.x;
	vec.y = instance->rotation.y;
	vec.z = instance->rotation.z;
	G2Anim_EnableController(&instance->anim, 0, 8);
	G2Anim_SetControllerAngleOrder(&instance->anim, 0, 8, 1);
	G2Anim_SetController_Vector(&instance->anim, 0, 8, &vec);
	G2Anim_EnableController(&instance->anim, 14, 14);
	G2Anim_EnableController(&instance->anim, 50, 76);
	G2Anim_EnableController(&instance->anim, 58, 76);
	ExtraRot = &ExtraRotData;
	ExtraRot->z = 0;
	ExtraRot->y = 0;
	ExtraRot->x = 0;
}

void razDeinitWallCrawlSteering(struct _Instance* instance) { // Matching - 100%
	G2Anim_DisableController(&instance->anim, 1, 38);
	instance->position.z -= 318;
	instance->oldPos.z -= 318;
	instance->matrix->t[2] -= 318;
	instance->oldMatrix->t[2] -= 318;
	G2Anim_InterpDisableController(&instance->anim, 0, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 0, 8, 300);
	G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 50, 76, 300);
	G2Anim_InterpDisableController(&instance->anim, 58, 76, 300);
}
