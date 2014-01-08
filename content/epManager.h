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

/**
 * @brief 管理类。
 */
class epManager : public Uncopyable {
public :
        static epManager* instance(void);

private :
        static epManager* p_;
        epManager (void);
        ~epManager(void);

private :
        /**
         * @brief 存放教室列表。
         */
        EPCLASSROOM_MAP classroomMap_;

        bool               insertClassroom     (const epClassroom&);
        bool               deleteClassroomById (int);
        const epClassroom& getClassroomById    (int);
};

#define EPMANAGER       (epManager::instance())

#endif // __EPMANAGER_H__
