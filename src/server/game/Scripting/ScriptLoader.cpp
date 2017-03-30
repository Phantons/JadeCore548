/*
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
*/

#include "AnticheatMgr.h"
#include "ScriptLoader.h"

//examples
void AddSC_example_creature();
void AddSC_example_escort();
void AddSC_example_gossip_codebox();
void AddSC_example_misc();
void AddSC_example_commandscript();

// Customs


// spells
void AddSC_deathknight_spell_scripts();
void AddSC_druid_spell_scripts();
void AddSC_generic_spell_scripts();
void AddSC_hunter_spell_scripts();
void AddSC_mage_spell_scripts();
void AddSC_paladin_spell_scripts();
void AddSC_priest_spell_scripts();
void AddSC_rogue_spell_scripts();
void AddSC_shaman_spell_scripts();
void AddSC_warlock_spell_scripts();
void AddSC_warrior_spell_scripts();
void AddSC_monk_spell_scripts();
void AddSC_mastery_spell_scripts();
void AddSC_quest_spell_scripts();
void AddSC_item_spell_scripts();
void AddSC_example_spell_scripts();
void AddSC_holiday_spell_scripts();

// Pets
void AddSC_deathknight_pet_scripts();

void AddSC_SmartSCripts();

//Commands
void AddSC_anticheat_commandscript();
void AddSC_account_commandscript();
void AddSC_achievement_commandscript();
void AddSC_ban_commandscript();
void AddSC_bf_commandscript();
void AddSC_cast_commandscript();
void AddSC_character_commandscript();
void AddSC_cheat_commandscript();
void AddSC_debug_commandscript();
void AddSC_disable_commandscript();
void AddSC_event_commandscript();
void AddSC_gm_commandscript();
void AddSC_go_commandscript();
void AddSC_gobject_commandscript();
void AddSC_guild_commandscript();
void AddSC_honor_commandscript();
void AddSC_instance_commandscript();
void AddSC_learn_commandscript();
void AddSC_list_commandscript();
void AddSC_lookup_commandscript();
void AddSC_message_commandscript();
void AddSC_misc_commandscript();
void AddSC_modify_commandscript();
void AddSC_npc_commandscript();
void AddSC_quest_commandscript();
void AddSC_reload_commandscript();
void AddSC_reset_commandscript();
void AddSC_server_commandscript();
void AddSC_tele_commandscript();
void AddSC_ticket_commandscript();
void AddSC_titles_commandscript();
void AddSC_wp_commandscript();

#ifdef SCRIPTS
//world
void AddSC_areatrigger_scripts();
void AddSC_generic_creature();
void AddSC_go_scripts();
void AddSC_guards();
void AddSC_item_scripts();
void AddSC_npc_professions();
void AddSC_npc_innkeeper();
void AddSC_npcs_special();
void AddSC_npc_taxi();
void AddSC_npc_companions();
void AddSC_achievement_scripts();
void AddSC_mine_cart();

// Outdoor PvP
void AddSC_outdoorpvp_hp();
void AddSC_outdoorpvp_na();
void AddSC_outdoorpvp_si();
void AddSC_outdoorpvp_tf();
void AddSC_outdoorpvp_zm();

// Player
void AddSC_chat_log();

/*

* World of Warcraft Scripts

*/

// Classic Dungeons - Alterac Valley
void AddSC_alterac_valley();
void AddSC_boss_balinda();
void AddSC_boss_drekthar();
void AddSC_boss_galvangar();
void AddSC_boss_vanndar();

// Classic Dungeons - Blackrock Depths
void AddSC_blackrock_depths();
void AddSC_boss_ambassador_flamelash();
void AddSC_boss_anubshiah();
void AddSC_boss_draganthaurissan();
void AddSC_boss_general_angerforge();
void AddSC_boss_gorosh_the_dervish();
void AddSC_boss_grizzle();
void AddSC_boss_high_interrogator_gerstahn();
void AddSC_boss_magmus();
void AddSC_boss_moira_bronzebeard();
void AddSC_boss_tomb_of_seven();
void AddSC_instance_blackrock_depths();
void AddSC_blackrock_spire();

// Classic Dungeons - Blackrock Spire
void AddSC_boss_drakkisath();
void AddSC_boss_halycon();
void AddSC_boss_highlordomokk();
void AddSC_boss_mothersmolderweb();
void AddSC_boss_overlordwyrmthalak();
void AddSC_boss_shadowvosh();
void AddSC_boss_thebeast();
void AddSC_boss_warmastervoone();
void AddSC_boss_quatermasterzigris();
void AddSC_boss_pyroguard_emberseer();
void AddSC_boss_gyth();
void AddSC_boss_rend_blackhand();
void AddSC_instance_blackrock_spire();

// Classic Dungeons - Deadmines
void AddSC_instance_deadmines();
void AddSC_deadmines();
void AddSC_boss_glubtok();
void AddSC_boss_helix_gearbreaker();
void AddSC_boss_foereaper5000();
void AddSC_boss_admiral_ripsnarl();
void AddSC_boss_captain_cookie();
void AddSC_boss_vanessa_vancleef();

// Classic Dungeons - Gnomergan
void AddSC_gnomeregan();
void AddSC_instance_gnomeregan();

// Classic Dungeons - Scarlet Enclave
void AddSC_the_scarlet_enclave();
void AddSC_the_scarlet_enclave_c1();
void AddSC_the_scarlet_enclave_c2();
void AddSC_the_scarlet_enclave_c5();

// Classic Dungeons - Scarlet Monastery
void AddSC_boss_brother_korloff();
void AddSC_boss_commander_durand();
void AddSC_boss_headless_horseman();
void AddSC_boss_thalnos_the_soulrender();
void AddSC_instance_scarlet_monastery();

// Classic Dungeons - Scarlet Halls
void AddSC_boss_armsmaster_harlan();
void AddSC_boss_flameweaver_koegler();
void AddSC_boss_houndmaster_braun();
void AddSC_instance_scarlet_halls();

// Classic Dungeons - Scholomance
//void AddSC_boss_darkmaster_gandling();
//void AddSC_boss_jandice_barov();
//void AddSC_boss_lillian_voss();
//void AddSC_boss_rattlegore();
//void AddSC_instance_scholomance();
//void AddSC_scholomance();

// Classic Dungeons - Shadowfang Keep
void AddSC_boss_baron_ashbury();
void AddSC_boss_baron_silverlaine();
void AddSC_boss_commander_springvale();
void AddSC_boss_lord_valden();
void AddSC_boss_lord_godfrey();
void AddSC_shadowfang_keep();
void AddSC_instance_shadowfang_keep();

// Classic Dungeons - Stratholme
void AddSC_boss_magistrate_barthilas();
void AddSC_boss_maleki_the_pallid();
void AddSC_boss_nerubenkan();
void AddSC_boss_cannon_master_willey();
void AddSC_boss_baroness_anastari();
void AddSC_boss_ramstein_the_gorger();
void AddSC_boss_timmy_the_cruel();
void AddSC_boss_postmaster_malown();
void AddSC_boss_baron_rivendare();
void AddSC_boss_dathrohan_balnazzar();
void AddSC_boss_order_of_silver_hand();
void AddSC_instance_stratholme();
void AddSC_stratholme();

// Classic Dungeons - Sunken Temple
void AddSC_sunken_temple();
void AddSC_instance_sunken_temple();

// Classic Dungeons - Uldaman
void AddSC_boss_archaedas();
void AddSC_boss_ironaya();
void AddSC_uldaman();
void AddSC_instance_uldaman();

// Classic Raids - Blackwing Lair
void AddSC_boss_razorgore();
void AddSC_boss_vael();
void AddSC_boss_broodlord();
void AddSC_boss_firemaw();
void AddSC_boss_ebonroc();
void AddSC_boss_flamegor();
void AddSC_boss_chromaggus();
void AddSC_boss_nefarian();
void AddSC_boss_victor_nefarius();

// Classic Raids - Molten Core
void AddSC_boss_lucifron();
void AddSC_boss_magmadar();
void AddSC_boss_gehennas();
void AddSC_boss_garr();
void AddSC_boss_baron_geddon();
void AddSC_boss_shazzrah();
void AddSC_boss_golemagg();
void AddSC_boss_sulfuron();
void AddSC_boss_majordomo();
void AddSC_boss_ragnaros();
void AddSC_instance_molten_core();

// Classic Zones - Eastern Kingdoms
//void AddSC_alterac_mountains();
void AddSC_arathi_highlands();
void AddSC_blasted_lands();
void AddSC_burning_steppes();
void AddSC_duskwood();
void AddSC_eastern_plaguelands();
void AddSC_elwyn_forest();
void AddSC_dun_morogh();
void AddSC_hinterlands();
void AddSC_ironforge();
void AddSC_loch_modan();
void AddSC_redridge_mountains();
void AddSC_silvermoon_city();
void AddSC_silverpine_forest();
void AddSC_stormwind_city();
void AddSC_stranglethorn_vale();
void AddSC_swamp_of_sorrows();
void AddSC_tirisfal_glades();
void AddSC_undercity();
void AddSC_western_plaguelands();
void AddSC_westfall();
void AddSC_wetlands();

