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

#include "epStudent.h"

typedef std::map<int, epStudent> EPSTUDENT_MAP;

/**
 * @brief 班级类。
 */
class epClass {
public :
        epClass(void);
        ~epClass(void);

        /**
         * @name get
         * @{ */
        int getClassId(void);
        /**  @} */

        /**
         * @name set
         * @{ */
        bool insertStudent     (const epStudent&);
        bool deleteStudentById (const int);
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        const epStudent* getStudentById(const int);
        /**  @} */

private :
        /**
         * @brief 班级ID。
         */
        int           id_;
        /**
         * @brief 学生列表。first为学号，second为学生对象。
         */
        EPSTUDENT_MAP studentMap_;
};

#endif // __EPCLASS_H__
