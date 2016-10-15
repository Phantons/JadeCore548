/*
* Copyright (C) 2016-20XX JadeCore <https://github.com/cooler-SAI/JadeCore548-patched>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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
 *
 * Raid: Throne of Thunder.
 * Boss: Horridon.
 *
 * Wowpedia boss history:
 *
 * "The Zandalari brought powerful creatures with them to the Isle of Thunder to use as engines of war. 
 *  Led by the War-God Jalak, the dinomancers of the Zandalari Beast Ward use ancient tribal magics to strengthen the great beasts and command obedience. 
 *  The horns of Horridon, the fabled mount of Jalak himself, can tear through the stone walls of a keep as a blade cuts silk."
 */

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

enum Yells
{
    // Boss

    ANN_FARRAKI_ADDS            = 0, // Farraki forces pour from the Farraki Tribal Door!
    ANN_GURUBASHI_ADDS,              // Gurubashi forces pour from the Gurubashi Tribal Door!
    ANN_DRAKKARI_ADDS,               // Drakkari forces pour from the Drakkari Tribal Door!
    ANN_AMANI_ADDS,                  // Amani forces pour from the Amani Tribal Door!

    // War God Jalak

    SAY_INTRO_1,                     // Welcome, weaklings, to the rebirth of the Zandalari Empire!
    SAY_INTRO_2,                     // The tribes have assembled - ye face not one force, but the combined might of all of the troll empire! The hand of Zul will span all the continents of Azeroth once again! An ye lesser races will know pain!
    SAY_INTRO_3,                     // Now, witness the true might of the Beast Ward. D'akala di'chuka HORRIDON! Kalimaste!

    SAY_FARRAKI,                     // Farraki tribe, flay their flesh wit the fury of the sands!
    SAY_GURUBASHI,                   // Gurubashi tribe, send their bloated corpses down the river with ya jungle poisons.
    SAY_DRAKKARI,                    // Drakkari tribe, show them the might of the frozen North!
    SAY_AMANI,                       // Amani tribe, avenge ye fallen warlords, in the name of Zul'jin!

    SAY_AGGRO,                       // Hahaha! Now it be my turn! Ye gonna see what it means to be a War-God!
    SAY_KILL,                        // Ya skull gonna make a fine ornament for my tusks.
    SAY_DEATH                        // Da'kala koraste...Horridon...destroy them...
};

enum Spells
{
    /*** Boss ***/

    SPELL_TRIPLE_PUNCTURE       = 136767, // On tank, 370000 to 430000 Physical damage and increases the damage taken from Triple Puncture by 10% for 1.50 min. Stacks.
    SPELL_DOUBLE_SWIPE          = 136741, // Both swipe spells trigger 136739 / 136740. 277500 to 322500 Physical damage in front and back of caster in a 35y cone.
    SPELL_HORRIDON_CHARGE       = 136769, // Horridon charges at a random player, performing a Double Swipe once he reaches their location.
    SPELL_HORRIDON_CHARGE_TRIG  = 146740, // Charge to Object spell. Custom.
    SPELL_DOUBLE_SWIPE_CHARGE   = 136770, // This one is triggered by Charge.
    SPELL_DIRE_CALL             = 137458, // Increase ally melee speed 50% (eff 0), 250000 Physical damage to all raid (eff 1), summon direhorn spirit dummy (effect 2) - 140947. HEROIC only.
    SPELL_HEADACHE              = 137294, // 10 second stun after crashing in door by Orb of Control usage. Triggers SPELL_CRACKED_SHELL.

    SPELL_BERSERK               = 144369, // Berserk, Enrage, Bye - Bye or, simply put, a wipe. :)

    // Unused boss spells:
    // SPELL_CRACKED_SHELL         = 137240, // Door slam 50% increase damage debuff.
    // SPELL_SUMMON_DIREHORN_SPIR  = 140945, // Triggered from 140947 spell on SPELL_DIRE_CALL effect 2. Done by DB (insert into spell_linked_spell values (140947, 140945, 0, 'summon Direhorn Spirit');).

    // Direhorn Spirit - Heroic
    SPELL_DIRE_FIXATION         = 140946, // Pet control Fixate aura (like Jin'rokh's Focused Lightning).
    SPELL_WEAK_LINK             = 140948, // Mob aura, knockback on damage taken.

    /*** Farraki tribe ***/

    // Sul'lithuz Stonegazer
    SPELL_STONE_GAZE            = 136708, // 10 second stun.

    // Farraki Skirmisher - melee only.

    // Farraki Wastewalker
    SPELL_BLAZING_SUNLIGHT      = 136719, // Damage + periodic.

    // Sand Trap (summoned by Farraki Wastewalker)
    SPELL_SAND_TRAP             = 136724, // Periodic aura, visual included. Does not move. Grows.

    /*** Gurubashi tribe ***/

    // Gurubashi Bloodlord
    SPELL_RENDING_CHARGE        = 136654,
    SPELL_RENDING_CHARGE_DOT    = 136654, // 40k / sec for 15 seconds. Stacks.

    // Gurubashi Venom Priest
    SPELL_VENOM_BOLT_VOLLEY     = 136587, // Damage + periodic.

    // Venomous Effusion (summoned by Venom Priest) - uses also SPELL_VENOM_BOLT_VOLLEY.

    // Living Poison (summoned by Venomous Effusion)
    SPELL_LIVING_POISON         = 136645, // Periodic aura, visual included. Moves random.

    /*** Drakkari tribe ***/

    // Risen Drakkari Warrior
    SPELL_UNCONTROLLED_ABOM     = 136709, // Attacks random targets and stacks Deadly Plague on them for 15k / sec dmg.

    // Risen Drakkari Champion - uses also SPELL_UNCONTROLLED_ABOM.

    // Drakkari Frozen Warlord
    SPELL_MORTAL_STRIKE_W       = 136670, // 200% weapon damage and 50% heal decrease for 8 secs.

    // Frozen Orb (summoned by Drakkari Frozen Warlord)
    SPELL_FROZEN_BOLT           = 136572, // periodic aura, visual included.

    /*** Amani tribe ***/

    // Amani'shi Protector - melee only.

    // Amani'shi Flame Caster
    SPELL_FIREBALL_FC           = 136465, // 92500 - 107500 damage.

    // Amani Warbear
    SPELL_WARBEAR_SWIPE         = 136463, // 92500 - 107500 damage 5y cone.

    // Amani'shi Beast Shaman
    SPELL_CHAIN_LIGHTNING       = 136480, // 74000 - 86000 damage, 3 target jumps.
    SPELL_HEX_OF_CONFUSION      = 136512, // Aura, triggers 136513 (60% chance) on ability use, inflicting self damage of 46250 to 53750.
    SPELL_LIGHTNING_NOVA_TOTEM  = 136487, // Summons NPC_LIGHTNING_NOVA_TOTEM.

    // Lightning Nova Totem (summoned by Amani'shi Beast Shaman)
    SPELL_LIGHTNING_NOVA        = 136489, // periodic aura, visual included.

    /*** Zandalari Dinomancer ("mini bosses" which signal the end of each triber phase) ***/

    SPELL_DINO_MENDING          = 136797, // Heals 1% of Horridon's health each sec. Cast continously till 50%. Interruptible.
    SPELL_DINO_FORM             = 137237, // Transform, increase damage by 50%, stops healing boss.

    /*** War-God Jalak ***/
    SPELL_BESTIAL_CRY           = 136817, // 100k raid dmg + 50% increase. Stacks.
    SPELL_RAMPAGE               = 136821, // On Horridon, when he dies.

    /*** Orb of Control ***/
    SPELL_ORB_OF_CONTROL_FARRAK = 137433, // These 4 correspond to the GO's.
    SPELL_ORB_OF_CONTROL_GURUB  = 137442,
    SPELL_ORB_OF_CONTROL_DRAKK  = 137443,
    SPELL_ORB_OF_CONTROL_AMANI  = 137444
};

