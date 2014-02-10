#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <map>

#include "../protocol.h"
#include "../Sock.h"
#include "../message/proto/protocol.pb.h"
#define SERV_IP "222.186.50.76"
#define SERV_PORT 9999 

#define MAX_BUFF_SIZE   8192
#define BUFCLR(buf)    (void)memset(buf, 0x00, sizeof(buf))
#define MEMCPYSTRTOBUF(buf) \
    (void) memcpy (buffer+MSG_HEAD_LEN+sizeof(int), g_tmp_str.c_str(), g_tmp_str.size())

#define SETHEADCLEN(type) \
    MSG_HEAD_LEN + sizeof (int) + sizeof (type)

using namespace std;

struct numberProto
{
    int number;
    string protocol;
};

struct numberProto NPARRAY [] =
{   
    {0, "Exit/Quit" },
    {1, "CT_Login" },
    {2, "CT_Logout" },
    {3, "CT_GetFuncList" },
    {4, "CT_SetFunc" },
    {5, "CT_GetGradeList" },
    {6, "CT_GetCourseList" },
    {7, "CT_GetClassList" },
    {8, "CT_GetClassRoomList" },
    {9, "CT_SetContent" },
	{10, "CT_GetContent" },
    {11, "CT_StartClass" },
 	{12, "CT_Courseware" },
    {13, "CT_ClassOver" },
    {14,"CT_GetStudentList" },
    {15,"CT_GetStudentInfo" },
    {16,"CT_GetTeacherInfo" },
    {17,"CT_GetActiveStudentList" },
    {18,"CT_UploadBook" },
    {19,"CT_GetPersonalBooksList" },
    {20,"CT_GetPublicBooksList" },
    {21,"CT_GetServerAddr" },
    {22,"CT_DownloadFromPersonal" },
    {23,"CT_DownloadFromPublic" },
    {24,"CT_TransferBook" },
    {25,"CT_Publish" },
	{26,"CT_UpdateBackground" },
	{27,"CT_UpdateCopyIntoPaint" },
	{28,"CT_UpdatePenAnderaser" },
	{29,"CT_UpdateFilling" },
	{30,"CT_UpdateStamp" },
	{31,"CT_UpdateFillPic" },
	{32,"CT_UpdateMutex" },
	{33,"CT_UpdateCollage" },
	{34,"CT_UpdateWord" },
	{35,"CT_SetGroup" },
	{36,"CT_UpdateDrawInfo" },
    {37,"Print_Message_List" },
};

#define ARRAY_SIZE  (sizeof(NPARRAY)/sizeof(NPARRAY[0]))

typedef map<int, string> PROTOCOL_MAP;
typedef vector<string> PROTOCOL_VEC;

int sfd;
char buf[MAX_BUFF_SIZE];
char* buffer = buf;
MSG_HEAD* head = NULL;
string g_tmp_str;
    
#if 0
void recv_data (int sfd, char** buffer)
{
    int i = recv_v (sfd, (*buffer), sizeof (int));
    if (sizeof (int) != i) {
        cout << "recv head error! actually received len = " << i
            <<", info = "<< strerror (errno)<<endl;
        close (sfd);
    }
    int *p =  (int*) (*buffer);
    i = recv_v (sfd,  (*buffer) + sizeof(int), *p-sizeof(int));

    if ((*p - sizeof(int)) != (int)i) {
        cout <<"recv body error! hope = "<< *p <<", actually received len = "<< i 
            <<", info = "<< strerror (errno) <<endl;
        close (sfd);
    }
}

void dumpHead (string proto, char* buffer)
{
    cout << "-------------------" << endl;
    cout << proto  << ":" << endl;
    cout << "message head.cLen=" << ((MSG_HEAD*) buffer)->cLen << endl;
    cout << "message head.cType=" << ((MSG_HEAD*) buffer)->cType << endl;
}

void dumpTail (char* buffer)
{
    cout << "-------------------" << endl;
}

void dumpLogin (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strLogin = buffer + MSG_HEAD_LEN + sizeof(int);

    sLogin sl;
    sl.ParseFromString (strLogin);

    if (sl.result())
        cout << "result of Server: true" << endl;
    else
        cout << "result of Server: false" << endl;

    dumpTail (buffer);  
}

void dumpGetContent (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strGetContent = buffer + MSG_HEAD_LEN + sizeof(int);
	sGetContent gct;
	gct.ParseFromString (strGetContent);
	if (gct.result())
        cout << "result of Server: true" << endl;
    else
        cout << "result of Server: false" << endl;
	cout << gct.course_list() <<endl;
    dumpTail (buffer);  
}

void dumpStartClass (string proto, char* buffer)
{
	dumpHead (proto, buffer);
}

void dumpUpdateDrawInfo (string proto, char* buffer)
{
	dumpHead (proto, buffer);
}

void dumpClassOver (string proto, char* buffer)
{
	dumpHead (proto, buffer);
}


void dumpGetFuncList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetFuncList;
   
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    int i = 0;

    //for (; i<count; i++) {
        sGetFuncList fl;
         strGetFuncList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetFuncList);
        //strGetFuncList = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int) + i*sizeof(sGetFuncList);
        fl.ParseFromString (strGetFuncList);
        //
        cout << "grade(" << i << ") id=" << fl.id () << endl;
        cout << "grade(" << i << ") name=" << fl.name () << endl;
        cout << "grade(" << i << ") res_path=" << fl.res_path () << endl;
    //}
    dumpTail (buffer);
}

void dumpSetGroup (string proto, char *buffer)
{
	dumpHead (proto, buffer);
	string strSetGroup = buffer + MSG_HEAD_LEN + sizeof(int);
	sSetGroup sg;
	sg.ParseFromString (strSetGroup);
	if (sg.result())
        cout << "result of Server: true" << endl;
    else
        cout << "result of Server: false" << endl;
	cout << sg.msg() <<endl;
    dumpTail (buffer);  
}

void dumpSetFunc (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strSetFunc;

    sSetFunc sf;
    strSetFunc = buffer + MSG_HEAD_LEN + sizeof(int);
    sf.ParseFromString (strSetFunc);

    //
    cout << "function result=" << sf.result () << endl;
    dumpTail (buffer);
}

