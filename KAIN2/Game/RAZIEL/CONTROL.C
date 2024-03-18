#include "Game/CORE.H"
#include "CONTROL.H"
#include "Game/RAZIEL/RAZIEL.H"

struct __Force* dword_B08390;
int PhysicsMode;
struct __Force* ExternalForcesPtr = &ExternalForces[0];

static inline int damp(int val, int damping)
{
	int temp;

	temp = val * damping;

	if (temp < 0)
	{
		temp += 0xFFF;
	}

	return -(temp >> 12);
}

void SetPhysics(struct _Instance *instance, short gravity, long x, long y, long z)
{
	SetExternalForce(ExternalForcesPtr, 0, 0, gravity, 0, 4096);

	instance->xVel = x;
	instance->yVel = y;
	instance->zVel = z;
}

void ResetPhysics(struct _Instance *instance, short gravity)
{
	int i;

	SetExternalForce(ExternalForcesPtr, 0, 0, gravity, 0, 4096);
	
	for(i = 1; i < 4; i++)
	{
		SetExternalForce(ExternalForcesPtr + i, 0, 0, 0, 0, 0);
	}

	instance->xVel = 0;
	instance->yVel = 0;
	instance->zVel = 0;
	instance->xAccl = 0;
	instance->yAccl = 0;
	instance->zAccl = gravity;
}


void SetDampingPhysics(struct _Instance* instance, int damping) // Matching - 100%
{
	instance->xAccl = damp(instance->xVel, damping);
	instance->yAccl = damp(instance->yVel, damping);
	instance->zAccl = damp(instance->zVel, damping);

	SetExternalForce(ExternalForces, (short)instance->xAccl, (short)instance->yAccl, (short)instance->zAccl, 0, 4096);
}

void SetImpulsePhysics(struct _Instance* instance, struct __Player* player)//Matching - 52.50%
{
	int vLength;
	int Dot;

	if (!(player->Mode & 0x40000))
	{
		Dot = (-instance->offset.x * player->iVelocity.x) + (-instance->offset.y * player->iVelocity.y) + (-instance->offset.z * player->iVelocity.z);
		vLength = (player->iVelocity.x * player->iVelocity.x) + (player->iVelocity.y * player->iVelocity.y) + (player->iVelocity.z * player->iVelocity.z);
		if (vLength)
		{
			instance->position.x += instance->offset.x + Dot * player->iVelocity.x / vLength;
			instance->position.y += instance->offset.y + Dot * player->iVelocity.y / vLength;
			instance->position.z += instance->offset.z + Dot * player->iVelocity.z / vLength;
		}
	}
}

void SetDropPhysics(struct _Instance* instance, struct __Player* player)
{
	SetExternalForce(ExternalForcesPtr, 0, 4, -16, 0, 4096);
}

// unused
void GetPhysicsVelocity(struct _Instance* instance, SVECTOR* in_pos, SVECTOR* out_pos)
{
	UNIMPLEMENTED();
}

void InitExternalForces(struct __Force* Forces, int MaxForces)//Matching - 99.69%
{
	int i;

	for (i = MaxForces - 1; i != 0; i--)
	{
		Forces[i].Friction = 0;
		Forces[i].LinearForce.x = 0;
		Forces[i].LinearForce.y = 0;
		Forces[i].LinearForce.z = 0;
	}

	ExternalForcesPtr = Forces;
}

void SetExternalForce(struct __Force* In, short x, short y, short z, int Space, int Friction)
{
	In->LinearForce.x = x;
	In->LinearForce.y = y;
	In->LinearForce.z = z;
	In->Friction = Friction;
	In->Type = Space;
}


void SetExternalTransitionForce(struct __Force* in, struct _Instance* instance, int time, int x, int y, int z) // Matching - 100%
{
	in->Type = 2;
	in->LinearForce.x = (x - instance->xVel) / time;
	in->LinearForce.y = (y - instance->yVel) / time;
	in->LinearForce.z = (z - instance->zVel) / time;
	in->Friction = (short)time;
}

