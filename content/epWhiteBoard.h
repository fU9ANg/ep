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
        /**
         * @brief 设置白板ID。
         *
         * @param int[in] 白板ID。
         *
         * @return 成功返回true，否则返回false。
         */
        bool setId(const int);
        /**  @} */

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取白板ID。
         *
         * @return 白板ID。
         */
        int getId(void) const;
        /**  @} */

        /**
         * @name Just for debug
         * @{ */
        virtual void dump(void);
        /**  @} */

private :
        /**
         * @brief 白板编号。
         */
        int id_;
};

#endif // __EPWHITEBOARD_H__
