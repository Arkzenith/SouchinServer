//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_PROCESSION_H
#define SOUCHINSERVER_PROCESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <sys/event.h>

#include "Acception.h"

class Procession {

protected:
    int conn;
protected:
    struct kevent *evs;

protected:
    Procession() { }

public:
    Procession(int conn) : conn(conn) { }

    Procession(struct kevent *evs) : evs(evs) { }

    virtual ~Procession() {
        if (conn > 0) {
            close(this->conn);
        }
        if (evs) {
            close(evs->ident);
        }
    }

public:
    static void Destory(Procession *procession) {
        delete procession;
    }

public:
    void release() {
        Procession::Destory(this);
    }

public:
    virtual void doProcess() = 0;
};


// 信息反射处理类  默认使用
class MessageProcession : public Procession {


public:
    MessageProcession(int conn) : Procession(conn) { }

    MessageProcession() : Procession() { }

public:
    virtual void doProcess() override {

        char buff[1024];
        bzero(buff, 1024);
        recv(this->conn, buff, 1024, 0);
        printf("接受到数据: %s \n", buff);
        char str[128];
        sprintf(str, "<<来自服务器测试用例[ 套接字描述符: %d ]的消息>>: 数据已收到! \n", conn);
        send(this->conn, str, strlen(str), 0);
        this->release();
    }
};

class kqueueProcession : public Procession {

public:
    virtual void doProcess() override {

        if (evs->filter & EVFILT_READ) {
            char *buff = (char *) malloc(sizeof(char) * evs->data);
            bzero(buff, evs->data + 1);
            int ret = recv(evs->ident, buff, evs->data, 0);
            printf("收到的信息: %s \n", buff);
            char str[128];
            sprintf(str, "<<来自服务器测试用例[ 套接字描述符: %d ]的消息>>: 数据已收到! \n", evs->ident);
            send(this->evs->ident, str, strlen(str), 0);
            free(buff);
        }
        this->release();
    }

    kqueueProcession(struct kevent *evs) : Procession(evs) { }

public:
    kqueueProcession() : Procession() { }
};

#endif //SOUCHINSERVER_PROCESSION_H
