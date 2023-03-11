#pragma once

enum MonsterAnim
{
	MONSTER_ANIM_HIT1 = 0,
	MONSTER_ANIM_HIT2 = 1,
	MONSTER_ANIM_WALK = 2,
	MONSTER_ANIM_RUN = 3,
	MONSTER_ANIM_GRABBED = 4,
	MONSTER_ANIM_STUNNED = 5,
	MONSTER_ANIM_JUMPFORWARD = 6,
	MONSTER_ANIM_JUMPBACK = 7,
	MONSTER_ANIM_JUMPLEFT = 8,
	MONSTER_ANIM_JUMPRIGHT = 9,
	MONSTER_ANIM_STANCE_HEALTHY = 10,
	MONSTER_ANIM_STANCE_VULNERABLE = 11,
	MONSTER_ANIM_STUNNED_RECOVERY = 12,
	MONSTER_ANIM_IMPALED = 13,
	MONSTER_ANIM_STRUGGLE = 14,
	MONSTER_ANIM_BREAKHOLD = 15,
	MONSTER_ANIM_THROWN = 16,
	MONSTER_ANIM_IMPACT = 17,
	MONSTER_ANIM_LANDONFEET = 18,
	MONSTER_ANIM_FALL = 19,
	MONSTER_ANIM_BREAKFALL = 20,
	MONSTER_ANIM_SPINLAND = 21,
	MONSTER_ANIM_IMPALEDEATH = 22,
	MONSTER_ANIM_GETUP = 23,
	MONSTER_ANIM_GENERALDEATH = 24,
	MONSTER_ANIM_MISSILEIMPALEDEATH = 25,
	MONSTER_ANIM_SURPRISED = 26,
	MONSTER_ANIM_AGONY = 27,
	MONSTER_ANIM_PUPATE = 28,
	MONSTER_ANIM_NOTICE = 29,
	MONSTER_ANIM_SOULSUCK = 30,
	MONSTER_ANIM_IMPALEHORZ = 31,
	MONSTER_ANIM_IMPALEVERT = 32,
	MONSTER_ANIM_SOULOUTGRND = 33,
	MONSTER_ANIM_SOULOUTHORZ = 34,
	MONSTER_ANIM_SOULOUTVERT = 35,
	MONSTER_ANIM_FALLOVER = 36,
	MONSTER_ANIM_EMBRACE = 37,
	MONSTER_ANIM_SHOVE = 38,
	MONSTER_ANIM_FLIPSWITCH = 39,
	MONSTER_ANIM_EVADELEFT = 40,
	MONSTER_ANIM_EVADERIGHT = 41,
	MONSTER_ANIM_ALARM = 42,
	MONSTER_ANIM_DEAF = 43,
	MONSTER_ANIM_BLIND = 44,
	MONSTER_ANIM_SUCKED = 45,
	MONSTER_NUM_ANIMS = 46,
};

// TODO - Reconstruct this enum.
enum MonsterBehavior
{
	MONSTER_NUM_BEHAVIORS = 18
};

struct _MonsterSenses
{
	short sightArc;
	short sightRadius;
	short sightElevation;
	short soundArc;
	short soundRadius;
	short soundElevation;
	unsigned short scentRadius;
	short alarmRadius;
	unsigned short allyRadius;
};

struct _MonsterCombatAttributes
{
	short stunTime;
	short damageTime;
	short recovery;
	short surpriseTime;
	short grabTime;
	short combatRange;
	short surpriseRange;
	short allyRange;
	short enemyAttackRange;
	short enemyRunAttackRange;
	short preferredCombatRange;
	short suckPower;
	short suckRange;
	short suckTime;
	char hitPoints;
	char numAttacks;
	char missileAttack;
	char ambushAttack;
	char evadeProbability[12];
	char attackList[1];
};

struct _MonsterAllegiances
{
	unsigned long enemies;
	unsigned long allies;
	unsigned long food;
	unsigned long gods;
};

