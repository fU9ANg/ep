#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../content/epGroup.h"

void CHandleMessage::handleSetGroup (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetSetGroup\n";
#endif

        // 该FD必须处于上课状态。
        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                SINGLE->bufpool.free(p);
                return;
        }

        bool result = true;
        std::string msg;
        cSetGroup sg;
        int cnt = unpacket_cnt(p);
        int i = 0;
        epGroup* pGroup = NULL;
        for (; i<cnt && result; ++i) {
                if (!unpacket(p, sg, i)) { // unpacket fail!!!
                        result = false;
                        msg = sg.group_id() + "组信息的数据包有问题。";
                        break;
                }

                pGroup = new epGroup();
                if (NULL == pGroup) {
                        result = false;
                        msg = "内存分配失败！";
                        break;
                }

                pGroup->setId(sg.group_id());
                pGroup->setName(sg.group_name());
                int student_cnt = sg.student_cnt();
                char* pc = const_cast<char*>(sg.student_list().c_str());
                epStudent* pStudent = NULL;
                for(int j=0; j<student_cnt; ++j) {
                        pStudent = const_cast<epStudent*>(pClassroom->getStudentById(*(int*)(pc+j)));
                        if (NULL != pStudent) {
                                pGroup->insertStudent(pStudent->getFd(), pStudent);
                        } else {
                                result = false;
                                msg = sg.group_id() + "组的" + *(int*)(pc+j) + std::string("号学生不在该上课的教室中。");
                                break;
                        }
                }

                if (result && !pClassroom->insertGroup(pGroup)) {
                        result = false;
                        msg = "将组加入教室时失败！";
                        break;
                }
        }

        if (i == cnt) {
                result = true;
                msg = "分组成功。";
        }

        if (!result) { // 分组失败，将之前可能的分组清除。
                pClassroom->deleteAllGroup();
        }

        sSetGroup tmp;
        tmp.set_result(result);
        tmp.set_msg(msg);

        Buf* pBuf = packet(ST_SetGroup, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
}
