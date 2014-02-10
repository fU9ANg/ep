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

        // ��¼/*{{{*/
        CT_Login = 100,
        ST_Login,

        CT_Logout,
        ST_Logout,

        CT_GetFuncList,
        ST_GetFuncList,

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

        // �����Ͽ����ݡ����ÿγ̣��༶�����ҡ�
        CT_SetContent,
        ST_SetContent,

        CT_GetContent,
        ST_GetContent,
/*}}}*/
        // ��ʼ�ϿΡ�ѧ��������ӽ��ҡ�/*{{{*/
        CT_StartClass = 200,
        ST_StartClass,

        // ��ʼ�μ���
        CT_Courseware,
        ST_Courseware,

        // �¿Ρ�
        CT_ClassOver,
        ST_ClassOver,

        // ��ȡѧ���б�
        CT_GetStudentList,
        ST_GetStudentList,

        // ��ȡ����ѧ���б���ʦ����Ͽ�ʱ�ɽ�ʦ�˻�ѧ������������Ļѧ���б�
        CT_GetActiveStudentList,
        ST_GetActiveStudentList,

        CT_GetActiveStudentList_1,
        ST_GetActiveStudentList_1,

        // ��������ѧ����¼��ѡ��ѧУ����ʱ����Ҫ����ѧ����������Ϣ֪ͨ���ý������Ͽε�����ѧ����
        ST_UpdateStudentStatus,

        // ��ȡѧ����ϸ��Ϣ��
        CT_GetStudentInfo,
        ST_GetStudentInfo,

        // ��ȡ��ʦ��ϸ��Ϣ��
        CT_GetTeacherInfo,
        ST_GetTeacherInfo,

        // ����
        CT_SetGroup,
        ST_SetGroup,

        // ���˽�����
        CT_Relay,
        ST_Relay,
        /*}}}*/

        // �汾��/*{{{*/
        // ������˷������汾�̲��б�
        CT_GetPersonalBooksList = 300,
        ST_GetPersonalBooksList,

        // ���󹫹��������汾�̲��б�
        CT_GetPublicBooksList,
        ST_GetPublicBooksList,

        // �����ϴ����ص�ַ��(����˽�˺͹���)
        CT_GetServerAddr,
        ST_GetServerAddr,

        // �����ػ汾�̲��ϴ������˷�������
        CT_UploadBook,
        ST_UploadBook,

        // ��˽�˷������������ء�
        CT_DownloadFromPersonal,
        ST_DownloadFromPersonal,

        // �ӹ����������������ء�
        CT_DownloadFromPublic,
        ST_DownloadFromPublic,

        // ָ�����䡣
        CT_TransferBook,
        ST_TransferBook,

        // ������
        CT_Publish,
        ST_Publish,
/*}}}*/

        // �滭/*{{{*/
        // ͬ���滭���á�
        CT_UpdateDrawSet,
        ST_UpdateDrawSet,

        // ͬ���滭��Ϣ��
        CT_UpdateDrawInfo,
        ST_UpdateDrawInfo,

        // ���������б�
        ST_UpdateGroupStatus, // ֻ��Ҫgroup_id��student_id

        // ͬ��ֽ�š�
        CT_UpdateBackground,
        ST_UpdateBackground,

        // ͬ����ġͼƬ���滭��
        CT_UpdateCopyIntoPaint,
        ST_UpdateCopyIntoPaint,

        // ͬ�����ʡ���Ƥ��
        CT_UpdatePenAnderaser,
        ST_UpdatePenAnderaser,

        // ͬ����ɫ��
        CT_UpdateFilling,
        ST_UpdateFilling,

        // ͬ��ӡ�� TODO
        CT_UpdateStamp,
        ST_UpdateStamp,

        // ͬ����ͼ
        CT_UpdateFillPic,
        ST_UpdateFillPic,

        // ͬ��������
        CT_UpdateMutex,
        ST_UpdateMutex,

        // ͬ��������
        CT_UpdateCollage,
        ST_UpdateCollage,

        // ͬ������
        CT_UpdateWord,
        ST_UpdateWord,

        // ͬ���߿�
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
        BT_BOOK,        // �鱾�̲�
        BT_DRAW,        // ͼ���̲�
        BT_LINE,        // ���߽̲�
        BT_PULLZE,      // ƴͼ�̲�
        BT_MIDI,        // �����̲�
};

// �����������õ������ݽṹ��
//

///////////////////

typedef struct sSubmitData {
	unsigned int cLen;          // ���ݳ���
	enum CommandType cType;     // ���ݱ��
	void* cData() {             // ָ�����ݵ�ָ��
		return this + 1;
	}
} MSG_HEAD;

#define MSG_HEAD_LEN    sizeof(struct sSubmitData)

#pragma pack()

#endif //_PROTOCOL_H