// Classic Zones - Kalimdor
void AddSC_ashenvale();
void AddSC_azshara();
void AddSC_darkshore();
void AddSC_desolace();
void AddSC_durotar();
void AddSC_dustwallow_marsh();
void AddSC_felwood();
void AddSC_feralas();
void AddSC_moonglade();
void AddSC_mulgore();
void AddSC_orgrimmar();
void AddSC_silithus();
void AddSC_southern_barrens();
void AddSC_stonetalon_mountains();
void AddSC_tanaris();
void AddSC_teldrassil();
void AddSC_the_barrens();
void AddSC_thousand_needles();
void AddSC_thunder_bluff();
void AddSC_ungoro_crater();
void AddSC_winterspring();

// Classic Dungeons - Blackfathom Depths
void AddSC_blackfathom_deeps();
void AddSC_boss_gelihast();
void AddSC_boss_kelris();
void AddSC_boss_aku_mai();
void AddSC_instance_blackfathom_deeps();

// Classic Dungeons - Maraudon
void AddSC_boss_celebras_the_cursed();
void AddSC_boss_landslide();
void AddSC_boss_noxxion();
void AddSC_boss_ptheradras();

// Classic Dungeons - Razorfen Downs
void AddSC_boss_amnennar_the_coldbringer();
void AddSC_razorfen_downs();
void AddSC_instance_razorfen_downs();

// Classic Dungeons - Wailing Caverns
void AddSC_wailing_caverns();
void AddSC_instance_wailing_caverns();

// Classic Dungeons - Zul'Farrak
void AddSC_zulfarrak();
void AddSC_instance_zulfarrak();

// Classic Dungeons - Razorfen Kraul
void AddSC_razorfen_kraul();
void AddSC_instance_razorfen_kraul();

// Classic Raids - Ruins of Ahn'qiraj
void AddSC_boss_kurinnaxx();
void AddSC_boss_rajaxx();
void AddSC_boss_moam();
void AddSC_boss_buru();
void AddSC_boss_ayamiss();
void AddSC_boss_ossirian();
void AddSC_instance_ruins_of_ahnqiraj();

// Classic Raids - Temple of Ahn'qiraj
void AddSC_boss_cthun();
void AddSC_boss_fankriss();
void AddSC_boss_huhuran();
void AddSC_bug_trio();
void AddSC_boss_sartura();
void AddSC_boss_skeram();
void AddSC_boss_twinemperors();
void AddSC_boss_ouro();
void AddSC_mob_anubisath_sentinel();
void AddSC_instance_temple_of_ahnqiraj();

/*

* Burning Crusade Scripts

*/

// Burning Crusade Zones
void AddSC_hellfire_peninsula();
void AddSC_nagrand();
void AddSC_netherstorm();
void AddSC_shadowmoon_valley();
void AddSC_shattrath_city();
void AddSC_blades_edge_mountains();
void AddSC_terokkar_forest();
void AddSC_zangarmarsh();
void AddSC_eversong_woods();
void AddSC_ghostlands();
void AddSC_isle_of_queldanas();
void AddSC_azuremyst_isle();
void AddSC_bloodmyst_isle();

// Burning Crusade World Bosses
void AddSC_boss_doomlordkazzak();
void AddSC_boss_doomwalker();

// Burning Crusade Dungeons - Auchindoun Auchenai Crypts
void AddSC_boss_exarch_maladaar();
void AddSC_boss_shirrak_the_dead_watcher();

// Burning Crusade Dungeons - Auchindoun Mana Tombs
void AddSC_boss_nexusprince_shaffar();
void AddSC_boss_pandemonius();

// Burning Crusade Dungeons - Auchindoun Sekketh Halls
void AddSC_boss_darkweaver_syth();
void AddSC_boss_talon_king_ikiss();
void AddSC_instance_sethekk_halls();

// Burning Crusade Dungeons - Auchindoun Shadow Labyrinth
void AddSC_instance_shadow_labyrinth();
void AddSC_boss_ambassador_hellmaw();
void AddSC_boss_blackheart_the_inciter();
void AddSC_boss_grandmaster_vorpil();
void AddSC_boss_murmur();

// Burning Crusade Dungeons - CR Steam Vault
void AddSC_boss_hydromancer_thespia();
void AddSC_boss_mekgineer_steamrigger();
void AddSC_boss_warlord_kalithresh();
void AddSC_instance_steam_vault();

// Burning Crusade Dungeons - CR Underbog
void AddSC_boss_hungarfen();
void AddSC_boss_the_black_stalker();

// Burning Crusade Dungeons - HC Blood Furnace
void AddSC_boss_broggok();
void AddSC_boss_kelidan_the_breaker();
void AddSC_boss_the_maker();
void AddSC_instance_blood_furnace();

// Burning Crusade Dungeons - HC Shattered Halls
void AddSC_boss_grand_warlock_nethekurse();
void AddSC_boss_warbringer_omrogg();
void AddSC_boss_warchief_kargath_bladefist();
void AddSC_instance_shattered_halls();

// Burning Crusade Dungeons - HC Ramparts
void AddSC_boss_watchkeeper_gargolmar();
void AddSC_boss_omor_the_unscarred();
void AddSC_boss_vazruden_the_herald();
void AddSC_instance_ramparts();

// Burning Crusade Dungeons - TK Arcatraz
void AddSC_arcatraz();
void AddSC_boss_harbinger_skyriss();
void AddSC_instance_arcatraz();

// Burning Crusade Dungeons  - TK Botanica
void AddSC_boss_high_botanist_freywinn();
void AddSC_boss_laj();
void AddSC_boss_warp_splinter();

// Burning Crusade Dungeons - TK The Mechanar
void AddSC_boss_gatewatcher_iron_hand();
void AddSC_boss_nethermancer_sepethrea();
void AddSC_boss_pathaleon_the_calculator();
void AddSC_boss_mechano_lord_capacitus();
void AddSC_instance_mechanar();
void AddSC_mechanar();

// Burning Crusade Dungeons - Magister's Terrace
void AddSC_boss_felblood_kaelthas();
void AddSC_boss_selin_fireheart();
void AddSC_boss_vexallus();
void AddSC_boss_priestess_delrissa();
void AddSC_instance_magisters_terrace();
void AddSC_magisters_terrace();

// Burning Crusade Dungeons - Old Hillsbrad
void AddSC_boss_captain_skarloc();
void AddSC_boss_epoch_hunter();
void AddSC_boss_lieutenant_drake();
void AddSC_instance_old_hillsbrad();
void AddSC_old_hillsbrad();

// Burning Crusade Dungeons - The Dark Portal
void AddSC_boss_aeonus();
void AddSC_boss_chrono_lord_deja();
void AddSC_boss_temporus();
void AddSC_dark_portal();
void AddSC_instance_dark_portal();

// Burning Crusade Raids - Karazhan
void AddSC_boss_attumen();
void AddSC_boss_curator();
void AddSC_boss_maiden_of_virtue();
void AddSC_boss_shade_of_aran();
void AddSC_boss_malchezaar();
void AddSC_boss_terestian_illhoof();
void AddSC_boss_moroes();
void AddSC_bosses_opera();
void AddSC_boss_netherspite();
void AddSC_instance_karazhan();
void AddSC_karazhan();
void AddSC_boss_nightbane();

// Burning Crusade Raids - Battle for Mount Hyjal
void AddSC_hyjal();
void AddSC_boss_archimonde();
void AddSC_instance_mount_hyjal();
void AddSC_hyjal_trash();
void AddSC_boss_rage_winterchill();
void AddSC_boss_anetheron();
void AddSC_boss_kazrogal();
void AddSC_boss_azgalor();

// Burning Crusade Raids - HC Magtheridon's Lair
void AddSC_boss_magtheridon();
void AddSC_instance_magtheridons_lair();

// Burning Crusade Raids - Black Temple
void AddSC_black_temple();
void AddSC_boss_illidan();
void AddSC_boss_shade_of_akama();
void AddSC_boss_supremus();
void AddSC_boss_gurtogg_bloodboil();
void AddSC_boss_mother_shahraz();
void AddSC_boss_reliquary_of_souls();
void AddSC_boss_teron_gorefiend();
void AddSC_boss_najentus();
void AddSC_boss_illidari_council();
void AddSC_instance_black_temple();

// Burning Crusade Raids - CR Serpentshrine Cavern
void AddSC_boss_fathomlord_karathress();
void AddSC_boss_hydross_the_unstable();
void AddSC_boss_lady_vashj();
void AddSC_boss_leotheras_the_blind();
void AddSC_boss_morogrim_tidewalker();
void AddSC_instance_serpentshrine_cavern();
void AddSC_boss_the_lurker_below();

// Burning Crusade Raids - Grull's Lair
void AddSC_boss_gruul();
void AddSC_boss_high_king_maulgar();
void AddSC_instance_gruuls_lair();

// Burning Crusade Raids - TK The Eye
void AddSC_boss_alar();
void AddSC_boss_kaelthas();
void AddSC_boss_void_reaver();
void AddSC_boss_high_astromancer_solarian();
void AddSC_instance_the_eye();
void AddSC_the_eye();

// Burning Crusade Raids - Sunwell Plateau
void AddSC_instance_sunwell_plateau();
void AddSC_boss_kalecgos();
void AddSC_boss_brutallus();
void AddSC_boss_felmyst();
void AddSC_boss_eredar_twins();
void AddSC_boss_muru();
void AddSC_boss_kiljaeden();
void AddSC_sunwell_plateau();

