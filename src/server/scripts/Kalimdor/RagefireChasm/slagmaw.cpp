 /*
Intro isn't scripted
*/
/*
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
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
#include "ScriptedCreature.h"
#include "ScriptPCH.h"
#include "ScriptedEscortAI.h"

enum eBosses
{
	BOSS_SLAGMAW,
};

enum eSpells
{
	SPELL_LAVA_SPLIT = 119434, // 1
};

enum eEvents
{
	EVENT_LAVA_SPLIT = 1,
	EVENT_CHANGE_POSITION1 = 2,
	EVENT_CHANGE_POSITION2 = 3,
	EVENT_CHANGE_POSITION3 = 4,
	EVENT_CHANGE_POSITION4 = 5,
};

enum eTexts
{
	TALK_AGGRO,
	TALK_DEATH,
	TALK_ARCING_SMASH,
	TALK_KILL,
	TALK_INFERNO_CHARGE,
};

class boss_slagmaw : public CreatureScript
{
public:
	boss_slagmaw() : CreatureScript("boss_slagmaw") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_slagmaw_AI(creature);
	}

	struct boss_slagmaw_AI : public BossAI
	{
		boss_slagmaw_AI(Creature* creature) : BossAI(creature, BOSS_SLAGMAW)
		{}
		void Reset()
		{
			me->SetReactState(REACT_AGGRESSIVE);
			events.Reset();
			_Reset();
		}

		void KilledUnit(Unit* u)
		{
			Talk(TALK_KILL);
		}


		void EnterCombat(Unit* unit)
		{
			events.ScheduleEvent(EVENT_LAVA_SPLIT, 0);
			events.ScheduleEvent(EVENT_CHANGE_POSITION1, 20500);
			Talk(TALK_AGGRO);
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

				case EVENT_CHANGE_POSITION1:
				{
					me->NearTeleportTo(-259.88f, 137.62f, -19.17f, 0.6f);
					events.ScheduleEvent(EVENT_CHANGE_POSITION2, 9000);
					break;
				}
				case EVENT_CHANGE_POSITION2:
				{
					me->NearTeleportTo(-229.58f, 134.22f, -19.17f, 2.3f);
					events.ScheduleEvent(EVENT_CHANGE_POSITION3, 9000);
					break;
				}
				case EVENT_CHANGE_POSITION3:
				{
					me->NearTeleportTo(-226.34f, 166.0f, -19.71f, 3.54f);
					events.ScheduleEvent(EVENT_CHANGE_POSITION4, 9000);
					break;
				}
				case EVENT_CHANGE_POSITION4:
				{
					me->NearTeleportTo(-258.17f, 172.41f,-19.63f, 5.58f);
					events.ScheduleEvent(EVENT_CHANGE_POSITION1, 9000);
					break;
				}
			  }
			}
			DoMeleeAttackIfReady();
		}
	};
};

void AddSC_boss_slagmaw()
{
	new boss_slagmaw();
}