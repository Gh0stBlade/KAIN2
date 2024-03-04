#include "CORE.H"
#include "OBTABLE.H"
#include "RAZIEL/RAZIEL.H"
#include "REAVER.H"
#include "GLYPH.H"
#include "PHYSOBS.H"
#include "MONSTER/MONAPI.H"
#include "FX.H"
#include "EVENT.H"
#include "GENERIC.H"
#include "GAMELOOP.H"
#include "SCRIPT.H"
#include "GEX2.H"
#include "Game/STREAM.H"

struct ObjectAccess objectAccess[28] =
{
	{
		"hud_____",
		NULL
	},
	{
		"flame___",
		NULL
	},
	{
		"fonts___",
		NULL
	},
	{
		"waterfx_",
		NULL
	},
	{
		"dbgfont_",
		NULL
	},
	{
		"cammode_",
		NULL
	},
	{
		"introfx_",
		NULL
	},
	{
		"mapicon_",
		NULL
	},
	{
		"paths___",
		NULL
	},
	{
		"soul____",
		NULL
	},
	{
		"particle",
		NULL
	},
	{
		"knife___",
		NULL
	},
	{
		"stick___",
		NULL
	},
	{
		"xbow____",
		NULL
	},
	{
		"fthrow__",
		NULL
	},
	{
		"moregg__",
		NULL
	},
	{
		"wcegg___",
		NULL
	},
	{
		"eggsac__",
		NULL
	},
	{
		"mound___",
		NULL
	},
	{
		"force___",
		NULL
	},
	{
		"glphicon",
		NULL
	},
	{
		"healthu_",
		NULL
	},
	{
		"sreavr__",
		NULL
	},
	{
		"wrpface_",
		NULL
	},
	{
		"healths_",
		NULL
	},
	{
		"eaggot__",
		NULL
	},
	{
		"eaggots_",
		NULL
	},
	{
		NULL,
		NULL
	}
};

/*struct ObjectFunc objectFunc[8] =
{
	{
		"sreavr__",
		&SoulReaverInit,
		&SoulReaverProcess,
		&SoulReaverCollide,
		&SoulReaverQuery,
		&SoulReaverPost,
		NULL,
		NULL,
		NULL
	},
	{
		"glphicon",
		&GlyphInit,
		NULL,
		&GlyphCollide,
		&GlyphQuery,
		&GlyphPost,
		NULL,
		NULL,
		NULL
	},
	{
		"physical",
		&InitPhysicalObject,
		&ProcessPhysicalObject,
		&CollidePhysicalObject,
		&PhysicalObjectQuery,
		&PhysicalObjectPost,
		NULL,
		&PhysicalRelocateTune,
		NULL
	},
	{
		"monster_",
		&MonsterInit,
		&MonsterProcess,
		&MonsterCollide,
		&MonsterQuery,
		&MonsterMessage,
		additional&MonsterAdditionalCollide,
		&MonsterRelocateTune,
		&MonsterRelocateInstanceObject
	},
	{
		"particle",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&FX_RelocateGeneric,
		NULL
	},
	{
		"litshaft",
		&LitShaftInit,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	},
	{
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	}
};*/

struct ObjectFunc objectFunc[8] =
{
	{
		"raziel__",
		RazielInit,
		RazielProcess,
		RazielCollide,
		RazielQuery,
		RazielPost,
		RazielAdditionalCollide,
		NULL,
		NULL
	},
	{
		"sreavr__",
		SoulReaverInit,
		SoulReaverProcess,
		SoulReaverCollide,
		SoulReaverQuery,
		SoulReaverPost,
		NULL,
		NULL,
		NULL
	},
	{
		"glphicon",
		GlyphInit,
		NULL,
		GlyphCollide,
		GlyphQuery,
		GlyphPost,
		NULL,
		NULL,
		NULL
	},
	{
		"physical",
		InitPhysicalObject,
		ProcessPhysicalObject,
		CollidePhysicalObject,
		PhysicalObjectQuery,
		PhysicalObjectPost,
		NULL,
		PhysicalRelocateTune,
		NULL
	},
	{
		"monster_",
		MonsterInit,
		MonsterProcess,
		MonsterCollide,
		MonsterQuery,
		MonsterMessage,
		MonsterAdditionalCollide,
		MonsterRelocateTune,
		MonsterRelocateInstanceObject
	},
	{
		"particle",
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		FX_RelocateGeneric,
		NULL
	},
	{
		"litshaft",
		LitShaftInit,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	}
};

