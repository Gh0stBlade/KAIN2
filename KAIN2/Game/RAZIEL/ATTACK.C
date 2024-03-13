#include "Game/CORE.H"
#include "ATTACK.H"
#include <Game/STATE.H>
#include "RAZIEL.H"
#include "CONTROL.H"
#include "STEERING.H"
#include <Game/FX.H>
#include <Game/REAVER.H>
#include <Game/GAMEPAD.H>
#include <Game/MATH3D.H>
#include <Game/G2/ANMCTRLR.H>
#include <Game/CAMERA.H>

int StateHandlerDecodeHold(int* Message, int* Data)  // Matching - 99.28%
{
    int rc;
    int WhoAmI;
    struct _Instance* heldInstance;
    int hitState;
    struct _Instance* heldWeapon;

    rc = 1;
    WhoAmI = 0;
    heldInstance = razGetHeldWeapon();
    if (Raziel.CurrentPlane == 2)
    {
        *Data = rc;
        *Message = ~0x7FFFFFFF;
        if (!(Raziel.Senses.EngagedMask & 0x200))
        {
            rc = 0;
            if (Raziel.Senses.heldClass == 0x1000 && Raziel.Abilities & 4)
            {
                rc = 1;
                *Message = 0x800010;
            }
        }
    }
    else
    {
        if (Raziel.Senses.EngagedMask & 0x200)
        {
            WhoAmI = INSTANCE_Query(Raziel.Senses.EngagedList[9].instance, 1);
        }
        if (WhoAmI & 8)
        {
            *Data = rc;
            if (Raziel.Senses.heldClass == 0x1000)
            {
                *Message = 0x1000023;
            }
            else
            {
                *Message = ~0x7FFFFFFF;
            }
        }
        else if (Raziel.Senses.EngagedMask & 0x200 && heldInstance != NULL && Raziel.Senses.heldClass != 3 && Raziel.Senses.heldClass != 8)
        {
            hitState = INSTANCE_Query(Raziel.Senses.EngagedList[9].instance, 0);
            if (hitState & 0x2000000)
            {
                if (heldInstance == Raziel.soulReaver)
                {
                    *Message = 0x1000023;
                    *Data = rc;
                }
                else
                {
                    rc = 0;
                }
            }
            else
            {
                if (hitState & 0x10000000)
                {
                    *Data = rc;
                    *Message = 0x100000A;
                }
                else
                {
                    *Data = 0;
                    *Message = 0x100000A;
                }
                if (heldInstance == Raziel.soulReaver)
                {
                    *Message = 0x1000023;
                    if (hitState & 0x21000000)
                    {
                        *Data = 1;
                    }
                    else
                    {
                        *Data = 0;
                    }
                }
                else if ((INSTANCE_Query(heldInstance, 2) & 32))
                {
                    *Message = 0x1000018;
                    if ((*Data != 0))
                    {
                        if ((INSTANCE_Query(heldInstance, 3) & 0x10000))
                        {
                            *Data = 1;
                        }
                        else
                        {
                            *Data = 0;
                        }
                    }
                }
            }
        }
        else if (Raziel.Senses.EngagedMask & 0x100 && heldInstance == NULL)
        {
            if ((INSTANCE_Query(Raziel.Senses.EngagedList[8].instance, 0)) < 0)
            {
                *Data = 1;
                *Message = 0x1000002;
            }
            else
            {
                *Data = 0;
                *Message = 0x1000002;
            }
        }
        else
        {
            if (Raziel.Senses.EngagedMask & 0x200 && Raziel.Senses.heldClass == 3)
            {
                *Message = 0x80000000;
                *Data = 1;
                return 1;
            }
            heldWeapon = razGetHeldWeapon();
            if (heldWeapon != NULL)
            {
                if (heldWeapon != Raziel.soulReaver || Raziel.Abilities & 4)
                {
                    *Message = 0x800010;
                }
                else
                {
                    rc = 0;
                }
            }
            else if (Raziel.Abilities & 4)
            {
                *Message = 0x80000;
            }
            else
            {
                rc = 0;
            }
        }
        if (Raziel.Senses.heldClass != 3)
        {
            return rc;
        }
        if (*Message != 0x800010)
        {
            return 0;
        }
    }
    return rc;
}


