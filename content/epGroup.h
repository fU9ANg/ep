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

#include "epStudent.h"

typedef std::map<int, epStudent> STUDENT_MAP;

/**
 * @brief 组别类。
 */
class epGroup {
public :
        epGroup(void);
        ~epGroup(void);
        /**
         * @name set
         * @{ */
        bool setId(const int);
        bool insertStudent(const epStudent&);
        bool deleteStudentById(const int);
        /**  @} */

        /**
         * @name get
         * @{ */
        int getId(void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        const epStudent* getStudentById(const int);
        /**  @} */

        bool sendtoGroup(Buf*);

private :
        /**
         * @brief 组ID
         */
        int id_;
        /**
         * @brief 该组学生列表。
         */
        STUDENT_MAP studentMap_;
};

#endif // __EPGROUP_H__