void dumpGetGradeList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetGradeList;
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    int i = 0;

    for (; i < count; i++) {
        sGetGradeList ggl;
        strGetGradeList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetGradeList);
        ggl.ParseFromString (strGetGradeList);
        //
        cout << "grade(" << i << ") id=" << ggl.grade_id () << endl;
        cout << "grade(" << i << ") name=" << ggl.grade_name () << endl;
    }

    dumpTail (buffer);
}

void dumpCourseware (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	cout << "hello student " << endl;
	dumpTail (buffer);
}
void dumpGetCourseList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetCourseList;
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    int i = 0;

    for (; i < count; i++) {
        sGetCourseList gcl;
        strGetCourseList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetCourseList);
        gcl.ParseFromString (strGetCourseList);
        //
        cout << "course(" << i << ") id=" << gcl.course_id() << endl;
        cout << "course(" << i << ") name=" << gcl.course_name() << endl;
        cout << "course(" << i << ") language=" << gcl.language() << endl;
        cout << "course(" << i << ") art=" << gcl.art() << endl;
        cout << "course(" << i << ") comunity=" << gcl.community() << endl;
        cout << "course(" << i << ") health=" << gcl.health() << endl;
        cout << "course(" << i << ") science=" << gcl.science() << endl;
        cout << "course(" << i << ") res_path=" << gcl.res_path() << endl;
    }
    dumpTail (buffer);
}

void dumpGetClassRoomList (string proto, char* buffer)
{    
    dumpHead (proto, buffer);
    string strGetClassRoomList;
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    int i = 0;

    for (; i < count; i++) {
        sGetClassRoomList crl;
        strGetClassRoomList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetClassRoomList);
        crl.ParseFromString (strGetClassRoomList);
        //
        cout << "ClassRoom(" << i << ") id=" << crl.classroom_id() << endl;
        cout << "ClassRoom(" << i << ") name=" << crl.classroom_name() << endl;
    }
    dumpTail (buffer);
}

void dumpSetContent (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strSetContent;

    sSetContent sc;
    strSetContent = buffer + MSG_HEAD_LEN + sizeof(int);
    sc.ParseFromString (strSetContent);

    //
    cout << "function result=" << sc.result () << endl;
    cout << "function message=" << sc.msg() << endl;
 
    
    dumpTail (buffer);
}
void dumpGetServerAddr(string proto, char* buffer)
{
    // ST_GetServerAddr
    dumpHead (proto, buffer);
    string strGetServerAddr;

    sGetServerAddr sa;
    strGetServerAddr = buffer + MSG_HEAD_LEN + sizeof(int);
    sa.ParseFromString (strGetServerAddr);

    //
    cout << "server addr=" << sa.serv_addr() << endl;
 
    dumpTail (buffer);
}

void dumpDownloadFromPersonal(string proto, char* buffer)
{
    // ST_DownloadFromPersonal
    dumpHead (proto, buffer);
    string strDownloadFromPersonal;

    sDownloadFromPersonal dfp;
    strDownloadFromPersonal = buffer + MSG_HEAD_LEN + sizeof(int);
    dfp.ParseFromString (strDownloadFromPersonal);

    //
    cout << "personal down addr=" << dfp.down_addr() << endl;
 
    dumpTail (buffer);
}

void dumpDownloadFromPublic(string proto, char* buffer)
{
    // ST_DownloadFromPublic
    dumpHead (proto, buffer);
    string strDownloadFromPublic;

    sDownloadFromPublic fp;
    strDownloadFromPublic= buffer + MSG_HEAD_LEN + sizeof(int);
    fp.ParseFromString (strDownloadFromPublic);

    //
    cout << "public down addr=" << fp.down_addr() << endl;
 
    dumpTail (buffer);
}

void dumpTransferBook(string proto, char* buffer)
{
    // ST_TransferBook
    dumpHead (proto, buffer);
    string strTransferBook;

    sTransferBook tb;
    strTransferBook = buffer + MSG_HEAD_LEN + sizeof(int);
    tb.ParseFromString (strTransferBook);

    //
    cout << "transfer.result =" << tb.result () << endl;
    cout << "transfer.message=" << tb.msg() << endl;
    
    dumpTail (buffer);
}

void dumpPublish(string proto, char* buffer)
{
    // ST_Publish
    dumpHead (proto, buffer);
    string strPublish;

    sPublish p;
    strPublish = buffer + MSG_HEAD_LEN + sizeof(int);
    p.ParseFromString (strPublish);

    //
    cout << "publish.result =" << p.result () << endl;
    cout << "publish.message=" << p.msg() << endl;
 
    dumpTail (buffer);
}

void dumpUploadBook (string proto, char* buffer)
{
    // ST_UploadBook
    dumpHead (proto, buffer);
    string strUploadBook;

    sUploadBook sc;
    strUploadBook = buffer + MSG_HEAD_LEN + sizeof(int);
    sc.ParseFromString (strUploadBook);

    //
    cout << "upload.result =" << sc.result () << endl;
    cout << "upload.message=" << sc.msg() << endl;
 
    
    dumpTail (buffer);
}
void dumpGetClassList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetClassList;
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    int i = 0;

    for (; i < count; i++) {
        sGetClassList gcl;
        strGetClassList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetClassList);
        gcl.ParseFromString (strGetClassList);
        //
        cout << "class(" << i << ") id=" << gcl.class_id() << endl;
        cout << "class(" << i << ") name=" << gcl.class_name() << endl;
    }
    dumpTail (buffer);
}

void dumpGetStudentList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetStudentList;
#if 0
    int count = 0;
    if ((((MSG_HEAD*) buffer)->cLen) == 8) {
    }dd
#endif
    int count = *(int*) (buffer + MSG_HEAD_LEN);
    printf ("count=%d\n", count);
    int i = 0;

    for (; i < count; i++) {
        sGetStudentList gsl;
        strGetStudentList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetStudentList);
        gsl.ParseFromString (strGetStudentList);
        //
        cout << "Student(" << i << ") id=" << gsl.id() << endl;
        cout << "Student(" << i << ") name=" << gsl.name() << endl;
        cout << "Student(" << i << ") res_path=" << gsl.res_path() << endl;
    }
    dumpTail (buffer);
}

