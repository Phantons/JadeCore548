/*
* Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
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

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "Group.h"
#include "ScriptedCreature.h"
#include "throne_of_thunder.h"  

/*
10-man	315M	45M	Heroic-only
*/

enum Spells
{
    // frozen_head
    SPELL_ARCTIC_FREEZE_AURA = 139841,
    SPELL_ARCTIC_FREEZE_DAMAGE = 139842,
    SPELL_ARCTIC_FREEZE_AURA_STUN = 139844,
    SPELL_ARCTIC_FREEZE_APPLY_AURA = 139843, // heroic only - Instant
  
    SPELL_TORRENT_OF_ICE_AURA = 139866,
    SPELL_TORRENT_OF_ICE_SUMMON = 139870,
    SPELL_TORRENT_OF_ICE_DUMMY = 139889,
    SPELL_TORRENT_OF_ICE_APPLY_AURA = 139890,
    SPELL_ICY_GROUND_DEBUFF = 139909,
    SPELL_ICY_GROUND_VISUAL = 134694,
    SPELL_FROST_RAMPAGE = 139816,
    // flaming_head
    SPELL_CINDERS_AURA = 139822,
    SPELL_CINDERS_SUMMON = 139834,
    SPELL_CINDERS_SUMMON_APPLY_AURA = 139835,
    SPELL_CINDERS_SUMMON_DUMMY = 139836,
    SPELL_IGNITE_FLESH_AURA = 137729,
    SPELL_IGNITE_FLESH_DAMAGE = 137730,
    //SPELL_IGNITE_FLESH_AURA = 137731, // heroic only - Instant

    SPELL_FLAME_RAMPAGE = 139758,
    // Venomous_Head
    SPELL_ACID_RAIN_DUMMY = 134378,
    SPELL_ACID_RAIN_TARGET_DUMMY_QUESTION_MARK = 134408, // dafuq blizz?
    SPELL_ACID_RAIN_PUKE_VISUAL = 139845,
    SPELL_ACID_RAIN_WHIRL_VISUAL = 139847,
    SPELL_ACID_RAIN_GLOBUE_PUKE_VISUAL = 139848,
    SPELL_ACID_RAIN_EXPLOSION_DAMAGE = 139850,
    SPELL_ROT_ARMOR_ARMOR_REDUCTION = 137737,
    SPELL_ROT_ARMOR_DAMAGE_DUMMY = 139838,
    SPELL_VENOM_RAMPAGE = 139819,

    SPELL_DAMAGE_INCREASE = 139586,
    
    SPELL_HYDRA_FRENZY = 139942,
    AURA_CONCEALING_FOG = 137973,
};
enum eeTriggers
{
    TRIGGER_CINDERS_POOL = 70432,
    TRIGGER_ICY_GROUND = 70446,
    TRIGGER_ACID_RAIN = 734244,
};
enum GameObjecto
{
    GAMEOBJECT_CHEST = 325262,
};
enum eeEvents
{
   // Megaera flame head
    EVENT_IGNITE_FLESH = 43242,
    EVENT_CINDERS = 43243,
   // Megaera frost head
   EVENT_ARCTIVE_FREEZE = 43244,
   EVENT_TORRENT_OF_ICE = 43245,
   // Megaera venom head
   EVENT_ROT_ARMOR = 43246,
   EVENT_ACID_RAIN = 43247,
   // Megaera
   EVENT_KILL_BOSS = 44425,
   EVENT_RAMPAGE = 44426,
   EVENT_STOP_RAMPAGE = 44427,
   // Enrage
   EVENT_ENRAGE = 266622,
};
enum eeActions
{
    ACTION_HEAD_GOT_KILLED = 543763,
    ACTION_FIRE_KILLED = 42343,
    ACTION_FROST_KILLED = 42344,
    ACTION_VENOM_KILLED = 42345,
    ACTION_RAMPAGE = 342266,
};
enum eeTalks
{
  
};
enum Times
{
};

Position socketone = {6429.48f, 4536.83f, -210.060f, 2.776865f};
Position sockettwo = {6429.7f, 4561.38f, -209.937f, 3.428967f};
Position socketthree = {6414.34f, 4511.92f, -209.99f, 2.148770f};
Position socketfour = { 6393.42f, 4504.77f, -209.707f, 1.916239f };

Position backgroundsocket[10] =
{    
    { 6446.33f, 4473.29f, -210.312f, 2.209119f },
    { 6478.64f, 4512.21f, -210.312f, 2.711774f },
    { 6483.73f, 4567.84f, -210.312f, 3.304177f },
    { 6399.89f, 4470.18f, -210.032f, 1.776580f },
    { 6475.30f, 4552.18f, -210.033f, 3.601265f },
    { 6409.92f, 4464.09f, -210.031f, 1.767359f },
    { 6450.82f, 4473.22f, -210.032f, 2.128642f },
    { 6480.98f, 4516.28f, -210.031f, 2.890998f },
    { 6487.01f, 4577.88f, -210.032f, 3.474753f },
    { 6470.99f, 4504.51f, -210.031f, 2.665792f },
};

