-- Dark Iron Land Mine
SET @ENTRY := 8035;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,1,4,0,100,3,0,0,0,0,21,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Stop Moving on Aggro'),
(@ENTRY,0,1,0,61,0,100,3,0,0,0,0,20,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Stop Attack on Aggro'),
(@ENTRY,0,2,3,9,0,100,3,0,10,0,0,11,4043,0,0,0,0,0,1,0,0,0,0,0,0,0,'Cast Detonation on Close'),
(@ENTRY,0,3,0,61,0,100,3,0,0,0,0,41,500,0,0,0,0,0,1,0,0,0,0,0,0,0,'Despawn');


DELETE FROM `creature` WHERE `id`=8035;

-- Dark Iron Agent
SET @ENTRY := 6212;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,9,0,100,2,0,5,15000,25000,11,11802,0,0,0,0,0,2,0,0,0,0,0,0,0,'Cast Dark Iron Land Mine on Close');

-- Walking Bomb
SET @ENTRY := 7915;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid`=@ENTRY;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,3,9,0,100,3,0,10,0,0,11,4043,0,0,0,0,0,1,0,0,0,0,0,0,0,'Cast Detonation on Close'),
(@ENTRY,0,1,0,61,0,100,3,0,0,0,0,41,500,0,0,0,0,0,1,0,0,0,0,0,0,0,'Despawn');





