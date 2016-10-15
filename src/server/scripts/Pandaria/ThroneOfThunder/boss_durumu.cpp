#include "GameObjectAI.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Spell.h"
#include "Vehicle.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "CreatureTextMgr.h"
#include "Unit.h"
#include "Player.h"
#include "Creature.h"
#include "InstanceScript.h"
#include "Map.h"
#include "VehicleDefines.h"
#include "SpellInfo.h"

#include "throne_of_thunder.h"

/* DURUMU TODO:
1. Maze phase aoe place must be spawned areatriggers 
*/

enum Actions 
{
    // Durumu
    ACTION_ROAMING_DIE = 1,
    ACTION_FOG_KILLED,
    ACTION_SAY_FORCE_OF_WILL,
    ACTION_SAY_SPELL_2,
    ACTION_SAY_SPELL_3,
    ACTION_SAY_SPELL_4,
    ACTION_MAZE_START,
    ACTION_MAZE_END,
    ACTION_SUMMON_BLUE_FOG,

    // Fogs
    ACTION_FOG_ACTIVATE,

    // Eyes
    ACTION_EYE_DRAIN_LIFE,
};

enum GoBjects
{
    GOBJECT_DOODAD_ENTER_TO_DURUMU = 218396,
    GOBJECT_DOODAD_EXIT_FROM_DURUMU = 218395,
};

Position CenterDurumu = { 5895.829f,  4512.626f, -6.276f, 6.242f };

Position YellowEyePositions[8] =
{
    { 5833.541f,  4523.083f, -6.277f, 1.873f },
    { 5859.474f,  4563.136f, -6.277f, 1.873f },
    { 5905.615f,  4573.638f, -6.277f, 1.873f },
    { 5945.436f,  4548.107f, -6.277f, 1.873f },
    { 5955.923f,  4502.487f, -6.277f, 1.873f },
    { 5930.874f,  4463.088f, -6.277f, 1.873f },
    { 5886.584f,  4452.482f, -6.277f, 1.873f },
    { 5845.496f,  4476.399f, -6.277f, 1.873f }
};

enum Spells
{
    SPELL_HARD_STARE = 133765,
    SPELL_GAZE = 134029,
    SPELL_FORCE_OF_WILL = 136932,
    SPELL_FORCE_OF_WILL_KNOCKBACK = 136413,
    SPELL_LINGERING_GAZE_CAST = 138467,
    SPELL_LINGERING_GAZE_MARKER = 134626,
    SPELL_LINGERING_GAZE_DMG = 133792,
    SPELL_BERSERK_KILLALL = 137747,
    SPELL_DRAIN_LIFE_STUN = 137727,
    SPELL_DRAIN_LIFE_CHANNEL = 133795,
    SPELL_DRAIN_LIFE_DAMAGE = 133798,
    SPELL_DRAIN_LIFE_HEAL = 133807,
    SPELL_DESINTEGRATION_BEAM = 133776,
    SPELL_MIND_DAGGERS = 139107,

    // Beams
    SPELL_YELLOW_BEAM = 133740,
    SPELL_RED_BEAM = 133734,
    SPELL_BLUE_BEAM = 133672,
    SPELL_YELLOW_BEAM_DMG = 133738,
    SPELL_RED_BEAM_DMG = 133732,
    SPELL_BLUE_BEAM_DMG = 133677,
    SPELL_YELLOW_BEAM_DMG_TO_ALL = 133739,
    SPELL_RED_BEAM_DMG_TO_ALL = 133733,
    SPELL_BLUE_BEAM_DMG_TO_ALL = 133678,

    SPELL_FOG_INVISIBLE = 137092,

    SPELL_CAUSTIC_SPIKE = 136154,
    SPELL_ICY_GRASP = 136177,
    SPELL_CRIMSON_BLOOM = 136122,
    SPELL_FLASH_FREEZE = 136124,
};

enum NpcEntry
{
    // Beam phase eyes
    YELLOW_EYE = 67856, // for yellow beam
    BLUE_EYE   = 67854, // for blue beam
    RED_EYE    = 67855, // for red beam
    YELLOW_EYE_MOVER = 67829, // yellow eye not work with player, just with invisible

    // Durumu eyes
    MIND_EYE   = 67875,
    HUNGRY_EYE = 67859,
    APPRAYISYING_EYE = 67858,

    // fogs
    RED_FOG    = 69050,
    BLUE_FOG   = 69052,
};

enum Events
{
    // Durumu
    EVENT_HARD_STARE = 1,
    EVENT_GAZE,
    EVENT_BERSERK,
    EVENT_LIGHT_SPECTRUM_SAY_SUMMON,
    EVENT_LIGHT_SPECTRUM,
    EVENT_MAZE_TIME,
    EVENT_MAZE_ORIENTATION,
    EVENT_MAZE_END,
    EVENT_MIND_DAGGERS,

