/**
 * @file epSchool.h
 * @brief 学校类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPSCHOOL_H__
#define __EPSCHOOL_H__

#include <iostream>
#include "epBase.h"

/**
 * @brief 学校类。
 */
class epSchool : public epBase {
public :
        epSchool(void);
        ~epSchool(void);

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取学校ID。
         *
         * @return 学校ID。
         */
        const std::string getNative(void) const;
        /**  @} */

        /**
         * @name set
         * @{ */
        /**
         * @brief 设置学校所属地区。
         *
         * @param std::string[in] 学校所属地区。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setSchoolNative(const std::string&);
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
         * @brief 学校所属地区。
         */
        std::string native_;
};

#endif // __EPSCHOOL_H__
