# SQL-Front 5.1  (Build 4.16)

/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE */;
/*!40101 SET SQL_MODE='' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES */;
/*!40103 SET SQL_NOTES='ON' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;


# Host: 192.168.0.254    Database: teaching_db
# ------------------------------------------------------
# Server version 5.5.29-0ubuntu0.12.04.1

DROP DATABASE IF EXISTS `teaching_db`;
CREATE DATABASE `teaching_db` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `teaching_db`;

# ======================================================
#
# Source for table report (报表)
#
DROP TABLE IF EXISTS `report`;
CREATE TABLE `report` (
        `report_id` int(11) NOT NULL AUTO_INCREMENT,
        `number_id` int(11) NOT NULL,
        `teacher_id` int(11) NOT NULL,
        `student_id` int(11) NOT NULL,
        `course_id` int(11) NOT NULL,
        `used_time` char(20) DEFAULT '0时0分0秒', # fixme: change data type to datetime
        PRIMARY KEY (`report_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

# ======================================================
#
# Source for table course (课程表)
#
DROP TABLE IF EXISTS `course`;
CREATE TABLE `course` (
        `course_id` int (11) NOT NULL,
        `course_name` char (40) NOT NULL,
        `resource_id` int(11) NOT NULL,
        `group_id` int (11) NOT NULL,
        `language` int (11) NOT NULL,
        `art` int (11) NOT NULL,
        `community` int (11) NOT NULL,
        `health` int (11) NOT NULL,
        `science` int (11) NOT NULL,
        `resPath` char (128) NOT NULL,
        PRIMARY KEY (`course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# ======================================================
#
# Source for table course_group_course (课件课程表)
#
DROP TABLE IF EXISTS `course_group_course`;
CREATE TABLE `course_group_course` (
        `group_course_id` int (11) NOT NULL,
        `group_id` int (11) NOT NULL,
        `course_id` int (11) NOT NULL,
        PRIMARY KEY (`group_course_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# ======================================================
#
# Source for table item (选项表)
#
DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
        `item_id` int (11) NOT NULL,
        `item_name` char (40) NOT NULL,
        PRIMARY KEY (`item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# ======================================================
#
# Source for table course_item (课程选项表)
#
DROP TABLE IF EXISTS `course_item`;
CREATE TABLE `course_item` (
        `course_item_id` int (11) NOT NULL AUTO_INCREMENT,
        `course_id` int (11) NOT NULL,
        `item_id` int (11) NOT NULL,
        `keys_info` varchar(512) NOT NULL,
        `fck_desc` varchar(512) NOT NULL,
        PRIMARY KEY (`course_item_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

# ======================================================
#
# Source for table native_place (所在地配置表)
#

DROP TABLE IF EXISTS `native_place`;
CREATE TABLE `native_place` (
        `native_id`     int(11)         NOT NULL,
        `province`      char(20)        NOT NULL,
        `city`          char(20)        DEFAULT NULL,
        PRIMARY KEY (`native_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

# ======================================================
#
# Source for table school (学校表)
#
DROP TABLE IF EXISTS `school`;
CREATE TABLE `school` (
        `school_id`     int(11)         NOT NULL,
        `school_name`   char(100)       NOT NULL,
        `native_id`     int(11)         NOT NULL,
        PRIMARY KEY (`school_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

# ======================================================
#
# Source for table teacher (教师表)
#

DROP TABLE IF EXISTS `teacher`;
CREATE TABLE `teacher` (
        `teacher_id`            int(11)         NOT NULL,
        `last_name`             char(100)       NOT NULL,
        `first_name`            char(100)       NOT NULL,
        `sex`                   char(2)         DEFAULT '男',
        `race_id`               int(11)         NOT NULL,
        `birthday`              date            NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(100)       NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `country_id`            int(11)         NOT NULL DEFAULT '0',
        `native_place_id`       int(11)         NOT NULL,
        `school_id`             int(11)         NOT NULL,
        PRIMARY KEY (`teacher_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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
        `race_id`               int(11)         NOT NULL,
        `birthday`              date            NOT NULL,
        `picture_id`            int(11)         NOT NULL,
        `account`               char(100)       NOT NULL,
        `password`              char(34)        NOT NULL,
        `country_id`            int(11)         DEFAULT '1',
        `native_place_id`       int(11)         NOT NULL,
        `class_id`              int(11)         NOT NULL,
        PRIMARY KEY (`student_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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

# ======================================================
#
# Source for table classroom (教室表)
#
DROP TABLE IF EXISTS `classroom`;
CREATE TABLE `classroom` (
        `classroom_id`          int(11)         NOT NULL,
        `classroom_name`        char(100)       NOT NULL,
        `white_board`           char(20)        NOT NULL,
        `grade_id`              int(11)         NOT NULL,
        PRIMARY KEY (`classroom_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