    // Eyes
    EVENT_FORCE_OF_WILL,
    EVENT_LINGERING_GAZE,
    EVENT_DRAIN_LIFE,
    EVENT_DRAIN_LIFE_TAR_CHECK,
    EVENT_EYES_ORIENTATION,

    // For roaming fogs and eyes
    EVENT_MOVE_RANDOM,

    // fogs
    EVENT_RED_FOG_WIND,
    EVENT_FOG_CAST,

    // Eye mover
    EVENT_INITIALIZE_TYPE_MOVE,
};

enum Yells
{
    SAY_INTRO,
    SAY_AGGRO,
    SAY_DEATH,
    SAY_BERSERK,
    SAY_KILL1,
    SAY_KILL2,
    SAY_FORCE_OF_WILL,
    SAY_SUMMON_SPECTRUM,
    SAY_LINGERING_GAZE,
    SAY_SPELL_4, // idk for what spell this used
    SAY_SPECTRUM_INF,
    SAY_TWO_FOG_REMAIN,
    SAY_ONE_FOG_REMAIN,
};

uint64 SelecGUIDtRandomPlayerInRage(Creature*me,float range,bool lingering_gaze = false)
{
    std::list<Player*> PlayerList;
    GetPlayerListInGrid(PlayerList, me, range);
    if (lingering_gaze)
        PlayerList.remove_if(JadeCore::UnitAuraCheck(true, SPELL_LINGERING_GAZE_MARKER));
    if (!PlayerList.empty())
    if (Player *target = JadeCore::Containers::SelectRandomContainerElement(PlayerList))
            return target->GetGUID();

    return 0;
}

uint64 FindDurumuGUID(Creature* me)
{
    std::list<Creature*> durumuList;
    GetCreatureListWithEntryInGrid(durumuList, me, 68036, 200.0f);
    for (auto Durumu : durumuList)
        return Durumu->GetGUID();
    return 0;
}

// Roaming Fog - 68313
class npc_roaming_fog : public CreatureScript
{
    public:
        npc_roaming_fog() : CreatureScript("npc_roaming_fog") { }

        struct npc_roaming_fogAI : public ScriptedAI
        {
            npc_roaming_fogAI(Creature* creature) : ScriptedAI(creature) 
            {
                Reset();
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_MOVE_RANDOM, urand(3000,8000));
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_MOVE_RANDOM:
                        if (!me->isInCombat())
                        {
                            Position position;
                            me->GetRandomPoint(CenterDurumu,40.0f,position);
                            me->GetMotionMaster()->MovePoint(0, position);
                        }
                        events.ScheduleEvent(EVENT_MOVE_RANDOM, urand(5000,8000));
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* killer)
            {
                if (Creature* durumu = ObjectAccessor::GetCreature(*me, FindDurumuGUID(me)))
                    durumu->GetAI()->DoAction(ACTION_ROAMING_DIE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_roaming_fogAI(creature);
        }
};

// durumu 68036
class boss_durumu : public CreatureScript
{
    public:
        boss_durumu() : CreatureScript("boss_durumu") { }

        struct boss_durumuAI : public BossAI
        {
            boss_durumuAI(Creature* creature) : BossAI(creature, DATA_DURUMU_THE_FORGOTTEN)
            {
                instance = creature->GetInstanceScript();
                RoamingKilled = 0;
                init();
            }

            InstanceScript* instance;
            EventMap events;
            uint8 RoamingKilled; // used for summon durumu
            uint8 FogKilled; // used for summoned fogs in fighting

            void init()
            {
                SetFly(true);
                me->SetHomePosition(CenterDurumu);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_DEFENSIVE);
                me->SetVisible(false);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void SetPlatform(uint32 entry, bool ready)
            {
                std::list<GameObject*> gobjects;
                GetGameObjectListWithEntryInGrid(gobjects, me, entry, 80.0f);

                for (auto gobject : gobjects)
                    gobject->SetGoState(ready ? GO_STATE_READY : GO_STATE_ACTIVE);
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (instance)
                    instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, FAIL);
            }

