/*
* Copyright (C) 2015-2016 JadeCore <https://github.com/cooler-SAI/JadeCore548-patched>
* Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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
#include "siege_of_orgrimmar.h"

enum eSpells
{
	// Stances
	SPELL_BATTLE_STANCE			= 143589,
	SPELL_BERSERKER_STANCE		= 143594,
	SPELL_DEFENSIVE_STANCE		= 143593,

	// Normal Spells
	SPELL_SUNDERING_BLOW		= 143494,
	SPELL_BONECRACKER			= 143638,
	SPELL_COOLING_OFF			= 143484,
	SPELL_EXECUTE				= 143502,

	// Rage Spells
	SPELL_HEROIC_SHOCKWAVE		= 143716,
	SPELL_AFTERSHOCK			= 143712,
	SPELL_KORKRON_BANNER		= 143591,
	SPELL_WAR_SONG				= 143503,
	SPELL_RAVAGER				= 143872,
	SPELL_RAVAGER_VISUAL		= 143874,
	SPELL_KORKRON_BANNER_AURA	= 143536,

	// Kor'kron Ironblade
	SPELL_IRONSTORM				= 143420,
	SPELL_LAST_STAND			= 143427,

	// Kor'kron Arcweaver
	SPELL_ARCANE_SHOCK			= 143432,
	SPELL_MAGISTRIKE			= 143431,
	SPELL_UNSTABLE_BLINK		= 143471,

	// Kor'kron Assassins
	SPELL_STEALTH				= 118969,
	SPELL_BACKSTAB				= 143481,
	SPELL_ASSASSINS_MARK		= 143480,

	// Kor'kron Warshamans
	SPELL_EARTH_SHIELD			= 143475,
	SPELL_EMPOWERED_CHAIN_HEAL	= 143473,
	SPELL_HEALING_TIDE_TOTEM	= 143474,
	SPELL_HEALING_TIDE			= 143477,
};

enum eEvents
{
	EVENT_BATTLE_STANCE		 = 1,
	EVENT_BERSERKER_STANCE	 = 2,
	EVENT_DEFFENSIVE_STANCE	 = 3,
	EVENT_DAMAGE_TAKEN		 = 4,

	// Rage Abilities
	EVENT_HEROIC_SHOCKWAVE	 = 5,
	EVENT_KORKRON_BANNER	 = 6,
	EVENT_WAR_SONG			 = 7,
	EVENT_RAVAGER			 = 8,
	EVENT_AFTERSHOCK		 = 9,

	EVENT_SUMMON_ADDS_ONE	 = 10,
	EVENT_SUMMON_ADDS_TWO	 = 11,
	EVENT_SUNDERING_BLOW	 = 12,
	EVENT_BONECRACKER		 = 13,

	EVENT_BATTLE_STANCE_RAGE = 14,
	EVENT_HEROIC_SHOCKWAVE_J = 15,

	EVENT_IRONSTORM			 = 16,
	EVENT_LAST_STAND		 = 17,

	EVENT_ARCANE_SHOCK		 = 18,
	EVENT_MAGISTRIKE		 = 19,
	EVENT_UNSTABLE_BLINK	 = 20,
	
	EVENT_BACKSTAB			 = 21,

	EVENT_EARTH_SHIELD		 = 22,
	EVENT_CHAIN_HEAL		 = 23,
	EVENT_HEALING_TIDE_TOTEM = 24,

	EVENT_EXECUTE			 = 25,
};

enum eCreatures
{
	CREATURE_KORKRON_IRONBLADE	= 71516,
	CREATURE_KORKRON_ASSASSINS	= 71518,
	CREATURE_KORKRON_ARCWEAVER	= 71517,
	CREATURE_KORKRON_WARSHAMAN	= 71519,
	CREATURE_KORKRON_SNIPERS	= 71656,
};

enum eSays
{
};

void AddRage(Unit* me,uint32 p_BaseValue, uint64 nazgrimGuid)
{
    if (nazgrimGuid == NULL)
        return;

    if (Unit* generalNazgrim = sObjectAccessor->GetCreature(*me, nazgrimGuid))
    {
        uint32 power = generalNazgrim->GetPower(Powers(POWER_RAGE));

		generalNazgrim->SetPower(Powers(POWER_RAGE), power + p_BaseValue);
    }
}

void RemoveRage(Unit* me, uint32 p_BaseValue, uint64 nazgrimGuid)
{
	if (nazgrimGuid == NULL)
		return;

	if (Unit* generalNazgrim = sObjectAccessor->GetCreature(*me, nazgrimGuid))
	{
		uint32 power = generalNazgrim->GetPower(Powers(POWER_RAGE));

		generalNazgrim->SetPower(Powers(POWER_RAGE), power - p_BaseValue);
	}
}

// 71515 - General Nazgrim
class boss_general_nazgrim : public CreatureScript
{
	public:
		boss_general_nazgrim() : CreatureScript("boss_general_nazgrim") { }

		struct boss_general_nazgrimAI : public BossAI
		{
			boss_general_nazgrimAI(Creature* creature) : BossAI(creature, DATA_GENERAL_NAZGRIM)
			{
				pInstance = creature->GetInstanceScript();
			}
			
			InstanceScript* pInstance;
			uint32 nazgrimGuid = me->GetGUID();
			
			void Reset()
			{
				_Reset();
				
				events.Reset();
				
				summons.DespawnAll();
				
				if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

				me->setFaction(16);
				
				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(349523325);
						me->SetHealth(349523325);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(523325523);
						me->SetHealth(523325523);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(976523325);
						me->SetHealth(976523325);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(1523325523);
						me->SetHealth(1523325523);
						break;
				}

				me->SetPower(POWER_RAGE, 0);
				me->SetInt32Value(UNIT_FIELD_POWER1, 0);
				me->SetMaxPower(POWER_RAGE, 1000);
				me->SetInt32Value(UNIT_FIELD_MAXPOWER1, 1000);
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
				_EnterCombat();
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, IN_PROGRESS);
                }

				events.ScheduleEvent(EVENT_BATTLE_STANCE, 0);
				events.ScheduleEvent(EVENT_SUMMON_ADDS_ONE, 45000);
				events.ScheduleEvent(EVENT_SUNDERING_BLOW, urand(5000, 10000));
				events.ScheduleEvent(EVENT_BONECRACKER, 30000);

				if (me->GetMap()->IsHeroic())
					events.ScheduleEvent(EVENT_EXECUTE, 15000);
            }
			
			void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
			
			void KilledUnit(Unit* who)
            {
            }
			
			void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_GENERAL_NAZGRIM, DONE);
                }
            }
			
			void DamageTaken(Unit* attacker, uint32& damage)
			{
				if (me->HasAura(SPELL_DEFENSIVE_STANCE) && attacker->HasAura(SPELL_SUNDERING_BLOW))
					return;
				else if (me->HasAura(SPELL_DEFENSIVE_STANCE) && !attacker->HasAura(SPELL_SUNDERING_BLOW))
					events.ScheduleEvent(EVENT_DAMAGE_TAKEN, 1000); // Made it as event because rage gain can occur only once per second
			}

			int GetRand()
			{
				if (roll_chance_i(52))
					return CREATURE_KORKRON_IRONBLADE;
				if (roll_chance_i(51))
					return CREATURE_KORKRON_ASSASSINS;
				if (roll_chance_i(49))
					return CREATURE_KORKRON_ARCWEAVER;
				if (roll_chance_i(48))
					return CREATURE_KORKRON_WARSHAMAN;

				if (me->GetMap()->IsHeroic())
					if (roll_chance_i(50))
						return CREATURE_KORKRON_SNIPERS;
			}

			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
				uint32 RageAmount = me->GetPower(Powers(POWER_RAGE));

				if (RageAmount <= 490 && RageAmount >= 300)
				{
					if (RageAmount > 490)
						return;

					if (me->HasAura(SPELL_COOLING_OFF))
						return;

					events.ScheduleEvent(EVENT_HEROIC_SHOCKWAVE_J, 0);
				}

				if (RageAmount <= 740 && RageAmount >= 500)
				{
					if (me->HasAura(SPELL_COOLING_OFF))
						return;

					events.ScheduleEvent(EVENT_KORKRON_BANNER, 1000);
				}

				if (RageAmount <= 990 && RageAmount >= 750)
				{
					if (me->HasAura(SPELL_COOLING_OFF))
						return;

					events.ScheduleEvent(EVENT_WAR_SONG, 1000);
				}

				if (RageAmount == 1000)
				{
					if (me->HasAura(SPELL_COOLING_OFF))
						return;

					events.ScheduleEvent(EVENT_RAVAGER, 1000);
				}

				switch (events.ExecuteEvent())
				{
					case EVENT_BATTLE_STANCE:
					{
						if (me->HasAura(SPELL_DEFENSIVE_STANCE))
							me->RemoveAura(SPELL_DEFENSIVE_STANCE);

						DoCast(me, SPELL_BATTLE_STANCE);
						events.ScheduleEvent(EVENT_BATTLE_STANCE_RAGE, 0);
						events.ScheduleEvent(EVENT_BERSERKER_STANCE, 60000);
						break;
					}

					case EVENT_BATTLE_STANCE_RAGE:
					{
						if (me->HasAura(SPELL_BATTLE_STANCE))
						{
							AddRage(me, 10, me->GetGUID());
							events.ScheduleEvent(EVENT_BATTLE_STANCE_RAGE, 1000);
						}

						break;
					}

					case EVENT_BERSERKER_STANCE:
					{
						DoCast(me, SPELL_BERSERKER_STANCE);
						events.ScheduleEvent(EVENT_DEFFENSIVE_STANCE, 60000);
						break;
					}

					case EVENT_DEFFENSIVE_STANCE:
					{
						DoCast(me, SPELL_DEFENSIVE_STANCE);
						events.ScheduleEvent(EVENT_BATTLE_STANCE, 60000);
						break;
					}

					case EVENT_DAMAGE_TAKEN:
					{
						if (me->HasAura(SPELL_DEFENSIVE_STANCE))
							AddRage(me, 50, me->GetGUID());

						break;
					}

					case EVENT_HEROIC_SHOCKWAVE_J: // Hackfix for jump...
					{
						if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 40.0f, true))
						{
							float posX = target->GetPositionX();
							float posY = target->GetPositionY();
							float posZ = target->GetPositionZ();
							float posO = target->GetOrientation();
							me->GetMotionMaster()->MoveJump(posX, posY, posZ, 30.0f, 15.0f);
						}

						events.ScheduleEvent(EVENT_HEROIC_SHOCKWAVE, 1000);
						RemoveRage(me, 300, me->GetGUID());
						break;
					}

					case EVENT_HEROIC_SHOCKWAVE:
					{
						if (me->HasAura(SPELL_COOLING_OFF))
							return;

						DoCast(me, SPELL_HEROIC_SHOCKWAVE);
						DoCast(me, SPELL_COOLING_OFF);

						break;
					}

					case EVENT_KORKRON_BANNER:
					{
						if (me->HasAura(SPELL_COOLING_OFF))
							return;

						DoCast(SPELL_KORKRON_BANNER);
						DoCast(me, SPELL_COOLING_OFF);
						
						break;
					}

					case EVENT_WAR_SONG:
					{
						if (me->HasAura(SPELL_COOLING_OFF))
							return;

						DoCast(SPELL_WAR_SONG);
						DoCast(me, SPELL_COOLING_OFF);

						break;
					}

					case EVENT_RAVAGER:
					{
						if (me->HasAura(SPELL_COOLING_OFF))
							return;

						if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 40.0f, true))
						{
							DoCast(target, SPELL_RAVAGER);
						}

						DoCast(me, SPELL_COOLING_OFF);
						break;
					}

					case EVENT_SUMMON_ADDS_ONE:
					{
						float posX = me->GetPositionX();
						float posY = me->GetPositionY();
						float posZ = me->GetPositionZ();
						float posO = me->GetOrientation();

						Position pos[2] =
						{
							{ posX-5.0f, posY-5.0f, posZ, posO },
							{ posX+5.0f, posY+5.0f, posZ, posO },
						};

						me->SummonCreature(GetRand(), pos[0], TEMPSUMMON_MANUAL_DESPAWN);
						me->SummonCreature(GetRand(), pos[1], TEMPSUMMON_MANUAL_DESPAWN);

						events.ScheduleEvent(EVENT_SUMMON_ADDS_TWO, 45000);
						break;
					}

					case EVENT_SUMMON_ADDS_TWO:
					{
						float posX = me->GetPositionX();
						float posY = me->GetPositionY();
						float posZ = me->GetPositionZ();
						float posO = me->GetOrientation();

						Position pos[2] =
						{
							{ posX - 5.0f, posY - 5.0f, posZ, posO },
							{ posX + 5.0f, posY + 5.0f, posZ, posO },
						};

						me->SummonCreature(GetRand(), pos[0], TEMPSUMMON_MANUAL_DESPAWN);
						me->SummonCreature(GetRand(), pos[1], TEMPSUMMON_MANUAL_DESPAWN);

						events.ScheduleEvent(EVENT_SUMMON_ADDS_ONE, 45000);
						break;
					}

					case EVENT_SUNDERING_BLOW:
					{
						DoCastVictim(SPELL_SUNDERING_BLOW);

						events.ScheduleEvent(EVENT_SUNDERING_BLOW, urand(5000, 10000));
						break;
					}

					case EVENT_BONECRACKER:
					{
						if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 40.0f, true))
						{
							DoCast(target, SPELL_BONECRACKER);
						}

						events.ScheduleEvent(EVENT_BONECRACKER, 30000);
						break;
					}

					case EVENT_EXECUTE:
					{
						DoCastVictim(SPELL_EXECUTE);

						events.ScheduleEvent(EVENT_EXECUTE, 15000);
						break;
					}
				}

				DoMeleeAttackIfReady();
			}
		};

        CreatureAI* GetAI(Creature* pCreature) const
		{
			return new boss_general_nazgrimAI(pCreature);
		}
};

// Done
class mob_orgrimmar_faithful : public CreatureScript
{
    public:
        mob_orgrimmar_faithful() : CreatureScript("mob_orgrimmar_faithful") { }

        struct mob_orgrimmar_faithfulAI: public ScriptedAI
        { 
            mob_orgrimmar_faithfulAI(Creature* creature) :  ScriptedAI(creature)
            { 
                pInstance = creature->GetInstanceScript(); 
            }

            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();
				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(2000000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(2000000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(5500000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(5500000);
						break;
				}

				me->setFaction(16);
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
            return new mob_orgrimmar_faithfulAI(creature);
        }
};

class mob_korkron_ironblade : public CreatureScript
{
    public:
        mob_korkron_ironblade() : CreatureScript("mob_korkron_ironblade") { }

        struct mob_korkron_ironbladeAI : public ScriptedAI
        {
            mob_korkron_ironbladeAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();

				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(8000000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(9300000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(22000000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(32000000);
						break;
				}

				events.ScheduleEvent(EVENT_IRONSTORM, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

				if (me->HasUnitState(UNIT_STATE_CASTING))
					return;

				events.Update(diff);

				if (HealthBelowPct(50))
				{
					if (me->HasAura(SPELL_LAST_STAND))
						return;

					events.ScheduleEvent(EVENT_LAST_STAND, 0);
				}

				switch (events.ExecuteEvent())
				{
					case EVENT_IRONSTORM:
					{
						DoCast(me, SPELL_IRONSTORM);
						events.ScheduleEvent(EVENT_IRONSTORM, 70000);
						break;
					}

					case EVENT_LAST_STAND:
					{
						DoCast(me, SPELL_LAST_STAND);
						break;
					}
				}
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_ironbladeAI(creature);
        }
};

class mob_korkron_arcweaver : public CreatureScript
{
    public:
        mob_korkron_arcweaver() : CreatureScript("mob_korkron_arcweaver") { }

        struct mob_korkron_arcweaverAI : public ScriptedAI
        {
            mob_korkron_arcweaverAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(6000000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(7400000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(17000000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(25600000);
						break;
				}

				events.ScheduleEvent(EVENT_ARCANE_SHOCK, 5000);
				events.ScheduleEvent(EVENT_MAGISTRIKE, 20000);
				events.ScheduleEvent(EVENT_UNSTABLE_BLINK, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

				if (me->HasUnitState(UNIT_STATE_CASTING))
					return;

				events.Update(diff);

				switch (events.ExecuteEvent())
				{
					case EVENT_ARCANE_SHOCK:
					{
						DoCastVictim(SPELL_ARCANE_SHOCK);

						events.ScheduleEvent(EVENT_ARCANE_SHOCK, 5000);
						break;
					}

					case EVENT_MAGISTRIKE:
					{
						DoCastVictim(SPELL_MAGISTRIKE);
						
						events.ScheduleEvent(EVENT_MAGISTRIKE, 20000);
						break;
					}

					case EVENT_UNSTABLE_BLINK:
					{
						DoCast(SPELL_UNSTABLE_BLINK);

						events.ScheduleEvent(EVENT_UNSTABLE_BLINK, 10000);
						break;
					}
				}
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_arcweaverAI(creature);
        }
};

class mob_korkron_assassin : public CreatureScript
{
    public:
        mob_korkron_assassin() : CreatureScript("mob_korkron_assassin") { }

        struct mob_korkron_assassinAI : public ScriptedAI
        {
            mob_korkron_assassinAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(7000000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(8400000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(20000000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(29500000);
						break;
				}

				DoCast(me, SPELL_STEALTH);
				events.ScheduleEvent(EVENT_BACKSTAB, 5000);
				if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 40.0f, true))
					DoCast(target, SPELL_ASSASSINS_MARK);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
					return;

				switch (events.ExecuteEvent())
				{
					case EVENT_BACKSTAB:
					{
						DoCastVictim(SPELL_BACKSTAB);

						events.ScheduleEvent(EVENT_BACKSTAB, 5000);
						break;
					}
				}

				DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_assassinAI(creature);
        }
};

class mob_korkron_warshaman : public CreatureScript
{
    public:
        mob_korkron_warshaman() : CreatureScript("mob_korkron_warshaman") { }

        struct mob_korkron_warshamanAI : public ScriptedAI
        {
            mob_korkron_warshamanAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(7200000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(8900000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(20000000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(31000000);
						break;
				}

				events.ScheduleEvent(EVENT_EARTH_SHIELD, 10000);
				events.ScheduleEvent(EVENT_CHAIN_HEAL, 3000);
				events.ScheduleEvent(EVENT_HEALING_TIDE_TOTEM, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
				if (me->HasUnitState(UNIT_STATE_CASTING))
					return;

				switch (events.ExecuteEvent())
				{
					case EVENT_EARTH_SHIELD:
					{
						if (Unit* target = DoSelectLowestHpFriendly(40.0f))
							DoCast(target, SPELL_EARTH_SHIELD);

						events.ScheduleEvent(EVENT_EARTH_SHIELD, 10000);
						break;
					}

					case EVENT_CHAIN_HEAL:
					{
						if (Unit* target = DoSelectLowestHpFriendly(40.0f))
							DoCast(target, SPELL_EMPOWERED_CHAIN_HEAL);

						events.ScheduleEvent(EVENT_CHAIN_HEAL, 3000);
						break;
					}

					case EVENT_HEALING_TIDE_TOTEM:
					{
						DoCast(SPELL_HEALING_TIDE_TOTEM);

						events.ScheduleEvent(EVENT_HEALING_TIDE_TOTEM, 15000);
						break;
					}
				}
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_warshamanAI(creature);
        }
};

// Done
class mob_aftershock : public CreatureScript
{
    public:
		mob_aftershock() : CreatureScript("mob_aftershock") { }

        struct mob_aftershockAI : public ScriptedAI
        {
			mob_aftershockAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
				me->SetReactState(ReactStates::REACT_PASSIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE);
				me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);

				DoCast(SPELL_AFTERSHOCK);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_aftershockAI(creature);
        }
};

// Done
class mob_korkron_banner : public CreatureScript
{
    public:
		mob_korkron_banner() : CreatureScript("mob_korkron_banner") { }

        struct mob_korkron_bannerAI : public ScriptedAI
        {
			mob_korkron_bannerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();

				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(1700000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(1700000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(2500000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(2500000);
						break;
				}

				DoCast(me, SPELL_KORKRON_BANNER_AURA);
				me->SetReactState(ReactStates::REACT_PASSIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
				me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korkron_bannerAI(creature);
        }
};

// Done
class mob_healing_tide_totem : public CreatureScript
{
    public:
		mob_healing_tide_totem() : CreatureScript("mob_healing_tide_totem") { }

        struct mob_healing_tide_totemAI : public ScriptedAI
        {
			mob_healing_tide_totemAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

            void Reset()
            {
                events.Reset();

				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(1200000);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(1200000);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(2300000);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(2300000);
						break;
				}

				me->SetReactState(ReactStates::REACT_PASSIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
				me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);

				DoCast(me, SPELL_HEALING_TIDE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_healing_tide_totemAI(creature);
        }
};

// Done
class mob_ravager : public CreatureScript
{
    public:
		mob_ravager() : CreatureScript("mob_ravager") { }

        struct mob_ravagerAI : public ScriptedAI
        {
			mob_ravagerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
				me->SetReactState(ReactStates::REACT_PASSIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
				me->GetMotionMaster()->MoveRandom(40.0f);

				DoCast(me, SPELL_RAVAGER_VISUAL);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ravagerAI(creature);
        }
};

// Done
class spell_war_song : public SpellScriptLoader
{
	public:
		spell_war_song() : SpellScriptLoader("spell_war_song") { }

		class spell_war_song_SpellScript : public SpellScript
		{
			PrepareSpellScript(spell_war_song_SpellScript);

			void HandleOnHit()
			{
				if (Unit* target = GetHitUnit())
				{
					uint32 health = target->GetMaxHealth();
					uint32 damage = health / 2; // 50% of the players health is dealt as damage
					SetHitDamage(damage);
				}
			}

			void Register()
			{
				OnHit += SpellHitFn(spell_war_song_SpellScript::HandleOnHit);
			}
		};

		SpellScript* GetSpellScript() const
		{
			return new spell_war_song_SpellScript();
		}
};

// Done
class spell_sundering_blow : public SpellScriptLoader
{
	public:
		spell_sundering_blow() : SpellScriptLoader("spell_sundering_blow") { }

		class spell_sundering_blow_SpellScript : public SpellScript
		{
			PrepareSpellScript(spell_sundering_blow_SpellScript);

			void HandleOnHit()
			{
				if (InstanceScript* m_Instance = GetCaster()->GetInstanceScript())
					if (Creature * genNazgrim = m_Instance->instance->GetCreature(m_Instance->GetData64(eData::DATA_GENERAL_NAZGRIM)))
					{
						AddRage(genNazgrim, 13, genNazgrim->GetGUID()); // On hit it gives 5 rage to nazgrim

						if (Unit* target = GetHitUnit())
							if (AuraPtr sunderingBlow = target->GetAura(SPELL_SUNDERING_BLOW))
							{
								uint32 stacks = sunderingBlow->GetStackAmount();

								AddRage(genNazgrim, 13*stacks, genNazgrim->GetGUID()); // And for every stack of the debuff 5 more rage
							}
					}
			}

			void Register()
			{
				OnHit += SpellHitFn(spell_sundering_blow_SpellScript::HandleOnHit);
			}
		};

		SpellScript* GetSpellScript() const
		{
			return new spell_sundering_blow_SpellScript();
		}
};

void AddSC_boss_general_nazgrim()
{
	new boss_general_nazgrim();

    new mob_orgrimmar_faithful();
    new mob_korkron_ironblade();
    new mob_korkron_arcweaver();
    new mob_korkron_assassin();
    new mob_korkron_warshaman();
	new mob_aftershock();
	new mob_korkron_banner();
	new mob_healing_tide_totem();
	new mob_ravager();

	new spell_war_song();
	new spell_sundering_blow();
};