struct _MonsterSubAttributes
{
	char* animList;
	struct _MonsterSenses* senses;
	struct _MonsterCombatAttributes* combatAttributes;
	struct _MonsterAllegiances* allegiances;
	char* behaviorList;
	short scale;
	short fallDistance;
	short defAmbushRange;
	short fleeRange;
	short speedPivotTurn;
	short speedWalkTurn;
	short speedRunTurn;
	short speedFleeTurn;
	short forgetTime;
	unsigned char modelNum;
	unsigned char physAbility;
	unsigned char stunnable;
	unsigned char grabable;
	unsigned char numSections;
	unsigned char sectionEnd[3];
	short defPlayerAttitude;
	short defWanderRange;
	short defGuardRange;
	unsigned char defInitialBehavior;
	unsigned char defTriggeredBehavior;
	char defActiveBehavior;
	unsigned char defSpectral;
	unsigned char sunVuln;
	unsigned char fireVuln;
	unsigned char waterVuln;
	unsigned char impaleVuln;
	unsigned char soundVuln;
	unsigned char weaponVuln;
	unsigned char missileVuln;
	unsigned char bruiseRed;
	unsigned char bruiseGreen;
	unsigned char bruiseBlue;
	short minSpikeRange;
	short maxSpikeRange;
	short maxSpikeAngle;
	short minSpikeHorzSpeed;
	short maxSpikeHorzSpeed;
	short maxSpikeVertSpeed;
	short upOnGroundOffset;
	short downOnGroundOffset;
};

struct _MonsterAnim
{
	unsigned short velocity;
	unsigned short distance;
	unsigned short playSpeed;
	unsigned char interpFrames;
	unsigned char startFrame;
	unsigned char interpOut;
	unsigned char alphaTable;
	unsigned char alphaTableOut;
	unsigned char controllingSection;
	char index[3];
	char pad;
};

struct _MonsterIdle
{
	char anim;
	char alertMod;
	char probability;
	char pad;
};

struct _MonsterBehavior
{
	char alertness;
	char idleFreq;
	char numIdles;
	char idleList[5];
};

struct _MonsterAttackAttributes
{
	short attackRange;
	short attackHeight;
	short knockBackDistance;
	char knockBackDuration;
	char damage;
	char sphereSegment;
	char sphereOnFrame;
	char sphereOnAnim;
	char sphereOffFrame;
	char sphereOffAnim;
	char turnFrames;
	char numAnims;
	char attackProbability[12];
	char animList[5];
};

struct _MonsterMissile
{
	unsigned short speed;
	unsigned short range;
	unsigned char frame;
	unsigned char anim;
	unsigned char segment;
	unsigned char damage;
	unsigned char type;
	unsigned char graphic;
	char gravity;
	char reload;
	char fireChance;
	unsigned char numAnims;
	char animList[2];
};

struct FXSplinter
{
	short flags;
	short soundFx;
	short chunkVelXY;
	short chunkVelZ;
	short chunkVelRng;
	short triVelRng;
	short lifetime;
	short faceLimit;
	short rotRateRng;
	short gravityZ;
};

struct _MonsterAttributes
{
	unsigned long magicnum;
	void* tunData;
	char* auxAnimList;
	char* ambientAnimList;
	long whatAmI;
	char numAuxAnims;
	char numAmbientAnims;
	unsigned char defaultAge;
	char pupateObject;
	unsigned char damageFXSegment;
	unsigned char headSegment;
	unsigned char neckSegment;
	unsigned char leftShoulderSegment;
	unsigned char rightShoulderSegment;
	unsigned char waistSegment;
	unsigned char leftKneeSegment;
	unsigned char rightKneeSegment;
	unsigned char leftFootSegment;
	unsigned char rightFootSegment;
	unsigned char spineSegment;
	unsigned char leftWeaponSegment;
	unsigned char rightWeaponSegment;
	unsigned char grabSegment;
	unsigned char bloodImpaleFrame;
	unsigned char bloodConeFrame;
	char numSubAttributes;
	char numCombatAttributes;
	char numAttackAttributes;
	char numMissiles;
	char numAnims;
	char numIdles;
	char numBehaviors;
	char numShatters;
	struct _MonsterSubAttributes** subAttributesList;
	struct _MonsterCombatAttributes** combatAttributesList;
	struct _MonsterAttackAttributes* attackAttributesList;
	struct _MonsterMissile* missileList;
	struct _MonsterAnim* animList;
	struct _MonsterIdle* idleList;
	struct _MonsterBehavior* behaviorList;
	void* shatterList; // FXSplinter*
};