enum Events
{
    // Boss

    EVENT_TRIPLE_PUNCTURE        = 1, // 10 secs from combat start, 11 seconds after that.
    EVENT_DOUBLE_SWIPE           = 2, // 16 - 17 secs from combat start, 17 seconds after that
    EVENT_HORRIDON_CHARGE        = 3, // 31 - 35 secs from combat start, 50 - 60 seconds after that.
    EVENT_DOUBLE_SWIPE_CHARGE    = 4,
    EVENT_DIRE_CALL              = 5, // HEROIC only. Every 62 - 70 seconds.

    // Taken from lua: "So it goes, door, 18.91 seconds later, 1 add jumps down. 18.91 seconds later, next 2 drop down. 18.91 seconds later, dinomancer drops down, then 56.75 seconds later, next door starts.".
    // "When the Zandalari Dinomancer transforms (at 50%), it drops its Orb of Control. A player can interact with the orb to temporarily dominate Horridon's mind, forcing the beast to charge into a tribal door."

    EVENT_CALL_ADDS              = 6, // 18.9 secs from combat start, and afterwards. Open add door.
    EVENT_CALL_NEW_ADDS          = 7, // Mostly 2.4 seconds after new door opens, so 113.5 seconds after that.
    EVENT_CALL_JALAK             = 8, // Jalak jumps in arena and engages, 143 seconds after all 4 add doors opened and closed.

    EVENT_BROKE_DOOR             = 9, // Charged door.

    EVENT_BERSERK                = 10, // 12 minutes into the fight.

    // Adds

    EVENT_STONE_GAZE             = 11,
    EVENT_BLAZING_SUNLIGHT       = 12,
    EVENT_SAND_TRAP              = 13,

    EVENT_RENDING_CHARGE         = 14,
    EVENT_VENOM_BOLT_VOLLEY      = 15,
    EVENT_EFFUSION_AND_POISON    = 16,

    EVENT_MORTAL_STRIKE_W        = 17,
    EVENT_FROZEN_ORB             = 18,

    EVENT_FIREBALL               = 19,
    EVENT_SWIPE                  = 20,
    EVENT_CHAIN_LIGHTNING        = 21,
    EVENT_HEX_OF_CONFUSION       = 22,
    EVENT_LIGHTNING_NOVA_TOTEM   = 23,

    EVENT_DINO_MENDING           = 24,

    EVENT_INTRO_1                = 25,
    EVENT_INTRO_2                = 26,
    EVENT_INTRO_3                = 27,

    EVENT_HORRIDON_MOVE          = 28,
    EVENT_HORRIDON_ENGAGE        = 29,

    EVENT_JALAK_JUMP             = 30,
    EVENT_ENGAGE                 = 31,
    EVENT_BESTIAL_CRY            = 32,
    EVENT_RAMPAGE                = 33,
};

enum Timers
{
    // Boss

    TIMER_TRIPLE_PUNCTURE_F     = 10000,
    TIMER_TRIPLE_PUNCTURE_S     = 11000,

    TIMER_DOUBLE_SWIPE_F        = 17000,
    TIMER_DOUBLE_SWIPE_S        = 17000,

    TIMER_CHARGE_F              = 33000,
    TIMER_CHARGE_S              = 55000,
    TIMER_DOUBLE_SWIPE_CHARGE   = 6000,

    TIMER_DIRE_CALL             = 66000, // HEROIC only.

    TIMER_CALL_ADDS             = 18900,
    TIMER_CALL_NEW_ADDS_F       = 115800,
    TIMER_CALL_NEW_ADDS_S       = 113500,

    TIMER_CALL_JALAK            = 143000,

    TIMER_BROKE_DOOR            = 3000,

    TIMER_BERSERK               = 720000, // 12 minutes.

    // Farraki

    TIMER_STONE_GAZE_F          = 7000,
    TIMER_STONE_GAZE_S          = 16000,

    TIMER_BLAZING_SUNLIGHT_F    = 8000,
    TIMER_BLAZING_SUNLIGHT_S    = 17000,

    TIMER_SAND_TRAP             = 14000,

    // Gurubashi

    TIMER_RENDING_CHARGE_F      = 5000,
    TIMER_RENDING_CHARGE_S      = 18000,

    TIMER_VENOM_BOLT_VOLLEY_F   = 16000,
    TIMER_VENOM_BOLT_VOLLEY_S   = 67000,
    TIMER_VENOM_BOLT_VOLLEY_I   = 6000,

    TIMER_EFFUSION_POISON_F     = 8000,
    TIMER_EFFUSION_POISON_S     = 29000,

    // Drakkari

    TIMER_MORTAL_STRIKE_W_F     = 9000,
    TIMER_MORTAL_STRIKE_W_S     = 1300,

    TIMER_FROZEN_ORB            = 19000,

    // Amani

    TIMER_FIREBALL              = 6000,

    TIMER_SWIPE                 = 9000,

    TIMER_CHAIN_LIGHTNING       = 14000,
    TIMER_HEX_OF_CONFUSION_F    = 6000,
    TIMER_HEX_OF_CONFUSION_S    = 29000,
    TIMER_LIGHTNING_NOVA_TOTEM  = 24000,

    // Zandalari Dinomancer
    TIMER_DINO_MENDING_F        = 6000,
    TIMER_DINO_MENDING_S        = 31000,
    TIMER_DINO_MENDING_I        = 3000,

    // War-God Jalak

    TIMER_INTRO_1               = 2000,
    TIMER_INTRO_2               = 7000,
    TIMER_INTRO_3               = 19000,

    TIMER_HORRIDON_MOVE         = 2000,
    TIMER_HORRIDON_ENGAGE       = 7000,

    TIMER_JUMP                  = 1000,
    TIMER_ENGAGE                = 5000,

    TIMER_BESTIAL_CRY_F         = 5000,
    TIMER_BESTIAL_CRY_S         = 10000
};

enum Actions
{
    // Boss
    ACTION_CRASH_DOOR           = 1, // On Orb of Control usage.

    // Gurubashi Venom Priest
    ACTION_VENOM_VOLLEY_REMOVED,

    // Zandalari Dinomancer
    ACTION_MENDING_REMOVED,

    // War-God Jalak
    ACTION_START_INTRO,              // Start boss intro.
    ACTION_JUMP_AND_ENGAGE           // After all 4 tribes defeated (last boss phase).
};

enum Phases
{
    PHASE_NONE                  = 0,

    PHASE_FARRAKI               = 1,
    PHASE_GURUBASHI             = 2,
    PHASE_DRAKKARI              = 3,
    PHASE_AMANI                 = 4,
    PHASE_JALAK                 = 5,
};

enum Npcs
{
    // Boss
    NPC_DIREHORN_SPIRIT          = 70688, // Heroic

    // Adds

    // Farraki
    NPC_SUL_LITHUZ_STONEGAZER    = 69172,
    NPC_FARRAKI_SKIRMISHER       = 69173,
    NPC_FARRAKI_WASTEWALKER      = 69175,
    NPC_SAND_TRAP                = 69346,

    // Gurubashi
    NPC_GURUBASHI_BLOODLORD      = 69167,
    NPC_GURUBASHI_VENOM_PRIEST   = 69164,
    NPC_VENOMOUS_EFFUSION_HORR   = 69314,
    NPC_LIVING_POISON            = 69313,

    // Drakkari
    NPC_RISEN_DRAKKARI_WARRIOR   = 69184,
    NPC_RISEN_DRAKKARI_CHAMPION  = 69185,
    NPC_DRAKKARI_FROZEN_WARLORD  = 69178,
    NPC_FROZEN_ORB_HORR          = 69268,

