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
	// Galakras
	SPELL_FLAMES_OF_GALAKROND	= 147029,
    SPELL_PULSING_FLAMES		= 147042,

	// Korgra the Snake

	// Lieutenant Krugruk

	// High Enforcer Thranok

	// Master Cannoneer Dagryn

	// Dragonmaw Bonecrushers

	// Dragonmaw Tidal Shamans

	// Dragonmaw Flameslingers

	// Dragonmaw Grunts

	// Dragonmaw Wind Reavers

	// Dragonmaw Proto-drakes

	// Dragonmaw Ebon Stalkers

	// Dragonmaw Flag Bearers

	// Kor'kron Demolishers
};

enum eEvents
{
	EVENT_FIRST_WAVE			= 1,
	EVENT_SECOND_WAVE			= 2,
	EVENT_THIRD_WAVE			= 3,
	EVENT_FOURTH_WAVE			= 4,
	EVENT_FIFTH_WAVE			= 5,
	EVENT_SIXTH_WAVE			= 6,
	EVENT_SEVENTH_WAVE			= 7,
	EVENT_EIGHTH_WAVE			= 8,
	EVENT_FLAMES_OF_GALAKROND	= 9,
	EVENT_PULSING_FLAMES		= 10,
};

enum eActions
{
};

enum ePhases
{
	PHASE_ONE	= 1,
	PHASE_TWO	= 2,
};

// TODO - need to get coords for adds spawning
Position waveAdds[5] = 
{
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // Left side 2nd position
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // Left side 1st position
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // Central position
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // Right side 1st position
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // Right side 2nd position
};

// TODO - need to get z coord for the ground
#define FLOOR_Z 20.0f

class boss_galakras : public CreatureScript
{
    public:
        boss_galakras() : CreatureScript("boss_galakras") { }

        struct boss_galakrasAI : public BossAI
        {
            boss_galakrasAI(Creature* creature) : BossAI(creature, DATA_GALAKRAS) 
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;

			void Reset() override
			{
				_Reset();
				me->SetDisableGravity(true);
				me->SetCanFly(true);
				me->SetReactState(ReactStates::REACT_PASSIVE);

				switch (me->GetMap()->GetDifficulty())
				{
					case MAN10_DIFFICULTY:
						me->SetMaxHealth(138691568);
						break;
					case MAN10_HEROIC_DIFFICULTY:
						me->SetMaxHealth(218068496);
						break;
					case MAN25_DIFFICULTY:
						me->SetMaxHealth(416946976);
						break;
					case MAN25_HEROIC_DIFFICULTY:
						me->SetMaxHealth(654205504);
						break;
				}
			}

			void EnterCombat(Unit* who)
			{
				_EnterCombat();

				events.SetPhase(PHASE_ONE);
				events.ScheduleEvent(EVENT_FIRST_WAVE, 10000, 0, PHASE_ONE);
			}

			void JustDied(Unit* killer)
			{
				_JustDied();
			}

			void JustReachedHome()
			{
				_JustReachedHome();
			}

			void KilledUnit(Unit* who)
			{

			}

			void UpdateAI(const uint32 diff)
			{
				events.Update(diff);

				if (!UpdateVictim())
					return;

				if (me->HasUnitState(UNIT_STATE_CASTING))
					return;

				if (me->GetPositionZ() <= FLOOR_Z && events.IsInPhase(PHASE_ONE))
				{
					me->SetReactState(ReactStates::REACT_AGGRESSIVE);
					events.SetPhase(PHASE_TWO);
					events.ScheduleEvent(EVENT_FLAMES_OF_GALAKROND, 20000, 0, PHASE_TWO);
					events.ScheduleEvent(EVENT_PULSING_FLAMES, 0, 0, PHASE_TWO);
				}

				switch (events.ExecuteEvent())
				{
					case EVENT_FIRST_WAVE: // Random adds
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_SECOND_WAVE, 60000, 0, PHASE_ONE);
						break;
					}

