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

        /**
         * @name set
         * @{ */
        bool setName     (const std::string&);
        bool setName     (const std::string&, const std::string&);
        bool setSex      (const std::string&);
        bool setAge      (const int);
        bool setRace     (const std::string&);
        bool setBirthday (const std::string&);
        bool setNative   (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        const std::string getName     (void) const;
        const std::string& getFirstname(void) const;
        const std::string& getSex      (void) const;
        int                getAge      (void) const;
        const std::string& getRace     (void) const;
        const std::string& getBirthday (void) const;
        const std::string& getNative   (void) const;
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
};

#endif // __EPPERSON_H__