    // Amani
    NPC_AMANI_SHI_PROTECTOR      = 69169,
    NPC_AMANI_SHI_FLAME_CASTER   = 69168,
    NPC_AMANI_WARBEAR            = 69177,
    NPC_AMANI_SHI_BEAST_SHAMAN   = 69176,
    NPC_LIGHTNING_NOVA_TOTEM     = 69215,

    // Zandalari Dinomancer
    NPC_ZANDALARI_DINOMANCER     = 69221,

    // War-God Jalak
    NPC_WAR_GOD_JALAK            = 69374
};

enum GameObjects
{
    GO_ORB_OF_CONTROL_FARRAKI    = 218193, // Used to get Horridon to crash into the Farraki gate.
    GO_ORB_OF_CONTROL_GURUBASHI  = 218374, // Used to get Horridon to crash into the Gurubashi gate.
    GO_ORB_OF_CONTROL_DRAKKARI   = 218375, // Used to get Horridon to crash into the Drakkari gate.
    GO_ORB_OF_CONTROL_AMANI      = 218376, // Used to get Horridon to crash into the Amani gate.

    GO_FARRAKI_DOOR              = 218672,
    GO_GURUBASHI_DOOR            = 218670,
    GO_DRAKKARI_DOOR             = 218671,
    GO_AMANI_DOOR                = 218673,
    GO_START_DOOR                = 218674  // Door Horridon breaks during intro.
};

Position const chargePlace[4] =
{
    { 5492.223f, 5813.665f, 130.04f }, // Farraki   - North-west.
    { 5488.879f, 5695.583f, 130.04f }, // Gurubashi - North-east.
    { 5372.876f, 5694.684f, 130.04f }, // Drakkari  - South-east.
    { 5373.481f, 5811.625f, 130.04f }, // Amani     - South-west.
};

Position const tribesmenSummonPlace[4] =
{
    { 5525.238f, 5850.004f, 131.123f }, // Farraki   - North-west.
    { 5530.141f, 5658.262f, 130.130f }, // Gurubashi - North-east.
    { 5337.873f, 5657.117f, 130.122f }, // Drakkari  - South-east.
    { 5336.561f, 5845.739f, 130.117f }, // Amani     - South-west.
};

Position const jalakIntroPos        = { 5432.82f, 5671.34f, 192.323f }; // Jalak summon position.
Position const midPos               = { 5434.05f, 5752.63f, 129.689f }; // Mid arena position Horridon moves to during intro.

float addOrientations[5] =
{
    3.94f,  // Farraki   - North-west.
    2.41f,  // Gurubashi - North-east.
    0.85f,  // Drakkari  - South-east.
    5.54f,  // Amani     - South-west.
    1.53f   // Jalak     - Intro.
};

#define MAX_TRIBAL_DOORS 4

class boss_horridon : public CreatureScript
{
    public:
        boss_horridon() : CreatureScript("boss_horridon") { }

        struct boss_horridonAI : public BossAI
        {
            boss_horridonAI(Creature* creature) : BossAI(creature, DATA_HORRIDON_EVENT), summons(me)
            {
                instance  = creature->GetInstanceScript();
                introDone = false;
                wiped = false;
            }

            InstanceScript* instance;
            SummonList summons;
            EventMap events;
            Phases phase;
            bool introDone, wiped;
            uint8 addWavesSummonedPhase;

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();

                for (uint8 i = 0; i < MAX_TRIBAL_DOORS + 1; ++i) // +1 cause of intro one.
                {
                    if (GameObject* door = me->FindNearestGameObject(GO_GURUBASHI_DOOR + i, 500.0f))
                    {
                        door->ResetDoorOrButton();
                        door->SetLootState(GO_READY);
                    }
                }

                phase = PHASE_NONE;
                addWavesSummonedPhase = 0;

                if (instance)
                    instance->SetData(DATA_HORRIDON_EVENT, NOT_STARTED);

                _Reset();

                if (Creature* Jalak = me->SummonCreature(NPC_WAR_GOD_JALAK, jalakIntroPos, TEMPSUMMON_MANUAL_DESPAWN))
                    Jalak->SetFacingTo(addOrientations[4]);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!introDone && me->IsWithinDistInMap(who, 50.0f, true) && who->GetTypeId() == TYPEID_PLAYER)
                {
                    me->SetReactState(REACT_DEFENSIVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                        Jalak->AI()->DoAction(ACTION_START_INTRO);
                    introDone = true;
                }
            }

            void EnterCombat(Unit* who)
            {
                if (wiped)
                    if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                        Jalak->AI()->Talk(SAY_INTRO_3);

                phase = PHASE_FARRAKI;

				events.ScheduleEvent(EVENT_TRIPLE_PUNCTURE, TIMER_TRIPLE_PUNCTURE_F);
				events.ScheduleEvent(EVENT_DOUBLE_SWIPE, TIMER_DOUBLE_SWIPE_F);
				events.ScheduleEvent(EVENT_HORRIDON_CHARGE, TIMER_CHARGE_F);

                if (IsHeroic())
				    events.ScheduleEvent(EVENT_DIRE_CALL, TIMER_DIRE_CALL);

				events.ScheduleEvent(EVENT_CALL_NEW_ADDS, TIMER_CALL_NEW_ADDS_F);
				events.ScheduleEvent(EVENT_CALL_ADDS, TIMER_CALL_ADDS);

				events.ScheduleEvent(EVENT_BERSERK, TIMER_BERSERK);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    instance->SetData(DATA_HORRIDON_EVENT, IN_PROGRESS);
                }