void dumpGetActiveStudentList (string proto, char* buffer)
{
	 dumpHead (proto, buffer);
	 string strGetActiveStudentList;
	 int count = *(int*) (buffer + MSG_HEAD_LEN);
	 int i = 0;
	 	
	 for(; i < count; i++) {
		  //sGetActiveStudentList gasl;
		  //strGetActiveStudentList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetActiveStudentList);
		  //gasl.ParseFromString (strGetActiveStudentList);
		  //
		  //cout << "ActiveStudent(" << i << ") id=" << gasl.id() << endl;
		  cout << "ActiveStudent(" << i << ") id="
                << *(int*) (buffer + MSG_HEAD_LEN + sizeof(int) + i*sizeof(int)) << endl;
	 }
	 dumpTail (buffer);
}

void dumpGetPublicBooksList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetPublicBooksList;

	 int count = *(int*) (buffer + MSG_HEAD_LEN);
	 int i = 0;
	 	
	 for(; i < count; i++) {
		  sGetPublicBooksList gasl;
		  strGetPublicBooksList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetPublicBooksList);
		  gasl.ParseFromString (strGetPublicBooksList);
		  //
		  cout << "book (" << i << ") id=" << gasl.book_id() << endl;
		  cout << "book (" << i << ") name=" << gasl.book_name() << endl;
		  cout << "book (" << i << ") type=" << gasl.book_type() << endl;
		  cout << "book (" << i << ") resPath=" << gasl.res_path() << endl;
	 }
     dumpTail (buffer);
}

void dumpGetPersonalBooksList (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetPersonalBooksList;

	 int count = *(int*) (buffer + MSG_HEAD_LEN);
	 int i = 0;
	 	
	 for(; i < count; i++) {
		  sGetPersonalBooksList gasl;
		  strGetPersonalBooksList = buffer + MSG_HEAD_LEN + sizeof (int) + i * sizeof (sGetPersonalBooksList);
		  gasl.ParseFromString (strGetPersonalBooksList);
		  //
		  cout << "book (" << i << ") id=" << gasl.book_id() << endl;
		  cout << "book (" << i << ") name=" << gasl.book_name() << endl;
		  cout << "book (" << i << ") type=" << gasl.book_type() << endl;
		  cout << "book (" << i << ") resPath=" << gasl.res_path() << endl;
	 }
     dumpTail (buffer);
}

void dumpGetStudentInfo (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetStudentInfo;
    sGetStudentInfo gsi;
    strGetStudentInfo = buffer + MSG_HEAD_LEN + sizeof(int);
    gsi.ParseFromString (strGetStudentInfo);

    cout << "student id=" << gsi.id() << endl;
    cout << "student number=" << gsi.number() << endl;
    cout << "student name=" << gsi.name() << endl;
    cout << "student sex=" << gsi.sex() << endl;
    cout << "student race=" << gsi.race() << endl;
    //cout << "student birthday=" << gsi.birthday() << endl;
    cout << "student native=" << gsi.native() << endl;
    cout << "student class_name=" << gsi.class_name() << endl;

    dumpTail (buffer);
}


void dumpGetTeacherInfo (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strGetTeacherInfo;
    sGetTeacherInfo gti;
    strGetTeacherInfo = buffer + MSG_HEAD_LEN + sizeof(int);
    gti.ParseFromString (strGetTeacherInfo);

    cout << "teacher id=" << gti.id() << endl;
    cout << "teacher number=" << gti.number() << endl;
    cout << "teacher name=" << gti.name() << endl;
    cout << "teacher sex=" << gti.sex() << endl;
    cout << "teacher race=" << gti.race() << endl;
    cout << "teacher birthday=" << gti.birthday() << endl;
    cout << "teacher native=" << gti.native() << endl;
    cout << "teacher school_name=" << gti.school_name() << endl;

    dumpTail (buffer);
}

void dumpUpdateBackground (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateBackground;
	sUpdateBackground dbg;
	strUpdateBackground = buffer + MSG_HEAD_LEN + sizeof(int);
	dbg.ParseFromString (strUpdateBackground);

	cout << "paper id= " << dbg.paper_id() <<endl;
	dumpTail (buffer);
}

void dumpUpdateCopyIntoPaint (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateCopyIntoPaint;
	sUpdateCopyIntoPaint dcip;
	strUpdateCopyIntoPaint = buffer + MSG_HEAD_LEN + sizeof(int);
	dcip.ParseFromString (strUpdateCopyIntoPaint);

	cout << "pic id= " << dcip.pic_id() <<endl;
	cout << "pic res= " << dcip.pic_res() <<endl;
	cout << "pic scale= " << dcip.pic_scale() <<endl;
	cout << "pic rotation= " << dcip.pic_rotation() <<endl;
	cout << "pic x= " << dcip.pt_x() <<endl;
	cout << "pic y= " << dcip.pt_y() <<endl;
	dumpTail (buffer);
}

void dumpUpdatePenAnderaser (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdatePenAnderaser;
	sUpdatePenAnderaser dpa;
	strUpdatePenAnderaser = buffer + MSG_HEAD_LEN + sizeof(int);
	dpa.ParseFromString (strUpdatePenAnderaser);

	cout << "pen color= " << dpa.pen_color() <<endl;
	cout << "pen size= " << dpa.pen_size() <<endl;
	cout << "pen trans= " << dpa.pen_trans() <<endl;
	cout << "paint type= " << dpa.paint_type() <<endl;
	cout << "pt x= " << dpa.pt_x() <<endl;
	cout << "pt y= " << dpa.pt_y() <<endl;
	cout << "prePt x= " << dpa.prept_x() <<endl;
	cout << "prePt y= " << dpa.prept_y() <<endl;

	dumpTail (buffer);
}

