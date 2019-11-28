--
-- Dumping data for table `creature_template`
--

DELETE FROM `creature_template` WHERE `entry` = 91000;
DELETE FROM `creature_template` WHERE `entry` = 91001;
DELETE FROM `creature_template` WHERE `entry` = 91002;

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES 
('91000', '0', '0', '0', '3348', '0', '3348', '0', 'Alaresque', 'Supreme Mage', '', '0', '60', '60', '0', '3052', '3052', '0', '0', '0', '35', '1', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '0', '1', '0', '1', '0', '0', '0', 'npc_buff'),

('91001', '0', '0', '0', '3486', '0', '3486', '0', 'Florana Nightingly', 'The Crimson Cross', '', '0', '60', '60', '0', '3052', '3052', '0', '0', '0', '35', '1', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '0', '1', '0', '1', '0', '0', '0', 'npc_heal'),

('91002', '0', '0', '0', '10746', '0', '10746', '0', 'Flo Nizzengale', 'The Crimson Cross', '', '0', '60', '60', '0', '3052', '3052', '0', '0', '0', '35', '1', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '0', '1', '0', '1', '0', '0', '0', 'npc_heal');