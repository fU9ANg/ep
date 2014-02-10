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
#include "../protocol.h"

/**
 * @brief 用户类。
 */
class epUser {
public :
        epUser(void);
        virtual ~epUser(void);

        /**
         * @brief 使用传入的用户名和密码初始化对象。
         *
         * @param std::string[in] 用户名。
         * @param std::string[in] 密码。
         *
         * @return 成功返回true，否则返回false。
         */
        virtual bool init(const std::string&, const std::string&);

        /**
         * @brief 获取该客户端类型。
         *
         * @return 相应相应客户端类型。
         */
        virtual const enum LoginType getType(void) const;

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

public :
        /**
         * @brief 功能类型。
         */
        enum FuncType funcType_;
        /**
         * @brief 用户ID。
         */
        int id_;
        /**
         * @brief 用户FD。
         */
        int fd_;
        /**
         * @brief 记录用户的状态。
         */
        enum UserStatus userStatus_;
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
