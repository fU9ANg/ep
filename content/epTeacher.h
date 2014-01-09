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
 * @brief 教师类。
 */
class epTeacher : public epPerson, public epUser {
public :
        epTeacher(void);
        ~epTeacher(void);

        /**
         * @name set
         * @{ */
        /**
         * @brief 设置老师ID。
         *
         * @param int[in] 老师ID。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setId(const int);
        /**
         * @brief 设置老师所属学校名字。
         *
         * @param std::string[in] 学校名字。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setSchoolName(const std::string&);
        /**
         * @brief 设置老师所属学校ID。
         *
         * @param int[in] 学校ID。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setSchoolId(const int);
        /**  @} */

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取老师ID。
         *
         * @return 老师ID。
         */
        int getId(void) const;
        /**
         * @brief 获取老师所属学校名字。
         *
         * @return 学校名字。
         */
        const std::string& getSchoolName (void) const;
        /**
         * @brief 获取老师所属学校ID。
         *
         * @return 学校ID。
         */
        int getSchoolId(void) const;
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

private :
        /**
         * @brief 教师编号。
         */
        int id_;
        /**
         * @brief 存放所属学校名字。
         */
        std::string schoolName_;
        /**
         * @brief 所属学校ID。
         */
        int schoolId_;
};

#endif // __EPTEACHER_H__
