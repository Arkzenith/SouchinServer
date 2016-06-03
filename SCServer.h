//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_SCSERVER_H
#define SOUCHINSERVER_SCSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>

#include "Acception.h"

#define SERVER_PORT 8334

class SCServer {

private:
    int socketd = 0 ;
    int port = 0;
    struct sockaddr_in serverAddress;
    Acception *acception = nullptr;


public:
    void setAcception(Acception *acception) {
        SCServer::acception = acception;
        acception->setSd(this->socketd);
    }

public:
    SCServer() {
        this->port=SERVER_PORT;
        this->initialize();
    }


    virtual ~SCServer() {
        close(this->socketd);
        printf("关闭服务器, bye!");
    }

    void initialize() {
        printf("SouChinServer 开始初始化 .... \n");
        socketd = socket(AF_INET,SOCK_STREAM,0);
        bzero(&serverAddress, sizeof(struct sockaddr_in));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(this->port);
        serverAddress.sin_addr.s_addr = htonl(0);
        if (-1 ==(bind(socketd,(struct sockaddr*)&serverAddress, sizeof(struct sockaddr)))) {
            printf("端口 : %d 已经被占用, \n",this->port);
        } else{
            printf("服务器设置的监听端口是: %d \n",this->port);
        }
        listen(socketd,127);
    }
    void start() {
        if (!acception) {
            printf("没有处理接入连接的策略,使用默认的<< 多线程 >>策略 \n");
            acception = new ThreadAcception();
        }
        acception->setSd(this->socketd);
        acception->doAccept();
    }

};


#endif //SOUCHINSERVER_SCSERVER_H
