/**
 * @file epBase.h
 * @brief 管理基类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-14
 */
#ifndef __EPBASE_H__
#define __EPBASE_H__

#include <iostream>

/**
 * @brief 管理基类。
 */
class epBase {
public :
        epBase(void);
        virtual ~epBase(void);

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        // virtual const enum ManagerType getType(void) = 0;

        /**
         * @brief 同类标识ID。
         */
        int id_;
        /**
         * @brief 管理者名称。
         */
        std::string name_;
        /**
         * @brief 是否已经使用。
         */
        bool isUsed_;
};

#endif // __EPBASE_H__