Position chestrewardforkillers = {6391.17f, 4548.81f, -209.175f, 5.709439f};
class boss_megaera : public CreatureScript
{
public:
    boss_megaera() : CreatureScript("boss_megaera") { }

    struct boss_megaeraAI : public BossAI
    {
        boss_megaeraAI(Creature* creature) : BossAI(creature, DATA_MEGAERA)
        {
            pInstance = creature->GetInstanceScript();
        }

        uint32 count = 0;
        uint32 red = 0;
        uint32 blue = 0;
        uint32 green = 0;

        InstanceScript* pInstance;
        EventMap events;

        void Reset()
        {
            _Reset();
            events.Reset();
            summons.DespawnAll();

            DespawnCreaturesInArea(TRIGGER_ICY_GROUND, me);
            DespawnCreaturesInArea(TRIGGER_CINDERS_POOL, me);
            DespawnCreaturesInArea(TRIGGER_ACID_RAIN, me);

            DespawnCreaturesInArea(CREATURE_HEAD_FIRE, me);
            DespawnCreaturesInArea(CREATURE_HEAD_FROST, me);
            DespawnCreaturesInArea(CREATURE_HEAD_VENOM, me);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
      
            me->SetReactState(REACT_AGGRESSIVE);
            me->AddUnitState(UNIT_STATE_CANNOT_AUTOATTACK);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);

            count = 0;

            red = 1;
            blue = 1;
            green = 1;