/*

* Wrath of the Lich King Scripts

*/

// WotLK Zones
void AddSC_dalaran();
void AddSC_borean_tundra();
void AddSC_dragonblight();
void AddSC_grizzly_hills();
void AddSC_howling_fjord();
void AddSC_icecrown();
void AddSC_sholazar_basin();
void AddSC_storm_peaks();
void AddSC_wintergrasp();
void AddSC_zuldrak();
void AddSC_crystalsong_forest();
void AddSC_isle_of_conquest();

// WotLK Dungeons - Gundrak
void AddSC_boss_slad_ran();
void AddSC_boss_moorabi();
void AddSC_boss_drakkari_colossus();
void AddSC_boss_gal_darah();
void AddSC_boss_eck();
void AddSC_instance_gundrak();

// WotLK Dungeons - Azjol-Nerub
void AddSC_boss_krik_thir();
void AddSC_boss_hadronox();
void AddSC_boss_anub_arak();
void AddSC_instance_azjol_nerub();

// WotLK Dungeons - Ahn'kahet
void AddSC_instance_ahnkahet();
void AddSC_boss_amanitar();
void AddSC_boss_taldaram();
void AddSC_boss_jedoga_shadowseeker();
void AddSC_boss_elder_nadox();
void AddSC_boss_volazj();

// WotLK Dungeons - Trial of the Champion
void AddSC_boss_argent_challenge();
void AddSC_boss_black_knight();
void AddSC_boss_grand_champions();
void AddSC_instance_trial_of_the_champion();
void AddSC_trial_of_the_champion();

// WotLK Dungeons - The Nexus
void AddSC_boss_magus_telestra();
void AddSC_boss_anomalus();
void AddSC_boss_ormorok();
void AddSC_boss_keristrasza();
void AddSC_boss_commander_nexus();
void AddSC_instance_nexus();

// WotLK Dungeons - The Oculus
void AddSC_boss_drakos();
void AddSC_boss_urom();
void AddSC_boss_varos();
void AddSC_boss_eregos();
void AddSC_instance_oculus();
void AddSC_oculus();

// WotLK Dungeons - Halls of Lightning
void AddSC_boss_bjarngrim();
void AddSC_boss_loken();
void AddSC_boss_ionar();
void AddSC_boss_volkhan();
void AddSC_instance_halls_of_lightning();

// WotLK Dungeons - Halls of Stone
void AddSC_boss_maiden_of_grief();
void AddSC_boss_krystallus();
void AddSC_boss_sjonnir();
void AddSC_instance_halls_of_stone();
void AddSC_halls_of_stone();

// WotLK Dungeons - Utgarde Keep
void AddSC_boss_keleseth();
void AddSC_boss_skarvald_dalronn();
void AddSC_boss_ingvar_the_plunderer();
void AddSC_instance_utgarde_keep();

// WotLK Dungeons - Utgarde Pinnacle
void AddSC_boss_svala();
void AddSC_boss_palehoof();
void AddSC_boss_skadi();
void AddSC_boss_ymiron();
void AddSC_instance_utgarde_pinnacle();

// WotLK Dungeons - Drak'tharon Keep
void AddSC_boss_trollgore();
void AddSC_boss_novos();
void AddSC_boss_dred();
void AddSC_boss_tharon_ja();
void AddSC_instance_drak_tharon();

// WotLK Dungeons - Violet Hold
void AddSC_boss_cyanigosa();
void AddSC_boss_erekem();
void AddSC_boss_ichoron();
void AddSC_boss_lavanthor();
void AddSC_boss_moragg();
void AddSC_boss_xevozz();
void AddSC_boss_zuramat();
void AddSC_instance_violet_hold();
void AddSC_violet_hold();

// WotLK Dungeons - Culling of Stratholme
void AddSC_boss_epoch();
void AddSC_boss_infinite_corruptor();
void AddSC_boss_salramm();
void AddSC_boss_mal_ganis();
void AddSC_boss_meathook();
void AddSC_culling_of_stratholme();
void AddSC_instance_culling_of_stratholme();

// WotLK Dungeons - Forge of Souls
void AddSC_instance_forge_of_souls();
void AddSC_forge_of_souls();
void AddSC_boss_bronjahm();
void AddSC_boss_devourer_of_souls();

// WotLK Dungeons - Pit of Saron
void AddSC_instance_pit_of_saron();
void AddSC_pit_of_saron();
void AddSC_boss_garfrost();
void AddSC_boss_ick();
void AddSC_boss_tyrannus();

// WotLK Dungeons - Hallf of Reflection
void AddSC_instance_halls_of_reflection();
void AddSC_halls_of_reflection();
void AddSC_boss_falric();
void AddSC_boss_marwyn();
void AddSC_boss_lich_king_hr();
void AddSC_utgarde_keep();

// WotLK Raids - Vault of Archavon
void AddSC_boss_archavon();
void AddSC_boss_emalon();
void AddSC_boss_koralon();
void AddSC_boss_toravon();
void AddSC_instance_archavon();

// WotLK Raids - Naxxramas
void AddSC_boss_anubrekhan();
void AddSC_boss_maexxna();
void AddSC_boss_patchwerk();
void AddSC_boss_grobbulus();
void AddSC_boss_razuvious();
void AddSC_boss_kelthuzad();
void AddSC_boss_loatheb();
void AddSC_boss_noth();
void AddSC_boss_gluth();
void AddSC_boss_sapphiron();
void AddSC_boss_four_horsemen();
void AddSC_boss_faerlina();
void AddSC_boss_heigan();
void AddSC_boss_gothik();
void AddSC_boss_thaddius();
void AddSC_instance_naxxramas();

// WotLK Raids - Obsidian Sanctum
void AddSC_boss_sartharion();
void AddSC_instance_obsidian_sanctum();

// WotLK Raids - Eye of Eternity
void AddSC_boss_malygos();
void AddSC_instance_eye_of_eternity();

// WotLK Raids - Ulduar
void AddSC_instance_ulduar();
void AddSC_ulduar_teleporter();
void AddSC_ulduar_scripts();
void AddSC_boss_flame_leviathan();
void AddSC_boss_ignis();
void AddSC_boss_razorscale();
void AddSC_boss_xt002();
void AddSC_boss_assembly_of_iron();
void AddSC_boss_kologarn();
void AddSC_boss_auriaya();
void AddSC_boss_hodir();
void AddSC_boss_freya();
void AddSC_boss_mimiron();
void AddSC_boss_thorim();
void AddSC_boss_general_vezax();
void AddSC_boss_algalon_the_observer();

// WotLK Raids - Onyxia's Lair
void AddSC_boss_onyxia();
void AddSC_instance_onyxias_lair();

// WotLK Raids - Trial of the Crusader
void AddSC_boss_anubarak_trial();
void AddSC_boss_faction_champions();
void AddSC_boss_jaraxxus();
void AddSC_boss_northrend_beasts();
void AddSC_boss_twin_valkyr();
void AddSC_trial_of_the_crusader();
void AddSC_instance_trial_of_the_crusader();

// WotLK Raids - Icecrown Citadel
void AddSC_instance_icecrown_citadel();
void AddSC_icecrown_citadel();
void AddSC_icecrown_citadel_teleport();
void AddSC_boss_lord_marrowgar();
void AddSC_boss_lady_deathwhisper();
void AddSC_boss_gunship_battle();
void AddSC_boss_deathbringer_saurfang();
void AddSC_boss_festergut();
void AddSC_boss_rotface();
void AddSC_boss_professor_putricide();
void AddSC_boss_blood_prince_council();
void AddSC_boss_blood_queen_lana_thel();
void AddSC_boss_valithria_dreamwalker();
void AddSC_boss_sindragosa();
void AddSC_boss_the_lich_king();

// WotLK Raids - Ruby Sanctum
void AddSC_instance_ruby_sanctum();
void AddSC_ruby_sanctum();
void AddSC_boss_baltharus_the_warborn();
void AddSC_boss_saviana_ragefire();
void AddSC_boss_general_zarithrian();
void AddSC_boss_halion();

/*

* Cataclysm Scripts

*/

// Cataclys Zones
void AddSC_gilneas();

// Cataclysm dungeons - Throne of the Tides
void AddSC_instance_throne_of_the_tides(); 
void AddSC_throne_of_the_tides();
void AddSC_boss_lady_nazjar();
void AddSC_boss_commander_ulthok();
void AddSC_boss_erunak_stonespeaker();
void AddSC_boss_ozumat();

// Cataclysm dungeons - Blackrock Caverns
void AddSC_instance_blackrock_caverns();
void AddSC_boss_romogg_bonecrusher();
void AddSC_boss_corla_herald_of_twilight();
void AddSC_boss_karsh_steelbender();
void AddSC_boss_beauty();
void AddSC_boss_ascendant_lord_obsidius();

// Cataclysm dungeons - The Vortex Pinnacle
void AddSC_instance_the_vortex_pinnacle();
void AddSC_the_vortex_pinnacle();
void AddSC_boss_grand_vizier_ertan();
void AddSC_boss_altairus();
void AddSC_boss_asaad();

