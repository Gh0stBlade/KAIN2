#include "CORE.H"
#include "GENERIC.H"
#include "Game/STATE.H"
#include "Game/G2/ANMG2ILF.H"
#include "SCRIPT.H"

void GenericInit(struct _Instance* instance, struct GameTracker* gameTracker)//Matching - 86.44%
{
	struct Object* object; // $s1
	struct Spline* spline; // $v1
	static struct _G2AnimInterpInfo_Type crap;

	object = instance->object;
	spline = NULL;

	if (instance->intro != NULL)
	{
		if (instance->intro->multiSpline != NULL)
		{
			spline = instance->intro->multiSpline->positional;
		}
	}

	if (spline == NULL)
	{
		instance->zAccl = -10;
		instance->maxXVel = 100;
		instance->maxYVel = 100;
		instance->maxZVel = 100;
	}

	if (object != NULL)
	{
		if (object->numAnims != 0)
		{
			if (!(object->oflags2 & 0x40000000))
			{
				G2EmulationInstanceSetTotalSections(instance, 1);
				
				G2EmulationInstanceSetStartAndEndSegment(instance, 0,0,(short)((unsigned short)object->modelList[instance->currentModel]->numSegments - 1));
				
				G2EmulationInstanceSetAnimation(instance, 0, 0, 0, 0);
				
				G2EmulationInstanceSetMode(instance, 0, 0);

				if (((unsigned int*)object->name)[0] == 0x65697261 && ((unsigned int*)object->name)[1] == 0x5F5F5F6C)
				{
					G2AnimSection_SetInterpInfo(&instance->anim.section[0], &crap);
				}
			}
		}
	}
}

void GenericCollide(struct _Instance* instance, struct GameTracker* gameTracker)
{
}

void GenericProcess(struct _Instance* instance, struct GameTracker* gameTracker)//Matching - 99.76%
{
	struct Object* object;

	object = instance->object;

	if (object && object->numAnims && !(object->oflags2 & 0x40000000))
	{
		G2EmulationInstancePlayAnimation(instance);
	}
}


unsigned long GenericQuery(struct _Instance* instance, unsigned long query)  // Matching - 100%
{
	long ret;
	struct evControlSaveDataData* pdata;
	struct Object* object;

	ret = 0;
	switch (query)
	{
	case 6:
		ret = SetPositionData(instance->position.x, instance->position.y, instance->position.z);
		break;
	case 7:
		ret = SetPositionData(instance->rotation.x, instance->rotation.y, instance->rotation.z);
		break;
	case 11:
		ret = 1;
		if (instance->flags2 & 0x8000000)
		{
			ret = 2;
		}
		break;
	case 12:
		ret = (long)instance->matrix;
		break;
	case 17:
		ret = G2EmulationInstanceQueryAnimation(instance, 0);
		break;
	case 18:
		ret = G2EmulationInstanceQueryFrame(instance, 0);
		break;
	case 24:
		if (instance->flags2 & 4)
		{
			pdata = (struct evControlSaveDataData*)CIRC_Alloc(12);
			ret = (long)pdata;
			pdata->length = 8;
			memcpy(pdata->data, &instance->flags, 8);
		}
		break;
	case 1:
		object = instance->object;
		if (object->oflags2 & 0x4000000)
		{
			ret = 0x40000;
			break;
		}
		if (object->oflags & 0x100000)
		{
			ret = 0x100000;
			break;
		}
		if (object->oflags2 & 32)
		{
			ret = 0x200000;
			break;
		}
		ret = 0x80000000;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 8:
	case 9:
	case 10:
	case 13:
	case 14:
	case 15:
	case 16:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
		ret = 0;
		break;
	}
	return ret;
}


void GenericMessage(struct _Instance* instance, unsigned long message, unsigned long data)  // Matching - 100%
{
	struct evAnimationInstanceSwitchData* Ptr;

	switch (message)
	{
	case 0x8000008:
		Ptr = (struct evAnimationInstanceSwitchData*)data;
		if (instance->anim.section[0].interpInfo != NULL)
		{
			G2EmulationInstanceSetAnimation(instance, 0, Ptr->anim, Ptr->frame, Ptr->frames);
		}
		else
		{
			G2EmulationInstanceSetAnimation(instance, 0, Ptr->anim, Ptr->frame, 0);
		}
		G2EmulationInstanceSetMode(instance, 0, Ptr->mode);
		break;
	case 0x4000A:
		STREAM_SetInstancePosition(instance, (struct evPositionData*)data);
		break;
	case 0x4000B:
		instance->rotation.x = ((struct evPositionData*)data)->x;
		instance->rotation.y = ((struct evPositionData*)data)->y;
		instance->rotation.z = ((struct evPositionData*)data)->z;
		break;
	case 0x8000010:
		G2EmulationInstanceSetMode(instance, 0, data);
		break;
	case 0x40002:
		ScriptKillInstance(instance, data);
		break;
	case 0x100007:
		instance->flags = (int)((struct _Instance*)data)->node.next[0].prev;
		instance->flags2 = (int)((struct _Instance*)data)->node.next[0].next;
		break;
	}
}

void GenericRelocateTune(struct Object* object, long offset)
{
	struct GenericTune* tune;

	tune = (struct GenericTune*)object->modelList;

	if (tune != NULL && tune->shatterData != NULL)
	{
		tune->shatterData = (char*)tune->shatterData + offset;
	}
}