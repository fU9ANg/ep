#ifndef __FUN_OBJ_HPP__
#define __FUN_OBJ_HPP__

/*
class dump_obj {
public :
        template <typename M, typename N>
        inline void
        operator() (std::pair<M, N>& map_pair) {
                (map_pair.second)->dump();
        }

        template <typename M>
        inline void
        operator() (M* obj) {
                obj->dump();
        }
};

class delete_obj {
public :
        template <typename M, typename N>
        inline void
        operator() (std::pair<M, N>& map_pair) {
                delete map_pair.second;
        }

        template <typename M>
        inline void
        operator() (M* obj) {
                delete obj;
        }
};

template <typename T>
class compare_obj {
public :
        compare_obj(const T& obj) : obj_(obj) { }

        template <typename M, typename N>
        inline bool
        operator() (std::pair<M, N>& map_pair) {
                // return *(map_pair.second) == obj_;
                return (map_pair.second)->operator==(obj_);
        }

        template <typename M>
        inline bool
        operator() (M* obj) {
                return obj->operator==(obj_);
        }

private :
        const T& obj_;
};
*/

#include <map>

template <typename T, typename _Res>
class fun_obj {
public :
        fun_obj(T obj) : obj_(obj) { }

        template <typename M, typename N>
        inline _Res
        operator() (std::pair<M, N>& map_pair) {
                return obj_(map_pair.second);
        }

        template <typename M>
        inline _Res
        operator() (M* obj) {
                return obj_(obj);
        }

private :
        T obj_;
};

template <typename T>
class fun_obj<T, void> {
public :
        fun_obj(T obj) : obj_(obj) { }

        template <typename M, typename N>
        inline void
        operator() (std::pair<M, N>& map_pair) {
                obj_(map_pair.second);
        }

        template <typename M>
        inline void
        operator() (M* obj) {
                obj_(obj);
        }

private :
        T obj_;
};

template <typename T>
class compare_operation {
public :
        compare_operation(const T& obj) : obj_(obj) { }
        template <typename M>
        inline bool
        operator() (M* obj) {
                return (*obj) == obj_;
        }
private :
        const T& obj_;
};

class delete_operation {
public :
        template <typename T>
        inline void
        operator() (T* obj) {
                delete obj;
        }
};

class dump_operation {
public :
        template <typename T>
        inline void
        operator() (T* obj) {
                obj->dump();
        }
};

#include "Single.h"
#include "netdef.h"

class send_to {
public :
        send_to(Buf* p, const bool to_self) : p_buf_(p), p_tmp_(), to_self_(to_self) { }
        virtual ~send_to() {
                if (NULL != p_buf_) {
                        SINGLE->bufpool.free(p_buf_);
                        p_buf_ = NULL;
                }
        }

protected :
        Buf* p_buf_;
        Buf* p_tmp_;
        const bool to_self_;
};

#include "../message/proto/protocol.pb.h"
class send_to_user : public send_to {
public :
        send_to_user(Buf* p, const bool to_self) : send_to(p, to_self) { }

        template <typename T> void
        operator() (T* obj) {
                if (US_ONLINE == obj->userStatus_
                                && (p_buf_->getfd()!=obj->fd_ || to_self_)) {
                        CLONE_BUF(p_tmp_, p_buf_);
                        p_tmp_->setfd(obj->fd_);
                        SINGLE->sendqueue.enqueue(p_tmp_);
                }
        }
};

/*
class send_to_ {
public :
        send_to_(Buf* p) : p_buf_(p), p_tmp_() {}
        virtual ~send_to_(void) {
                if (NULL != p_buf_) {
                        SINGLE->bufpool.free(p_buf_);
                        p_buf_ = NULL;
                }
        }

private :
        Buf* p_buf_;
        Buf* p_tmp_;
};

template <typename COND, typename SEND_TO>
class send_to_if : public send_to_ {
public :
        send_to_if(Buf* p, COND cond) : send_to_(p, cond) {}
        void operator() (T* obj) {
                if (cond(obj)) {
                        CLONE_BUF(p_tmp_, p_buf_);
                        p_tmp_->setfd(obj->fd_);
                        SINGLE->sendqueue.enqueue(p_tmp_);
                }
        }
};
*/

/*
template <typename T, typename COND>
class broadcast_if : public send_to_<COND> {
};

template <typename cond>
class send_to_user_if {
};
*/

template <typename T, typename TAG>
class define_type {
public :
        define_type(void) {}
        define_type(const T& _Arg) : _M(_Arg) {}
        T value(void) { return _M; }
        define_type& operator=(const T& rhs) { _M = rhs; return *this; }
        define_type& operator=(const define_type& rhs) { if (*this != rhs) _M = rhs.value(); return *this; }
private :
        T _M;
};

// 对一个user对象（epUser及其子类的对象作相应操作）
class do_for_user {
public :
        template <typename condition>
        void operator() (condition cond) {
                if (cond) {
                }
        }
private :
};

/*
   template <typename MSG_LIST_TYPE, typename MSG_TYPE>
   class combine_pkg {
   public :
   combine_pkg(MSG_TYPE msg_type, int fd) : node_(), msg_type_(mst_type), cnt_(), fd_(fd) {}

   template <typename MSG_NODE_TYPE>
   void operator() (MSG_NODE_TYPE& tmp) {
   MSG_NODE_TYPE* node_ = obj_.add_node();
   (*node_) = tmp;
   ++cnt_;
   }

   ~combine_pkg(void) {
   Buf* pBuf = packet_list(msg_type_, obj_, fd_);
   if(NULL != pBuf) {
   SINGLE->sendqueue.enqueue(pBuf);
   }
   }

   private :
   MSG_LIST_TYPE obj_;
   MSG_TYPE msg_type_;
   int cnt_;
   int fd_;
   };
   */

#endif // __FUN_OBJ_HPP__
