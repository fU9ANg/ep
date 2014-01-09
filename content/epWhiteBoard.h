/**
 * @file epWhiteBoard.h
 * @brief 白板类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPWHITEBOARD_H__
#define __EPWHITEBOARD_H__

#include "epUser.h"

/**
 * @brief 白板类
 */
class epWhiteBoard : public epUser {
public :
        epWhiteBoard(void);
        ~epWhiteBoard(void);

        /**
         * @name set
         * @{ */
        bool setId(const int);
        /**  @} */

        /**
         * @name get
         * @{ */
        int getId(void) const;
        /**  @} */

        /**
         * @brief 打印该对象的属性。
         */
        virtual void dump(void);

private :
        /**
         * @brief 白板编号。
         */
        int id_;
};

#endif // __EPWHITEBOARD_H__
