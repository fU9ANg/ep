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
        CT_StartClass = 200,
        ST_StartClass,

        // 开始课件。
        CT_Courseware,
        ST_Courseware,

        // 下课。
        CT_ClassOver,
        ST_ClassOver,

        // 获取学生列表。
        CT_GetStudentList,
        ST_GetStudentList,

        // 获取在线学生列表。教师点击上课时由教师端或学生端主动请求的活动学生列表。
        CT_GetActiveStudentList,
        ST_GetActiveStudentList,

        CT_GetActiveStudentList_1,
        ST_GetActiveStudentList_1,

        // 当晚来的学生登录并选择学校功能时，需要将该学生的上线信息通知给该教室内上课的其它学生。
        ST_UpdateStudentStatus,

        // 获取学生详细信息。
        CT_GetStudentInfo,
        ST_GetStudentInfo,

        // 获取老师详细信息。
        CT_GetTeacherInfo,
        ST_GetTeacherInfo,

        // 分组
        CT_SetGroup,
        ST_SetGroup,

        // 多人接力。
        CT_Relay,
        ST_Relay,
        /*}}}*/

        // 绘本。/*{{{*/
        // 请求个人服务器绘本教材列表。
        CT_GetPersonalBooksList = 300,
        ST_GetPersonalBooksList,

        // 请求公共服务器绘本教材列表。
        CT_GetPublicBooksList,
        ST_GetPublicBooksList,

        // 请求上传下载地址。(包括私人和公共)
        CT_GetServerAddr,
        ST_GetServerAddr,

        // 将本地绘本教材上传到个人服务器。
        CT_UploadBook,
        ST_UploadBook,

        // 从私人服务器进行下载。
        CT_DownloadFromPersonal,
        ST_DownloadFromPersonal,

        // 从公共服务器进行下载。
        CT_DownloadFromPublic,
        ST_DownloadFromPublic,

        // 指定传输。
        CT_TransferBook,
        ST_TransferBook,

        // 发布。
        CT_Publish,
        ST_Publish,
/*}}}*/

        // 绘画/*{{{*/
        // 同步绘画设置。
        CT_UpdateDrawSet,
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

/*}}}*/
};

enum LoginType {
        LT_USER,
        LT_HEADMASTER,
        LT_TEACHER,
        LT_PARENTS,
        LT_STUDENT,
        LT_WHITEBOARD,
};

enum FuncType {
        FT_INVALID,
        FT_PERSONAL = 1,
        FT_SCHOOL,
        FT_FAMILY,
        FT_FRIEND,
        FT_OFFICE,
};

enum BookType {
        BT_INVALID,
        BT_BOOK,        // 书本教材
        BT_DRAW,        // 图画教材
        BT_LINE,        // 连线教材
        BT_PULLZE,      // 拼图教材
        BT_MIDI,        // 语音教材
};

enum UserStatus {
        US_INVALIED,
        US_ONLINE,
        US_OFFLINE,
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