            void Reset()
            {
                if (instance)
                {
                   instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RED_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RED_BEAM_DMG);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLUE_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLUE_BEAM_DMG);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_YELLOW_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_YELLOW_BEAM_DMG);
                }
                
                SetPlatform(GOBJECT_DOODAD_ENTER_TO_DURUMU, true);

                events.Reset();
                _Reset();
            }

            void SetFly(bool IsFlying)
            {
                me->SetDisableGravity(IsFlying);
                me->SetCanFly(IsFlying);
            }

            void JustDied(Unit* killer)
            {
                me->AI()->Talk(SAY_DEATH);

                SetPlatform(GOBJECT_DOODAD_EXIT_FROM_DURUMU, true);
                SetPlatform(GOBJECT_DOODAD_ENTER_TO_DURUMU, true);

                if (instance)
                {
                    instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, DONE);
                   instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove.
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RED_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_RED_BEAM_DMG);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLUE_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLUE_BEAM_DMG);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_YELLOW_BEAM);
                   instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_YELLOW_BEAM_DMG);
                }

                _JustDied();
            }

            void KilledUnit(Unit* who)
            {
                me->AI()->Talk(urand(0,1) == 0 ? SAY_KILL1 : SAY_KILL2);
            }

            void EnterCombat(Unit* attacker)
            {
                SetPlatform(GOBJECT_DOODAD_ENTER_TO_DURUMU, false);
                me->AI()->Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_HARD_STARE, 5000);
                events.ScheduleEvent(EVENT_GAZE, 2000);
                events.ScheduleEvent(EVENT_BERSERK, 600000);
                events.ScheduleEvent(EVENT_LIGHT_SPECTRUM_SAY_SUMMON, 40000);
                SummonEyes();

                if (instance)
                {
                    instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, IN_PROGRESS);
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }
            }

            void SendMazeToEyes(bool start)
            {
                std::list<Creature*> eyesList;
                GetCreatureListWithEntryInGrid(eyesList, me, MIND_EYE, 200.0f);
                GetCreatureListWithEntryInGrid(eyesList, me, HUNGRY_EYE, 200.0f);
                GetCreatureListWithEntryInGrid(eyesList, me, APPRAYISYING_EYE, 200.0f);
                for (auto eye : eyesList)
                    eye->AI()->DoAction(start == true ? ACTION_MAZE_START : ACTION_MAZE_END);
            }

            void SummonBeamsEye()
            {
                if (Creature * eyeDontMover = me->SummonCreature(YELLOW_EYE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ())) // Yellow eye
                    if (Creature * tarMover = me->SummonCreature(YELLOW_EYE_MOVER, YellowEyePositions[0])) // Eyebeam target(mover)
                        eyeDontMover->CastSpell(tarMover, SPELL_YELLOW_BEAM, true);
                
                // We must use one list for two beams coz not get problem with one player for two beams
                std::list<Player*> PlayerList;
                GetPlayerListInGrid(PlayerList, me, 100.0f);

                if (!PlayerList.empty())
                    if (Creature * eyeDontMover = me->SummonCreature(RED_EYE,me->GetPositionX(), me->GetPositionY(),me->GetPositionZ()))
                        if (Player *target = JadeCore::Containers::SelectRandomContainerElement(PlayerList))
                        {
                            eyeDontMover->CastSpell(target, SPELL_RED_BEAM, true);
                            PlayerList.remove(target);
                        }
                        
                if (!PlayerList.empty()) // Check again coz we change player list before
                    if (Creature * eyeDontMover = me->SummonCreature(BLUE_EYE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                        if (Player *target = JadeCore::Containers::SelectRandomContainerElement(PlayerList))
                            eyeDontMover->CastSpell(target, SPELL_BLUE_BEAM, true);
            }

            void SummonEyes()
            {
                me->SummonCreature(MIND_EYE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                me->SummonCreature(HUNGRY_EYE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
                me->SummonCreature(APPRAYISYING_EYE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ());
            }

            void SummonSomeCreatures(uint32 entry, uint8 num, float minDist)
            {
                for (uint8 i=1;i<=num;++i)
                {
                    Position position = CenterDurumu;
                    while (me->GetDistance(position) <= minDist)
                        me->GetRandomPoint(CenterDurumu, 60.0f, position);
                    me->SummonCreature(entry, position);
                }
            }

            void DespawnCreature(uint32 entry)
            {
                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, entry, 80.0f);
                for (auto NowCreature : creatureList)
                {
                    if (NowCreature->GetEntry() == BLUE_FOG && NowCreature->GetDisplayId() == NowCreature->GetNativeDisplayId()) // if blue fog was been activated not despawn him
                        continue;
                    NowCreature->DespawnOrUnsummon();
                }
            }

            void CheckPlayersInWater()
            {
                std::list<Player*> PlayerList;
                GetPlayerListInGrid(PlayerList, me, 100.0f);
                for (auto player : PlayerList)
                    if (player->IsInWater())
                        player->Kill(player);
            }

            void UpdateAI(const uint32 diff)
            { 
                // Falled to water in out of platform = die
                CheckPlayersInWater();

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_HARD_STARE:
                        if (me->getVictim())
                        {
                            DoPlaySoundToSet(me, 35334); // wound
                            DoCast(me->getVictim(),SPELL_HARD_STARE);
                        }
                        events.ScheduleEvent(EVENT_HARD_STARE, 13000);
                        break;
                    case EVENT_GAZE:
                        {
                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 2.0f, true);
                            if (!target)
                                if (!me->HasUnitState(UNIT_STATE_CASTING))
                                    DoCast(SPELL_GAZE);
                            events.ScheduleEvent(EVENT_GAZE, 2000);
                            break;
                        }
                    case EVENT_BERSERK:
                        events.CancelEventGroup(0); // clear all events
                        me->AI()->Talk(SAY_BERSERK);
                        DoCast(SPELL_BERSERK_KILLALL);
                        break;
                    // Spectrum phase start
                    case EVENT_LIGHT_SPECTRUM_SAY_SUMMON:
                        FogKilled = 0;
                        me->AI()->Talk(SAY_SUMMON_SPECTRUM);
                        SummonSomeCreatures(RED_FOG, 3, 10.0f); // 3 red fog
                        SummonSomeCreatures(BLUE_FOG, 1, 10.0f); // 1 blue fog
                        events.ScheduleEvent(EVENT_LIGHT_SPECTRUM, 3000);
                        break;
                    case EVENT_LIGHT_SPECTRUM:
                        me->AI()->Talk(SAY_SPECTRUM_INF);
                        SummonBeamsEye();
                        break;
                    // Spectrum phase end

                    // Disintegration Beam phase start
                    case EVENT_MAZE_TIME:
                        Maze(true);
                        events.ScheduleEvent(EVENT_MAZE_ORIENTATION, 5000);
                        events.ScheduleEvent(EVENT_MAZE_END, 47000);
                        events.ScheduleEvent(EVENT_MIND_DAGGERS, 6000);
                        SendMazeToEyes(true);
                        break;
                    case EVENT_MAZE_ORIENTATION:
                        UpdateOrientation();
                        TryKillPlayerInFront();
                        events.ScheduleEvent(EVENT_MAZE_ORIENTATION, 200);
                        break;
                    case EVENT_MAZE_END:
                        Maze(false);
                        SendMazeToEyes(false);
                        events.ScheduleEvent(EVENT_LIGHT_SPECTRUM_SAY_SUMMON, 40000);
                        break;
                    case EVENT_MIND_DAGGERS:
                        DoCast(SPELL_MIND_DAGGERS);
                        events.ScheduleEvent(EVENT_MIND_DAGGERS, 6000);
                        break;
                    // Disintegration Beam phase end
                    default:
                        break;
                }

                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    DoMeleeAttackIfReady();
            }

            void Maze(bool start)
            {
                me->SetReactState(start ? REACT_PASSIVE : REACT_AGGRESSIVE);
                start ? events.CancelEvent(EVENT_HARD_STARE) : events.ScheduleEvent(EVENT_HARD_STARE, 2000);
                start ? events.CancelEvent(EVENT_GAZE) : events.ScheduleEvent(EVENT_GAZE, 2000);

                if (!start)
                {
                    DespawnCreature(YELLOW_EYE_MOVER);

                    events.CancelEvent(EVENT_MIND_DAGGERS);
                    events.CancelEvent(EVENT_MAZE_ORIENTATION);

                    me->RemoveAurasDueToSpell(SPELL_DESINTEGRATION_BEAM);
                }
                else
                {
                    me->SetOrientation(6.272f);
                    if (Creature * tarMover = me->SummonCreature(YELLOW_EYE_MOVER,YellowEyePositions[0]))
                        me->CastSpell(tarMover,SPELL_DESINTEGRATION_BEAM,true);
                }
            }

            void UpdateOrientation()
            {
                std::list<Creature*> beamTarList;
                uint64 tarGuid = 0;
                GetCreatureListWithEntryInGrid(beamTarList, me, YELLOW_EYE_MOVER, 200.0f);
                for (auto Beam : beamTarList)
                    tarGuid = Beam->GetGUID();

                if (Creature* beamTar = ObjectAccessor::GetCreature(*me, tarGuid))
                    me->SetFacingToObject(beamTar);
            }

            void TryKillPlayerInFront()
            {
                // In maze time phase all players in front of durumu beam instant killed
                std::list<Player*> PlayerList;
                GetPlayerListInGrid(PlayerList, me, 80.0f);
                for (auto player : PlayerList)
                    if (me->isInFront(player, M_PI/6))
                        me->Kill(player);
            }

            void EndBeamPhase()
            {
                // RemoveBeamAuras
                std::list<Player*> PlayerList;
                GetPlayerListInGrid(PlayerList, me, 200.0f);

                for (auto itr : PlayerList)
                {
                    itr->RemoveAurasDueToSpell(SPELL_BLUE_BEAM);
                    itr->RemoveAurasDueToSpell(SPELL_RED_BEAM);
                }

                // Despawn creatures from phase
                DespawnCreature(RED_EYE);
                DespawnCreature(BLUE_EYE);
                DespawnCreature(YELLOW_EYE);
                DespawnCreature(YELLOW_EYE_MOVER);
                DespawnCreature(BLUE_FOG);
                
                std::list<Creature*> hungryList;
                GetCreatureListWithEntryInGrid(hungryList, me, HUNGRY_EYE, 80.0f); 
                for (auto eye : hungryList)
                    eye->GetAI()->DoAction(ACTION_EYE_DRAIN_LIFE); // find hungry eye to start drain life

                events.ScheduleEvent(EVENT_MAZE_TIME, 75000);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SUMMON_BLUE_FOG:
                        SummonSomeCreatures(BLUE_FOG, 1, 10.0f); // 1 blue fog
                        break;
                    case ACTION_ROAMING_DIE: // When players kill roaming
                        ++RoamingKilled;
                        if (RoamingKilled == 3)
                        {
                            me->AI()->Talk(SAY_INTRO);
                            me->SetVisible(true);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        }
                        break;
                    case ACTION_FOG_KILLED: // red fog die
                        ++FogKilled;
                        if (FogKilled == 3)
                            EndBeamPhase();
                        else
                            me->AI()->Talk(FogKilled == 1 ? SAY_TWO_FOG_REMAIN : SAY_ONE_FOG_REMAIN);
                        break;
                    case ACTION_SAY_FORCE_OF_WILL: // force of will
                        me->AI()->Talk(SAY_FORCE_OF_WILL);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_durumuAI(creature);
        }
};

