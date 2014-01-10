#ifndef __GLOBAL_FUNCTIONS_H__
#define __GLOBAL_FUNCTIONS_H__

#include "protocol.h"
#include "Single.h"

namespace {

/*
template <typename T>
Buf* package(const enum CommandType msg_type, const T& obj) {
	MSG_HEAD header;
	header.cType = msg_type;
	header.cLen = sizeof(T) + MSG_HEAD_LEN + sizeof(int);

	Buf* pBuf = SINGLE->bufpool.malloc();
	pBuf->reset();
	memcpy(pBuf->ptr(), &header, sizeof(header));
	*(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = 1;
	memcpy(((char*)pBuf->ptr())+sizeof(header)+sizeof(int), &obj, sizeof(obj));

	pBuf->set_size(header.cLen);

	return pBuf;
}

template <typename T>
T& unpackage(Buf* pBuf, T&, int cnt = 0) {
	return *(T*) (((char*)pBuf->ptr()) + sizeof(MSG_HEAD) + sizeof(int) + sizeof(T)*cnt);
}

static int unpack_cnt(Buf* pBuf) {
	return *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN);
}

static Buf* combine_buf(Buf* dist, Buf** src) {
	if (NULL == dist && NULL == *src) {
		return NULL;
	}

	if (NULL == dist) {
		return *src;
	}

	if (NULL == *src) {
		return dist;
	}

	if (((MSG_HEAD*)dist->ptr())->cType != ((MSG_HEAD*)(*src)->ptr())->cType) {
		return NULL;
	} else {
		MSG_HEAD* dist_head = (MSG_HEAD*)dist->ptr();
		MSG_HEAD* src_head = (MSG_HEAD*)(*src)->ptr();

		*(int*)((char*)dist->ptr() + MSG_HEAD_LEN) = unpack_cnt(dist) + unpack_cnt(*src);
		memcpy((char*)dist->ptr()+dist_head->cLen \
				, (char*)(*src)->ptr()+MSG_HEAD_LEN+sizeof(int) \
				, src_head->cLen - MSG_HEAD_LEN - sizeof(int));

		dist_head->cLen = dist_head->cLen + src_head->cLen - MSG_HEAD_LEN - sizeof(int);
		SINGLE->bufpool.free(*src);
		*src = NULL;

		return dist;
	}
}
*/

/*
static int
unpacket_cnt(Buf* pBuf) {
	return *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN);
}
*/

template <typename T> T&
unpacket(Buf* pBuf, T& obj, int cnt = 0) {
	std::string recv_buf = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int) + sizeof(T)*cnt;
	obj.ParseFromString(recv_buf);
	return obj;
}

template <typename T> Buf*
packet(enum CommandType type, const T& obj, int fd) {
	Buf* pBuf = SINGLE->bufpool.malloc();

	MSG_HEAD* head = (MSG_HEAD*)pBuf->ptr();
	head->cType = type;

	head->cLen  = MSG_HEAD_LEN + sizeof(obj);
	*(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = 1;

	std::string send_buf;
	obj.SerializeToString(&send_buf);
	memcpy(((char*)pBuf->ptr())+MSG_HEAD_LEN+sizeof(int), send_buf.c_str(), send_buf.size());

	pBuf->setsize(head->cLen);
	pBuf->setfd(fd);

	return pBuf;
}

template <typename T> Buf*
packet(enum CommandType type, std::vector<T>& vc, int fd) {
	Buf* pBuf = SINGLE->bufpool.malloc();

	MSG_HEAD* head = (MSG_HEAD*)pBuf->ptr();
	head->cType = type;

        int cnt = vc.size();
	head->cLen = MSG_HEAD_LEN + sizeof(vc[0])*cnt;
	*(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = cnt;

	std::string send_buf;
        for (int i=0; i<cnt; ++i) {
                vc[i].SerializeToString(&send_buf);
                memcpy(((char*)pBuf->ptr())+MSG_HEAD_LEN+sizeof(int)+sizeof(vc[0])*i
                                , send_buf.c_str(), send_buf.size());
        }

        pBuf->setsize(head->cLen);
        pBuf->setfd(fd);

        return pBuf;
}

/*
static Buf*
packet(enum CommandType type, int fd) {
        Buf* pBuf = SINGLE->bufpool.malloc();

        MSG_HEAD* head = (MSG_HEAD*)pBuf->ptr();
        head->cType = type;

        head->cLen  = MSG_HEAD_LEN;
        *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = 0;

        pBuf->setsize(head->cLen);
        pBuf->setfd(fd);

        return pBuf;
}

static Buf*
combine_buf(Buf* dist, Buf** src) {
        if (NULL == dist && NULL == *src) {
                return NULL;
        }

        if (NULL == dist) {
                return *src;
        }

        if (NULL == *src) {
                return dist;
        }

        if (((MSG_HEAD*)dist->ptr())->cType != ((MSG_HEAD*)(*src)->ptr())->cType) {
                return NULL;
        } else {
                MSG_HEAD* dist_head = (MSG_HEAD*)dist->ptr();
                MSG_HEAD* src_head  = (MSG_HEAD*)(*src)->ptr();

                *(int*)((char*)dist->ptr() + MSG_HEAD_LEN) = unpacket_cnt(dist) + unpacket_cnt(*src);
                memcpy((char*)dist->ptr()+dist_head->cLen \
                                , (char*)(*src)->ptr()+MSG_HEAD_LEN+sizeof(int) \
                                , src_head->cLen - MSG_HEAD_LEN - sizeof(int));

                dist_head->cLen = dist_head->cLen + src_head->cLen - MSG_HEAD_LEN - sizeof(int);
                SINGLE->bufpool.free(*src);
                *src = NULL;

                return dist;
        }
}
*/

}; // end of namespace

#endif // __GLOBAL_FUNCTIONS_H__
