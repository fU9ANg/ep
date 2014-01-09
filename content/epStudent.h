/**
 * @file epStudent.h
 * @brief 学生类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPSTUDENT_H__
#define __EPSTUDENT_H__

#include "epPerson.h"
#include "epUser.h"
#include "../netdef.h" // for EPSTUDENT_INVALIED_ID

/**
 * @brief 学生类。
 */
class epStudent : public epPerson, public epUser {
public :
        epStudent(void) : id_(EPSTUDENT_INVALIED_STUDENT_ID) {}
        ~epStudent(void);

        /**
         * @name set
         * @{ */
        bool setClassName (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        const std::string& getClassName (void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */

private :
        /**
         * @brief 学号。
         */
        int        id_;

        /**
         * @brief 存放所属班。
         */
        std::string className_;
};

#endif // __EPSTUDENT_H__
