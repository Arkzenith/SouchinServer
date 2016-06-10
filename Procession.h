//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_PROCESSION_H
#define SOUCHINSERVER_PROCESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Acception.h"

class Procession {

protected:
    int conn;

protected:
    Procession() { }

public:
    Procession(int conn) : conn(conn) { }

    virtual ~Procession() {
        close(this->conn);
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
class MessageProcessioin : public Procession {


public:
    MessageProcessioin(int conn) : Procession(conn) { }

    MessageProcessioin() : Procession() { }

public:
    virtual void doProcess() override {

        char buff[512];
        bzero(buff, 512);
        recv(this->conn, buff, 512, 0);
        printf("接受到数据: %s \n", buff);
        char str[128];
        sprintf(str, "<<来自服务器测试用例[ 套接字描述符: %d ]的消息>>: 数据已收到! \n", conn);
        send(this->conn, str, strlen(str), 0);
        this->release();
    }
};


#endif //SOUCHINSERVER_PROCESSION_H
