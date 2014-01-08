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
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID

typedef std::map<int, epClass> EPCLASS_MAP;

/**
 * @brief 教室类。
 */
class epClassroom {
public :
        epClassroom(void) : id_(EPCLASSROOM_INVALID_CLASSROOM_ID) {}
        ~epClassroom(void);

        bool           insertClass     (const epClass&);
        bool           deleteClassById (int);
        const epClass& getClassById    (int);

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
         * @brief 教师。
         */
        epTeacher teacher_;
        /**
         * @brief 白板。
         */
        epWhiteBoard whiteboard_;
};

#endif // __EPCLASSROOM_H__
