//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_ACCEPTION_H
#define SOUCHINSERVER_ACCEPTION_H
#include <unistd.h>
#include <thread>

#ifdef __LINUX__
#include <sys/epoll.h>

#endif

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
//        注意此方法存在内存泄漏, 需要在使用完ptr后释放
        if (procession == NULL) {
            printf("没事设置连接处理器: Procession, 使用默认设置:MessageProcession\n");
            procession = new MessageProcessioin(ptr);
        }
        printf("连接处理器得到套接字描述符: %d \n", *(ptr->conn));
        procession->doProcess();
//        必须重置指针!
        procession = nullptr;
    }

public:
    virtual void doAccept() = 0;
};


// 多线程 套接字 接受类  默认使用
class ThreadAcception : public Acception {

public:
    virtual void doAccept() override {
        printf("使用多线程模式 \n");
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

#ifdef __LINUX__

class EpollAcception : public Acception {

public:
    virtual void doAccept() override {
        printf("使用epoll 模式 \n");
    }
};

#endif

#endif //SOUCHINSERVER_ACCEPTION_H
