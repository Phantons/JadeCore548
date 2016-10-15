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
#include "Vehicle.h"

/*######
## npc_aradne
######*/

class npc_aradne : public CreatureScript
{
public:
    npc_aradne() : CreatureScript("npc_aradne") { }

    enum eAradne
    {
        RAISE_UNDEAD = 93446,
    };

    struct npc_aradneAI : public ScriptedAI
    {
        npc_aradneAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;

        void Reset()
        {
            m_timer = 0;
            m_phase = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type == WAYPOINT_MOTION_TYPE)
            switch (id)
            {
                case 4:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 12:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 15:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 20:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 26:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 31:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 33:
                    m_phase = 1;
                    m_timer = 100;
                    break;
                case 37:
                    m_phase = 1;
                    m_timer = 100;
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    if (Creature* npc = me->FindNearestCreature(50373, 15.0f))
                        me->CastSpell(npc, 93446, false);

                    m_phase = 2;
                    break;
                case 2:
                    Talk(0);
                    m_phase = 3;
                    break;
                default:
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_aradneAI(pCreature);
    }
};

/*######
## npc_deathknell_grave_target
######*/

class npc_deathknell_grave_target : public CreatureScript
{
public:
    npc_deathknell_grave_target() : CreatureScript("npc_deathknell_grave_target") { }

    enum eRisenDead
    {
        SPELL_SUMMON_RISEN_DEAD  = 93447,
        SPELL_DIRT_EXPLOSION     = 89199,
        SPELL_CHURNING_DIRT      = 92788
    };

    struct npc_deathknell_grave_targetAI : public ScriptedAI
    {
        npc_deathknell_grave_targetAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;

        void Reset()
        {
            m_timer = 0;
            m_phase = 0;
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (spell->Id == 93446)
            {
                m_timer = 100;
                m_phase = 1;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    me->AddAura(SPELL_DIRT_EXPLOSION, me);
                    DoCast(me, SPELL_CHURNING_DIRT);
                    m_phase = 2;
                    break;
                case 2:
                    me->CastSpell(me, SPELL_SUMMON_RISEN_DEAD, true);
                    m_phase = 3;
                    m_timer = 3000;
                    break;
                case 3:
                    me->RemoveAllAuras();
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_deathknell_grave_targetAI(pCreature);
    }
};

/*######
## npc_risen_dead
######*/

class npc_risen_dead : public CreatureScript
{
public:
    npc_risen_dead() : CreatureScript("npc_risen_dead") { }

    struct npc_risen_deadAI : public ScriptedAI
    {
        npc_risen_deadAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        uint32 m_WishToBeDead;

        void Reset()
        {
            m_timer = 2000;
            m_phase = 0;
            m_WishToBeDead = urand(0, 100);
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            if (m_WishToBeDead < 33)
                ShowDead();
            else if (m_WishToBeDead < 66)
                ShowLife();
            else
                ShowRunning();
        }

        void ShowDead()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmote(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    Talk(7);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(2);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(1);
                    me->Kill(me);
                    m_timer = 5000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }

        void ShowLife()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmote(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(5);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(4);
                    SetEquipmentSlots(false, 10614, 1984);
                    me->GetMotionMaster()->MovePath(5037401, false);
                    m_timer = 20000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }

        void ShowRunning()
        {
            switch (m_phase)
            {
                case 0:
                    me->HandleEmote(EMOTE_ONESHOT_QUESTION);
                    m_timer = 200;
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 5000;
                    m_phase = 2;
                    break;
                case 2:
                    Talk(3);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    Talk(6);
                    me->CastSpell(me, 44427, true);
                    Talk(8);
                    me->GetMotionMaster()->MovePath(5037400, false);
                    m_timer = 5000;
                    m_phase = 4;
                    break;
                case 4:
                    m_timer = 0;
                    m_phase = 5;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_risen_deadAI(pCreature);
    }
};

/*######
## npc_undertaker_mordo
######*/

class npc_undertaker_mordo : public CreatureScript
{
public:
    npc_undertaker_mordo() : CreatureScript("npc_undertaker_mordo") { }

    struct npc_undertaker_mordoAI : public ScriptedAI
    {
        npc_undertaker_mordoAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        Creature* m_npc;

        void Reset()
        {
            m_timer = urand(30000, 45000);
            m_phase = 0;
            m_npc = NULL;
        }

