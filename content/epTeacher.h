/**
 * @file epTeacher.h
 * @brief 教师类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPTEACHER_H__
#define __EPTEACHER_H__

#include "epPerson.h"
#include "epUser.h"

#include "../netdef.h" // for EPTEACHER_INVALID_TEACHER_ID

/**
 * @brief 教室类。
 */
class epTeacher : public epPerson, public epUser {
public :
        epTeacher(void) : id_(EPTEACHER_INVALID_TEACHER_ID) {}
        ~epTeacher(void);

        /**
         * @name set
         * @{ */
        bool setschoolName (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        const std::string& getschoolName (void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */

private :
        /**
         * @brief 教师编号。
         */
        int id_;

        /**
         * @brief 存放所属学校。
         */
        std::string schoolName_;
};