// Cataclysm dungeons - The Stonecore
void AddSC_instance_the_stonecore(); // The Stonecore
void AddSC_the_stonecore();
void AddSC_boss_corborus();
void AddSC_boss_slabhide();
void AddSC_boss_ozruk();
void AddSC_boss_high_priestess_azil();

// Cataclysm dungeons - Grim Batol
void AddSC_instance_grim_batol();
void AddSC_grim_batol();
void AddSC_boss_general_umbriss();
void AddSC_boss_forgemaster_throngus();
void AddSC_boss_drahga_shadowburner();
void AddSC_boss_erudax();

// Cataclysm dungeons - Lost City of the Tol'Vir
void AddSC_instance_lost_city_of_the_tolvir();
void AddSC_lost_city_of_the_tolvir();
void AddSC_boss_general_husam();
void AddSC_boss_lockmaw_augh();
void AddSC_boss_high_prophet_barim();
void AddSC_boss_siamat();

// Cataclysm dungeons - Halls of Origination
void AddSC_instance_halls_of_origination();
void AddSC_halls_of_origination();
void AddSC_boss_temple_guardian_anhuur();
void AddSC_boss_earthrager_ptah();
void AddSC_boss_anraphet();
void AddSC_boss_ammunae();
void AddSC_boss_isiset();
void AddSC_boss_setesh();
void AddSC_boss_rajh();

// Cataclysm Dungeons - Zul'aman
void AddSC_boss_akilzon();
void AddSC_boss_halazzi();
void AddSC_boss_hex_lord_malacrass();
void AddSC_boss_janalai();
void AddSC_boss_nalorakk();
void AddSC_boss_daakara();
void AddSC_instance_zulaman();
void AddSC_zulaman();

// Cataclysm Dungeons - Zul'gurub
void AddSC_boss_grilek();
void AddSC_boss_hazzarah();
void AddSC_boss_jindo_the_godbreaker();
void AddSC_boss_kilnara();
void AddSC_boss_mandokir();
void AddSC_boss_renataki();
void AddSC_boss_venoxis();
void AddSC_boss_wushoolay();
void AddSC_boss_zanzil();
void AddSC_instance_zulgurub();

// Cataclysm dungeons - End Time
void AddSC_instance_end_time();
void AddSC_end_time();
void AddSC_end_time_teleport();
void AddSC_boss_echo_of_tyrande();
void AddSC_boss_echo_of_sylvanas();
void AddSC_boss_echo_of_baine();
void AddSC_boss_echo_of_jaina();
void AddSC_boss_murozond();

// Cataclysm dungeons - Well of Eternity
void AddSC_instance_well_of_eternity();
void AddSC_well_of_eternity();
void AddSC_well_of_eternity_teleport();
void AddSC_boss_perotharn();
void AddSC_boss_queen_azshara();
void AddSC_boss_mannoroth();

// Cataclysm dungeons - Hour of Twilight
void AddSC_instance_hour_of_twilight();
void AddSC_hour_of_twilight();
void AddSC_boss_arcurion();
void AddSC_boss_asira_dawnslayer();
void AddSC_boss_archbishop_benedictus();

// Cataclysm raids - Baradin Hold
void AddSC_instance_baradin_hold();
void AddSC_boss_argaloth();
void AddSC_boss_occuthar();
void AddSC_boss_alizabal();

// Cataclysm raids - Bastion of Twilight
void AddSC_instance_bastion_of_twilight();
void AddSC_bastion_of_twilight();
void AddSC_boss_halfus_wyrmbreaker();
void AddSC_boss_theralion_and_valiona();
void AddSC_boss_ascendant_council();
void AddSC_boss_chogall();
void AddSC_boss_sinestra();

// Cataclysm raids - Blackwing Descent
void AddSC_instance_blackwing_descent();
void AddSC_blackwing_descent();
void AddSC_boss_magmaw();
void AddSC_boss_omnotron_defence_system();
void AddSC_boss_maloriak();
void AddSC_boss_chimaeron();
void AddSC_boss_atramedes();

// Cataclysm raids - Throne of the Four Winds
void AddSC_instance_throne_of_the_four_winds();
void AddSC_throne_of_the_four_winds();
void AddSC_boss_conclave_of_wind();

// Cataclysm raids - Firelands
void AddSC_instance_firelands();
void AddSC_firelands();
void AddSC_boss_shannox();
void AddSC_boss_bethtilac();
void AddSC_boss_alysrazor();
void AddSC_boss_lord_rhyolith();
void AddSC_boss_baleroc();
void AddSC_boss_majordomo_staghelm();
void AddSC_boss_ragnaros_firelands();

// Cataclysm raids - Dragon Soul
void AddSC_instance_dragon_soul();
void AddSC_dragon_soul();
void AddSC_boss_morchok();
void AddSC_boss_yorsahj_the_unsleeping();
void AddSC_boss_warlord_zonozz();
void AddSC_boss_hagara_the_stormbinder();
void AddSC_boss_ultraxion();
void AddSC_boss_warmaster_blackhorn();
void AddSC_spine_of_deathwing();
void AddSC_madness_of_deathwing();

/*

* Mists of Pandaria Scripts

*/

// MoP zones
void AddSC_townlong_steppes();
void AddSC_dread_wastes();
void AddSC_valley_of_the_four_winds();
void AddSC_krasarang_wilds();
void AddSC_jade_forest();
void AddSC_vale_of_eternal_blossoms();
void AddSC_kun_lai_summit();
void AddSC_WanderingIsland_North();
void AddSC_WanderingIsland_East();
void AddSC_WanderingIsland_West();
void AddSC_WanderingIsland_South();
void AddSC_timeless_isle();
void AddSC_zone_timeless_isle();

// MoP dungeons - Temple of Jade Serpent
void AddSC_instance_temple_of_jade_serpent();
void AddSC_boss_wise_mari();
void AddSC_boss_lorewalker_stonestep();
void AddSC_boss_liu_flameheat();
void AddSC_boss_sha_of_doubt();

// MoP dungeons - The Stormstoud Brewery
void AddSC_boss_hoptallus();
void AddSC_boss_ook_ook();
void AddSC_boss_yan_zhu_the_uncasked();
void AddSC_instance_stormstout_brewery();
void AddSC_stormstout_brewery();

// MoP dungeons - Gate of the Setting Sun
void AddSC_instance_gate_setting_sun();
void AddSC_gate_setting_sun();
void AddSC_boss_saboteur_kiptilak();
void AddSC_boss_striker_gadok();
void AddSC_boss_commander_rimok();
void AddSC_boss_raigonn();

// MoP dungeons - Mogu'shan Palace
void AddSC_instance_mogu_shan_palace();
void AddSC_boss_trial_of_the_king();
void AddSC_boss_gekkan();
void AddSC_boss_xin_the_weaponmaster();

// MoP dungeons - Shado'pan Monastery
void AddSC_instance_shadopan_monastery();
void AddSC_shadopan_monastery();
void AddSC_boss_gu_cloudstrike();
void AddSC_boss_master_snowdrift();
void AddSC_boss_sha_of_violence();
void AddSC_boss_taran_zhu();

// MoP dungeons - Siege of the Niuzao Temple
void AddSC_siege_of_the_niuzoa_temple();
void AddSC_boss_jinbak();
void AddSC_boss_vojak();
void AddSC_WING_LEADER();
void AddSC_boss_general_pavalak();

// MoP world bosses
void AddSC_boss_sha_of_anger();
void AddSC_boss_galion();
void AddSC_boss_chi_ji();
void AddSC_boss_nalak();
void AddSC_boss_ordos();

// MoP raids - Mogu'shan Vaults
void AddSC_instance_mogu_shan_vault();
void AddSC_mogu_shan_vault();
void AddSC_boss_stone_guard();
void AddSC_boss_feng();
void AddSC_boss_garajal();
void AddSC_boss_spirit_kings();
void AddSC_boss_elegon();
void AddSC_boss_will_of_emperor();

// MoP raids - Heart of Fear
void AddSC_Heart_Of_Fear();
void AddSC_imperial_zorlok();
//void AddSC_instance_heart_of_fear();
void AddSC_Lord_Tayak();
void AddSC_lord_meljarak();
void AddSC_boss_garalon();
void AddSC_empress_shekzeer();

// MoP raids - Terrace of Endless Spring
void AddSC_instance_terrace_of_endless_spring();
void AddSC_terrace_of_endless_spring();
void AddSC_boss_protectors_of_the_endless();
void AddSC_boss_tsulong();
void AddSC_boss_lei_shi();
void AddSC_boss_sha_of_fear();

// MoP raids - Throne of Thunder
void AddSC_boss_horridon();
void AddSC_boss_jin_rokh_breaker();
void AddSC_boss_ji_kun();
void AddSC_boss_durumu();
void AddSC_megaera();
void AddSC_tortos();
void AddSC_council();
void AddSC_instance_throne_of_thunder();

// MoP raids - Siege of Orgrimmar
void AddSC_siege_of_orgrimmar();
void AddSC_instance_siege_of_orgrimmar();
void AddSC_immersus();
void AddSC_fallen_protectors();
void AddSC_norushen();
void AddSC_sha_of_pride();
void AddSC_boss_iron_juggernaut();
void AddSC_boss_dark_shaman();
void AddSC_boss_general_nazgrim();
void AddSC_boss_malkorok();
void AddSC_garrosh_hellscream();
#endif

