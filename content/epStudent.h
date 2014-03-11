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
#include "../protocol.h"

/**
 * @brief 学生类。
 */
class epStudent : public epPerson, public epUser {
public :
        epStudent(void);
        ~epStudent(void);

        /**
         * @brief 使用用户名和密码初始化学生对象。
         *
         * @param std::string[in] 用户名。
         * @param std::string[in] 密码。
         *
         * @return 设置成功返回true，否则返回false。
         */
        virtual bool init(const std::string&
                        , const std::string&
                        , const int
                        , const enum user_status);

        /**
         * @brief 获取客户端类型。
         *
         * @return 相应客户端类型。
         */
        virtual const enum LoginType getType(void) const;

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);

        /**  @} */

        /**
         * @brief 学号。
         */
        std::string studentNum_;

        /**
         * @brief 存放所属班名称。
         */
        std::string className_;

        /**
         * @brief 所属班级ID。
         */
        int classId_;

        /**
        * @brief 监护人名字
        */
        std::string guardian_;

        std::string guardian_phone_;

        /**
        * @brief 所属年纪ID
        */
        int gradeId_;

        /**
        * @brief 所属年纪名字
        */
        std::string gradeName_;

        inline bool operator==(const std::string& account) {
                return epUser::operator==(account);
        }
        inline bool operator==(const int student_id) { return id_ == student_id; }
};

#endif // __EPSTUDENT_H__