void OBTABLE_InstanceInit(struct _Instance *instance)
{
	long id;
	
	id = instance->object->id;

	if (id < 0)
	{
		GenericInit(instance, &gameTrackerX);
	}
	else
	{
		if (objectFunc[id].initFunc != NULL)
		{
			objectFunc[id].initFunc(instance, &gameTrackerX);
		}
	}

	instance->flags2 |= 0x200000;

	if (!(instance->flags & 0x100000))
	{
		SCRIPT_InstanceSplineInit(instance);
	}

	if (instance->intro != NULL && (instance->intro->flags & 0x20))
	{
		instance->flags2 &= 0xFFFFFFFB;
		instance->flags2 &= 0xFFFDFFFF;
	}
}

void OBTABLE_GetInstanceCollideFunc(struct _Instance* instance) // Matching - 100%
{
	struct ObjectFunc* temp;  // not from SYMDUMP
	long id;

	id = instance->object->id;

	if (id >= 0)
	{
		temp = objectFunc;

		instance->collideFunc = temp[id].collideFunc;
	}
	else
	{
		instance->collideFunc = GenericCollide;
	}
}

void OBTABLE_GetInstanceAdditionalCollideFunc(struct _Instance *instance) // Matching - 100%
{ 
	long id;

	id = instance->object->id;

	if (id >= 0)
	{
		instance->additionalCollideFunc = objectFunc[id].additionalCollideFunc;
	}
	else
	{
		instance->additionalCollideFunc = NULL;
	}
}

void OBTABLE_GetInstanceProcessFunc(struct _Instance* instance) // Matching - 100%
{
	struct ObjectFunc* temp;  // not from SYMDUMP
	long id;

	id = instance->object->id;

	if (id >= 0)
	{
		temp = objectFunc;

		instance->processFunc = temp[id].processFunc;
	}
	else
	{
		instance->processFunc = GenericProcess;
	}
}

void OBTABLE_GetInstanceQueryFunc(struct _Instance* instance) // Matching - 100%
{
	struct ObjectFunc* temp;  // not from SYMDUMP
	long id;

	id = instance->object->id;

	if (id >= 0)
	{
		temp = objectFunc;

		instance->queryFunc = temp[id].queryFunc;
	}
	else
	{
		instance->queryFunc = GenericQuery;
	}
}

void OBTABLE_GetInstanceMessageFunc(struct _Instance* instance) // Matching - 100%
{
	struct ObjectFunc* temp;  // not from SYMDUMP
	long id;

	id = instance->object->id;

	if (id >= 0)
	{
		temp = objectFunc;

		instance->messageFunc = temp[id].messageFunc;
	}
	else
	{
		instance->messageFunc = GenericMessage;
	}
}

void OBTABLE_InitObjectWithID(struct Object* object)
{
	long id;
	struct ObjectAccess* oa;

	if (object != NULL)
	{
		if ((object->oflags2 & 0x40000))
		{
			id = 0;

			while (1)
			{
				if (objectFunc[id].scriptName != NULL)
				{
					if (!strcmp(objectFunc[id].scriptName, "physical"))
					{
						if (objectFunc[id].scriptName == NULL)
						{
							object->id = -1;
						}
						else
						{
							object->id = (short)id;
						}

						break;
					}
				}
				else
				{
					object->id = -1;
					break;
				}

				id++;
			}
		}
		else if ((object->oflags2 & 0x80000))
		{
			id = 0;

			while (1)
			{
				if (objectFunc[id].scriptName != NULL)
				{
					if (!strcmp(objectFunc[id].scriptName, "monster_"))
					{
						if (objectFunc[id].scriptName == NULL)
						{
							object->id = -1;
						}
						else
						{
							object->id = (short)id;
						}

						break;
					}
				}
				else
				{
					object->id = -1;
					break;
				}

				id++;
			}
		}
		else
		{
			id = 0;

			while (1)
			{
				if (objectFunc[id].scriptName != NULL)
				{
					if (!strcmp(objectFunc[id].scriptName, object->script))
					{
						if (objectFunc[id].scriptName == NULL)
						{
							object->id = -1;
						}
						else
						{
							object->id = (short)id;
						}

						break;
					}
				}
				else
				{
					object->id = -1;
					break;
				}

				id++;
			}
		}

		oa = &objectAccess[0];

		while (oa->objectName != NULL)
		{
			if (((unsigned int*)object->name)[0] == ((unsigned int*)oa->objectName)[0] &&
				((unsigned int*)object->name)[1] == ((unsigned int*)oa->objectName)[1])
			{
				oa->object = object;
				break;
			}
			else
			{
				oa++;
			}
		}
	}
}