// durumu fogs 69052, 69050
class npc_durumu_fog : public CreatureScript
{
    public:
        npc_durumu_fog() : CreatureScript("npc_durumu_fog") { }

        struct npc_durumu_fogAI : public ScriptedAI
        {
            npc_durumu_fogAI(Creature* creature) : ScriptedAI(creature) 
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->SetDisplayId(17286);
                activate = false;
            }
            
            EventMap events;
            bool activate;

            void IsSummonedBy(Unit* summoner)
            {
                if (me->GetEntry() == RED_FOG)
                    events.ScheduleEvent(EVENT_RED_FOG_WIND, 2000);
                events.ScheduleEvent(EVENT_FOG_CAST, 3000);
            }

            void UpdateAI(const uint32 diff)
            { 
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_RED_FOG_WIND:
                        if (!activate)
                        {
                            DoCast(SPELL_FOG_INVISIBLE);
                            events.ScheduleEvent(EVENT_RED_FOG_WIND, 8000);
                        }
                        break;
                    case EVENT_FOG_CAST:
                        if (activate)
                            if (IsMeInFrontOfBeam())
                                DoCast(me->GetEntry() != BLUE_FOG ? SPELL_CAUSTIC_SPIKE : SPELL_ICY_GRASP);
                            else
                                DoCast(me->GetEntry() != BLUE_FOG ? SPELL_CRIMSON_BLOOM : SPELL_FLASH_FREEZE);
                        events.ScheduleEvent(EVENT_FOG_CAST, 3000);
                        break;
                }
            }

            bool IsMeInFrontOfBeam()
            {
                std::list<Creature*> beam;
                GetCreatureListWithEntryInGrid(beam, me, me->GetEntry() == BLUE_FOG ? BLUE_EYE : RED_EYE, 200.0f);
                for (auto beamEye : beam)
                    if (beamEye->isInFront(me, M_PI/7))
                        return true;

                return false;
            }

            void JustDied(Unit* killer)
            {
                if (Creature* durumu = ObjectAccessor::GetCreature(*me, FindDurumuGUID(me)))
                {
                    if (me->GetEntry() == BLUE_FOG) // blue fog when die cast aoe and summon blue fog in another place
                    {
                        DoCast(SPELL_FLASH_FREEZE);
                        durumu->GetAI()->DoAction(ACTION_SUMMON_BLUE_FOG);
                    }

                    if (me->GetEntry() == RED_FOG)
                        durumu->GetAI()->DoAction(ACTION_FOG_KILLED); // need 3 red fog killed for end phase
                }
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_FOG_ACTIVATE:
                        me->RemoveAurasDueToSpell(SPELL_FOG_INVISIBLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        me->SetDisplayId(me->GetNativeDisplayId());
                        activate = true;
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_durumu_fogAI(creature);
        }
};

