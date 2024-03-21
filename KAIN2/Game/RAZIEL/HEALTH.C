#include "Game/CORE.H"
#include "Game/RAZIEL/HEALTH.H"
#include "Game/GAMELOOP.H"
#include "Game/RAZIEL/RAZLIB.H"
#include "Game/FX.H"
#include "Game/RAZIEL/RAZIEL.H"
#include "Game/CAMERA.H"
#include "Game/STATE.H"
#include "Game/GAMEPAD.H"
#include "Game/SOUND.H"

void InitHealthSystem() // Matching - 100%
{
	Raziel.DamageFrequency = 0;
	Raziel.HealthScale = 1;
	Raziel.HealthBalls = 0;
	Raziel.GlyphManaBalls = 0;

	if (gameTrackerX.gameData.asmData.MorphType == 0)
	{
		Raziel.CurrentPlane = 2;
		Raziel.HitPoints = 100000;
		
		razMaterialShift();
	}
	else
	{
		Raziel.CurrentPlane = 1;
		Raziel.HitPoints = GetMaxHealth();
		
		razSpectralShift();
	}

	if (razInBaseArea("under", sizeof("under")-1))
	{
		Raziel.HitPoints = 100;
	}
	else
	{
		razSetPlayerEventHistory(0x1000);
	}
}

void GainHealth(int data)
{
	Raziel.HitPoints += MIN(data * 20000, 4095) >> 12;

	if (Raziel.HitPoints >= GetMaxHealth() && Raziel.CurrentPlane == 1)
	{
		Raziel.HitPoints = GetMaxHealth();

		razReaverOn();
	}
}


void LoseHealth(int amount) // Matching - 100%
{
	if (((!(ControlFlag & 0x1000000)) && (Raziel.invincibleTimer == 0)) && (Raziel.HitPoints > 525))
	{
		Raziel.HitPoints -= (amount * 20000) / 4096;

		Raziel.DamageFrequency -= (amount * 20000) / 4096;

		Raziel.invincibleTimer = PlayerData->healthInvinciblePostHit * 122880;

		if (Raziel.CurrentPlane == 1)
		{
			razReaverOff();

			if (Raziel.soulReaver != NULL)
			{
				INSTANCE_Post(Raziel.soulReaver, 0x800101, 0);

				razReaverImbue(2);
			}
		}

		if (!(gameTrackerX.gameFlags & 0x80))
		{
			GAMEPAD_Shock0(1, 9000);
		}
	}
}

void DrainHealth(int amount) // Matching - 99.15%
{
	if ((ControlFlag & 0x01000000) == 0)
	{
		amount /= 4096;

		if (Raziel.CurrentPlane == 1)
		{
			Raziel.HitPoints += (int)(PlayerData->healthMaterialRate * amount * gameTrackerX.timeMult) / 4096;
			if (Raziel.soulReaver)
			{
				INSTANCE_Post(Raziel.soulReaver, 0x800101, 0);
				razReaverImbue(2);
			}
		}
		else if (Raziel.invincibleTimer == 0)
		{
			if (Raziel.HitPoints >= 526)
			{
				Raziel.HitPoints += (int)(-PlayerData->healthSpectralRate * amount * gameTrackerX.timeMult) / 4096;
			}
		}
	}
}

void BumpUpHealth()
{
	Raziel.HealthScale++;

	Raziel.HitPoints = GetMaxHealth();
}

int GetMaxHealth()
{
	if (Raziel.CurrentPlane != 2)
	{
		return (Raziel.HealthScale + 1) * 100000;
	}
	else
	{
		return 100000;
	}
}