					case EVENT_SECOND_WAVE: // Random adds
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_THIRD_WAVE, 60000, 0, PHASE_ONE);
						break;
					}

					case EVENT_THIRD_WAVE: // Random adds and mini boss 2 minutes after pull
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_FOURTH_WAVE, 30000, 0, PHASE_ONE);
						break;
					}

					case EVENT_FOURTH_WAVE: // Random adds and mini boss 2 and a half minutes after pull
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_FIFTH_WAVE, 30000, 0, PHASE_ONE);
						break;
					}

					case EVENT_FIFTH_WAVE: // Random adds
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_SIXTH_WAVE, 30000, 0, PHASE_ONE);
						break;
					}

					case EVENT_SIXTH_WAVE: // Random adds
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_SEVENTH_WAVE, 30000, 0, PHASE_ONE);
						break;
					}

					case EVENT_SEVENTH_WAVE:
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						events.ScheduleEvent(EVENT_EIGHTH_WAVE, 60000, 0, PHASE_ONE);
						break;
					}

					case EVENT_EIGHTH_WAVE:
					{
						me->SummonCreature(/*need to find creature ids*/1, waveAdds[0], TEMPSUMMON_MANUAL_DESPAWN); // Left side 2nd position
						me->SummonCreature(/*need to find creature ids*/2, waveAdds[1], TEMPSUMMON_MANUAL_DESPAWN); // Left side 1st position
						me->SummonCreature(/*need to find creature ids*/3, waveAdds[2], TEMPSUMMON_MANUAL_DESPAWN); // Central position
						me->SummonCreature(/*need to find creature ids*/4, waveAdds[3], TEMPSUMMON_MANUAL_DESPAWN); // Right side 1st position
						me->SummonCreature(/*need to find creature ids*/5, waveAdds[4], TEMPSUMMON_MANUAL_DESPAWN); // Right side 2nd position
						break;
					}

					case EVENT_FLAMES_OF_GALAKROND:
					{
						if (Unit* target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 40.0f, true))
						{
							DoCast(target, SPELL_FLAMES_OF_GALAKROND);
						}

						events.ScheduleEvent(EVENT_FLAMES_OF_GALAKROND, 20000, 0, PHASE_TWO);
						break;
					}

					case EVENT_PULSING_FLAMES:
					{
						DoCast(SPELL_PULSING_FLAMES);
						break;
					}
				}
			}
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_galakrasAI(creature);
        }
};

class mob_master_cannoneer_dagryn : public CreatureScript
{
    public:
        mob_master_cannoneer_dagryn() : CreatureScript("mob_master_cannoneer_dagryn") { }

        struct mob_master_cannoneer_dagrynAI : public ScriptedAI
        {
            mob_master_cannoneer_dagrynAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_master_cannoneer_dagrynAI(creature);
        }
};

class mob_high_enforcer_thranok : public CreatureScript
{
    public:
        mob_high_enforcer_thranok() : CreatureScript("mob_high_enforcer_thranok") { }

        struct mob_high_enforcer_thranokAI : public ScriptedAI
        {
            mob_high_enforcer_thranokAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_high_enforcer_thranokAI(creature);
        }
};

class mob_lieutenant_krugruk : public CreatureScript
{
    public:
        mob_lieutenant_krugruk() : CreatureScript("mob_lieutenant_krugruk") { }

        struct mob_lieutenant_krugrukAI : public ScriptedAI
        {
            mob_lieutenant_krugrukAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lieutenant_krugrukAI(creature);
        }
};

class mob_korgra_the_snake: public CreatureScript
{
    public:
        mob_korgra_the_snake() : CreatureScript("mob_korgra_the_snake") { }

        struct mob_korgra_the_snakeAI : public ScriptedAI
        {
            mob_korgra_the_snakeAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korgra_the_snakeAI(creature);
        }
};

class mob_dragonmaw_bonecrusher : public CreatureScript
{
    public:
        mob_dragonmaw_bonecrusher() : CreatureScript("mob_dragonmaw_bonecrusher") { }

        struct mob_dragonmaw_bonecrusherAI : public ScriptedAI
        {
            mob_dragonmaw_bonecrusherAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dragonmaw_bonecrusherAI(creature);
        }
};

void AddSC_galakras()
{
    new boss_galakras();
    new mob_master_cannoneer_dagryn();
    new mob_high_enforcer_thranok();
    new mob_lieutenant_krugruk();
    new mob_korgra_the_snake();
    new mob_dragonmaw_bonecrusher();
}
