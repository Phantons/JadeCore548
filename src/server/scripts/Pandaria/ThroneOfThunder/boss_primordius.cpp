#include "throne_of_thunder.h"
#include "CreatureTextMgr.h"
#include "ScriptPCH.h"

enum eSpells : uint32
{
    SPELL_PRIMORDIUS_INTRO_COSMETIC         = 137064,
    SPELL_SUBMERGE_COSMETIC                 = 140483,
    SPELL_EMERGE                            = 139832,

    SPELL_PRIMORDIAL_STRIKE                 = 136037,   // Cast on main target next melee
    SPELL_MALFORMED_BLOOD                   = 136050,   // Stacking dot, applied to tank
    SPELL_CONGEAL_BLOOD                     = 136051,
    SPELL_EVOLUTION                         = 139144,   // Boss gains damage each time he evolves
    SPELL_MUTATED_ABOMINATION               = 140544,   // Boss aura

    SPELL_BERSERK_PRIMORDIUS                = 47008,
    SPELL_ZERO_POWER                        = 72242,

    SPELL_VENTRAL_SACS                      = 136210,   // Applies ventral sacs aura
    SPELL_VENTRAL_SACS_DAMAGE               = 136211,

    SPELL_GAS_BLADDER                       = 136215,   // Enables caustic gas
    SPELL_CAUSTIC_GAS                       = 136216,   // Split between all targets hit, 800k damage

    SPELL_ACIDIC_SPINES                     = 136218,   // Triggers missile aoe on random players
    SPELL_ACIDIC_EXPLOSION                  = 136856,   // Target selection spell, basepoints is trigger missile
    SPELL_ACIDIC_SPINES_MISSILE             = 136219,
    SPELL_ACIDIC_SPINES_DAMAGE              = 136220,

    SPELL_PATHOGEN_GLANDS                   = 136225,   // Enables volatile pathogen
    SPELL_VOLATILE_PATHOGEN                 = 136228,   // DoT spell, needs explicit target
    SPELL_VOLATILE_PATHOGEN_DAMAGE          = 136231,

    SPELL_METABOLIC_BOOST                   = 136245,   // Mods spell cooldown, timers need to be explicitly checked here (possibly through event function)

    SPELL_ERUPTING_PUSTULES                 = 136246,
    SPELL_PUSTULE_ERUPTION                  = 140499,   // Script effect for targeted pustule eruption
    SPELL_PUSTULE_ERUPTION_RAND             = 136249,
    SPELL_PUSTULE_ERUPTION_PLAYER           = 136248,
    SPELL_PUSTULE_ERUPTION_DAMAGE           = 136247,


    // Add / player spells
    SPELL_VOLATILE_POOL                     = 140506,   // Area trigger aura on pools
    SPELL_VOLATILE_MUTATION                 = 140508,   // Volatile mutation spell, probably from pools
    SPELL_VOLATILE_MUTATION_PRIMORDIUS      = 140509,   // Volatile mutation  on primordius
    SPELL_VOLATILE_MUTATION_AURA            = 141094,   // Aura added on players from area trigger
    
    SPELL_MUTAGENIC_POOL                    = 136049,   // Create areatrigger
    SPELL_MUTATION                          = 136178,   // Trigger

    SPELL_MUTATE_PRIMORDIUS                 = 136203,   // Living fluid

    SPELL_KEEN_EYESIGHT                     = 136180,   // Crit increase
    SPELL_IMPAIRED_EYESIGHT                 = 136181,   // Crit decrease
    SPELL_IMPROVED_SYNAPSES                 = 136182,   // Haste increase
    SPELL_DULLED_SYNAPSES                   = 136183,   // Haste decrease
    SPELL_THICK_BONES                       = 136184,   // Stats increase
    SPELL_FRAGILE_BONES                     = 136185,   // Stats reduce
    SPELL_CLEAR_MIND                        = 136186,   // Mastery increase
    SPELL_CLOUDED_MIND                      = 136187,   // Mastery decrease

    SPELL_FULLY_MUTATED                     = 140546,   // Stacks of any combination
    SPELL_FULLY_MUTATED_PET                 = 140706
};

enum eEvents : uint32
{
    EVENT_EMPTY,
    EVENT_RELOCATE,
    EVENT_SUBMERGE,

    EVENT_INTRO_1,
    EVENT_INTRO_2,
    EVENT_INTRO_3,
    EVENT_INTRO_4,

    EVENT_LIVING_FLUID,
    EVENT_PRIMORDIAL_STRIKE,
    EVENT_GAIN_ENERGY,
    EVENT_CAUSTIC_GAS,
    EVENT_VOLATILE_PATHOGEN,
    EVENT_CHECK_ENERGY,
    EVENT_BERSERK,