void AddScripts()
{
	AddExampleScripts();
	AddSpellScripts();
	AddSC_SmartSCripts();
	AddCommandScripts();
	sAnticheatMgr->StartScripts();
#ifdef SCRIPTS
	AddWorldScripts();
	AddEasternKingdomsScripts();
	AddKalimdorScripts();
	AddOutlandScripts();
	AddNorthrendScripts();
	AddPandarieScripts();
	AddBattlegroundScripts();
	AddOutdoorPvPScripts();
	AddCustomScripts();
#endif
}

void AddExampleScripts()
{
	AddSC_example_creature();
	AddSC_example_escort();
	AddSC_example_gossip_codebox();
	AddSC_example_misc();
	AddSC_example_commandscript();
}

void AddSpellScripts()
{
	AddSC_deathknight_spell_scripts();
	AddSC_druid_spell_scripts();
	AddSC_generic_spell_scripts();
	AddSC_hunter_spell_scripts();
	AddSC_mage_spell_scripts();
	AddSC_paladin_spell_scripts();
	AddSC_priest_spell_scripts();
	AddSC_rogue_spell_scripts();
	AddSC_shaman_spell_scripts();
	AddSC_warlock_spell_scripts();
	AddSC_warrior_spell_scripts();
	AddSC_monk_spell_scripts();
	AddSC_mastery_spell_scripts();
	AddSC_quest_spell_scripts();
	AddSC_item_spell_scripts();
	AddSC_example_spell_scripts();
	AddSC_holiday_spell_scripts();
}

void AddPetScripts()
{
	AddSC_deathknight_pet_scripts();

}

void AddCommandScripts()
{
	AddSC_anticheat_commandscript();
	AddSC_account_commandscript();
	AddSC_achievement_commandscript();
	AddSC_ban_commandscript();
	AddSC_bf_commandscript();
	AddSC_cast_commandscript();
	AddSC_character_commandscript();
	AddSC_cheat_commandscript();
	AddSC_debug_commandscript();
	AddSC_disable_commandscript();
	AddSC_event_commandscript();
	AddSC_gm_commandscript();
	AddSC_go_commandscript();
	AddSC_gobject_commandscript();
	AddSC_guild_commandscript();
	AddSC_honor_commandscript();
	AddSC_instance_commandscript();
	AddSC_learn_commandscript();
	AddSC_lookup_commandscript();
	AddSC_list_commandscript();
	AddSC_message_commandscript();
	AddSC_misc_commandscript();
	AddSC_modify_commandscript();
	AddSC_npc_commandscript();
	AddSC_quest_commandscript();
	AddSC_reload_commandscript();
	AddSC_reset_commandscript();
	AddSC_server_commandscript();
	AddSC_tele_commandscript();
	AddSC_ticket_commandscript();
	AddSC_titles_commandscript();
	AddSC_wp_commandscript();
}

void AddWorldScripts()
{
#ifdef SCRIPTS
	AddSC_areatrigger_scripts();
	AddSC_generic_creature();
	AddSC_go_scripts();
	AddSC_guards();
	AddSC_item_scripts();
	AddSC_npc_professions();
	AddSC_npc_innkeeper();
	AddSC_npcs_special();
	AddSC_npc_taxi();
	AddSC_npc_companions();
	AddSC_achievement_scripts();
	AddSC_chat_log();
	AddSC_mine_cart();
#endif
}

void AddEasternKingdomsScripts()
{
#ifdef SCRIPTS

	// Classic Zones
	//AddSC_alterac_mountains();
	AddSC_arathi_highlands();
	AddSC_blasted_lands();
	AddSC_burning_steppes();
	AddSC_duskwood();
	AddSC_eastern_plaguelands();
	AddSC_eversong_woods();
	AddSC_elwyn_forest();
	AddSC_dun_morogh();
	AddSC_ghostlands();
	AddSC_hinterlands();
	AddSC_ironforge();
	AddSC_isle_of_queldanas();
	AddSC_loch_modan();
	AddSC_redridge_mountains();
	AddSC_silvermoon_city();
	AddSC_silverpine_forest();
	AddSC_stormwind_city();
	AddSC_stranglethorn_vale();
	AddSC_swamp_of_sorrows();
	AddSC_tirisfal_glades();
	AddSC_undercity();
	AddSC_western_plaguelands();
	AddSC_westfall();
	AddSC_wetlands();

	// Classic Dungeons - Alterac Valley
	AddSC_alterac_valley();
	AddSC_boss_balinda();
	AddSC_boss_drekthar();
	AddSC_boss_galvangar();
	AddSC_boss_vanndar();

	// Classic Dungeons - Blackrock Depths
	AddSC_blackrock_depths();
	AddSC_boss_ambassador_flamelash();
	AddSC_boss_anubshiah();
	AddSC_boss_draganthaurissan();
	AddSC_boss_general_angerforge();
	AddSC_boss_gorosh_the_dervish();
	AddSC_boss_grizzle();
	AddSC_boss_high_interrogator_gerstahn();
	AddSC_boss_magmus();
	AddSC_boss_moira_bronzebeard();
	AddSC_boss_tomb_of_seven();
	AddSC_instance_blackrock_depths();

	// Classic Dungeons - Blackrock Spire
	AddSC_boss_drakkisath();
	AddSC_boss_halycon();
	AddSC_boss_highlordomokk();
	AddSC_boss_mothersmolderweb();
	AddSC_boss_overlordwyrmthalak();
	AddSC_boss_shadowvosh();
	AddSC_boss_thebeast();
	AddSC_boss_warmastervoone();
	AddSC_boss_quatermasterzigris();
	AddSC_boss_pyroguard_emberseer();
	AddSC_boss_gyth();
	AddSC_boss_rend_blackhand();
	AddSC_instance_blackrock_spire();
	AddSC_blackrock_spire();

	// Classic Dungeons - Deadmines
	AddSC_instance_deadmines();
	AddSC_deadmines();
	AddSC_boss_glubtok();
	AddSC_boss_helix_gearbreaker();
	AddSC_boss_foereaper5000();
	AddSC_boss_admiral_ripsnarl();
	AddSC_boss_captain_cookie();
	AddSC_boss_vanessa_vancleef();

	// Classic Dungeons - Gnomergan
	AddSC_gnomeregan();
	AddSC_instance_gnomeregan();

	// Classic Dungeons - Scarlet Enclave
	AddSC_the_scarlet_enclave();
	AddSC_the_scarlet_enclave_c1();
	AddSC_the_scarlet_enclave_c2();
	AddSC_the_scarlet_enclave_c5();

	// Classic Dungeons - Scarlet Monastery
	AddSC_boss_brother_korloff();
	AddSC_boss_commander_durand();
	AddSC_boss_headless_horseman();
	AddSC_boss_thalnos_the_soulrender();
	AddSC_instance_scarlet_monastery();

	// Classic Dungeons - Scarlet Halls
	AddSC_boss_armsmaster_harlan();
	AddSC_boss_flameweaver_koegler();
	AddSC_boss_houndmaster_braun();
	AddSC_instance_scarlet_halls();

	// Classic Dungeons - Scholomance
	//AddSC_boss_darkmaster_gandling();
	//AddSC_boss_jandice_barov();
	//AddSC_boss_lillian_voss();
	//AddSC_boss_rattlegore();
	//AddSC_instance_scholomance();
	//AddSC_scholomance();

	// Classic Dungeons - Shadowfang Keep
	AddSC_boss_baron_ashbury();
	AddSC_boss_baron_silverlaine();
	AddSC_boss_commander_springvale();
	AddSC_boss_lord_valden();
	AddSC_boss_lord_godfrey();
	AddSC_shadowfang_keep();
	AddSC_instance_shadowfang_keep();

	// Classic Dungeons - Stratholme
	AddSC_boss_magistrate_barthilas();
	AddSC_boss_maleki_the_pallid();
	AddSC_boss_nerubenkan();
	AddSC_boss_cannon_master_willey();
	AddSC_boss_baroness_anastari();
	AddSC_boss_ramstein_the_gorger();
	AddSC_boss_timmy_the_cruel();
	AddSC_boss_postmaster_malown();
	AddSC_boss_baron_rivendare();
	AddSC_boss_dathrohan_balnazzar();
	AddSC_boss_order_of_silver_hand();
	AddSC_instance_stratholme();
	AddSC_stratholme();

	// Classic Dungeons - Sunken Temple
	AddSC_sunken_temple();
	AddSC_instance_sunken_temple();

	// Classic Dungeons - Uldaman
	AddSC_boss_archaedas();
	AddSC_boss_ironaya();
	AddSC_uldaman();
	AddSC_instance_uldaman();

	// Classic Raids - Molten Core
	AddSC_boss_lucifron();
	AddSC_boss_magmadar();
	AddSC_boss_gehennas();
	AddSC_boss_garr();
	AddSC_boss_baron_geddon();
	AddSC_boss_shazzrah();
	AddSC_boss_golemagg();
	AddSC_boss_sulfuron();
	AddSC_boss_majordomo();
	AddSC_boss_ragnaros();
	AddSC_instance_molten_core();

	// Classic Raids - Blackwing Lair
	AddSC_boss_razorgore();
	AddSC_boss_vael();
	AddSC_boss_broodlord();
	AddSC_boss_firemaw();
	AddSC_boss_ebonroc();
	AddSC_boss_flamegor();
	AddSC_boss_chromaggus();
	AddSC_boss_nefarian();
	AddSC_boss_victor_nefarius();

	/*
	
	*	Cataclysm Scripts
	
	*/

	// Cataclysm Zones
	AddSC_gilneas();

	// Cataclysm Dungeons - Zul'aman
	AddSC_boss_akilzon();
	AddSC_boss_halazzi();
	AddSC_boss_hex_lord_malacrass();
	AddSC_boss_janalai();
	AddSC_boss_nalorakk();
	AddSC_boss_daakara();
	AddSC_instance_zulaman();
	AddSC_zulaman();

	// Cataclysm Dungeons - Zul'gurub
	AddSC_boss_grilek();
	AddSC_boss_hazzarah();
	AddSC_boss_jindo_the_godbreaker();
	AddSC_boss_kilnara();
	AddSC_boss_mandokir();
	AddSC_boss_renataki();
	AddSC_boss_venoxis();
	AddSC_boss_wushoolay();
	AddSC_boss_zanzil();
	AddSC_instance_zulgurub();

	// Cataclysm Raids - Bastion of Twilight
	AddSC_instance_bastion_of_twilight();
	AddSC_bastion_of_twilight();
	AddSC_boss_halfus_wyrmbreaker();
	AddSC_boss_theralion_and_valiona();
	AddSC_boss_ascendant_council();
	AddSC_boss_chogall();
	AddSC_boss_sinestra();

	// Cataclysm Raids - Blackwing Descent
	AddSC_instance_blackwing_descent();
	AddSC_blackwing_descent();
	AddSC_boss_magmaw();
	AddSC_boss_omnotron_defence_system();
	AddSC_boss_maloriak();
	AddSC_boss_chimaeron();
	AddSC_boss_atramedes();

	// Cataclysm Raids - Baradin Hold
	AddSC_instance_baradin_hold();
	AddSC_boss_argaloth();
	AddSC_boss_occuthar();
	AddSC_boss_alizabal();

#endif
}

