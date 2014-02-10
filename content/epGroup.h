/**
 * @file epGroup.h
 * @brief 组别类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-09
 */

#ifndef __EPGROUP_H__
#define __EPGROUP_H__

#include <map>
#include "../Buf.h"

#include "epBase.h"

#include "epStudent.h"

typedef std::map<int, epStudent*> STUDENT_MAP;

/**
 * @brief 组别类。
 */
class epGroup : public epBase {
public :
        epGroup(void);
        ~epGroup(void);
        /**
         * @name set
         * @{ */
        /**
         * @brief 向学生列表中插入指定FD和学生对象。
         *
         * @param int[in] 指定FD。
         * @param epStudent 指定学生对象。
         *
         * @return 成功返回true，否则返回false。
         */
        bool insertStudent(int, epStudent*);
        /**
         * @brief 移除学生列表中的指定FD的学生对象，并不delete学生对象。
         *
         * @param int[in] 指定FD。
         *
         * @return 成功返回true，否则返回false。
         */
        bool removeStudentByFd(const int);
        bool removeAllStudent(void);
        /**  @} */

        /**
         * @name get
         * @{ */
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**
         * @brief 获取学生列表中指定FD的学生对象指针。
         *
         * @param int[in] 指定FD。
         *
         * @return 成功返回学生对象指针，否则返回NULL。
         */
        const epStudent* getStudentByFd(const int);
        int getNextIdByFd(const int);
        /**  @} */

        /**
         * @brief 将指定消息发送给该组所有学生。
         *
         * @param Buf*[in] 指定消息内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool sendtoAllStudent(Buf*, const bool toSelf=false);
        bool sendtoStudentByFd(const int, Buf*);

        bool setLock(const int);

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */


private :
        /**
         * @brief 该组学生列表。
         */
        STUDENT_MAP studentMap_;
        /**
         * @brief 组内同步锁。
         */
        int lock_;
};

#endif // __EPGROUP_H__
