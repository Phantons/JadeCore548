////////////////////////////////////////////////////////////////////////////////
///
///  HeroesWoW
///  Copyright 2015 HeroesWoW
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "throne_of_thunder.h"

/*
1.1.1. Ji-Kun and Adds
p_Difficulty	Ji-Kun	Hatchlings	Fledgelings	Juveniles	Nest Guardians
10-man	244M	1.2M	1.2M	1.9M	Heroic-only
10-man Heroic	366M	1.8M	1.8M	2.7M	3.3M
25-man	732M	1.2M	???M	???M	Heroic-only
25-man Heroic	1,099M	1.9M	???M	2.74M	4.22M
LFR	???M	???M	???M	???M	Heroic-only
1.1.2. Eggs
p_Difficulty	Young Egg of Ji-Kun	Mature Egg of Ji-Kun
10-man	1.2M	1.5M
10-man Heroic	1.8M	2.7M
25-man	???M	2M
25-man Heroic	1.9M	2.74M
LFR	???M	???M
1.2. Enrage Timer

We do not know what the hard enrage timer of this fight is, currently. There does not appear to be a soft enrage timer of any sort.

1.3. Raid Composition

p_Difficulty	Tanks	Healers	DPS
10-man	2	2-3	5-6
25-man	2	5-7	16-18
*/

enum eJiKunSpells
{
    SpellInfectedTalonAura   = 140092,
    SpellTalonRake           = 134366,
    SpellDownDraft           = 134370,
    SpellCaw                 = 138926,
    SpellQuills              = 140801,

    // Hatchling Phase
    SpellFeedYoung           = 137528,
    SpellSlimed              = 134256,
    SpellFeedPoolVisual      = 139284,
    SpellFeedPoolDot         = 138319,
    SpellNutrimentBuff       = 112879,
    SpellNutrimentProjectile = 140741,
    SpellFeedVisual          = 138840,
    SpellEat                 = 134321,
    SpellMorph               = 134322,
    SpellImpactFeedPool      = 138209,
    SpellJumpToTarget        = 138907,
    SpellJumpToTargetFood    = 138904,
    SpellCryForFood          = 137500,
    SpellBeamGreen           = 138914,
    SpellBeamFood            = 138915,
    SpellHatchJuventile      = 138905,
    SpellSlimedSoap          = 138309,

    // Flight
    SpellDaedalingWings      = 134339,
    SpellLessonOfIcarus      = 140571,
    SpellFlight              = 133755,

    // Juventile
    SpellCheep               = 139296,

    // Fledgling
    SpellJiKunFledglingSumm  = 134367,

    SpellEnrage              = 47008,
};

enum eJiKunCreatures
{
    CreatureYoungEggOfJiKun  = 68194,
    CreatureMatureEggOfJiKun = 69628,

    CreatureHatchling        = 68192,
    CreatureFledgling        = 68193,
    CreatureJuventile        = 70095,

    CreatureFledglingPoop    = 68202,
    CreatureFeedPoolTrigger  = 68188,

    CreatureBooger           = 70130,
    CreatureGreenBooger      = 68178,
};

enum eJiKunEvents
{
    // Basic
    EventInfectedTalon  = 1,
    EventTalonRake      = 2,
    EventDownDraft      = 3,
    EventCaw            = 4,
    EventQuills         = 5, 
    
    // Feed
    EventFeedYoung      = 6,
    EventSlimed         = 7,
    EventFeedPool       = 8,
    EventNutriment      = 9,
    EventCallFood       = 10,
    EventPoisonImpact   = 30,

    // Flight
    EventDaedalingWings = 31,

    // Eggs
    EventYoungEgg       = 11,
    EventMatureEgg      = 12,
    EventEatFood        = 13,
    EventHatch          = 14,

    // Adds
    EventCheep          = 15,
    EventFledglingCheep = 16,
    EventLayEgg         = 17,
    EventRandomMovement = 18,

    // Misc
    EventEnrage         = 32,
    EventDownDraftDis   = 33,
};

enum eJiKunActions
{
    ActionEatFood = 90,
};

// testing slava

Position l_PositionTopBottom = { 6194.445f, 4267.452f, -70.423f,  5.626903f };
Position l_PositionMidBottom = { 6159.333f, 4370.531f, -70.539f,  5.399492f };
Position l_PositionLowBottom = { 6219.288f, 4332.667f, -58.754f, 0.308391f };

