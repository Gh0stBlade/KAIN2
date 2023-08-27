#include "Game/CORE.H"
#include "ALGOCTRL.H"
#include "Game/INSTANCE.H"
#include "Game/STATE.H"
#include "Game/G2/ANMCTRLR.H"
#include "Game/G2/QUATG2.H"
#include "Game/MATH3D.H"

static int AlgoControlFlag;

void InitAlgorithmicWings(struct _Instance* instance) // Matching - 99.51%
{
	struct _G2EulerAngles_Type Rot;
	unsigned char i;

	if ((AlgoControlFlag & 1) == 0)
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
					G2EulerAngles_FromMatrix(&Rot, &(instance->anim).segMatrices[i - 1], 0x15);
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
					G2EulerAngles_FromMatrix(&Rot, &(instance->anim).segMatrices[i - 1], 0x15);
				}

				G2Anim_EnableController(&instance->anim, i, 8);
				G2EmulationSetInterpController_Vector(instance, i, 8, (struct _G2SVector3_Type*)&Rot, (i - 58) * 3, 2);
			}

			AlgoControlFlag |= 1;
		}
	}
}

void DeInitAlgorithmicWings(struct _Instance* instance) // Matching - 99.31%
{
	unsigned char i;

	if ((AlgoControlFlag & 1) != 0)
	{
		for (i = 51; i < 54; i++)
		{
			G2Anim_DisableController(&instance->anim, i, 8);
		}

		for (i = 59; i < 62; i++)
		{
			G2Anim_DisableController(&instance->anim, i, 8);
		}

		AlgoControlFlag &= 0xfffffffe;
	}
}

void AlgorithmicWings(struct _Instance* instance, struct evAnimationControllerDoneData* ControllerData) // Matching - 81.50%
{
	struct _G2EulerAngles_Type Rot;

	G2EulerAngles_FromMatrix(&Rot, &instance->anim.segMatrices[ControllerData->segment - 1], 0x15);
	G2EmulationSetInterpController_Vector(instance, ControllerData->segment, ControllerData->type, (struct _G2SVector3_Type*)&Rot, 5, 2);
}

void AlgorithmicNeck(struct _Instance* Player, struct _Instance* Target) { // Matching - 96.53%
    _Position From;
    _Position To;
    struct _Rotation Rot1;
    int Diff;
    MATRIX matrix;
    struct evCollideInstanceStatsData data;
    int query;
    Raziel.Senses.Flags &= 0xFFFFFFF7;
    if ((Raziel.Senses.Flags & 0x10) != 0)
    {
        TransposeMatrix(Player->oldMatrix, &matrix);
        if ((((INSTANCE_SetStatsData(Player, NULL, &Raziel.Senses.lookAtPoint, &data, &matrix)) != 0)
            && ((data.distance < 3200) != 0)) && (MATH3D_ConeDetect(&data.relativePosition, 967, 967) != 0))
        {
            Raziel.Senses.Flags |= 8;
        }
    }
    if ((Target == 0) && ((Raziel.Senses.Flags & 8) == 0))
    {
        if ((G2Anim_IsControllerActive(&Player->anim, 17, 8)) != G2FALSE)
        {
            G2Anim_InterpDisableController(&Player->anim, 17, 8, 900);
        }
    }
    else {
        if ((G2Anim_IsControllerActive(&Player->anim, 17, 8)) == G2FALSE)
        {
            G2Anim_EnableController(&Player->anim, 17, 8);
        }
        if ((Raziel.Senses.Flags & 8) != 0)
        {
            To.x = Raziel.Senses.lookAtPoint.x;
            To.y = Raziel.Senses.lookAtPoint.y;
            To.z = Raziel.Senses.lookAtPoint.z;
        }
        else {
            query = INSTANCE_Query(Target, 12);
            if (query != 0)
            {
                To.x = ((struct _Instance*)query)->flags;
                To.y = ((struct _Instance*)query)->flags2;
                To.z = ((struct _Instance*)query)->object->oflags;
            }
            else
            {
                return;
            }
        }
        From.x = *(short*)&Player->matrix[17].t[0];
        From.y = *(short*)&Player->matrix[17].t[1];
        From.z = *(short*)&Player->matrix[17].t[2];
        MATH3D_RotationFromPosToPos(&From, &To, &Rot1);
        Diff = AngleDiff(Player->rotation.z, Rot1.z);
        if ((Diff < 513) == 0)
        {
            Rot1.z = Player->rotation.z + 512;
        }
        if ((Diff < -512) != 0)
        {
            Rot1.z = Player->rotation.z - 512;
        }
        if (((unsigned int)Rot1.x - 513 < 1535) != 0)
        {
            Rot1.x = 512;
        }
        if (((unsigned int)Rot1.x - 2049 < 1389) != 0)
        {
            Rot1.x = 3438;
        }
        MATH3D_ZYXtoXYZ(&Rot1);
        G2EmulationSetInterpController_Vector(Player, 17, 8, &Rot1, 3, 0);
    }
}