void AddKalimdorScripts()
{
#ifdef SCRIPTS

	// Classic Dungeons - Blackfathom Depths
	AddSC_blackfathom_deeps();
	AddSC_boss_gelihast();
	AddSC_boss_kelris();
	AddSC_boss_aku_mai();
	AddSC_instance_blackfathom_deeps();

	// Classic Dungeons - Maraudon
	AddSC_boss_celebras_the_cursed();
	AddSC_boss_landslide();
	AddSC_boss_noxxion();
	AddSC_boss_ptheradras();

	// Classic Dungeons - Razorfen Downs
	AddSC_boss_amnennar_the_coldbringer();
	AddSC_razorfen_downs();
	AddSC_instance_razorfen_downs();

	// Classic Dungeons - Razorfen Kraul
	AddSC_razorfen_kraul();
	AddSC_instance_razorfen_kraul();

	// Classic Raids - Ruins of Ahn'qiraj
	AddSC_boss_kurinnaxx();
	AddSC_boss_rajaxx();
	AddSC_boss_moam();
	AddSC_boss_buru();
	AddSC_boss_ayamiss();
	AddSC_boss_ossirian();
	AddSC_instance_ruins_of_ahnqiraj();

	// Classic Raids - Temple of Ahn'qiraj
	AddSC_boss_cthun();
	AddSC_boss_fankriss();
	AddSC_boss_huhuran();
	AddSC_bug_trio();
	AddSC_boss_sartura();
	AddSC_boss_skeram();
	AddSC_boss_twinemperors();
	AddSC_boss_ouro();
	AddSC_mob_anubisath_sentinel();
	AddSC_instance_temple_of_ahnqiraj();

	// Classic Dungeons - Wailing Caverns
	AddSC_wailing_caverns();
	AddSC_instance_wailing_caverns();

	// Classic Dungeons - Zul'farrak
	AddSC_zulfarrak();
	AddSC_instance_zulfarrak();

	// Classic Zones
	AddSC_ashenvale();
	AddSC_azshara();
	AddSC_azuremyst_isle();
	AddSC_bloodmyst_isle();
	AddSC_darkshore();
	AddSC_desolace();
	AddSC_durotar();
	AddSC_dustwallow_marsh();
	AddSC_felwood();
	AddSC_feralas();
	AddSC_moonglade();
	AddSC_mulgore();
	AddSC_orgrimmar();
	AddSC_silithus();
	AddSC_southern_barrens();
	AddSC_stonetalon_mountains();
	AddSC_tanaris();
	AddSC_teldrassil();
	AddSC_the_barrens();
	AddSC_thousand_needles();
	AddSC_thunder_bluff();
	AddSC_ungoro_crater();
	AddSC_winterspring();

	/*
	
	*	Cataclysm Scripts
	
	*/

	// Cataclysm Dungeons - Lost City of the Tol'Vir
	AddSC_instance_lost_city_of_the_tolvir();
	AddSC_lost_city_of_the_tolvir();
	AddSC_boss_general_husam();
	AddSC_boss_lockmaw_augh();
	AddSC_boss_high_prophet_barim();
	AddSC_boss_siamat();

	// Cataclysm Dungeons - Halls of Origination
	AddSC_instance_halls_of_origination();
	AddSC_halls_of_origination();
	AddSC_boss_temple_guardian_anhuur();
	AddSC_boss_earthrager_ptah();
	AddSC_boss_anraphet();
	AddSC_boss_ammunae();
	AddSC_boss_isiset();
	AddSC_boss_setesh();
	AddSC_boss_rajh();

	// Cataclysm Dungeons - End Time
	AddSC_instance_end_time();
	AddSC_end_time();
	AddSC_end_time_teleport();
	AddSC_boss_echo_of_tyrande();
	AddSC_boss_echo_of_sylvanas();
	AddSC_boss_echo_of_baine();
	AddSC_boss_echo_of_jaina();
	AddSC_boss_murozond();

	// Cataclysm Dungeons - Well of Eternity
	AddSC_instance_well_of_eternity();
	AddSC_well_of_eternity();
	AddSC_well_of_eternity_teleport();
	AddSC_boss_perotharn();
	AddSC_boss_queen_azshara();
	AddSC_boss_mannoroth();

	// Cataclysm Dungeons - Hour of Twilight
	AddSC_instance_hour_of_twilight();
	AddSC_hour_of_twilight();
	AddSC_boss_arcurion();
	AddSC_boss_asira_dawnslayer();
	AddSC_boss_archbishop_benedictus();

	// Cataclysm Raids - Throne of the Four Winds
	AddSC_instance_throne_of_the_four_winds();
	AddSC_throne_of_the_four_winds();
	AddSC_boss_conclave_of_wind();

	// Cataclysm Raids - Firelands
	AddSC_instance_firelands();
	AddSC_firelands();
	AddSC_boss_shannox();
	AddSC_boss_bethtilac();
	AddSC_boss_alysrazor();
	AddSC_boss_lord_rhyolith();
	AddSC_boss_baleroc();
	AddSC_boss_majordomo_staghelm();
	AddSC_boss_ragnaros_firelands();

	// Cataclysm Raids - Dragon Soul
	AddSC_instance_dragon_soul();
	AddSC_dragon_soul();
	AddSC_boss_morchok();
	AddSC_boss_yorsahj_the_unsleeping();
	AddSC_boss_warlord_zonozz();
	AddSC_boss_hagara_the_stormbinder();
	AddSC_boss_ultraxion();
	AddSC_boss_warmaster_blackhorn();
	AddSC_spine_of_deathwing();
	AddSC_madness_of_deathwing();
#endif
}

