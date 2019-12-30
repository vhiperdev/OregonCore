/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50562
Source Host           : localhost:3306
Source Database       : characters2

Target Server Type    : MYSQL
Target Server Version : 50562
File Encoding         : 65001

Date: 2019-12-30 17:39:32
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for character_stockdata
-- ----------------------------
DROP TABLE IF EXISTS `character_stockdata`;
CREATE TABLE `character_stockdata` (
  `iD` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Creature Identifier',
  `1` float NOT NULL DEFAULT '0' COMMENT 'Ravenholdt Manor',
  `2` float NOT NULL DEFAULT '0' COMMENT 'Bloodsail',
  `3` float NOT NULL DEFAULT '0' COMMENT 'Steamwheedle Cartel',
  `4` float NOT NULL DEFAULT '0' COMMENT 'Theramore Silk Traders',
  `5` float NOT NULL DEFAULT '0' COMMENT 'Eldoar''norore',
  PRIMARY KEY (`iD`),
  KEY `idx_id` (`iD`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Creature System';

-- ----------------------------
-- Records of character_stockdata
-- ----------------------------
INSERT INTO `character_stockdata` VALUES ('1', '0', '0', '0', '0', '0');
