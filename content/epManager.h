/**
 * @file epManager.h
 * @brief 管理类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPMANAGER_H__
#define __EPMANAGER_H__

#include <map>

#include "Uncopyable.h"
#include "epClassroom.h"

typedef std::map<int, epClassroom> EPCLASSROOM_MAP;
typedef std::map<int, epUser*> EPUSER_MAP;

/**
 * @brief 管理类。
 */
class epManager : public Uncopyable {
public :
        static epManager*  instance(void);

        /**
         * @brief 插入指定教室到管理类。
         *
         * @param epClassroom 指定教室。
         *
         * @return 插入成功返回true，否则返回false。
         */
        bool               insertClassroom     (const epClassroom&);
        /**
         * @brief 移除指定ID的教室，并不删除教室类。
         *
         * @param int 
         *
         * @return 
         */
        bool               deleteClassroomById (int);
        const epClassroom* getClassroomById    (int);

        /**
         * @brief 将指定FD的对象插入到游离列表中。
         *
         * @param int 指定FD。
         * @param epUser 指定对象。
         *
         * @return 插入成功返回true，否则返回false。
         */
        bool insertUser(int, const epUser&);
        /**
         * @brief 从列表中删除指定FD的对象。并不delete对象，只是在列表中删除关系。
         *
         * @param int 指定FD
         *
         * @return 移除成功返回true, 失败返回 false.
         */
        bool removeUserByFd(const int);
        /**
         * @brief 获取指定FD的对象指针。
         *
         * @param int[in] 需要查找对象的FD。
         *
         * @return 成功返回指向相应对象的指针，失败返回NULL。
         */
        const epUser* getUserByFd(const int);

        /**
         * @name Just for debug
         * @{ */
        /**
         * @brief 打印教室列表。
         */
        virtual void dumpClassroom(void) const;
        /**
         * @brief 打印游离状态的 client。
         */
        virtual void dumpUser(void) const;
        /**  @} */

private :
        static epManager* p_;
        epManager (void);
        ~epManager(void);

private :
        /**
         * @brief 教室列表。first为教室ID
         */
        EPCLASSROOM_MAP classroomMap_;
        /**
         * @brief 存入游离的FD，first为FD。
         */
        EPUSER_MAP userMap_;
};

#define EPMANAGER       (epManager::instance())

#endif // __EPMANAGER_H__
