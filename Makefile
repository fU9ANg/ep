#
# makefile
#

# set src, dst directory and flags for protobuf
PROTOC      = protoc
PBFLAGS     = --cpp_out
PBSRC       = ./proto
PBDST       = ./message

# which c plus plus compiler
CPP     = ccache g++

# which c compiler
CC      = ccache cc

# options for compiler
CFLAGS  = -Wall -Werror

# flags of buildhouse
BHFLAGS	= \
	  -D__DEBUG_HANDLE_HEAD_ \
	  -D__DEBUG_DUMP__ \
	  -D__DEBUG__

#-D_BUILD_HOUSE_GMAE

# is debug? (for development)
CDEBUG  = -g -DDEBUG

# macro
CMACRO	= 
#-D_OLD_MAKEHOUSE_GAME


# objects
OBJS    = 	main.o \
		Config.o \
		SendTask.o \
		database.o \
		Evloop.o \
		Sock.o \
		Single.o \
		ThreadPool.o \
		RecvTask.o \
		AuthTask.o \
		ProcessManager.o \
		\
		aes/aes_cbc.o \
		aes/aes_core.o \
		aes/AESEncrypt.o \
		aes/cbc128.o \
		\
		content/LoginCheck.o \
		content/epManager.o \
		content/epSchool.o \
		content/epClassroom.o \
		content/epClass.o \
		content/epGroup.o \
		content/epPerson.o \
		content/epUser.o \
		content/epTeacher.o \
		content/epStudent.o \
		content/epWhiteBoard.o \
		content/epParents.o \
		content/epHeadmaster.o \
		\
		handler/HandleMessage.o \
		handler/HandleSystem.o \
		handler/HandleLogin.o \
		handler/HandleLogout.o \
		handler/HandleSetFunc.o \
		handler/HandleSetContent.o \
		handler/HandleGetClassList.o \
		handler/HandleGetGradeList.o \
		handler/HandleGetCourseList.o \
		handler/HandleGetStudentList.o \
		handler/HandleGetClassRoomList.o \
		handler/HandleGetStudentInfo.o \
		handler/HandleGetFuncList.o \
		handler/HandleGetTeacherInfo.o \
		handler/HandleStartClass.o \
		handler/HandleGetActiveStudentList.o \
		handler/HandleGetContent.o \
		handler/HandleSetGroup.o \
		handler/HandleGetPersonalBooksList.o \
		handler/HandleGetPublicBooksList.o \
		handler/HandleGetServerAddr.o \
		handler/HandleUploadBook.o \
		handler/HandleDownloadFromPersonal.o \
		handler/HandleDownloadFromPublic.o \
		handler/HandleTransferBook.o \
		handler/HandlePublish.o \
        handler/HandleUpdateMutex.o \
        handler/HandleUpdateGroupDrawMsg.o \
		\
		message/proto/protocol.pb.o

RMOBJS	= cscope.* \
	  tags

# binary
BIN	    = epServer

# where are include files kept
INC	    = -I. -I./includes -I./handler -I./content -I./message

# for Linker
LINK        = libs/libev.a libs/libglog.a libs/liblua52.so libs/libmysqlcppconn.so -lprotobuf
#LINK        = -lev -lglog -lmysqlcppconn -llua5.2
# rock..
all	: clean precompile_protobuf $(BIN)

precompile_protobuf	:
	$(PROTOC) $(PBFLAGS)=$(PBDST) $(PBSRC)/*

# how to compiling programs
$(BIN):$(OBJS)
#	$(CPP) $(CFLAGS) $(CDEBUG) -o $@ $(OBJS)
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) -o $@ $(OBJS) $(LINK)
%.o:%.cpp
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<
%.o:%.cc
	$(CPP) $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<
%.o:%.c
	$(CC)  $(CFLAGS) $(CDEBUG) $(BHFLAGS) $(CMACRO) $(INC) -o $@ -c $<

.PHONY: clean

clean:
	-rm -rf $(OBJS) $(RMOBJS) $(BIN) *~ logs .lock
