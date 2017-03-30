#include "throne_of_thunder.h"

enum creatures
{
    NPC_MASSIVE_ANIMA_GOLEM = 69699,
    NPC_ANIMA_ORB = 69756,
    NPC_CRIMSON_WAKE = 69951
};

enum auras
{
    SPELL_ANIMA_ORB_VISUAL = 138322,
    SPELL_TRANSFUSION_MISSILE = 138908,
    SPELL_TRANSFUSION_TRANSFORM = 138399,
    SPELL_DISABLE_ENERGY_REGEN = 72242,

    SPELL_POWERED_DOWN = 138373,
    SPeLL_CRITICALLY_DAMAGED_ACTIVATED = 138409,

    SPELL_ANIMA_CAPACITY_4 = 138384,
    SPELL_ANIMA_CAPACITY_8 = 138385,
    SPELL_ANIMA_CAPACITY_36 = 138386,
    SPELL_ANIMA_CAPACITY_100 = 138387,

    SPELL_EVASIVE = 140759,
    SPELL_ACCELERATION_LINK_BUFF = 138453,
    SPELL_ACCELERATION_LINK = 138451,

    SPELL_CRIMSON_WAKE = 138480,

    SPELL_EXPLOSIVE_SLAM = 138569,
    SPELL_MATTER_SWAP = 138609,

    SPELL_SIPHON_ANIMA = 138644,
    SPELL_ANIMA_RING_SPAWN = 136954,
    SPELL_INTERRUPTING_JOLT = 138673
};

enum actions
{
    ACTION_ANIMA_ORB_ACTIVATE = 0,
    ACTION_RESET_GOLEM
};

Position spawnPosAnimusGolems[ANIMUS_GOLEMS_COUNT] =
{
    { 5765.64f, 4813.86f, 75.2741f, 3.12613f },
    { 5765.41f, 4799.18f, 75.2741f, 3.12613f },
    { 5765.52f, 4806.41f, 75.2741f, 3.12613f },
    { 5702.35f, 4765.82f, 77.5699f, 1.51941f },
    { 5709.2f, 4765.81f, 77.569f, 1.53929f },
    { 5716.79f, 4765.48f, 77.569f, 1.56131f },
    { 5702.47f, 4847.43f, 77.5702f, 4.70422f },
    { 5709.23f, 4847.22f, 77.5697f, 4.6534f },
    { 5715.75f, 4847.46f, 77.569f, 4.75044f },
    { 5778.58f, 4831.12f, 77.5509f, 3.13913f },
    { 5778.56f, 4824.25f, 77.5509f, 3.13913f },
    { 5777.63f, 4791.4f, 77.5547f, 3.12187f },
    { 5777.63f, 4784.22f, 77.5548f, 3.13131f },
    { 5762.58f, 4764.37f, 77.5699f, 1.55893f },
    { 5755.46f, 4764.46f, 77.5699f, 1.55893f },
    { 5748.52f, 4764.51f, 77.5694f, 1.5448f },
    { 5763.83f, 4848.07f, 77.57f, 4.71793f },
    { 5756.58f, 4848.03f, 77.57f, 4.71793f },
    { 5749.67f, 4847.99f, 77.57f, 4.71793f },
    { 5724.65f, 4836.8f, 75.2745f, 4.71793f },
    { 5739.72f, 4836.88f, 75.2745f, 4.71793f },
    { 5732.24f, 4836.84f, 75.2745f, 4.71793f },
    { 5739.55f, 4776.39f, 75.2756f, 1.56823f },
    { 5732.2f, 4776.41f, 75.2756f, 1.56823f },
    { 5724.85f, 4776.43f, 75.2756f, 1.56823f }
};

Position spawnPosLargeAnimaGolems[LARGE_ANIMUS_GOLEMS_COUNT] =
{
    { 5707.06f, 4752.47f, 77.5413f, 1.57309f },
    { 5715.58f, 4752.49f, 77.5413f, 1.57309f },
    { 5706.1f, 4859.57f, 77.5485f, 4.68368f },
    { 5713.8f, 4859.35f, 77.5485f, 4.68368f },
    { 5759.67f, 4859.51f, 77.5464f, 4.70208f },
    { 5751.27f, 4859.6f, 77.5463f, 4.70208f },
    { 5759.03f, 4752.57f, 77.542f, 1.55266f },
    { 5750.4f, 4752.72f, 77.5421f, 1.55266f },
};

