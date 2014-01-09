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

/**
 * @brief 学生类。
 */
class epStudent : public epPerson, public epUser {
public :
        epStudent(void);
        ~epStudent(void);

        /**
         * @name set
         * @{ */
        bool setId(const int);
        bool setClassId(const int);
        bool setClassName (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        int getId(void) const;
        int getClassId(void) const;
        const std::string getClassName (void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */

        virtual void dump(void);

private :
        /**
         * @brief 学号。
         */
        int        id_;
        /**
         * @brief 存放所属班。
         */
        std::string className_;
        /**
         * @brief 所属班级ID。
         */
        int classId_;
};

#endif // __EPSTUDENT_H__