void dumpUpdateFilling (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateFilling;
	sUpdateFilling df;
	strUpdateFilling = buffer + MSG_HEAD_LEN + sizeof(int);
	df.ParseFromString (strUpdateFilling);

	cout << "pt list= " << df.pt_list() <<endl;
	cout << "trans= " << df.trans() <<endl;
	cout << "color= " << df.color() <<endl;

	dumpTail (buffer);
}

void dumpUpdateStamp (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateStamp;
	sUpdateStamp ds;
	strUpdateStamp = buffer + MSG_HEAD_LEN + sizeof(int);
	ds.ParseFromString (strUpdateStamp);

	cout << "pic id= " << ds.pic_id() <<endl;
	cout << "pt x= " << ds.pt_x() <<endl;
	cout << "pt y= " << ds.pt_y() <<endl;

	dumpTail (buffer);
}

void dumpUpdateFillPic (string proto, char* buffer) 
{
	dumpHead (proto, buffer);
	string strUpdateFillPic;
	sUpdateFillPic dfp;
	strUpdateFillPic = buffer + MSG_HEAD_LEN + sizeof(int);
	dfp.ParseFromString (strUpdateFillPic);

	cout << "pic id= " << dfp.pic_id() <<endl;
	cout << "pt x= " << dfp.pt_x() <<endl;
	cout << "pt y= " << dfp.pt_y() <<endl;

	dumpTail (buffer);
}

void dumpUpdateMutex (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateMutex;
	sUpdateMutex dm;
	strUpdateMutex = buffer + MSG_HEAD_LEN + sizeof(int);
	dm.ParseFromString (strUpdateMutex);

	cout << "result= " << dm.result() <<endl;
	dumpTail (buffer);
}

void dumpUpdateCollage (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateCollage;
	sUpdateCollage dc;
	strUpdateCollage = buffer + MSG_HEAD_LEN + sizeof(int);
	dc.ParseFromString (strUpdateCollage);

	cout << "id= " << dc.id() <<endl;
	cout << "pic id= " << dc.pic_id() <<endl;
	cout << "pic type= " << dc.pic_type() <<endl;
	cout << "pic scale= " << dc.pic_scale() <<endl;
	cout << "pic rotation= " << dc.pic_rotation() <<endl;
	cout << "pt x= " << dc.pt_x() <<endl;
	cout << "pt y= " << dc.pt_y() <<endl;
	cout << "layer = " << dc.layer() <<endl;

	dumpTail (buffer);
}

void dumpUpdateWord (string proto, char* buffer)
{
	dumpHead (proto, buffer);
	string strUpdateWord;
	cUpdateWord dw;
	strUpdateWord = buffer + MSG_HEAD_LEN + sizeof(int);
	dw.ParseFromString (strUpdateWord);

	cout << "id= " << dw.id() <<endl;
	cout << "frame id= " << dw.frame_id() <<endl;
	cout << "msg= " << dw.msg() <<endl;
	cout << "front= " << dw.front() <<endl;
	cout << "color= " << dw.color() <<endl;
	cout << "size= " << dw.size() <<endl;
	cout << "effect id= " << dw.effect_id() <<endl;
	cout << "frame x = " << dw.frame_x() <<endl;
	cout << "frame y =" << dw.frame_y() << endl;

	dumpTail (buffer);
}


void handlerLogin()
{
    string client_account, client_passwd;
    int client_type;
    ///// CT_Login
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_Login;
    head->cLen = MSG_HEAD_LEN + sizeof (cLogin) + sizeof (int);


    cLogin login;
    cout << "ClientType:";
    cin >> client_type;
    cout << "Account:";
    cin >> client_account;
    cout << "Password:";
    cin >> client_passwd;
    login.set_type(client_type);
    login.set_account(client_account);
    login.set_passwd (client_passwd);

    g_tmp_str.clear();
    login.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);

#if 0
    // ST_Login
    recv_data (sfd, &buffer);
    dumpLogin ("ST_Login", buffer);
#endif
}

void handlerStartClass ()
{
	BUFCLR(buffer);
	head = (MSG_HEAD*) buffer;
	head->cType = CT_StartClass;
	head->cLen = MSG_HEAD_LEN ;
	send_v (sfd, buffer, head->cLen);
	BUFCLR(buffer);
}

void handlerUpdateDrawInfo ()
{
	BUFCLR(buffer);
	head = (MSG_HEAD*) buffer;
	head->cType = CT_UpdateDrawInfo;
	head->cLen = MSG_HEAD_LEN ;
	send_v (sfd, buffer, head->cLen);
	BUFCLR(buffer);
}

void handlerClassOver()
{
	BUFCLR(buffer);
	head = (MSG_HEAD*) buffer;
	head->cType = CT_ClassOver;
	head->cLen = MSG_HEAD_LEN ;
	send_v (sfd, buffer, head->cLen);
	BUFCLR(buffer);
}

void handlerGetContent()
{
	BUFCLR(buffer);
	head = (MSG_HEAD*) buffer;
	head->cType = CT_GetContent;
	head->cLen = MSG_HEAD_LEN ;
	send_v (sfd, buffer, head->cLen);
	BUFCLR(buffer);
}

void handlerCourseware()
{
	BUFCLR(buffer);
	head = (MSG_HEAD*) buffer;
	head->cType = CT_Courseware;
	head->cLen = MSG_HEAD_LEN ;
	send_v (sfd, buffer, head->cLen);
	BUFCLR(buffer);
}
void handlerLogout()
{
    ///// CT_Logout
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_Logout;
    head->cLen = MSG_HEAD_LEN;

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);

    exit (1);
}

void handlerGetFuncList()
{
    ///// CT_GetFuncList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetFuncList;
    head->cLen = MSG_HEAD_LEN;

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0    
    // ST_GetFuncList
    recv_data (sfd, &buffer);
    dumpGetFuncList ("ST_GetFuncList", buffer);
#endif
}

void handlerSetFunc()
{
    int func_type;
    ///// CT_SetFunc
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_SetFunc;
    head->cLen = SETHEADCLEN (cSetFunc);

    cSetFunc sf;
    cout << "function type:";
    cin >> func_type;
    sf.set_func_type(func_type);

    g_tmp_str.clear();
    sf.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);
    send_v (sfd, buffer, head->cLen);
    BUFCLR (buffer);