        void UpdateAI(const uint32 diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 0:
                    if (Creature* npc_trigger = me->FindNearestCreature(50373, 20.0f))
                        if (m_npc = me->SummonCreature(50414, npc_trigger->GetPositionX(), npc_trigger->GetPositionY(), npc_trigger->GetPositionZ(), 1.0f, TEMPSUMMON_TIMED_DESPAWN, 55000))
                            m_npc->GetMotionMaster()->MovePath(5041400, false);
                    m_phase = 1;
                    break;
                case 1:
                    Talk(0);
                    m_timer = 3000;
                    m_phase = 2;
                    break;
                case 2:
                    if (m_npc && m_npc->isAlive())
                    {
                        m_npc->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_KNEEL);
                        m_npc->SetFacingToObject(me);
                    }
                    m_timer = 750;
                    m_phase = 3;
                    break;
                case 3:
                    me->HandleEmoteState(EMOTE_STATE_USE_STANDING);
                    m_timer = 6000;
                    m_phase = 4;
                    break;
                case 4:
                    me->HandleEmoteState(EMOTE_ONESHOT_NONE);
                    m_timer = 750;
                    m_phase = 5;
                    break;
                case 5:
                    me->HandleEmote(EMOTE_ONESHOT_POINT);
                    m_timer = 2500;
                    m_phase = 6;
                    break;
                case 6:
                    Talk(1);
                    m_timer = 250;
                    m_phase = 7;
                    break;
                case 7:
                    if (m_npc && m_npc->isAlive())
                    {
                        m_npc->AddAura(93460, m_npc);
                        m_npc->CastSpell(m_npc, 93460);
                        m_npc->SetDisplayId(36775);
                        m_npc->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_STAND);
                    }
                    m_timer = 2500;
                    m_phase = 8;
                    break;
                case 8:
                    if (m_npc && m_npc->isAlive())
                    {
                        m_npc->HandleEmote(EMOTE_ONESHOT_TALK);
                        m_npc->AI()->Talk(0);
                    }
                    m_timer = 3750;
                    m_phase = 9;
                    break;
                case 9:
                    if (m_npc && m_npc->isAlive())
                        m_npc->HandleEmote(EMOTE_ONESHOT_SALUTE);
                    m_timer = 2500;
                    m_phase = 10;
                    break;
                case 10:
                    if (m_npc && m_npc->isAlive())
                        m_npc->GetMotionMaster()->MovePath(5041401, false);
                    m_timer = urand(15000, 45000);
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_undertaker_mordoAI(pCreature);
    }
};

/*######
## npc_marshal_redpath
######*/

class npc_marshal_redpath : public CreatureScript
{
public:
    npc_marshal_redpath() : CreatureScript("npc_marshal_redpath") { }

    enum eMarshal
    {
        QUEST_THE_WAKENING = 24960,
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_THE_WAKENING) == QUEST_STATUS_INCOMPLETE)
            return false;

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (player->GetQuestStatus(24960) == QUEST_STATUS_INCOMPLETE)
        {
            uint32 id = player->PlayerTalkClass->GetGossipMenu().GetMenuId();
            if (id == 12486 && player->GetReqKillOrCastCurrentCount(QUEST_THE_WAKENING, 49230) == 0)
            {
                player->CLOSE_GOSSIP_MENU();
                CAST_AI(npc_marshal_redpathAI, creature->AI())->StartAnimation(player);
            }
        }
        return false;
    }

    struct npc_marshal_redpathAI : public ScriptedAI
    {
        npc_marshal_redpathAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        Player* m_player;

        void Reset()
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
            m_timer = 0;
            m_phase = 0;
            m_player = NULL;
        }

        void StartAnimation(Player* player)
        {
            if (!m_phase)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                m_player = player;
                m_phase = 1;
                m_timer = 100;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    me->SetFacingToObject(m_player);
                    Talk(0);
                    m_timer = 4000;
                    m_phase = 2;
                    break;
                case 2:
                    if (m_player)
                        m_player->KilledMonsterCredit(49230);
                    m_timer = 4000;
                    m_phase = 3;
                    break;
                case 3:
                    me->GetMotionMaster()->MovePath(4923000, false);
                    m_timer = 10000;
                    m_phase = 4;
                    break;
                case 4:
                    me->DespawnOrUnsummon();
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_marshal_redpathAI(pCreature);
    }
};

/*######
## npc_lilian_voss
######*/

class npc_lilian_voss : public CreatureScript
{
public:
    npc_lilian_voss() : CreatureScript("npc_lilian_voss") { }

    enum eLilian
    {
        QUEST_THE_WAKENING = 24960,
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_THE_WAKENING) == QUEST_STATUS_INCOMPLETE)
            return false;

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (player->GetQuestStatus(QUEST_THE_WAKENING) == QUEST_STATUS_INCOMPLETE)
        {
            uint32 id = player->PlayerTalkClass->GetGossipMenu().GetMenuId();
            if (id == 17565 && player->GetReqKillOrCastCurrentCount(QUEST_THE_WAKENING, 38895) == 0)
            {
                player->CLOSE_GOSSIP_MENU();
                CAST_AI(npc_lilian_vossAI, creature->AI())->StartAnimation(player);
            }
        }

