
/*
 * HandleMessage.h
 */

#ifndef _HANDLE_MESSAGE_H
#define _HANDLE_MESSAGE_H

#include <cstdio>
#include <cstdlib>
#include <list>
#include <map>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../protocol.h"
#include "../Single.h"
#include "../Buf.h"
#include "../SQL.h"

using namespace std;

#define _EXECUTE_

typedef void (*handlefunc)(Buf*);
typedef map<int, handlefunc> HANDLEMAP;

class CHandleMessage
{
public:

#define MSGNAME(name, handle) static void handle (Buf* p)
#       include "MSGHANDLE"
#undef MSGNAME

public:
        static handlefunc getHandler (int iCommandType)
        {
                HANDLEMAP::iterator it = CHandleMessage::m_HandleMap.find(iCommandType);
                if (it != CHandleMessage::m_HandleMap.end())
                        return it->second;

                return NULL;
        }

        static void initHandlers (void)
        {
#define MSGNAME(name, handle) \
                CHandleMessage::setHandler (name, &CHandleMessage::handle)
#include "MSGHANDLE"
#undef MSGNAME
                setHandler(CT_UpdateCopyIntoPaint, &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdatePenAnderaser,  &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateFilling,       &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateStamp,         &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateFillPic,       &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateCollage,       &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateWord,          &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateFrame,         &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdateWord,          &CHandleMessage::handleUpdateGroupDrawMsg);
                setHandler(CT_UpdatePuzzleResult,  &CHandleMessage::handleUpdateGroupDrawMsg);

                setHandler(CT_Reduce,              &CHandleMessage::handleTeacherSendtoWhiteboard);
                setHandler(CT_unDisplay,           &CHandleMessage::handleTeacherSendtoWhiteboard);

                setHandler(CT_unLock,              &CHandleMessage::handleSendtoAll);
        }

        static void setHandler (int iCommandType, handlefunc hHandler)
        {
                if (hHandler != NULL)
                        m_HandleMap.insert (std::pair<int, handlefunc>(iCommandType, hHandler));
        }

private:
        static bool postMessage (Buf* p, enum CommandType iCommandType, void* data, unsigned int iLen);

private:
        static HANDLEMAP m_HandleMap;

public:

};


#endif //_HANDLE_MESSAGE_H
