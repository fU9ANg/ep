#include <string.h>

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#pragma pack(1)
enum CommandType {
        CT_Test = 1,
        ST_Test,

        CT_Heart,
        ST_Heart,

        CT_Auth,
        ST_Auth,

        // 登录/*{{{*/
        CT_Login = 100,
        ST_Login,

        CT_Logout,
        ST_Logout,

        CT_GetFuncList,
        ST_GetFuncList,

        CT_EnableClassroom,
        ST_EnableClassroom,

        CT_SetFunc,
        ST_SetFunc,

        CT_GetSchoolAccountList,
        ST_GetSchoolAccountList,

        CT_GetGradeList,
        ST_GetGradeList,

        CT_GetCourseList,
        ST_GetCourseList,

        CT_GetClassList,
        ST_GetClassList,

        CT_GetClassRoomList,
        ST_GetClassRoomList,

        // 设置上课内容。设置课程，班级，教室。
        CT_SetContent,
        ST_SetContent,

        CT_GetContent,
        ST_GetContent,
        /*}}}*/
        // 开始上课。学生进入电子教室。/*{{{*/
        CT_StartClass = 300,
        ST_StartClass,

        // 请求教室状态。
        CT_GetClassroomStatus,
        ST_GetClassroomStatus,

        // 请求教室信息。
        CT_GetClassroomInfo,
        ST_GetClassroomInfo,

        // 开始课件。
        CT_Courseware,
        ST_Courseware,

        // 更新课程。
        CT_UpdateCourseware,
        ST_UpdateCourseware,

        // 获得课程状态列表。
        CT_GetCoursewareListStatus,
        ST_GetCoursewareListStatus,

        // 结束课件。
        CT_CourseOver,
        ST_CourseOver,

        // 早退。
        CT_LeaveEarly,
        ST_LeaveEarly,

        // 下课。
        CT_ClassOver,
        ST_ClassOver,

        // 获取学生列表。
        CT_GetStudentList,
        ST_GetStudentList,

        // 获取在线学生列表。教师点击上课时由教师端或学生端主动请求的活动学生列表。
        CT_GetActiveStudentList,
        ST_GetActiveStudentList,

        CT_GetGroupStudentList,
        ST_GetGroupStudentList,

        // 当晚来的学生登录并选择学校功能时，需要将该学生的上线信息通知给该教室内上课的其它学生。
        ST_UpdateStudentStatus,

        // 获取学生详细信息。
        CT_GetStudentInfo,
        ST_GetStudentInfo,

        // 获取老师详细信息。
        CT_GetTeacherInfo,
        ST_GetTeacherInfo,

        // 获取班级信息
        CT_GetClassInfo,
        ST_GetClassInfo,

        // 获取白板信息
        CT_GetWhiteboardInfo,
        ST_GetWhiteboardInfo,



        // 分组
        CT_SetGroup,
        ST_SetGroup,

        // 组列表。
        CT_GetGroupList,
        ST_GetGroupList,

        CT_GetGroupInfo,
        ST_GetGroupInfo,

        // 多人接力。
        CT_Relay,
        ST_Relay,
        /*}}}*/

        // 绘本。/*{{{*/
        // 请求公共服务器绘本教材列表。
        CT_GetPublicBooksList = 500,
        ST_GetPublicBooksList,

        // 请求商店绘本教材列表。
        CT_GetStoreBooksList,
        ST_GetStoreBooksList,

        // 请求上传下载地址。(包括商店和公共)
        CT_GetServerAddr,
        ST_GetServerAddr,

        // 发布。
        CT_Publish,
        ST_Publish,

        // 从公共服务器进行下载。
        CT_DownloadFromPublic,
        ST_DownloadFromPublic,

        // 赠送。
        CT_Largess,
        ST_Largess,

        // 转让。
        CT_Assignment,
        ST_Assignment,

        // 购买
        CT_Purchase,
        ST_Purchase,

        // 出售。
        CT_Sell,
        ST_Sell,

        // 更新网络绘本。
        CT_UpdatePublic,
        ST_UpdatePublic,

        // 收藏。
        CT_StoreUp,
        ST_StoreUp,

        /*}}}*/

        // 绘画/*{{{*/
        // 同步绘画设置。
        CT_UpdateDrawSet = 700,
        ST_UpdateDrawSet,

        // 同步绘画信息。
        CT_UpdateDrawInfo,
        ST_UpdateDrawInfo,

        // 更新组列列表。
        ST_UpdateGroupStatus, // 只需要group_id，student_id

        // 同步纸张。
        CT_UpdateBackground,
        ST_UpdateBackground,

        // 同步临摹图片到绘画层
        CT_UpdateCopyIntoPaint,
        ST_UpdateCopyIntoPaint,

        // 同步画笔、橡皮擦
        CT_UpdatePenAnderaser,
        ST_UpdatePenAnderaser,

        // 同步填色。
        CT_UpdateFilling,
        ST_UpdateFilling,

        // 同步印章 TODO
        CT_UpdateStamp,
        ST_UpdateStamp,

        // 同步填图
        CT_UpdateFillPic,
        ST_UpdateFillPic,

        // 同步锁定。
        CT_UpdateMutex,
        ST_UpdateMutex,

        // 同步屏贴。
        CT_UpdateCollage,
        ST_UpdateCollage,

        // 同步文字
        CT_UpdateWord,
        ST_UpdateWord,

        // 同步边框。
        CT_UpdateFrame,
        ST_UpdateFrame,

        CT_GetStudentDrawLog,
        ST_GetStudentDrawLog,

        CT_GetNextStudent,
        ST_GetNextStudent,

        CT_Display,
        ST_Display,

        CT_unDisplay,
        ST_unDisplay,

        CT_Enlarge,
        ST_Enlarge,

        CT_Reduce,
        ST_Reduce,
        /*}}}*/

        CT_GetPuzzleInfo,
        ST_GetPuzzleInfo,

        CT_UpdatePuzzleResult,
        ST_UpdatePuzzleResult,

        // 更新连线。
        CT_UpdateConn,
        ST_UpdateConn,

        CT_LockConn,
        ST_LockConn,

        CT_Lock = 1000,
        ST_Lock,

        CT_unLock,
        ST_unLock,
};


enum BookType {
        BT_INVALID = 0,
        BT_BOOK,        // 书本教材
        BT_DRAW,        // 图画教材
        BT_LINE,        // 连线教材
        BT_PULLZE,      // 拼图教材
        BT_MIDI,        // 语音教材
        BT_COURSEWARE,
        BT_ENCYCLOPEDIAS,
};

// 下面是所有用到的数据结构体
//

///////////////////

typedef struct sSubmitData {
        unsigned int cLen;          // 数据长度
        enum CommandType cType;     // 数据编号
        void* cData() {             // 指向数据的指针
                return this + 1;
        }
} MSG_HEAD;

#define MSG_HEAD_LEN    sizeof(struct sSubmitData)

#pragma pack()

#endif //_PROTOCOL_H