Position spawnPosMassiveAnimaGolems[MASSIVE_ANIMUS_GOLEMS_COUNT] =
{
    { 5780.85f, 4842.08f, 77.5974f, 3.13913f },
    { 5782.3f, 4773.38f, 77.5881f, 3.12345f },
};

Position spawnPosDarkAnimus = { 5795.26f, 4806.41f, 77.5782f, 3.13875f };

class boss_dark_animus : public CreatureScript
{
public:
    boss_dark_animus() : CreatureScript("boss_dark_animus") { }

    class ai_impl : public BossAI
    {
    public:
        ai_impl(Creature* creature) : BossAI(creature, DATA_DARK_ANIMUS)
        {
            activated = false;
        }

        enum events
        {
            EVENT_UPDATE_WAKING = 1,
            EVENT_SIPHON_ANIMA = 2,
            EVENT_ANIMA_RING = 3,
            EVENT_INTERRUPTING_JOLT = 4
        };

        enum texts
        {
            SAY_DARK_ANIMUS_SIPHON = 0
        };

        void Reset()
        {
            if (activated)
                return;

            activated = true;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_AGGRESSIVE);

            me->RemoveAurasDueToSpell(SPELL_ANIMA_CAPACITY_100);
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 100);
            me->SetPower(POWER_ENERGY, 0);
            me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
            me->AddAura(SPELL_ANIMA_CAPACITY_100, me);
            me->AddAura(SPELL_POWERED_DOWN, me);

            eventsCosmetic.Reset();
            eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
            events.Reset();
            events.ScheduleEvent(EVENT_SIPHON_ANIMA, urand(20000, 40000));
            // events.ScheduleEvent(EVENT_ANIMA_RING, urand(10000, 12500)); we dont have moving ATs
            events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, urand(35000, 50000));
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_RESET_GOLEM:
            {
                activated = false;
                me->Respawn(true);
                me->AI()->EnterEvadeMode();
                break;
            }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            eventsCosmetic.Update(diff);
            switch (eventsCosmetic.ExecuteEvent())
            {
            case EVENT_UPDATE_WAKING:
            {
                if (me->GetPower(POWER_ENERGY) >= 4)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAurasDueToSpell(SPELL_POWERED_DOWN);
                }
                else
                    eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
                break;
            }
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_SIPHON_ANIMA:
            {
                Talk(SAY_DARK_ANIMUS_SIPHON);
                DoCast(SPELL_SIPHON_ANIMA);
                events.ScheduleEvent(EVENT_SIPHON_ANIMA, urand(20000, 25000));
                break;
            }
            case EVENT_ANIMA_RING:
            {
                events.ScheduleEvent(EVENT_ANIMA_RING, urand(10000, 12500));
                if (me->GetPower(POWER_ENERGY) < 25)
                    return;

                DoCast(SPELL_ANIMA_RING_SPAWN);
                break;
            }
            case EVENT_INTERRUPTING_JOLT:
            {
                events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, urand(35000, 50000));
                if (me->GetPower(POWER_ENERGY) < 75)
                    return;

                DoCast(SPELL_INTERRUPTING_JOLT);
                break;
            }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap eventsCosmetic;
        EventMap events;
        bool activated;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

// 69756
class npc_anima_orb : public CreatureScript
{
public:
    npc_anima_orb() : CreatureScript("npc_anima_orb") { }

    class ai_impl : public ScriptedAI
    {
    public:

        ai_impl(Creature* creature) : ScriptedAI(creature), summons(me)
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            me->SetReactState(REACT_PASSIVE);
            events.ScheduleEvent(EVENT_SUMMON_GOLEMS, 1000);
            popped = false;
        }

        enum texts
        {
            TEXT_SAY_DIED_EMOTE
        };

        enum events
        {
            EVENT_CHECK_WIPE_OR_RESET = 1,
            EVENT_SUMMON_GOLEMS
        };

