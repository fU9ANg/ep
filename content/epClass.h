/**
 * @file epClass.h
 * @brief 班级类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPCLASS_H__
#define __EPCLASS_H__

#include <map>
#include <vector>
#include <list>

#include "epBase.h"
// #include "epManager.h"

#include "epStudent.h"
#include "../Buf.h"

#include <../MapT.hpp>
#include <../ListT.hpp>

/**
 * @brief 班级类。
 */
class epClass : public epBase {
public :

typedef MapT<int, epStudent*> EPSTUDENT_MAP;
typedef ListT<epStudent*>     EPSTUDENT_LIST;

        epClass(void);
        ~epClass(void);

        /**
         * @name get
         * @{ */
        /**
         * @brief 根据学生ID获取学生对象指针。
         *
         * @param int[in] 学生ID。
         *
         * @return 成功返回学生对象指针，否则返回NULL。
         */
        const epStudent* getStudentById(const int);
        /**
         * @brief 获取在线学生列表。
         *
         * @return 在线学生列表。
         */
        const std::vector<int> getActiveStudent(void);
        /**  @} */

        /**
         * @name set
         * @{ */
        /**
         * @brief 将指定fd和学生对象插入到学生列表中。
         *
         * @param epStudent[in] 指定学生对象。
         *
         * @return 插入成功返回true，否则返回false。
         */
        bool insertStudent(const epStudent*);
        /**
         * @brief 从学生列表中移除指定FD的学生对象，并不delete该学生对象。
         *
         * @param int[in] 指定FD。
         *
         * @return 移除成功返回true，否则返回false。
         */
        bool removeStudentByFd(const int);
        bool moveAllStudentToUser(void);

        /**
         * @brief 根据FD删除学生。
         *
         * @param int[in] 指定FD。
         *
         * @return 删除成功返回true，否则返回false。
         */
        bool deleteStudentByFd(const int);
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**
         * @brief 获取指定fd的学生对象指针。
         *
         * @param int[in] 指定FD。
         *
         * @return 成功返回学生对象指针，否则返回NULL。
         */
        const epStudent* getStudentByFd(const int);
        const epUser* getUserByAccount(const std::string&);
        /**  @} */

        /**
         * @name sendto
         * @{ */
        /**
         * @brief 将指定消息内容发送给该班所有学生。
         *
         * @param Buf[in] 指定消息内容。
         * @param bool[in] true表示要发送给自己，false表示不发送给自己。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllStudent(Buf*, const bool toSelf=false);
        /**
         * @brief 将指定消息发送给指定FD的学生。
         *
         * @param int[in] 指定FD。
         * @param Buf[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoStudentByFd(const int, Buf*);
        /**  @} */

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        bool moveStudentFromMapToListByFd(const int);
        bool moveStudentFromListToMapByAccount(const int, const std::string&);

private :
        /**
         * @brief 学生列表。first为FD，second为学生对象。
         */
        EPSTUDENT_MAP  studentMap_;
        EPSTUDENT_LIST offlineStudentList_;
};

#endif // __EPCLASS_H__
