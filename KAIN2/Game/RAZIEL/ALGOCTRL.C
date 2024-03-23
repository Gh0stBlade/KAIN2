#include "Game/CORE.H"
#include "ALGOCTRL.H"
#include "Game/INSTANCE.H"
#include "Game/STATE.H"
#include "Game/G2/ANMCTRLR.H"
#include "Game/G2/QUATG2.H"
#include <Game/MATH3D.H>

static int AlgoControlFlag;

void InitAlgorithmicWings(struct _Instance* instance) // Matching - 100%
{
	struct _G2EulerAngles_Type Rot;
	unsigned char i;

	if (!(AlgoControlFlag & 0x1))
	{
		if (instance->matrix == NULL)
		{
			INSTANCE_Post(instance, 0x100006, 0);
		}
		else
		{
			for (i = 51; i < 54; i++)
			{
				if (instance->matrix == NULL)
				{
					Rot.z = 0;
					Rot.y = 0;
					Rot.x = 0;
				}
				else
				{
					G2EulerAngles_FromMatrix(&Rot, &(instance->anim).segMatrices[i - 1], 21);
				}

				G2Anim_EnableController(&instance->anim, i, 8);
				G2EmulationSetInterpController_Vector(instance, i, 8, (struct _G2SVector3_Type*)&Rot, (i - 50) * 3, 2);
			}

			for (i = 59; i < 62; i++)
			{
				if (instance->matrix == NULL)
				{
					Rot.z = 0;
					Rot.y = 0;
					Rot.x = 0;
				}
				else
				{
					G2EulerAngles_FromMatrix(&Rot, &(instance->anim).segMatrices[i - 1], 21);
				}

				G2Anim_EnableController(&instance->anim, i, 8);
				G2EmulationSetInterpController_Vector(instance, i, 8, (struct _G2SVector3_Type*)&Rot, (i - 58) * 3, 2);
			}

			AlgoControlFlag |= 0x1;
		}
	}
}

void DeInitAlgorithmicWings(struct _Instance* instance) // Matching - 100%
{
	unsigned char i;

	if ((AlgoControlFlag & 1))
	{
		for (i = 51; i < 54; i++)
		{
			G2Anim_DisableController(&instance->anim, i, 8);
		}

		for (i = 59; i < 62; i++)
		{
			G2Anim_DisableController(&instance->anim, i, 8);
		}

		AlgoControlFlag &= ~0x1;
	}
}

void AlgorithmicWings(struct _Instance* instance, struct evAnimationControllerDoneData* ControllerData) // Matching - 81.50%
{
	struct _G2EulerAngles_Type Rot;

	G2EulerAngles_FromMatrix(&Rot, &instance->anim.segMatrices[ControllerData->segment - 1], 0x15);
	G2EmulationSetInterpController_Vector(instance, ControllerData->segment, ControllerData->type, (struct _G2SVector3_Type*)&Rot, 5, 2);
}

void AlgorithmicNeck(struct _Instance* Player, struct _Instance* Target) // Matching - 100%
{
    struct _Position From;
    struct _Position To;
    struct _Rotation Rot1;
    int Diff;
    MATRIX matrix;

    Raziel.Senses.Flags &= ~0x8;

    if ((Raziel.Senses.Flags & 0x10))
    {
        struct evCollideInstanceStatsData data;

        TransposeMatrix(Player->oldMatrix, &matrix);

        if (((INSTANCE_SetStatsData(Player, NULL, &Raziel.Senses.lookAtPoint, &data, &matrix)) != 0)
            && (data.distance < 3200) && (MATH3D_ConeDetect(&data.relativePosition, 967, 967) != 0))
        {
            Raziel.Senses.Flags |= 0x8;
        }
    }

    if ((Target == NULL) && (!(Raziel.Senses.Flags & 0x8)))
    {
        if ((G2Anim_IsControllerActive(&Player->anim, 17, 8)) != G2FALSE)
        {
            G2Anim_InterpDisableController(&Player->anim, 17, 8, 900);
        }

        return;
    }

    if ((G2Anim_IsControllerActive(&Player->anim, 17, 8)) == G2FALSE)
    {
        G2Anim_EnableController(&Player->anim, 17, 8);
    }

    if ((Raziel.Senses.Flags & 0x8))
    {
        To.x = (short)Raziel.Senses.lookAtPoint.x;
        To.y = (short)Raziel.Senses.lookAtPoint.y;
        To.z = (short)Raziel.Senses.lookAtPoint.z;
    }
    else
    {
        MATRIX* targetMatrix; // not from SYMDUMP
        unsigned long query; // not from SYMDUMP

        query = INSTANCE_Query(Target, 12);

        targetMatrix = (MATRIX*)query;

        if (query == 0)
        {
            return;
        }

        To.x = (short)targetMatrix->t[0];
        To.y = (short)targetMatrix->t[1];
        To.z = (short)targetMatrix->t[2];
    }

    From.x = (short)(&Player->matrix[17])->t[0];
    From.y = (short)(&Player->matrix[17])->t[1];
    From.z = (short)(&Player->matrix[17])->t[2];

    MATH3D_RotationFromPosToPos(&From, &To, &Rot1);

    Diff = AngleDiff(Player->rotation.z, Rot1.z);

    if (Diff > 512)
    {
        Rot1.z = Player->rotation.z + 512;
    }

    if (Diff < -512)
    {
        Rot1.z = Player->rotation.z - 512;
    }

    if ((unsigned short)Rot1.x - 513 < 1535U)
    {
        Rot1.x = 512;
    }

    if ((unsigned short)Rot1.x - 2049 < 1389U)
    {
        Rot1.x = 3438;
    }

    MATH3D_ZYXtoXYZ(&Rot1);

    G2EmulationSetInterpController_Vector(Player, 17, 8, (struct _G2SVector3_Type*)&Rot1, 3, 0);
}




