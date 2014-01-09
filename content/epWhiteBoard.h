/**
 * @file epWhiteBoard.h
 * @brief 白板类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-08
 */

#ifndef __EPWHITEBOARD_H__
#define __EPWHITEBOARD_H__

#include "../netdef.h" // for EPWHITEBOARD_INVALID_WHITEBOARD_ID

/**
 * @brief 白板类
 */
class epWhiteBoard {
public :
        epWhiteBoard(void) : id_(EPWHITEBOARD_INVALID_WHITEBOARD_ID) {}
        ~epWhiteBoard(void);

private :
        /**
         * @brief 白板编号。
         */
        int id_;
};

#endif // __EPWHITEBOARD_H__
