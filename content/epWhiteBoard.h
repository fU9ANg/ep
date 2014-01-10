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
#include "../protocol.h"

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
         * @brief 设置白板编号。
         *
         * @param int[in] 白板编号。
         *
         * @return 成功返回true，否则返回false。
         */
        virtual bool setWhiteboardNum(const std::string&);
        /**
         * @brief 使用用户名和密码初始化白板对象。
         *
         * @param std::string[in] 用户名。
         * @param std::string[in] 密码。
         *
         * @return 成功返回true，否则返回false。
         */
        virtual bool init(const std::string&, const std::string&);
        /**  @} */

        /**
         * @name get
         * @{ */
        /**
         * @brief 获取白板编号。
         *
         * @return 白板编号。
         */
        virtual std::string getWhiteboardNum(void) const;
        /**
         * @brief 获取客户端类型。
         *
         * @return 客户端类型。
         */
        virtual enum LoginType getType(void) const;
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
        std::string whiteboardNum_;
};

#endif // __EPWHITEBOARD_H__
