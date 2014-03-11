#ifndef __EPLOGBASE_H__
#define __EPLOGBASE_H__

#include "../Mutex.h"
#include <fstream>

enum LogType {
        LOG_INVALID,
        LOG_CONN,
        LOG_DRAW,
        LOG_PUZZLE,
};

typedef enum LogType log_type;

template <log_type T>
class epLog {
public :
        typedef std::fstream log_file_type;
        typedef std::string  log_filename_type;
        typedef std::string  buf_type;

        epLog(log_filename_type& filename) : log_filename_(filename) {}
        virtual ~epLog(void) { if (log_.is_open()) log_.close(); }

        virtual const log_type get_type(void) { return T; }

        bool write_log(buf_type& buf) {
                MutexLockGuard guard(pthread_lock_);

                if (!log_.is_open())
                        log_.open(log_filename_.c_str(), log_file_type::in | log_file_type::out);
                log_.seekg(0, log_.end);
                log_ << buf << std::endl;
                return true;
        }

        log_file_type& get_log_file(void) {
                return log_;
        }

protected :
        MutexLock         pthread_lock_;
        log_filename_type log_filename_;
        log_file_type     log_;
};

#include "../Buf.h"

template <log_type T>
class read_log {
public :

        read_log(typename epLog<T>::log_file_type& log)
                : log_(log), read_cnt_(), max_read_cnt_()
        {
                log_.seekg(0, log_.end);
                max_read_cnt_ = log_.tellg();
        }

        typename epLog<T>::buf_type operator() () {
                if (!log_.is_open() || read_cnt_==max_read_cnt_)
                        return typename epLog<T>::buf_type();

                log_.getline(buf, MAX_BUF);
                typename epLog<T>::buf_type tmp(buf);
                read_cnt_ += tmp.size();
                return tmp;
        }

private :
        typename epLog<T>::log_file_type& log_;
        int                       read_cnt_;
        int                       max_read_cnt_;
        char buf[MAX_BUF];
};

#endif // __EPLOGBASE_H__
