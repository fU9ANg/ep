/**
* @file Sock.h
* @brief 常用的SQL查询脚本
* @author fU9ANg (bb.newlife@gmail.com)
* @version v1.0
* @date 2014-01-03
*/

#ifndef _SQL_H_
#define _SQL_H_

// 学生表操作SQL
#define SQL_GET_STU_INFO_BY_NUM                     \
"SELECT s.student_id AS student_id, s.first_name AS first_name, s.last_name AS last_name, s.account AS account, s.password AS password FROM student AS s WHERE s.number=?"

#define SQL_GET_STU_INFO_BY_ACCOUT_AND_PW           \
"SELECT s.student_id AS student_id, s.first_name AS first_name, s.last_name AS last_name, s.account AS account, s.password AS password FROM student AS s WHERE s.account=? AND s.password=?"

#define SQL_GET_STU_INFO_BY_NAME                    \
"SELECT s.student_id AS student_id, s.first_name AS first_name, s.last_name AS last_name, s.account AS account, s.password AS password FROM student AS s WHERE s.first_name=? AND s.last_name=?"

#define SQL_GET_STU_LIST_BY_CLASSID                 \
"SELECT s.student_id AS student_id, s.first_name AS first_name, s.last_name AS last_name, s.account AS account, s.password AS password, c.class_name AS class_name FROM student AS s, class AS c WHERE s.class_id=c.class_id AND c.class_id=?"

#define SQL_GET_STU_LIST_BY_CLASSNAME               \
"SELECT s.student_id AS student_id, s.first_name AS first_name, s.last_name AS last_name, s.account AS account, s.password AS password, c.class_name AS class_name FROM student AS s, class AS c WHERE s.class_id=c.class_id AND c.class_name=?"

// 教师表操作SQL
#define SQL_GET_TEA_INFO_BY_ID                      \
"SELECT t.teacher_id AS teacher_id, t.last_name AS last_name, t.first_name AS first_name, t.sex AS sex, t.birthday AS birthday, t.account AS account, t.password AS password FROM teacher AS t WHERE t.teacher_id=?"

#define SQL_GET_TEA_INFO_BY_NAME                    \
"SELECT t.teacher_id AS teacher_id, t.last_name AS last_name, t.first_name AS first_name, t.sex AS sex, t.birthday AS birthday, t.account AS account, t.password AS password FROM teacher AS t WHERE t.first_name=? AND t.last_name=?"

#define SQL_GET_TEA_INFO_BY_ACCOUNT                 \
"SELECT t.teacher_id AS teacher_id, t.last_name AS last_name, t.first_name AS first_name, t.sex AS sex, t.birthday AS birthday, t.account AS account, t.password AS password FROM teacher AS t WHERE t.account=?"

#define SQL_GET_TEA_INFO_BY_ACCOUNT_AND_PW          \
"SELECT t.teacher_id AS teacher_id, t.last_name AS last_name, t.first_name AS first_name, t.sex AS sex, t.birthday AS birthday, t.account AS account, t.password AS password FROM teacher AS t WHERE t.account=? AND t.password=?"

// 课程表操作SQL
#define SQL_GET_GRADE_COURSE_LIST                   \
"SELECT g.grade_name, c.course_name, cg.group_name, c.language, c.art, c.community, c.health, c.science, c.resPath FROM grade AS g, course AS c, course_group as cg, grade_course as gc WHERE g.grade_id=gc.grade_id AND gc.course_id=c.course_id AND c.group_id=cg.group_id"

#define SQL_GET_COURSE_LIST_BY_GRADE                \
"SELECT g.grade_name, c.course_name, cg.group_name, c.language, c.art, c.community, c.health, c.science, c.resPath FROM grade AS g, course AS c, course_group as cg, grade_course as gc WHERE g.grade_id=gc.grade_id AND gc.course_id=c.course_id AND c.group_id=cg.group_id AND g.grade_name=?"

#define SQL_GET_COURSE_LIST_BY_TYPE                 \
"SELECT g.grade_name, c.course_name, cg.group_name, c.language, c.art, c.community, c.health, c.science, c.resPath FROM grade AS g, course AS c, course_group as cg, grade_course as gc WHERE g.grade_id=gc.grade_id AND gc.course_id=c.course_id AND c.group_id=cg.group_id AND cg.group_name=?"

#define SQL_GET_COURSE_BY_GRADE_AND_NAME            \
"SELECT g.grade_name, c.course_name, cg.group_name, c.language, c.art, c.community, c.health, c.science, c.resPath FROM grade AS g, course AS c, course_group as cg, grade_course as gc WHERE g.grade_id=gc.grade_id AND gc.course_id=c.course_id AND c.group_id=cg.group_id AND g.grade_name=? AND c.course_name=?"

#define SQL_GET_COURSE_BY_GRADE_AND_TYPE            \
"SELECT g.grade_name, c.course_name, cg.group_name, c.language, c.art, c.community, c.health, c.science, c.resPath FROM grade AS g, course AS c, course_group as cg, grade_course as gc WHERE g.grade_id=gc.grade_id AND gc.course_id=c.course_id AND c.group_id=cg.group_id AND g.grade_name=? AND cg.group_name=?"

// 年级和班级操作SQL
#define SQL_GET_ALL_CLASS_LIST                      \
"SELECT g.grade_name AS grade_name, c.class_name AS class_name, t.last_name, t.first_name FROM grade AS g, class AS c, teacher AS t WHERE c.class_teacher_id=t.teacher_id AND c.grade_id=g.grade_id"

#define SQL_GET_CLASS_LIST_BY_GRADE                 \
"SELECT g.grade_name AS grade_name, c.class_name AS class_name, t.last_name, t.first_name FROM grade AS g, class AS c, teacher AS t WHERE c.class_teacher_id=t.teacher_id AND c.grade_id=g.grade_id AND g.grade_name=?"

#endif
