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

/**
 * @brief 学校类。
 */
class epSchool {
public :
        epSchool(void) : id_(EPSCHOOL_INVALIED_SCHOOL_ID) {}
        ~epSchool(void);

        /**
         * @name get
         * @{ */
        int                getSchoolId   (void) const;
        const std::string& getSchoolName (void) const;
        const std::string& getNative     (void) const;
        /**  @} */

        /**
         * @name set
         * @{ */
        bool setSchoolId     (const int);
        bool setSchoolName   (const std::string&);
        bool setSchoolNative (const std::string&);
        /**  @} */

        /**
         * @name get by ???
         * @{ */
        /**  @} */


private :
        /**
         * @brief 学校ID
         */
        int         id_;

        /**
         * @brief 学校名称。
         */
        std::string schoolName_;

        /**
         * @brief 学校所属地区。
         */
        std::string native_;
};

#endif // __EPSCHOOL_H__
