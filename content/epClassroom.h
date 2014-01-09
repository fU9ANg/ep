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
        const int getId(void) const;
        const epTeacher getTeacher(void) const;
        const epWhiteBoard getWhiteBoard(void) const;
        /**  @} */

        /**
         * @name set
         * @{ */
        bool setId(const int);
        bool setTeacher(const epTeacher&);
        bool setWhiteBoard(const epWhiteBoard&);
        /**  @} */

private :
        /**
         * @brief 教室ID。
         */
        int id_;
        /**
         * @brief 班列表。
         */
        epClass   class_;
        /**
         * @brief 组列表。
         */
        GROUP_MAP groupMap_;
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