        void ResetEncounter(Creature* me)
        {
            if (me->GetInstanceScript()->GetBossState(DATA_DARK_ANIMUS) != IN_PROGRESS)
                return;

            for (auto itr : summons)
                if (Creature* summon = me->GetMap()->GetCreature(itr))
                    if (summon->IsInCombat())
                        return;

            std::list<Player*> m_players;
            me->GetPlayerListInGrid(m_players, 150.0f);
            for (auto itr : m_players)
                if (itr->IsAlive())
                    return;

            summons.DespawnAll();
            events.ScheduleEvent(EVENT_SUMMON_GOLEMS, 25000);
            me->GetInstanceScript()->SetBossState(DATA_DARK_ANIMUS, FAIL);
        }

        void SpawnGolems()
        {
            for (uint8 x = 0; x < ANIMUS_GOLEMS_COUNT; x++)
                if (!me->SummonCreature(NPC_ANIMUS_GOLEM, spawnPosAnimusGolems[x + 1]))
                {
                    TC_LOG_ERROR("general", "Throne of Thunder (Dark Animus): golem spawn terminated, couldn't spawn animus golem %u", x + 1);
                    return;
                }

            for (uint8 x = 0; x < LARGE_ANIMUS_GOLEMS_COUNT; x++)
                if (!me->SummonCreature(NPC_LARGE_ANIMA_GOLEM, spawnPosLargeAnimaGolems[x + 1]))
                {
                    TC_LOG_ERROR("general", "Throne of Thunder (Dark Animus): golem spawn terminated, couldn't spawn large anima golem %u", x + 1);
                    return;
                }

            for (uint8 x = 0; x < MASSIVE_ANIMUS_GOLEMS_COUNT; x++)
                if (!me->SummonCreature(NPC_MASSIVE_ANIMA_GOLEM, spawnPosMassiveAnimaGolems[x + 1]))
                {
                    TC_LOG_ERROR("general", "Throne of Thunder (Dark Animus): golem spawn terminated, couldn't spawn massive anima golem %u", x + 1);
                    return;
                }

            if (!me->SummonCreature(BOSS_DARK_ANIMUS, spawnPosDarkAnimus))
                TC_LOG_ERROR("general", "Throne of Thunder (Dark Animus): couldn't spawn dark animus");
        }

        void Reset()
        {
            if (!popped)
            {
                me->AddAura(SPELL_ANIMA_ORB_VISUAL, me);
                me->SetMaxHealth(1);
                me->SetVisible(true);
                me->SetReactState(REACT_PASSIVE);
            }
        }

        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }

        void EnterCombat()
        {
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_ANIMA_ORB_ACTIVATE:
            {
                popped = false;
                Reset();
                break;
            }
            }
        }

        void DamageTaken(Unit* caster, uint32 &dmg)
        {
            if (int32(me->GetHealth() - dmg) < 1)
            {
                dmg = 0;
                me->RemoveAurasDueToSpell(SPELL_ANIMA_ORB_VISUAL);
                std::list<Creature*> golems;
                me->GetCreatureListWithEntryInGrid(golems, NPC_ANIMUS_GOLEM, 150.0f);
                for (auto itr : golems)
                    me->CastSpell(itr, SPELL_TRANSFUSION_MISSILE, true);

                events.ScheduleEvent(EVENT_CHECK_WIPE_OR_RESET, 5000);
                Talk(TEXT_SAY_DIED_EMOTE);
                me->SetVisible(false);
                popped = true;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_CHECK_WIPE_OR_RESET:
            {
                ResetEncounter(me);
                events.ScheduleEvent(EVENT_CHECK_WIPE_OR_RESET, 5000);
                break;
            }
            case EVENT_SUMMON_GOLEMS:
            {
                DoAction(ACTION_ANIMA_ORB_ACTIVATE);
                SpawnGolems();
                break;
            }
            }
        }

    private:
        EventMap events;
        SummonList summons;
        bool popped;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

//69784
class npc_animus_golem : public CreatureScript
{
public:
    npc_animus_golem() : CreatureScript("npc_animus_golem") { }

    class ai_impl : public ScriptedAI
    {
    public:
        ai_impl(Creature* creature) : ScriptedAI(creature)
        {
            activated = false;
            critical = false;
        }

