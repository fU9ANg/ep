/**
 * @file epUser.h
 * @brief 用户类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPUSER_H__
#define __EPUSER_H__

#include <iostream>

/**
 * @brief 用户类。
 */
class epUser {
public :
        epUser(void);
        virtual ~epUser(void);

        /**
         * @name set
         * @{ */
        /**
         * @brief 设置帐号。
         *
         * @param std::string[in] 指定帐号内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setAccount(const std::string&);
        /**
         * @brief 设置密码。
         *
         * @param std::string[in] 指定密码内容。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setPasswd (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取帐号。
         *
         * @return 成功返回帐号。
         */
        const std::string& getAccount(void) const;
        /**
         * @brief 获取密码。
         *
         * @return 成功返回密码。
         */
        const std::string& getPasswd (void) const;
        /**  @} */

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

private :
        /**
         * @brief 存放账号。
         */
        std::string account_;
        /**
         * @brief 存放密码。
         */
        std::string passwd_;
};

#endif // __EPUSER_H__
