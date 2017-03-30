#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    SPELL_ELECTROSTATIC_CHARGE        = 143385,
    SPELL_OVERCHARGE                  = 145774,
    SPELL_LAUNCH_SAWBLADE             = 143265,
    SPELL_PROTECTIVE_FRENZY           = 145365
    //Automated Shredder
    SPELL_OVERLOAD                    = 145444,
    SPELL_DEATH_FROM_ABOVE            = 144208
};

enum eEvents
{
    EVENT_ELECTROSTATIC_CHARGE = 1,
    EVENT_OVERCHARGE           = 2,
    EVENT_LAUNCH_SAWBLADE      = 3,
    EVENT_PROTECTIVE_FRENZY    = 4,
    EVENT_AUTOMATED_SHREDDER   = 5,
    //Automated Shredder
    EVENT_OVERLOAD             = 6,
    EVENT_DEATH_FROM_ABOVE     = 7
};

enum eCreature
{
    CREATURE_AUTOMATED_SHREDDER = 71591
};

enum eSays
{
    /*
    Siegecrafter Blackfuse yells: Alright, Shanna. You're up.
    Siegecrafter Blackfuse yells: Another satisfied customer.
    Siegecrafter Blackfuse yells: Goro'dan! You're on deck!
    Siegecrafter Blackfuse yells: Hey! You're gonna pay for that!
    Siegecrafter Blackfuse yells: Hey, I heard you like mines, so... enjoy!
    Siegecrafter Blackfuse yells: Let's settle this.
    Siegecrafter Blackfuse yells: Looks like I'm in the market for a new assistant.
    Siegecrafter Blackfuse yells: Meet the LT-403 Evaporation laser. A personal point of pride.
    Siegecrafter Blackfuse yells: Presenting... the beautiful new ST-03 Shockwave missile turret!
    Siegecrafter Blackfuse yells: That's for the Juggernaut.
    Siegecrafter Blackfuse yells: The SD-5 Automated Shredder defense unit. A labor of love.
    Siegecrafter Blackfuse yells: Time to mix business with pleasure.
    Siegecrafter Blackfuse yells: What's better than a bomb, huh?! I'll tell ya - a bomb with two bombs inside!
    Siegecrafter Blackfuse yells: Ya' ever wonder how magnets work?
    Siegecrafter Blackfuse yells: You son of a...
    Siegecrafter Blackfuse says: Don't... think you've won...
    Siegecrafter Blackfuse says: My legacy... will live on...
    */
};

class boss_siegecrafter_blackfuse : public CreatureScript
{
	public:
		boss_siegecrafter_blackfuse() : CreatureScript("boss_siegecrafter_blackfuse") { }

		struct boss_siegecrafter_blackfuseAI : public BossAI
		{
			boss_siegecrafter_blackfuseAI(Creature* creature) : BossAI(creature, DATA_SIEGECRAFTER_BLACKFUSE)
			{
				pInstance = creature->GetInstanceScript();
			}
			
			EventMap events;
			InstanceScript* pInstance;
			
			void Reset()
			{
				Reset();
				
				events.Reset();
				
				summons.DespawnAll();
				
				if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
			}
			
			void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, FAIL);
            }
			
			void EnterCombat(Unit* attacker)
            {
				// @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, IN_PROGRESS);
                }
                DoCastVictim(SPELL_ELECTROSTATIC_CHARGE);
                events.ScheduleEvent(EVENT_ELECTROSTATIC_CHARGE, urand(17000, 22000));
                events.ScheduleEvent(EVENT_LAUNCH_SAWBLADE, 7000);
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
                    pInstance->SetBossState(DATA_SIEGECRAFTER_BLACKFUSE, DONE);
                }
            }
			
			void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                        case EVENT_ELECTROSTATIC_CHARGE:
                        {
                            DoCastVictim(SPELL_ELECTROSTATIC_CHARGE);
                            events.ScheduleEvent(EVENT_ELECTROSTATIC_CHARGE, urand(17000, 22000));
                        }
                        case EVENT_LAUNCH_SAWBLADE:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_LAUNCH_SAWBLADE);
                            events.ScheduleEvent(EVENT_LAUNCH_SAWBLADE, urand(10000, 15000));
                        }
                        case EVENT_AUTOMATED_SHREDDER:
                        {
                            me->SummonCreature(CREATURE_AUTOMATED_SHREDDER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                        }
                        default:
                            break;
                    }
            }
        DoMeleeAttackIfReady();
        }
};

        CreatureAI* GetAI(Creature* pCreature) const
		{
			return new boss_siegecrafter_blackfuseAI(pCreature);
		}
};

class mob_automated_shredder : public CreatureScript
{
	public:
		mob_automated_shredder() : CreatureScript("mob_automated_shredder") { }

		struct mob_automated_shredderAI : public ScriptedAI
		{
			mob_automated_shredderAI(Creature* creature) : ScriptedAI(creature)
			{
				pInstance = creature->GetInstanceScript();
			}

			InstanceScript* pInstance;
			EventMap events;

			void Reset()
			{
				events.Reset();
			}

			void EnterCombat(Unit* attacker)
			{
				events.ScheduleEvent(EVENT_OVERLOAD, urand(2000, 5000));
				events.ScheduleEvent(EVENT_DEATH_FROM_ABOVE, 12000);
			}

			void UpdateAI(const uint32 diff)
			{
				if (!UpdateVictim())
					return;

				events.Update(diff);
				while (uint32 eventId = events.ExecuteEvent())
				{
					switch (eventId)
					{
					case EVENT_OVERLOAD:
					{
						DoCastVictim(SPELL_OVERLOAD);
						events.ScheduleEvent(EVENT_ELECTROSTATIC_CHARGE, urand(4000, 6000));
					}
					case EVENT_DEATH_FROM_ABOVE:
					{
						DoCastVictim(SPELL_DEATH_FROM_ABOVE);
					}
					default:
						break;
					}
				}
				DoMeleeAttackIfReady();
			}
		};
		
		CreatureAI* GetAI(Creature* creature) const
		{
			return new mob_automated_shredderAI(creature);
		}
};

void AddSC_siegecrafter_blackfuse()
{
	new boss_siegecrafter_blackfuse();
    new mob_automated_shredder();
};
