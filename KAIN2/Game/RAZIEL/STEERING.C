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

int DecodeDirection(int Source, int Destination, short* Difference, short* Zone)
{
	int rc; // $s1
	long diff;///@FIXME not in original, likely macro used.

	rc = 0;
	
	diff = AngleDiff(Destination, Source);

	if ((diff & 0x1FF) < 0x3FF)
	{
		Difference[0] = diff;

		Zone[0] = 0;

		rc = 0x10000001;
	}
	else
	{
		if ((diff - 512) < 0x400)
		{
			Zone[0] = 1024;

			rc = 0x10000004;
		}
		else
		{
			if ((diff + 0x5FF) < 0x400)
			{
				Zone[0] = -1024;

				rc = 0x10000002;
			}
			else
			{
				if (diff >= 0x600)
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

	Raziel.ZDirection = (1024 - ratan2((controlCommand[4] * 4096) / 96, ((controlCommand[3] * 4096) / 96) & 0xFFF)) & 0xFFF;

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

					AngleMoveToward(&instance->rotation.z, MATH3D_AngleFromPosToPos(&instance->position, &Raziel.Senses.EngagedList[6].instance->position), gameTrackerX.timeMult >> 5);

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


// autogenerated function stub: 
// void /*$ra*/ SteerTurn(struct _Instance *instance /*$a0*/, int rc /*$a1*/)
void SteerTurn(struct _Instance *instance, int rc)
{ // line 487, offset 0x800a298c
	/* begin block 1 */
		// Start line: 488
		// Start offset: 0x800A298C

		/* begin block 1.1 */
			// Start line: 494
			// Start offset: 0x800A29B8
			// Variables:
				int rot; // $v0
		/* end block 1.1 */
		// End offset: 0x800A29DC
		// End Line: 506
	/* end block 1 */
	// End offset: 0x800A2A10
	// End Line: 511

	/* begin block 2 */
		// Start line: 974
	/* end block 2 */
	// End Line: 975
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ SteerMove(struct _Instance *instance /*$a0*/, int rc /*$a1*/)
void SteerMove(struct _Instance *instance, int rc)
{ // line 550, offset 0x800a2a20
	/* begin block 1 */
		// Start line: 1100
	/* end block 1 */
	// End Line: 1101
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// int /*$ra*/ SteerAutoFace(struct _Instance *instance /*$s3*/, long *controlCommand /*$a1*/)
int SteerAutoFace(struct _Instance *instance, long *controlCommand)
{ // line 589, offset 0x800a2ad0
	/* begin block 1 */
		// Start line: 590
		// Start offset: 0x800A2AD0
		// Variables:
			short angle; // $s0
			int rc; // $s2
			struct _Instance *target; // $a1
			struct _G2SVector3_Type autoFaceRot; // stack offset -32
			int diff; // $s1
			int predict; // $s2
	/* end block 1 */
	// End offset: 0x800A2D44
	// End Line: 675

	/* begin block 2 */
		// Start line: 1138
	/* end block 2 */
	// End Line: 1139

	/* begin block 3 */
		// Start line: 1148
	/* end block 3 */
	// End Line: 1149
			UNIMPLEMENTED();
	return 0;
}


// autogenerated function stub: 
// void /*$ra*/ SteerSwim(struct _Instance *instance /*$s0*/)
void SteerSwim(struct _Instance *instance)
{ // line 681, offset 0x800a2d64
	/* begin block 1 */
		// Start line: 682
		// Start offset: 0x800A2D64
		// Variables:
			int step; // $a0
			int velocity; // $a1
	/* end block 1 */
	// End offset: 0x800A2E3C
	// End Line: 731

	/* begin block 2 */
		// Start line: 1371
	/* end block 2 */
	// End Line: 1372
			UNIMPLEMENTED();
}

void SteerWallcrawling(struct _Instance* instance)
{
	Raziel.Bearing = AngleDiff(ExtraRot->y - 2048, Raziel.ZDirection);
}


// autogenerated function stub: 
// void /*$ra*/ SteerDisableAutoFace(struct _Instance *instance /*$s1*/)
void SteerDisableAutoFace(struct _Instance *instance)
{ // line 748, offset 0x800a2e88
	/* begin block 1 */
		// Start line: 1549
	/* end block 1 */
	// End Line: 1550
	UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ SteerSwitchMode(struct _Instance *instance /*$s1*/, int mode /*$s2*/)
void SteerSwitchMode(struct _Instance *instance, int mode)
{ // line 776, offset 0x800a2f1c
	/* begin block 1 */
		// Start line: 777
		// Start offset: 0x800A2F1C

		/* begin block 1.1 */
			// Start line: 846
			// Start offset: 0x800A30A8
			// Variables:
				int rotx; // $v1
		/* end block 1.1 */
		// End offset: 0x800A3110
		// End Line: 856
	/* end block 1 */
	// End offset: 0x800A315C
	// End Line: 883

	/* begin block 2 */
		// Start line: 1607
	/* end block 2 */
	// End Line: 1608

	/* begin block 3 */
		// Start line: 1611
	/* end block 3 */
	// End Line: 1612
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ razInitWallCrawlSteering(struct _Instance *instance /*$s0*/)
void razInitWallCrawlSteering(struct _Instance *instance)
{ // line 888, offset 0x800a3178
#if defined(PC_VERSION)
	MATRIX* matrix; // eax
	__int16 y; // dx
	__int16 z; // ax
	struct _G2SVector3_Type vector; // [esp+Ch] [ebp-8h] BYREF

	G2Anim_EnableController(&instance->anim, 1, 38);
	matrix = instance->matrix;
	vector.x = 0;
	instance->position.z += 318;
	instance->oldPos.z += 318;
	vector.y = 0;
	vector.z = -318;
	matrix->t[2] += 318;
	instance->oldMatrix->t[2] += 318;
	G2Anim_SetController_Vector(&instance->anim, 1, 38, &vector);
	G2Anim_EnableController(&instance->anim, 0, 14);
	y = instance->rotation.y;
	z = instance->rotation.z;
	vector.x = instance->rotation.x;
	vector.y = y;
	vector.z = z;
	G2Anim_EnableController(&instance->anim, 0, 8);
	G2Anim_SetControllerAngleOrder(&instance->anim, 0, 8, 1);
	G2Anim_SetController_Vector(&instance->anim, 0, 8, &vector);
	G2Anim_EnableController(&instance->anim, 14, 14);
	G2Anim_EnableController(&instance->anim, 50, 76);
	G2Anim_EnableController(&instance->anim, 58, 76);
	dword_B08AB0 = &stru_B08AB8;
	stru_B08AB8.z = 0;
	stru_B08AB8.y = 0;
	stru_B08AB8.x = 0;
#else
	UNIMPLEMENTED();
#endif
}


// autogenerated function stub: 
// void /*$ra*/ razDeinitWallCrawlSteering(struct _Instance *instance /*$s0*/)
void razDeinitWallCrawlSteering(struct _Instance *instance)
{ // line 922, offset 0x800a32b4
#if defined(PC_VERSION)
	MATRIX* matrix; // eax

	G2Anim_DisableController(&instance->anim, 1, 38);
	matrix = instance->matrix;
	instance->position.z -= 318;
	instance->oldPos.z -= 318;
	matrix->t[2] -= 318;
	instance->oldMatrix->t[2] -= 318;
	G2Anim_InterpDisableController(&instance->anim, 0, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 0, 8, 300);
	G2Anim_InterpDisableController(&instance->anim, 14, 14, 300);
	G2Anim_InterpDisableController(&instance->anim, 50, 76, 300);
	G2Anim_InterpDisableController(&instance->anim, 58, 76, 300);
#else
	UNIMPLEMENTED();
#endif
}




