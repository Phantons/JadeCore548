SET @A_ENTRY = 50372;
SET @DGT_ENTRY = 50373;
SET @RD_ENTRY = 50374;

UPDATE `creature` SET `MovementType`=2,`spawndist`=0, `position_x`=1674.95, `position_y`=1683.32, `position_z`=139.203 WHERE `id`=@A_ENTRY;
UPDATE `creature` SET `MovementType`=0,`spawndist`=0 WHERE `id`=@DGT_ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (@A_ENTRY, @DGT_ENTRY, @RD_ENTRY) AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@A_ENTRY*100 AND `source_type`=9;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_aradne" WHERE `entry`=@A_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_deathknell_grave_target" WHERE `entry`=@DGT_ENTRY;
UPDATE `creature_template` SET `AIName`="", `scriptname`="npc_risen_dead" WHERE `entry`=@RD_ENTRY;
UPDATE `creature_template_addon` SET `path_id`=@A_ENTRY*100 WHERE `entry`=@A_ENTRY;
UPDATE `creature_template_addon` SET `auras`=NULL WHERE `entry`=@RD_ENTRY;
DELETE FROM creature_text WHERE entry=@RD_ENTRY;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(@RD_ENTRY, 0, 0, 'What... what''s happening to me? Why have you done this to me?', 12, 0, 100, 6, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 1, 0, 'I should be dead and to death I will return!', 12, 0, 100, 5, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 2, 0, 'I never asked for this! Leave me alone!', 12, 0, 100, 5, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 3, 0, 'What magic is this that turns back the hand of death?', 12, 0, 100, 6, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 4, 0, 'If the Banshee Queen has offered me this chance, I will gladly serve.', 12, 0, 100, 1, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 5, 0, 'This is Lady Sylvanas''s doing?', 12, 0, 100, 6, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 6, 0, 'What use has the Dark Lady for me?', 12, 0, 100, 6, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 7, 0, 'I... I don''t remember... Why have you done this?', 12, 0, 100, 6, 0, 0, 'Risen Dead'),
(@RD_ENTRY, 8, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Risen Dead');
DELETE FROM `waypoint_data` WHERE `id` IN (@A_ENTRY*100, @RD_ENTRY*100, (@RD_ENTRY*100)+1);
INSERT INTO `waypoint_data`(`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`) VALUES
(@A_ENTRY*100, 1, 1674.95, 1683.32, 139.203, 0, 0, 0),
(@A_ENTRY*100, 2, 1676.98, 1681.87, 138.799, 0, 0, 0),
(@A_ENTRY*100, 3, 1679.82, 1681.69, 138.267, 0, 0, 0),
(@A_ENTRY*100, 4, 1681.72, 1683.03, 137.959, 0, 0, 0),
(@A_ENTRY*100, 5, 1683.93, 1685.34, 137.606, 2.383, 10000, 0),
(@A_ENTRY*100, 6, 1686.35, 1686.91, 137.154, 0, 0, 0),
(@A_ENTRY*100, 7, 1688.19, 1690.18, 136.926, 0, 0, 0),
(@A_ENTRY*100, 8, 1689.24, 1693.53, 136.947, 0, 0, 0),
(@A_ENTRY*100, 9, 1691.95, 1695.14, 136.673, 0, 0, 0),
(@A_ENTRY*100, 10, 1695.06, 1695.14, 136.008, 0, 0, 0),
(@A_ENTRY*100, 11, 1697.34, 1693.19, 135.244, 0, 0, 0),
(@A_ENTRY*100, 12, 1698, 1691.2, 135.049, 0, 0, 0),
(@A_ENTRY*100, 13, 1698.51, 1688.87, 134.864, 3.184, 10000, 0),
(@A_ENTRY*100, 14, 1698.55, 1685.78, 134.671, 0, 0, 0),
(@A_ENTRY*100, 15, 1698.47, 1683.06, 134.616, 0, 0, 0),
(@A_ENTRY*100, 16, 1697.66, 1680.8, 134.579, 3.011, 10000, 0),
(@A_ENTRY*100, 17, 1699.37, 1684.32, 134.532, 0, 0, 0),
(@A_ENTRY*100, 18, 1702.7, 1690.31, 134.334, 0, 0, 0),
(@A_ENTRY*100, 19, 1704.96, 1695.01, 134.167, 0, 0, 0),
(@A_ENTRY*100, 20, 1706.95, 1696.84, 133.934, 0, 0, 0),
(@A_ENTRY*100, 21, 1707.11, 1700.6, 134.225, 1.44, 10000, 0),
(@A_ENTRY*100, 22, 1710.43, 1700.99, 133.2, 0, 0, 0),
(@A_ENTRY*100, 23, 1716.15, 1701.67, 131.765, 0, 0, 0),
(@A_ENTRY*100, 24, 1721.19, 1702.27, 130.121, 0, 0, 0),
(@A_ENTRY*100, 25, 1726.76, 1703.41, 128.53, 0, 0, 0),
(@A_ENTRY*100, 26, 1730.24, 1703.47, 128.115, 0, 0, 0),
(@A_ENTRY*100, 27, 1733.31, 1703.32, 128.178, 1.44, 10000, 0),
(@A_ENTRY*100, 28, 1726.35, 1702.37, 128.503, 0, 0, 0),
(@A_ENTRY*100, 29, 1719.89, 1698.69, 130.855, 0, 0, 0),
(@A_ENTRY*100, 30, 1710.81, 1691.48, 133.626, 0, 0, 0),
(@A_ENTRY*100, 31, 1703.63, 1685.48, 134.024, 0, 0, 0),
(@A_ENTRY*100, 32, 1697.91, 1681.32, 134.569, 3.235, 10000, 0),
(@A_ENTRY*100, 33, 1698.1, 1683.77, 134.69, 0, 0, 0),
(@A_ENTRY*100, 34, 1698.15, 1688.19, 134.898, 3.027, 10000, 0),
(@A_ENTRY*100, 35, 1699.09, 1691.26, 134.879, 0, 0, 0),
(@A_ENTRY*100, 36, 1701.96, 1695.36, 134.619, 0, 0, 0),
(@A_ENTRY*100, 37, 1705.86, 1696.63, 134.068, 0, 0, 0),
(@A_ENTRY*100, 38, 1706.74, 1700.46, 134.263, 1.378, 10000, 0),
(@A_ENTRY*100, 39, 1700.76, 1698.66, 135.131, 0, 0, 0),
(@A_ENTRY*100, 40, 1696.39, 1697.44, 135.9, 0, 0, 0),
(@A_ENTRY*100, 41, 1691.82, 1695.35, 136.723, 0, 0, 0),
(@A_ENTRY*100, 42, 1687.56, 1692.08, 137.147, 0, 0, 0),
(@A_ENTRY*100, 43, 1686.83, 1690.55, 137.241, 0, 0, 0),
(@RD_ENTRY*100, 1, 1702, 1677.55, 134.298, 0, 0, 1),
(@RD_ENTRY*100, 2, 1693.66, 1670.64, 134.032, 0, 0, 1),
(@RD_ENTRY*100, 3, 1686.21, 1661.74, 133.171, 0, 0, 1),
(@RD_ENTRY*100, 4, 1681.66, 1655.15, 136.316, 0, 0, 1),
(@RD_ENTRY*100, 5, 1676.76, 1643.16, 139.253, 0, 0, 1),
(@RD_ENTRY*100, 6, 1669.11, 1631.08, 139.495, 0, 0, 1),
(@RD_ENTRY*100, 7, 1664.203, 1620.63, 140.526, 0, 0, 1),
((@RD_ENTRY*100)+1, 1, 1695.44, 1672.29, 133.945, 0, 0, 0),
((@RD_ENTRY*100)+1, 2, 1691.08, 1660.22, 131.406, 0, 0, 0),
((@RD_ENTRY*100)+1, 3, 1709.56, 1643.45, 124.713, 0, 0, 0),
((@RD_ENTRY*100)+1, 4, 1723.48, 1633.31, 120.426, 0, 0, 0);
