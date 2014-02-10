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
#include "../protocol.h"
/**
 * @brief 教师类。
 */
class epTeacher : public epPerson, public epUser {
public :
        epTeacher(void);
        ~epTeacher(void);

        /**
         * @brief 通过传入的用户名和密码初始化教师对象。
         *
         * @param std::string[in] 用户名。
         * @param std::string[in] 密码。
         *
         * @return 成功返回true，否则返回false。
         */
        virtual bool init(const std::string&, const std::string&);

        /**
         * @brief 获取客户端相应类型。
         *
         * @return 客户端类型。
         */
        virtual const enum LoginType getType(void) const;

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        /**
         * @brief 教师编号。
         */
        std::string teacherNum_;
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
