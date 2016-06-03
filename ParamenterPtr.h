//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_PARAMENTERPTR_H
#define SOUCHINSERVER_PARAMENTERPTR_H

#include <sys/socket.h>
#include <unistd.h>

struct ParamenterPtr {
    socklen_t *socklen=NULL;
    int *conn = NULL;
    struct sockaddr *addr=NULL;
};

#endif //SOUCHINSERVER_PARAMENTERPTR_H
