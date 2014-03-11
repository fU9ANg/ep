# SQL-Front 5.1  (Build 4.16)

/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE */;
/*!40101 SET SQL_MODE='' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES */;
/*!40103 SET SQL_NOTES='ON' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;


# Host: 192.168.0.254    Database: epdb 
# ------------------------------------------------------
# Server version 5.5.29-0ubuntu0.12.04.1

DROP DATABASE IF EXISTS `epdb`;
CREATE DATABASE `epdb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `epdb`;

# ======================================================
#
# Source for table func(功能表)
#
DROP TABLE IF EXISTS `func`;
CREATE TABLE `func` (
    `func_id` int (11) NOT NULL,
    `func_name` char (40) NOT NULL,
    `res_path` char (128) NOT NULL,
    PRIMARY KEY (`func_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table func 
#
LOCK TABLES `func` WRITE;
/*!40000 ALTER TABLE `func` DISABLE KEYS */;
INSERT INTO `func` VALUES (1,'个人','/var/resources/abc.png');
INSERT INTO `func` VALUES (2,'学校','/var/resources/deg.png');
INSERT INTO `func` VALUES (3,'家庭','1.png');
INSERT INTO `func` VALUES (4,'好友','2.png');
INSERT INTO `func` VALUES (5,'办公室','2.png');
INSERT INTO `func` VALUES (6,'教材室','2.png');
/*!40000 ALTER TABLE `func` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table client_type(客户端类型表)
#
DROP TABLE IF EXISTS `client_type`;
CREATE TABLE `client_type` (
    `client_type_id` int (11) NOT NULL,
    `client_type_name` char (40) NOT NULL,
    PRIMARY KEY (`client_type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table client_type 
#
LOCK TABLES `client_type` WRITE;
/*!40000 ALTER TABLE `client_type` DISABLE KEYS */;
INSERT INTO `client_type` VALUES (1,'院长');
INSERT INTO `client_type` VALUES (2,'教师');
INSERT INTO `client_type` VALUES (3,'家长');
INSERT INTO `client_type` VALUES (4,'学生');
INSERT INTO `client_type` VALUES (5,'白板');
/*!40000 ALTER TABLE `client_type` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table client_type_func(客户端功能关系表)
#
DROP TABLE IF EXISTS `client_type_func`;
CREATE TABLE `client_type_func` (
    `client_type_id` int (11) NOT NULL,
    `func_id` int (11) NOT NULL,
    PRIMARY KEY (`func_id`, `client_type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table client_type_func 
#
LOCK TABLES `client_type_func` WRITE;
/*!40000 ALTER TABLE `client_type_func` DISABLE KEYS */;
INSERT INTO `client_type_func` VALUES (2,2);
INSERT INTO `client_type_func` VALUES (2,5);
INSERT INTO `client_type_func` VALUES (4,1);
/*!INSERT INTO `client_type_func` VALUES (4,2)*/;
INSERT INTO `client_type_func` VALUES (1,2);
INSERT INTO `client_type_func` VALUES (1,6);
/*!40000 ALTER TABLE `client_type_func` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table course_group (课件组表)
#
DROP TABLE IF EXISTS `course_group`;
CREATE TABLE `course_group` (
    `group_id` int (11) NOT NULL,
    `group_name` char (40) NOT NULL,
    PRIMARY KEY (`group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table course_group
#
LOCK TABLES `course_group` WRITE;
/*!40000 ALTER TABLE `course_group` DISABLE KEYS */;
INSERT INTO `course_group` VALUES (1, '绘本');
INSERT INTO `course_group` VALUES (2, '绘画');
INSERT INTO `course_group` VALUES (3, '连线');
INSERT INTO `course_group` VALUES (4, '拼图');
INSERT INTO `course_group` VALUES (5, '语音');
/*!40000 ALTER TABLE `course_group` ENABLE KEYS */;
/*!40000 ALTER TABLE `course_group` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table course_type (课程类型表)
#
DROP TABLE IF EXISTS `course_type`;
CREATE TABLE `course_type` (
        `course_type_id` int (11) NOT NULL AUTO_INCREMENT,
        `course_type_name` char (40) NOT NULL,
        PRIMARY KEY (`course_type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table course_type
#
LOCK TABLES `course_type` WRITE;
/*!40000 ALTER TABLE `course_type` DISABLE KEYS */;
INSERT INTO `course_type` VALUES (1,'默认');
INSERT INTO `course_type` VALUES (2,'多人接力');
INSERT INTO `course_type` VALUES (3,'多人绘画');
INSERT INTO `course_type` VALUES (4,'自由绘画');
/*!40000 ALTER TABLE `course_type` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table course (课程表)
#
DROP TABLE IF EXISTS `course`;
CREATE TABLE `course` (
        `course_id` int (11) NOT NULL AUTO_INCREMENT,
        `course_name` char (40) NOT NULL,
        `group_id` int (11) NOT NULL,
        `course_type_id` int (11) NOT NULL,
        `language` int (11) NOT NULL,
        `art` int (11) NOT NULL,
        `community` int (11) NOT NULL,
        `health` int (11) NOT NULL,
        `science` int (11) NOT NULL,
        `course_date` char (32)  NOT NULL,
        `resPath`     char (128) NOT NULL,
        `iconPath`    char (128) NOT NULL,
        `xmlPath`     char (128) NOT NULL,
        PRIMARY KEY (`course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table course
#
LOCK TABLES `course` WRITE;
/*!40000 ALTER TABLE `course` DISABLE KEYS */;
INSERT INTO `course` VALUES (1 ,'连线1',3,2,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/connect/lianxian1.png', 'general/pic/ui/icon/Icon_officeset_conns.png',   '/mnt/sdcard/EduPlatform/production/connect/lianxian1.xml');
INSERT INTO `course` VALUES (2 ,'连线2',3,2,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/connect/lianxian2.png', 'general/pic/ui/icon/Icon_officeset_conns.png',   '/mnt/sdcard/EduPlatform/production/connect/lianxian2.xml');
INSERT INTO `course` VALUES (3 ,'连线3',3,3,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/connect/lianxian3.png', 'general/pic/ui/icon/Icon_officeset_conns.png',   '/mnt/sdcard/EduPlatform/production/connect/lianxian3.xml');
INSERT INTO `course` VALUES (4 ,'拼图1',4,3,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/puzzle/jiaocai2.png',   'general/pic/ui/icon/Icon_officeset_puzzles.png', '/mnt/sdcard/EduPlatform/production/puzzle/jiaocai2.xml');
INSERT INTO `course` VALUES (5 ,'拼图2',4,4,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/puzzle/puzzle2.png',    'general/pic/ui/icon/Icon_officeset_puzzles.png', '/mnt/sdcard/EduPlatform/production/puzzle/puzzle2.xml');
INSERT INTO `course` VALUES (6 ,'拼图3',4,4,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/puzzle/puzzle3.png',    'general/pic/ui/icon/Icon_officeset_puzzles.png', '/mnt/sdcard/EduPlatform/production/puzzle/puzzle3.xml');

INSERT INTO `course` VALUES (7 ,'画图1',3,2,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/picture/picture1.png',  'general/pic/ui/icon/Icon_offictset_pictures.png', '/mnt/sdcard/EduPlatform/production/picture/picture1.xml');
INSERT INTO `course` VALUES (8 ,'画图2',3,2,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/picture/picture2.png',  'general/pic/ui/icon/Icon_offictset_pictures.png', '/mnt/sdcard/EduPlatform/production/picture/picture2.xml');
INSERT INTO `course` VALUES (9 ,'画图3',3,3,1,1,1,1,1, '2013/1/14', '/mnt/sdcard/EduPlatform/production/picture/picture3.png',  'general/pic/ui/icon/Icon_offictset_pictures.png', '/mnt/sdcard/EduPlatform/production/picture/picture3.xml');
INSERT INTO `course` VALUES (10,'画图4',4,3,1,1,1,1,1, '2014/1/14', '/mnt/sdcard/EduPlatform/production/picture/picture4.png',  'general/pic/ui/icon/Icon_offictset_pictures.png', '/mnt/sdcard/EduPlatform/production/picture/picture4.xml');
/*!40000 ALTER TABLE `course` ENABLE KEYS */;
UNLOCK TABLES;


# ======================================================
#
# Source for table author_course (作品表)
#
DROP TABLE IF EXISTS `author_course`;
CREATE TABLE `author_course` (
        `author_course_id` int (11) NOT NULL AUTO_INCREMENT,
        `author_id`        int (11) NOT NULL,
        `client_type_id`   int (11) NOT NULL,
        `course_id`        int (11) NOT NULL,
        `issell`           int (2)  NOT NULL,
        `owner`            int (11) NOT NULL,
        `owner_type_id`    int (11) NOT NULL,
        `flag`             int (2)  NOT NULL,
        PRIMARY KEY (`author_course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table author_course
#
LOCK TABLES `author_course` WRITE;
/*!40000 ALTER TABLE `author_course` DISABLE KEYS */;
INSERT INTO `author_course` VALUES (1 ,1,2,1,1, 1,2, 0);
INSERT INTO `author_course` VALUES (2 ,1,2,2,1, 1,2, 0);
INSERT INTO `author_course` VALUES (3 ,1,2,3,1, 1,2, 0);
INSERT INTO `author_course` VALUES (4 ,1,2,4,0, 1,2, 0);

INSERT INTO `author_course` VALUES (5 ,2,2,1,1, 2,2, 0);
INSERT INTO `author_course` VALUES (6 ,2,2,2,1, 2,2, 0);
INSERT INTO `author_course` VALUES (7 ,2,2,3,0, 2,2, 0);

INSERT INTO `author_course` VALUES (8 ,3,2,2,1, 3,2, 0);
INSERT INTO `author_course` VALUES (9 ,3,2,3,1, 3,2, 0);
INSERT INTO `author_course` VALUES (10,3,2,4,0, 3,2, 0);
/*!40000 ALTER TABLE `author_course` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table grade_course (年级课程表)
#
DROP TABLE IF EXISTS `grade_course`;
CREATE TABLE `grade_course` (
        `grade_course_id` int (11) NOT NULL AUTO_INCREMENT,
        `grade_id` int (11) NOT NULL,
        `course_id` int (11) NOT NULL,
        PRIMARY KEY (`grade_course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table grade_course
#
LOCK TABLES `grade_course` WRITE;
/*!40000 ALTER TABLE `grade_course` DISABLE KEYS */;
INSERT INTO `grade_course` VALUES (1,1,1);
INSERT INTO `grade_course` VALUES (2,1,2);
INSERT INTO `grade_course` VALUES (3,1,3);
INSERT INTO `grade_course` VALUES (4,1,4);
INSERT INTO `grade_course` VALUES (5,1,7);
INSERT INTO `grade_course` VALUES (6,1,10);

INSERT INTO `grade_course` VALUES (7,2,1);
INSERT INTO `grade_course` VALUES (8,2,3);
INSERT INTO `grade_course` VALUES (9,2,8);

INSERT INTO `grade_course` VALUES (10,3,2);
INSERT INTO `grade_course` VALUES (11,3,3);
INSERT INTO `grade_course` VALUES (12,3,4);
INSERT INTO `grade_course` VALUES (13,3,9);
/*!40000 ALTER TABLE `grade_course` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table resource_type (资源类型表)
#
DROP TABLE IF EXISTS `resource_type`;
CREATE TABLE `resource_type` (
        `type_id` int (11) NOT NULL,
        `name` int (11) NOT NULL,
        PRIMARY KEY (`type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table resource_type
#
LOCK TABLES `resource_type` WRITE;
/*!40000 ALTER TABLE `resource_type` DISABLE KEYS */;
INSERT INTO `resource_type` VALUES (1, '学生头像资源');
INSERT INTO `resource_type` VALUES (2, '教师头像资源');
INSERT INTO `resource_type` VALUES (3, '学生头像资源');
INSERT INTO `resource_type` VALUES (4, '学生头像资源');

INSERT INTO `resource_type` VALUES (5, '学生头像资源');
INSERT INTO `resource_type` VALUES (6, '学生头像资源');

INSERT INTO `resource_type` VALUES (7, '学生头像资源');
INSERT INTO `resource_type` VALUES (8, '学生头像资源');
INSERT INTO `resource_type` VALUES (9, '学生头像资源');
/*!40000 ALTER TABLE `resource_type` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table resource (资源表)
#
DROP TABLE IF EXISTS `resource`;
CREATE TABLE `resource` (
        `resource_id`   int(11)         NOT NULL,
        `type_id`       int(11)         NOT NULL,
        `name`          int(11)         NOT NULL,
        `img_path`      char(128)       NOT NULL,
        PRIMARY KEY (`resource_id`, `type_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table resource
#
LOCK TABLES `resource` WRITE;
/*!40000 ALTER TABLE `resource` DISABLE KEYS */;
INSERT INTO `resource` VALUES ( 1,1,'学生头像1', '/material/profile/student01.png');
INSERT INTO `resource` VALUES ( 2,1,'学生头像2', '/material/profile/student02.png');
INSERT INTO `resource` VALUES ( 3,1,'学生头像3', '/material/profile/student03.png');
INSERT INTO `resource` VALUES ( 4,1,'学生头像4', '/material/profile/student04.png');
INSERT INTO `resource` VALUES ( 5,1,'学生头像5', '/material/profile/student05.png');
INSERT INTO `resource` VALUES ( 6,1,'学生头像6', '/material/profile/student06.png');
INSERT INTO `resource` VALUES ( 7,1,'学生头像7', '/material/profile/student07.png');
INSERT INTO `resource` VALUES ( 8,1,'学生头像8', '/material/profile/student08.png');
INSERT INTO `resource` VALUES ( 9,1,'学生头像9', '/material/profile/student09.png');
INSERT INTO `resource` VALUES (10,1,'学生头像10','/material/profile/student10.png');
INSERT INTO `resource` VALUES (11,1,'学生头像11','/material/profile/student11.png');
INSERT INTO `resource` VALUES (12,1,'学生头像12','/material/profile/student12.png');
INSERT INTO `resource` VALUES (13,1,'学生头像13','/material/profile/student13.png');
INSERT INTO `resource` VALUES (14,1,'学生头像14','/material/profile/student14.png');
INSERT INTO `resource` VALUES (15,1,'学生头像15','/material/profile/student15.png');
INSERT INTO `resource` VALUES (16,1,'学生头像16','/material/profile/student16.png');
INSERT INTO `resource` VALUES (17,1,'学生头像17','/material/profile/student17.png');
INSERT INTO `resource` VALUES (18,1,'学生头像18','/material/profile/student18.png');
INSERT INTO `resource` VALUES (19,1,'学生头像19','/material/profile/student19.png');
INSERT INTO `resource` VALUES (20,1,'学生头像20','/material/profile/student20.png');
INSERT INTO `resource` VALUES (21,1,'学生头像21','/material/profile/student21.png');
INSERT INTO `resource` VALUES (22,1,'学生头像22','/material/profile/student22.png');
INSERT INTO `resource` VALUES (23,1,'学生头像23','/material/profile/student23.png');
INSERT INTO `resource` VALUES (24,1,'学生头像24','/material/profile/student24.png');
INSERT INTO `resource` VALUES (25,1,'学生头像25','/material/profile/student25.png');
INSERT INTO `resource` VALUES (26,1,'学生头像26','/material/profile/student26.png');
INSERT INTO `resource` VALUES (27,1,'学生头像27','/material/profile/student27.png');
INSERT INTO `resource` VALUES (28,1,'学生头像28','/material/profile/student28.png');
INSERT INTO `resource` VALUES (29,1,'学生头像29','/material/profile/student29.png');
INSERT INTO `resource` VALUES (30,1,'学生头像30','/material/profile/student30.png');
INSERT INTO `resource` VALUES (31,1,'学生头像31','/material/profile/student31.png');
INSERT INTO `resource` VALUES (32,1,'学生头像32','/material/profile/student32.png');
INSERT INTO `resource` VALUES (33,1,'学生头像33','/material/profile/student33.png');
INSERT INTO `resource` VALUES (34,1,'学生头像34','/material/profile/student34.png');
INSERT INTO `resource` VALUES (35,1,'学生头像35','/material/profile/student35.png');

INSERT INTO `resource` VALUES (36,1,'学生头像36','/material/profile/student16.png');
INSERT INTO `resource` VALUES (37,1,'学生头像37','/material/profile/student17.png');
INSERT INTO `resource` VALUES (38,1,'学生头像38','/material/profile/student18.png');
INSERT INTO `resource` VALUES (39,1,'学生头像39','/material/profile/student19.png');

INSERT INTO `resource` VALUES (40,2,'教师头像1' ,'/material/profile/teacher01.png');
INSERT INTO `resource` VALUES (41,2,'教师头像2' ,'/material/profile/teacher02.png');
INSERT INTO `resource` VALUES (42,2,'教师头像3' ,'/material/profile/teacher03.png');
INSERT INTO `resource` VALUES (43,2,'教师头像4' ,'/material/profile/teacher01.png');
INSERT INTO `resource` VALUES (44,2,'教师头像5' ,'/material/profile/teacher02.png');
/*!40000 ALTER TABLE `resource` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table country (国家表)
#

DROP TABLE IF EXISTS `country`;
CREATE TABLE `country` (
        `country_id`    int(11)         NOT NULL,
        `country_name`  char(100)       NOT NULL,
        PRIMARY KEY (`country_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table country
#

LOCK TABLES `country` WRITE;
/*!40000 ALTER TABLE `country` DISABLE KEYS */;
INSERT INTO `country` VALUES (1,'中国');
INSERT INTO `country` VALUES (2,'美国');
/*!40000 ALTER TABLE `country` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table province (省表)
#

DROP TABLE IF EXISTS `province`;
CREATE TABLE `province` (
        `province_id`    int(11)         NOT NULL,
        `province_name`  char(100)       NOT NULL,
        `alias`          char(10)        NOT NULL,
        PRIMARY KEY (`province_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table province
#

LOCK TABLES `province` WRITE;
/*!40000 ALTER TABLE `province` DISABLE KEYS */;
INSERT INTO `province` VALUES (1,'四川省','川');
INSERT INTO `province` VALUES (2,'台湾省','台');
/*!40000 ALTER TABLE `province` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table city (市表)
#

DROP TABLE IF EXISTS `city`;
CREATE TABLE `city` (
        `city_id`        int(11)         NOT NULL,
        `city_name`      char(100)       NOT NULL,
        `province_id`    int(11)         NOT NULL,
        PRIMARY KEY (`city_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table city
#

LOCK TABLES `city` WRITE;
/*!40000 ALTER TABLE `city` DISABLE KEYS */;
INSERT INTO `city` VALUES (1,'成都市',1);
INSERT INTO `city` VALUES (2,'德阳市',1);
/*!40000 ALTER TABLE `city` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table area (区表)
#

DROP TABLE IF EXISTS `area`;
CREATE TABLE `area` (
        `area_id`       int(11)         NOT NULL,
        `area_name`     char(100)       NOT NULL,
        `city_id`       int(11)         NOT NULL,
        PRIMARY KEY (`area_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table area 
#

LOCK TABLES `area` WRITE;
/*!40000 ALTER TABLE `area` DISABLE KEYS */;
INSERT INTO `area` VALUES (1,'金牛区',1);
INSERT INTO `area` VALUES (2,'锦江区',1);
INSERT INTO `area` VALUES (3,'青羊区',1);
INSERT INTO `area` VALUES (4,'武侯区',1);
INSERT INTO `area` VALUES (5,'成华区',1);
/*!40000 ALTER TABLE `area` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table school (学校表)
#
DROP TABLE IF EXISTS `school`;
CREATE TABLE `school` (
        `school_id`     int(11)         NOT NULL,
        `school_name`   char(100)       NOT NULL,
        `area_id`       int(11)         NOT NULL,
        `headmaster_id` int(11)         NOT NULL,
        `school_address`char(100)       NOT NULL,
        PRIMARY KEY (`school_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table school
#
LOCK TABLES `school` WRITE;
/*!40000 ALTER TABLE `school` DISABLE KEYS */;
INSERT INTO `school` VALUES (1,'双楠A幼儿园',1,0,'地址：四川成都市高薪区旺旺大厦');
INSERT INTO `school` VALUES (2,'双楠B幼儿园',1,0,'地址：四川成都市高薪区旺旺大厦');
INSERT INTO `school` VALUES (3,'双楠C幼儿园',1,0,'地址：四川成都市高薪区旺旺大厦');
INSERT INTO `school` VALUES (4,'双楠D幼儿园',1,0,'地址：四川成都市高薪区旺旺大厦');
INSERT INTO `school` VALUES (5,'双楠E幼儿园',1,0,'地址：四川成都市高薪区旺旺大厦');
/*!40000 ALTER TABLE `school` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table grade (年级表)
#
DROP TABLE IF EXISTS `grade`;
CREATE TABLE `grade` (
        `grade_id`      int(11)         NOT NULL,
        `grade_name`    char(100)       NOT NULL,
        `school_id`     int(11)         NOT NULL,
        PRIMARY KEY (`grade_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table grade
#
LOCK TABLES `grade` WRITE;
/*!40000 ALTER TABLE `grade` DISABLE KEYS */;
INSERT INTO `grade` VALUES (1,'小班',1);
INSERT INTO `grade` VALUES (2,'中班',1);
INSERT INTO `grade` VALUES (3,'大班',1);
/*!40000 ALTER TABLE `grade` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table class (班级表)
#
DROP TABLE IF EXISTS `class`;
CREATE TABLE `class` (
        `class_id`              int(11)         NOT NULL,
        `class_name`            char(40)        NOT NULL,
        `grade_id`              int(11)         NOT NULL,
        `class_teacher_id`      int(11)         NOT NULL,
        PRIMARY KEY (`class_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table class
#
LOCK TABLES `class` WRITE;
/*!40000 ALTER TABLE `class` DISABLE KEYS */;
INSERT INTO `class` VALUES (1 ,'小班向阳班',1,1);
INSERT INTO `class` VALUES (2 ,'小班草莓班',1,1);
INSERT INTO `class` VALUES (3 ,'小班苹果班',1,1);
INSERT INTO `class` VALUES (4 ,'小班豆豆班',1,1);
INSERT INTO `class` VALUES (5 ,'小班果冻班',1,1);

INSERT INTO `class` VALUES (6 ,'中班向阳班',2,2);
INSERT INTO `class` VALUES (7 ,'中班草莓班',2,2);
INSERT INTO `class` VALUES (8 ,'中班苹果班',2,2);
INSERT INTO `class` VALUES (9 ,'中班豆豆班',2,2);
INSERT INTO `class` VALUES (10,'中班果冻班',2,2);

INSERT INTO `class` VALUES (11,'大班向阳班',3,3);
INSERT INTO `class` VALUES (12,'大班草莓班',3,3);
INSERT INTO `class` VALUES (13,'大班苹果班',3,3);
INSERT INTO `class` VALUES (14,'大班豆豆班',3,3);
INSERT INTO `class` VALUES (15,'大班果冻班',3,3);

/*!40000 ALTER TABLE `class` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table teacher (教师表)
#
DROP TABLE IF EXISTS `teacher`;
CREATE TABLE `teacher` (
        `teacher_id`            int(11)         NOT NULL,
        `last_name`             char(100)       NOT NULL,
        `first_name`            char(100)       NOT NULL,
        `sex`                   char(2)         DEFAULT '女',
        `age`                   int(11)         NOT NULL DEFAULT '0',
        `race_id`               int(11)         NOT NULL,
        `birthday`              date            NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(100)       NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `country_id`            int(11)         NOT NULL DEFAULT '0',
        `native_place_id`       int(11)         NOT NULL,
        `school_id`             int(11)         NOT NULL,
        `client_type_id`        int(11)         DEFAULT 2,
        `status`                char(32)        NOT NULL,
        `telephone`             char(16)        NOT NULL,
        `qq`                    char(16)        NOT NULL,
        `identity`              char(20)        NOT NULL,
        `address`               char(254)       NOT NULL,
        PRIMARY KEY (`teacher_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table teacher
#

LOCK TABLES `teacher` WRITE;
/*!40000 ALTER TABLE `teacher` DISABLE KEYS */;
INSERT INTO `teacher` VALUES (1,'赵','一','女',1,1,'1989-12-30','T1','T',40, 1,1,1,2,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `teacher` VALUES (2,'钱','二','男',1,1,'1989-12-30','T2','T',41, 1,1,1,2,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `teacher` VALUES (3,'孙','三','男',1,1,'1989-12-30','T3','T',42, 1,1,1,2,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `teacher` VALUES (4,'李','四','男',1,1,'1989-12-30','T4','T',43, 1,1,1,2,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `teacher` VALUES (5,'周','五','男',1,1,'1989-12-30','T5','T',44, 1,1,1,2,'在职','13528254451','58456213','585456325987451236','成都市');
/*!40000 ALTER TABLE `teacher` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table parents (家长表)
#
DROP TABLE IF EXISTS `parents`;
CREATE TABLE `parents` (
        `parents_id`            int(11)         NOT NULL,
        `last_name`             char(100)       NOT NULL,
        `first_name`            char(100)       NOT NULL,
        `sex`                   char(2)         DEFAULT '女',
        `age`                   int(11)         NOT NULL DEFAULT '0',
        `race_id`               int(11)         NOT NULL,
        `birthday`              date            NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(100)       NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `country_id`            int(11)         NOT NULL DEFAULT '0',
        `native_place_id`       int(11)         NOT NULL,
        `school_id`             int(11)         NOT NULL,
        `client_type_id`        int(11)         DEFAULT 3,
        `status`                char(32)        NOT NULL,
        `telephone`             char(16)        NOT NULL,
        `qq`                    char(16)        NOT NULL,
        `identity`              char(20)        NOT NULL,
        `address`               char(254)       NOT NULL,
        PRIMARY KEY (`parents_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table parents
#

LOCK TABLES `parents` WRITE;
/*!40000 ALTER TABLE `parents` DISABLE KEYS */;
INSERT INTO `parents` VALUES (1 ,'家长','1' ,'女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (2 ,'家长','2' ,'男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (3 ,'家长','3' ,'男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (4 ,'家长','4' ,'男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (5 ,'家长','5' ,'男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (6 ,'家长','6' ,'女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (7 ,'家长','7' ,'男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (8 ,'家长','8' ,'男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (9 ,'家长','9' ,'男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (10,'家长','10','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (11,'家长','11','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (12,'家长','12','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (13,'家长','13','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (14,'家长','14','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (15,'家长','15','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (16,'家长','16','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (17,'家长','17','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (18,'家长','18','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (19,'家长','19','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (20,'家长','20','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (21,'家长','21','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (22,'家长','22','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (23,'家长','23','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (24,'家长','24','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (25,'家长','25','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (26,'家长','26','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (27,'家长','27','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (28,'家长','28','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (29,'家长','29','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (30,'家长','30','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (31,'家长','31','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (32,'家长','32','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (33,'家长','33','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (34,'家长','34','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (35,'家长','35','男',1,1,'1989-12-30','P5','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (36,'家长','36','女',1,1,'1989-12-30','P1','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (37,'家长','37','男',1,1,'1989-12-30','P2','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (38,'家长','38','男',1,1,'1989-12-30','P3','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
INSERT INTO `parents` VALUES (39,'家长','39','男',1,1,'1989-12-30','P4','P',2001,1,1,1,3,'在职','13528254451','58456213','585456325987451236','成都市');
/*!40000 ALTER TABLE `parents` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table headmaster(园长表)
#
DROP TABLE IF EXISTS `headmaster`;
CREATE TABLE `headmaster` (
        `headmaster_id`         int(11)         NOT NULL,
        `number`                char(20)        NOT NULL,
        `last_name`             char(10)        NOT NULL,
        `first_name`            char(10)        NOT NULL,
        `sex`                   char(2)         DEFAULT '男',
        `age`                   int(11)         NOT NULL DEFAULT '0',
        `race_id`               int(11)         NOT NULL,
        `school_id`             int(11)         NOT NULL,
        `birthday`              date            NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(34)        NOT NULL,
        `country_id`            int(11)         DEFAULT '1',
        `native_place_id`       int(11)         NOT NULL,
        `class_id`              int(11)         NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `client_type_id`        int(11)         DEFAULT 1,
        `status`                char(32)        NOT NULL,
        PRIMARY KEY (`headmaster_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table headmaster 
#

LOCK TABLES `headmaster` WRITE;
/*!40000 ALTER TABLE `headmaster` DISABLE KEYS */;
INSERT INTO `headmaster` VALUES (1 ,'10000001','yang','c1' ,'男',1,1,1,'2000-01-01','H1','H',1,1,1,1001,1,'在职');
INSERT INTO `headmaster` VALUES (2 ,'10000002','yang','c2' ,'男',1,1,1,'2000-01-01','H2','H',1,1,1,1002,1,'在职');
INSERT INTO `headmaster` VALUES (3 ,'10000003','yang','c3' ,'女',1,1,1,'2000-01-01','H3','H',1,1,1,1003,1,'在职');
INSERT INTO `headmaster` VALUES (4 ,'10000004','yang','c4' ,'女',1,1,1,'2000-01-01','H4','H',1,1,1,1004,1,'在职');
INSERT INTO `headmaster` VALUES (5 ,'10000005','yang','c5' ,'女',1,1,1,'2000-01-01','H5','H',1,1,1,1005,1,'在职');
/*!40000 ALTER TABLE `headmaster` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table student (学生表)
#
DROP TABLE IF EXISTS `student`;
CREATE TABLE `student` (
        `student_id`            int(11)         NOT NULL,
        `number`                char(20)        NOT NULL,
        `last_name`             char(10)        NOT NULL,
        `first_name`            char(10)        NOT NULL,
        `sex`                   char(2)         DEFAULT '男',
        `age`                   int(11)         NOT NULL DEFAULT '0',
        `race_id`               int(11)         NOT NULL,
        `birthday`              char(15)        NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(34)        NOT NULL,
        `country_id`            int(11)         DEFAULT '1',
        `native_place_id`       int(11)         NOT NULL,
        `class_id`              int(11)         NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `city_id`               int(11)         NOT NULL,
        `school_id`             int(11)         NOT NULL,
        `client_type_id`        int(11)         DEFAULT 4,
        `status`                char(32)        NOT NULL,
        `parents1_id`           int(11)         NOT NULL,
        `parents2_id`           int(11)         NOT NULL,
        `address`               char(254)       NOT NULL,
        PRIMARY KEY (`student_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table student
#

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES (1 ,'10000001','yang','c1' ,'男',1,1,'2000-01-01','S1' ,'S',1,1,1, 1,1,1,4,'在校',1 ,1 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (2 ,'10000002','yang','c2' ,'男',1,1,'2000-01-01','S2' ,'S',1,1,1, 2,1,1,4,'在校',2 ,2 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (3 ,'10000003','yang','c3' ,'女',1,1,'2000-01-01','S3' ,'S',1,1,1, 3,1,1,4,'在校',3 ,3 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (4 ,'10000004','yang','c4' ,'女',1,1,'2000-01-01','S4' ,'S',1,1,1, 4,1,1,4,'在校',4 ,4 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (5 ,'10000005','yang','c5' ,'女',1,1,'2000-01-01','S5' ,'S',1,1,1, 5,1,1,4,'在校',5 ,5 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (6 ,'10000006','yang','c6' ,'男',1,1,'2000-01-01','S6' ,'S',1,1,1, 6,1,1,4,'在校',6 ,6 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (7 ,'10000007','yang','c7' ,'女',1,1,'2000-01-01','S7' ,'S',1,1,1, 7,1,1,4,'在校',7 ,7 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (8 ,'10000008','yang','c8' ,'女',1,1,'2000-01-01','S8' ,'S',1,1,1, 8,1,1,4,'在校',8 ,8 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (9 ,'10000009','yang','c9' ,'男',1,1,'2000-01-01','S9' ,'S',1,1,1, 9,1,1,4,'在校',9 ,9 ,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (10,'10000010','yang','c10','女',1,1,'2000-01-01','S10','S',1,1,1,10,1,1,4,'在校',10,10,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (11,'10000011','yang','c11','女',1,1,'2000-01-01','S11','S',1,1,1,11,1,1,4,'在校',11,11,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (12,'10000012','yang','c12','女',1,1,'2000-01-01','S12','S',1,1,1,12,1,1,4,'在校',12,12,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (13,'10000013','yang','c13','女',1,1,'2000-01-01','S13','S',1,1,1,13,1,1,4,'在校',13,13,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (14,'10000014','yang','c14','女',1,1,'2000-01-01','S14','S',1,1,1,14,1,1,4,'在校',14,14,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (15,'10000015','yang','c15','女',1,1,'2000-01-01','S15','S',1,1,1,15,1,1,4,'在校',15,15,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (16,'10000016','yang','c16','男',1,1,'2000-01-01','S16','S',1,1,1,16,1,1,4,'在校',16,16,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (17,'10000017','yang','c17','女',1,1,'2000-01-01','S17','S',1,1,1,17,1,1,4,'在校',17,17,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (18,'10000018','yang','c18','女',1,1,'2000-01-01','S18','S',1,1,1,18,1,1,4,'在校',18,18,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (19,'10000019','yang','c19','女',1,1,'2000-01-01','S19','S',1,1,1,19,1,1,4,'在校',19,19,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (20,'10000020','yang','c20','女',1,1,'2000-01-01','S20','S',1,1,1,20,1,1,4,'在校',20,20,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (21,'10000021','yang','c21','男',1,1,'2000-01-01','S21','S',1,1,1,21,1,1,4,'在校',21,21,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (22,'10000022','yang','c22','女',1,1,'2000-01-01','S22','S',1,1,1,22,1,1,4,'在校',22,22,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (23,'10000023','yang','c23','女',1,1,'2000-01-01','S23','S',1,1,1,23,1,1,4,'在校',23,23,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (24,'10000024','yang','c24','男',1,1,'2000-01-01','S24','S',1,1,1,24,1,1,4,'在校',24,24,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (25,'10000025','yang','c25','女',1,1,'2000-01-01','S25','S',1,1,1,25,1,1,4,'在校',25,25,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (26,'10000026','yang','c26','男',1,1,'2000-01-01','S26','S',1,1,1,26,1,1,4,'在校',26,26,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (27,'10000027','yang','c27','女',1,1,'2000-01-01','S27','S',1,1,1,27,1,1,4,'在校',27,27,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (28,'10000028','yang','c28','男',1,1,'2000-01-01','S28','S',1,1,1,28,1,1,4,'在校',28,28,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (29,'10000029','yang','c29','女',1,1,'2000-01-01','S29','S',1,1,1,29,1,1,4,'在校',29,29,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (30,'10000030','yang','c30','男',1,1,'2000-01-01','S30','S',1,1,1,30,1,1,4,'在校',30,30,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (31,'10000031','yang','c31','女',1,1,'2000-01-01','S31','S',1,1,1,31,1,1,4,'在校',31,31,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (32,'10000032','yang','c32','女',1,1,'2000-01-01','S32','S',1,1,1,32,1,1,4,'在校',32,32,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (33,'10000033','yang','c33','女',1,1,'2000-01-01','S33','S',1,1,1,33,1,1,4,'在校',33,33,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (34,'10000034','yang','c34','女',1,1,'2000-01-01','S34','S',1,1,1,34,1,1,4,'在校',34,34,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (35,'10000035','yang','c35','男',1,1,'2000-01-01','S35','S',1,1,1,35,1,1,4,'在校',35,35,'成都市旺旺大厦4号');
                                                                                                                    
INSERT INTO `student` VALUES (36,'10000036','yang','c36','男',1,1,'2000-01-01','S36','S',1,1,3,36,1,1,4,'在校',36,36,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (37,'10000037','yang','c37','男',1,1,'2000-01-01','S37','S',1,1,3,37,1,1,4,'在校',37,37,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (38,'10000038','yang','c38','女',1,1,'2000-01-01','S38','S',1,1,2,38,1,1,4,'在校',38,38,'成都市旺旺大厦4号');
INSERT INTO `student` VALUES (39,'10000039','yang','c39','女',1,1,'2000-01-01','S39','S',1,1,2,39,1,1,4,'在校',39,39,'成都市旺旺大厦4号');
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table race (民族配置表)
#

DROP TABLE IF EXISTS `race`;
CREATE TABLE `race` (
        `race_id`       int(11)         NOT NULL,
        `race_name`     char(40)        NOT NULL,
        PRIMARY KEY (`race_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table race
#

LOCK TABLES `race` WRITE;
/*!40000 ALTER TABLE `race` DISABLE KEYS */;
INSERT INTO `race` VALUES (1,'汉族');
INSERT INTO `race` VALUES (2,'少数民族');
/*!40000 ALTER TABLE `race` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table native_place (所在地配置表)
#

DROP TABLE IF EXISTS `native_place`;
CREATE TABLE `native_place` (
    `native_id` int(11) NOT NULL,
    `province` char(20) NOT NULL,
    `city` char(20) DEFAULT NULL,
    PRIMARY KEY (`native_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table native_place
#

LOCK TABLES `native_place` WRITE;
/*!40000 ALTER TABLE `native_place` DISABLE KEYS */;
INSERT INTO `native_place` VALUES (1,'四川',NULL);
/*!40000 ALTER TABLE `native_place` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table board(平板表)
#
DROP TABLE IF EXISTS `board`;
CREATE TABLE `board` (
        `board_id`          int(11)         NOT NULL,
        `number`            int(11)         NOT NULL,
        `board_name`        char(100)       NOT NULL,
        `class_id`          char(20)        NOT NULL,
        `grade_id`          int(11)         NOT NULL,
        `status`            char(20)        NOT NULL,
        PRIMARY KEY (`board_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table board
#
LOCK TABLES `board` WRITE;
/*!40000 ALTER TABLE `board` DISABLE KEYS */;
INSERT INTO `board` VALUES (1, 'NTX-GT01', '白板1', 1, 1, '使用');
INSERT INTO `board` VALUES (2, 'NTX-GT02', '白板2', 1, 1, '使用');
INSERT INTO `board` VALUES (3, 'NTX-GT03', '白板3', 1, 1, '入库');
INSERT INTO `board` VALUES (4, 'NTX-GT04', '白板4', 1, 1, '使用');
INSERT INTO `board` VALUES (5, 'NTX-GT05', '白板5', 1, 1, '损坏');
/*!40000 ALTER TABLE `board` ENABLE KEYS */;
UNLOCK TABLES;

# ======================================================
#
# Source for table classroom (教室表)
#
DROP TABLE IF EXISTS `classroom`;
CREATE TABLE `classroom` (
        `classroom_id`          int(11)         NOT NULL,
        `classroom_name`        char(100)       NOT NULL,
        `white_board`           char(20)        NOT NULL,
        `school_id`              int(11)         NOT NULL,
        PRIMARY KEY (`classroom_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

#
# Dumping data for table classroom
#
LOCK TABLES `classroom` WRITE;
/*!40000 ALTER TABLE `classroom` DISABLE KEYS */;
INSERT INTO `classroom` VALUES (1,'A教室','白板1',1);
INSERT INTO `classroom` VALUES (2,'B教室','白板2',1);
INSERT INTO `classroom` VALUES (3,'C教室','白板3',1);
INSERT INTO `classroom` VALUES (4,'D教室','白板4',1);
INSERT INTO `classroom` VALUES (5,'E教室','白板5',1);
/*!40000 ALTER TABLE `classroom` ENABLE KEYS */;
UNLOCK TABLES;

/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
