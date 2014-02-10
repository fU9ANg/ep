/**
 * @file epHeadmaster.h
 * @brief 校长类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-11
 */

#ifndef __EPHEADMASTER_H__
#define __EPHEADMASTER_H__

#include "epUser.h"
#include "epPerson.h"

/**
 * @brief 校长类。
 */
class epHeadmaster : public epPerson, public epUser {
public :
        bool init(const std::string&, const std::string&);
        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

        int schoolId_;
        std::string schoolName_;
        /**
         * @brief 园长编号。
         */
        std::string headmasterNum_;
};

#endif // __EPHEADMASTER_H__