void AddOutlandScripts()
{
#ifdef SCRIPTS

	// Burning Crusade Dungeons - Auchindoun Auchenai Crypts 
	AddSC_boss_exarch_maladaar();
	AddSC_boss_shirrak_the_dead_watcher();

	// Burning Crusade Dungeons - Auchindoun Mana Tombs
	AddSC_boss_nexusprince_shaffar();
	AddSC_boss_pandemonius();

	// Burning Crusade Dungeons - Auchindoun Sekketh Halls 
	AddSC_boss_darkweaver_syth();
	AddSC_boss_talon_king_ikiss();
	AddSC_instance_sethekk_halls();

	// Burning Crusade Dungeons - Auchindoun Shadow Labyrinth
	AddSC_instance_shadow_labyrinth();
	AddSC_boss_ambassador_hellmaw();
	AddSC_boss_blackheart_the_inciter();
	AddSC_boss_grandmaster_vorpil();
	AddSC_boss_murmur();

	// Burning Crusade Dungeons - Steam Vault
	AddSC_boss_hydromancer_thespia();
	AddSC_boss_mekgineer_steamrigger();
	AddSC_boss_warlord_kalithresh();
	AddSC_instance_steam_vault();

	// Burning Crusade Dungeons - Underbog
	AddSC_boss_hungarfen();
	AddSC_boss_the_black_stalker();

	// Burning Crusade Dungeons - Blood Furnace
	AddSC_boss_broggok();
	AddSC_boss_kelidan_the_breaker();
	AddSC_boss_the_maker();
	AddSC_instance_blood_furnace();

	// Burning Crusade Dungeons - Shattered Halls
	AddSC_boss_grand_warlock_nethekurse();
	AddSC_boss_warbringer_omrogg();
	AddSC_boss_warchief_kargath_bladefist();
	AddSC_instance_shattered_halls();

	// Burning Crusade Dungeons - Hellfire Rapmarts
	AddSC_boss_watchkeeper_gargolmar();
	AddSC_boss_omor_the_unscarred();
	AddSC_boss_vazruden_the_herald();
	AddSC_instance_ramparts();

	// Burning Crusade Dungeons - Arcatraz
	AddSC_arcatraz();
	AddSC_boss_harbinger_skyriss();
	AddSC_instance_arcatraz();

	// Burning Crusade Dungeons - Botanica
	AddSC_boss_high_botanist_freywinn();
	AddSC_boss_laj();
	AddSC_boss_warp_splinter();

	// Burning Crusade Dungeons - The Mechanar
	AddSC_boss_gatewatcher_iron_hand();
	AddSC_boss_nethermancer_sepethrea();
	AddSC_boss_pathaleon_the_calculator();
	AddSC_boss_mechano_lord_capacitus();
	AddSC_instance_mechanar();
	AddSC_mechanar();

	// Burning Crusade Dungeons - Old Hillsbrad
	AddSC_boss_captain_skarloc();
	AddSC_boss_epoch_hunter();
	AddSC_boss_lieutenant_drake();
	AddSC_instance_old_hillsbrad();
	AddSC_old_hillsbrad();

	// Burning Crusade Dungeons - The Dark Portal
	AddSC_boss_aeonus();
	AddSC_boss_chrono_lord_deja();
	AddSC_boss_temporus();
	AddSC_dark_portal();
	AddSC_instance_dark_portal();

	// Burning Crusade Dungeons - Magister's Terrace
	AddSC_boss_felblood_kaelthas();
	AddSC_boss_selin_fireheart();
	AddSC_boss_vexallus();
	AddSC_boss_priestess_delrissa();
	AddSC_instance_magisters_terrace();
	AddSC_magisters_terrace();

	// Burning Crusade Raids - Karazhan
	AddSC_boss_attumen();
	AddSC_boss_curator();
	AddSC_boss_maiden_of_virtue();
	AddSC_boss_shade_of_aran();
	AddSC_boss_malchezaar();
	AddSC_boss_terestian_illhoof();
	AddSC_boss_moroes();
	AddSC_bosses_opera();
	AddSC_boss_netherspite();
	AddSC_instance_karazhan();
	AddSC_karazhan();
	AddSC_boss_nightbane();

	// Burning Crusade Raids - Gruul's Lair
	AddSC_boss_gruul();
	AddSC_boss_high_king_maulgar();
	AddSC_instance_gruuls_lair();

	// Burning Crusade Raids - Magtheridon's Lair
	AddSC_boss_magtheridon();
	AddSC_instance_magtheridons_lair();

	// Burning Crusade Raids - The Eye
	AddSC_boss_alar();
	AddSC_boss_kaelthas();
	AddSC_boss_void_reaver();
	AddSC_boss_high_astromancer_solarian();
	AddSC_instance_the_eye();
	AddSC_the_eye();

	// Burning Crusade Raids - Black Temple
	AddSC_black_temple();
	AddSC_boss_illidan();
	AddSC_boss_shade_of_akama();
	AddSC_boss_supremus();
	AddSC_boss_gurtogg_bloodboil();
	AddSC_boss_mother_shahraz();
	AddSC_boss_reliquary_of_souls();
	AddSC_boss_teron_gorefiend();
	AddSC_boss_najentus();
	AddSC_boss_illidari_council();
	AddSC_instance_black_temple();

	// Burning Crusade Raids - Serpentshrine Caverns
	AddSC_boss_fathomlord_karathress();
	AddSC_boss_hydross_the_unstable();
	AddSC_boss_lady_vashj();
	AddSC_boss_leotheras_the_blind();
	AddSC_boss_morogrim_tidewalker();
	AddSC_instance_serpentshrine_cavern();
	AddSC_boss_the_lurker_below();

	// Burning Crusade Raids - Battle for Mountain Hyjal
	AddSC_hyjal();
	AddSC_boss_archimonde();
	AddSC_instance_mount_hyjal();
	AddSC_hyjal_trash();
	AddSC_boss_rage_winterchill();
	AddSC_boss_anetheron();
	AddSC_boss_kazrogal();
	AddSC_boss_azgalor();

	// Burning Crusade Raids - Sunwell Plateau
	AddSC_instance_sunwell_plateau();
	AddSC_boss_kalecgos();
	AddSC_boss_brutallus();
	AddSC_boss_felmyst();
	AddSC_boss_eredar_twins();
	AddSC_boss_muru();
	AddSC_boss_kiljaeden();
	AddSC_sunwell_plateau();

	// Burning Crusade Zones
	AddSC_blades_edge_mountains();
	AddSC_boss_doomlordkazzak();
	AddSC_boss_doomwalker();
	AddSC_hellfire_peninsula();
	AddSC_nagrand();
	AddSC_netherstorm();
	AddSC_shadowmoon_valley();
	AddSC_shattrath_city();
	AddSC_terokkar_forest();
	AddSC_zangarmarsh();
#endif
}

