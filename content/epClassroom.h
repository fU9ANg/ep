/**
 * @file epClassroom.h
 * @brief 教室类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPCLASSROOM_H__
#define __EPCLASSROOM_H__

#include <map>

#include "epBase.h"

#include "epClass.h"
#include "epTeacher.h"
#include "epWhiteBoard.h"
#include "epGroup.h"

typedef std::map<int, epGroup*> EPGROUP_MAP;
typedef std::map<int, epClass*> EPCLASS_MAP;

/**
 * @brief 教室类。
 */
class epClassroom : public epBase {
public :
        epClassroom(void);
        ~epClassroom(void);

        /**
         * @name get
         * @{ */
        /**
         * @brief 根据学生ID获取学生对象指针。
         *
         * @param int[in] 学号ID。
         *
         * @return 成功返回指向学生对象的指针，否则返回NULL。
         */
        const epStudent* getStudentById(const int);
        /**
         * @brief 根据学生FD获取学生对象指针。
         *
         * @param int[in] 指定FD。
         *
         * @return 成功返回指向学生对象的指针，否则返回NULL。
         */
        const epStudent* getStudentByFd(const int);
        /**
         * @brief 根据指定FD删除用户（包括教师和白板）。
         *
         * @param int[in] 指定FD。
         *
         * @return 成功返回true，否则返回false。
         */
        bool deleteUserByFd(const int);
        /**
         * @brief 根据客户端FD获取该用户对象指针。
         *
         * @param int[in] 指定用户FD。
         *
         * @return 查找成功返回对象指针，否则返回NULL。
         */
        const epUser* getUserByFd(const int);
        const epUser* getUserByAccount(const std::string&);
        /**  @} */

        /**
         * @name set
         * @{ */
        /**  @} */

        bool insertClass(epClass*);
        bool insertStudent(const epStudent*);
        bool removeClassById(const int);
        bool moveAllStudentToUser(void);
        bool moveWhiteboardToUser(void);
        bool moveTeacherToUser(void);
        epClass* getClassById(const int);
        std::vector<epClass*> getClassList(void);
        const epGroup* getGroupByFd(const int);
        bool insertGroup(epGroup*);
        bool removeGroupById(const int);
        bool removeAllGroup(void);
        bool deleteGroupById(const int);
        bool deleteAllGroup(void);
        epGroup* getGroupById(const int);

        /**
         * @name sendto
         * @{ */
        /**
         * @brief 将指定消息发送给该教室内的所有客户端。
         *
         * @param Buf[in] 指定消息。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllClass(Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息内容发送给所有在该教室上课的班。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAll(Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息内容发送给在该教室上课的指定班。
         *
         * @param int[in] 班级ID。
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoClassById(const int, Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息内容发送给所有在该教室上课的分组。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllGroup(Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息内容发送给在该教室上课的指定分组。
         *
         * @param int[in] 组ID。
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoGroupById(const int, Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息内容发送给在该教室上课的老师。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoTeacher(Buf*, const bool toSelf=true);
        /**
         * @brief 将指定消息内容发送给所属该教室的白板。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoWhiteBoard(Buf*, const bool toSelf=true);
        /**  @} */

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        /**
         * @brief 教师。
         */
        epTeacher* teacher_;
        /**
         * @brief 白板。
         */
        epWhiteBoard* whiteboard_;
        /**
         * @brief 该教室上课时的课程列表。
         */
        std::string courseList_;
private :
        /**
         * @brief 班列表。
         */
        EPCLASS_MAP classMap_;
        /**
         * @brief 组列表。
         */
        EPGROUP_MAP groupMap_;
};

#endif // __EPCLASSROOM_H__