void StateHandlerAttack2(struct __CharacterState* In, int CurrentSection, int Data)  // Matching - 99.28%
{
    struct __Event* Ptr;
    int message;
    int messageData;
    int Frame;
    int Anim;
    int ignoreHit;
    struct _Instance* ignoreInst;
    struct _Instance* inst;
    struct _Instance* weaponInst;
    struct evMonsterHitData* data;
    struct evMonsterHitData* hitData;
    struct _G2AnimSection_Type* animSection;

    Frame = G2EmulationQueryFrame(In, CurrentSection);
    Anim = G2EmulationQueryAnimation(In, CurrentSection);
    ignoreHit = 0;
    ignoreInst = NULL;
    if (CurrentSection == 0)
    {
        if ((ControlFlag & 32) == 0)
        {
            if (((ControlFlag & 0x10000000) != 0) && (Raziel.Mode & 2) == 0)
            {
                if ((PadData[0] & 0x8000000F) != 0)
                {
                    SteerSwitchMode(In->CharacterInstance, 2);
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 0);
                }
            }
        }
        else
        {
            SteerSwitchMode(In->CharacterInstance, 0);
        }
    }
    while (Ptr = PeekMessageQueue(&In->SectionList[CurrentSection].Event))
    {
        switch (Ptr->ID)
        {
        case 0x100001:
            Raziel.currentAttack = Ptr->Data;
            In->SectionList[CurrentSection].Data2 = 0;
            if (razGetHeldWeapon() != NULL)
            {
                switch (Raziel.Senses.heldClass)
                {
                case 1:
                    Raziel.currentAttack = Ptr->Data + 1;
                    break;
                case 2:
                    Raziel.currentAttack = Ptr->Data + 2;
                    break;
                case 4096:
                    Raziel.currentAttack = Ptr->Data + 3;
                    break;
                case 3:
                    Raziel.currentAttack = Ptr->Data + 4;
                    break;
                }
            }
            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];
            if (Ptr->Data >= 10)
            {
                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationAlpha(In, 1, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);
                }
            }
            else
            {
                G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);
            }
            if (CurrentSection == 1)
            {
                unsigned long startColor;
                unsigned long endColor;

                startColor = Raziel.attack->ribbonStartColor;
                endColor = Raziel.attack->ribbonEndColor;
                if (razGetHeldWeapon() != NULL)
                {
                    inst = razGetHeldWeapon();
                    if ((INSTANCE_Query(inst, 2) & 32) != 0)
                    {
                        if ((INSTANCE_Query(inst, 3) & 0x10000) != 0)
                        {
                            startColor = PlayerData->nonBurningRibbonStartColor;
                            endColor = PlayerData->nonBurningRibbonEndColor;
                        }
                    }
                    else if (Raziel.Senses.heldClass == 4096)
                    {
                        startColor = REAVER_GetGlowColor(inst);
                        endColor = 0;
                    }
                }
                else
                {
                    inst = In->CharacterInstance;
                }
                FX_StartRibbon(inst, Raziel.attack->ribbonStartSegment, Raziel.attack->ribbonEndSegment, 0, Raziel.attack->ribbonLifeTime, Raziel.attack->ribbonFaceLifeTime, (short)Raziel.attack->ribbonStartOpacity, startColor, endColor);
                if (Ptr->Data >= 10)
                {
                    if ((Raziel.Senses.EngagedMask & 64) != 0)
                    {
                        SteerSwitchMode(In->CharacterInstance, 15);
                        SetTimer(4);
                    }
                }
                else
                {
                    SteerSwitchMode(In->CharacterInstance, 9);
                }
                ControlFlag |= 0x2000;
                if (Ptr->Data == 5)
                {
                    SetTimer(1);
                }
            }
            Raziel.attackLastHit = NULL;
            Raziel.attackCurrentHit = NULL;
            Raziel.glowEffect = NULL;
            Raziel.Mode |= 0x200000;
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
        case 0x100015:
            if ((Raziel.steeringMode == 15) && (PadData[0] & 0x8000000F) != 0)
            {
                SteerSwitchMode(In->CharacterInstance, 2);
            }
            else
            {
                SteerSwitchMode(In->CharacterInstance, 0);
            }
            break;
        case 0x2000000:
        case 0x80000000:
            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];
            if ((Anim == Raziel.attack->anim) && (Frame >= Raziel.attack->ignoreDelay))
            {
                Raziel.attackFlags |= 4;
            }
            if (CurrentSection == 1)
            {
                if (Raziel.glowEffect != NULL)
                {
                    FX_StopGlowEffect(Raziel.glowEffect, 0);
                    Raziel.glowEffect = NULL;
                }
            }
            break;
        case 0x4020000:
        case 0x1000001:
            break;
        case 0x100004:
            if (CurrentSection == 1)
            {
                DisableWristCollision(In->CharacterInstance, 2);
                DisableWristCollision(In->CharacterInstance, 1);
                Raziel.dropOffHeight = 256;
                Raziel.fallZVelocity = -96;
                weaponInst = razGetHeldWeapon();
                if (weaponInst != NULL)
                {
                    INSTANCE_Post(weaponInst, 0x200005, 0);
                    INSTANCE_Post(weaponInst, 0x200003, 7);
                }
            }
            ControlFlag &= 0xFF7FFFFF;
            break;
        case 0x8000000:
            if (((Raziel.attackFlags & 4) != 0) && (PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2 + 1] != NULL))
            {
                if (Raziel.currentAttack >= 10)
                {
                    if ((PadData[0] & 0x8000000F) != 0)
                    {
                        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
                    }
                }
                else
                {
                    EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
                }
            }
            else if (Raziel.currentAttack >= 10)
            {
                if ((PadData[0] & 0x8000000F) != 0)
                {
                    StateSwitchStateData(In, CurrentSection, &StateHandlerMove, 0);
                }
                else
                {
                    StateSwitchStateCharacterData(In, &StateHandlerStopMove, 60);
                }
            }
            else if ((Raziel.nothingCounter < 7) && ((Raziel.Senses.EngagedMask & 64) != 0))
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerAutoFace, 0);
            }
            else
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(1, 0, Raziel.attack->framesOut));
            }
            break;
        case 0x80000020:
            if ((StateHandlerDecodeHold(&message, &messageData)) != 0)
            {
                EnMessageQueueData(&In->SectionList[CurrentSection].Event, message, messageData);
                ControlFlag |= 0x800000;
            }
            break;
        case 0x100000:
            In->SectionList[CurrentSection].Data2++;
            if (PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2] == NULL)
            {
                In->SectionList[CurrentSection].Data2 = 1;
            }
            Raziel.attack = PlayerData->attackList[Raziel.currentAttack][In->SectionList[CurrentSection].Data2];
            G2EmulationSwitchAnimationAlpha(In, CurrentSection, Raziel.attack->anim, 0, Raziel.attack->framesIn, 1, Raziel.attack->alphaIn);
            if (CurrentSection == 1)
            {
                unsigned long startColor;
                unsigned long endColor;

                startColor = (Raziel.attack)->ribbonStartColor;
                endColor = (Raziel.attack)->ribbonEndColor;
                if (razGetHeldWeapon() != NULL)
                {
                    weaponInst = razGetHeldWeapon();
                    if ((INSTANCE_Query(weaponInst, 2) & 32) != 0)
                    {
                        if ((INSTANCE_Query(weaponInst, 3) & 0x10000) != 0)
                        {
                            startColor = PlayerData->nonBurningRibbonStartColor;
                            endColor = PlayerData->nonBurningRibbonEndColor;
                        }
                    }
                    else if (Raziel.Senses.heldClass == 4096)
                    {
                        startColor = REAVER_GetGlowColor(weaponInst);
                        endColor = 0;
                    }
                }
                else
                {
                    weaponInst = In->CharacterInstance;
                }
                Raziel.attackLastHit = Raziel.attackCurrentHit;
                Raziel.attackCurrentHit = NULL;
                FX_StartRibbon(weaponInst, Raziel.attack->ribbonStartSegment, Raziel.attack->ribbonEndSegment, 0, Raziel.attack->ribbonLifeTime, Raziel.attack->ribbonFaceLifeTime, (short)Raziel.attack->ribbonStartOpacity, startColor, endColor);
            }
            EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x100002, 0);
            PurgeMessageQueue(&In->SectionList[CurrentSection].Event);
            break;
        case 0x100002:
            Raziel.attackFlags = 0;
            break;
        case 0x1000002:
            if (Raziel.CurrentPlane == 1)
            {
                StateSwitchStateData(In, CurrentSection, &StateHandlerGrab, messageData);
            }
            break;
        case 0x100000A:
            if (Ptr->Data != 0)
            {
                razSetPlayerEventHistory(128);
                ControlFlag |= 0x40000;
                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, 2);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 59, 0, 3, 1);
                }
                StateSwitchStateData(In, CurrentSection, &StateHandlerCannedReaction, 0);
                if ((CurrentSection == 0) && (Raziel.Senses.EngagedMask & 512) != 0)
                {
                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 520, 0, 3, 0);
                }
            }
            else
            {
                ControlFlag |= 32;
                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationCharacter(In, 72, 0, 3, CurrentSection);
                    if ((Raziel.Senses.EngagedMask & 512) != 0)
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, 0);
                    }
                }
            }
            break;
        case 0x8000004:
            if ((Raziel.Senses.EngagedMask & 512) != 0)
            {
                struct _Instance* Inst;
                Inst = Raziel.Senses.EngagedList[9].instance;
                if (Raziel.Senses.heldClass == 4096)
                {
                    if (Raziel.currentSoulReaver == 6)
                    {
                        INSTANCE_Post(Inst, 0x100000C, 32);
                    }
                    else
                    {
                        INSTANCE_Post(Inst, 0x1000023, 4096);
                    }
                }
                else
                {
                    INSTANCE_Post(Inst, 0x100000C, 32);
                    Inst = razGetHeldWeapon();
                    if (Inst != NULL)
                    {
                        INSTANCE_Post(Inst, 0x800029, 0);
                    }
                }
            }
            break;
        case 0x1000018:
            if (Ptr->Data != 0)
            {
                razSetPlayerEventHistory(256);
                ControlFlag |= 0x40000;
                if (CurrentSection == 2)
                {
                    G2EmulationSwitchAnimation(In, 2, 0, 0, 3, CurrentSection);
                }
                else
                {
                    G2EmulationSwitchAnimation(In, CurrentSection, 138, 0, 0, 1);
                }
                if (((CurrentSection == 1) && (Raziel.Senses.EngagedMask & 512) != 0))
                {
                    Raziel.alarmTable = 900;
                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 486, 0, 20, 0);
                    In->CharacterInstance->anim.section[CurrentSection].swAlarmTable = &Raziel.alarmTable;
                }
            }
            else
            {
                ControlFlag |= 32;
                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationCharacter(In, 138, 0, 3, CurrentSection);
                    if ((Raziel.Senses.EngagedMask & 512) != 0)
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, 0);
                    }
                }
            }
            break;
        case 0x1000023:
            if (Ptr->Data == 0)
            {
                ControlFlag |= 32;
            }
            if (CurrentSection == 0)
            {
                razSwitchVAnimCharacterSingle(In->CharacterInstance, 24, NULL, NULL);
                if ((Raziel.Senses.EngagedMask & 512) != 0)
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x1000023, Ptr->Data);
                    razAlignYRotMoveInterp(In->CharacterInstance, Raziel.Senses.EngagedList[9].instance, 486, 0, 20, 0);
                    if (Ptr->Data != 0)
                    {
                        Raziel.alarmTable = 3500;
                        In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
                    }
                }
            }
            break;
        case 0x800010:
            StateSwitchStateData(In, CurrentSection, &StateHandlerThrow2, 0);
            break;
        case 0x80000:
            Raziel.playerEvent |= 1024;
            razSetPlayerEventHistory(1024);
            StateSwitchStateData(In, CurrentSection, &StateHandlerThrow2, 0);
            if (CurrentSection == 1)
            {
                razLaunchForce(In->CharacterInstance);
            }
            break;
        case 0x1000024:
            ignoreHit = 1;
            break;
        case 0x2000002:
            if (CurrentSection == 1)
            {
                inst = (struct _Instance*)Ptr->Data;
                ignoreInst = (struct _Instance*)inst->node.prev;
                data = (struct evMonsterHitData*)Ptr->Data;
                INSTANCE_Post(data->sender, 0x1000000, SetMonsterHitData(In->CharacterInstance, Raziel.attackLastHit, ((int)inst->prev * Raziel.attack->hitPowerScale) / 4096, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));
                if (((Raziel.attack)->handsToCollide & 2) != 0)
                {
                    INSTANCE_Post(data->sender, 0x400000, SetFXHitData(In->CharacterInstance, 41, 32, 256));
                }
                if (((Raziel.attack)->handsToCollide & 1) != 0)
                {
                    INSTANCE_Post(data->sender, 0x400000, SetFXHitData(In->CharacterInstance, 31, 32, 256));
                }
                Raziel.attackCurrentHit = data->sender;
            }
            break;
        case 0x80000001:
            if (CurrentSection == 0)
            {
                if ((ControlFlag & 0x10000000) != 0)
                {
                    Raziel.Mode = 8;
                    if (razSwitchVAnimCharacterGroup(In->CharacterInstance, 0, NULL, NULL) != 0)
                    {
                        G2EmulationSwitchAnimationCharacter(In, 26, 0, 0, 1);
                    }
                    StateSwitchStateCharacterData(In, &StateHandlerCompression, 0);
                    ControlFlag &= -0x2001;
                    break;
                }
                EnMessageQueueData(&In->SectionList[CurrentSection].Defer, 0x80000001, 0);
            }
            break;
        case 0x100001F:
            if (CurrentSection == 1)
            {
                inst = (struct _Instance*)Ptr->Data;
                if (inst->node.next != NULL)
                {
                    hitData = (struct evMonsterHitData*)Ptr->Data;
                    INSTANCE_Post(hitData->lastHit, 0x100001F, SetMonsterHitData(hitData->sender, NULL, hitData->power, Raziel.attack->knockBackDistance, Raziel.attack->knockBackFrames));
                }
            }
            break;
        default:
            DefaultStateHandler(In, CurrentSection, Data);
        }
        DeMessageQueue(&In->SectionList[CurrentSection].Event);
    }
    if (((Raziel.attackFlags & 4) != 0) && (animSection = &In->CharacterInstance->anim.section[CurrentSection & 0xFF],
        animSection->elapsedTime >= Raziel.attack->switchDelay * 100))
    {
        EnMessageQueueData(&In->SectionList[CurrentSection].Event, 0x100000, 0);
    }
    if ((ignoreHit != 0) && (ignoreInst != NULL))
    {
        INSTANCE_Post(ignoreInst, 0x1000024, 0);
    }
}