void AddNorthrendScripts()
{
#ifdef SCRIPTS

	// WotLK Zones
	AddSC_dalaran();
	AddSC_borean_tundra();
	AddSC_dragonblight();
	AddSC_grizzly_hills();
	AddSC_howling_fjord();
	AddSC_icecrown();
	AddSC_sholazar_basin();
	AddSC_storm_peaks();
	AddSC_wintergrasp();
	AddSC_zuldrak();
	AddSC_crystalsong_forest();
	AddSC_isle_of_conquest();

	// WotLK Dungeons - Gundrak
	AddSC_boss_slad_ran();
	AddSC_boss_moorabi();
	AddSC_boss_drakkari_colossus();
	AddSC_boss_gal_darah();
	AddSC_boss_eck();
	AddSC_instance_gundrak();
	AddSC_boss_amanitar();

	// WotLK Dungeons - Ahn'kahet
	AddSC_boss_taldaram();
	AddSC_boss_elder_nadox();
	AddSC_boss_jedoga_shadowseeker();
	AddSC_boss_volazj();
	AddSC_instance_ahnkahet();

	// WotLK Dungeons - Trial of the Champion
	AddSC_boss_argent_challenge();
	AddSC_boss_black_knight();
	AddSC_boss_grand_champions();
	AddSC_instance_trial_of_the_champion();
	AddSC_trial_of_the_champion();

	// WotLK Dungeons - Azjol-Nerub
	AddSC_boss_krik_thir();
	AddSC_boss_hadronox();
	AddSC_boss_anub_arak();
	AddSC_instance_azjol_nerub();

	// WotLK Dungeons - The Nexus
	AddSC_boss_magus_telestra();
	AddSC_boss_anomalus();
	AddSC_boss_ormorok();
	AddSC_boss_keristrasza();
	AddSC_boss_commander_nexus();
	AddSC_instance_nexus();

	// WotLK Dungeons - The Oculus
	AddSC_boss_drakos();
	AddSC_boss_urom();
	AddSC_boss_varos();
	AddSC_boss_eregos();
	AddSC_instance_oculus();
	AddSC_oculus();

	// WotLK Dungeons - Halls of Lightning
	AddSC_boss_bjarngrim();
	AddSC_boss_loken();
	AddSC_boss_ionar();
	AddSC_boss_volkhan();
	AddSC_instance_halls_of_lightning();

	// WotLK Dungeons - Halls of Stone
	AddSC_boss_maiden_of_grief();
	AddSC_boss_krystallus();
	AddSC_boss_sjonnir();
	AddSC_instance_halls_of_stone();
	AddSC_halls_of_stone();

	// WotLK Dungeons - Utgarde Keep
	AddSC_boss_keleseth();
	AddSC_boss_skarvald_dalronn();
	AddSC_boss_ingvar_the_plunderer();
	AddSC_instance_utgarde_keep();

	// WotLK Dungeons - Utgarde Pinnacle
	AddSC_boss_svala();
	AddSC_boss_palehoof();
	AddSC_boss_skadi();
	AddSC_boss_ymiron();
	AddSC_instance_utgarde_pinnacle();
	AddSC_utgarde_keep();

	// WotLK Dungeons - Vault of Archavon
	AddSC_boss_archavon();
	AddSC_boss_emalon();
	AddSC_boss_koralon();
	AddSC_boss_toravon();
	AddSC_instance_archavon();

	// WotLK Dungeons - Drak'tharon Keep
	AddSC_boss_trollgore();
	AddSC_boss_novos();
	AddSC_boss_dred();
	AddSC_boss_tharon_ja();
	AddSC_instance_drak_tharon();

	// WotLK Dungeons - Violet Hold
	AddSC_boss_cyanigosa();
	AddSC_boss_erekem();
	AddSC_boss_ichoron();
	AddSC_boss_lavanthor();
	AddSC_boss_moragg();
	AddSC_boss_xevozz();
	AddSC_boss_zuramat();
	AddSC_instance_violet_hold();
	AddSC_violet_hold();

	// WotLK Dungeons - Culling of Stratholme
	AddSC_boss_epoch();
	AddSC_boss_infinite_corruptor();
	AddSC_boss_salramm();
	AddSC_boss_mal_ganis();
	AddSC_boss_meathook();
	AddSC_culling_of_stratholme();
	AddSC_instance_culling_of_stratholme();

	// WotLK Dungeons - Forge of Souls
	AddSC_instance_forge_of_souls();
	AddSC_forge_of_souls();
	AddSC_boss_bronjahm();
	AddSC_boss_devourer_of_souls();

	// WotLK Dungeons - Pit of Saron
	AddSC_instance_pit_of_saron();
	AddSC_pit_of_saron();
	AddSC_boss_garfrost();
	AddSC_boss_ick();
	AddSC_boss_tyrannus();

	// WotLK Dungeons - Halls of Reflection
	AddSC_instance_halls_of_reflection();
	AddSC_halls_of_reflection();
	AddSC_boss_falric();
	AddSC_boss_marwyn();
	AddSC_boss_lich_king_hr();

	// WotLK Raids - Naxxramas
	AddSC_boss_anubrekhan();
	AddSC_boss_maexxna();
	AddSC_boss_patchwerk();
	AddSC_boss_grobbulus();
	AddSC_boss_razuvious();
	AddSC_boss_kelthuzad();
	AddSC_boss_loatheb();
	AddSC_boss_noth();
	AddSC_boss_gluth();
	AddSC_boss_sapphiron();
	AddSC_boss_four_horsemen();
	AddSC_boss_faerlina();
	AddSC_boss_heigan();
	AddSC_boss_gothik();
	AddSC_boss_thaddius();
	AddSC_instance_naxxramas();

	// WotLK Raids - Eye of Eternity
	AddSC_boss_malygos();
	AddSC_instance_eye_of_eternity();

	// WotLK Raids - Obsidian Sanctum
	AddSC_boss_sartharion();
	AddSC_instance_obsidian_sanctum();

	// WotLK Raids - Ulduar
	AddSC_instance_ulduar();
	AddSC_ulduar_teleporter();
	AddSC_ulduar_scripts();
	AddSC_boss_flame_leviathan();
	AddSC_boss_ignis();
	AddSC_boss_razorscale();
	AddSC_boss_xt002();
	AddSC_boss_assembly_of_iron();
	AddSC_boss_kologarn();
	AddSC_boss_auriaya();
	AddSC_boss_hodir();
	AddSC_boss_freya();
	AddSC_boss_mimiron();
	AddSC_boss_thorim();
	AddSC_boss_general_vezax();
	AddSC_boss_algalon_the_observer();

	// WotLK Raids - Onyxia's Lair
	AddSC_boss_onyxia();
	AddSC_instance_onyxias_lair();

	// WotLK Raids - Trial of the Crusader
	AddSC_boss_anubarak_trial();
	AddSC_boss_faction_champions();
	AddSC_boss_jaraxxus();
	AddSC_trial_of_the_crusader();
	AddSC_boss_twin_valkyr();
	AddSC_boss_northrend_beasts();
	AddSC_instance_trial_of_the_crusader();

	// WotLK Raids - Icecrown Citadel
	AddSC_instance_icecrown_citadel();
	AddSC_icecrown_citadel();
	AddSC_icecrown_citadel_teleport();
	AddSC_boss_lord_marrowgar();
	AddSC_boss_lady_deathwhisper();
	AddSC_boss_gunship_battle();
	AddSC_boss_deathbringer_saurfang();
	AddSC_boss_festergut();
	AddSC_boss_rotface();
	AddSC_boss_professor_putricide();
	AddSC_boss_blood_prince_council();
	AddSC_boss_blood_queen_lana_thel();
	AddSC_boss_valithria_dreamwalker();
	AddSC_boss_sindragosa();
	AddSC_boss_the_lich_king();

	// WotLK Raids - Ruby Sanctum
	AddSC_instance_ruby_sanctum();
	AddSC_ruby_sanctum();
	AddSC_boss_baltharus_the_warborn();
	AddSC_boss_saviana_ragefire();
	AddSC_boss_general_zarithrian();
	AddSC_boss_halion();

#endif
}

void AddPandarieScripts()
{
#ifdef SCRIPTS

	// Pandaria Zones
	AddSC_townlong_steppes();
	AddSC_dread_wastes();
	AddSC_valley_of_the_four_winds();
	AddSC_krasarang_wilds();
	AddSC_jade_forest();
	AddSC_kun_lai_summit();
	AddSC_vale_of_eternal_blossoms();
	AddSC_timeless_isle();
	AddSC_zone_timeless_isle();
	AddSC_WanderingIsland_North();
	AddSC_WanderingIsland_East();
	AddSC_WanderingIsland_West();
	AddSC_WanderingIsland_South();

	// Pandaria Dungeons - Temple of Jade Serpent
	AddSC_instance_temple_of_jade_serpent();
	AddSC_boss_wise_mari();
	AddSC_boss_lorewalker_stonestep();
	AddSC_boss_liu_flameheat();
	AddSC_boss_sha_of_doubt();

	// Pandaria Dungeons - The Stormstout Brewery
	AddSC_boss_hoptallus();
	AddSC_boss_ook_ook();
	AddSC_boss_yan_zhu_the_uncasked();
	AddSC_instance_stormstout_brewery();
	AddSC_stormstout_brewery();

	// Pandaria Dungeons - The Mogu'shan Palace
	AddSC_instance_mogu_shan_palace();
	AddSC_boss_trial_of_the_king();
	AddSC_boss_gekkan();
	AddSC_boss_xin_the_weaponmaster();

	// Pandaria Dungeons - Gate of the Setting Sun
	AddSC_instance_gate_setting_sun();
	AddSC_gate_setting_sun();
	AddSC_boss_saboteur_kiptilak();
	AddSC_boss_striker_gadok();
	AddSC_boss_commander_rimok();
	AddSC_boss_raigonn();

	// Pandaria Dungeons - Shadopan Monastery
	AddSC_instance_shadopan_monastery();
	AddSC_shadopan_monastery();
	AddSC_boss_gu_cloudstrike();
	AddSC_boss_master_snowdrift();
	AddSC_boss_sha_of_violence();
	AddSC_boss_taran_zhu();

	// Pandaria Dungeons - Shadopan Monastery
	AddSC_siege_of_the_niuzoa_temple();
	AddSC_boss_jinbak();
	AddSC_WING_LEADER();
	AddSC_boss_vojak();
	AddSC_boss_general_pavalak();

	// Pandaria World Bosses
	AddSC_boss_sha_of_anger();
	AddSC_boss_galion();
	AddSC_boss_chi_ji();
	AddSC_boss_nalak();
	AddSC_boss_ordos();

	// Pandaria Raids - Mogu'shan Vaults
	AddSC_instance_mogu_shan_vault();
	AddSC_mogu_shan_vault();
	AddSC_boss_stone_guard();
	AddSC_boss_feng();
	AddSC_boss_garajal();
	AddSC_boss_spirit_kings();
	AddSC_boss_elegon();
	AddSC_boss_will_of_emperor();

	// Pandaria Raids - Heart of Fear
	AddSC_Heart_Of_Fear();
	AddSC_imperial_zorlok();
	AddSC_Lord_Tayak();
	AddSC_lord_meljarak();
	AddSC_boss_garalon();
	AddSC_empress_shekzeer();

	// Pandaria Raids - Terrace of Endless Spring
	AddSC_instance_terrace_of_endless_spring();
	AddSC_terrace_of_endless_spring();
	AddSC_boss_protectors_of_the_endless();
	AddSC_boss_tsulong();
	AddSC_boss_lei_shi();
	AddSC_boss_sha_of_fear();

	// Pandaria Raids - Throne of Thunder
	AddSC_boss_horridon();
	AddSC_boss_jin_rokh_breaker();
	AddSC_boss_ji_kun();
	AddSC_boss_durumu();
	AddSC_megaera();
	AddSC_tortos();
	AddSC_council();
	AddSC_instance_throne_of_thunder();

	// Pandaria Raids - Siege of Orgrimmar
	AddSC_siege_of_orgrimmar();
	AddSC_instance_siege_of_orgrimmar();
	AddSC_immersus();
	AddSC_fallen_protectors();
	AddSC_norushen();
	AddSC_sha_of_pride();
	AddSC_boss_iron_juggernaut();
	AddSC_boss_dark_shaman();
	AddSC_boss_general_nazgrim();
	AddSC_boss_malkorok();
	AddSC_garrosh_hellscream();

#endif
}

void AddOutdoorPvPScripts()
{
#ifdef SCRIPTS
	AddSC_outdoorpvp_hp();
	AddSC_outdoorpvp_na();
	AddSC_outdoorpvp_si();
	AddSC_outdoorpvp_tf();
	AddSC_outdoorpvp_zm();
#endif
}

void AddBattlegroundScripts()
{
#ifdef SCRIPTS
#endif
}

#ifdef SCRIPTS
/* This is where custom scripts' loading functions should be declared. */

#endif

void AddCustomScripts()
{
#ifdef SCRIPTS
	
#endif
}