struct _walbossAttributes
{
	unsigned char segmentToAttach;
	unsigned char frameToBirth;
	unsigned char frameToExpel;
	unsigned char maxEggHits;
	short maxAngle;
	short timeForHitAgony;
	short segmentForBodyTwist;
	short timeToThornPrep;
	short timeToThornPoke;
	short timeToEggThrob;
	short timeToEggExplode;
	short razielStunTime;
	short eggIgniteDist;
	short angleForShortAttack;
	short closeAttackStart;
	short junk;
	struct {
		short plusDelta;
		short minusDelta;
		short validAtHitPoint;
	} attackDeltas;
};

struct _MonsterSubAttributes64
{
	union
	{
		char* animList;
		unsigned long long animList_;
	};
	union
	{
		struct _MonsterSenses* senses;
		unsigned long long senses_;
	};
	union
	{
		struct _MonsterCombatAttributes* combatAttributes;
		unsigned long long combatAttributes_;
	};
	union
	{
		struct _MonsterAllegiances* allegiances;
		unsigned long long allegiances_;
	};
	union
	{
		char* behaviorList;
		unsigned long long behaviorList_;
	};
	short scale;
	short fallDistance;
	short defAmbushRange;
	short fleeRange;
	short speedPivotTurn;
	short speedWalkTurn;
	short speedRunTurn;
	short speedFleeTurn;
	short forgetTime;
	unsigned char modelNum;
	unsigned char physAbility;
	unsigned char stunnable;
	unsigned char grabable;
	unsigned char numSections;
	unsigned char sectionEnd[3];
	short defPlayerAttitude;
	short defWanderRange;
	short defGuardRange;
	unsigned char defInitialBehavior;
	unsigned char defTriggeredBehavior;
	char defActiveBehavior;
	unsigned char defSpectral;
	unsigned char sunVuln;
	unsigned char fireVuln;
	unsigned char waterVuln;
	unsigned char impaleVuln;
	unsigned char soundVuln;
	unsigned char weaponVuln;
	unsigned char missileVuln;
	unsigned char bruiseRed;
	unsigned char bruiseGreen;
	unsigned char bruiseBlue;
	short minSpikeRange;
	short maxSpikeRange;
	short maxSpikeAngle;
	short minSpikeHorzSpeed;
	short maxSpikeHorzSpeed;
	short maxSpikeVertSpeed;
	short upOnGroundOffset;
	short downOnGroundOffset;
};

struct _MonsterAttributes64
{
	unsigned long magicnum;
	union
	{
		void* tunData;
		unsigned long long tunData_;
	};
	union
	{
		char* auxAnimList;
		unsigned long long auxAnimList_;
	};
	union
	{
		char* ambientAnimList;
		unsigned long long ambientAnimList_;
	};
	long whatAmI;
	char numAuxAnims;
	char numAmbientAnims;
	unsigned char defaultAge;
	char pupateObject;
	unsigned char damageFXSegment;
	unsigned char headSegment;
	unsigned char neckSegment;
	unsigned char leftShoulderSegment;
	unsigned char rightShoulderSegment;
	unsigned char waistSegment;
	unsigned char leftKneeSegment;
	unsigned char rightKneeSegment;
	unsigned char leftFootSegment;
	unsigned char rightFootSegment;
	unsigned char spineSegment;
	unsigned char leftWeaponSegment;
	unsigned char rightWeaponSegment;
	unsigned char grabSegment;
	unsigned char bloodImpaleFrame;
	unsigned char bloodConeFrame;
	char numSubAttributes;
	char numCombatAttributes;
	char numAttackAttributes;
	char numMissiles;
	char numAnims;
	char numIdles;
	char numBehaviors;
	char numShatters;
	union
	{
		struct _MonsterSubAttributes** subAttributesList;
		unsigned long long subAttributesList_;
	};
	union
	{
		struct _MonsterCombatAttributes** combatAttributesList;
		unsigned long long combatAttributesList_;
	};
	union
	{
		struct _MonsterAttackAttributes* attackAttributesList;
		unsigned long long attackAttributesList_;
	};
	union
	{
		struct _MonsterMissile* missileList;
		unsigned long long missileList_;
	};
	union
	{
		struct _MonsterAnim* animList;
		unsigned long long animList_;
	};
	union
	{
		struct _MonsterIdle* idleList;
		unsigned long long idleList_;
	};
	union
	{
		struct _MonsterBehavior* behaviorList;
		unsigned long long behaviorList_;
	};
	union
	{
		void* shatterList; // FXSplinter
		unsigned long long shatterList_;
	};
};