#if 0
    // ST_SetFunc
    recv_data (sfd, &buffer);
    dumpSetFunc ("ST_SetFunc", buffer);
#endif
}

void handlerGetGradeList()
{
    ///// CT_GetGradeList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetGradeList;
    head->cLen = MSG_HEAD_LEN;

    send_v (sfd, buffer, head->cLen);
    BUFCLR (buffer);
#if 0    
    // ST_GetGradeList
    recv_data (sfd, &buffer);
    dumpGetGradeList ("ST_GetGradeList", buffer);
#endif
}

void handlerGetCourseList()
{
    int grade_id;
    ///// CT_GetCourseList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetCourseList;
    head->cLen = MSG_HEAD_LEN + sizeof (cGetCourseList);

    cGetCourseList cl;
    cout << "grade id:";
    cin >> grade_id;
    cl.set_grade_id(grade_id);

    g_tmp_str.clear();
    cl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0
    // ST_GetCourseList
    recv_data (sfd, &buffer);
    dumpGetCourseList ("ST_GetCourseList", buffer);
#endif
}

void handlerGetClassList()
{
    int grade_id;
    ///// CT_GetClassList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetClassList;
    head->cLen = MSG_HEAD_LEN + sizeof (cGetClassList);


    cGetClassList cgcl;
    cout << "grade id:";
    cin >> grade_id;
    cgcl.set_grade_id (grade_id);

    cgcl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF (buffer);
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0   
    // ST_GetClassList
    recv_data (sfd, &buffer);
    dumpGetClassList ("ST_GetClassList", buffer);
#endif
}

void handlerGetClassRoomList()
{
    ///// CT_GetClassRoomList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetClassRoomList;
    head->cLen = MSG_HEAD_LEN;

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);

#if 0 
    // ST_GetClassRoomList
    recv_data (sfd, &buffer);
    dumpGetClassRoomList ("ST_GetClassRoomList", buffer);
#endif
}

void handlerSetContent ()
{
    int class_id, classroom_id;
    string course_list;
    //// CT_SetContent
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_SetContent;
    head->cLen = MSG_HEAD_LEN + sizeof (int) + sizeof (cSetContent);
   

    cSetContent sc;
    cout << "class id:"; 
    cin >> class_id;
    cout << "classroom id:"; 
    cin >> classroom_id;
    cout << "course list:"; 
    cin >> course_list;
    sc.set_class_id (class_id);
    sc.set_classroom_id (classroom_id);
    sc.set_course_list(course_list);

    g_tmp_str.clear();
    sc.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0 
    // ST_SetContent
    recv_data (sfd, &buffer);
    dumpSetContent ("ST_SetContent", buffer);
#endif
}

void handlerGetStudentList()
{
    int class_id;
    // CT_GetStudentList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetStudentList;
    head->cLen = SETHEADCLEN (cGetStudentList);
   
    cout << "class id:";
    cin >> class_id;
 
    cGetStudentList sl;
    sl.set_class_id (class_id);
    //g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0
    // ST_GetStudentList
    recv_data (sfd, &buffer);
    dumpGetStudentList ("ST_GetStudentList", buffer);
#endif
}

void handlerGetStudentInfo()
{
    int student_id;
    ///// CT_GetStudentInfo
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetStudentInfo;
    head->cLen = SETHEADCLEN(CT_GetStudentInfo);
   
    cout << "student id:";
    cin >> student_id; 
    cGetStudentInfo si;
    si.set_id (student_id);

    g_tmp_str.clear();
    si.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0 
    // ST_GetStudentInfo
    recv_data (sfd, &buffer);
    dumpGetStudentInfo ("ST_GetStudentInfo", buffer);
#endif
}

void handlerGetTeacherInfo()
{
    int teacher_id;
    //// CT_GetTeacherInfo
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetTeacherInfo;
    head->cLen = SETHEADCLEN(CT_GetTeacherInfo);
   
    cout << "teacher id:";
    cin >> teacher_id; 
    cGetTeacherInfo ti;
    ti.set_id (teacher_id);

    g_tmp_str.clear();
    ti.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0 
    // ST_GetTeacherInfo
    recv_data (sfd, &buffer);
    dumpGetTeacherInfo ("ST_GetTeacherInfo", buffer);
#endif
}