void StateHandlerCannedReaction(struct __CharacterState* In, int CurrentSection, int Data) // Matching - 100%
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
                    Raziel.alarmTable = 4500;

                    Raziel.Mode = 0x10000;

                    ControlFlag = 0x1041009;

                    PhysicsMode = 3;

                    SteerSwitchMode(In->CharacterInstance, 0);

                    In->CharacterInstance->anim.section[0].swAlarmTable = &Raziel.alarmTable;
                }

                break;
            case 0x100004:
                if (CurrentSection == 1)
                {
                    G2EmulationSwitchAnimationSync(In, 2, 1, 4);
                }

                break;
            case 0x100014:
            case 0x08000000:
                StateSwitchStateData(In, CurrentSection, &StateHandlerIdle, SetControlInitIdleData(0, 0, 3));

                Raziel.Mode = 1;
                break;
            case 0x8000003:
            {
                struct _Instance* Inst;

                if (CurrentSection == 0)
                {
                    Inst = razGetHeldItem();

                    if ((Raziel.Senses.EngagedMask & 0x200))
                    {
                        INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, SetMonsterImpaleData(Inst, &In->CharacterInstance->rotation, &In->CharacterInstance->position, 520));
                    }
                }

                break;
            }
            case 0x8000004:
            {
                struct _Instance* Inst;

                Inst = razGetHeldItem();

                INSTANCE_Post(Inst, 0x800008, 2);

                razReaverOn();

                if ((Raziel.Senses.EngagedMask & 0x200))
                {
                    INSTANCE_Post(Raziel.Senses.EngagedList[9].instance, 0x100000A, SetMonsterImpaleData(Inst, &In->CharacterInstance->rotation, &In->CharacterInstance->position, 520));
                }

                break;
            }
            case 0x80000000:
                break;
            case 0x80000008:
                break;
            case 0x1000001:
                break;
            case 0x1000000:
                break;
            case 0x80000020:
                break;
            default:
                DefaultStateHandler(In, CurrentSection, Data);
                break;
            }

            DeMessageQueue(&In->SectionList[CurrentSection].Event);
        }
    }
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerStumble(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s3*/, int Data /*$s7*/)
void StateHandlerStumble(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 770, offset 0x8009d1c8
	/* begin block 1 */
		// Start line: 771
		// Start offset: 0x8009D1C8
		// Variables:
			struct __Event *Ptr; // $s0

		/* begin block 1.1 */
			// Start line: 821
			// Start offset: 0x8009D500
			// Variables:
				struct evActionPlayHostAnimationData *data; // $v0
		/* end block 1.1 */
		// End offset: 0x8009D500
		// End Line: 823

		/* begin block 1.2 */
			// Start line: 831
			// Start offset: 0x8009D554
			// Variables:
				//struct evMonsterHitData *data; // $v0
		/* end block 1.2 */
		// End offset: 0x8009D554
		// End Line: 832
	/* end block 1 */
	// End offset: 0x8009D5C0
	// End Line: 871

	/* begin block 2 */
		// Start line: 1609
	/* end block 2 */
	// End Line: 1610
				UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerHitReaction(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s2*/, int Data /*$s4*/)
void StateHandlerHitReaction(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 892, offset 0x8009d5ec
	/* begin block 1 */
		// Start line: 893
		// Start offset: 0x8009D5EC
		// Variables:
			struct __Event *Ptr; // $v0

		/* begin block 1.1 */
			// Start line: 902
			// Start offset: 0x8009D6D0
			// Variables:
				struct evMonsterHitData *data; // $s0

			/* begin block 1.1.1 */
				// Start line: 906
				// Start offset: 0x8009D6DC
				// Variables:
					struct _Instance *weapon; // $v0
			/* end block 1.1.1 */
			// End offset: 0x8009D768
			// End Line: 927
		/* end block 1.1 */
		// End offset: 0x8009D7B0
		// End Line: 934
	/* end block 1 */
	// End offset: 0x8009D848
	// End Line: 969

	/* begin block 2 */
		// Start line: 1784
	/* end block 2 */
	// End Line: 1785
					UNIMPLEMENTED();
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerThrow2(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s2*/, int Data /*$s7*/)
void StateHandlerThrow2(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 1049, offset 0x8009d868
	/* begin block 1 */
		// Start line: 1050
		// Start offset: 0x8009D868
		// Variables:
			struct __Event *Ptr; // $a1
			int Anim; // $s4

		/* begin block 1.1 */
			// Start line: 1193
			// Start offset: 0x8009DE6C
			// Variables:
				struct evMonsterHitData *data; // $v1
		/* end block 1.1 */
		// End offset: 0x8009DE80
		// End Line: 1196

		/* begin block 1.2 */
			// Start line: 1271
			// Start offset: 0x8009E07C
			// Variables:
				struct _Instance *weaponInst; // $s0

			/* begin block 1.2.1 */
				// Start line: 1275
				// Start offset: 0x8009E090
				// Variables:
					int spin_type; // $a3

				/* begin block 1.2.1.1 */
					// Start line: 1289
					// Start offset: 0x8009E0E8
					// Variables:
						_Position To; // stack offset -64
						struct _Rotation Rot; // stack offset -56
						MATRIX *matrix; // $v0
				/* end block 1.2.1.1 */
				// End offset: 0x8009E0E8
				// End Line: 1294
			/* end block 1.2.1 */
			// End offset: 0x8009E1B4
			// End Line: 1310
		/* end block 1.2 */
		// End offset: 0x8009E1E0
		// End Line: 1314

		/* begin block 1.3 */
			// Start line: 1361
			// Start offset: 0x8009E320
			// Variables:
				//_Position To; // stack offset -64
				//struct _Rotation Rot; // stack offset -48
				//MATRIX *matrix; // $v0
		/* end block 1.3 */
		// End offset: 0x8009E364
		// End Line: 1371
	/* end block 1 */
	// End offset: 0x8009E364
	// End Line: 1394

	/* begin block 2 */
		// Start line: 2098
	/* end block 2 */
	// End Line: 2099
						UNIMPLEMENTED();
}


void PointAt(struct _Instance* instance, struct _Position* Target, struct _Rotation* Rot1) // Matching - 100%
{
    struct _Position From;
    SVECTOR v1;
    VECTOR v3;
    MATRIX* tempMat;

    tempMat = instance->matrix;

    v1.vx = Raziel.throwData->launchPointX;
    v1.vy = Raziel.throwData->launchPointY;
    v1.vz = Raziel.throwData->launchPointZ;

    ApplyMatrix(tempMat, &v1, &v3);

    v3.vx += instance->position.x;
    v3.vy += instance->position.y;
    v3.vz += instance->position.z;

    From.x = (short)v3.vx;
    From.y = (short)v3.vy;
    From.z = (short)v3.vz;

    MATH3D_RotationFromPosToPos(&From, Target, Rot1);

    Rot1->z -= instance->rotation.z;
    Rot1->y = 0;

    LimitRotation(Rot1);

    if (G2Anim_IsControllerActive(&instance->anim, 14, 14) == 0)
    {
        G2Anim_EnableController(&instance->anim, 14, 14);
    }

    MATH3D_ZYXtoXYZ(Rot1);
    G2Anim_SetController_Vector(&instance->anim, 14, 14, (struct _G2SVector3_Type*)Rot1);
}

void ThrowSetFocusPoint(struct _Instance* instance, struct _Rotation* rot) // Matching - 100%
{
    MATRIX* pTempMat;
    struct _Instance* weaponInstance;
    SVECTOR v1;
    VECTOR v4;

    rot->x = -rot->x;

    pTempMat = theCamera.core.cwTransform2;

    weaponInstance = razGetHeldWeapon();

    if (weaponInstance != NULL && weaponInstance->matrix != NULL)
    {
        v1.vx = 0;
        v1.vy = 0;
        v1.vz = Raziel.throwData->velocity;

        ApplyMatrix(pTempMat, &v1, &v4);

        Raziel.throwTarget.x = (short)v4.vx;
        Raziel.throwTarget.y = (short)v4.vy;
        Raziel.throwTarget.z = (short)v4.vz;

        if (G2Anim_IsControllerActive(&instance->anim, 0xE, 0xE) == G2FALSE)
        {
            G2Anim_EnableController(&instance->anim, 0xE, 0xE);
        }

        MATH3D_ZYXtoXYZ(rot);

        G2Anim_SetController_Vector(&instance->anim, 0xE, 0xE, (struct _G2SVector3_Type*)rot);

        v4.vx = weaponInstance->matrix[1].t[0];
        v4.vy = weaponInstance->matrix[1].t[1];
        v4.vz = weaponInstance->matrix[1].t[2];

        CAMERA_SetLookFocusAndDistance(&theCamera, &v4, PlayerData->throwManualDistance);
    }
}


void LimitRotation(struct _Rotation* rot) // Matching - 100%
{
    rot->z = rot->z & 0xFFF;
    rot->x = rot->x & 0xFFF;

    while (rot->z >= 4097)
    {
        rot->z -= 4096;
    }

    if (rot->z > 0)
    {
        if (rot->z < 2048)
        {
            if (Raziel.throwData->maxZRotation < rot->z)
            {
                rot->z = Raziel.throwData->maxZRotation;
            }
        }
        else
        {
            if (rot->z < Raziel.throwData->minZRotation)
            {
                rot->z = Raziel.throwData->minZRotation;
            }
        }
    }
    else
    {
        rot->z += 4096;
        if (rot->z < Raziel.throwData->minZRotation)
        {
            rot->z = Raziel.throwData->minZRotation;
        }
    }

    while (rot->x >= 4097)
    {
        rot->x -= 4096;
    }

    if (rot->x > 0)
    {
        if (rot->x < 2048)
        {
            if (Raziel.throwData->maxXRotation < rot->x)
            {
                rot->x = Raziel.throwData->maxXRotation;
            }
        }
        else
        {
            if (rot->x < Raziel.throwData->minXRotation)
            {
                rot->x = Raziel.throwData->minXRotation;
            }
        }
    }
    else
    {
        rot->x += 4096;
        if (rot->x < Raziel.throwData->minXRotation)
        {
            rot->x = Raziel.throwData->minXRotation;
        }
    }
}


// autogenerated function stub: 
// void /*$ra*/ StateHandlerGrab(struct __CharacterState *In /*$s1*/, int CurrentSection /*$s3*/, int Data /*$s7*/)
void StateHandlerGrab(struct __CharacterState *In, int CurrentSection, int Data)
{ // line 1541, offset 0x8009e764
	/* begin block 1 */
		// Start line: 1542
		// Start offset: 0x8009E764
		// Variables:
			struct __Event *Ptr; // $s2
			int Anim; // $s4

		/* begin block 1.1 */
			// Start line: 1575
			// Start offset: 0x8009E9B0
			// Variables:
				struct _Instance *Inst; // $a1
		/* end block 1.1 */
		// End offset: 0x8009EA00
		// End Line: 1586

		/* begin block 1.2 */
			// Start line: 1612
			// Start offset: 0x8009EA70
			// Variables:
				//struct _Instance *Inst; // $s0
		/* end block 1.2 */
		// End offset: 0x8009EA8C
		// End Line: 1631

		/* begin block 1.3 */
			// Start line: 1718
			// Start offset: 0x8009EC64
			// Variables:
				//struct _Instance *Inst; // $s0
		/* end block 1.3 */
		// End offset: 0x8009ED24
		// End Line: 1751

		/* begin block 1.4 */
			// Start line: 1758
			// Start offset: 0x8009ED3C
			// Variables:
				//struct _Instance *Inst; // $s0
		/* end block 1.4 */
		// End offset: 0x8009ED9C
		// End Line: 1771

		/* begin block 1.5 */
			// Start line: 1778
			// Start offset: 0x8009EDFC
			// Variables:
				//struct _Instance *Inst; // $s0
		/* end block 1.5 */
		// End offset: 0x8009EE5C
		// End Line: 1793
	/* end block 1 */
	// End offset: 0x8009EED0
	// End Line: 1825

	/* begin block 2 */
		// Start line: 3171
	/* end block 2 */
	// End Line: 3172
				UNIMPLEMENTED();
}