// durumu eyebeam mover 67829
class npc_eyebeam_target : public CreatureScript
{
    public:
        npc_eyebeam_target() : CreatureScript("npc_eyebeam_target") { }

        struct npc_eyebeam_targetAI : public ScriptedAI
        {
            npc_eyebeam_targetAI(Creature* creature) : ScriptedAI(creature) 
            {
            }
            
            bool moveRight;
            int8 pointNum;

            void IsSummonedBy(Unit* summoner)
            {
                me->SetDisplayId(11686);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetDisableGravity(true);
                me->SetCanFly(true);

                moveRight = urand(0,1) == 0 ? true : false;
                events.ScheduleEvent(EVENT_INITIALIZE_TYPE_MOVE, 1000);
            }

            void UpdateAI(const uint32 diff)
            { 
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_INITIALIZE_TYPE_MOVE:
                        if (me->HasAura(SPELL_DESINTEGRATION_BEAM))
                            events.ScheduleEvent(EVENT_MOVE_RANDOM, 4000);
                        else
                        {
                            me->SetSpeed(MOVE_FLIGHT, 0.4f);
                            events.ScheduleEvent(EVENT_MOVE_RANDOM, 1000);
                        }
                        break;
                    case EVENT_MOVE_RANDOM:
                        pointNum = moveRight == true ? pointNum + 1 : pointNum - 1;
                        if (pointNum > 7)
                            pointNum = 0;
                        if (pointNum < 0)
                            pointNum = 7;

                        me->GetMotionMaster()->MovePoint(0,YellowEyePositions[pointNum]);

                        if (me->HasAura(SPELL_DESINTEGRATION_BEAM))
                            events.ScheduleEvent(EVENT_MOVE_RANDOM, 5000);
                        else
                            events.ScheduleEvent(EVENT_MOVE_RANDOM, 15000);
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_eyebeam_targetAI(creature);
        }
};
// durumu eyes 67875,67859,67858,67856,67854,67855
class npc_durumu_eye : public CreatureScript
{
    public:
        npc_durumu_eye() : CreatureScript("npc_durumu_eye") { }

