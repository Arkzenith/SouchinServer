//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_ACCEPTION_H
#define SOUCHINSERVER_ACCEPTION_H

#include <unistd.h>
#include <thread>
//#include <sys/event.h>

#ifdef __LINUX__

#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <error.h>

#define MAXEVENTS 1024

#elif __UNIX__

#include <sys/event.h>
#include <sys/types.h>
#include <sys/time.h>

#define MAX_KEVENT 32
#endif

#include "SCServer.h"
#include "Procession.h"

// 套接字 接受类接口
class Acception {
protected:
    int sd = 0;

public:
    virtual void setSd(int sd) {
        Acception::sd = sd;
    }

    Acception() { }

public:
    void doProcess(int conn) {
        Procession *procession = new MessageProcessioin(conn);
        procession->doProcess();
    }

public:
    virtual void doAccept() = 0;
};


// 多线程 套接字 接受类  默认使用
class ThreadAcception : public Acception {

public:
    virtual void doAccept() override {
        printf("启动 多线程 模式 \n");
        while (1) {
            int conn = accept(Acception::sd, NULL, NULL);
            std::thread(&Acception::doProcess, this, conn).detach();
        }
    }
};

#ifdef __UNIX__

class KqueueAcception : public Acception {
private:
    int kq;

public:
    virtual void setSd(int sd) override {
        Acception::setSd(sd);
        if (0 != this->addEvent(Acception::sd, EVFILT_READ)) {
            printf("kevent error!\n ");
        }
    }

    KqueueAcception() : Acception() {
        printf("启动 Kqueue 模式 \n");
        this->kq = kqueue();
        printf("kqueue 描述符: %d \n", this->kq);
    }

public:
    virtual void doAccept() override {

        waitEvent();
    }

private:
    int addEvent(int socketd, int actions) {
        struct kevent ev[1];
        EV_SET(&ev[0], socketd, actions, EV_ADD, 0, 0, NULL);
        return kevent(this->kq, ev, 1, NULL, 0, NULL);
    }

private:
    void waitEvent() {
        struct kevent evs[MAX_KEVENT];
        while (1) {
            int nevs = kevent(this->kq, NULL, 0, evs, MAX_KEVENT, 0);
            handleEvent(evs, nevs);
        }
    }

    void handleEvent(struct kevent *evs, int nevs) {
        for (int i = 0; i < nevs; i++) {
            if (evs[i].flags & EV_EOF) {
                close(evs[i].ident);
                continue;
            }
            if (evs[i].ident == Acception::sd) {
                for (int j = 0; j < evs[i].data; j++) {
                    int conn = accept(Acception::sd, NULL, NULL);
                    this->addEvent(conn, EVFILT_READ | EVFILT_WRITE);
                    printf("注册新的 socket 描述符 : %d\n", conn);
                }
            } else {
                if (evs[i].filter & EVFILT_READ) {
                    char *buff = (char *) malloc(sizeof(char) * evs[i].data);
                    bzero(buff, evs[i].data + 1);
                    int ret = recv(evs[i].ident, buff, evs[i].data, 0);
                    printf("收到的信息: %s \n", buff);
                    free(buff);
                }
            }
        }
    }
};

#endif

#ifdef __LINUX__

class EpollAcception : public Acception {

public:
    virtual void doAccept() override {
        printf("启动 epoll 模式 \n");
        int efd = epoll_create1(0);
        struct epoll_event ev, events[MAXEVENTS];
        bzero(&ev, sizeof(struct epoll_event));
        ev.events = EPOLLIN | EPOLLET;
        if (-1 == epoll_ctl(efd, EPOLL_CTL_ADD, Acception::sd, &ev)) {
            printf("EPOLL 发生错误[epoll_ctl],程序退出! socket descriptor %d \n", Acception::sd);
            exit(EXIT_FAILURE);
        }
        while (1) {
            int nfds = epoll_wait(efd, events, MAXEVENTS, -1);
            if (-1 == nfds) {
                printf("EPOLL 发生错误[epoll_wait],程序退出 !socket descriptor %d \n", Acception::sd);
                exit(EXIT_FAILURE);
            }
            if (nfds ==0) continue;
            for (int i = 0; i < nfds; i++) {
                if (events[i].data.fd == Acception::sd) {
                    while (1) {
                        ev.data.fd = accept(Acception::sd, NULL, NULL);
                        if (ev.data.fd > 0) {
                            fcntl(ev.data.fd,F_SETFL,fcntl(ev.data.fd,F_GETFL,0)|O_NONBLOCK);
                            ev.events = EPOLLIN | EPOLLET;
                            epoll_ctl(efd, EPOLL_CTL_ADD, ev.data.fd, &ev);
                        } else {
                            if (errno == EAGAIN) break;
                        }
                    }
                }
                else {
                    if (events[i].events & EPOLLIN) {
                        char buff[1024] = {0};
                        int ret = 999, rs = 1;
                        while (rs) {
                            ret = recv(events[i].data.fd, buff, 1024, 0);
                            if (ret < 0) {
                                if (errno == EAGAIN) break;
                                else {
                                    epoll_ctl(efd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                                    close(events[i].data.fd);
                                    break;
                                }
                            } else if (ret == 0) {
                                rs = 0;
                            }
                            if (ret == sizeof(buff)) {
                                rs = 1;
                            } else {
                                rs = 0;
                            }

                            printf("接受到数据: %s \n", buff);
                        }
                    }
                }
            }

        }
    }
};

#endif

#endif //SOUCHINSERVER_ACCEPTION_H
