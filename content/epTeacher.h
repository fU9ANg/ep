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

/**
 * @brief 教室类。
 */
class epTeacher : public epPerson, public epUser {
public :
        epTeacher(void);
        ~epTeacher(void);

        /**
         * @name set
         * @{ */
        bool setId(const int);
        bool setschoolName (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        int getId(void) const;
        const std::string& getschoolName (void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */

        /**
         * @brief 打印该对象的属性。
         */
        virtual void dump(void);

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

#endif // __EPTEACHER_H__
