/*
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum spells
{
	// Korthik Slicer
	ARTERIAL_BLEEDING = 123422,
	GOUGE_THROAT = 123434,
	// Srathik Shield Master
	MASS_SPELL_REFLECT = 124817,
	SHIELD_SLAM = 124815,
	SUNDER_ARMOR = 123417,
	// setthik-swiftblade
	SPELL_RIPOSTE = 123436,
	BLURRING_SLASH = 123437,
	// zarthik - supplicant
	SPELL_MASH_AND_GNASH = 124809,
	// Enslaved Bonesmasher
	JAW_BONESLAM = 123490,
	// Zephyr
	ZEPHYR_VISUAL = 123598,
	// Sethik battle mender
	MENDING = 122193,
	// zarthik windblade
	SONIC_BOMB = 125886,
	DISPATCH = 129154,
};
enum Events
{
	// Korthik Silcer
	EVENT_ARTERIAL_BLEEDING = 10,
	EVENT_GOUGE_THROAT = 11,
	// Srathik Shield Master
	EVENT_SHIELD_SLAM = 12,
	//	MASS_SPELL_REFLECT = 13,
	EVENT_SUNDER_ARMOR = 100,
	// setthik-swiftblade
	EVENT_RIPOSTE = 14,
	EVENT_BLURRING_SLASH = 15,
	// zarthik - supplicant
	EVENT_Mash_and_Gnash = 16,
	// enslaved bonesmasher
	EVENT_JAW_BONESLAM = 17,
	// Zar'thik Battle-Mender
	EVENT_MENDING = 18,
	// zarthik windblade
	EVENT_DISPATCH = 580,
	EVENT_SONIC_BLADE = 581,
	EVENT_STICKY_RESIN = 582,
};
enum Creatures
{
	TRIGGER_ZEPHYR = 321435436,
};

class Korthik_Slicer : public CreatureScript
{
public:
	Korthik_Slicer() : CreatureScript("Korthik_Slicer") {}

	struct Korthik_SlicerAI : public ScriptedAI
	{
		Korthik_SlicerAI(Creature* creature) : ScriptedAI(creature)
		{
			me->setFaction(16);
			me->SetLevel(92);
		}

		uint32 gougeThroatTimer;
		uint32 arterialBleedingTimer;

		void Reset()
		{
			gougeThroatTimer = urand(3000, 12000);
			arterialBleedingTimer = urand(1000, 6000);
		}

		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			if (gougeThroatTimer <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
				{
					me->CastSpell(target, GOUGE_THROAT, true);
				}
				gougeThroatTimer = urand(3000, 12000);
			}
			else
				gougeThroatTimer -= diff;

			if (arterialBleedingTimer <= diff)
			{
				// proper for 25 man
				if (Is25ManRaid())
				{ 
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
					{
						me->CastSpell(target, ARTERIAL_BLEEDING, true);
					}			
				}
				else
				{
					for (int i = 0; i <= 3; i++)
					{
						if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
						{
							me->CastSpell(target, ARTERIAL_BLEEDING, true);
						}
					}
				}
				arterialBleedingTimer = urand(1000, 5000);
			}
			else
				arterialBleedingTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Korthik_SlicerAI(creature);
	}
};
class Srathik_Shield_Master : public CreatureScript
{
public:
	Srathik_Shield_Master() : CreatureScript("Srathik_Shield_Master") {}

	struct Srathik_Shield_MasterAI : public ScriptedAI
	{
		Srathik_Shield_MasterAI(Creature* creature) : ScriptedAI(creature)
		{
			me->setFaction(16);
			me->SetLevel(92);
		}

		uint32 shieldSlamTimer;
		uint32 spellReflectTimer;
		uint32 sunderArmorTimer;

		void Reset()
		{
			shieldSlamTimer = urand(4500, 12000);
			spellReflectTimer = urand(30000, 50000); // 1m
			sunderArmorTimer = urand(2000, 5000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			if (shieldSlamTimer <= diff)
			{
				me->CastSpell(me->getVictim(), SHIELD_SLAM, true);
				shieldSlamTimer = urand(8000, 16000);
			}
			else
				shieldSlamTimer -= diff;

			if (spellReflectTimer <= diff)
			{
				DoCastAOE(MASS_SPELL_REFLECT);
				spellReflectTimer = urand(30000, 60000); // 1m
			}
			else
				spellReflectTimer -= diff;

			if (sunderArmorTimer <= diff)
			{
				if (me->getVictim())
				{
					if (!me->getVictim()->HasAura(SUNDER_ARMOR))
					{
						me->CastSpell(me->getVictim(), SUNDER_ARMOR);
					}
					else
					{
						AuraPtr aura_B = me->getVictim()->GetAura(SUNDER_ARMOR);
						if (!aura_B)
							return;

						aura_B->RefreshDuration();

						if (aura_B && aura_B->GetStackAmount() <= 5)
						{
							aura_B->SetStackAmount(aura_B->GetStackAmount() + 1);
						}
					}
				}
				sunderArmorTimer = urand(6000, 8000);
			}
			else
				sunderArmorTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Srathik_Shield_MasterAI(creature);
	}
};
class SetThik_SwiftBlade : public CreatureScript
{
public:
	SetThik_SwiftBlade() : CreatureScript("SetThik_SwiftBlade") {}

	struct SetThik_SwiftBladeAI : public ScriptedAI
	{
		SetThik_SwiftBladeAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(9485820);
				me->SetHealth(9485820);
			}
			else
			{
				me->SetMaxHealth(4215920);
				me->SetHealth(4215920);
			}
			me->setFaction(16);
			me->SetLevel(92);
		}
		uint32 RiposteTimer;
		uint32 BlurringStrikeTimer;
		void Reset()
		{
			RiposteTimer = urand(15000, 15000);
			BlurringStrikeTimer = urand(4000, 9000); // 1m
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			if (BlurringStrikeTimer <= diff)
			{
				me->CastSpell(me->getVictim(), BLURRING_SLASH, true);
				BlurringStrikeTimer = urand(4000, 9000); // 1m
			}
			else
				BlurringStrikeTimer -= diff;

			if (RiposteTimer <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
				{
					me->CastSpell(target, SPELL_RIPOSTE);
					RiposteTimer = urand(15000, 15000);
				}
			}
			else
				RiposteTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new SetThik_SwiftBladeAI(creature);
	}
};
class Sarthik_Supplicants : public CreatureScript
{
public:
	Sarthik_Supplicants() : CreatureScript("Sarthik_Supplicants") {}

	struct Sarthik_SupplicantsAI : public ScriptedAI
	{
		Sarthik_SupplicantsAI(Creature* creature) : ScriptedAI(creature)
		{
			me->SetLevel(92);
			me->setFaction(16);
		}
		uint32 MashGnashTimer;
		void Reset()
		{
			MashGnashTimer = urand(8000, 19000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			if (MashGnashTimer <= diff)
			{
				DoCastAOE(SPELL_MASH_AND_GNASH);
				MashGnashTimer = urand(8000, 19000);
			}
			else
				MashGnashTimer -= diff;
			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Sarthik_SupplicantsAI(creature);
	}
};
class Bone_Smasher : public CreatureScript
{
public:
	Bone_Smasher() : CreatureScript("Bone_Smasher") {}

	struct Bone_SmasherAI : public ScriptedAI
	{
		Bone_SmasherAI(Creature* creature) : ScriptedAI(creature)
		{
			me->SetLevel(92);
			me->setFaction(16);
		}
		uint32 MashGnashTimer;
		void Reset()
		{
			MashGnashTimer = urand(8000, 14000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (MashGnashTimer <= diff)
			{
				me->CastSpell(me->getVictim(), JAW_BONESLAM);
				MashGnashTimer = urand(8000, 14000);
			}
			else
				MashGnashTimer -= diff;
			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Bone_SmasherAI(creature);
	}
};
class Battle_Mender : public CreatureScript
{
public:
	Battle_Mender() : CreatureScript("Battle_Mender") {}
	struct Battle_MenderAI : public ScriptedAI
	{
		Battle_MenderAI(Creature* creature) : ScriptedAI(creature)
		{
			me->SetLevel(92);
			me->setFaction(16);
		}
		uint32 quickeningTimer;
		uint32 mendingTimer;
		void Reset()
		{
			quickeningTimer = 17000;
			mendingTimer = 12000;
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (mendingTimer <= diff)
			{
				switch (urand(0, 1))
				{
				case 0:
					if (Creature* Blade = me->FindNearestCreature(930334, 20.0f, true))
						me->CastSpell(Blade, 122193);
					break;
				case 1:
					if (Creature* Battle = me->FindNearestCreature(63030, 20.0f, true))
						me->CastSpell(Battle, 122193);
					break;
				}
				mendingTimer = 12000;
			}
			else
				mendingTimer -= diff;

			if (quickeningTimer <= diff)
			{
				DoCastAOE(122149);
				quickeningTimer = 17000;
			}
			else
				quickeningTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Battle_MenderAI(creature);
	}
};
class Korthik_Extremist : public CreatureScript
{
public:
	Korthik_Extremist() : CreatureScript("Korthik_Extremist") {}

	struct Korthik_ExtremistAI : public ScriptedAI
	{
		Korthik_ExtremistAI(Creature* creature) : ScriptedAI(creature)
		{
			me->SetLevel(92);
			me->setFaction(16);
		}
		void Reset()
		{
		}
		void EnterCombat(Unit* who)
		{
			if (!me->HasAura(123502))
				me->AddAura(123502, me);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Korthik_ExtremistAI(creature);
	}
};
class Korthik_Zealot : public CreatureScript
{
public:
	Korthik_Zealot() : CreatureScript("Korthik_Zealot") {}

	struct Korthik_ExtremistAI : public ScriptedAI
	{
		Korthik_ExtremistAI(Creature* creature) : ScriptedAI(creature)
		{
			me->SetLevel(92);
			me->setFaction(16);
		}

		uint16 RushTimer;

		void Reset()
		{
			RushTimer = urand(7000, 20000);
		}
		void SpellHitTarget(Unit* target, SpellInfo const* spell)
		{
			if (spell->Id == 123622)
			{
				me->CastSpell(target, 123626, true);
				me->CastSpell(target, 123638, true);
			}
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (RushTimer <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
				{
					if (target->getFaction() != me->getFaction())
					{
						me->CastSpell(target, 123622);
						RushTimer = urand(14000, 14000);
					}
				}
			}
			else
				RushTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Korthik_ExtremistAI(creature);
	}
};

// 2ND QUARTER
class Gustwing_NPC : public CreatureScript
{
public:
	Gustwing_NPC() : CreatureScript("Gustwing_NPC") {}

	struct Korthik_ExtremistAI : public ScriptedAI
	{
		Korthik_ExtremistAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(3301009);
				me->SetHealth(3301009);
			}
			else
			{
				me->SetMaxHealth(1222596);
				me->SetHealth(1222596);
			}
			me->SetLevel(91);
			me->setFaction(16);
		}

		uint16 GustTimer;

		void Reset()
		{
			GustTimer = urand(5000, 20000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (GustTimer <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50, true))
				{
					if (target->getFaction() != me->getFaction())
					{
						me->CastSpell(target, 124072);
						GustTimer = urand(10000, 20000);
					}
				}
			}
			else
				GustTimer -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Korthik_ExtremistAI(creature);
	}
};
class Zephyr_Caster : public CreatureScript
{
public:
	Zephyr_Caster() : CreatureScript("Zephyr_Caster") {}

	struct Zephyr_CasterAI : public ScriptedAI
	{
		Zephyr_CasterAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(17074476);
				me->SetHealth(17074476);
			}
			else
			{
				me->SetMaxHealth(6323880);
				me->SetHealth(6323880);
			}
			me->SetLevel(91);
			me->setFaction(16);
		}

		uint16 ZephyrCast;

		void Reset()
		{
			ZephyrCast = urand(10000, 20000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (ZephyrCast <= diff)
			{
				me->CastSpell(me, 124085);
				ZephyrCast = urand(10000, 20000);
			}
			else
				ZephyrCast -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new Zephyr_CasterAI(creature);
	}
};
class korthik_swarmguard : public CreatureScript
{
public:
	korthik_swarmguard() : CreatureScript("korthik_swarmguard") {}

	struct korthik_swarmguardAI : public ScriptedAI
	{
		korthik_swarmguardAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(18971640);
				me->SetHealth(18971640);
			}
			else
			{
				me->SetMaxHealth(6323880);
				me->SetHealth(6323880);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}

		void Reset()
		{
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			// Seperation anxiety
			if (!me->FindNearestCreature(64917, 14.0, true))
			{
				if (!me->HasAura(126933))
				{
					me->AddAura(126933, me);
				}
			}
			DoMeleeAttackIfReady();
		}
		void JustDied(Unit* killer)
		{
			if (Creature* AmberCaller = me->FindNearestCreature(64917, 50.0f, true))
			{
				if (me->HasAura(126926))
				{
					AmberCaller->RemoveAura(126926);
				}

				AmberCaller->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			}
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new korthik_swarmguardAI(creature);
	}
};
class korthik_ambercaller : public CreatureScript
{
public:
	korthik_ambercaller() : CreatureScript("korthik_ambercaller") {}

	struct korthik_ambercallerAI : public ScriptedAI
	{
		korthik_ambercallerAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(11382984);
				me->SetHealth(11382984);
			}
			else
			{
				me->SetMaxHealth(4215920);
				me->SetHealth(4215920);
			}
			me->SetLevel(92);

			me->setFaction(16);
			me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
			me->SetSpeed(MOVE_RUN, 0.0f, true);
		}

		uint16 AmberVolley;

		void Reset()
		{
			AmberVolley = urand(8000, 16000);
			me->CastSpell(me, 126926); // immune
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			// amber volley
			if (AmberVolley <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
				{
					me->CastSpell(target, 126941);
					AmberVolley = urand(8000, 14000);
				}
			}
			else
				AmberVolley -= diff;

		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new korthik_ambercallerAI(creature);
	}
};
class korthik_fleshrender : public CreatureScript
{
public:
	korthik_fleshrender() : CreatureScript("korthik_fleshrender") {}

	struct korthik_fleshrenderAI : public ScriptedAI
	{
		korthik_fleshrenderAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(28457460);
				me->SetHealth(28457460);
			}
			else
			{
				me->SetMaxHealth(10539800);
				me->SetHealth(10539800);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		// 126907
		uint16 GriveousWhirl_Bleed;
		// 126901
		uint16 Mortal_Rend;

		void Reset()
		{
			GriveousWhirl_Bleed = urand(20000, 20000);
			Mortal_Rend = urand(6000, 8000);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;
			// GiverousWhirl
			if (GriveousWhirl_Bleed <= diff)
			{
				me->CastSpell(me, 126907);
				GriveousWhirl_Bleed = 14000;
			}
			else
				GriveousWhirl_Bleed -= diff;
			// Mortal rend
			if (Mortal_Rend <= diff)
			{
				me->CastSpell(me->getVictim(), 126901);
				Mortal_Rend = urand(6000, 8000);
			}
			else
				Mortal_Rend -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new korthik_fleshrenderAI(creature);
	}
};
class pool_tender : public CreatureScript
{
public:
	pool_tender() : CreatureScript("pool_tender") {}

	struct pool_tenderAI : public ScriptedAI
	{
		pool_tenderAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(12647760);
				me->SetHealth(12647760);
			}
			else
			{
				me->SetMaxHealth(4215920);
				me->SetHealth(4215920);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		void Reset()
		{
		}
		void EnterCombat(Unit* who)
		{
			me->AddAura(125426, me);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new pool_tenderAI(creature);
	}
};
class amber_ridden : public CreatureScript
{
public:
	amber_ridden() : CreatureScript("amber_ridden") {}

	struct amber_riddenAI : public ScriptedAI
	{
		amber_riddenAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(22765968);
				me->SetHealth(22765968);
			}
			else
			{
				me->SetMaxHealth(8431840);
				me->SetHealth(8431840);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		// 125069
		uint16 amberspew;
		// 125081
		uint16 slam;

		void Reset()
		{
			slam = 3500;
			amberspew = urand(7000, 9000);
		}
		void JustDied(Unit* killer)
		{
			me->CastSpell(me, 125054);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (slam <= diff)
			{
				me->CastSpell(me, 125081);
				slam = urand(18000, 20000);
			}
			else
				slam -= diff;

			if (amberspew <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
				{
					me->CastSpell(target, 125069);
				}
				amberspew = 12000;
			}
			else
				amberspew -= diff;


			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new amber_riddenAI(creature);
	}
};
class searstring : public CreatureScript
{
public:
	searstring() : CreatureScript("searstring") {}

	struct searstring_AI : public ScriptedAI
	{
		searstring_AI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(45531936);
				me->SetHealth(45531936);
			}
			else
			{
				me->SetMaxHealth(16863680);
				me->SetHealth(16863680);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		// 125490
		uint16 burningSting;
		// 125480
		uint16 slash;

		void Reset()
		{
			slash = 18000;
			burningSting = urand(7000, 12000);
		}
		void JustDied(Unit* killer)
		{
			me->CastSpell(me, 125054);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (slash <= diff)
			{
				me->CastSpell(me->getVictim(), 125480);
				slash = urand(18000, 20000);
			}
			else
				slash -= diff;

			if (burningSting <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
				{
					me->CastSpell(target, 125490);
				}
				burningSting = urand(7000, 12000);
			}
			else
				burningSting -= diff;


			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new searstring_AI(creature);
	}
};
class coagulated_ooze : public CreatureScript
{
public:
	coagulated_ooze() : CreatureScript("coagulated_ooze") {}

	struct coagulated_oozeAI : public ScriptedAI
	{
		coagulated_oozeAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(1181823);
				me->SetHealth(1181823);
			}
			else
			{
				me->SetMaxHealth(3794328);
				me->SetHealth(3794328);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		void Reset()
		{
		}
		void JustDied(Unit* killer)
		{
			me->CastSpell(me, 125054);
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new coagulated_oozeAI(creature);
	}
};
// SPELLS
class Zephyr_NPC : public CreatureScript
{
public:
	Zephyr_NPC() : CreatureScript("Zephyr_NPC") { }

	struct npc_tornado_lord_tayakAI : public CreatureAI
	{
		npc_tornado_lord_tayakAI(Creature* creature) : CreatureAI(creature)
		{
			me->SetObjectScale(1.0f);
		}
		uint32 despawnTime;
		void Reset()
		{
			despawnTime = 6000;

			me->SetSpeed(MOVE_RUN, 3.0F, true);
			me->setFaction(me->getFaction());
			me->AddAura(ZEPHYR_VISUAL, me);

			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
			{
				if (MotionMaster* motion = me->GetMotionMaster())
					motion->MovePoint(1, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
			}
		}

		void UpdateAI(uint32 const diff)
		{
			if (despawnTime <= diff)
			{
				me->DespawnOrUnsummon();
			}
			else
				despawnTime -= diff;

			events.Update(diff);

			std::list<Player*> PL_list;

			JadeCore::AnyPlayerInObjectRangeCheck check(me, 1.4f);
			JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, PL_list, check);
			me->VisitNearbyObject(1.4f, searcher);

			for (std::list<Player*>::const_iterator it = PL_list.begin(); it != PL_list.end(); ++it)
			{
				if (!(*it))
					return;

				(*it)->KnockbackFrom(me->GetPositionX(), me->GetPositionY(), 0, 11);
				me->DealDamage((*it), 6000, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			}
		}
	private:
		EventMap events;
		std::list<Player*> Player_List;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_tornado_lord_tayakAI(creature);
	}
};
class Summon_Zephyr : public SpellScriptLoader
{
public:
	Summon_Zephyr() : SpellScriptLoader("Summon_Zephyr") {}

	class Summon_ZephyrSP : public SpellScript
	{
		PrepareSpellScript(Summon_ZephyrSP);

		void HAfterCast()
		{
			if (!GetCaster())
				return;

			if (Creature* Mob = GetCaster()->ToCreature())
			{
				Creature* Zephyr = Mob->SummonCreature(TRIGGER_ZEPHYR, Mob->GetPositionX(), Mob->GetPositionY(), Mob->GetPositionZ(), Mob->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 1200);

				if (Zephyr)
					Zephyr->setFaction(GetCaster()->getFaction());
			}
		}
		void Register()
		{
			AfterCast += SpellCastFn(Summon_ZephyrSP::HAfterCast);
		}
	};
	SpellScript* GetSpellScript() const
	{
		return new Summon_ZephyrSP();
	}
};
class auguster : public CreatureScript
{
public:
	auguster() : CreatureScript("auguster") {}

	struct augusterAI : public ScriptedAI
	{
		augusterAI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(28457460);
				me->SetHealth(28457460);
			}
			else
			{
				me->SetMaxHealth(10118208);
				me->SetHealth(10118208);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}

		uint16 hivebomb;
		uint16 hivejuice;

		void EnterCombat(Unit* who)
		{
		}
		void Reset()
		{
			hivebomb = urand(8000, 14000);
			hivejuice = urand(19000, 25000);
		}
		void JustDied(Unit* killer)
		{
			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
			{
				me->CastSpell(target, 125422);
			}
		}
		void UpdateAI(const uint32 diff)
		{
			if (!UpdateVictim())
				return;

			if (hivebomb <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
				{
					me->CastSpell(target, 125865);
					hivebomb = urand(8000, 14000);
				}
			}
			else
				hivebomb -= diff;

			if (hivejuice <= diff)
			{
				if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, true))
				{
					me->CastSpell(target, 125871);
					hivejuice = urand(19000, 25000);
				}
			}
			else
				hivejuice -= diff;

			DoMeleeAttackIfReady();
		}
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new augusterAI(creature);
	}
};
class windblade_trash : public CreatureScript
{
public:
	windblade_trash() : CreatureScript("windblade_trash") {}

	struct windblade_AI : public ScriptedAI
	{
		windblade_AI(Creature* creature) : ScriptedAI(creature)
		{
			if (Is25ManRaid())
			{
				me->SetMaxHealth(25295520);
				me->SetHealth(25295520);
			}
			else
			{
				me->SetMaxHealth(8431840);
				me->SetHealth(8431840);
			}
			me->SetLevel(92);
			me->setFaction(16);
		}
		void EnterCombat(Unit* who)
		{
			events.ScheduleEvent(EVENT_SONIC_BLADE, urand(6000, 10000));
			events.ScheduleEvent(DISPATCH, 21000);
		}
		void Reset()
		{

		}
		void UpdateAI(const uint32 diff)
		{
			events.Update(diff);

			if (!UpdateVictim())
				return;

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SONIC_BLADE:
					me->CastSpell(me->getVictim(), SONIC_BOMB);
					events.ScheduleEvent(EVENT_SONIC_BLADE, urand(6000, 10000));
					break;
				case EVENT_DISPATCH:
					me->AddAura(DISPATCH, me);
					events.ScheduleEvent(DISPATCH, 21000);
					break;
				}
			}

			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
	};
	CreatureAI* GetAI(Creature* creature) const
	{
		return new windblade_AI(creature);
	}
};
void AddSC_Heart_Of_Fear()
{
	new Sarthik_Supplicants();
	new Battle_Mender();
	new SetThik_SwiftBlade();
	new Korthik_Slicer();
	new Srathik_Shield_Master();
	new Bone_Smasher();
	new Korthik_Zealot();
	new Korthik_Extremist();
	new korthik_fleshrender();
	new korthik_ambercaller();
	new korthik_swarmguard();
	new coagulated_ooze();
	new searstring();
	new amber_ridden();
	new pool_tender();
	new windblade_trash();
	new auguster();

	// SPELLS
	new Summon_Zephyr();
	new Zephyr_NPC();
}