void ProcessHealth(struct _Instance* instance)//Matching - 91.10%
{
	if (Raziel.invincibleTimer || (Raziel.playerEventHistory & 0x1000) == 0)
	{
		Raziel.invincibleTimer -= gameTrackerX.timeMult;
		
		if (Raziel.invincibleTimer < 0)
		{
			Raziel.invincibleTimer = 0;
		}
		
		if (Raziel.CurrentPlane == 1)
		{
			FX_Health_Spiral(Raziel.HealthScale, Raziel.HitPoints - 100000, 100000 * Raziel.HealthScale);
		}
		else
		{
			FX_Health_Spiral(0, Raziel.HitPoints, 100000);
		}
	}
	else
	{
		if (Raziel.HitPoints == GetMaxHealth() || Raziel.CurrentPlane == 2)
		{
			razReaverOn();
		}
		else
		{
			razReaverOff();
		}

		if (Raziel.CurrentPlane == 1)
		{
			if (instance->waterFace && !(Raziel.Abilities & 0x10))
			{
				DrainHealth(40960);
			}

			if (!Raziel.soulReaver || Raziel.HitPoints != GetMaxHealth())
			{
				int temp = (PlayerData->healthMaterialRate * gameTrackerX.timeMult);
				temp = (temp < 0) ? ((temp)+0xFFF) : ((temp));
				Raziel.HitPoints += temp >> 12;
			}

			if (Raziel.HitPoints < 100000)
			{
				razPlaneShift(instance);
				
				Raziel.invincibleTimer = 122880 * PlayerData->healthInvinciblePostShunt;

				if ((Raziel.Mode & 0x40000))
				{
					CAMERA_ChangeToOutOfWater(&theCamera, instance);
				}
			}
			else
			{
				if (Raziel.HitPoints <= 149999)
				{
					int temp = (1000 * gameTrackerX.timeMult);
					temp = (temp < 0) ? ((temp)+0xFFF) : ((temp));
					Raziel.DamageFrequency -= temp >> 12;

					if (Raziel.DamageFrequency < 0)
					{
						Raziel.DamageFrequency = Raziel.HitPoints - 100000;

						if (Raziel.HitPoints - 100000 < 18750)
						{
							Raziel.DamageFrequency = 18750;
						}
						
						FX_DoInstancePowerRing(instance, 750 - 300 * (50000 - Raziel.DamageFrequency) / 31250, 0, 0, 0);
						
						if ((gameTrackerX.gameFlags & 0x80) == 0)
						{
							GAMEPAD_Shock1(128, 20480);
						}

						FX_Health_Spiral(Raziel.HealthScale, Raziel.HitPoints - 100000, 100000 * Raziel.HealthScale);
					}
				}
			}
		}
		else
		{
			if (Raziel.HitPoints < 526)
			{
				int temp = (PlayerData->healthSpectralRate * gameTrackerX.timeMult);
				temp = (temp < 0) ? ((temp)+0xFFF) : ((temp));
				Raziel.HitPoints -= temp >> 12;
			}
			else
			{
				if (Raziel.HitPoints < 100000)
				{
					int temp = (PlayerData->healthSpectralRate * gameTrackerX.timeMult);
					temp = (temp < 0) ? ((temp)+0xFFF) : ((temp));
					Raziel.HitPoints += temp >> 12;
				}
				else
				{
					Raziel.HitPoints = 100000;
				}
			}

			if (!(ControlFlag & 0x800000) && Raziel.HitPoints < 525)
			{
				StateSwitchStateCharacterData(&Raziel.State, StateHandlerIdle, SetControlInitIdleData(0, 0, 3));
				
				G2EmulationSwitchAnimationCharacter(&Raziel.State, 214, 0, 3, 1);
				
				Raziel.HitPoints = 525;

				ControlFlag |= 0x804000;
			}

			if (Raziel.HitPoints < 0)
			{
				gameTracker->streamFlags |= 0x80000;

				if (Raziel.soulReaver)
				{
					INSTANCE_Post(Raziel.soulReaver, 0x800105, 0);
				}

				razSetPlayerEventHistory(0x8000);
				
				Raziel.HitPoints = 50000;
				
				gameTrackerX.gameData.asmData.MorphType = 1;
				
				Raziel.playerEvent |= 0x8000;
				
				razPlayUnderworldSounds(gameTrackerX.playerInstance);
			}

			FX_Health_Spiral(0, Raziel.HitPoints, 100000);
		}
	}
}

int HealthCheckForLowHealth() // Matching - 100%
{
	if (STREAM_IsMorphInProgress() == 0)
	{
		if (Raziel.CurrentPlane == 1)
		{
			if (Raziel.HitPoints <= 100099)
			{
				return 1;
			}
		}
		else if (Raziel.HitPoints < 100)
		{
			return 1;
		}

		return 0;
	}

	return 1;
}

void DrainMana(int amount)
{
	Raziel.GlyphManaBalls -= amount;

	if (Raziel.GlyphManaBalls == 0)
	{
		Raziel.GlyphManaBalls = 0;
	}

	if (Raziel.GlyphManaMax < Raziel.GlyphManaBalls)
	{
		Raziel.GlyphManaBalls = Raziel.GlyphManaMax;
	}
}

void SetMana(int amount)
{ 
	if (amount <= 0)
	{
		Raziel.GlyphManaBalls = 0;
	}
	else
	{
		Raziel.GlyphManaBalls = Raziel.GlyphManaMax;
	}
}

void HealthInstantDeath(struct _Instance* instance) // Matching - 98.91%
{
	gameTrackerX.gameData.asmData.MorphType = 1;
	razSpectralShift();
	Raziel.HitPoints = 50000;
	gameTracker->streamFlags |= 0x80000;
	if (Raziel.soulReaver != 0)
	{
		INSTANCE_Post(Raziel.soulReaver, 0x800105, 0);
	}
	razSetPlayerEventHistory(0x8000);
	Raziel.playerEvent |= 0x8000;
	razPlayUnderworldSounds(gameTrackerX.playerInstance);
}

void RAZIEL_DebugHealthSetScale(long healthScale)//Matching - 99.32%
{
	Raziel.HealthScale = (short)healthScale;

	Raziel.HealthBalls = (short)(5 * (healthScale - 1));

	Raziel.HitPoints = 100000 * (short)healthScale + 100000;
}

void RAZIEL_DebugManaSetMax(long manaScale)
{
	Raziel.GlyphManaMax = (short)(manaScale << 2);
	Raziel.GlyphManaBalls = (short)(manaScale << 2);
}

void RAZIEL_DebugHealthFillUp() // Matching - 100%
{
	if (Raziel.CurrentPlane == 1)
	{
		Raziel.HitPoints = 100000 + (Raziel.HealthScale * 100000);
	}
	else
	{
		Raziel.HitPoints = 100000;
	}
}

void RAZIEL_DebugManaFillUp()
{
	SetMana(1);
}

void razPlayUnderworldSounds(struct _Instance* instance) // Matching - 100%
{
	if (Raziel.soundHandle != 0)
	{
		SndEndLoop(Raziel.soundHandle);

		Raziel.soundHandle = 0;
	}

	razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 26, -250, -250, 120, 120, 0, 3500);

	razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 26, -300, -300, 120, 120, 0, 3500);

	razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 18, -220, -220, 120, 120, 0, 3500);

	razSetupSoundRamp(instance, (struct _SoundRamp*)&Raziel.soundHandle, 26, -250, -250, 120, 120, 0, 3500);
}
