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
#include "GameObjectAI.h"

enum eBosses
{
	BOSS_LAVA_GUARD_GORDOTH,
};

enum eSpells
{
	SPELL_GROUND_RUPTURE = 119999, // 1
	SPELL_SEISMIC_SLAM = 120024, // 2
};

enum eEvents
{
	EVENT_GROUND_RUPTURE = 1,
	EVENT_SEISMIC_SLAM = 2,
};

enum eTexts
{
	TALK_AGGRO,
	TALK_INTRO,
	TALK_DEATH,
	TALK_ARCING_SMASH,
	TALK_KILL,
	TALK_INFERNO_CHARGE,
};

class boss_lavaguardgordoth : public CreatureScript
{
public:
	boss_lavaguardgordoth() : CreatureScript("boss_adarogg") { }

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_lavaguardgordoth_AI(creature);
	}

	struct boss_lavaguardgordoth_AI : public BossAI
	{
		boss_lavaguardgordoth_AI(Creature* creature) : BossAI(creature, BOSS_LAVA_GUARD_GORDOTH)
		{
			intro = false;
		}
		bool intro;
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


		void MoveInLineOfSight(Unit* who)
		{
			if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && !intro && me->IsWithinDistInMap(who, 90.0f))
			{
				Talk(TALK_INTRO);
				intro = true;
				me->GetMotionMaster()->MoveJump(-369.5f, 166.21f, -18.52f, 5.0f, 5, 1.312f);
				DoCastVictim(120024);
			}
		}


		void EnterCombat(Unit* unit)
		{
			events.ScheduleEvent(EVENT_GROUND_RUPTURE, 15500);
			events.ScheduleEvent(EVENT_SEISMIC_SLAM, 20500);
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

				case EVENT_GROUND_RUPTURE:
				{
					me->CastSpell(me, SPELL_GROUND_RUPTURE, true);
					events.ScheduleEvent(EVENT_GROUND_RUPTURE, 12500);
					break;
				}
				case EVENT_SEISMIC_SLAM:
				{
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 50.0f, true))
						DoCast(target, SPELL_SEISMIC_SLAM);
					events.ScheduleEvent(EVENT_SEISMIC_SLAM, 20000);
					Talk(TALK_INFERNO_CHARGE);
					break;
				}
			  }
			}
			DoMeleeAttackIfReady();
		}
	};
};

void AddSC_lavaguardgordoth()
{
	new boss_lavaguardgordoth();
}