        enum events
        {
            EVENT_UPDATE_WAKING = 1
        };

        void Reset()
        {
            if (activated)
            {
                me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
                me->AddAura(SPELL_ANIMA_CAPACITY_4, me);
                if (critical)
                {
                    me->AddAura(SPELL_CRITICALLY_DAMAGED, me);
                    me->AddAura(SPeLL_CRITICALLY_DAMAGED_ACTIVATED, me);
                }

                me->AddAura(SPELL_ACCELERATION_LINK, me);
                me->AddAura(SPELL_EVASIVE, me);
                return;
            }

            ScriptedAI::Reset();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_DEFENSIVE);

            me->RemoveAurasDueToSpell(SPELL_ANIMA_CAPACITY_4);
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 100);
            me->SetPower(POWER_ENERGY, 0);
            me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
            me->AddAura(SPELL_ANIMA_CAPACITY_4, me);
            me->AddAura(SPELL_POWERED_DOWN, me);
            me->AddAura(SPELL_ACCELERATION_LINK, me);

            events.Reset();
            events.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_RESET_GOLEM:
            {
                activated = false;
                me->Respawn(true);
                me->AI()->EnterEvadeMode();
                break;
            }
            }
        }

        void EnterCombat(Unit* /*target*/)
        {
            //if (me->GetInstanceScript()->GetBossState(DATA_DARK_ANIMUS) == NOT_STARTED || me->GetInstanceScript()->GetBossState(DATA_DARK_ANIMUS) == FAIL)
            me->GetInstanceScript()->SetBossState(DATA_DARK_ANIMUS, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Creature*> golems;
            me->GetCreatureListWithEntryInGrid(golems, NPC_LARGE_ANIMA_GOLEM, 20.0f);
            me->GetCreatureListWithEntryInGrid(golems, NPC_MASSIVE_ANIMA_GOLEM, 20.0f);
            me->GetCreatureListWithEntryInGrid(golems, BOSS_DARK_ANIMUS, 20.0f);

            for (auto itr : golems)
                if (itr->GetEntry() != NPC_LARGE_ANIMA_GOLEM && itr->GetEntry() != NPC_MASSIVE_ANIMA_GOLEM && itr->GetEntry() != BOSS_DARK_ANIMUS)
                    golems.remove(itr);

            if (golems.empty())
                return;

            golems.sort(Trinity::DistanceCompareOrderPred(me, true));

            if (Creature* target = golems.front())
                me->CastSpell(target, SPELL_TRANSFUSION_MISSILE, true);
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_UPDATE_WAKING:
            {
                if (me->GetPower(POWER_ENERGY) >= 4)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAurasDueToSpell(SPELL_POWERED_DOWN);

                    if (me->HasAura(SPELL_CRITICALLY_DAMAGED))
                    {
                        critical = true;
                        me->AddAura(SPeLL_CRITICALLY_DAMAGED_ACTIVATED, me);
                    }

                    activated = true;
                }
                else
                {
                    if (activated)
                        Reset();
                    else
                        events.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
                }
                break;
            }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

    private:
        EventMap events;
        bool activated;
        bool critical;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

// 69700
class npc_large_animus_golem : public CreatureScript
{
public:
    npc_large_animus_golem() : CreatureScript("npc_large_animus_golem") { }

    class ai_impl : public ScriptedAI
    {
    public:
        ai_impl(Creature* creature) : ScriptedAI(creature)
        {
            activated = false;
            critical = false;
        }

        enum events
        {
            EVENT_UPDATE_WAKING = 1,
            EVENT_CRIMSON_WAKE
        };

        void Reset()
        {
            if (activated)
            {
                me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
                me->AddAura(SPELL_ANIMA_CAPACITY_4, me);
                if (critical)
                {
                    me->AddAura(SPELL_CRITICALLY_DAMAGED, me);
                    me->AddAura(SPeLL_CRITICALLY_DAMAGED_ACTIVATED, me);
                }

                me->AddAura(SPELL_ACCELERATION_LINK, me);
                return;
            }

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_DEFENSIVE);

            me->RemoveAurasDueToSpell(SPELL_ANIMA_CAPACITY_8);
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 100);
            me->SetPower(POWER_ENERGY, 0);
            me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
            me->AddAura(SPELL_ANIMA_CAPACITY_8, me);
            me->AddAura(SPELL_POWERED_DOWN, me);

            events.Reset();
            eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
            events.ScheduleEvent(EVENT_CRIMSON_WAKE, urand(12000, 14000));
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_RESET_GOLEM:
            {
                activated = false;
                me->Respawn(true);
                me->AI()->EnterEvadeMode();
                break;
            }
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Creature*> golems;
            me->GetCreatureListWithEntryInGrid(golems, NPC_MASSIVE_ANIMA_GOLEM, 20.0f);
            me->GetCreatureListWithEntryInGrid(golems, BOSS_DARK_ANIMUS, 20.0f);

            for (auto itr : golems)
                if (itr->GetEntry() != NPC_LARGE_ANIMA_GOLEM && itr->GetEntry() != NPC_MASSIVE_ANIMA_GOLEM && itr->GetEntry() != BOSS_DARK_ANIMUS)
                    golems.remove(itr);

            if (golems.empty())
                return;

            golems.sort(Trinity::DistanceCompareOrderPred(me, true));

            if (Creature* target = golems.front())
                me->CastSpell(target, SPELL_TRANSFUSION_MISSILE, true);
        }

        void UpdateAI(uint32 const diff)
        {
            eventsCosmetic.Update(diff);
            switch (eventsCosmetic.ExecuteEvent())
            {
            case EVENT_UPDATE_WAKING:
            {
                if (me->GetPower(POWER_ENERGY) >= 4)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAurasDueToSpell(SPELL_POWERED_DOWN);

                    if (me->HasAura(SPELL_CRITICALLY_DAMAGED))
                    {
                        critical = true;
                        me->AddAura(SPeLL_CRITICALLY_DAMAGED_ACTIVATED, me);
                    }

                    activated = true;
                }
                else
                {
                    if (activated)
                        Reset();
                    else
                        eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
                }
                break;
            }
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_CRIMSON_WAKE:
            {
                DoCast(SELECT_TARGET_RANDOM, SPELL_CRIMSON_WAKE);
                events.ScheduleEvent(EVENT_CRIMSON_WAKE, urand(10000, 12000));
                break;
            }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap eventsCosmetic;
        EventMap events;
        bool activated;
        bool critical;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

// 69951
class npc_crimson_wake : public CreatureScript
{
public:
    npc_crimson_wake() : CreatureScript("npc_crimson_wake") { }

    class ai_impl : public ScriptedAI
    {
    public:
        ai_impl(Creature* creature) : ScriptedAI(creature) { }

        enum auras
        {
            CRIMSON_WAKE_PERIODIC_SLOW = 138482,
            CRIMSON_WAKE_PERIODIC_MEDIUM = 138484,
            CRIMSON_WAKE_PERIODIC_FAST = 138544
        };

        enum
        {
            EVENT_FOLLOW = 1
        };

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->AddAura(CRIMSON_WAKE_PERIODIC_SLOW, me);
            me->SetSpeed(MOVE_RUN, 0.5f, true);
            me->SetSpeed(MOVE_WALK, 0.5f, true);
        }

        void AfterSummon(Unit* /*summoner*/, Unit* target, uint32 spell)
        {
            std::list<Player*> players;
            me->GetPlayerListInGrid(players, 100.0f);
            if (!players.empty())
            {
                followTarget = Trinity::Containers::SelectRandomContainerElement(players);
                events.ScheduleEvent(EVENT_FOLLOW, 250);
            }
        }

        void UpdateAI(uint32 const diff)
        {
            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_FOLLOW:
            {
                if (!followTarget)
                    return;

                me->GetMotionMaster()->MovePoint(1, followTarget->GetPosition());
                events.ScheduleEvent(EVENT_FOLLOW, 250);
                break;
            }
            }
        }

    private:
        EventMap events;
        Player* followTarget;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

// 69699
class npc_massive_animus_golem : public CreatureScript
{
public:
    npc_massive_animus_golem() : CreatureScript("npc_massive_animus_golem") { }

    class ai_impl : public ScriptedAI
    {
    public:
        ai_impl(Creature* creature) : ScriptedAI(creature)
        {
            activated = false;
        }

        enum events
        {
            EVENT_UPDATE_WAKING = 1,
            EVENT_EXPLOSIVE_SLAM,
            EVENT_MATTER_SWAP
        };

        void Reset()
        {
            if (activated)
                return;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_DEFENSIVE);

            me->RemoveAurasDueToSpell(SPELL_ANIMA_CAPACITY_36);
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 100);
            me->SetPower(POWER_ENERGY, 0);
            me->AddAura(SPELL_DISABLE_ENERGY_REGEN, me);
            me->AddAura(SPELL_ANIMA_CAPACITY_36, me);
            me->AddAura(SPELL_POWERED_DOWN, me);

            events.Reset();
            eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
            events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, urand(10000, 12500));
        }

        void JustDied(Unit* /*killer*/)
        {
            std::list<Creature*> golems;
            me->GetCreatureListWithEntryInGrid(golems, BOSS_DARK_ANIMUS, 20.0f);

            for (auto itr : golems)
                if (itr->GetEntry() != NPC_LARGE_ANIMA_GOLEM && itr->GetEntry() != NPC_MASSIVE_ANIMA_GOLEM && itr->GetEntry() != BOSS_DARK_ANIMUS)
                    golems.remove(itr);

            if (golems.empty())
                return;

            golems.sort(Trinity::DistanceCompareOrderPred(me, true));

            if (Creature* target = golems.front())
                me->CastSpell(target, SPELL_TRANSFUSION_MISSILE, true);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case ACTION_RESET_GOLEM:
            {
                activated = false;
                me->Respawn(true);
                me->AI()->EnterEvadeMode();
                break;
            }
            }
        }

        void UpdateAI(uint32 const diff)
        {
            eventsCosmetic.Update(diff);
            switch (eventsCosmetic.ExecuteEvent())
            {
            case EVENT_UPDATE_WAKING:
            {
                if (me->GetPower(POWER_ENERGY) >= 4)
                {
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAurasDueToSpell(SPELL_POWERED_DOWN);
                    activated = true;
                }
                else
                {
                    if (activated)
                        Reset();
                    else
                        eventsCosmetic.ScheduleEvent(EVENT_UPDATE_WAKING, 1000);
                }
                break;
            }
            }

            if (!UpdateVictim())
                return;

            events.Update(diff);
            switch (events.ExecuteEvent())
            {
            case EVENT_EXPLOSIVE_SLAM:
            {
                DoCast(SPELL_EXPLOSIVE_SLAM);
                events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, urand(10000, 12500));
                break;
            }
            case EVENT_MATTER_SWAP:
            {
                DoCast(SPELL_MATTER_SWAP);
                events.ScheduleEvent(EVENT_MATTER_SWAP, urand(15000, 25000));
                break;
            }
            }

            DoMeleeAttackIfReady();
        }

    private:
        EventMap eventsCosmetic;
        EventMap events;
        bool activated;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new ai_impl(creature);
    }
};