    EVENT_MOVE,
    EVENT_CHECK_PROXIMITY
};

enum eTalks : uint32
{
    TALK_INTRO_1,
    TALK_INTRO_2,
    TALK_INTRO_3,
    TALK_INTRO_4,

    TALK_AGGRO,
    TALK_EVOLVE,
    TALK_SLAY,
    TALK_DEATH,

    EMOTE_VENTRAL_SACS,
    EMOTE_GAS_BLADDER,
    EMOTE_ACIDIC_SPINES,
    EMOTE_PATHOGEN_GLANDS,
    EMOTE_METABOLIC_BOOST,
    EMOTE_ERUPTING_PUSTULES
};

enum eCreatures : uint32
{
    NPC_LIVING_FLUID        = 69069,
    NPC_AT_STALKER          = 69081
};

const int32 ACTION_ACTIVATE_PRIMORDIUS = 32;

static const Position aFluidPos[10] =
{
    { 5660.9f, 4656.5f, 55.5f, 3.1f },
    { 5650.3f, 4689.8f, 55.5f, 3.6f },
    { 5625.5f, 4713.5f, 55.5f, 4.1f },
    { 5558.9f, 4714.1f, 55.5f, 5.2f },
    { 5535.1f, 4689.2f, 55.5f, 5.8f },
    { 5525.8f, 4656.1f, 55.5f, 0.02f},
    { 5534.9f, 4622.8f, 55.5f, 0.5f },
    { 5558.4f, 4597.4f, 55.5f, 1.0f },
    { 5626.2f, 4597.7f, 55.5f, 2.1f },
    { 5650.9f, 4622.4f, 55.5f, 2.6f }
};

static const std::pair<uint32, uint32> m_pMutations[4] = 
{
    { SPELL_KEEN_EYESIGHT, SPELL_IMPAIRED_EYESIGHT },
    { SPELL_IMPROVED_SYNAPSES, SPELL_DULLED_SYNAPSES },
    { SPELL_THICK_BONES, SPELL_FRAGILE_BONES },
    { SPELL_CLEAR_MIND, SPELL_CLOUDED_MIND }
};

static const uint32 m_auiEvolutions[6] =
{
    SPELL_VENTRAL_SACS,
    SPELL_ACIDIC_SPINES,
    SPELL_ERUPTING_PUSTULES,
    SPELL_METABOLIC_BOOST,
    SPELL_PATHOGEN_GLANDS,
    SPELL_GAS_BLADDER
};

static inline const uint32 GetTotalMutations(Unit* pUnit)
{
    uint32 m_uiMutations = 0;

    Aura* pAura;

    for (uint8 i = 0; i < 4; ++i)
    {
        if (pAura = pUnit->GetAura(m_pMutations[i].first))
            m_uiMutations += pAura->GetStackAmount();
    }

    return m_uiMutations;
};

class boss_primordius : public CreatureScript
{
public:
    boss_primordius() : CreatureScript("boss_primordius") {}

    class boss_primordiusAI : public BossAI
    {
    public:
        boss_primordiusAI(Creature* pCreature) : BossAI(pCreature, DATA_PRIMORDIUS)
        {
            m_uiPosition = 10;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            
            if (Aura* pAura = me->AddAura(SPELL_EMERGE, me))
                pAura->SetDuration(-1);

            me->AddAura(SPELL_PRIMORDIUS_INTRO_COSMETIC, me);
            me->SetReactState(REACT_PASSIVE);
            m_bIntroDone = false;
        }

        EventMap m_mDuringCastEvents;
        EventMap m_mOocEvents;

        std::array<uint32, 3> m_uiEvolutions;

        bool m_bDouble;
        bool m_bIntroDone;

        uint32 m_uiPosition;

        void Reset() override
        {         
            me->setPowerType(POWER_ENERGY);
            me->SetMaxPower(POWER_ENERGY, 60);
            me->SetPower(POWER_ENERGY, 0);
            me->AddAura(SPELL_ZERO_POWER, me);

            me->AddAura(SPELL_MUTATED_ABOMINATION, me);

            me->SetFullHealth();

            instance->SetBossState(DATA_PRIMORDIUS, NOT_STARTED);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            for (uint8 i = 0; i < 3; ++i)
            {
                m_uiEvolutions[i] = 0;
            }

            events.Reset();
            m_mDuringCastEvents.Reset();
            m_mOocEvents.Reset();

            if (m_bIntroDone)
                m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 4000);

            summons.DespawnAll();

            m_bDouble = false;
        }