void ProcessPhysics(struct __Player* player, struct __CharacterState* In, int CurrentSection, int Mode)//Matching - 98.70%
{
	int time;
	struct _Instance* instance;

	instance = In->CharacterInstance;

	if (In->CharacterInstance->matrix != NULL)
	{
		time = (In->CharacterInstance->anim.section[0].speedAdjustment * gameTrackerX.timeMult) >> 12;

		switch (Mode)
		{
		case 0:

			ApplyExternalLocalForces(player, In->CharacterInstance, ExternalForcesPtr, 4, (struct _Vector*)&In->CharacterInstance->xAccl);

			PhysicsMoveLocalZClamp(In->CharacterInstance, player->RotationSegment, time, 0);
			break;
		case 4:

			ApplyExternalLocalForces(player, In->CharacterInstance, ExternalForcesPtr, 4, (struct _Vector*)&In->CharacterInstance->xAccl);

			PhysicsMoveLocalZClamp(In->CharacterInstance, player->RotationSegment, time, 0);

			PHYSICS_StopIfCloseToTarget(In->CharacterInstance, 0, 0, 0);

			if (In->CharacterInstance->xAccl == 0 &&
				In->CharacterInstance->yAccl == 0 &&
				In->CharacterInstance->zAccl == 0)
			{
				SetExternalForce(ExternalForcesPtr, 0, 0, 0, 0, 0);
			}

			break;
		case 5:
			PhysicsMoveLocalZClamp(In->CharacterInstance, player->RotationSegment, time, 0);

			PHYSICS_StopIfCloseToTarget(instance, 0, 0, player->swimTargetSpeed);

			if (instance->xAccl == 0 &&
				instance->yAccl == 0 &&
				instance->zAccl == 0)
			{
				INSTANCE_Post(instance, 0x100011, player->swimTargetSpeed);
			}

			break;
		case 6:
			PhysicsMoveLocalZClamp(In->CharacterInstance, player->RotationSegment, time, 1);

			PHYSICS_StopIfCloseToTarget(instance, 0, 0, player->swimTargetSpeed);

			if (instance->xAccl == 0 &&
				instance->yAccl == 0 &&
				instance->zAccl == 0)
			{
				INSTANCE_Post(instance, 0x100011, player->swimTargetSpeed);
			}

			break;
		}
	}
}

void ApplyExternalLocalForces(struct __Player* player, struct _Instance* instance, struct __Force* Forces, int MaxForces, struct _Vector* Out)//Matching - 100%
{
	int i;
	int friction;

	Out->z = 0;
	Out->y = 0;
	Out->x = 0;

	for (i = 0; i < MaxForces; i++)
	{
		if (Forces[i].Friction != 0)
		{
			if (Forces[i].Type == 0)
			{
				Out->x += Forces[i].LinearForce.x;
				Out->y -= Forces[i].LinearForce.y;
				Out->z += Forces[i].LinearForce.z;
			}
			else if (Forces[i].Type == 1)
			{
				Out->x += Forces[i].LinearForce.x;
				Out->y += Forces[i].LinearForce.y;
				Out->z += Forces[i].LinearForce.z;
			}
			if (Forces[i].Type == 2)
			{
				Out->x += Forces[i].LinearForce.x;
				Out->y -= Forces[i].LinearForce.y;
				Out->z += Forces[i].LinearForce.z;

				Forces[i].Friction--;
			}
			else
			{
				friction = Forces[i].Friction;

				Forces[i].LinearForce.x = (Forces[i].LinearForce.x * friction) < 0 ? (((Forces[i].LinearForce.x * friction) + 0xFFF)) : ((Forces[i].LinearForce.x * friction));
				Forces[i].LinearForce.x >>= 12;
				Forces[i].LinearForce.y = (Forces[i].LinearForce.y * friction) < 0 ? (((Forces[i].LinearForce.y * friction) + 0xFFF)) : ((Forces[i].LinearForce.y * friction));
				Forces[i].LinearForce.y >>= 12;
				Forces[i].LinearForce.z = (Forces[i].LinearForce.z * friction) < 0 ? (((Forces[i].LinearForce.z * friction) + 0xFFF)) : ((Forces[i].LinearForce.z * friction));
				Forces[i].LinearForce.z >>= 12;

				if (Forces[i].LinearForce.x == 0 && Forces[i].LinearForce.y == 0 && Forces[i].LinearForce.z == 0)
				{
					Forces[i].Friction = 0;
				}
			}
		}
	}

	Out->y = -Out->y;
}




