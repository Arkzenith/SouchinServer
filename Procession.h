//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_PROCESSION_H
#define SOUCHINSERVER_PROCESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Acception.h"
#include "ParamenterPtr.h"

class Procession {
protected:
    struct ParamenterPtr *paramenterPtr;

public:
    void setParamenterPtr(ParamenterPtr *paramenterPtr) {
        Procession::paramenterPtr = paramenterPtr;
    }

protected:
    Procession() { }

public:
    Procession(ParamenterPtr *paramenterPtr) : paramenterPtr(paramenterPtr) { }

    virtual ~Procession() {
        if (paramenterPtr) {
            printf("销毁 Procession\n");
            close(*(paramenterPtr->conn));
            free(paramenterPtr->addr);
            free(paramenterPtr->socklen);
            free(paramenterPtr->conn);
        }
    }

public:
    static void Destory(Procession *procession) {
        delete procession;
    }

public:
    virtual void doProcess() = 0;
};


// 信息反射处理类  默认使用
class MessageProcessioin : public Procession {


public:
    MessageProcessioin(ParamenterPtr *paramenterPtr) : Procession(paramenterPtr) { }

    MessageProcessioin() : Procession() { }

public:
    virtual void doProcess() override {
        int conn = *(paramenterPtr->conn);
        char buff[512];
        bzero(buff, 512);
        recv(conn, buff, 512, 0);
        printf("接受到数据: %s \n", buff);
        char str[128];
        sprintf(str, "<<来自服务器测试用例[ 套接字描述符: %d ]的消息>>: 数据已收到! \n", conn);
        send(conn, str, strlen(str), 0);
        Procession::Destory(this);
    }
};


#endif //SOUCHINSERVER_PROCESSION_H