        void DoAction(const int32 iAction) override
        {
            if (iAction == ACTION_START_INTRO)
            {
                m_bIntroDone = true;
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 7000);

                me->RemoveAurasDueToSpell(SPELL_EMERGE);
                me->RemoveAurasDueToSpell(SPELL_PRIMORDIUS_INTRO_COSMETIC);

                me->SetReactState(REACT_AGGRESSIVE);
            }

            if (iAction == ACTION_ACTIVATE_PRIMORDIUS)
            {
                m_mOocEvents.ScheduleEvent(EVENT_INTRO_1, 2000);
            }
        }

        void EnterCombat(Unit* /*pwho*/) override
        {
            instance->SetBossState(DATA_PRIMORDIUS, IN_PROGRESS);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);

            me->SummonCreature(59508, me->GetPositionX(), me->GetPositionY(), TEMPSUMMON_MANUAL_DESPAWN);

            m_mDuringCastEvents.ScheduleEvent(EVENT_LIVING_FLUID, 3000);
            m_mDuringCastEvents.ScheduleEvent(EVENT_BERSERK, 8 * MINUTE*IN_MILLISECONDS);
            m_mDuringCastEvents.ScheduleEvent(EVENT_GAIN_ENERGY, 800);

            events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 10000 + rand() % 5000);
            events.ScheduleEvent(EVENT_CHECK_ENERGY, 300);

            me->RemoveAurasDueToSpell(SPELL_SUBMERGE_COSMETIC);

            Talk(TALK_AGGRO);
        }

        void JustDied(Unit* /*pKiller*/) override
        {
            instance->SetBossState(DATA_PRIMORDIUS, DONE);
            instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

            Talk(TALK_DEATH);
        }

        void KilledUnit(Unit* /*pVictim*/) override
        {
            Talk(TALK_SLAY);
        }

        void JustSummoned(Creature* pSummoned) override
        {
            summons.Summon(pSummoned);
        }

        void SummonedCreatureDespawn(Creature* pSummoned) override
        {
            summons.Despawn(pSummoned);
        }

        void UpdateAI(const uint32 uiDiff) override
        {
            if (!UpdateVictim())
            {
                m_mOocEvents.Update(uiDiff);

                switch (m_mOocEvents.ExecuteEvent())
                {
                    case EVENT_RELOCATE:
                    {
                        uint32 m_uiNewPos = urand(0, 9);

                        while (m_uiNewPos == m_uiPosition)
                            m_uiNewPos = urand(0, 9);

                        m_uiPosition = m_uiNewPos;

                        me->NearTeleportTo(aFluidPos[m_uiPosition].GetPositionX(), aFluidPos[m_uiPosition].GetPositionY(), aFluidPos[m_uiPosition].GetPositionZ(), aFluidPos[m_uiPosition].GetOrientation());
                        me->SetFacingTo(aFluidPos[m_uiPosition].GetOrientation());

                        if (!m_bIntroDone)
                        {
                            if (Aura* pAura = me->GetAura(SPELL_SUBMERGE_COSMETIC))
                                pAura->SetDuration(10000);
                        }
                        else
                            m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 12000);
                    }
                    break;
                    case EVENT_SUBMERGE:
                    {
                        DoCast(SPELL_SUBMERGE_COSMETIC);
                        m_mOocEvents.ScheduleEvent(EVENT_RELOCATE, 4000);
                    }
                    break;
                    case EVENT_INTRO_1:
                        me->RemoveAurasDueToSpell(SPELL_EMERGE);
                        Talk(TALK_INTRO_1, 0, false, TEXT_RANGE_AREA);
                        m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 6000);
                        m_mOocEvents.ScheduleEvent(EVENT_INTRO_2, 21500);
                    break;
                    case EVENT_INTRO_2:
                        Talk(TALK_INTRO_2, 0, false, TEXT_RANGE_AREA);
                        m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 6000);
                        m_mOocEvents.ScheduleEvent(EVENT_INTRO_3, 21500);
                        break;
                    case EVENT_INTRO_3:
                        Talk(TALK_INTRO_3, 0, false, TEXT_RANGE_AREA);
                        m_mOocEvents.ScheduleEvent(EVENT_SUBMERGE, 6000);
                        m_mOocEvents.ScheduleEvent(EVENT_INTRO_4, 21500);
                        break;
                    case EVENT_INTRO_4:
                        Talk(TALK_INTRO_4, 0, false, TEXT_RANGE_AREA);
                        DoAction(ACTION_START_INTRO);
                        break;
                    }      
                return;
            }

            m_mDuringCastEvents.Update(uiDiff);
            events.Update(uiDiff);

            switch (m_mDuringCastEvents.ExecuteEvent())
            {
            case EVENT_LIVING_FLUID:
                SummonLivingFluid();
                break;
            case EVENT_BERSERK:
                DoCast(me, SPELL_BERSERK_PRIMORDIUS, true);
                break;
            case EVENT_GAIN_ENERGY:
                me->SetPower(POWER_ENERGY, me->GetPower(POWER_ENERGY) + 1);
                
                if (m_bDouble)
                {
                    m_mDuringCastEvents.ScheduleEvent(EVENT_GAIN_ENERGY, 800);
                    m_bDouble = false;
                }
                else
                {
                    m_mDuringCastEvents.ScheduleEvent(EVENT_GAIN_ENERGY, 1500);
                    m_bDouble = true;
                }
                break;
            }

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CHECK_ENERGY:
                    if (me->GetPower(POWER_ENERGY) >= 60)
                    {
                        DoCast(me, SPELL_EVOLUTION);
                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 2500);
                    }
                    else
                        events.ScheduleEvent(EVENT_CHECK_ENERGY, 300);
                    break;
                case EVENT_PRIMORDIAL_STRIKE:
                    DoCast(me->GetVictim(), SPELL_PRIMORDIAL_STRIKE);
                    events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 20000 + rand() % 5000 * GetEventTimer());
                    break;
                case EVENT_CAUSTIC_GAS:
                    if (me->HasAura(SPELL_GAS_BLADDER))
                    {
                        DoCast(SPELL_CAUSTIC_GAS);
                        events.ScheduleEvent(EVENT_CAUSTIC_GAS, 12000 + rand() % 5000 * GetEventTimer());
                    }
                    break;
                case EVENT_VOLATILE_PATHOGEN:
                    if (me->HasAura(SPELL_PATHOGEN_GLANDS))
                    {
                        if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 1, 0.f, true, -(int32)SPELL_VOLATILE_PATHOGEN))
                            DoCast(pUnit, SPELL_VOLATILE_PATHOGEN);
                        else if (Unit* pUnit = SelectTarget(SELECT_TARGET_RANDOM, 0, 0.f, true))
                            DoCast(pUnit, SPELL_VOLATILE_PATHOGEN);

                        events.ScheduleEvent(EVENT_VOLATILE_PATHOGEN, 25000 * GetEventTimer());
                    }
                    break;
                }
            }
            
            DoMeleeAttackIfReady();
        }

        float GetEventTimer() const
        {
            if (me->HasAura(SPELL_METABOLIC_BOOST))
                return 0.8f;
            
            return 1.f;
        }

        void SummonLivingFluid()
        {
            for (uint8 i = 0; i < 10; ++i)
            {
                me->SummonCreature(NPC_LIVING_FLUID, aFluidPos[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 5000 + rand() % 2000);
            }

            m_mDuringCastEvents.ScheduleEvent(EVENT_LIVING_FLUID, 10000);
        }

        std::array<uint32, 3> GetEvolutions() const
        {
            return m_uiEvolutions;
        }

        uint32 GetNumberOfEvolutions() const
        {
            uint32 m_uiValue = 0;

            for (uint8 i = 0; i < 3; ++i)
            {
                if (m_uiEvolutions[i] > 0)
                    ++m_uiValue;
            }

            return m_uiValue;
        }

        void ShiftEvolutions(uint32 m_uiEvolution)
        {
            me->SetPower(POWER_ENERGY, 0);
            bool m_bShift = true;

            // Fill the array from std::begin() so that 
            // the oldest evolution is placed first
            for (uint8 i = 0; i < 3; ++i)
            {
                // If we're not filled yet, we can stop the function here
                if (m_uiEvolutions[i] == 0)
                {
                    m_uiEvolutions[i] = m_uiEvolution;
                    m_bShift = false;
                    break;
                }
            }

            // Shift evolutions
            if (m_bShift)
            {
                me->RemoveAurasDueToSpell(m_uiEvolutions[0]);

                m_uiEvolutions[0] = m_uiEvolutions[1];
                m_uiEvolutions[1] = m_uiEvolutions[2];
                m_uiEvolutions[2] = m_uiEvolution;
            }

            me->AddAura(m_uiEvolution, me);
            Talk(TALK_EVOLVE);

            events.DelayEvents(2000);

            switch (m_uiEvolution)
            {
            case SPELL_GAS_BLADDER:
                events.ScheduleEvent(EVENT_CAUSTIC_GAS, 1000);
                Talk(EMOTE_GAS_BLADDER, 0, false, TEXT_RANGE_AREA);
                break;
            case SPELL_PATHOGEN_GLANDS:
                events.ScheduleEvent(EVENT_VOLATILE_PATHOGEN, 1000);
                Talk(EMOTE_PATHOGEN_GLANDS, 0, false, TEXT_RANGE_AREA);
                break;
            case SPELL_VENTRAL_SACS:
                Talk(EMOTE_VENTRAL_SACS, 0, false, TEXT_RANGE_AREA);
                break;
            case SPELL_ACIDIC_SPINES:
                Talk(EMOTE_ACIDIC_SPINES, 0, false, TEXT_RANGE_AREA);
                break;
            case SPELL_METABOLIC_BOOST:
                Talk(EMOTE_METABOLIC_BOOST, 0, false, TEXT_RANGE_AREA);
                break;
            case SPELL_ERUPTING_PUSTULES:
                Talk(EMOTE_ERUPTING_PUSTULES, 0, false, TEXT_RANGE_AREA);
                break;
            default:
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_primordiusAI(pCreature);
    }
};

class npc_living_fluid : public CreatureScript
{
public:
    npc_living_fluid() : CreatureScript("npc_living_fluid") {}

    class ai_impl : public ScriptedAI
    {
    public:
        ai_impl(Creature* pCreature) : ScriptedAI(pCreature) {}

        bool m_bIsVolatile;
        bool m_bHasDied;

        void Reset() override
        {
            me->SetReactState(REACT_PASSIVE);

            events.ScheduleEvent(EVENT_MOVE, 1000);
            events.ScheduleEvent(EVENT_CHECK_PROXIMITY, 1500);

            DecideVolatility();

            m_bHasDied = false;

            me->SetWalk(true);
        }

        void DamageTaken(Unit* /*pDealer*/, uint32& uiDamage) override
        {
            if (uiDamage >= me->GetHealth())
            {
                if (m_bHasDied)
                    uiDamage = 0;

                if (m_bIsVolatile && !m_bHasDied)
                {
                    uiDamage = 0;

                    me->SetWalk(false);
                    me->SetSpeed(MOVE_RUN, 0.35f, true);
                    me->SetSpeed(MOVE_WALK, 0.35f, true);
                    events.ScheduleEvent(EVENT_MOVE, 200);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    DoCast(me, SPELL_VOLATILE_POOL);

                    me->StopMoving();
                    me->GetMotionMaster()->MoveIdle();

                    events.Reset();

                    m_bHasDied = true;

                    me->SetDisplayId(11686);
                    me->UpdateObjectVisibility();
                }
                else
                {
                    if (Creature* pBoss = GetClosestCreatureWithEntry(me, BOSS_PRIMORDIUS, 200.f))
                    {
                        if (Creature* pStalker = pBoss->SummonCreature(NPC_AT_STALKER, *me))
                            pStalker->CastSpell(me, SPELL_MUTAGENIC_POOL, true);
                    }
                }
            }
        }

        void UpdateAI(const uint32 uiDiff) override
        {
            events.Update(uiDiff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case EVENT_MOVE:
                Move();
                break;
            case EVENT_CHECK_PROXIMITY:
                CheckProximity();
                break;
            }
        }

        void Move()
        {
            if (Creature* pPrimordius = GetClosestCreatureWithEntry(me, BOSS_PRIMORDIUS, 200.f))
            {
                me->GetMotionMaster()->MovePoint(4, *pPrimordius);
            }

            events.ScheduleEvent(EVENT_MOVE, 200);
        }

        void CheckProximity()
        {
            if (Creature* pPrimordius = GetClosestCreatureWithEntry(me, BOSS_PRIMORDIUS, 20.f))
            {
                if (me->GetExactDist2d(pPrimordius) < 5.1f)
                {
                    me->StopMoving();
                    me->GetMotionMaster()->MoveIdle();
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    DoCast(SPELL_MUTATE_PRIMORDIUS);
                    me->DespawnOrUnsummon(200);
                    return;
                }
            }

            events.ScheduleEvent(EVENT_CHECK_PROXIMITY, 300);
        }

        void DecideVolatility()
        {
            if (rand() % 9 < 4)
                m_bIsVolatile = true;
            else
                m_bIsVolatile = false;
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new ai_impl(pCreature);
    }
};

class hasAuraPredicate
{
    private:
        Unit* pUnit;
    public:
        hasAuraPredicate(Unit* _pUnit) : pUnit(_pUnit) {}

        bool operator()(const uint32 mValue) const
        {
            return pUnit->HasAura(mValue);
        }
};

class spell_evolve_primordius : public SpellScriptLoader
{
public:
    spell_evolve_primordius() : SpellScriptLoader("spell_evolve_primordius") {}

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
        {
            if (Creature* pCreature = GetHitCreature())
            {
                std::list<uint32> m_lEvolveList;

                for (uint8 i = 0; i < 6; ++i)
                    m_lEvolveList.push_back(m_auiEvolutions[i]);

                m_lEvolveList.remove_if(hasAuraPredicate(pCreature));

                // We'll crash next line anyway, might as well assert the shit out of this function here
                ASSERT(!m_lEvolveList.empty());

                uint32 m_uiAura = Trinity::Containers::SelectRandomContainerElement(m_lEvolveList);
                CAST_AI(boss_primordius::boss_primordiusAI, pCreature->AI())->ShiftEvolutions(m_uiAura);
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_acidic_explosion : public SpellScriptLoader
{
public:
    spell_acidic_explosion() : SpellScriptLoader("spell_acidic_explosion") {}

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void SelectTargets(std::list<WorldObject*>&targets)
        {
            targets.remove_if(notPlayerPredicate());
        }

        void HandleEffectHitTarget(SpellEffIndex eff_idx)
        {
            Unit* pUnit = GetHitUnit();
            Unit* pCaster = GetCaster();

            if (!pCaster || !pUnit)
                return;

            pCaster->CastSpell(pUnit, GetSpellInfo()->Effects[eff_idx].BasePoints, true);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_impl::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_pustule_eruption : public SpellScriptLoader
{
public:
    spell_pustule_eruption() : SpellScriptLoader("spell_pustule_eruption") {}

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void SelectTargets(std::list<WorldObject*>&targets)
        {
            targets.remove_if(notPlayerPredicate());

            if (targets.size() > 1)
                Trinity::Containers::RandomResizeList(targets, 1);
        }

        void HandleEffectHitTarget(SpellEffIndex eff_idx)
        {
            Unit* pUnit = GetHitUnit();
            Unit* pCaster = GetCaster();

            if (!pCaster || !pUnit)
                return;

            pCaster->CastSpell(pUnit, GetSpellInfo()->Effects[eff_idx].BasePoints, true);
        }

        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_impl::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_caustic_gas : public SpellScriptLoader
{
public:
    spell_caustic_gas() : SpellScriptLoader("spell_caustic_gas") {}

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        uint32 m_uiTargetCount;

        bool Load() override
        {
            m_uiTargetCount = 1;
            return true;
        }

        void SelectTargets(std::list<WorldObject*>&targets)
        {
            targets.remove_if(notPlayerPredicate());

            m_uiTargetCount = targets.size();
        }

        void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
        {
            SetHitDamage(GetHitDamage() / m_uiTargetCount);
        }

        void Register() override
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_impl::SelectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_mutation_primordius : public SpellScriptLoader
{
public:
    spell_mutation_primordius() : SpellScriptLoader("spell_mutation_primordius") {}

    class spell_impl : public SpellScript
    {
        PrepareSpellScript(spell_impl);

        void HandleEffectHitTarget(SpellEffIndex /*eff_idx*/)
        {
            if (Unit* pUnit = GetHitUnit())
            {
                if (pUnit->HasAura(SPELL_FULLY_MUTATED))
                {
                    uint32 m_uiInfection = m_pMutations[urand(0, 3)].second;

                    if (Aura* pAura = pUnit->GetAura(m_uiInfection))
                        pAura->ModStackAmount(1);
                    else
                        pUnit->AddAura(m_uiInfection, pUnit);
                }
                else
                {
                    const std::pair<uint32, uint32> m_pMutationPair = m_pMutations[urand(0, 3)];
                    const uint32 m_uiTotalStacks = GetTotalMutations(pUnit);

                    if (Aura* pAura = pUnit->GetAura(m_pMutationPair.first))
                    {
                        if (pAura->GetStackAmount() >= 4)
                        {
                            pAura->Remove(AURA_REMOVE_BY_ENEMY_SPELL);

                            if (Aura* pNegAura = pUnit->GetAura(m_pMutationPair.second))
                                pNegAura->ModStackAmount(1);
                            else
                                pUnit->AddAura(m_pMutationPair.second, pUnit);
                        }
                        else
                            pAura->ModStackAmount(1);
                    }
                    else
                        pUnit->AddAura(m_pMutationPair.first, pUnit);

                    if (m_uiTotalStacks + 1 >= 5)
                    {
                        for (uint8 i = 0; i < 4; ++i)
                        {
                            if (Aura* pAura = pUnit->GetAura(m_pMutations[i].first))
                                pAura->RefreshDuration();
                        }

                        pUnit->CastSpell(pUnit, SPELL_FULLY_MUTATED, true);

                        if (Pet* pPet = pUnit->ToPlayer()->GetPet())
                            pUnit->AddAura(SPELL_FULLY_MUTATED_PET, pPet);
                    }
                }
            }
        }

        void Register() override
        {
            OnEffectHitTarget += SpellEffectFn(spell_impl::HandleEffectHitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_impl();
    }
};

class spell_primordius_mutation_aura : public SpellScriptLoader
{
public:
    spell_primordius_mutation_aura() : SpellScriptLoader("spell_primordius_mutation_aura") {}

    class aura_impl : public AuraScript
    {
        PrepareAuraScript(aura_impl);

        void HandleOnDispel(DispelInfo* /*info*/)
        {
            for (uint8 i = 0; i < 4; ++i)
            {
                uint32 m_uiMutation = m_pMutations[i].first;
                uint32 m_uiInfection = m_pMutations[i].second;

                if (GetId() != m_uiMutation)
                    GetOwner()->ToUnit()->RemoveAura(m_uiMutation);

                if (GetId() != m_uiInfection)
                    GetOwner()->ToUnit()->RemoveAura(m_uiInfection);
            }
        }

        void Register() override
        {
            OnDispel += AuraDispelFn(aura_impl::HandleOnDispel);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new aura_impl();
    }
};

class spell_submerge_cosmetic_aura : public SpellScriptLoader
{
public:
    spell_submerge_cosmetic_aura() : SpellScriptLoader("spell_submerge_cosmetic_aura") {}

    class aura_impl : public AuraScript
    {
        PrepareAuraScript(aura_impl);

        void HandleOnApply(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetOwner()->ToUnit())
                target->SetVisible(false);
        }

        void HandleOnRemove(AuraEffect const* /*aureff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Unit* target = GetOwner()->ToUnit())
                if (!target->IsVisible())
                    target->SetVisible(true);
        }

        void Register()
        {
            OnEffectApply += AuraEffectApplyFn(aura_impl::HandleOnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(aura_impl::HandleOnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new aura_impl();
    }
};

class sat_mutagenic_pool : public SpellAreaTriggerScript
{
public:
    sat_mutagenic_pool() : SpellAreaTriggerScript("sat_mutagenic_pool") {}

    class sat_impl : public IAreaTriggerOnce
    {
        bool CheckTriggering(WorldObject* pTriggering) override
        {
            Player* pPlayer = pTriggering->ToPlayer();

            if (!pPlayer)
                return false;

            return pPlayer->IsAlive() && m_target->IsWithinDistInMap(pPlayer, m_range);
        }

        void OnTrigger(WorldObject* pTriggering) override
        {
            pTriggering->ToUnit()->CastSpell(pTriggering->ToUnit(), SPELL_MUTATION, true);
            return;
        }
    };

    IAreaTrigger* GetInterface() const override
    {
        return new sat_impl();
    }
};

class sat_volatile_pool_player : public SpellAreaTriggerScript
{
public:
    sat_volatile_pool_player() : SpellAreaTriggerScript("sat_volatile_pool_player") {}

    class sat_impl : public IAreaTriggerAura
    {
        bool CheckTriggering(WorldObject* pTriggering) override
        {
            Player* pPlayer = pTriggering->ToPlayer();

            if (!pPlayer)
                return false;

            return pPlayer->IsAlive() && (m_target->GetExactDist2d(pPlayer) < m_range);
        }

        void OnTriggeringApply(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                if (!pTarget->HasAura(SPELL_VOLATILE_MUTATION_AURA, m_caster->GetGUID()))
                    m_caster->AddAura(SPELL_VOLATILE_MUTATION_AURA, pTarget);
            }
        }

        void OnTriggeringRemove(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                if (Aura* pAura = pTarget->GetAura(SPELL_VOLATILE_MUTATION_AURA, m_caster->GetGUID()))
                    pAura->Remove(AURA_REMOVE_BY_EXPIRE);
            }
        }

        void OnTriggeringUpdate(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                if (Aura* pAura = pTarget->GetAura(SPELL_VOLATILE_MUTATION_AURA))
                    pAura->RefreshDuration();
            }
        }
    };

    IAreaTrigger* GetInterface() const
    {
        return new sat_impl();
    }
};

class sat_volatile_pool_primordius : public SpellAreaTriggerScript
{
public:
    sat_volatile_pool_primordius() : SpellAreaTriggerScript("sat_volatile_pool_primordius") {}

    class sat_impl : public IAreaTriggerAura
    {
        bool CheckTriggering(WorldObject* pTriggering) override
        {
            Player* pPlayer = pTriggering->ToPlayer();

            if (!pPlayer || !m_caster)
                return false;

            return m_caster->IsWithinDistInMap(pPlayer, m_range);
        }

        void OnTriggeringApply(WorldObject* pTriggering) override
        {
            if (Player* pPlayer = pTriggering->ToPlayer())
            {
                if (Creature* pBoss = GetClosestCreatureWithEntry(pPlayer, BOSS_PRIMORDIUS, 15.f))
                {
                    if (pBoss->IsWithinDistInMap(m_caster, 4.f))
                    {
                        pBoss->CastSpell(pBoss, SPELL_VOLATILE_MUTATION_PRIMORDIUS, true);

                        Destroy();
                        return;
                    }
                }
            }
        }
    };

    IAreaTrigger* GetInterface() const
    {
        return new sat_impl();
    }
};

class sat_red_pool_player : public SpellAreaTriggerScript
{
public:
    sat_red_pool_player() : SpellAreaTriggerScript("sat_red_pool_player") {}

    class sat_impl : public IAreaTriggerAura
    {
        bool CheckTriggering(WorldObject* pTriggering) override
        {
            Player* pPlayer = pTriggering->ToPlayer();

            if (!pPlayer)
                return false;

            return pPlayer->IsAlive() && (m_target->GetExactDist2d(pPlayer) < m_range);
        }

        uint32 GetMutation()
        {
            switch (rand()%3)
            {
            case 0:
                return 136180;
            case 1:
                return 136182;
            case 2:
                return 136184;
            case 3:
                return 136186;
            default:
                break;
            }
        }

        void OnTriggeringApply(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                    m_caster->AddAura(GetMutation(), pTarget);
            }
        }

        void OnTriggeringRemove(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                if (Aura* pAura = pTarget->GetAura(SPELL_VOLATILE_MUTATION_AURA, m_caster->GetGUID()))
                    pAura->Remove(AURA_REMOVE_BY_EXPIRE);
            }
        }

        void OnTriggeringUpdate(WorldObject* pTriggering) override
        {
            if (Unit* pTarget = pTriggering->ToUnit())
            {
                if (Aura* pAura = pTarget->GetAura(SPELL_VOLATILE_MUTATION_AURA))
                    pAura->RefreshDuration();
            }
        }
    };

    IAreaTrigger* GetInterface() const
    {
        return new sat_impl();
    }
};

class AreaTrigger_at_primordius_intro : public AreaTriggerScript
{
public:

    AreaTrigger_at_primordius_intro()
        : AreaTriggerScript("at_primordius_intro")
    {
    }

    bool OnTrigger(Player* pPlayer, AreaTriggerEntry const* /*pTrigger*/)
    {
        if (Creature* pBoss = GetClosestCreatureWithEntry(pPlayer, BOSS_PRIMORDIUS, 150.f))
        {
            if (pBoss->GetInstanceScript()->GetData(TYPE_PRIMORDIUS_INTRO) == DONE)
                return true;

            if (pBoss->AI())
            {
                pBoss->GetInstanceScript()->SetData(TYPE_PRIMORDIUS_INTRO, DONE);
                pBoss->AI()->DoAction(ACTION_ACTIVATE_PRIMORDIUS);
                return true;
            }
        }
        return true;
    }
};

class trigger_done_mutation : public CreatureScript
{
public:
    trigger_done_mutation() : CreatureScript("trigger_done_mutation")
    {
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new trigger_done_mutationAI(creature);
    }

    struct trigger_done_mutationAI : public ScriptedAI
    {
        trigger_done_mutationAI(Creature* creature) : ScriptedAI(creature) {}

        EventMap events;

        void Reset()
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*who*/) {}

        void JustDied(Unit* /*killer*/) {}

        bool trig;

        void UpdateAI(const uint32 diff)
        {

            events.Update(diff);

            if (Creature* pBoss = GetClosestCreatureWithEntry(me, BOSS_PRIMORDIUS, 200.f))
            {
                std::list<Player*> PlayersInArea;
                GetPlayerListInGrid(PlayersInArea, me, 200.0f);

                uint32 k = 0;
                for (auto _player : PlayersInArea)
                {
                    if (_player->HasAura(SPELL_KEEN_EYESIGHT))
                        k++;
                    if (_player->HasAura(SPELL_IMPROVED_SYNAPSES))
                        k++;
                    if (_player->HasAura(SPELL_THICK_BONES))
                        k++;
                    if (_player->HasAura(SPELL_CLEAR_MIND))
                        k++;
                    if (k >= 2)
                        if (!_player->HasAura(SPELL_FULLY_MUTATED))
                            me->AddAura(SPELL_FULLY_MUTATED, _player);
                }
            }
        }
    };
};



void AddSC_boss_primordius()
{
    new boss_primordius();
    new npc_living_fluid();

    new spell_acidic_explosion();
    new spell_pustule_eruption();
    new spell_caustic_gas();
    new spell_evolve_primordius();
    new spell_mutation_primordius();
    new spell_primordius_mutation_aura();

    new spell_submerge_cosmetic_aura();

    new sat_mutagenic_pool();
    new sat_volatile_pool_player();
    new sat_red_pool_player();
    new trigger_done_mutation();
    //new sat_volatile_pool_primordius();

    new AreaTrigger_at_primordius_intro();
}
