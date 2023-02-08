#pragma once

struct __Idle 
{
	unsigned char anim;
	unsigned char frame;
	unsigned char frames;
	unsigned char type;
};

struct __AttackItem 
{
	unsigned char anim;
	unsigned char framesIn;
	unsigned char alphaIn;
	unsigned char framesOut;
	unsigned char switchDelay;
	unsigned char ignoreDelay;
	unsigned char segmentToGlow;
	unsigned char startCollisionFrame;
	unsigned char handsToCollide;
	unsigned char endCollisionFrame;
	unsigned char ribbonStartFrame;
	unsigned char ribbonStartSegment;
	unsigned char ribbonEndSegment;
	unsigned char ribbonLifeTime;
	unsigned char ribbonFaceLifeTime;
	unsigned char knockBackFrames;
	unsigned char glowFadeInFrames;
	unsigned char glowFadeOutFrames;
	unsigned short ribbonStartOpacity;
	unsigned long ribbonStartColor;
	unsigned long ribbonEndColor;
	unsigned long glowColor;
	unsigned short knockBackDistance;
	unsigned short hitPowerScale;
};

struct __ThrowItem 
{
	unsigned char prepAnim;
	unsigned char prepAnimFramesIn;
	unsigned char prepAnimAlphaIn;
	unsigned char coilAnim;
	unsigned char coilAnimFramesIn;
	unsigned char coilAnimAlphaIn;
	unsigned char throwAnim;
	unsigned char throwAnimFramesInFactor;
	unsigned char throwFrameToLaunch;
	unsigned char framesOut;
	short launchPointX;
	short launchPointY;
	short launchPointZ;
	short maxXRotation;
	short minXRotation;
	short maxZRotation;
	short minZRotation;
	short velocity;
	short gravity;
	short coilRot;
	short selfAdjustingFlag;
};

struct __VAnim 
{
	unsigned char anim0;
	unsigned char anim1;
	unsigned char anim2;
	unsigned char frames;
	unsigned char mode;
	unsigned char alpha;
	unsigned short frame;
};

struct __SAnim 
{
	struct __VAnim* anim;
	struct __SAnim* nextAnim;
	short mode;
	short data;
	short speedAdjust;
	short pad;
};

struct RazielData 
{
	unsigned long version;
	unsigned long nonBurningRibbonStartColor;
	unsigned long nonBurningRibbonEndColor;
	struct __Idle*** idleList;
	struct __AttackItem*** attackList;
	struct __ThrowItem** throwList;
	struct __VAnim* virtualAnimations;
	struct __VAnim* virtualAnimSingle;
	struct __SAnim** stringAnimations;
	short throwFadeValue;
	short throwFadeInRate;
	int throwFadeOutRate;
	int throwManualDistance;
	short healthMaterialRate;
	short healthSpectralRate;
	short healthInvinciblePostHit;
	short healthInvinciblePostShunt;
	short forceMinPitch;
	short forceMaxPitch;
	short forceMinVolume;
	short forceMaxVolume;
	unsigned long forceRampTime;
	int SwimPhysicsFallDamping;
	int SwimPhysicsWaterFrequency;
	int SwimPhysicsWaterAmplitude;
	int SwimPhysicsUnderDeceleration;
	int SwimPhysicsUnderKickVelocity;
	int SwimPhysicsUnderKickAccel;
	int SwimPhysicsUnderVelocity;
	int SwimPhysicsUnderKickDecel;
	int SwimPhysicsUnderStealthAdjust;
	int SwimPhysicsCoilVelocity;
	int SwimPhysicsCoilDecelerationIn;
	int SwimPhysicsCoilDecelerationOut;
	int SwimPhysicsShotVelocity;
	int SwimPhysicsShotAccelerationIn;
	int SwimPhysicsShotAccelerationOut;
	int SwimPhysicsSurfVelocity;
	int SwimPhysicsSurfAccelerationIn;
	int SwimPhysicsSurfAccelerationOut;
	int SwimPhysicsSurfKickVelocity;
	int SwimPhysicsSurfKickAccel;
	int SwimPhysicsSurfMinRotation;
	int SwimPhysicsSurfMaxRotation;
	int SwimPhysicsSurfKickDecel;
};