        struct npc_durumu_eyeAI : public ScriptedAI
        {
            npc_durumu_eyeAI(Creature* creature) : ScriptedAI(creature) 
            {
                instance = creature->GetInstanceScript();
                me->SetHomePosition(me->GetPositionX(),me->GetPositionY(),me->GetPositionZ(),me->GetOrientation());
            }
            
            InstanceScript* instance;
            EventMap events;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                InitializeEvents();
            }

            void InitializeEvents()
            {
                switch (me->GetEntry())
                {
                   case MIND_EYE:
                       events.ScheduleEvent(EVENT_MOVE_RANDOM, 2000);
                       events.ScheduleEvent(EVENT_FORCE_OF_WILL, 32000);
                       break;
                   case HUNGRY_EYE:
                       events.ScheduleEvent(EVENT_MOVE_RANDOM, 2000);
                       events.ScheduleEvent(EVENT_DRAIN_LIFE_TAR_CHECK, 1500);
                       break;
                   case APPRAYISYING_EYE:
                       events.ScheduleEvent(EVENT_MOVE_RANDOM, 2000);
                       events.ScheduleEvent(EVENT_LINGERING_GAZE, 15000);
                       break;
                   case YELLOW_EYE:
                   case BLUE_EYE:
                   case RED_EYE: 
                       events.ScheduleEvent(EVENT_EYES_ORIENTATION, 1000);
                       me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                       break;
                   default:
                       break;
                }
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->SetInCombatWithZone();
            }

            void CheckBetweenPlayerAndDmg()
            {
                std::list<Player*> PlayerList,PlayersInBetween;
                Player * DrainLifeTarget = NULL;
                GetPlayerListInGrid(PlayerList, me, 100.0f);

                for (auto itr : PlayerList)
                    if (itr->HasAura(SPELL_DRAIN_LIFE_STUN))
                        DrainLifeTarget = itr;

                if (DrainLifeTarget != NULL)
                {
                    for (auto itr : PlayerList)
                        if (itr->IsInBetween(me, DrainLifeTarget, 5.0f) && itr->GetGUID() != DrainLifeTarget->GetGUID())
                            PlayersInBetween.push_back(itr);

                    Player * NearstPlayer = DrainLifeTarget;
                    if (NearstPlayer)
                    {
                        for (auto itr : PlayersInBetween)
                            if (itr->GetDistance(me) < NearstPlayer->GetDistance(me))
                                NearstPlayer = itr;

                        DoCast(NearstPlayer,SPELL_DRAIN_LIFE_DAMAGE, true);
                    }
                }
            }

            void RandomMove()
            {
                Position position;
                me->GetRandomPoint(CenterDurumu,5.0f,position);
                if (!me->HasUnitState(UNIT_STATE_CASTING))
                    me->GetMotionMaster()->MovePoint(0, position);
            }