                _EnterCombat();
            }

			void EnterEvadeMode()
            {
                me->AddUnitState(UNIT_STATE_EVADE);

                DespawnSummon(NPC_AMANI_SHI_BEAST_SHAMAN);
                DespawnSummon(NPC_VENOMOUS_EFFUSION_HORR);
                DespawnSummon(NPC_SAND_TRAP);
                DespawnSummon(NPC_LIVING_POISON);
                DespawnSummon(NPC_FROZEN_ORB_HORR);
                DespawnSummon(NPC_LIGHTNING_NOVA_TOTEM);

                DespawnGameObject(GO_ORB_OF_CONTROL_FARRAKI);
                DespawnGameObject(GO_ORB_OF_CONTROL_GURUBASHI);
                DespawnGameObject(GO_ORB_OF_CONTROL_DRAKKARI);
                DespawnGameObject(GO_ORB_OF_CONTROL_AMANI);

                wiped = true;

                me->RemoveAllAuras();
                Reset();
                me->DeleteThreatList();
                me->CombatStop(true);
                me->GetMotionMaster()->MovementExpired();
                me->GetMotionMaster()->MoveTargetedHome();

                if (instance)
                {
                    instance->SetData(DATA_HORRIDON_EVENT, FAIL);
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me); // Remove
                }

                _EnterEvadeMode();

                if (!me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                    if (Creature* Jalak = me->SummonCreature(NPC_WAR_GOD_JALAK, jalakIntroPos, TEMPSUMMON_MANUAL_DESPAWN))
                        Jalak->SetFacingTo(addOrientations[4]);
            }

            void JustReachedHome()
            {
                me->ClearUnitState(UNIT_STATE_EVADE);

                _JustReachedHome();
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
				summon->setActive(true);
            }

            void JustDied(Unit* killer)
            {
                summons.DespawnAll();

                DespawnSummon(NPC_AMANI_SHI_BEAST_SHAMAN);
                DespawnSummon(NPC_VENOMOUS_EFFUSION_HORR);
                DespawnSummon(NPC_SAND_TRAP);
                DespawnSummon(NPC_LIVING_POISON);
                DespawnSummon(NPC_FROZEN_ORB_HORR);
                DespawnSummon(NPC_LIGHTNING_NOVA_TOTEM);

                DespawnGameObject(GO_ORB_OF_CONTROL_FARRAKI);
                DespawnGameObject(GO_ORB_OF_CONTROL_GURUBASHI);
                DespawnGameObject(GO_ORB_OF_CONTROL_DRAKKARI);
                DespawnGameObject(GO_ORB_OF_CONTROL_AMANI);

                if (instance)
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    instance->SetData(DATA_HORRIDON_EVENT, DONE);
                }

                _JustDied();
            }

            void DespawnSummon(uint32 entry)
            {
                std::list<Creature*> summonsList;
                GetCreatureListWithEntryInGrid(summonsList, me, entry, 200.0f);
                if (!summonsList.empty())
                    for (std::list<Creature*>::iterator summs = summonsList.begin(); summs != summonsList.end(); summs++)
                        (*summs)->DespawnOrUnsummon();
            }

            void DespawnGameObject(uint32 entry)
            {
                std::list<GameObject*> summonsList;
                GetGameObjectListWithEntryInGrid(summonsList, me, entry, 200.0f);
                if (!summonsList.empty())
                    for (std::list<GameObject*>::iterator summs = summonsList.begin(); summs != summonsList.end(); summs++)
                        (*summs)->RemoveFromWorld();
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                switch (spell->Id)
                {
                    case SPELL_ORB_OF_CONTROL_FARRAK:
                        me->GetMotionMaster()->MoveCharge(chargePlace[0].GetPositionX(), chargePlace[0].GetPositionY(), chargePlace[0].GetPositionZ(), 30.0f);
                        events.CancelEvent(EVENT_CALL_ADDS);
                        events.ScheduleEvent(EVENT_BROKE_DOOR, TIMER_BROKE_DOOR);
                        break;

                    case SPELL_ORB_OF_CONTROL_GURUB:
                        me->GetMotionMaster()->MoveCharge(chargePlace[1].GetPositionX(), chargePlace[1].GetPositionY(), chargePlace[1].GetPositionZ(), 30.0f);
                        events.CancelEvent(EVENT_CALL_ADDS);
                        events.ScheduleEvent(EVENT_BROKE_DOOR, TIMER_BROKE_DOOR);
                        break;

                    case SPELL_ORB_OF_CONTROL_DRAKK:
                        me->GetMotionMaster()->MoveCharge(chargePlace[2].GetPositionX(), chargePlace[2].GetPositionY(), chargePlace[2].GetPositionZ(), 30.0f);
                        events.CancelEvent(EVENT_CALL_ADDS);
                        events.ScheduleEvent(EVENT_BROKE_DOOR, TIMER_BROKE_DOOR);
                        break;

                    case SPELL_ORB_OF_CONTROL_AMANI:
                        me->GetMotionMaster()->MoveCharge(chargePlace[3].GetPositionX(), chargePlace[3].GetPositionY(), chargePlace[3].GetPositionZ(), 30.0f);
                        events.CancelEvent(EVENT_CALL_ADDS);
                        events.ScheduleEvent(EVENT_BROKE_DOOR, TIMER_BROKE_DOOR);
                        break;

                    default: break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || !CheckInRoom() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (instance && instance->IsWipe())
                {
                    EnterEvadeMode();
                    return;
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TRIPLE_PUNCTURE:
				            DoCast(me->getVictim(), SPELL_TRIPLE_PUNCTURE);
				            events.ScheduleEvent(EVENT_TRIPLE_PUNCTURE, TIMER_TRIPLE_PUNCTURE_S);
                            break;

                        case EVENT_DOUBLE_SWIPE:
				            DoCast(me, SPELL_DOUBLE_SWIPE);
				            events.ScheduleEvent(EVENT_DOUBLE_SWIPE, TIMER_DOUBLE_SWIPE_S);
                            break;

                        case EVENT_HORRIDON_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 100.0f, true))
                                DoCast(target, SPELL_HORRIDON_CHARGE);
				            events.ScheduleEvent(EVENT_HORRIDON_CHARGE, TIMER_CHARGE_S);
				            events.ScheduleEvent(EVENT_DOUBLE_SWIPE_CHARGE, TIMER_DOUBLE_SWIPE_CHARGE);
                            break;

                        case EVENT_DOUBLE_SWIPE_CHARGE:
                            DoCast(me, SPELL_DOUBLE_SWIPE_CHARGE);
                            break;

                        case EVENT_DIRE_CALL:
				            DoCast(me, SPELL_DIRE_CALL);
				            events.ScheduleEvent(EVENT_DIRE_CALL, TIMER_DIRE_CALL);
                            break;

                        case EVENT_CALL_NEW_ADDS:
                            if (phase == PHASE_JALAK)
                                break; // fail-safe.

                            // increase phase.
                            if (phase == PHASE_AMANI)
                                phase = PHASE_JALAK;
                            else if (phase == PHASE_DRAKKARI)
                                phase = PHASE_AMANI;
                            else if (phase == PHASE_GURUBASHI)
                                phase = PHASE_DRAKKARI;
                            else if (phase == PHASE_FARRAKI)
                                phase = PHASE_GURUBASHI;

                            addWavesSummonedPhase = 0;
                            events.CancelEvent(EVENT_CALL_ADDS);

                            if (phase != PHASE_AMANI)
                            {
				                events.ScheduleEvent(EVENT_CALL_ADDS, 100);
				                events.ScheduleEvent(EVENT_CALL_NEW_ADDS, TIMER_CALL_NEW_ADDS_S);
                            }
                            else // Go to last phase with War-God Jalak!
                            {
				                events.ScheduleEvent(EVENT_CALL_ADDS, 100);
				                events.ScheduleEvent(EVENT_CALL_JALAK, TIMER_CALL_JALAK);
                            }
                            break;

                        case EVENT_CALL_ADDS:
                        {
                            switch (phase)
                            {
                                case PHASE_FARRAKI:
                                {
                                    switch (addWavesSummonedPhase)
                                    {
                                        case 0: // 1 Stonegazer, 2 Skirmisher, 1 Wastewalker.
                                            Talk(ANN_FARRAKI_ADDS);
                                            if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                                                Jalak->AI()->Talk(SAY_FARRAKI);

                                            if (Creature* gazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                gazer->SetFacingTo(addOrientations[0]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMISHER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    skirmisher->SetFacingTo(addOrientations[0]);
                                            if (Creature* walker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                walker->SetFacingTo(addOrientations[0]);
                                            break;

                                        case 1: // 1 Stonegazer, 1 Skirmisher, 2 Wastewalker.
                                            if (Creature* gazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                gazer->SetFacingTo(addOrientations[0]);
                                            if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMISHER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                skirmisher->SetFacingTo(addOrientations[0]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* walker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    walker->SetFacingTo(addOrientations[0]);
                                            break;

                                        case 2: // 1 Stonegazer, 2 Skirmisher, 1 Wastewalker, Dinomancer.
                                            if (Creature* gazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                gazer->SetFacingTo(addOrientations[0]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMISHER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    skirmisher->SetFacingTo(addOrientations[0]);
                                            if (Creature* walker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                walker->SetFacingTo(addOrientations[0]);
                                            if (Creature* dinomancer = me->SummonCreature(NPC_ZANDALARI_DINOMANCER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                dinomancer->SetFacingTo(addOrientations[0]);
                                            break;

                                        default: // Summon any of 0 / 1.
                                            if (urand(0, 1) == 0)
                                            {
                                                if (Creature* gazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    gazer->SetFacingTo(addOrientations[0]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMISHER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                        skirmisher->SetFacingTo(addOrientations[0]);
                                                if (Creature* walker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    walker->SetFacingTo(addOrientations[0]);
                                            }
                                            else // 1
                                            {
                                                if (Creature* gazer = me->SummonCreature(NPC_SUL_LITHUZ_STONEGAZER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    gazer->SetFacingTo(addOrientations[0]);
                                                if (Creature* skirmisher = me->SummonCreature(NPC_FARRAKI_SKIRMISHER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                    skirmisher->SetFacingTo(addOrientations[0]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* walker = me->SummonCreature(NPC_FARRAKI_WASTEWALKER, tribesmenSummonPlace[0], TEMPSUMMON_MANUAL_DESPAWN))
                                                        walker->SetFacingTo(addOrientations[0]);
                                            }
                                            break;
                                    }

                                    if (GameObject* door = me->FindNearestGameObject(GO_FARRAKI_DOOR, 500.0f))
                                        door->UseDoorOrButton(0);
                                    break;
                                }

                                case PHASE_GURUBASHI:
                                {
                                    switch (addWavesSummonedPhase)
                                    {
                                        case 0: // 2 Bloodlord, 1 Venom Priest.
                                            Talk(ANN_GURUBASHI_ADDS);
                                            if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                                                Jalak->AI()->Talk(SAY_GURUBASHI);

                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    bloodlord->SetFacingTo(addOrientations[1]);
                                            if (Creature* priest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                priest->SetFacingTo(addOrientations[1]);
                                            break;

                                        case 1: // 1 Bloodlord, 2 Venom Priest.
                                            if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                bloodlord->SetFacingTo(addOrientations[1]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* priest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    priest->SetFacingTo(addOrientations[1]);
                                            break;

                                        case 2: // 2 Bloodlord, 2 Venom Priest, Dinomancer.
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    bloodlord->SetFacingTo(addOrientations[1]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* priest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    priest->SetFacingTo(addOrientations[1]);
                                            if (Creature* dinomancer = me->SummonCreature(NPC_ZANDALARI_DINOMANCER, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                dinomancer->SetFacingTo(addOrientations[1]);
                                            break;

                                        default: // Summon any of 0 / 1.
                                            if (urand(0, 1) == 0)
                                            {
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                        bloodlord->SetFacingTo(addOrientations[1]);
                                                if (Creature* priest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    priest->SetFacingTo(addOrientations[1]);
                                            }
                                            else // 1
                                            {
                                                if (Creature* bloodlord = me->SummonCreature(NPC_GURUBASHI_BLOODLORD, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                    bloodlord->SetFacingTo(addOrientations[1]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* priest = me->SummonCreature(NPC_GURUBASHI_VENOM_PRIEST, tribesmenSummonPlace[1], TEMPSUMMON_MANUAL_DESPAWN))
                                                        priest->SetFacingTo(addOrientations[1]);
                                            }
                                            break;
                                    }

                                    if (GameObject* door = me->FindNearestGameObject(GO_GURUBASHI_DOOR, 500.0f))
                                        door->UseDoorOrButton(0);
                                    break;
                                }

                                case PHASE_DRAKKARI:
                                {
                                    switch (addWavesSummonedPhase)
                                    {
                                        case 0: // 1 Champion, 2 Warrior, 1 Warlord.
                                            Talk(ANN_DRAKKARI_ADDS);
                                            if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                                                Jalak->AI()->Talk(SAY_DRAKKARI);

                                            if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                champion->SetFacingTo(addOrientations[2]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warrior->SetFacingTo(addOrientations[2]);
                                            if (Creature* warlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                warlord->SetFacingTo(addOrientations[2]);
                                            break;

                                        case 1: // 1 Champion, 1 Warrior, 2 Warlord.
                                            if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                champion->SetFacingTo(addOrientations[2]);
                                            if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                warrior->SetFacingTo(addOrientations[2]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* warlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warlord->SetFacingTo(addOrientations[2]);
                                            break;

                                        case 2: // 1 Champion, 2 Warrior, 1 Warlord, Dinomancer.
                                            if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                champion->SetFacingTo(addOrientations[2]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warrior->SetFacingTo(addOrientations[2]);
                                            if (Creature* warlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                warlord->SetFacingTo(addOrientations[2]);
                                            if (Creature* dinomancer = me->SummonCreature(NPC_ZANDALARI_DINOMANCER, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                dinomancer->SetFacingTo(addOrientations[2]);
                                            break;

                                        default: // Summon any of 0 / 1.
                                            if (urand(0, 1) == 0)
                                            {
                                                if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    champion->SetFacingTo(addOrientations[2]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                        warrior->SetFacingTo(addOrientations[2]);
                                                if (Creature* warlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warlord->SetFacingTo(addOrientations[2]);
                                            }
                                            else // 1
                                            {
                                                if (Creature* champion = me->SummonCreature(NPC_RISEN_DRAKKARI_CHAMPION, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    champion->SetFacingTo(addOrientations[2]);
                                                if (Creature* warrior = me->SummonCreature(NPC_RISEN_DRAKKARI_WARRIOR, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warrior->SetFacingTo(addOrientations[2]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* warlord = me->SummonCreature(NPC_DRAKKARI_FROZEN_WARLORD, tribesmenSummonPlace[2], TEMPSUMMON_MANUAL_DESPAWN))
                                                        warlord->SetFacingTo(addOrientations[2]);
                                            }
                                            break;
                                    }

                                    if (GameObject* door = me->FindNearestGameObject(GO_DRAKKARI_DOOR, 500.0f))
                                        door->UseDoorOrButton(0);
                                    break;
                                }

                                case PHASE_AMANI:
                                {
                                    switch (addWavesSummonedPhase)
                                    {
                                        case 0: // 1 Flame Caster, 2 Protector, 1 Warbear.
                                            Talk(ANN_AMANI_ADDS);
                                            if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                                                Jalak->AI()->Talk(SAY_AMANI);

                                            if (Creature* flamecaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                flamecaster->SetFacingTo(addOrientations[3]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    protector->SetFacingTo(addOrientations[3]);
                                            if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                warbear->SetFacingTo(addOrientations[3]);
                                            break;

                                        case 1: // 1 Flame Caster, 1 Protector, 2 Warbear.
                                            if (Creature* flamecaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                flamecaster->SetFacingTo(addOrientations[3]);
                                            if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                protector->SetFacingTo(addOrientations[3]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warbear->SetFacingTo(addOrientations[3]);
                                            break;

                                        case 2: // 1 Flame Caster, 2 Protector, 1 Warbear, Dinomancer.
                                            if (Creature* flamecaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                flamecaster->SetFacingTo(addOrientations[3]);
                                            for (uint8 i = 0; i < 2; i++)
                                                if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    protector->SetFacingTo(addOrientations[3]);
                                            if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                warbear->SetFacingTo(addOrientations[3]);
                                            if (Creature* dinomancer = me->SummonCreature(NPC_ZANDALARI_DINOMANCER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                dinomancer->SetFacingTo(addOrientations[3]);
                                            break;

                                        default: // Summon any of 0 / 1.
                                            if (urand(0, 1) == 0)
                                            {
                                                if (Creature* flamecaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    flamecaster->SetFacingTo(addOrientations[3]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                        protector->SetFacingTo(addOrientations[3]);
                                                if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    warbear->SetFacingTo(addOrientations[3]);
                                            }
                                            else // 1
                                            {
                                                if (Creature* flamecaster = me->SummonCreature(NPC_AMANI_SHI_FLAME_CASTER, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    flamecaster->SetFacingTo(addOrientations[3]);
                                                if (Creature* protector = me->SummonCreature(NPC_AMANI_SHI_PROTECTOR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                    protector->SetFacingTo(addOrientations[3]);
                                                for (uint8 i = 0; i < 2; i++)
                                                    if (Creature* warbear = me->SummonCreature(NPC_AMANI_WARBEAR, tribesmenSummonPlace[3], TEMPSUMMON_MANUAL_DESPAWN))
                                                        warbear->SetFacingTo(addOrientations[3]);
                                            }
                                            break;
                                    }

                                    if (GameObject* door = me->FindNearestGameObject(GO_AMANI_DOOR, 500.0f))
                                        door->UseDoorOrButton(0);
                                    break;
                                }

                                default: break;
                            }

                            addWavesSummonedPhase++;
				            events.ScheduleEvent(EVENT_CALL_ADDS, TIMER_CALL_ADDS);
                            break;
                        }

                        case EVENT_CALL_JALAK:
                            if (Creature* Jalak = me->FindNearestCreature(NPC_WAR_GOD_JALAK, 500.0f, true))
                                Jalak->AI()->DoAction(ACTION_JUMP_AND_ENGAGE);
                            break;

                        case EVENT_BROKE_DOOR:
                            // me->GetMotionMaster()->MovementExpired();
                            me->RemoveAurasDueToSpell(SPELL_ORB_OF_CONTROL_FARRAK);
                            me->RemoveAurasDueToSpell(SPELL_ORB_OF_CONTROL_GURUB);
                            me->RemoveAurasDueToSpell(SPELL_ORB_OF_CONTROL_DRAKK);
                            me->RemoveAurasDueToSpell(SPELL_ORB_OF_CONTROL_AMANI);
                            DoCast(me, SPELL_HEADACHE);
                            break;

                        case EVENT_BERSERK:
				            DoCast(me, SPELL_BERSERK);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_horridonAI(creature);
        }
};

/*** Adds ***/

// Direhorn Spirit (Heroic) 70688
class npc_direhorn_spirit_horridon : public CreatureScript
{
    public:
        npc_direhorn_spirit_horridon() : CreatureScript("npc_direhorn_spirit_horridon") { }

        struct npc_direhorn_spirit_horridonAI : public ScriptedAI
        {
            npc_direhorn_spirit_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                me->AddAura(SPELL_WEAK_LINK, me);
                me->SetReactState(REACT_PASSIVE);

                if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 150.0f, true))
                {
                    target->AddAura(SPELL_DIRE_FIXATION, target);
                    me->Attack(target, false);
                    me->GetMotionMaster()->MoveChase(target);

                    // Not tauntable.
                    me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                    me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_ATTACK_ME, true);
                }
            }

            void Reset() { }

            void JustDied(Unit* /*killer*/)
            {
                me->getVictim()->RemoveAurasDueToSpell(SPELL_DIRE_FIXATION);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_direhorn_spirit_horridonAI(creature);
        }
};

/*** Farraki tribe ***/

// Farraki Skirmisher 69173, Amani'shi Protector 69169
class npc_farraki_skirmisher_amani_protector : public CreatureScript
{
    public:
        npc_farraki_skirmisher_amani_protector() : CreatureScript("npc_farraki_skirmisher_amani_protector") { }

        struct npc_farraki_skirmisher_amani_protectorAI : public ScriptedAI
        {
            npc_farraki_skirmisher_amani_protectorAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_farraki_skirmisher_amani_protectorAI(creature);
        }
};

// Sul'lithuz Stonegazer 69172
class npc_sul_lithuz_stonegazer : public CreatureScript
{
    public:
        npc_sul_lithuz_stonegazer() : CreatureScript("npc_sul_lithuz_stonegazer") { }

        struct npc_sul_lithuz_stonegazerAI : public ScriptedAI
        {
            npc_sul_lithuz_stonegazerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_STONE_GAZE, TIMER_STONE_GAZE_F);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STONE_GAZE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f, true))
                                DoCast(target, SPELL_STONE_GAZE);
                            events.ScheduleEvent(EVENT_STONE_GAZE, TIMER_STONE_GAZE_S);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sul_lithuz_stonegazerAI(creature);
        }
};

// Farraki Wastewalker 69175
class npc_farraki_wastewalker : public CreatureScript
{
    public:
        npc_farraki_wastewalker() : CreatureScript("npc_farraki_wastewalker") { }

        struct npc_farraki_wastewalkerAI : public ScriptedAI
        {
            npc_farraki_wastewalkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_BLAZING_SUNLIGHT, TIMER_BLAZING_SUNLIGHT_F);
                events.ScheduleEvent(EVENT_SAND_TRAP, TIMER_SAND_TRAP);
            }

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLAZING_SUNLIGHT:
                            DoCast(me, SPELL_BLAZING_SUNLIGHT);
                            events.ScheduleEvent(EVENT_BLAZING_SUNLIGHT, TIMER_BLAZING_SUNLIGHT_S);
                            break;

                        case EVENT_SAND_TRAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->SummonCreature(NPC_SAND_TRAP, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_SAND_TRAP, TIMER_SAND_TRAP);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_farraki_wastewalkerAI(creature);
        }
};

// Sand Trap 69346
class npc_sand_trap : public CreatureScript
{
    public:
        npc_sand_trap() : CreatureScript("npc_sand_trap") { }

        struct npc_sand_trap_AI : public ScriptedAI
        {
            npc_sand_trap_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
                me->AddAura(SPELL_SAND_TRAP, me);
                me->DespawnOrUnsummon(60000);
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sand_trap_AI(creature);
        }
};

/*** Gurubashi tribe ***/

// Gurubashi Bloodlord 69167
class npc_gurubashi_bloodlord : public CreatureScript
{
    public:
        npc_gurubashi_bloodlord() : CreatureScript("npc_gurubashi_bloodlord") { }

        struct npc_gurubashi_bloodlordAI : public ScriptedAI
        {
            npc_gurubashi_bloodlordAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_RENDING_CHARGE, TIMER_RENDING_CHARGE_F);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_RENDING_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 60.0f, true))
                                DoCast(target, SPELL_RENDING_CHARGE);
                            events.ScheduleEvent(EVENT_RENDING_CHARGE, TIMER_RENDING_CHARGE_S);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gurubashi_bloodlordAI(creature);
        }
};

// Gurubashi Venom Priest 69164
class npc_gurubashi_venom_priest : public CreatureScript
{
    public:
        npc_gurubashi_venom_priest() : CreatureScript("npc_gurubashi_venom_priest") { }

        struct npc_gurubashi_venom_priestAI : public ScriptedAI
        {
            npc_gurubashi_venom_priestAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, TIMER_VENOM_BOLT_VOLLEY_F);
                events.ScheduleEvent(EVENT_EFFUSION_AND_POISON, TIMER_EFFUSION_POISON_F);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_VENOM_VOLLEY_REMOVED:
                        events.CancelEvent(EVENT_VENOM_BOLT_VOLLEY);
                        events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, TIMER_VENOM_BOLT_VOLLEY_I);
                        break;

                    default: break;
                }
            };

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VENOM_BOLT_VOLLEY:
                            DoCast(me, SPELL_VENOM_BOLT_VOLLEY);
                            events.ScheduleEvent(EVENT_VENOM_BOLT_VOLLEY, TIMER_VENOM_BOLT_VOLLEY_S);
                            break;

                        case EVENT_EFFUSION_AND_POISON:
                            me->SummonCreature(NPC_VENOMOUS_EFFUSION_HORR, me->GetPositionX() + 3.0f, me->GetPositionY(), me->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                            me->SummonCreature(NPC_LIVING_POISON, me->GetPositionX() + 3.0f, me->GetPositionY(), me->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_EFFUSION_AND_POISON, TIMER_EFFUSION_POISON_S);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gurubashi_venom_priestAI(creature);
        }
};

// Venomous Effusion 69314
class npc_venomous_effusion_horridon : public CreatureScript
{
    public:
        npc_venomous_effusion_horridon() : CreatureScript("npc_venomous_effusion_horridon") { }

        struct npc_venomous_effusion_horridonAI : public ScriptedAI
        {
            npc_venomous_effusion_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                me->AddAura(SPELL_VENOM_BOLT_VOLLEY, me);
                me->GetMotionMaster()->MoveRandom(40.0f);
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_venomous_effusion_horridonAI(creature);
        }
};

// Living Poison 69313
class npc_living_poison_horridon : public CreatureScript
{
    public:
        npc_living_poison_horridon() : CreatureScript("npc_living_poison_horridon") { }

        struct npc_living_poison_horridon_AI : public ScriptedAI
        {
            npc_living_poison_horridon_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
                me->AddAura(SPELL_LIVING_POISON, me);
                me->GetMotionMaster()->MoveRandom(40.0f);
                me->DespawnOrUnsummon(60000);
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_living_poison_horridon_AI(creature);
        }
};

/*** Drakkari tribe ***/

// Risen Drakkari Warrior 69184, Risen Drakkari Champion 69185
class npc_risen_drakkari_warrior_champion : public CreatureScript
{
    public:
        npc_risen_drakkari_warrior_champion() : CreatureScript("npc_risen_drakkari_warrior_champion") { }

        struct npc_risen_drakkari_warrior_championAI : public ScriptedAI
        {
            npc_risen_drakkari_warrior_championAI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                me->AddAura(SPELL_UNCONTROLLED_ABOM, me);
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_risen_drakkari_warrior_championAI(creature);
        }
};

// Drakkari Frozen Warlord 69178
class npc_drakkari_frozen_warlord : public CreatureScript
{
    public:
        npc_drakkari_frozen_warlord() : CreatureScript("npc_drakkari_frozen_warlord") { }

        struct npc_drakkari_frozen_warlordAI : public ScriptedAI
        {
            npc_drakkari_frozen_warlordAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_MORTAL_STRIKE_W, TIMER_MORTAL_STRIKE_W_F);
                events.ScheduleEvent(EVENT_FROZEN_ORB, TIMER_FROZEN_ORB);
            }

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MORTAL_STRIKE_W:
                            DoCast(me->getVictim(), SPELL_MORTAL_STRIKE_W);
                            events.ScheduleEvent(EVENT_MORTAL_STRIKE_W, TIMER_MORTAL_STRIKE_W_S);
                            break;

                        case EVENT_FROZEN_ORB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->SummonCreature(NPC_FROZEN_ORB_HORR, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_FROZEN_ORB, TIMER_FROZEN_ORB);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_drakkari_frozen_warlordAI(creature);
        }
};

// Frozen Orb 69268
class npc_frozen_orb_horridon : public CreatureScript
{
    public:
        npc_frozen_orb_horridon() : CreatureScript("npc_frozen_orb_horridon") { }

        struct npc_frozen_orb_horridon_AI : public ScriptedAI
        {
            npc_frozen_orb_horridon_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
                me->AddAura(SPELL_FROZEN_BOLT, me);
                me->GetMotionMaster()->MoveRandom(40.0f);
                me->DespawnOrUnsummon(60000);
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frozen_orb_horridon_AI(creature);
        }
};

/*** Amani tribe ***/

// Amani'shi Flame Caster 69168
class npc_amani_shi_flame_caster_horridon : public CreatureScript
{
    public:
        npc_amani_shi_flame_caster_horridon() : CreatureScript("npc_amani_shi_flame_caster_horridon") { }

        struct npc_amani_shi_flame_caster_horridonAI : public ScriptedAI
        {
            npc_amani_shi_flame_caster_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_FIREBALL, TIMER_FIREBALL);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIREBALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                DoCast(target, SPELL_FIREBALL_FC);
                            events.ScheduleEvent(EVENT_FIREBALL, TIMER_FIREBALL);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_amani_shi_flame_caster_horridonAI(creature);
        }
};

// Amani Warbear 69177
class npc_amani_warbear_horridon : public CreatureScript
{
    public:
        npc_amani_warbear_horridon() : CreatureScript("npc_amani_warbear_horridon") { }

        struct npc_amani_warbear_horridonAI : public ScriptedAI
        {
            npc_amani_warbear_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_SWIPE, TIMER_SWIPE);
            }

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void JustDied(Unit* /*killer*/)
            {
                me->SummonCreature(NPC_AMANI_SHI_BEAST_SHAMAN, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SWIPE:
                            DoCast(me, SPELL_WARBEAR_SWIPE);
                            events.ScheduleEvent(EVENT_SWIPE, TIMER_SWIPE);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_amani_warbear_horridonAI(creature);
        }
};

// Amani'shi Beast Shaman 69176
class npc_amani_shi_beast_shaman_horridon : public CreatureScript
{
    public:
        npc_amani_shi_beast_shaman_horridon() : CreatureScript("npc_amani_shi_beast_shaman_horridon") { }

        struct npc_amani_shi_beast_shaman_horridonAI : public ScriptedAI
        {
            npc_amani_shi_beast_shaman_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, TIMER_CHAIN_LIGHTNING);
                events.ScheduleEvent(EVENT_HEX_OF_CONFUSION, TIMER_HEX_OF_CONFUSION_F);
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA_TOTEM, TIMER_LIGHTNING_NOVA_TOTEM);
            }

            void JustSummoned(Creature* summon)
            {
				summon->setActive(true);

				if (me->isInCombat())
					summon->SetInCombatWithZone();
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHAIN_LIGHTNING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                                DoCast(target, SPELL_CHAIN_LIGHTNING);
                            events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, TIMER_CHAIN_LIGHTNING);
                            break;

                        case EVENT_HEX_OF_CONFUSION:
                            DoCast(me, SPELL_HEX_OF_CONFUSION);
                            events.ScheduleEvent(EVENT_HEX_OF_CONFUSION, TIMER_HEX_OF_CONFUSION_S);
                            break;

                        case EVENT_LIGHTNING_NOVA_TOTEM:
                            me->SummonCreature(NPC_LIGHTNING_NOVA_TOTEM, me->GetPositionX() + 5.0f, me->GetPositionY(), me->GetPositionZ() + 0.1f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                            events.ScheduleEvent(EVENT_LIGHTNING_NOVA_TOTEM, TIMER_LIGHTNING_NOVA_TOTEM);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_amani_shi_beast_shaman_horridonAI(creature);
        }
};

// Lightning Nova Totem 69215
class npc_lightning_nova_totem : public CreatureScript
{
    public:
        npc_lightning_nova_totem() : CreatureScript("npc_lightning_nova_totem") { }

        struct npc_lightning_nova_totem_AI : public ScriptedAI
        {
            npc_lightning_nova_totem_AI(Creature* creature) : ScriptedAI(creature) { }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                Reset();
                me->AddAura(SPELL_LIGHTNING_NOVA, me);
                me->DespawnOrUnsummon(60000);
            }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void UpdateAI(uint32 const diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_lightning_nova_totem_AI(creature);
        }
};

/*** Zandalari Dinomancer ***/

// Zandalari Dinomancer 69221
class npc_zandalari_dinomancer_horridon : public CreatureScript
{
    public:
        npc_zandalari_dinomancer_horridon() : CreatureScript("npc_zandalari_dinomancer_horridon") { }

        struct npc_zandalari_dinomancer_horridonAI : public ScriptedAI
        {
            npc_zandalari_dinomancer_horridonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool transformed;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                DoZoneInCombat(me, 100.0f);
            }

            void Reset()
            {
                events.Reset();
                transformed = false;
                if (me->GetPositionZ() > 160.0f)
                    me->SetReactState(REACT_PASSIVE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                events.ScheduleEvent(EVENT_DINO_MENDING, TIMER_DINO_MENDING_F);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_MENDING_REMOVED:
                        events.CancelEvent(EVENT_DINO_MENDING);
                        events.ScheduleEvent(EVENT_DINO_MENDING, TIMER_DINO_MENDING_I);
                        break;

                    default: break;
                }
            };

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (!transformed && me->HealthBelowPct(51))
                {
                    if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                    {
                        switch (CAST_AI(boss_horridon::boss_horridonAI, Horridon->AI())->phase)
                        {
                            case PHASE_FARRAKI:
                                me->getVictim()->SummonGameObject(GO_ORB_OF_CONTROL_FARRAKI, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY, 0);
                                break;

                            case PHASE_GURUBASHI:
                                me->getVictim()->SummonGameObject(GO_ORB_OF_CONTROL_GURUBASHI, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY, 0);
                                break;

                            case PHASE_DRAKKARI:
                                me->getVictim()->SummonGameObject(GO_ORB_OF_CONTROL_DRAKKARI, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY, 0);
                                break;

                            case PHASE_AMANI:
                                me->getVictim()->SummonGameObject(GO_ORB_OF_CONTROL_AMANI, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, RESPAWN_IMMEDIATELY, 0);
                                break;

                            default: break;
                        }
                    }

                    DoCast(me, SPELL_DINO_FORM);
                    transformed = true;
                }

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DINO_MENDING:
                            if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                                DoCast(Horridon, SPELL_DINO_MENDING);
                            events.ScheduleEvent(EVENT_DINO_MENDING, TIMER_DINO_MENDING_S);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_zandalari_dinomancer_horridonAI(creature);
        }
};

/*** War-God Jalak ***/

class npc_war_god_jalak : public CreatureScript
{
    public:
        npc_war_god_jalak() : CreatureScript("npc_war_god_jalak") { }

        struct npc_war_god_jalakAI : public ScriptedAI
        {
            npc_war_god_jalakAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool introDone, engaged;

            void IsSummonedBy(Unit* summoner)
            {
                Reset();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }

            void Reset()
            {
                events.Reset();
                introDone = false;
                engaged = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
                events.ScheduleEvent(EVENT_BESTIAL_CRY, TIMER_BESTIAL_CRY_F);
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_START_INTRO:
                        events.ScheduleEvent(EVENT_INTRO_1, TIMER_INTRO_1);
                        break;

                    case ACTION_JUMP_AND_ENGAGE:
                        events.ScheduleEvent(EVENT_JALAK_JUMP, TIMER_JUMP);
                        break;

                    default: break;
                }
            };

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    Talk(SAY_KILL);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);
                if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                    Horridon->AddAura(SPELL_RAMPAGE, Horridon);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim() && introDone && engaged || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_1:
                            Talk(SAY_INTRO_1);
                            events.ScheduleEvent(EVENT_INTRO_2, TIMER_INTRO_2);
                            break;

                        case EVENT_INTRO_2:
                            Talk(SAY_INTRO_2);
                            events.ScheduleEvent(EVENT_INTRO_3, TIMER_INTRO_3);
                            break;

                        case EVENT_INTRO_3:
                            Talk(SAY_INTRO_3);
                            // Break Horridon door and send him to mid.
                            if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                                if (GameObject* door = Horridon->FindNearestGameObject(GO_START_DOOR, 500.0f))
                                    door->UseDoorOrButton(0);
                            events.ScheduleEvent(EVENT_HORRIDON_MOVE, TIMER_HORRIDON_MOVE);
                            break;

                        case EVENT_HORRIDON_MOVE:
                            // Send Horridon to mid.
                            if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                            {
								Horridon->SetHomePosition(midPos.GetPositionX(), midPos.GetPositionY(), midPos.GetPositionZ(), 0);
                                Horridon->GetMotionMaster()->MoveCharge(midPos.GetPositionX(), midPos.GetPositionY(), midPos.GetPositionZ());
                            }
                            events.ScheduleEvent(EVENT_HORRIDON_ENGAGE, TIMER_HORRIDON_ENGAGE);
                            break;

                        case EVENT_HORRIDON_ENGAGE:
                            // Engage Horridon in combat.
                            if (Creature* Horridon = me->FindNearestCreature(BOSS_HORRIDON, 500.0f, true))
                            {
                                Horridon->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                                Horridon->SetReactState(REACT_AGGRESSIVE);
                                Horridon->AI()->DoZoneInCombat(Horridon, 200.0f);
                            }
                            introDone = true;
                            break;

                        case EVENT_JALAK_JUMP:
                            me->GetMotionMaster()->MoveJump(midPos.GetPositionX(), midPos.GetPositionY(), midPos.GetPositionZ(), 15.0f, 15.0f);
                            events.ScheduleEvent(EVENT_ENGAGE, TIMER_ENGAGE);
                            break;

                        case EVENT_ENGAGE:
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetReactState(REACT_AGGRESSIVE);
                            DoZoneInCombat(me, 200.0f);
                            engaged = true;
                            break;

                        case EVENT_BESTIAL_CRY:
                            if (!engaged)
                                break;

                            DoCast(me, SPELL_BESTIAL_CRY);
                            events.ScheduleEvent(EVENT_BESTIAL_CRY, TIMER_BESTIAL_CRY_S);
                            break;

                        default: break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_war_god_jalakAI(creature);
        }
};

/*** Spells ***/

// Rending Charge - 136653
class spell_rending_charge_horridon : public SpellScriptLoader
{
    public:
        spell_rending_charge_horridon() : SpellScriptLoader("spell_rending_charge_horridon") { }

        class spell_rending_charge_horridon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rending_charge_horridon_SpellScript);

            void HandleCharge(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_RENDING_CHARGE_DOT, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rending_charge_horridon_SpellScript::HandleCharge, EFFECT_0, SPELL_EFFECT_CHARGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rending_charge_horridon_SpellScript();
        }
};

// Venom Bolt Volley 136587
class spell_venom_bolt_volley_horridon : public SpellScriptLoader
{
    public:
        spell_venom_bolt_volley_horridon() : SpellScriptLoader("spell_venom_bolt_volley_horridon") { }

        class spell_venom_bolt_volley_horridon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_venom_bolt_volley_horridon_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (!(mode & AURA_EFFECT_HANDLE_REAL))
                    return;

                if (GetOwner()->GetTypeId() != TYPEID_UNIT)
                    return;

                Unit* caster = GetCaster();

                if (!caster || !GetTargetApplication())
                    return;

                if (!caster->ToCreature())
                    return;

                if (caster->ToCreature()->GetEntry() != NPC_GURUBASHI_VENOM_PRIEST)
                    return;

                // Only on removal by interrupt.
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    return;
 
                caster->ToCreature()->AI()->DoAction(ACTION_VENOM_VOLLEY_REMOVED);
            }

            void Register() 
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_venom_bolt_volley_horridon_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const 
        {
            return new spell_venom_bolt_volley_horridon_AuraScript();
        }
};

// Dino-Mending 136797
class spell_dino_mending_horridon : public SpellScriptLoader
{
    public:
        spell_dino_mending_horridon() : SpellScriptLoader("spell_dino_mending_horridon") { }

        class spell_dino_mending_horridon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dino_mending_horridon_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (!(mode & AURA_EFFECT_HANDLE_REAL))
                    return;

                if (GetOwner()->GetTypeId() != TYPEID_UNIT)
                    return;

                Unit* caster = GetCaster();

                if (!caster || !GetTargetApplication())
                    return;

                // Only on removal by interrupt.
                if (GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_EXPIRE || GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    return;
 
                caster->ToCreature()->AI()->DoAction(ACTION_MENDING_REMOVED);
            }

            void Register() 
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dino_mending_horridon_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_OBS_MOD_HEALTH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const 
        {
            return new spell_dino_mending_horridon_AuraScript();
        }
};

void AddSC_boss_horridon()
{
    new boss_horridon();

    new npc_direhorn_spirit_horridon();
    new npc_farraki_skirmisher_amani_protector();
    new npc_sul_lithuz_stonegazer();
    new npc_farraki_wastewalker();
    new npc_sand_trap();
    new npc_gurubashi_bloodlord();
    new npc_gurubashi_venom_priest();
    new npc_venomous_effusion_horridon();
    new npc_living_poison_horridon();
    new npc_risen_drakkari_warrior_champion();
    new npc_drakkari_frozen_warlord();
    new npc_frozen_orb_horridon();
    new npc_amani_shi_flame_caster_horridon();
    new npc_amani_warbear_horridon();
    new npc_amani_shi_beast_shaman_horridon();
    new npc_lightning_nova_totem();
    new npc_zandalari_dinomancer_horridon();
    new npc_war_god_jalak();

    new spell_rending_charge_horridon();
    new spell_venom_bolt_volley_horridon();
    new spell_dino_mending_horridon();
}