Position l_PositionTopTop    = { 6153.50f, 4331.00f, 70.147f, 0.757968f   };
Position l_PositionMidTop    = { 6218.72f, 4352.984f, 66.202f, 3.262941f  };
Position l_PositionLowTop    = { 6172.024f, 4240.175f, 41.599f, 0.041575f };

static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
    if (creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

class ji_kun_delete_beacons : public BasicEvent
{
public:
    explicit ji_kun_delete_beacons(GameObject* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (obj)
        {
            if (InstanceScript* instance = obj->GetInstanceScript())
            {
                switch (modifier)
                {
                case 0:
                    obj->Delete();
                    break;
                }
            }
        }
        return true;
    }
private:
    GameObject* storm;
    GameObject* obj;
    int modifier;
    int Event;
};

class npc_teleport_to_ji_kun : public CreatureScript
{
public:
    npc_teleport_to_ji_kun() : CreatureScript("npc_teleport_to_ji_kun") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->PlayerTalkClass->ClearMenus();
        if (InstanceScript* instance = player->GetInstanceScript())
            if (Creature* JiKun = instance->instance->GetCreature(instance->GetData64(DATA_JI_KUN)))
            {
                if (!JiKun->isInCombat())
                    player->NearTeleportTo(6107.726f, 4294.886f, -31.862038f, 0.506807f);
            } 

		return true;
    }
};

// Ji Kun
class boss_ji_kun : public CreatureScript
{
public:
    boss_ji_kun() : CreatureScript("boss_ji_kun") { }