            uint64 SelectBeamTargetGuid()
            {
                if (me->GetEntry() == YELLOW_EYE)
                {
                    std::list<Creature*> beamTarList;
                    GetCreatureListWithEntryInGrid(beamTarList, me, YELLOW_EYE_MOVER, 80.0f);
                    for (auto Beam : beamTarList)
                        return Beam->GetGUID();
                }
                else // red or blue beam
                {
                    std::list<Player*> PlayerList;
                    GetPlayerListInGrid(PlayerList, me, 80.0f);

                    for (auto itr : PlayerList)
                        if (itr->HasAura(me->GetEntry() == RED_EYE ?  SPELL_RED_BEAM : SPELL_BLUE_BEAM))
                            return itr->GetGUID();
                }
                return 0;
            }

            void TrySearchFogs()
            {
                std::list<Creature*> fogList;
                GetCreatureListWithEntryInGrid(fogList, me, me->GetEntry() == RED_EYE ? RED_FOG : BLUE_FOG, 80.0f);
                for (auto fog : fogList)
                    if (me->isInFront(fog,M_PI/7)) // if fog is front
                        fog->GetAI()->DoAction(ACTION_FOG_ACTIVATE);
            }

            void UpdateAI(const uint32 diff)
            { 
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_MOVE_RANDOM:
                        RandomMove();
                        events.ScheduleEvent(EVENT_MOVE_RANDOM, urand(4000,12000));
                        break;
                    case EVENT_FORCE_OF_WILL:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                        {
                            me->SetFacingTo(me->GetAngle(target));
                            DoCast(target,SPELL_FORCE_OF_WILL);
                        }
                        //DoCast(SPELL_FORCE_OF_WILL);
                        if (Creature* durumu = ObjectAccessor::GetCreature(*me, FindDurumuGUID(me)))
                            durumu->GetAI()->DoAction(ACTION_SAY_FORCE_OF_WILL);

                        events.ScheduleEvent(EVENT_FORCE_OF_WILL, 20000);
                        break;
                   case EVENT_LINGERING_GAZE:
                        for (uint8 i=0;i<=1;++i)
                            if (Player* target = ObjectAccessor::GetPlayer(*me, SelecGUIDtRandomPlayerInRage(me,100.0f, true)))
                                DoCast(target,SPELL_LINGERING_GAZE_MARKER);

                        DoCast(SPELL_LINGERING_GAZE_CAST);
                        events.ScheduleEvent(EVENT_LINGERING_GAZE, 45000);
                        break;
                   case EVENT_DRAIN_LIFE:
                       if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 80.0f, true))
                       {
                           DoCast(target,SPELL_DRAIN_LIFE_STUN);
                           DoCast(target,SPELL_DRAIN_LIFE_CHANNEL);
                       }
                       break;
                   case EVENT_DRAIN_LIFE_TAR_CHECK:
                       CheckBetweenPlayerAndDmg();
                       events.ScheduleEvent(EVENT_DRAIN_LIFE_TAR_CHECK, 1500);
                       break;
                   case EVENT_EYES_ORIENTATION:
                        if (Unit* target = ObjectAccessor::GetUnit(*me, SelectBeamTargetGuid()))
                            me->SetFacingToObject(target);

                       if (me->GetEntry() != YELLOW_EYE)
                           TrySearchFogs();

                       events.ScheduleEvent(EVENT_EYES_ORIENTATION, 200);
                       break;
                   default:
                        break;
                }

            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_EYE_DRAIN_LIFE:
                        events.ScheduleEvent(EVENT_DRAIN_LIFE, 12000);
                        break;
                    case ACTION_MAZE_START:
                        events.CancelEvent(EVENT_LINGERING_GAZE);
                        events.CancelEvent(EVENT_FORCE_OF_WILL);
                        break;
                    case ACTION_MAZE_END:
                        switch (me->GetEntry())
                        {
                           case MIND_EYE:
                              events.ScheduleEvent(EVENT_FORCE_OF_WILL, 25000);
                               break;
                           case APPRAYISYING_EYE:
                              events.ScheduleEvent(EVENT_LINGERING_GAZE, 19000);
                               break;
                           default:
                              break;
                        }
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_durumu_eyeAI(creature);
        }
};

// Arterial Cut 133768
class spell_arterial_cut : public SpellScriptLoader
{
    public:
        spell_arterial_cut() : SpellScriptLoader("spell_arterial_cut") { }

        class spell_arterial_cut_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arterial_cut_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (GetTarget())
                    if (GetTarget()->GetHealth() >= GetTarget()->GetMaxHealth())
                        if (GetTarget()->HasAura(GetId()))
                            GetTarget()->RemoveAura(GetId());
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_arterial_cut_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arterial_cut_AuraScript();
        }
};