void handlerUploadBook()
{   
    int class_id;
    string book_name;
    int  book_type  ;
    string time     ;
    int  belongs    ; // 所属班，用二进制占位表示。
    int  art        ;
    int  language   ;
    int  community  ;
    int  health     ;
    int  science    ;
    string res_path ;

    //// CT_UploadBook
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UploadBook;
    head->cLen = SETHEADCLEN(cUploadBook);
   
    //cout << "class id:";
    //cin >> class_id; 
    cUploadBook ub;
    ub.set_book_name ("绘刘师兄5");
    ub.set_book_type (2);
    ub.set_time ("2014/1/1");
    ub.set_belongs (7);
    ub.set_art (13);
    ub.set_language (14);
    ub.set_community (15);
    ub.set_health (16);
    ub.set_science (17);
    ub.set_res_path ("..../res_path");

    g_tmp_str.clear();
    ub.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerGetPublicBooksList()
{   
    //// CT_GetPublicBooksList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetPublicBooksList;
    head->cLen = MSG_HEAD_LEN; //SETHEADCLEN(cGetPersonalBooksList);
   
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerGetPersonalBooksList()
{   
    //// CT_GetPersonalBooksList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetPersonalBooksList;
    head->cLen = MSG_HEAD_LEN; //SETHEADCLEN(cGetPersonalBooksList);
   
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerGetServerAddr()
{
    //// CT_GetServerAddr
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetServerAddr;
    head->cLen = MSG_HEAD_LEN;
   
    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerDownloadFromPersonal()
{
    int book_id;
    //// CT_DownloadFromPersonal
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_DownloadFromPersonal;
    head->cLen = SETHEADCLEN(cDownloadFromPersonal);
   
    cout << "book id:";
    cin >> book_id; 
    cDownloadFromPersonal dfp;
    dfp.set_book_id (book_id);

    g_tmp_str.clear();
    dfp.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerDownlaodFromPublic()
{
    int book_id;
    //// CT_DownloadFromPublic
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_DownloadFromPublic;
    head->cLen = SETHEADCLEN(cDownloadFromPublic);
   
    cout << "book id:";
    cin >> book_id; 
    cDownloadFromPublic fp;
    fp.set_book_id (book_id);

    g_tmp_str.clear();
    fp.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerTransferBook()
{
    int book_id;
    string account;
    //// CT_TransferBook
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_TransferBook;
    head->cLen = SETHEADCLEN(cTransferBook);
   
    cout << "book id:";
    cin >> book_id; 
    cout << "account:";
    cin >> account; 
    cTransferBook tb;
    tb.set_book_id (book_id);
    tb.set_account (account);

    g_tmp_str.clear();
    tb.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerPublish()
{
    int book_id;
    //// CT_Publish
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_Publish;
    head->cLen = SETHEADCLEN(cPublish);
   
    cout << "book id:";
    cin >> book_id; 
    cPublish p;
    p.set_book_id (book_id);

    g_tmp_str.clear();
    p.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerGetActiveStudentList()
{   
    int class_id;
    //// CT_GetActiveStudentList
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_GetActiveStudentList;
    head->cLen = SETHEADCLEN(cGetActiveStudentList);
   
    cout << "class id:";
    cin >> class_id; 
    cGetActiveStudentList sl;
    sl.set_class_id (class_id);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
#if 0 
    // ST_GetActiveStudentList
    recv_data (sfd, &buffer);
    dumpGetActiveStudentList ("ST_GetActiveStudentList", buffer);
#endif
}

void handlerUpdateBackground ()
{
    int paper_id;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateBackground;
    head->cLen = SETHEADCLEN(cUpdateBackground);
   
    cout << "paper id:";
    cin >> paper_id; 
    cUpdateBackground sl;
    sl.set_paper_id(paper_id);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateCopyIntoPaint ()
{
    int pic_id;
	string pic_res;
	string pic_scale;
	string pic_rotation;
	string pt_x;
	string pt_y;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateCopyIntoPaint;
    head->cLen = SETHEADCLEN(cUpdateCopyIntoPaint);
   
    cout << "pic id:";
    cin >> pic_id; 
	cout << "pic res:";
	cin >> pic_res;
	cout << "pic scale:";
	cin >> pic_scale;
	cout << "pic rotation:";
	cin >> pic_rotation;
	cout <<"pt x:";
	cin >> pt_x;
	cout << "pt y:";
	cin >> pt_y;
    cUpdateCopyIntoPaint sl;
	sl.set_pic_id (pic_id);
	sl.set_pic_res (pic_res);
	sl.set_pic_scale (pic_scale);
	sl.set_pic_rotation (pic_rotation);
	sl.set_pt_x (pt_x);
	sl.set_pt_y (pt_y);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdatePenAnderaser ()
{
	int pen_type;
	int pen_color;
	string pen_size;
	int pen_trans;
	int paint_type;
	string pt_x;
	string pt_y;
	string prePt_x;
	string prePt_y;

    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdatePenAnderaser;
    head->cLen = SETHEADCLEN(cUpdatePenAnderaser);

    cout << "pen_type:";
    cin >> pen_type; 
	cout << "pen color:";
	cin >> pen_color;
	cout << "pen size:";
	cin >> pen_size;
	cout << "pen trans:";
	cin >> pen_trans;
	cout << "paint_type:";
	cin >> paint_type;
	cout <<"pt x:";
	cin >> pt_x;
	cout << "pt y:";
	cin >> pt_y;
	cout << "prePt x:";
	cin >> prePt_x;
	cout << "prePt y:";
	cin >> prePt_y;
    cUpdatePenAnderaser sl;
	sl.set_pen_type (pen_type);
	sl.set_pen_color (pen_color);
	sl.set_pen_size (pen_size);
	sl.set_pen_trans (pen_trans);
	sl.set_paint_type(paint_type);
	sl.set_pt_x (pt_x);
	sl.set_pt_y (pt_y);
	sl.set_prept_x(prePt_x);
	sl.set_prept_y(prePt_y);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateFilling ()
{
	string pt_list;
	int trans;
	int color;

    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateFilling;
    head->cLen = SETHEADCLEN(cUpdateFilling);
   
    cout << "pt list:";
    cin >> pt_list; 
	cout << "trans:";
	cin >> trans;
	cout << "color:";
	cin >> color;
    cUpdateFilling sl;
    sl.set_pt_list (pt_list);
	sl.set_trans (trans);
	sl.set_color (color);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateStamp ()
{
	int pic_id;
	string pt_x;
	string pt_y;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateStamp;
    head->cLen = SETHEADCLEN(cUpdateStamp);
   
	cout << "pic Id:";
	cin >> pic_id;
	cout <<"pt x:";
	cin >> pt_x;
	cout << "pt y:";
	cin >> pt_y;
    cUpdateStamp sl;
	sl.set_pic_id (pic_id);
	sl.set_pt_x (pt_x);
	sl.set_pt_y (pt_y);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateFillPic ()
{
	int pic_id;
	string pt_x;
	string pt_y;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateFillPic;
    head->cLen = SETHEADCLEN(cUpdateFillPic);
   
	cout << "pic Id:";
	cin >> pic_id;
	cout <<"pt x:";
	cin >> pt_x;
	cout << "pt y:";
	cin >> pt_y;
    cUpdateFillPic sl;
	sl.set_pic_id (pic_id);
	sl.set_pt_x (pt_x);
	sl.set_pt_y (pt_y);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateMutex ()
{
	int lock;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateMutex;
    head->cLen = SETHEADCLEN(cUpdateMutex);
   
	cout << "lock:";
	cin >> lock;
    cUpdateMutex sl;
	sl.set_lock (lock);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerSetGroup ()
{
	int group_id;
	string group_name;
	int student_cnt;
	string student_list;

	BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_SetGroup;
    head->cLen = SETHEADCLEN(cSetGroup);
   
	cout << "group id:";
	cin >> group_id;
	cout << "group name:";
	cin >> group_name;
	cout << "student cnt:";
	cin >> student_cnt;
	cout << "student list:";
	cin >> student_list;
    cSetGroup sl;
	sl.set_group_id (group_id);
	sl.set_group_name (group_name);
	sl.set_student_cnt (student_cnt);
	sl.set_student_list (student_list);
    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);

}

void handlerUpdateCollage ()
{
	int id;
	int pic_id;
	int pic_type;
	string pic_scale;
	string pic_rotation;
	string pt_x;
	string pt_y;
	int layer;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateCollage;
    head->cLen = SETHEADCLEN(cUpdateCollage);
   
	cout << "id:";
	cin >> id;
	cout << "pic id:";
    cin >> pic_id; 
	cout << "pic type:";
	cin >> pic_type;
	cout << "pic scale:";
	cin >> pic_scale;
	cout << "pic rotation:";
	cin >> pic_rotation;
	cout <<"pt x:";
	cin >> pt_x;
	cout << "pt y:";
	cin >> pt_y;
	cout << "layer:";
	cin >> layer;
    cUpdateCollage sl;
	sl.set_id(id);
	sl.set_pic_id (pic_id);
	sl.set_pic_type (pic_type);
	sl.set_pic_scale (pic_scale);
	sl.set_pic_rotation (pic_rotation);
	sl.set_pt_x (pt_x);
	sl.set_pt_y (pt_y);
	sl.set_layer(layer);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void handlerUpdateWord ()
{
	int id;
	int frame_id;
	string msg;
	int front;
	int color;
	int size;
	int effect_id;
	string frame_x;
	string frame_y;
    BUFCLR(buffer);
    head = (MSG_HEAD*) buffer;
    head->cType = CT_UpdateWord;
    head->cLen = SETHEADCLEN(cUpdateWord);
   
	cout << "id:";
	cin >> id;
	cout << "frame id:";
    cin >> frame_id; 
	cout << "msg:";
	cin >> msg;
	cout << "front:";
	cin >> front;
	cout << "color:";
	cin >> color;
	cout <<"size:";
	cin >> size;
	cout << "effect_id:";
	cin >> effect_id;
	cout << "frame_x:";
	cin >> frame_x;
	cout << "frame_y:";
	cin >> frame_y;
    cUpdateWord sl;
	sl.set_id(id);
	sl.set_frame_id (frame_id);
	sl.set_msg (msg);
	sl.set_front (front);
	sl.set_color(color);
	sl.set_size (size);
	sl.set_effect_id (effect_id);
	sl.set_frame_x(frame_x);
	sl.set_frame_y(frame_y);

    g_tmp_str.clear();
    sl.SerializeToString (&g_tmp_str);
    MEMCPYSTRTOBUF(buffer);

    send_v (sfd, buffer, head->cLen);
    BUFCLR(buffer);
}

void printArray (struct numberProto* array, size_t size)
{
    int loop = 0;
    int idx  = 0;
    cout << "==============================================\n";
    for (; idx < size; idx++) {
        string strProtocol = array[idx].protocol + " ]";
        printf ("[%2d -> %-35s", idx, strProtocol.c_str());
        if (++loop == 3) {
            cout << endl;
            loop = 0;
        }
    }
    cout << "\n==============================================\n";


}

void dumpLogout (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strLogout;
    sLogout l;
    strLogout= buffer + MSG_HEAD_LEN + sizeof(int);
    l.ParseFromString (strLogout);

    cout << "logout logintype=" << l.login_type() << endl;
    cout << "logout client number=" << l.id() << endl;

    dumpTail (buffer);
}

void dumpUpdateStudentStatus (string proto, char* buffer)
{
    dumpHead (proto, buffer);
    string strUpdateStudentStatus;
    sUpdateStudentStatus l;
    strUpdateStudentStatus = buffer + MSG_HEAD_LEN + sizeof(int);
    l.ParseFromString (strUpdateStudentStatus);

    cout << "student id=" << l.student_id () << endl;

    dumpTail (buffer);
}

void* recv_callback (void* data)
{
    while (1) {
        // ST_Logout
        recv_data (sfd, &buffer);
        switch (((MSG_HEAD*) buffer)->cType) {
            case ST_Login:
                dumpLogin ("ST_Login", buffer);
                break;
            case ST_UpdateStudentStatus:
                dumpUpdateStudentStatus ("ST_UpdateStudentStatus", buffer);
                break;
            case ST_Logout:
                dumpLogout("ST_Logout", buffer);
                break;
            case ST_GetFuncList:
                dumpGetFuncList("ST_GetFuncList", buffer);
                break;
            case ST_SetFunc:
                dumpSetFunc("ST_SetFunc", buffer);
                break;
            case ST_GetGradeList:
                dumpGetGradeList("ST_GetGradeList", buffer);
                break;
            case ST_GetCourseList:
                dumpGetCourseList("ST_GetCourseList", buffer);
                break;
            case ST_GetClassList:
                dumpGetClassList("ST_GetClassList", buffer);
                break;
            case ST_GetClassRoomList:
                dumpGetClassRoomList("ST_GetClassRoomList", buffer);
                break;
            case ST_SetContent:
                dumpSetContent("ST_SetContent", buffer);
                break;
            case ST_GetStudentList:
                dumpGetStudentList("ST_GetStudentList", buffer);
                break;
            case ST_GetStudentInfo:
                dumpGetStudentInfo("ST_GetStudentInfo", buffer);
                break;
            case ST_GetTeacherInfo:
                dumpGetTeacherInfo("ST_GetTeacherInfo", buffer);
                break;
            case ST_GetActiveStudentList:
                dumpGetActiveStudentList("ST_GetActiveStudentList", buffer);
                break;
            case ST_GetPersonalBooksList:
                dumpGetPersonalBooksList("ST_GetPersonalBooksList", buffer);
                break;
            case ST_GetPublicBooksList:
                dumpGetPublicBooksList("ST_GetPublicBooksList", buffer);
                break;
            case ST_GetServerAddr:
                dumpGetServerAddr("ST_GetServerAddr", buffer);
                break;
            case ST_DownloadFromPersonal:
                dumpDownloadFromPersonal("ST_DownloadFromPersonal", buffer);
                break;
            case ST_DownloadFromPublic:
                dumpDownloadFromPublic("ST_DownloadFromPublic", buffer);
                break;
            case ST_TransferBook:
                dumpTransferBook("ST_TransferBook", buffer);
                break;
            case ST_Publish:
                dumpPublish("ST_Publish", buffer);
                break;
            case ST_UploadBook:
                dumpUploadBook ("ST_UploadBook", buffer);
                break;
			case ST_UpdateBackground:
				dumpUpdateBackground ("ST_UpdateBackground", buffer);
				break;
			case ST_UpdateCopyIntoPaint:
				dumpUpdateCopyIntoPaint ("ST_UpdateCopyIntoPaint", buffer);
				break;
			case ST_UpdatePenAnderaser:
				dumpUpdatePenAnderaser ("ST_UpdatePenAnderaser", buffer);
				break;
			case ST_UpdateFilling:
				dumpUpdateFilling ("ST_UpdateFilling", buffer);
				break;
			case ST_UpdateStamp:
				dumpUpdateStamp ("ST_UpdateStamp", buffer);
				break;
			case ST_UpdateFillPic:
				dumpUpdateFillPic ("ST_UpdateFillPic", buffer);
				break;
			case ST_UpdateMutex:
				dumpUpdateMutex ("ST_UpdateMutex", buffer);
				break;
			case ST_UpdateCollage:
				dumpUpdateCollage ("ST_UpdateCollage", buffer);
				break;
			case ST_UpdateWord:
				dumpUpdateWord ("ST_UpdateWord", buffer);
				break;
			case ST_Courseware:
				dumpCourseware ("ST_Courseware", buffer);
				break;
			case ST_GetContent:
				dumpGetContent ("ST_GetContent", buffer);
				break;
			case ST_StartClass:
				dumpStartClass ("ST_StartClass", buffer);
				break;
			case ST_ClassOver:
				dumpClassOver ("ST_ClassOver", buffer);
				break;
			case ST_SetGroup:
				dumpSetGroup ("ST_Group", buffer);
				break;
			case ST_UpdateDrawInfo:
				dumpUpdateDrawInfo ("ST_UpdateDrawInfo", buffer);
				break;
        }
        
        //cout << "Input Message Id: ";
        usleep (100);
    }
}

#endif

int main (int argc, char** argv)
{
    int strMessageId;

    pthread_t pid_recv = 0;

#if 1
    if ((sfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror ("socket: ");
        return (-1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr (SERV_IP);
    addr.sin_port = htons (SERV_PORT);

    if (connect (sfd, (struct sockaddr*) &addr, sizeof (struct sockaddr)) < 0) {
        perror ("connect: ");
        return (-1);
    }

    getchar ();
#endif

#if 1
    //pthread_create(&pid_recv, NULL, recv_callback, NULL);

    //pthread_join  (pid_update, NULL);
    //pthread_join  (pid_logout, NULL);
#endif

#if 0
    printArray (NPARRAY, ARRAY_SIZE);
    cout << "Input Message Id: ";
    while (1) {
        cin >> strMessageId;

        if (NPARRAY[strMessageId].protocol == "Print_Message_List") {
            printArray (NPARRAY, ARRAY_SIZE);
        }
        else if (NPARRAY[strMessageId].protocol == "CT_Login") {
            handlerLogin();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_Logout") {
            handlerLogout();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetFuncList") {
            handlerGetFuncList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_SetFunc") {
            handlerSetFunc();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetGradeList") {
            handlerGetGradeList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetCourseList") {
            handlerGetCourseList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetClassList") {
            handlerGetClassList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetClassRoomList") {
            handlerGetClassRoomList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_SetContent") {
            handlerSetContent();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetStudentList") {
            handlerGetStudentList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetStudentInfo") {
            handlerGetStudentInfo();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetTeacherInfo") {
            handlerGetTeacherInfo();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetActiveStudentList") {
            handlerGetActiveStudentList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_UploadBook") {
            handlerUploadBook();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetPersonalBooksList") {
            handlerGetPersonalBooksList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetPublicBooksList") {
            handlerGetPublicBooksList();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_GetServerAddr") {
            handlerGetServerAddr();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_DownloadFromPersonal") {
            handlerDownloadFromPersonal();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_DownloadFromPublic") {
            handlerDownlaodFromPublic();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_TransferBook") {
            handlerTransferBook();
        }
        else if (NPARRAY[strMessageId].protocol == "CT_Publish") {
            handlerPublish();
        }
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateBackground") {
			handlerUpdateBackground();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateCopyIntoPaint") {

			handlerUpdateCopyIntoPaint();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdatePenAnderaser") {
			handlerUpdatePenAnderaser();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateFilling") {
			handlerUpdateFilling();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateStamp") {
			handlerUpdateStamp();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateFillPic") {
			handlerUpdateFillPic();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateMutex") {
			handlerUpdateMutex();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateCollage") {
			handlerUpdateCollage();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateWord") {
            handlerUpdateWord();
        }

		else if (NPARRAY[strMessageId].protocol == "CT_Courseware") {
			handlerCourseware();
		}
			
		else if (NPARRAY[strMessageId].protocol == "CT_GetContent") {
			handlerGetContent();
		}

		else if (NPARRAY[strMessageId].protocol == "CT_StartClass") {
			handlerStartClass();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_ClassOver") {
			handlerClassOver();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_SetGroup") {
			handlerSetGroup();
		}
		else if (NPARRAY[strMessageId].protocol == "CT_UpdateDrawInfo") {
			handlerUpdateDrawInfo();
		}

        else if (NPARRAY[strMessageId].protocol == "Exit/Quit") {
            cout << "Bye." << endl;
            break;
        }
		
        else {
            cout << "[Error]: can't find handler." << endl;
        }
    }
#endif    
    return (0);
}
