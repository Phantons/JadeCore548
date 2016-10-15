SET @UM_ENTRY = 1568;
SET @RR_ENTRY = 50414;
SET @D1_ENTRY = 49141;
SET @MR_ENTRY = 49230;
SET @LV_ENTRY = 38895;
SET @VM_ENTRY = 49231;
SET @MZ_ENTRY = 1501;
SET @C_A_ENTRY = 49129;
SET @C_A_GUID = 988269;
SET @C_D_ENTRY = 49128;
SET @C_D_GUID = 204125;

UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_undertaker_mordo" WHERE `entry`=@UM_ENTRY;
UPDATE `creature_template` SET `AIName`="",`equipment_id`=@D1_ENTRY,`ScriptName`="npc_darnell_script_1" WHERE `entry`=@D1_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_marshal_redpath", `gossip_menu_id`=12485 WHERE `entry`=@MR_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_lilian_voss" WHERE `entry`=@LV_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_valdred_moray", `gossip_menu_id`=12488 WHERE `entry`=@VM_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_mindless_zombie" WHERE `entry`=@MZ_ENTRY;
UPDATE `creature_template_addon` SET `bytes1`=8, `emote`=0 WHERE `entry`=@VM_ENTRY;
DELETE FROM `creature_equip_template` WHERE (`entry`=@D1_ENTRY);
INSERT INTO `creature_equip_template` (`entry`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(@D1_ENTRY, 1895, 1957, 0);
UPDATE `creature_template` SET `unit_flags`=32776, `faction_A`=5, `faction_H`=5 WHERE `entry`=@D1_ENTRY;
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (@C_A_ENTRY, @C_D_ENTRY);
UPDATE `creature` SET `MovementType`=1,`spawndist`=2 WHERE `id`=@MZ_ENTRY;
UPDATE `creature` SET `MovementType`=2,`spawndist`=0 WHERE `guid` IN (@C_A_GUID, @C_D_GUID);
UPDATE `creature_template_addon` SET `path_id`=@C_A_GUID WHERE `entry`=@C_A_ENTRY;
UPDATE `creature_template_addon` SET `path_id`=@C_D_GUID WHERE `entry`=@C_D_ENTRY;
UPDATE `creature` SET `orientation`=5.8555 WHERE `id`=@MR_ENTRY;
DELETE FROM `creature_text` WHERE (`entry`=@D1_ENTRY);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@D1_ENTRY, 0, 0, 'Greetings.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 1, 0, 'The shadow grave is in this direction. Follow me.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 2, 0, 'This way!', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 3, 0, 'Let''s see now... WHERE could they be...', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 4, 0, 'Maybe they''re over here?', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 5, 0, 'Nice work! You''ve found them. Let''s bring these back to Mordo.', 12, 0, 100, 1, 0, 0, 'Darnell'),
(@D1_ENTRY, 6, 0, 'I saw someone up there whose jaw fell off. I wonder if Mordo can fix him up?', 12, 0, 100, 1, 0, 0, 'Darnell');
UPDATE `creature_template` SET `AIName`="" WHERE `entry`=@RR_ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@UM_ENTRY, @RR_ENTRY, @D1_ENTRY, @MR_ENTRY, @LV_ENTRY, @VM_ENTRY) AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@RR_ENTRY*100, @D1_ENTRY*100, (@D1_ENTRY*100)+1) AND `source_type`=9;
DELETE FROM `waypoints` WHERE entry IN (@D1_ENTRY*100, @RR_ENTRY*100, 5037402, 5037401);
DELETE FROM `waypoint_data` WHERE `id` IN (@RR_ENTRY*100, (@RR_ENTRY*100)+1, @MR_ENTRY*100, @LV_ENTRY*100, @VM_ENTRY*100, @C_A_GUID, @C_D_GUID);
INSERT INTO `waypoint_data`(`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`) VALUES
((@RR_ENTRY*100)+1, 1, 1691.29, 1675.85, 135.295, 0, 0, 0),
((@RR_ENTRY*100)+1, 2, 1690.59, 1661.87, 131.92, 0, 0, 0),
((@RR_ENTRY*100)+1, 3, 1719.38, 1633.98, 121.065, 0, 0, 0),
(@RR_ENTRY*100, 1, 1694.16, 1677.08, 134.786, 0, 0, 0),
(@RR_ENTRY*100, 2, 1691.29, 1675.84, 135.295, 0, 0, 0),
(@MR_ENTRY*100, 1, 1752.72, 1626.94, 116.001, 0, 0, 1),
(@MR_ENTRY*100, 2, 1758.17, 1604.42, 110.959, 0, 0, 1),
(@MR_ENTRY*100, 3, 1756.88, 1585.49, 111.543, 0, 0, 1),
(@MR_ENTRY*100, 4, 1751.61, 1564.8, 113.489, 0, 0, 1),
(@MR_ENTRY*100, 5, 1752.27, 1548.56, 113.92, 0, 0, 1),
(@MR_ENTRY*100, 6, 1763.14, 1538.02, 114.346, 0, 0, 1),
(@LV_ENTRY*100, 1, 1752.21, 1652.93, 119.551, 0, 0, 1),
(@LV_ENTRY*100, 2, 1752.72, 1635.44, 116.883, 0, 0, 1),
(@LV_ENTRY*100, 3, 1755.51, 1611.9, 112.498, 0, 0, 1),
(@LV_ENTRY*100, 4, 1756.03, 1591.45, 111.872, 0, 0, 1),
(@LV_ENTRY*100, 5, 1755.69, 1570.22, 112.78, 0, 0, 1),
(@LV_ENTRY*100, 6, 1751.78, 1550.02, 114.082, 0, 0, 1),
(@LV_ENTRY*100, 7, 1762.13, 1539.01, 114.289, 0, 0, 1),
(@VM_ENTRY*100, 1, 1728.97, 1696.68, 127.857, 0, 0, 1),
(@VM_ENTRY*100, 2, 1716.91, 1690.64, 132.728, 0, 0, 1),
(@VM_ENTRY*100, 3, 1707.91, 1685.12, 133.644, 0, 0, 1),
(@VM_ENTRY*100, 4, 1695.44, 1677.7, 134.632, 0, 0, 1),
(@C_A_GUID, 1, 1725.01, 1667.63, 137.94, 0, 0, 0),
(@C_A_GUID, 2, 1710.03, 1656.84, 140.835, 0, 0, 0),
(@C_A_GUID, 3, 1698.03, 1652.92, 146.397, 0, 0, 0),
(@C_A_GUID, 4, 1702.94, 1634.51, 140.377, 0, 0, 0),
(@C_A_GUID, 5, 1716.34, 1617.19, 134.848, 0, 0, 0),
(@C_A_GUID, 6, 1731.1, 1618.59, 133.88, 0, 0, 0),
(@C_A_GUID, 7, 1747.64, 1639.03, 128.976, 0, 0, 0),
(@C_A_GUID, 8, 1746.05, 1657.5, 131.738, 0, 0, 0),
(@C_A_GUID, 9, 1744.97, 1676.7, 139.807, 0, 0, 0),
(@C_A_GUID, 10, 1737.38, 1689.39, 142.346, 0, 0, 0),
(@C_A_GUID, 11, 1721.84, 1699.9, 145.727, 0, 0, 0),
(@C_A_GUID, 12, 1702.88, 1701.67, 147.952, 0, 0, 0),
(@C_A_GUID, 13, 1684.23, 1697.14, 151.702, 0, 0, 0),
(@C_A_GUID, 14, 1679.76, 1677.61, 157.186, 0, 0, 0),
(@C_A_GUID, 15, 1688.75, 1665.62, 152.315, 0, 0, 0),
(@C_A_GUID, 16, 1706.31, 1666.57, 144.594, 0, 0, 0),
(@C_A_GUID, 17, 1721.01, 1675.69, 138.709, 0, 0, 0),
(@C_D_GUID, 1, 1757.78, 1593.55, 116.917, 0, 0, 0),
(@C_D_GUID, 2, 1758.89, 1604.89, 127.626, 0, 0, 0),
(@C_D_GUID, 3, 1754.94, 1620.19, 131.002, 0, 0, 0),
(@C_D_GUID, 4, 1741.96, 1635.04, 141.067, 0, 0, 0),
(@C_D_GUID, 5, 1722.93, 1649.94, 144.151, 0, 0, 0),
(@C_D_GUID, 6, 1703.4, 1655.41, 156.222, 0, 0, 0),
(@C_D_GUID, 7, 1683.45, 1647.61, 158.232, 0, 0, 0),
(@C_D_GUID, 8, 1678.86, 1635.12, 154.974, 0, 0, 0),
(@C_D_GUID, 9, 1681.15, 1625.05, 153.188, 0, 0, 0),
(@C_D_GUID, 10, 1699.46, 1616.94, 146.098, 0, 0, 0),
(@C_D_GUID, 11, 1701.61, 1601.05, 142.797, 0, 0, 0),
(@C_D_GUID, 12, 1697.91, 1578.1, 144.346, 0, 0, 0),
(@C_D_GUID, 13, 1705.4, 1557.76, 143.995, 0, 0, 0),
(@C_D_GUID, 14, 1719.17, 1551.01, 136.678, 0, 0, 0),
(@C_D_GUID, 15, 1736.93, 1555, 129.218, 0, 0, 0),
(@C_D_GUID, 16, 1746.94, 1565.07, 123.072, 0, 0, 0),
(@C_D_GUID, 17, 1755.16, 1577.36, 119.816, 0, 0, 0);
UPDATE `gossip_menu_option` SET `action_menu_id`=12487 WHERE `menu_id`=12488;
UPDATE `gossip_menu_option` SET `action_menu_id`=12489 WHERE `menu_id`=12487;
UPDATE `gossip_menu_option` SET `action_menu_id`=0 WHERE `menu_id`=12489;
UPDATE `gossip_menu` SET `text_id`=17569 WHERE `entry`=12488;
UPDATE `gossip_menu` SET `text_id`=17570 WHERE `entry`=12487;
UPDATE `npc_text` SET `text0_0`="I... died?$b$bYes, you're right.  I died.  It was an orc... he cut off my hands, and left me to die.$b$b<Valdred looks down at his hands.>$b$bThese aren't my hands!  THESE AREN'T MY HANDS!" WHERE `ID`=17570;