// 138908
class spell_transfusion_missile : public SpellScriptLoader
{
public:
    spell_transfusion_missile() : SpellScriptLoader("spell_transfusion_missile") { }

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void HandleMissile(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
                target->CastSpell(target, SPELL_TRANSFUSION_TRANSFORM, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleMissile, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

//138452
class spell_acceleration_link : public SpellScriptLoader
{
public:
    spell_acceleration_link() : SpellScriptLoader("spell_acceleration_link") { }

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void HandleLink(SpellEffIndex /*effIndex*/)
        {
            std::list<Creature*> _golems;
            GetCaster()->GetCreatureListWithEntryInGrid(_golems, NPC_ANIMUS_GOLEM, 6.5f);
            if (!GetCaster()->HasAura(SPELL_ACCELERATION_LINK_BUFF))
            {
                GetCaster()->CastSpell(GetCaster(), SPELL_ACCELERATION_LINK_BUFF, true);
                return;
            }

            uint8 _count = 0;
            for (auto itr : _golems)
            {
                if (itr->isDead())
                    continue;

                _count++;
            }

            if (_count > 1)
                _count--;

            if (Aura* aura = GetCaster()->GetAura(SPELL_ACCELERATION_LINK_BUFF))
            {
                if (!_count)
                    aura->Remove();
                else
                    aura->SetStackAmount(_count);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleLink, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

//138451
class spell_acceleration_link_select : public SpellScriptLoader
{
public:
    spell_acceleration_link_select() : SpellScriptLoader("spell_acceleration_link_select") { }

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void Filter(std::list<WorldObject*>& targets)
        {
            std::list<WorldObject*> _new;
            for (auto itr : targets)
                if (itr->GetTypeId() == TYPEID_UNIT && !itr->ToUnit()->isDead() && itr->GetEntry() == NPC_ANIMUS_GOLEM)
                    _new.push_back(itr);

            targets.swap(_new);
            _new.clear();

            if (targets.empty())
                GetCaster()->RemoveAurasDueToSpell(SPELL_ACCELERATION_LINK_BUFF);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_impl::Filter, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_matter_swap : public SpellScriptLoader
{
public:
    spell_matter_swap() : SpellScriptLoader("spell_matter_swap") { }

    //class spell_impl : public SpellScript
    //{
    //    PrepareAuraScript(spell_impl);
    //
    //    void FilterTargets(std::list<WorldObject*>& list)
    //    {
    //        list.clear();
    //    }
    //
    //    void Register()
    //    {
    //        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_impl::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
    //    }
    //};
    //
    //SpellScript* GetSpellScript() const
    //{
    //    return new spell_impl();
    //}

    class aura_impl : public AuraScript
    {
        PrepareAuraScript(aura_impl);

        enum
        {
            SPELL_MATTER_SWAP_DMG = 138618
        };

        Player* returnMostDistantPlayer(Player* player)
        {
            Player* target = NULL;
            std::list<Player*> players;
            player->GetPlayerListInGrid(players, 100.0f);
            players.sort(Trinity::DistanceCompareOrderPred(GetOwner()));
            if (players.back())
                target = players.back();

            return players.back();
        }

        void HandlePercent(AuraEffect const* eff)
        {
            if (AuraEffect* null = eff->GetBase()->GetEffect(EFFECT_0))
                null->SetAmount(eff->GetAmount() - 10);

            if (AuraEffect* first = eff->GetBase()->GetEffect(EFFECT_1))
                first->SetAmount(100 - eff->GetAmount());

            eff->GetBase()->SetNeedClientUpdateForTargets();

            if (Player* player = GetOwner()->ToPlayer())
                if (Player* target = returnMostDistantPlayer(player))
                    player->AddAura(eff->GetSpellEffectInfo().TriggerSpell, target);

        }

        void HandleSwap(AuraEffect const* eff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* player = GetOwner()->ToPlayer())
            {
                if (Player* target = returnMostDistantPlayer(player))
                {
                    int32 dmg = CalculatePct(player->GetMaxHealth(), eff->GetAmount());
                    player->CastCustomSpell(player, SPELL_MATTER_SWAP_DMG, &dmg, NULL, NULL, true);
                    int32 dmg2 = CalculatePct(player->GetMaxHealth(), 100 - eff->GetAmount());
                    player->CastCustomSpell(target, SPELL_MATTER_SWAP_DMG, &dmg2, NULL, NULL, true);
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(aura_impl::HandlePercent, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            OnEffectRemove += AuraEffectRemoveFn(aura_impl::HandleSwap, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new aura_impl();
    }
};

class spell_siphon_anima : public SpellScriptLoader
{
public:
    spell_siphon_anima() : SpellScriptLoader("spell_siphon_anima") { }

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void HandleLink(SpellEffIndex /*effIndex*/)
        {
            GetCaster()->SetPower(POWER_ENERGY, GetCaster()->GetPower(POWER_ENERGY) + 1);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleLink, EFFECT_0, SPELL_EFFECT_POWER_DRAIN);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

void AddSC_boss_dark_animus()
{
    new boss_dark_animus();
    new npc_anima_orb();
    new npc_animus_golem();
    new npc_large_animus_golem();
    new npc_crimson_wake();
    new npc_massive_animus_golem();
    new spell_transfusion_missile();
    new spell_acceleration_link();
    new spell_acceleration_link_select();
    new spell_matter_swap();
    new spell_siphon_anima();
}