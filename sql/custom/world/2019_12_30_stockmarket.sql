/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50562
Source Host           : localhost:3306
Source Database       : world2

Target Server Type    : MYSQL
Target Server Version : 50562
File Encoding         : 65001

Date: 2019-12-30 17:39:52
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for stocks
-- ----------------------------
DROP TABLE IF EXISTS `stocks`;
CREATE TABLE `stocks` (
  `iD` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'Creature Identifier',
  `Scale` int(10) unsigned NOT NULL DEFAULT '120',
  `OwnedRatio` float NOT NULL DEFAULT '0',
  `Worth` int(10) unsigned NOT NULL DEFAULT '120' COMMENT 'in gold',
  `UpdateTime` int(10) NOT NULL,
  `comments` text NOT NULL,
  PRIMARY KEY (`iD`),
  KEY `idx_id` (`iD`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Creature System';

-- ----------------------------
-- Records of stocks
-- ----------------------------
INSERT INTO `stocks` VALUES ('0', '1', '0', '10', '18542578', '');
INSERT INTO `stocks` VALUES ('1', '1', '0', '10', '0', 'Ravenholdt Manor');
INSERT INTO `stocks` VALUES ('2', '1', '0', '10', '0', 'Bloodsail');
INSERT INTO `stocks` VALUES ('3', '1', '0', '10', '0', 'Steamwheedle Cartel');
INSERT INTO `stocks` VALUES ('4', '1', '0', '10', '0', 'Theramore Silk Traders');
INSERT INTO `stocks` VALUES ('5', '1', '0', '10', '0', 'Eldoar\'norore');
