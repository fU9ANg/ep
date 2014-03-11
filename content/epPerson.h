/**
 * @file epPerson.h
 * @brief 人物基类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPPERSON_H__
#define __EPPERSON_H__

#include <iostream>

/**
 * @brief 人物基类。
 */
class epPerson {
public :
        epPerson (void);
        virtual ~epPerson(void);

        void init(const std::string& first_name
                        , const std::string& last_name
                        , const std::string& sex
                        , const int          age
                        , const std::string& race
                        , const std::string& birthday
                        , const std::string& native
                        , const std::string& address
                        ) {
                firstName_ = first_name;
                lastName_  = last_name;
                sex_       = sex;
                age_       = age;
                race_      = race;
                birthday_  = birthday;
                native_    = native;
                address_   = address;
        }

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        /**
         * @brief 存放姓。
         */
        std::string firstName_;

        /**
         * @brief 存入名。
         */
        std::string lastName_;

        /**
         * @brief 存放性别。
         */
        std::string sex_;

        /**
         * @brief 存放年龄。
         */
        int         age_;

        /**
         * @brief 存放民族。
         */
        std::string race_;

        /**
         * @brief 存放生日的字符串形式。
         */
        std::string birthday_;

        /**
         * @brief 存放国籍，地区。
         */
        std::string native_;
        std::string address_;
};

#endif // __EPPERSON_H__