// Drain life dmg calculate 133798
class spell_drain_life_damage : public SpellScriptLoader
{
    public:
        spell_drain_life_damage() : SpellScriptLoader("spell_drain_life_damage") { }

        class spell_drain_life_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_drain_life_damage_SpellScript);

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            { 
                if (!GetCaster() || !GetHitUnit())
                    return;

                int32 amount = GetHitDamage();

                if (GetHitUnit()->HasAura(SPELL_DRAIN_LIFE_DAMAGE))
                    amount += (amount * 0.60)*GetHitUnit()->GetAura(SPELL_DRAIN_LIFE_DAMAGE)->GetStackAmount();

                int32 bp = amount * 25;
                std::list<Creature*> durumu;
                GetCreatureListWithEntryInGrid(durumu, GetCaster(), 68036, 100.0f);
                for (auto durumuTarget : durumu)
                    durumuTarget->CastCustomSpell(durumuTarget,SPELL_DRAIN_LIFE_HEAL,&bp,NULL,NULL,true);

                SetHitDamage(amount);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_drain_life_damage_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_drain_life_damage_SpellScript();
        }
};

// force of will removing 136932
class spell_force_of_will : public SpellScriptLoader
{
    public:
        spell_force_of_will() : SpellScriptLoader("spell_force_of_will") { }

        class spell_force_of_will_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_force_of_will_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                GetCaster()->CastSpell(GetCaster(),SPELL_FORCE_OF_WILL_KNOCKBACK,true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_force_of_will_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_force_of_will_AuraScript();
        }
};

// Lingering gaze marker remove 134626
class spell_lingering_gaze : public SpellScriptLoader
{
    public:
        spell_lingering_gaze() : SpellScriptLoader("spell_lingering_gaze") { }

        class spell_lingering_gaze_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_lingering_gaze_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetTarget() || !GetCaster())
                    return;

                GetCaster()->CastSpell(GetTarget(),SPELL_LINGERING_GAZE_DMG,true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_lingering_gaze_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_lingering_gaze_AuraScript();
        }
};

// Beam damage 133732 133677 133738
class spell_beam_aoe_dmg : public SpellScriptLoader
{
    public:
        spell_beam_aoe_dmg() : SpellScriptLoader("spell_beam_aoe_dmg") { }

        class spell_beam_aoe_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_beam_aoe_dmg_SpellScript);

            void SelectTarget(std::list<WorldObject*>& targets)
            {
                if (!GetCaster())
                    return;

                std::list<Unit*> CheckTargets;
                for (auto target : targets)
                {
                    if (!target->ToUnit())
                        continue;

                    if (GetSpellInfo()->Id == SPELL_RED_BEAM_DMG && !target->ToUnit()->HasAura(SPELL_RED_BEAM)) // If target not target of beam
                        CheckTargets.push_back(target->ToUnit());
                    
                    if (GetSpellInfo()->Id == SPELL_BLUE_BEAM_DMG && !target->ToUnit()->HasAura(SPELL_BLUE_BEAM)) // If target not target of beam
                        CheckTargets.push_back(target->ToUnit());

                    if (GetSpellInfo()->Id == SPELL_YELLOW_BEAM_DMG && !target->ToUnit()->HasAura(SPELL_YELLOW_BEAM)) // If target not target of beam
                        CheckTargets.push_back(target->ToUnit());
                }

                if (CheckTargets.size() == 0)
                {
                    if (GetSpellInfo()->Id == SPELL_RED_BEAM_DMG) // redbeam dmg
                        GetCaster()->CastSpell(GetCaster(),SPELL_RED_BEAM_DMG_TO_ALL,true); //aoe dmg to all
                    if (GetSpellInfo()->Id == SPELL_BLUE_BEAM_DMG) // bluebeam dmg
                        GetCaster()->CastSpell(GetCaster(),SPELL_BLUE_BEAM_DMG_TO_ALL,true); //aoe dmg to all
                    if (GetSpellInfo()->Id == SPELL_YELLOW_BEAM_DMG) // yellowbeam dmg
                        GetCaster()->CastSpell(GetCaster(),SPELL_YELLOW_BEAM_DMG_TO_ALL,true); //aoe dmg to all
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_beam_aoe_dmg_SpellScript::SelectTarget, EFFECT_0, TARGET_UNIT_CONE_ENEMY_110);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_beam_aoe_dmg_SpellScript();
        }
};

void AddSC_boss_durumu()
{
    new npc_roaming_fog();
    new boss_durumu();
    new spell_arterial_cut();
    new spell_force_of_will();
    new spell_lingering_gaze();
    new npc_durumu_eye();
    new spell_drain_life_damage();
    new npc_eyebeam_target();
    new npc_durumu_fog();
    new spell_beam_aoe_dmg();
}