void OBTABLE_ClearObjectReferences() // Matching - 100%
{
	struct ObjectAccess* oa;

	for (oa = objectAccess; oa->objectName != NULL; oa++)
	{
		oa->object = NULL;
	}
}

void OBTABLE_RemoveObjectEntry(struct Object* object) // Matching - 100%
{
	struct ObjectAccess* oa;

	for (oa = objectAccess; oa->objectName != NULL; oa++)
	{
		if (oa->object == object)
		{
			oa->object = NULL;
			break;
		}
	}
}


// autogenerated function stub: 
// struct Object * /*$ra*/ OBTABLE_FindObject(char *objectName /*$s3*/)
struct Object * OBTABLE_FindObject(char *objectName)
{ // line 394, offset 0x8003de44
	/* begin block 1 */
		// Start line: 395
		// Start offset: 0x8003DE44
		// Variables:
			struct Object *object; // $s0
			struct _ObjectTracker *otr; // $v0
			int i; // $s2
	/* end block 1 */
	// End offset: 0x8003DED4
	// End Line: 412

	/* begin block 2 */
		// Start line: 812
	/* end block 2 */
	// End Line: 813

	/* begin block 3 */
		// Start line: 817
	/* end block 3 */
	// End Line: 818
			UNIMPLEMENTED();
	return null;
}

void OBTABLE_ChangeObjectAccessPointers(struct Object* oldObject, struct Object* newObject) // Matching - 100%
{
	struct ObjectAccess* oa;

	for (oa = objectAccess; oa->objectName != NULL; oa++)
	{
		if (oa->object == oldObject)
		{
			oa->object = newObject;
			break;
		}
	}
}


// autogenerated function stub: 
// void /*$ra*/ OBTABLE_RelocateObjectTune(struct Object *object /*$a0*/, long offset /*$a1*/)
void OBTABLE_RelocateObjectTune(struct Object *object, long offset)
{ // line 428, offset 0x8003df38
	/* begin block 1 */
		// Start line: 429
		// Start offset: 0x8003DF38
		// Variables:
			long id; // $a2

		/* begin block 1.1 */
			// Start line: 442
			// Start offset: 0x8003DF84
		/* end block 1.1 */
		// End offset: 0x8003DF8C
		// End Line: 444
	/* end block 1 */
	// End offset: 0x8003DF8C
	// End Line: 445

	/* begin block 2 */
		// Start line: 902
	/* end block 2 */
	// End Line: 903
			UNIMPLEMENTED();
}

void OBTABLE_RelocateInstanceObject(struct _Instance* instance, long offset)
{
	int id;

	id = instance->object->id;

	if (id >= 0)
	{
		if (objectFunc[id].relocateInstObFunc)
		{
			objectFunc[id].relocateInstObFunc(instance, offset);
		}
	}
}

void OBTABLE_InitAnimPointers(struct _ObjectTracker* objectTracker) // Matching - 100%
{
	struct Object* object;
	int i;
	char* earlyOut;
	struct _G2AnimKeylist_Type** keyPtr;
	struct _ObjectOwnerInfo* oi;
	struct _ObjectTracker* otr;
	struct Object* ownerOb;
	int j;
	int objectIndex;

	object = objectTracker->object;

	if ((object->oflags2 & 0x10000000))
	{
		keyPtr = object->animList;

		earlyOut = NULL;

		for (i = object->numAnims; i != 0; i--, keyPtr++)
		{
			oi = (struct _ObjectOwnerInfo*)keyPtr[0];

			if (oi->magicnum == 0xFACE0FF)
			{
				otr = STREAM_GetObjectTracker(oi->objectName);

				if (otr != NULL)
				{
					objectIndex = (objectTracker - gameTrackerX.GlobalObjects);

					ownerOb = otr->object;

					for (j = 0; j < otr->numObjectsUsing; j++)
					{
						if (otr->objectsUsing[j] == objectIndex)
						{
							break;
						}
					}

					if (j == otr->numObjectsUsing)
					{
						otr->numObjectsUsing += 1;
						otr->objectsUsing[j] = objectIndex;
					}

					if (otr->objectStatus == 2)
					{
						keyPtr[0] = ownerOb->animList[oi->animID];
					}
					else
					{
						earlyOut = oi->objectName;
					}
				}
			}
		}

		if (earlyOut != NULL)
		{
			return;
		}

		object->oflags2 &= 0xEFFFFFFF;
	}

	if ((object->oflags2 & 0x80000))
	{
		MonsterTranslateAnim(object);
	}
}