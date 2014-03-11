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
#include "../message/proto/protocol.pb.h"
#include "../Mutex.h"
#include <map>
#include "epObject.h"

/**
 * @brief 用户类。
 */
class epUser : public epObject {
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
        virtual bool init(const std::string&
                        , const std::string&
                        , const int
                        , const enum user_status);

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

        bool operator==(const std::string&);

        bool check(const std::string&, const std::string&);

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
        enum user_status userStatus_;
        /**
         * @brief 存放账号。
         */
        std::string account_;
        /**
         * @brief 存放密码。
         */
        std::string passwd_;

protected :
        MutexLock lock_;

public :
};

#endif // __EPUSER_H__
