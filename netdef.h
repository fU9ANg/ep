#ifndef	__NETDEF_H__
#define __NETDEF_H__

// for Buf.h
#ifndef BUF_MAX_SIZE
#    define BUF_MAX_SIZE	(512)
#endif

#ifndef BUF_INVALIED_FD
#    define BUF_INVALIED_FD	(-1)
#endif

#ifndef BUF_INVALIED_ID
#    define BUF_INVALIED_ID	(-1)
#endif

// for PoolT.h
#ifndef POOLT_MAX_SIZE
#    define POOLT_MAX_SIZE	(20000)
#endif

// for MsgReceiver.cpp
#ifndef MSGRECEIVER_SERV_IP
#    define MSGRECEIVER_SERV_IP	("192.168.0.194")
#endif

#ifndef MSGRECEIVER_SERV_PORT
#    define MSGRECEIVER_SERV_PORT	(8888)
#endif

// for epClass.h
#ifndef EPCLASS_INVALID_CLASS_ID
#       define EPCLASS_INVALID_CLASS_ID (-1)
#endif

// for epSchool.h
#ifndef EPSCHOOL_INVALID_ID
#       define EPSCHOOL_INVALID_SCHOOL_ID      (-1)
#endif

// for epStudent.h
#ifndef EPSTUDENT_INVALIED_ID
#       define EPSTUDENT_INVALIED_STUDENT_ID    (-1)
#endif

// for epClassroom.h
#ifndef EPCLASSROOM_INVALID_CLASSROOM_ID
#       define EPCLASSROOM_INVALID_CLASSROOM_ID (-1)
#endif

// for epTeacher.h
#ifndef EPTEACHER_INVALID_TEACHER_ID
#       define EPTEACHER_INVALID_TEACHER_ID     (-1)
#endif

// for epWhiteBoard.h
#ifndef EPWHITEBOARD_INVALID_WHITEBOARD_ID
#       define EPWHITEBOARD_INVALID_WHITEBOARD_ID       (-1)
#endif

#endif // __NETDEF_H__
