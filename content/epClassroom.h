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

#include "epClass.h"
#include "epTeacher.h"
#include "epWhiteBoard.h"
#include "epGroup.h"

typedef std::map<int, epGroup> GROUP_MAP;

/**
 * @brief 教室类。
 */
class epClassroom {
public :
        epClassroom(void);
        ~epClassroom(void);

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取教室ID。
         *
         * @return 教室ID。
         */
        const int getId(void) const;
        /**
         * @brief 获取所属该教室的老师对象。
         *
         * @return 老师对象。
         */
        const epTeacher getTeacher(void) const;
        /**
         * @brief 获取所属该教室的白板对象。
         *
         * @return 白板对象。
         */
        const epWhiteBoard getWhiteBoard(void) const;
        /**  @} */

        /**
         * @name set
         * @{ */
        /**
         * @brief 设置教室ID。
         *
         * @param int[in] 教室ID。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setId(const int);
        /**
         * @brief 指定老师。
         *
         * @param epTeacher[in] 老师对象。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setTeacher(const epTeacher&);
        /**
         * @brief 指定白板。
         *
         * @param epWhiteBoard[in] 白板对象。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setWhiteBoard(const epWhiteBoard&);
        /**  @} */

        bool insertClass(const epClass&);
        bool removeClassById(const int);
        bool insertGroup(const epGroup&);
        bool removeGroupById(const int);

        /**
         * @name sendto
         * @{ */
        /**
         * @brief 将指定消息内容发送给所有在该教室上课的班。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllClass(Buf*);
        /**
         * @brief 将指定消息内容发送给在该教室上课的指定班。
         *
         * @param int[in] 班级ID。
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoClass(const int, Buf*);
        /**
         * @brief 将指定消息内容发送给所有在该教室上课的分组。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllGroup(Buf*);
        /**
         * @brief 将指定消息内容发送给在该教室上课的指定分组。
         *
         * @param int[in] 组ID。
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoGroup(const int, Buf*);
        /**
         * @brief 将指定消息内容发送给在该教室上课的老师。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoTeacher(Buf*);
        /**
         * @brief 将指定消息内容发送给所属该教室的白板。
         *
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoWhiteBoard(Buf*);
        /**  @} */


        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void) const;
        /**  @} */


private :
        /**
         * @brief 教室ID。
         */
        int id_;
        /**
         * @brief 班列表。
         */
        EPCLASS_MAP classMap_;
        /**
         * @brief 组列表。
         */
        EPGROUP_MAP groupMap_;
        /**
         * @brief 教师。
         */
        epTeacher teacher_;
        /**
         * @brief 白板。
         */
        epWhiteBoard whiteboard_;
};

#endif // __EPCLASSROOM_H__