            Creature* head = me->SummonCreature(CREATURE_HEAD_FIRE, backgroundsocket[1], TEMPSUMMON_MANUAL_DESPAWN);
            if (head)
            {
                head->SetReactState(REACT_AGGRESSIVE);
                head->AddAura(137973, head);
                head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            //SummonRed(red, true);
            SummonBlue(blue, false);
            SummonGreen(green, false);
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();

            DespawnCreaturesInArea(TRIGGER_ICY_GROUND, me);
            DespawnCreaturesInArea(TRIGGER_CINDERS_POOL, me);
            DespawnCreaturesInArea(TRIGGER_ACID_RAIN, me);

            DespawnCreaturesInArea(CREATURE_HEAD_FIRE, me);
            DespawnCreaturesInArea(CREATURE_HEAD_FROST, me);
            DespawnCreaturesInArea(CREATURE_HEAD_VENOM, me);
        }
        void JustReachedHome()
        {
            _JustReachedHome();
            summons.DespawnAll();

            DespawnCreaturesInArea(TRIGGER_ICY_GROUND, me);
            DespawnCreaturesInArea(TRIGGER_CINDERS_POOL, me);
            DespawnCreaturesInArea(TRIGGER_ACID_RAIN, me);

            DespawnCreaturesInArea(CREATURE_HEAD_FIRE, me);
            DespawnCreaturesInArea(CREATURE_HEAD_FROST, me);
            DespawnCreaturesInArea(CREATURE_HEAD_VENOM, me);
        } 
        void SummonRed(int32 modifier, bool clouded)
        {
            if (!clouded)
            {
                for (int i = 0; i < modifier; i++)
                {
                    if (i == 0)
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_FIRE, socketone, TEMPSUMMON_MANUAL_DESPAWN);
                        
                        if (head)
                        {
                            head->SetReactState(REACT_AGGRESSIVE);
                            if (AuraPtr aura = head->GetAura(SPELL_HYDRA_FRENZY))
                            {
                                aura->SetStackAmount(aura->GetStackAmount() + 1);
                            }
                            else
                            {
                                head->AddAura(SPELL_HYDRA_FRENZY, head);
                            }
                        }
                    }
                    else
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_FIRE, backgroundsocket[i], TEMPSUMMON_MANUAL_DESPAWN);
                        if (head)
                        {
                            head->SetReactState(REACT_AGGRESSIVE);
                            head->AddAura(137973, head);
                            head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < modifier; i++)
                {       
                    Creature* head = me->SummonCreature(CREATURE_HEAD_FIRE, backgroundsocket[i], TEMPSUMMON_MANUAL_DESPAWN);
                    if (head)
                    {
                        head->SetReactState(REACT_AGGRESSIVE);
                        head->AddAura(137973, head);
                        head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }
            std::list<Creature*> heads;

            heads.clear();
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    if (Player* nearest = me->FindNearestPlayer(600.0f, true))
                    {
                        itr->Attack(nearest, true);
                        itr->AddThreat(nearest, 100.0f);
                    }
                }
            }
        }
        void SummonBlue(int32 modifier, bool clouded)
        {
            if (!clouded)
            {
                for (int i = 0; i < modifier; i++)
                {
                    if (i == 0)
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_FROST, sockettwo, TEMPSUMMON_MANUAL_DESPAWN);

                        if (head)
                        {
                            head->SetReactState(REACT_AGGRESSIVE);
                            if (AuraPtr aura = head->GetAura(SPELL_HYDRA_FRENZY))
                            {
                                aura->SetStackAmount(aura->GetStackAmount() + 1);
                            }
                            else
                            {
                                head->AddAura(SPELL_HYDRA_FRENZY, head);
                            }
                        }
                    }
                    else
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_FROST, backgroundsocket[i + 1], TEMPSUMMON_MANUAL_DESPAWN);

                        if (head)
                            head->SetReactState(REACT_AGGRESSIVE);
                        head->AddAura(137973, head);
                        head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);            
                    }
                }
            }
            else
            {
                for (int i = 0; i < modifier; i++)
                {
                    Creature* head = me->SummonCreature(CREATURE_HEAD_FROST, backgroundsocket[i + 1], TEMPSUMMON_MANUAL_DESPAWN);

                    if (head)
                        head->SetReactState(REACT_AGGRESSIVE);
                    head->AddAura(137973, head);
                    head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                }
            }
            std::list<Creature*> heads;

            heads.clear();
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    if (Player* nearest = me->FindNearestPlayer(600.0f, true))
                    {
                        itr->Attack(nearest, true);
                        itr->AddThreat(nearest, 100.0f);
                    }
                }
            }
        }
        void SummonGreen(int32 modifier, bool clouded)
        {
            if (!clouded)
            {
                for (int i = 0; i < modifier; i++)
                {
                    if (i == 0)
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_VENOM, socketthree, TEMPSUMMON_MANUAL_DESPAWN);
                        head->SetReactState(REACT_AGGRESSIVE);
                        if (head)
                        {
                            if (AuraPtr aura = head->GetAura(SPELL_HYDRA_FRENZY))
                            {
                                aura->SetStackAmount(aura->GetStackAmount() + 1);
                            }
                            else
                            {
                                head->AddAura(SPELL_HYDRA_FRENZY, head);
                            }
                        }
                    }
                    else
                    {
                        Creature* head = me->SummonCreature(CREATURE_HEAD_VENOM, backgroundsocket[i + 2], TEMPSUMMON_MANUAL_DESPAWN);

                        if (head)
                        {
                            head->AddAura(137973, head);
                            head->SetReactState(REACT_AGGRESSIVE);
                            head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                        }
                    }
                    /*
                    if (red > 2 || blue > 2)
                    {
                        if (red > blue)
                        {
                            Creature* head = me->SummonCreature(CREATURE_HEAD_FIRE, socketthree, TEMPSUMMON_MANUAL_DESPAWN);

                            red++;
                        }
                        else
                        {
                            Creature* head = me->SummonCreature(CREATURE_HEAD_FROST, socketthree, TEMPSUMMON_MANUAL_DESPAWN);

                            blue++;
                        }
                    }
                    */
                }
            }
            else
            {
                for (int i = 0; i < modifier; i++)
                {
                    Creature* head = me->SummonCreature(CREATURE_HEAD_VENOM, backgroundsocket[i + 2], TEMPSUMMON_MANUAL_DESPAWN);

                    if (head)
                    {
                        head->SetReactState(REACT_AGGRESSIVE);
                        head->AddAura(137973, head);
                        head->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }

            std::list<Creature*> heads;
            heads.clear();

            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    if (Player* nearest = me->FindNearestPlayer(600.0f, true))
                    {
                        itr->Attack(nearest, true);
                        itr->AddThreat(nearest, 100.0f);
                    }
                }
            }
        }
        void DoAction(const int32 action)
        {
            switch (action) // killing boss segement
            {
                case ACTION_FIRE_KILLED:
                {
                    std::list<Creature*> heads;
                    heads.clear();

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->DespawnOrUnsummon();
                        }
                    }

                    red++;
                    count++;
       
                    SummonRed(red, true);
                    SummonBlue(blue, false);
                    SummonGreen(green, false);

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->AI()->DoAction(ACTION_RAMPAGE);
                        }
                    }
                    if (count == 6)
                    {
                        events.ScheduleEvent(EVENT_KILL_BOSS, 1000);
                    }
                    break;
                }
                case ACTION_FROST_KILLED:
                {
                    std::list<Creature*> heads;
                    heads.clear();

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->DespawnOrUnsummon();
                        }
                    }

                    blue++;
                    count++;
           
                    SummonRed(red, false);
                    SummonBlue(blue, true);
                    SummonGreen(green, false);

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->AI()->DoAction(ACTION_RAMPAGE);
                        }
                    }
                    if (count == 6)
                    {
                        events.ScheduleEvent(EVENT_KILL_BOSS, 1000);
                    }
                    break;
                }
                case ACTION_VENOM_KILLED:
                {
                    std::list<Creature*> heads;
                    heads.clear();

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->DespawnOrUnsummon();
                        }
                    }

                    green++;
                    count++;

                    SummonRed(red, false);
                    SummonBlue(blue, false);
                    SummonGreen(green, true);

                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
                    me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

                    if (!heads.empty())
                    {
                        for (auto itr : heads)
                        {
                            itr->AI()->DoAction(ACTION_RAMPAGE);
                        }
                    }

                    if (count == 6)
                    {
                        events.ScheduleEvent(EVENT_KILL_BOSS, 1000);
                    }
                    break;
                }
            }

        }
        void EnterCombat(Unit* attacker)
        {
            if (pInstance)
            {
                pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                DoZoneInCombat();
            }

            std::list<Creature*> heads;

            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 300.0F);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 300.0F);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 300.0F);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    itr->Attack(attacker, true);
                    itr->AddThreat(attacker, 100.0f);
                }
            }
        }
        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_KILL_BOSS:
                summons.DespawnAll();
                DespawnCreaturesInArea(TRIGGER_ICY_GROUND, me);
                DespawnCreaturesInArea(TRIGGER_CINDERS_POOL, me);
                DespawnCreaturesInArea(TRIGGER_ACID_RAIN, me);

                me->Kill(me);
                me->DespawnOrUnsummon(4000);
                summons.DespawnAll();

                DespawnCreaturesInArea(CREATURE_HEAD_FIRE, me);
                DespawnCreaturesInArea(CREATURE_HEAD_FROST, me);
                DespawnCreaturesInArea(CREATURE_HEAD_VENOM, me);

                std::list<Player*> pl_list;
                pl_list.clear();

                me->GetPlayerListInGrid(pl_list, 100.0f);

                if (pl_list.empty())
                    return;

                CurrencyOnKillEntry const* Curr = sObjectMgr->GetCurrencyOnKillEntry(me->ToCreature()->GetEntry());
                if (!Curr)
                    return;

                for (auto itr : pl_list)
                {
                    if (Curr->currencyId1 && Curr->currencyCount1)
                    {
                        if (CurrencyTypesEntry const* entry = sCurrencyTypesStore.LookupEntry(Curr->currencyId1))
                        {
                            if (Curr->currencyId1 == CURRENCY_TYPE_JUSTICE_POINTS)
                            {
                                if ((itr->GetCurrency(Curr->currencyId1, true) + Curr->currencyCount1) > itr->GetCurrencyTotalCap(entry))
                                {
                                    uint32 max = itr->GetCurrencyTotalCap(entry);
                                    uint32 lessPoint = max - itr->GetCurrency(Curr->currencyId1, true);
                                    uint32 rest = Curr->currencyCount1 - lessPoint;

                                    itr->ModifyCurrency(Curr->currencyId1, lessPoint);
                                }
                                else
                                    itr->ModifyCurrency(Curr->currencyId1, Curr->currencyCount1);
                            }
                            else
                                itr->ModifyCurrency(Curr->currencyId1, Curr->currencyCount1);
                        }
                    }
                }

                // Place the chest
                me->SummonGameObject(GAMEOBJECT_CHEST, chestrewardforkillers.GetPositionX(), chestrewardforkillers.GetPositionY(), chestrewardforkillers.GetPositionZ(), chestrewardforkillers.GetOrientation(), 0,0,0,0,0);
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_megaeraAI(creature);
    }
};
class creature_head_megaera_fire : public CreatureScript
{
public:
    creature_head_megaera_fire() : CreatureScript("creature_head_megaera_fire") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            events.Reset();
            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
     
        }
        void MoveInLineOfSight(Unit* who)
        {
        }
        void JustDied(Unit* killer)
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->DoAction(ACTION_FIRE_KILLED);
            }
        }
        void JustReachedHome()
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->Reset();
            }
        }
        void DoAction(const int32 action)
        {
            switch (action) // killing boss segement
            {
                case ACTION_RAMPAGE:
                    events.ScheduleEvent(EVENT_RAMPAGE, urand(3000, 7000));
                    events.ScheduleEvent(EVENT_STOP_RAMPAGE, 24000);
                    break;
            }
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_IGNITE_FLESH, 10000);
            events.ScheduleEvent(EVENT_CINDERS, 20000);

            std::list<Creature*> heads;
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    itr->Attack(attacker, true);
                    itr->AddThreat(attacker, 100.0f);
                }
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_RAMPAGE:
                    events.CancelEvent(EVENT_CINDERS);
                    events.CancelEvent(EVENT_IGNITE_FLESH);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        me->CastSpell(target, SPELL_FLAME_RAMPAGE);

                    events.ScheduleEvent(EVENT_RAMPAGE, urand(2000, 7000));
                    break;
                case EVENT_STOP_RAMPAGE:
                    events.CancelEvent(EVENT_RAMPAGE);

                    events.ScheduleEvent(EVENT_IGNITE_FLESH, 10000);
                    events.ScheduleEvent(EVENT_CINDERS, 20000);
                    break;
                case EVENT_IGNITE_FLESH:
                    if (me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    me->CastSpell(me->getVictim(), SPELL_IGNITE_FLESH_AURA);
                    events.ScheduleEvent(EVENT_IGNITE_FLESH, 13000);
                    break;
                case EVENT_CINDERS:
                    if (!me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true, -SPELL_CINDERS_AURA))
                        me->CastSpell(target, SPELL_CINDERS_AURA);

                    events.ScheduleEvent(EVENT_CINDERS, urand(16000, 22000));
                    break;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class creature_head_megaera_frost : public CreatureScript
{
public:
    creature_head_megaera_frost() : CreatureScript("creature_head_megaera_frost") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            events.Reset();

            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
     
        }
        void MoveInLineOfSight(Unit* who)
        {
        }
        void JustReachedHome()
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->Reset();
            }
        }
        void DoAction(const int32 action)
        {
            switch (action) // killing boss segement
            {
            case ACTION_RAMPAGE:
                events.ScheduleEvent(EVENT_RAMPAGE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_STOP_RAMPAGE, 24000);
                break;
            }
        }
        void JustDied(Unit* killer)
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->DoAction(ACTION_FROST_KILLED);
            }
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_ARCTIVE_FREEZE, 12000);
            events.ScheduleEvent(EVENT_TORRENT_OF_ICE, 20000);

            std::list<Creature*> heads;
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    itr->Attack(attacker, true);
                    itr->AddThreat(attacker, 100.0f);
                }
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_RAMPAGE:
                    events.CancelEvent(EVENT_ARCTIVE_FREEZE);
                    events.CancelEvent(EVENT_TORRENT_OF_ICE);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        me->CastSpell(target, SPELL_FROST_RAMPAGE);

                    events.ScheduleEvent(EVENT_RAMPAGE, urand(2000, 7000));
                    break;
                case EVENT_STOP_RAMPAGE:
                    events.ScheduleEvent(EVENT_ARCTIVE_FREEZE, 1000);
                    events.ScheduleEvent(EVENT_TORRENT_OF_ICE, 1000);
                    events.CancelEvent(EVENT_RAMPAGE);
                    break;
                case EVENT_ARCTIVE_FREEZE:
                    if (me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    me->CastSpell(me->getVictim(), SPELL_ARCTIC_FREEZE_AURA);

                    events.ScheduleEvent(EVENT_ARCTIVE_FREEZE, 14000);
                    break;
                case EVENT_TORRENT_OF_ICE:
                    if (!me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                    {
                        me->CastSpell(target, 139866); // icy laser
                    }

                    events.ScheduleEvent(EVENT_TORRENT_OF_ICE, urand(15000, 35000));
                    break;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class creature_head_megaera_venom : public CreatureScript
{
public:
    creature_head_megaera_venom() : CreatureScript("creature_head_megaera_venom") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        void Reset()
        {
            events.Reset();
            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
          
        }
        void MoveInLineOfSight(Unit* who)
        {
        }
        void JustReachedHome()
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->Reset();
            }
        }
        void JustDied(Unit* killer)
        {
            if (Creature* Megaera = instance->instance->GetCreature(instance->GetData64(DATA_MEGAERA)))
            {
                Megaera->AI()->DoAction(ACTION_VENOM_KILLED);
            }
        }
        void DoAction(const int32 action)
        {
            switch (action) // killing boss segement
            {
            case ACTION_RAMPAGE:
                events.ScheduleEvent(EVENT_RAMPAGE, urand(3000, 7000));
                events.ScheduleEvent(EVENT_STOP_RAMPAGE, 24000);
                break;
            }
        }
        void EnterCombat(Unit* attacker)
        {
            events.ScheduleEvent(EVENT_ROT_ARMOR, 10000);
            events.ScheduleEvent(EVENT_ACID_RAIN, 20000);

            std::list<Creature*> heads;
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FIRE, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_FROST, 500.0f);
            me->GetCreatureListWithEntryInGrid(heads, CREATURE_HEAD_VENOM, 500.0f);

            if (!heads.empty())
            {
                for (auto itr : heads)
                {
                    itr->Attack(attacker, true);
                    itr->AddThreat(attacker, 100.0f);
                }
            }
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_RAMPAGE:
                    events.CancelEvent(EVENT_ACID_RAIN);
                    events.CancelEvent(EVENT_ROT_ARMOR);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        me->CastSpell(target, 139818);

                    events.ScheduleEvent(EVENT_RAMPAGE, urand(2000, 7000));
                    break;
                case EVENT_STOP_RAMPAGE:
                    events.ScheduleEvent(EVENT_ROT_ARMOR, 12000);
                    events.ScheduleEvent(EVENT_ACID_RAIN, 20000);

                    events.CancelEvent(EVENT_RAMPAGE);
                    break;
                case EVENT_ROT_ARMOR:
                    if (me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    me->CastSpell(me->getVictim(), SPELL_ROT_ARMOR_DAMAGE_DUMMY);

                    events.ScheduleEvent(EVENT_ROT_ARMOR, 14000);
                    break;
                case EVENT_ACID_RAIN:
                    if (!me->HasAura(AURA_CONCEALING_FOG))
                        return;

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 300.0f, true))
                    {
                        Position pos;
                        target->GetRandomNearPosition(pos, 15.0f);

                        target->SummonCreature(TRIGGER_ACID_RAIN, pos, TEMPSUMMON_MANUAL_DESPAWN);
                    }

                    events.ScheduleEvent(EVENT_ACID_RAIN, urand(20000, 30000));
                    break;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class creature_megaera_acid_nova : public CreatureScript
{
public:
    creature_megaera_acid_nova() : CreatureScript("creature_megaera_acid_nova") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;
        uint32 time;
        void Reset()
        {
            time = 4000;
            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
          
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

            me->CastSpell(me, SPELL_ACID_RAIN_WHIRL_VISUAL);
        }
        void UpdateAI(const uint32 diff)
        {
            if (time <= diff)
            {
                me->CastSpell(me, SPELL_ACID_RAIN_EXPLOSION_DAMAGE);

                time = 70000;
                me->DespawnOrUnsummon(1000);
            }
            else
                time -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class trigger_megaera_fire_pool : public CreatureScript
{
public:
    trigger_megaera_fire_pool() : CreatureScript("trigger_megaera_fire_pool") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;
        uint32 time;
        void Reset()
        {
            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
      
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

            me->CastSpell(me, SPELL_CINDERS_SUMMON_APPLY_AURA);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class creature_trigger_icy_ground : public CreatureScript
{
public:
    creature_trigger_icy_ground() : CreatureScript("creature_trigger_icy_ground") { }

    struct megaera_heads : public ScriptedAI
    {
        megaera_heads(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;
        std::list<Player*> PL_list;
        void Reset()
        {
            PL_list.clear();
            me->setFaction(16);

            me->AddUnitMovementFlag(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
        
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

            me->AddAura(139875, me);
        }
        void UpdateAI(const uint32 diff)
        {
            PL_list.clear();

            JadeCore::AnyPlayerInObjectRangeCheck check(me, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(me, PL_list, check);
            me->VisitNearbyObject(2.0f, searcher);

            if (PL_list.empty())
                return;

            for (std::list<Player*>::const_iterator it = PL_list.begin(); it != PL_list.end(); ++it)
            {
                if (!(*it))
                    return;

                if (!(*it)->HasAura(SPELL_ICY_GROUND_DEBUFF))
                    me->AddAura(SPELL_ICY_GROUND_DEBUFF, (*it));
                else
                {
                    AuraPtr icy_ground = me->GetAura(139909);
                    AuraPtr icy_ground_two = me->GetAura(139922);

                    if (icy_ground_two)
                    {
                        icy_ground_two->SetDuration(2);
                    }
                    if (icy_ground)
                    {
                        icy_ground->SetDuration(2);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new megaera_heads(creature);
    }
};
class spell_megaera_arctic_freeze_stun : public SpellScriptLoader
{
public:
    spell_megaera_arctic_freeze_stun() : SpellScriptLoader("spell_megaera_arctic_freeze_stun") { }

    class megaera_spells : public SpellScript
    {
        PrepareSpellScript(megaera_spells);

        void OnHitEffect()
        {
            if (GetHitUnit())
            {
                AuraPtr aura = GetHitUnit()->GetAura(139843);

                if (aura)
                {
                    if (aura->GetStackAmount() > 4)
                    {
                        aura->Remove();
                        GetHitUnit()->AddAura(SPELL_ARCTIC_FREEZE_AURA_STUN, GetHitUnit());
                    }
                }
            }
        }
        void Register()
        {
            OnHit += SpellHitFn(megaera_spells::OnHitEffect);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new megaera_spells();
    }
};
class spell_megaera_fire_cinders_pool_spawning_mechanism : public SpellScriptLoader
{
public:
    spell_megaera_fire_cinders_pool_spawning_mechanism() : SpellScriptLoader("spell_megaera_fire_cinders_pool_spawning_mechanism") { }

    class megaera_auras : public AuraScript
    {
        PrepareAuraScript(megaera_auras);

        void OnTick(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster() && GetCaster()->GetTypeId() != TYPEID_PLAYER)
            {
                if (Creature* caster = GetCaster()->ToCreature())
                {
                    if (Creature* icy_trap = caster->FindNearestCreature(TRIGGER_CINDERS_POOL, 1.0f))
                        icy_trap->DespawnOrUnsummon(1000);
                }
            }
        }
        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (GetTarget())
            {
                if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                {
                    Position pos;
                    GetTarget()->ToPlayer()->GetPosition(&pos);

                    GetTarget()->SummonCreature(TRIGGER_CINDERS_POOL, pos, TEMPSUMMON_TIMED_DESPAWN, 60000);
                }
            }
        }

        void Register()
        {
            AfterEffectRemove += AuraEffectRemoveFn(megaera_auras::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new megaera_auras();
    }
};
class spell_megaera_torrent_of_ice_tick : public SpellScriptLoader
{
public:
    spell_megaera_torrent_of_ice_tick() : SpellScriptLoader("spell_megaera_torrent_of_ice_tick") { }

    class megaera_spells : public AuraScript
    {
        PrepareAuraScript(megaera_spells);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster())
            {
                if (GetTarget())
                GetTarget()->CastSpell(GetTarget(), 139889);
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(megaera_spells::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new megaera_spells();
    }
};
class spell_megaera_icy_ground_checkup : public SpellScriptLoader
{
public:
    spell_megaera_icy_ground_checkup() : SpellScriptLoader("spell_megaera_icy_ground_checkup") { }

    class megaera_spells : public AuraScript
    {
        PrepareAuraScript(megaera_spells);

        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            if (GetCaster())
            {
                if (GetTarget())
                {
                    if (GetTarget()->HasAura(SPELL_ICY_GROUND_DEBUFF) || GetTarget()->HasAura(139922))
                    {
                        if (!GetTarget()->FindNearestCreature(TRIGGER_ICY_GROUND, 1.5f))
                        {
                            GetTarget()->RemoveAura(SPELL_ICY_GROUND_DEBUFF);
                            GetTarget()->RemoveAura(139922);
                        }
                    }
                }
            }
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(megaera_spells::HandlePeriodic, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new megaera_spells();
    }
};
class spell_frost_beam_target_change : public SpellScriptLoader
{
public:
    spell_frost_beam_target_change() : SpellScriptLoader("spell_frost_beam_target_change") { }

    class megaera_spells : public SpellScript
    {
        PrepareSpellScript(megaera_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetA = 0;
            spell->Effects[0].TargetB = TARGET_UNIT_NEARBY_ENEMY;
            spell->AttributesEx6 = 0;
            return true;
        }

        void CorrectTargets(std::list<WorldObject*>& targets)
        {
            if (!GetCaster())
                return;

            targets.clear();

            std::list<Player*> pl_list;

            GetCaster()->GetPlayerListInGrid(pl_list, 300.0f);

            if (pl_list.empty())
                return;

            std::list<Player*>::const_iterator it = pl_list.begin();
            std::advance(it, urand(0, pl_list.size() - 1));

            targets.push_back((*it)->ToUnit());
        }
        void Register()
        {
            OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(megaera_spells::CorrectTargets, EFFECT_0, TARGET_UNIT_NEARBY_ENEMY);
        };
    };

    SpellScript* GetSpellScript() const
    {
        return new megaera_spells();
    }
};
// cba creating new files
#include "ScriptPCH.h"

class portable_checkpointer_raids : public ItemScript
{
public:
    portable_checkpointer_raids() : ItemScript("portable_checkpointer_raids") { }

    bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Teleport my group to me", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 106);

        if (player->isInCombat())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Check pointing while combat is forbidden. ");
            return false;
        }
        if (player->isDead())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You cannot use the Check pointer while being dead");
            return false;
        }
        if (player->InBattleground() || player->InArena())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Check pointer cannot be used while in Battlegrounds or Arena");
            return false;
        }


        player->PlayerTalkClass->ClearMenus();

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Throne of Thunder", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Terrace of Endless Springs", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Siege of Orgrimmar", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Chi Ji", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);
    
        player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
        return true;
    }

    void OnGossipSelect(Player* player, Item* item, uint32 sender, uint32 action) override
    {
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {
            // Chi Ji
        case GOSSIP_ACTION_INFO_DEF + 103:
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(870, -651.344f, -5061.347f, -6.277f, 0.921034f);
            break;
        case  GOSSIP_ACTION_INFO_DEF + 102:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Siege of Orgrimmar: Entrance I", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Immersus", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 30);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Fallen Protector", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 31);
            player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
            break;
        case  GOSSIP_ACTION_INFO_DEF + 29:
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(870, 1231.856f, 612.376f, 322.921f ,5.794870f);
            break;
        case  GOSSIP_ACTION_INFO_DEF + 30: // Immersus
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1136, 1369.969f, 783.367f, 246.835f, 5.976441f);
            break;
        case  GOSSIP_ACTION_INFO_DEF + 31: // Fallen Protectors
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1136, 1263.330f, 1038.54f, 428.323f, 3.254037f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 100:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Throne of Thunder: Entrance", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1000);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Jin'rokh", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Horridon", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Council", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Tortos", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Megaera", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 1000: // Throne of Thunder: Entrance
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1064, 7249.856f, 5026.959f, 76.510f, 5.783755f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 1: // jinrokh
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, 5838.217f, 6269.57f, 124.035f, 6.205191f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 2: // horridon
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, 5430.268f, 5825.70f, 129.602f, 4.835043f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3: // council
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, 6055.891f, 5430.49f, 136.089f, 4.473192f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 4: // Tortos
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, 6044.57f, 5002.73f, -61.188f, 4.576826f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 5: // Megaera
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, 6322.45f, 4624.43f, -199.271f, 5.581851f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 101:
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface\\ICONS\\achievement_general:24:24:-16:0|t|r Lei Shi", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
            player->PlayerTalkClass->SendGossipMenu(1, item->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 6: // Lei Shi
            player->CLOSE_GOSSIP_MENU();

            player->TeleportTo(1098, -1016.601f, -2953.42f, 18.443f, 1.716834f);
            break;
        case GOSSIP_ACTION_INFO_DEF + 106: // Group Summon
        {
            if (!player->GetGroup())
                ChatHandler(player->GetSession()).PSendSysMessage("You must be in a party to use it");
            return;

            if (player->GetGroup()->GetLeaderGUID() != player->GetGUID())
                ChatHandler(player->GetSession()).PSendSysMessage("You must be the leader of the party to use it");
            return;

            if (player->IsMounted())
                ChatHandler(player->GetSession()).PSendSysMessage("You cannot be mounted while using it");
            return;

            if (player->isInCombat())
            {
                ChatHandler(player->GetSession()).PSendSysMessage("Check pointing while combat is forbidden. ");
                return;
            }

            Group* group = NULL;

            group = player->GetGroup();

            for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* i_player = itr->getSource();

                if (!i_player || i_player == i_player || !i_player->GetSession())
                    continue;

                // stop flight if need
                if (i_player->isInFlight())
                {
                    i_player->GetMotionMaster()->MovementExpired();
                    i_player->CleanupAfterTaxiFlight();
                }
                // save only in non-flight case
                else
                    i_player->SaveRecallPosition();

                // before GM
                float x, y, z;
                i_player->GetSession()->GetPlayer()->GetClosePoint(x, y, z, i_player->GetObjectSize());
                i_player->TeleportTo(i_player->GetSession()->GetPlayer()->GetMapId(), x, y, z, i_player->GetOrientation());
                break;
            }
        }
        }
    }
};


// CBA creating new files in cmake
class player_on_login : public PlayerScript
{
public:
    player_on_login() : PlayerScript("player_on_login") {}

    void OnLogin(Player* player) override
    {
        // learn Death Knights the missing Runeforge and Death Gate spell 
        if (player->getClass() == CLASS_DEATH_KNIGHT)
        {
            player->learnSpell(53428, false);
            player->learnSpell(50977, false);
        }
    }
};


void AddSC_megaera()
{
    new player_on_login();
    new portable_checkpointer_raids();

    new boss_megaera();
    new creature_head_megaera_venom();
    new creature_head_megaera_fire();
    new creature_head_megaera_frost();
    new creature_trigger_icy_ground();
    new creature_megaera_acid_nova();
    new trigger_megaera_fire_pool();
    new spell_megaera_fire_cinders_pool_spawning_mechanism();
    new spell_megaera_arctic_freeze_stun();
    new spell_megaera_icy_ground_checkup();
    new spell_megaera_torrent_of_ice_tick();
    new spell_frost_beam_target_change();
}
