UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN (48624, 25063);
UPDATE `creature_template_addon` SET `bytes1`=5, `bytes2`=1, `emote`=426 WHERE `entry`=8583;
UPDATE `creature` SET `MovementType`=0,`spawndist`=0 WHERE `id` IN (44617, 7690, 39251);
UPDATE `creature` SET `MovementType`=1,`spawndist`=2 WHERE `id` IN (3098);

UPDATE `quest_template` SET `PrevQuestId`=0, `NextQuestId`=25126, `NextQuestIdChain`=25126 WHERE `Id`=25152;
UPDATE `quest_template` SET `PrevQuestId`=25152, `NextQuestId`=25172, `NextQuestIdChain`=25172 WHERE `Id`=25126;
UPDATE `quest_template` SET `PrevQuestId`=25126, `NextQuestId`=25127, `NextQuestIdChain`=25127 WHERE `Id`=25172;
UPDATE `quest_template` SET `PrevQuestId`=25172, `NextQuestId`=0, `NextQuestIdChain`=0 WHERE `Id`=25127;

UPDATE `quest_template` SET `PrevQuestId`=0, `NextQuestId`=25132, `NextQuestIdChain`=25132 WHERE `Id`=25131;
UPDATE `quest_template` SET `PrevQuestId`=25131, `NextQuestId`=25133, `NextQuestIdChain`=25133 WHERE `Id`=25132;
UPDATE `quest_template` SET `PrevQuestId`=25132, `NextQuestId`=25167, `NextQuestIdChain`=25167 WHERE `Id`=25133;
UPDATE `quest_template` SET `PrevQuestId`=25133, `NextQuestId`=25168, `NextQuestIdChain`=25168 WHERE `Id`=25167;
UPDATE `quest_template` SET `PrevQuestId`=25167, `NextQuestId`=25171, `NextQuestIdChain`=25171 WHERE `Id`=25168;
UPDATE `quest_template` SET `PrevQuestId`=25168, `NextQuestId`=0, `NextQuestIdChain`=0 WHERE `Id`=25171;
