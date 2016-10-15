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
#include "throne_of_thunder.h"

class instance_throne_of_thunder : public InstanceMapScript
{
    public:
        instance_throne_of_thunder() : InstanceMapScript("instance_throne_of_thunder", 1098) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_throne_of_thunder_map_script(map);
        }

        struct instance_throne_of_thunder_map_script : public InstanceScript
        {
            instance_throne_of_thunder_map_script(Map* map) : InstanceScript(map) { }

            InstanceScript* Pinstance = this;

            uint64 jinrokhguid;
			uint64 horridonGuid;
            uint64 sulGuid;
            uint64 malakGuid;
            uint64 garjalGuid;
            uint64 karazGuid;
            uint64 marliGuid;
            uint64 tortosGuid;
            uint64 megaeraGuid;
            uint64 jiKunGuid;
			uint64 horridonDoorsGuid[4];

            // doors
            uint64 jinrokhentrance;
            uint64 jinrokhhorridon;
            uint64 horridonentrance;
            uint64 horridoncouncil;
            uint64 counciltortos;

            void Initialize()
            {
                SetBossNumber(12);
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case 694655:
                    case BOSS_JINROKH_BREAKER:
                        jinrokhguid = creature->GetGUID();
                        break;
                    case 684766:
					case BOSS_HORRIDON:
						horridonGuid = creature->GetGUID();
						break;
                    case 691311:
                    case BOSS_MALAK:
                        malakGuid = creature->GetGUID();
                        break;
                    case 691344:
                    case BOSS_KAZARJIN:
                        karazGuid = creature->GetGUID();
                        break;
                    case 691322:
                    case BOSS_MARLI:
                        marliGuid = creature->GetGUID();
                        break;
                    case 690788:
                    case BOSS_SUL:
                        sulGuid = creature->GetGUID();
                        break;
                    case CREATURE_GARJAL:
                        garjalGuid = creature->GetGUID();
                        break;
                    case 679777:
                    case BOSS_TORTOS:
                        tortosGuid = creature->GetGUID();
                        break;
                    case BOSS_MEGAERA:
                        megaeraGuid = creature->GetGUID();
                        break;
                    case 697122:
                    case BOSS_JI_KUN:
                        jiKunGuid = creature->GetGUID();
                        break;
					default:
                        break;
                }
            }

            void OnUnitDeath(Unit* unit)
            {
                Creature* creature = unit->ToCreature();
                if (!creature)
                    return;

                switch (creature->GetEntry())
                {
                    case BOSS_JINROKH_BREAKER:
                        if (GameObject* JinrokhHorridonDoor = instance->GetGameObject(Pinstance->GetData64(DATA_JINROKH_HORRIDON_DOOR)))
                        {
                            if (GameObject* JinrokhDoor = instance->GetGameObject(Pinstance->GetData64(DATA_JINROKH_DOOR)))
                            {
                                JinrokhHorridonDoor->Delete();
                                JinrokhDoor->Delete();
                            }
                        }
                        break;
                    case BOSS_HORRIDON:
                        if (GameObject* HorridonDoor = instance->GetGameObject(Pinstance->GetData64(DATA_HORRIDON_DOOR)))
                        {
                            if (GameObject* CouncilDoor = instance->GetGameObject(Pinstance->GetData64(DATA_HORRIDON_COUNCIL_DOOR)))
                            {
                                HorridonDoor->Delete();
                                CouncilDoor->Delete();
                            }
                        }
                        break;
                    case CREATURE_GARJAL:
                        if (GameObject* CouncilTortos = instance->GetGameObject(Pinstance->GetData64(DATA_COUNCIL_TORTOS_DOOR)))
                        {
                            CouncilTortos->Delete();
                        }
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
				    case OBJECT_GURUBASHI_DOOR:
					    horridonDoorsGuid[0] = go->GetGUID();
					    break;
				    case OBJECT_DRAKKARI_DOOR:
					    horridonDoorsGuid[1] = go->GetGUID();
					    break;
				    case OBJECT_FARAKKI_DOOR:
					    horridonDoorsGuid[2] = go->GetGUID();
					    break;
				    case OBJECT_AMANI_DOOR:
					    horridonDoorsGuid[3] = go->GetGUID();
					    break;
                    case GO_JIN_ROKH_FRONT_DOOR:
                        jinrokhentrance = go->GetGUID();
                        break;
                    case GO_JIN_ROKH_BACK_DOOR:
                        jinrokhhorridon = go->GetGUID();
                        break;
                    case OBJECT_HORRIDON_ENTRACE:
                        horridonentrance = go->GetGUID();
                        break;
                    case OBJECT_HORRIDON_COUNCIL:
                        horridoncouncil = go->GetGUID();
                        break;
                    case OBJECT_COUNCIL_TORTOS:
                        counciltortos = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case DATA_JIN_ROKH_BREAKER_EVENT:
                        return jinrokhguid;
                        break;
				    case DATA_HORRIDON_EVENT:
					    return horridonGuid;
                        break;
                    case DATA_JINROKH_DOOR:
                        return jinrokhentrance;
                        break;
                    case DATA_JINROKH_HORRIDON_DOOR:
                        return jinrokhhorridon;
                        break;
                    case DATA_HORRIDON_DOOR:
                        return horridonentrance;
                        break;
                    case DATA_HORRIDON_COUNCIL_DOOR:
                        return horridoncouncil;
                        break;
                    case DATA_TORTOS:
                        return tortosGuid;
                        break;
				    case DATA_GURU_DOOR:
				    case DATA_DRAK_DOOR:
				    case DATA_FARA_DOOR:
				    case DATA_AMANI_DOOR:
					    return horridonDoorsGuid[type - 2]; //MANY LAZY
					    break;
                    case DATA_GARAJAL:
                        return garjalGuid;
                        break;
                    case DATA_MALAK:
                        return malakGuid;
                        break;
                    case DATA_KARAZJIN:
                        return karazGuid;
                        break;
                    case DATA_HIGH_PRIESTSESS:
                        return marliGuid;
                        break;
                    case DATA_SAND_CRAWLER:
                        return sulGuid;
                        break;
                    case DATA_MEGAERA:
                        return megaeraGuid;
                        break;
                    case DATA_JI_KUN:
                        return jiKunGuid;
                        break;
				    default:
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

void AddSC_instance_throne_of_thunder()
{
	new instance_throne_of_thunder();
}
