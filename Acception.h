//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_ACCEPTION_H
#define SOUCHINSERVER_ACCEPTION_H
#include <unistd.h>
#include <thread>

#include "SCServer.h"
#include "Procession.h"
#include "ParamenterPtr.h"

// 套接字 接受类接口
class Acception {
protected:
    int sd = 0;
    Procession *procession = nullptr;

public:
    void setSd(int sd) {
        Acception::sd = sd;
    }

    void setProcession(Procession *procession) {
        Acception::procession = procession;
    }

    Acception(Procession *procession) : procession(procession) { }

    Acception() { }

public:
    void doProcess(struct ParamenterPtr *ptr) {
        if (!procession) {
            printf("没事设置连接处理器: Procession, 使用默认设置:MessageProcession\n");
            procession = new MessageProcessioin();
        }
        printf("连接处理器得到套接字描述符: %d \n", *(ptr->conn));
        procession->doProcess(ptr);
    }

public:
    virtual void doAccept() = 0;
};


// 多线程 套接字 接受类  默认使用
class ThreadAcception : public Acception {

public:
    virtual void doAccept() override {

        while (1) {
            struct ParamenterPtr *paraPtr = (struct ParamenterPtr*)malloc(sizeof(struct ParamenterPtr));
            bzero(paraPtr, sizeof(struct ParamenterPtr));
            int conn = accept(Acception::sd,paraPtr->addr,paraPtr->socklen);
            paraPtr->conn = (int*) malloc(sizeof(int));
            *(paraPtr->conn)=conn;
            std::thread(&Acception::doProcess,this,paraPtr).detach();
        }
    }

};


#endif //SOUCHINSERVER_ACCEPTION_H