        return false;
    }

    struct npc_lilian_vossAI : public ScriptedAI
    {
        npc_lilian_vossAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        Player* m_player;

        void Reset()
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
            m_timer = 0;
            m_phase = 0;
            m_player = NULL;
        }

        void StartAnimation(Player* player)
        {
            if (!m_phase)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                me->HandleEmoteState(EMOTE_STATE_NONE);
                m_player = player;
                m_phase = 1;
                m_timer = 100;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    me->SetFacingToObject(m_player);
                    Talk(0);
                    m_timer = 4000;
                    m_phase = 2;
                    break;
                case 2:
                    if (m_player)
                        m_player->KilledMonsterCredit(38895);
                    m_timer = 5000;
                    m_phase = 3;
                    break;
                case 3:
                    me->GetMotionMaster()->MovePath(3889500, false);
                    m_timer = 10000;
                    m_phase = 4;
                    break;
                case 4:
                    me->DespawnOrUnsummon();
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lilian_vossAI(pCreature);
    }
};

/*######
## npc_valdred_moray
######*/

class npc_valdred_moray : public CreatureScript
{
public:
    npc_valdred_moray() : CreatureScript("npc_valdred_moray") { }

    enum eValdred
    {
        QUEST_THE_WAKENING = 24960,
    };

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_THE_WAKENING) == QUEST_STATUS_INCOMPLETE)
            return false;

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        if (player->GetQuestStatus(QUEST_THE_WAKENING) == QUEST_STATUS_INCOMPLETE)
        {
            uint32 id = player->PlayerTalkClass->GetGossipMenu().GetMenuId();
            if (id == 12489 && player->GetReqKillOrCastCurrentCount(QUEST_THE_WAKENING, 49231) == 0)
            {
                player->CLOSE_GOSSIP_MENU();
                CAST_AI(npc_valdred_morayAI, creature->AI())->StartAnimation(player);
            }
        }

        return false;
    }

    struct npc_valdred_morayAI : public ScriptedAI
    {
        npc_valdred_morayAI(Creature *c) : ScriptedAI(c) { }

        uint32 m_timer;
        uint32 m_phase;
        Player* m_player;

        void Reset()
        {
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
            m_timer = 0;
            m_phase = 0;
            m_player = NULL;
        }

        void StartAnimation(Player* player)
        {
            if (!m_phase)
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_QUESTGIVER);
                me->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_STAND);
                m_player = player;
                m_phase = 1;
                m_timer = 100;
            }
        }

        void UpdateAI(uint32 const diff)
        {
            if (m_timer <= diff)
            {
                m_timer = 1000;
                DoWork();
            }
            else m_timer -= diff;
        }

        void DoWork()
        {
            switch (m_phase)
            {
                case 1:
                    me->SetFacingToObject(m_player);
                    Talk(0);
                    m_timer = 4000;
                    m_phase = 2;
                    break;
                case 2:
                    if (m_player)
                        m_player->KilledMonsterCredit(49231);
                    m_timer = 4000;
                    m_phase = 3;
                    break;
                case 3:
                    me->GetMotionMaster()->MovePath(4923100, false);
                    m_timer = 6000;
                    m_phase = 4;
                    break;
                case 4:
                    me->DespawnOrUnsummon();
                    m_timer = 0;
                    m_phase = 0;
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_valdred_morayAI(pCreature);
    }
};

/*######
## npc_mindless_zombie
######*/

class npc_mindless_zombie : public CreatureScript
{
public:
    npc_mindless_zombie() : CreatureScript("npc_mindless_zombie") { }

    struct npc_mindless_zombieAI : public ScriptedAI
    {
        npc_mindless_zombieAI(Creature *c) : ScriptedAI(c) { }

        uint32 timer;
        uint32 step;

        void Reset()
        {
            me->RemoveAllAuras();
            timer = urand(20000, 45000);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!me->isInCombat())
            {
                if (timer <= diff)
                {
                    timer = urand(18000, 43000);
                    Crazed();
                }
                else timer -= diff;
            }
            DoMeleeAttackIfReady();
        }

        void Crazed()
        {
            if (Player *player = me->FindNearestPlayer(15.0f, true))
            {
                me->SetFacingToObject(player);
                me->CastSpell(me, 44427, true);
                me->SetWalk(false);
                me->GetMotionMaster()->MovePoint(0, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_mindless_zombieAI(pCreature);
    }
};

void AddSC_tirisfal_glades()
{
    new npc_aradne();
    new npc_deathknell_grave_target();
    new npc_risen_dead();
    new npc_undertaker_mordo();
    new npc_marshal_redpath();
    new npc_lilian_voss();
    new npc_valdred_moray();
    new npc_mindless_zombie();
}
