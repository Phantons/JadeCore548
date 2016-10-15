/*
* Copyright (C) 2008-20XX JadeCore <http://www.pandashan.com>
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
#include "InstanceScript.h"
#include "VMapFactory.h"
#include "siege_of_orgrimmar.h"

class instance_siege_of_orgrimmar : public InstanceMapScript
{
    public:
        instance_siege_of_orgrimmar() : InstanceMapScript("instance_siege_of_orgrimmar", 1136) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_instance_siege_of_orgrimmar_map(map);
        }

        struct instance_instance_siege_of_orgrimmar_map : public InstanceScript
        {
            instance_instance_siege_of_orgrimmar_map(Map* map) : InstanceScript(map) { }

            InstanceScript* Pinstance = this;

			// Immerseus
            uint64 ImmersusGuid;
            uint64 ImmersusBackDoorGuid;
            uint64 ImmersusFrontDoorGuid;

			// Fallen Protectors
            uint64 StoneGuid;
            uint64 HuGuid;
            uint64 SunGuid;

            // Norushen   
            uint64 NorushenBossGuid;
            uint64 NorushenChoGuid;
            uint64 NorushenAlmalgamOfCorruptionGuid;
            uint64 NorushenTriggerGuid;
            uint64 NorushenEntranceGuid;
            uint64 PrideEntranceGuid;

            // Sha of Pride
            uint64 NorushenGuid;
            uint64 ShaOfPrideGuid;
            uint64 North01Guid;
            uint64 North02Guid;
            uint64 North03Guid;
            uint64 South01Guid;
            uint64 South02Guid;
            uint64 South03Guid;
            uint64 West01Guid;
            uint64 West02Guid;
            uint64 West03Guid;
            uint64 East01Guid;
            uint64 East02Guid;
            uint64 East03Guid;

			// Galakras

			// Iron Juggernaut
			uint64 JuggernautGuid;

			// Kor'kron Dark Shamans
			uint64 HarommGuid;
			uint64 KardrisGuid;

			// General Nazgrim
			uint64 NazgrimGuid;

			// Malkorok

			// Spoils of Pandaria

			// Thok the Bloodthirsty

			// Siegecrafter Blackfuse

			// Paragons of the Klaxxi

			// Garrosh Hellscream
            uint64 GarroshGuid;
            uint64 GarroshThrallGuid;
            uint64 GarroshHeartOfYshaaraj;
            uint64 GarroshRightDoor;
            uint64 GarroshLeftDoor;

			// Lorewalker Cho
            uint64 Cho01Guid;

            void Initialize()
            {
				// Immerseus
                ImmersusGuid						= 0;
                ImmersusBackDoorGuid				= 0;
                ImmersusFrontDoorGuid				= 0;

                // Fallen Protectors
                StoneGuid							= 0;
                HuGuid								= 0;
                SunGuid								= 0;

                // Norushen
                NorushenBossGuid					= 0;
                NorushenChoGuid						= 0;
                NorushenAlmalgamOfCorruptionGuid	= 0;
                NorushenTriggerGuid					= 0;
                PrideEntranceGuid					= 0;
                NorushenEntranceGuid				= 0;

                // Sha of Pride
                ShaOfPrideGuid						= 0;
                North01Guid							= 0;
                North02Guid							= 0;
                North03Guid							= 0;
                South01Guid							= 0;
                South02Guid							= 0;
                South03Guid							= 0;
                West01Guid							= 0;
                West02Guid							= 0;
                West03Guid							= 0;
                East01Guid							= 0;
                East02Guid							= 0;
                East03Guid							= 0;

				// Galakras

				// Iron Juggernaut
				JuggernautGuid						= 0;

				// Kor'kron Dark Shamans
				HarommGuid							= 0;
				KardrisGuid							= 0;

				// General Nazgrim
				NazgrimGuid							= 0;

				// Malkorok

				// Spoils of Pandaria

				// Thok the Bloodthirsty

				// Siegecrafter Blackfuse

				// Paragons of the Klaxxi

				// Garrosh Hellscream
                GarroshGuid							= 0;
                GarroshThrallGuid					= 0;
                GarroshHeartOfYshaaraj				= 0;
                GarroshRightDoor					= 0;
                GarroshRightDoor					= 0;

				// Lorewalker Cho
                Cho01Guid							= 0;

                SetBossNumber(14);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
					// Immerseus
                    case BOSS_IMMERSUS:
                    case 724360:
                    case 724366:
                    case 724367:
                    case 724368:
                        ImmersusGuid = creature->GetGUID();
                        break;

					// Fallen Protectors
                    case BOSS_STONE:
                    case 714750:
                    case 714751:
                        StoneGuid = creature->GetGUID();
                        break;
                    case BOSS_HU:
                    case 714790:
                    case 714791:
                        HuGuid = creature->GetGUID();
                        break;
                    case BOSS_SUN:
                    case 714800:
                    case 714801:
                        SunGuid = creature->GetGUID();
                        break;

                        // Norushen
                    case BOSS_NORUSHEN:
                        NorushenBossGuid = creature->GetGUID();
                        break;
                    case CREATURE_ALMAGLEM_OF_CORRUPTION:
                        NorushenAlmalgamOfCorruptionGuid = creature->GetGUID();
                        break;
                    case CREATURE_CHO_NORUSHEN:
                        NorushenChoGuid = creature->GetGUID();
                        break;
                    case CREATURE_TRIGGER:
                        NorushenTriggerGuid = creature->GetGUID();
                        break;
                    case CREATURE_NORUSHEN:
                        NorushenGuid = creature->GetGUID();
                        break;

					// Sha of Pride
                    case BOSS_SHA_OF_PRIDE:
                    case 475683:
                    case 475684:
                        ShaOfPrideGuid = creature->GetGUID();
                        break;

					// Galakras

					// Iron Juggrenaut
					case BOSS_IRON_JUGGERNAUT:
						JuggernautGuid = creature->GetGUID();
						break;

					// Kor'kron Dark Shamans
					case BOSS_EARTHBREAKER_HAROMM:
						HarommGuid = creature->GetGUID();
						break;
					case BOSS_WAVEBINDER_KARDRIS:
						KardrisGuid = creature->GetGUID();
						break;
					// General Nazgrim
					case BOSS_GENERAL_NAZGRIM:
						NazgrimGuid = creature->GetGUID();
						break;
					// Malkorok

					// Spoils of Pandaria

					// Thok the Bloodthirsty

					// Siegecrafter Blackfuse

					// Paragons of the Klaxxi

					// Garrosh Hellscream
                    case BOSS_GARROSH_HELLSCREAM:
                        GarroshGuid = creature->GetGUID();
                        break;
                    case CREATURE_HEART_OF_YSHAARAJ_MAIN:
                        GarroshHeartOfYshaaraj = creature->GetGUID();
                        break;
                    case CREATURE_THRALL:
                        GarroshThrallGuid = creature->GetGUID();
                        break;

					// Lorewalker Cho
                    case CREATURE_CHO01:
                        Cho01Guid = creature->GetGUID();
                        break;
                }
            }

            void OnUnitDeath(Unit* unit)
            {
                Creature* creature = unit->ToCreature();
                if (!creature)
                    return;

                // switch (creature->GetEntry())
                // {
                // }
            }
            
            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GAMEOBJECT_PRISON_NORTH_01:
                    {
                        North01Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 772.099f, 1088.094f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 0);
                        break;
                    }
                    case GAMEOBJECT_PRISON_NORTH_02:
                    {
                        North02Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 781.142f, 1100.392f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);
                    
                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 1);
                        break;
                    }
                    case GAMEOBJECT_PRISON_NORTH_03:
                    {
                        North03Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 765.644f, 1101.382f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 2);
                        break;
                    }
                    case GAMEOBJECT_PRISON_SOUTH_01:
                    {
                        South01Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 730.769f, 1124.302f, 356.073f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 3);
                        break;
                    }
                    case GAMEOBJECT_PRISON_SOUTH_02:
                    {
                        South02Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 715.333f, 1124.915f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 4);
                        break;
                    }
                    case GAMEOBJECT_PRISON_SOUTH_03:
                    {
                        South03Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 724.410f, 1137.356f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 5);
                        break;
                    }
                    case GAMEOBJECT_PRISON_WEST_01:
                    {
                        West01Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 760.158f, 1145.122f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 6);
                        break;
                    }
                    case GAMEOBJECT_PRISON_WEST_02:
                    {
                        West02Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 772.734f, 1137.248f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 7);
                        break;
                    }
                    case GAMEOBJECT_PRISON_WEST_03:
                    {
                        West03Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 759.798f, 1130.546f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 8);
                        break;
                    }
                    case GAMEOBJECT_PRISON_EAST_01:
                    {
                        East01Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 723.364f, 1088.186f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 9);
                        break;
                    }
                    case GAMEOBJECT_PRISON_EAST_02:
                    {
                        East02Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 735.883f, 1080.281f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 10);
                        break;
                    }
                    case GAMEOBJECT_PRISON_EAST_03:
                    {
                        East03Guid = go->GetGUID();

                        Creature* Switcher = go->SummonCreature(CREATURE_PRISON_TRIGGER_SWITCHERS, 736.386f, 1095.060f, 356.072f, 0, TEMPSUMMON_MANUAL_DESPAWN);

                        if (Switcher && Switcher->GetAI())
                            Switcher->GetAI()->SetGUID(go->GetGUID(), 11);
                        break;
                    }
                    case GAMEOBJECT_RIGHT_DOOR:
                    {
                        GarroshRightDoor = go->GetGUID();                      
                        break;
                    }
                    case GAMEOBJECT_LEFT_DOOR:
                    {
                        GarroshLeftDoor = go->GetGUID();
                        break;
                    }
                    /// Norushen + Pride Entrance
                    case GAMEOBJECT_NORUSHEN_ENTRANCE:
                    {
                        NorushenEntranceGuid = go->GetGUID();
                        break;
                    }
                    case GAMEOBJECT_PRIDE_ENTRNANCE:
                    {
                        PrideEntranceGuid = go->GetGUID();
                        break;
                    }

                default:
                    break;
                }
            }
            
            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
					// Immerseus
					case DATA_IMMERSUS:
						return ImmersusGuid;
						break;


					// Fallen Protectors
					case DATA_STONE:
						return StoneGuid;
						break;
					case DATA_HU:
						return HuGuid;
						break;
					case DATA_SUN:
						return SunGuid;
						break;

						// Norushen
					case DATA_NORUSHEN_BOSS:
						return NorushenBossGuid;
						break;
					case DATA_NORUSHEN_CHO:
						return NorushenChoGuid;
						break;
					case DATA_NORUSHEN_TRIGGER:
						return NorushenTriggerGuid;
						break;
					case DATA_NORUSHEN_ALMAGLEM_OF_CORRUPTION:
						return NorushenAlmalgamOfCorruptionGuid;
						break;
					case DATA_NORUSHEN_ENTRANCE:
						return NorushenEntranceGuid;
						break;
					case DATA_NORUSHEN:
						return NorushenGuid;
						break;

					// Sha of Pride
					case DATA_SHA_OF_PRIDE:
						return ShaOfPrideGuid;
						break;
					case DATA_PRIDE_ENTRANCE:
						return PrideEntranceGuid;
						break;
					case DATA_SOUTH01:
						return South01Guid;
						break;
					case DATA_SOUTH02:
						return South02Guid;
						break;
					case DATA_SOUTH03:
						return South03Guid;
						break;
					case DATA_WEST01:
						return West01Guid;
						break;
					case DATA_WEST02:
						return West02Guid;
						break;
					case DATA_WEST03:
						return West03Guid;
						break;
					case DATA_EAST01:
						return East01Guid;
						break;
					case DATA_EAST02:
						return East02Guid;
						break;
					case DATA_EAST03:
						return East03Guid;
						break;
					case DATA_NORTH01:
						return North01Guid;
						break;
					case DATA_NORTH02:
						return North02Guid;
						break;
					case DATA_NORTH03:
						return North03Guid;
						break;

					// Iron Juggernaut
					case DATA_IRON_JUGGERNAUT:
						return JuggernautGuid;
						break;

					// Kor'kron Dark Shamans
					case DATA_EARTHBREAKER_HAROMM:
						return HarommGuid;
						break;
					case DATA_WAVEBINDER_KARDRIS:
						return KardrisGuid;
						break;

					// General Nazgrim
					case DATA_GENERAL_NAZGRIM:
						return NazgrimGuid;
						break;

					// Malkorok

					// Spoils of Pandaria

					// Thok the Bloodthirsty

					// Siegecrafter Blackfuse

					// Paragons of the Klaxxi

					// Garrosh Hellscream
					case DATA_GARROSH_HEART_OF_YSHAARAJ:
						return GarroshHeartOfYshaaraj;
						break;
					case DATA_GARROSH_HELLSCREAM:
						return GarroshGuid;
						break;
					case DATA_THRALL:
						return GarroshThrallGuid;
						break;
					case DATA_LEFT_DOOR_GARROSH:
						return GarroshLeftDoor;
						break;
					case DATA_RIGHT_DOOR_GARROSH:
						return GarroshRightDoor;
						break;

					// Lorewalker Cho
					case DATA_LORE_WALKER_CHO_01:
						return Cho01Guid;
						break;
                }

                return 0;
            }
            bool IsWipe()
            {
                Map::PlayerList const& PlayerList = instance->GetPlayers();

                if (PlayerList.isEmpty())
                    return true;

                return false;
            }
        };
};

void AddSC_instance_siege_of_orgrimmar()
{
	new instance_siege_of_orgrimmar();
}