    struct boss_ji_kunAI : public BossAI
    {
        boss_ji_kunAI(Creature* creature) : BossAI(creature, DATA_JI_KUN)
        {
           m_Instance = creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        EventMap m_Events;
        Unit* m_Target;
        Unit* m_TargetThrow;
        uint32 m_Count = 0;
        uint32 m_DownDraftDiff = 0;
        uint32 m_DownDraftDiffCancel = 0;
        bool m_Intro;
        bool m_DownDraft;  

        void Reset() override
        {
            _Reset();
            events.Reset();
            summons.DespawnAll();

            me->SetMaxHealth(244000000);

            instance->DoRemoveAurasDueToSpellOnPlayers(eJiKunSpells::SpellSlimed);
            instance->DoRemoveAurasDueToSpellOnPlayers(eJiKunSpells::SpellSlimedSoap);
            instance->DoRemoveAurasDueToSpellOnPlayers(133755);
            instance->DoRemoveAurasDueToSpellOnPlayers(140014);
            instance->DoRemoveAurasDueToSpellOnPlayers(140571);

            int32 Creatures[9] = { eJiKunCreatures::CreatureBooger, eJiKunCreatures::CreatureFeedPoolTrigger,
                eJiKunCreatures::CreatureFledgling, eJiKunCreatures::CreatureFledglingPoop,
                eJiKunCreatures::CreatureGreenBooger, eJiKunCreatures::CreatureHatchling, 
                eJiKunCreatures::CreatureJuventile, eJiKunCreatures::CreatureMatureEggOfJiKun, 
                eJiKunCreatures::CreatureYoungEggOfJiKun};

            for (int i = 0; i <= 8; i++)
                DespawnCreaturesInArea(Creatures[i], me);

            m_DownDraftDiffCancel = 10000;
            m_DownDraftDiff = 3700;
            m_DownDraft = false;
            m_Count = 0;

            me->setFaction(16);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, UnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void JustReachedHome() override
        {
            _JustReachedHome();

            if (m_Instance)
                m_Instance->SetBossState(eData::DATA_JI_KUN, EncounterState::FAIL);

            int32 Creatures[9] = { eJiKunCreatures::CreatureBooger, eJiKunCreatures::CreatureFeedPoolTrigger,
                eJiKunCreatures::CreatureFledgling, eJiKunCreatures::CreatureFledglingPoop,
                eJiKunCreatures::CreatureGreenBooger, eJiKunCreatures::CreatureHatchling,
                eJiKunCreatures::CreatureJuventile, eJiKunCreatures::CreatureMatureEggOfJiKun,
                eJiKunCreatures::CreatureYoungEggOfJiKun };

            for (int i = 0; i <= 8; i++)
                DespawnCreaturesInArea(Creatures[i], me);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            if (m_Instance)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            // Enrage
            events.ScheduleEvent(eJiKunEvents::EventEnrage, 420000);

            // Basic
            events.ScheduleEvent(eJiKunEvents::EventCaw, 22 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eJiKunEvents::EventQuills, 45 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eJiKunEvents::EventInfectedTalon, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eJiKunEvents::EventDownDraft, 80 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eJiKunEvents::EventTalonRake, 10 * TimeConstants::IN_MILLISECONDS);

            // Eggs
            events.ScheduleEvent(eJiKunEvents::EventYoungEgg, 6 * TimeConstants::IN_MILLISECONDS);

            // Poision Pool
            events.ScheduleEvent(eJiKunEvents::EventFeedPool, 4 * TimeConstants::IN_MILLISECONDS);
        }

        void JustDied(Unit* p_Killer) override
        {
            if (m_Instance)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                m_Instance->SetBossState(eData::DATA_JI_KUN, EncounterState::DONE);
            }

            summons.DespawnAll();
            DespawnCreaturesInArea(eJiKunCreatures::CreatureHatchling, me);
            DespawnCreaturesInArea(eJiKunCreatures::CreatureFledgling, me);
            DespawnCreaturesInArea(eJiKunCreatures::CreatureJuventile, me);
            DespawnCreaturesInArea(eJiKunCreatures::CreatureMatureEggOfJiKun, me);
            DespawnCreaturesInArea(eJiKunCreatures::CreatureYoungEggOfJiKun, me);

            _JustDied();
        }

        void InformPlayers(const char * format)
        {
            std::list<Player*> pl_list;
            me->GetPlayerListInGrid(pl_list, 300.0f);

            if (pl_list.empty())
                return;

            for (auto itr : pl_list)
            {
                itr->GetSession()->SendNotification(format);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (m_DownDraft)
            {
                if (m_DownDraftDiffCancel <= p_Diff)
                {
                    m_DownDraft = false;

                    m_DownDraftDiffCancel = 10000;
                }
                else
                    m_DownDraftDiffCancel -= p_Diff;

                if (m_DownDraftDiff <= p_Diff)
                {
                    std::list<Player*> l_PlayerListDown;
                    me->GetPlayerListInGrid(l_PlayerListDown, 50.0f);

                    if (l_PlayerListDown.empty())
                        return;

                    for (auto itr : l_PlayerListDown)
                    {
                        if (!itr->HasAura(133755))
                        itr->CastSpell(itr, 133331);
                        //itr->GetMotionMaster()->MoveKnockbackFrom(itr->GetPositionX(), itr->GetPositionY(), 6.0f, 3.0f);
                    }

                    m_DownDraftDiff = 3700;
                }
                else
                    m_DownDraftDiff -= p_Diff;
            }

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventEnrage:
                    me->CastSpell(me, eJiKunSpells::SpellEnrage);
                    break;
                case eJiKunEvents::EventCaw:
                    InformPlayers("Ji Kun is casting Caw, SPREAD OUT!");

                    if (Unit * l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 45.0f, true))
                        me->CastSpell(l_Random, eJiKunSpells::SpellCaw);

                    events.ScheduleEvent(eJiKunEvents::EventCaw, 25 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventQuills:
                    me->CastSpell(me, eJiKunSpells::SpellQuills);

                    InformPlayers("Ji Kun sends her[Quils] flying in every direction");

                    events.ScheduleEvent(eJiKunEvents::EventQuills, 45 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventInfectedTalon:
                    if (Unit * l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Random, eJiKunSpells::SpellInfectedTalonAura);

                    events.ScheduleEvent(eJiKunEvents::EventInfectedTalon, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventTalonRake:
                    if (Unit * l_Victim = me->getVictim())
                    {
                        me->CastSpell(l_Victim, eJiKunSpells::SpellInfectedTalonAura);
                        me->CastSpell(l_Victim, eJiKunSpells::SpellTalonRake);
                    }

                    events.ScheduleEvent(eJiKunEvents::EventTalonRake, 58 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventDownDraft:
                    m_DownDraft = true;
                    m_DownDraftDiff = 3000;
                    m_DownDraftDiffCancel = 10000;
                    me->CastSpell(me, eJiKunSpells::SpellDownDraft);

 
                    InformPlayers("Ji Kun uses her wings to create a massive [Down Draft]!");
                    events.ScheduleEvent(eJiKunEvents::EventDownDraft, 80 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventDownDraftDis:
                    m_DownDraft = false;
                    break;
                case eJiKunEvents::EventFeedPool:
                    me->CastSpell(me, eJiKunSpells::SpellFeedYoung);
              
                    events.ScheduleEvent(eJiKunEvents::EventFeedPool, 38 * TimeConstants::IN_MILLISECONDS);
                    break;
                    // Eggs
                case eJiKunEvents::EventYoungEgg:
                {                  
                    m_Count++;

                    GameObject* beacon = NULL;

                    if (m_Count == 8)
                        m_Count = 0;

                    switch (m_Count)
                    {
                        case 0:
                            beacon = me->SummonGameObject(148937, l_PositionLowBottom.GetPositionX(), l_PositionLowBottom.GetPositionY(), l_PositionLowBottom.GetPositionZ(), l_PositionLowBottom.GetOrientation(), 0,0,0,0,0);
                            
                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            for (int i = 0; i < 7; i++)
                            {
                                me->SummonCreature(CreatureYoungEggOfJiKun, l_PositionLowBottom.GetPositionX() + i, l_PositionLowBottom.GetPositionY() + i, l_PositionLowBottom.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                            break;
                        case 1:
                            beacon = me->SummonGameObject(148937, l_PositionTopBottom.GetPositionX(), l_PositionTopBottom.GetPositionY(), l_PositionTopBottom.GetPositionZ(), l_PositionTopBottom.GetOrientation(), 0, 0, 0, 0, 0);

                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            for (int i = 0; i < 7; i++)
                            {
                                me->SummonCreature(CreatureYoungEggOfJiKun, l_PositionTopBottom.GetPositionX() + i, l_PositionTopBottom.GetPositionY() + i, l_PositionTopBottom.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                            break;
                        case 2:
                            beacon = me->SummonGameObject(148937, l_PositionMidBottom.GetPositionX(), l_PositionMidBottom.GetPositionY(), l_PositionMidBottom.GetPositionZ(), l_PositionMidBottom.GetOrientation(), 0, 0, 0, 0, 0);

                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            for (int i = 0; i < 7; i++)
                            {
                                me->SummonCreature(CreatureYoungEggOfJiKun, l_PositionMidBottom.GetPositionX() + i, l_PositionMidBottom.GetPositionY() + i, l_PositionMidBottom.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                            break;
                        case 3:
                            beacon = me->SummonGameObject(148937, l_PositionTopTop.GetPositionX(), l_PositionTopTop.GetPositionY(), l_PositionTopTop.GetPositionZ(), l_PositionTopTop.GetOrientation(), 0, 0, 0, 0, 0);
                            InformPlayers("Ji Kun spawn Hatchlings in the upper nests!");

                            me->SummonCreature(CreatureMatureEggOfJiKun, l_PositionTopTop.GetPositionX(), l_PositionTopTop.GetPositionY(), l_PositionTopTop.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            break;
                        case 4:
                            beacon = me->SummonGameObject(148937, l_PositionLowBottom.GetPositionX(), l_PositionLowBottom.GetPositionY(), l_PositionLowBottom.GetPositionZ(), l_PositionLowBottom.GetOrientation(), 0, 0, 0, 0, 0);

                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            for (int i = 0; i < 7; i++)
                            {
                                me->SummonCreature(CreatureYoungEggOfJiKun, l_PositionLowBottom.GetPositionX() + i, l_PositionLowBottom.GetPositionY() + i, l_PositionLowBottom.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                            break;
                        case 5:
                            InformPlayers("Ji Kun spawn Hatchlings in the upper nests!");


                            beacon = me->SummonGameObject(148937, l_PositionMidTop.GetPositionX(), l_PositionMidTop.GetPositionY(), l_PositionMidTop.GetPositionZ(), l_PositionMidTop.GetOrientation(), 0, 0, 0, 0, 0);

                            me->SummonCreature(CreatureMatureEggOfJiKun, l_PositionMidTop.GetPositionX(), l_PositionMidTop.GetPositionY(), l_PositionMidTop.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            break;
                        case 6:
                            InformPlayers("Ji Kun spawn Hatchlings in the upper nests!");

                            beacon = me->SummonGameObject(148937, l_PositionLowBottom.GetPositionX(), l_PositionLowBottom.GetPositionY(), l_PositionLowBottom.GetPositionZ(), l_PositionLowBottom.GetOrientation(), 0, 0, 0, 0, 0);

                            me->SummonCreature(CreatureMatureEggOfJiKun, l_PositionLowBottom.GetPositionX(), l_PositionLowBottom.GetPositionY(), l_PositionLowBottom.GetPositionY(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            break;
                        case 7:
                            beacon = me->SummonGameObject(148937, l_PositionLowBottom.GetPositionX(), l_PositionLowBottom.GetPositionY(), l_PositionLowBottom.GetPositionZ(), l_PositionLowBottom.GetOrientation(), 0, 0, 0, 0, 0);

                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            for (int i = 0; i < 7; i++)
                            {
                                me->SummonCreature(CreatureYoungEggOfJiKun, l_PositionLowBottom.GetPositionX() + i, l_PositionLowBottom.GetPositionY() + i, l_PositionLowBottom.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            }
                            break;
                        case 8:
                            InformPlayers("Ji Kun spawn Hatchlings in the lower nests!");

                            beacon = me->SummonGameObject(148937, l_PositionMidTop.GetPositionX(), l_PositionMidTop.GetPositionY(), l_PositionMidTop.GetPositionZ(), l_PositionMidTop.GetOrientation(), 0, 0, 0, 0, 0);

                            me->SummonCreature(CreatureMatureEggOfJiKun, l_PositionMidTop.GetPositionX(), l_PositionMidTop.GetPositionY(), l_PositionMidTop.GetPositionZ(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                            break;                        
                    }

                    if (beacon)
                        me->m_Events.AddEvent(new ji_kun_delete_beacons(beacon, 0), me->m_Events.CalculateTime(7000));

                    events.ScheduleEvent(eJiKunEvents::EventYoungEgg, 40 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new boss_ji_kunAI(creature);
    }
};

// Young Egg
class creature_ji_kun_young_egg : public CreatureScript
{
public:
    creature_ji_kun_young_egg() : CreatureScript("creature_ji_kun_young_egg") { }

    struct creature_ji_kun_young_eggAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_young_eggAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset() override
        {
            me->setFaction(16);
            me->SetReactState(REACT_PASSIVE);

            me->SetMaxHealth(1200000);

            events.ScheduleEvent(eJiKunEvents::EventHatch, 2 * TimeConstants::IN_MILLISECONDS);
        }       

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventHatch:
                    me->DespawnOrUnsummon(6000);
                    me->SummonCreature(eJiKunCreatures::CreatureHatchling, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_DEAD_DESPAWN);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new creature_ji_kun_young_eggAI(creature);
    }
};

// Mature egg
class creature_ji_kun_mature_egg : public CreatureScript
{
public:
    creature_ji_kun_mature_egg() : CreatureScript("creature_ji_kun_mature_egg") { }

    struct creature_ji_kun_mature_eggAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_mature_eggAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset() override
        {
            me->setFaction(16);
            me->SetReactState(REACT_PASSIVE);
            me->CastSpell(me, 119879);

            me->SetMaxHealth(1500000);

            events.ScheduleEvent(eJiKunEvents::EventHatch, 20 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
            case eJiKunEvents::EventHatch:
                me->DespawnOrUnsummon(6000);
                me->SummonCreature(eJiKunCreatures::CreatureJuventile, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_DEAD_DESPAWN);
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new creature_ji_kun_mature_eggAI(creature);
    }
};

// Fledgling Poop
class creature_ji_kun_fledgling_poop : public CreatureScript
{
public:
    creature_ji_kun_fledgling_poop() : CreatureScript("creature_ji_kun_fledgling_poop") { }

    struct creature_ji_kun_fledgling_poopAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_fledgling_poopAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset() override
        {
            me->setFaction(16);
            me->SetReactState(REACT_PASSIVE);

            me->SetMaxHealth(1200000);

            events.ScheduleEvent(eJiKunEvents::EventHatch, 10 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
            case eJiKunEvents::EventHatch:
                me->DespawnOrUnsummon(1500);
                me->SummonCreature(eJiKunCreatures::CreatureFledgling, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), TEMPSUMMON_DEAD_DESPAWN);
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new creature_ji_kun_fledgling_poopAI(creature);
    }
};

// Mature egg
class creature_ji_kun_hatchlings : public CreatureScript
{
public:
    creature_ji_kun_hatchlings() : CreatureScript("creature_ji_kun_hatchlings") { }

    struct creature_ji_kun_hatchlingsAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_hatchlingsAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        bool m_Ate;

        void Reset() override
        {
            me->setFaction(16);
            DoZoneInCombat();
            me->SetMaxHealth(1200000);

            if (pInstance)
            {
                if (Creature* JiKun = Creature::GetCreature(*me, pInstance->GetData64(DATA_JI_KUN)))
                    Creature* booger = me->SummonCreature(eJiKunCreatures::CreatureBooger, JiKun->GetPositionX(), JiKun->GetPositionY(), -19.895319f, JiKun->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
            }

            m_Ate = false;
            events.ScheduleEvent(eJiKunEvents::EventCheep, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
            //events.ScheduleEvent(eJiKunEvents::EventCallFood, 2 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
            case eJiKunActions::ActionEatFood:
                events.ScheduleEvent(eJiKunEvents::EventEatFood, 8000);
                break;
            }
        }

        void JustDied(Unit* p_Killer) override
        {
            me->CastSpell(me, 134338); // feathers drop areatyrigger
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);    

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventEatFood:
                    if (m_Ate)
                        return;

                    m_Ate = true;

                    me->CastSpell(me, eJiKunSpells::SpellEat);
                    me->RemoveAura(SpellFeedVisual);

                    events.ScheduleEvent(eJiKunEvents::EventCheep, urand(6000, 8000));
                    events.ScheduleEvent(eJiKunEvents::EventLayEgg, 15);
                    break;
                case eJiKunEvents::EventLayEgg:
                    me->CastSpell(me, eJiKunSpells::SpellJiKunFledglingSumm);
                    events.ScheduleEvent(eJiKunEvents::EventLayEgg, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventCheep:
                    if (me->HasAura(eJiKunSpells::SpellMorph))
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        {
                            int32 damage = 100000;

                            me->CastCustomSpell(target, eJiKunSpells::SpellCheep, &damage, NULL, NULL, true, NULL);
                        }
                    }
                    else
                    {
                        if (Unit * target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                            me->CastSpell(target, eJiKunSpells::SpellCheep);
                    }
                    events.ScheduleEvent(eJiKunEvents::EventCheep, urand(6000, 8000));
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_ji_kun_hatchlingsAI(creature);
    }
};

// Fledgling
class creature_ji_kun_fledgling : public CreatureScript
{
public:
    creature_ji_kun_fledgling() : CreatureScript("creature_ji_kun_fledgling") { }

    struct creature_ji_kun_fledglingAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_fledglingAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset() override
        {
            me->setFaction(16);
            DoZoneInCombat();

            me->SetMaxHealth(1200000);

            events.ScheduleEvent(eJiKunEvents::EventLayEgg, 15);
            events.ScheduleEvent(eJiKunEvents::EventCheep, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
        }

        void JustDied(Unit* p_Killer) override
        {
            me->CastSpell(me, 134338); // feathers drop areatyrigger
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventCheep:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                    {
                        int32 damage = 100000;

                        me->CastCustomSpell(target, eJiKunSpells::SpellCheep, &damage, NULL, NULL, true, NULL);
                    }

                    events.ScheduleEvent(eJiKunEvents::EventCheep, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eJiKunEvents::EventLayEgg:
                    me->CastSpell(me, eJiKunSpells::SpellJiKunFledglingSumm);
                    events.ScheduleEvent(eJiKunEvents::EventLayEgg, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_ji_kun_fledglingAI(creature);
    }
};

// feed poop
class creature_feed_booger_projectile : public CreatureScript
{
public:
    creature_feed_booger_projectile() : CreatureScript("creature_feed_booger_projectile") { }

    struct creature_feed_pool_projectileAI : public ScriptedAI
    {
        creature_feed_pool_projectileAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        bool m_Explosion;

        void Reset() override
        {
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

            me->SetDisableGravity(true);
            me->SetCanFly(true);

            if (TempSummon* tempo = me->ToTempSummon())
                if (Unit* owner = tempo->GetSummoner())
                    me->CastSpell(owner, eJiKunSpells::SpellJumpToTargetFood);

            m_Explosion = false;
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (!m_Explosion)
            {              
                if (Creature* creature = me->FindNearestCreature(CreatureHatchling, 1.0f, true))
                {               
                    me->DespawnOrUnsummon(1000);
                    
                    creature->AddAura(eJiKunSpells::SpellFeedPoolVisual, creature);
                    
                    if (creature->GetAI())
                        creature->GetAI()->DoAction(eJiKunActions::ActionEatFood);

                    m_Explosion = true;
                }
                
                if (Player* player = me->FindNearestPlayer(1.0f, true))
                {
                    me->DespawnOrUnsummon(1000);

                    player->CastSpell(player, eJiKunSpells::SpellNutrimentProjectile);

                    m_Explosion = true;
                }
            }

            if (!UpdateVictim())
                return;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_feed_pool_projectileAI(creature);
    }
};

// Poison poop
class creature_poison_projectile : public CreatureScript
{
public:
    creature_poison_projectile() : CreatureScript("creature_poison_projectile") { }

    struct creature_feed_pool_projectileAI : public ScriptedAI
    {
        creature_feed_pool_projectileAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        bool m_Explosion;

        void Reset() override
        {
            me->setFaction(35);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);

            me->SetDisableGravity(true);
            me->SetCanFly(true);

            m_Explosion = false;

            events.ScheduleEvent(eJiKunEvents::EventPoisonImpact, 8 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (Player* nearest = me->FindNearestPlayer(1.0f, true))
            {
                if (me->GetPositionZ() > -18.351f)
                {
                    me->DespawnOrUnsummon();
                    nearest->CastSpell(nearest, eJiKunSpells::SpellSlimedSoap);
                }
            }

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventPoisonImpact:
                    me->CastSpell(me, eJiKunSpells::SpellImpactFeedPool, true);
                    me->DespawnOrUnsummon(500);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_feed_pool_projectileAI(creature);
    }
};

/// Juventile
class creature_ji_kun_juventile : public CreatureScript
{
public:
    creature_ji_kun_juventile() : CreatureScript("creature_ji_kun_juventile") { }

    struct creature_ji_kun_juventileAI : public ScriptedAI
    {
        creature_ji_kun_juventileAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;

        void Reset() override
        {
            me->setFaction(16);
            DoZoneInCombat();

            me->SetMaxHealth(1900000);

            me->SetDisableGravity(true);
            me->SetCanFly(true);

            events.ScheduleEvent(eJiKunEvents::EventRandomMovement, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eJiKunEvents::EventCheep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eJiKunEvents::EventRandomMovement:
                    Position g_Pos;
                    me->GetRandomNearPosition(g_Pos, 20.0f);
               
                    me->GetMotionMaster()->MoveTakeoff(0, g_Pos.GetPositionX(), g_Pos.GetPositionY(), -11.132423f);
                    events.ScheduleEvent(eJiKunEvents::EventRandomMovement, 8 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eJiKunEvents::EventCheep:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                    {
                        int32 damage = 200000;

                        me->CastCustomSpell(target, eJiKunSpells::SpellCheep, &damage, NULL, NULL, true, NULL);
                    }

                    events.ScheduleEvent(eJiKunEvents::EventCheep, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_ji_kun_juventileAI(creature);
    }
};

// Feed Pool
class creature_ji_kun_feed_pool : public CreatureScript
{
public:
    creature_ji_kun_feed_pool() : CreatureScript("creature_ji_kun_feed_pool") { }

    struct creature_ji_kun_feed_poolAI : public Scripted_NoMovementAI
    {
        creature_ji_kun_feed_poolAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            pInstance = p_Creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        EventMap events;


        uint32 diff;
        uint32 diffstack;
        uint32 stack;

        void Reset() override
        {
            me->setFaction(16);
            DoZoneInCombat();
            diff = 500;
            diffstack = 1000;
            
            me->SetObjectScale(2.4f);
            stack = 0;

            me->CastSpell(me, eJiKunSpells::SpellFeedPoolVisual);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            events.ScheduleEvent(eJiKunEvents::EventCheep, 8 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (diff <= p_Diff)
            {
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 1.1f);

                if (l_ListPlayers.empty())
                    return;

                for (auto itr : l_ListPlayers)
                {
                    itr->CastSpell(itr, eJiKunSpells::SpellSlimed);

                    if (AuraPtr aura = me->GetAura(SpellSlimed))
                    {
                        aura->SetDuration(1);
                    }
                    
                    stack++;

                    if (stack > 2)
                    {
                        me->DespawnOrUnsummon();
                    }
                }

                diff = 1000;
            }
            else
                diff -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new creature_ji_kun_feed_poolAI(creature);
    }
};

// Feed Young
class spell_ji_kun_feed_poison_dummy : public SpellScriptLoader
{
public:
    spell_ji_kun_feed_poison_dummy() : SpellScriptLoader("spell_ji_kun_feed_poison_dummy") { }

    class spell_ji_kun_feed_poison_dummy_aura_Script : public AuraScript
    {
        PrepareAuraScript(spell_ji_kun_feed_poison_dummy_aura_Script);

        void OnTick(constAuraEffectPtr aurEff)
        {
            PreventDefaultAction();

            if (Unit* caster = GetCaster())
            {
                std::list<Player*> pl_list;
                caster->GetPlayerListInGrid(pl_list, 30.0f);

                if (pl_list.empty())
                    return;

                for (auto itr : pl_list)
                {
                    Creature* globule = caster->SummonCreature(eJiKunCreatures::CreatureGreenBooger, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), caster->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0);

                    if (globule)
                    {
                        globule->CastSpell(itr, eJiKunSpells::SpellJumpToTargetFood);
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_ji_kun_feed_poison_dummy_aura_Script::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_ji_kun_feed_poison_dummy_aura_Script();
    }
};

// Eat - 
class spell_eat : public SpellScriptLoader
{
public:
    spell_eat() : SpellScriptLoader("spell_eat") {}

    class spell_eat_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_eat_spell_script);

        void Unload()
        {
            if (GetCaster() && GetCaster()->GetTypeId() != TYPEID_PLAYER)
            {
                GetCaster()->CastSpell(GetCaster(), eJiKunSpells::SpellMorph);
                GetCaster()->SetName("Fledgling");
                GetCaster()->ToCreature()->SetReactState(REACT_AGGRESSIVE);
            }
        }
        void HandleAfterCast()
        {
        }

        void Register()
        {   
            AfterCast += SpellCastFn(spell_eat_spell_script::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_eat_spell_script();
    }
};

// Call Food
class spell_call_food : public SpellScriptLoader
{
public:
    spell_call_food() : SpellScriptLoader("spell_call_food") {}

    class spell_call_food_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_call_food_spell_script);

        void HandleAfterCast()
        {
            if (GetCaster())
            {
                if (InstanceScript* Pinstance = GetCaster()->GetInstanceScript())
                {
                    if (Creature* JiKun = Creature::GetCreature(*GetCaster(), Pinstance->GetData64(DATA_JI_KUN)))
                    {
                        Creature* booger = JiKun->SummonCreature(eJiKunCreatures::CreatureBooger, JiKun->GetPositionX(), JiKun->GetPositionY(), JiKun->GetPositionZ(), JiKun->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);

                        if (booger)
                        {
                            booger->CastSpell(GetCaster(), eJiKunSpells::SpellJumpToTarget);
                        }
                    }
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_call_food_spell_script::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_call_food_spell_script();
    }
};

// Flight
class spell_flight : public SpellScriptLoader
{
public:
    spell_flight() : SpellScriptLoader("spell_flight") { }

    class spell_flight_spell_Script : public SpellScript
    {
        PrepareSpellScript(spell_flight_spell_Script);

        SpellCastResult CheckTarget()
        {
            if (GetCaster())
            {
                if (!GetCaster()->HasAura(140014))
                    return SPELL_FAILED_DONT_REPORT;
                else
                    return SPELL_CAST_OK;
            }
			return SPELL_FAILED_DONT_REPORT;
        }

        void OnCastRemoveStack()
        {
            if (GetCaster())
            {
                if (AuraPtr aura = GetCaster()->GetAura(140014))
                {
                    if (aura->GetStackAmount() > 1)
                        aura->SetStackAmount(aura->GetStackAmount() - 1);
                    else
                        aura->Remove();
                }
            }
        }
       

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_flight_spell_Script::CheckTarget);
            OnCast += SpellCastFn(spell_flight_spell_Script::OnCastRemoveStack);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_flight_spell_Script();
    }
};


void AddSC_boss_ji_kun()
{
    new boss_ji_kun();
    new creature_ji_kun_hatchlings();
    new creature_ji_kun_fledgling();
    new creature_ji_kun_juventile();
    new creature_ji_kun_young_egg();
    new creature_ji_kun_mature_egg();
    new npc_teleport_to_ji_kun();
    new creature_ji_kun_fledgling_poop();
    new creature_ji_kun_feed_pool();
    new creature_feed_booger_projectile();
    new creature_poison_projectile();
    new spell_call_food();
    new spell_eat();
    new spell_flight();
    new spell_ji_kun_feed_poison_dummy();
}

