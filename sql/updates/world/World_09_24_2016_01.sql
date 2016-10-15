UPDATE `creature_template` SET `ScriptName` = 'boss_general_nazgrim' WHERE `entry` = 71515;
UPDATE `creature_template` SET `ScriptName` = 'mob_orgrimmar_faithful' WHERE `entry` = 71715;
UPDATE `creature_template` SET `ScriptName` = 'mob_korkron_ironblade' WHERE `entry` = 71516;
UPDATE `creature_template` SET `ScriptName` = 'mob_korkron_arcweaver' WHERE `entry` = 71517;
UPDATE `creature_template` SET `ScriptName` = 'mob_korkron_assassin' WHERE `entry` = 71518;
UPDATE `creature_template` SET `ScriptName` = 'mob_korkron_warshaman' WHERE `entry` = 71519;
UPDATE `creature_template` SET `ScriptName` = 'mob_aftershock' WHERE `entry` = 71697;
UPDATE `creature_template` SET `ScriptName` = 'mob_korkron_banner' WHERE `entry` = 71626;
UPDATE `creature_template` SET `ScriptName` = 'mob_healing_tide_totem' WHERE `entry` = 71610;
UPDATE `creature_template` SET `ScriptName` = 'mob_ravager' WHERE `entry` = 71762;

DELETE FROM `spell_script_names` WHERE `spell_id` = 143503;
DELETE FROM `spell_script_names` WHERE `spell_id` = 143494;

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (143503, 'spell_war_song');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (143494, 'spell_sundering_blow');