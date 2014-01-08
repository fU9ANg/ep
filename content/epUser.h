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
        ~epUser(void);

        /**
         * @name set
         * @{ */
        bool setAccount(const std::string&);
        bool setPasswd (const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        const std::string& getAccount(void) const;
        const std::string& getPasswd (void) const;
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
