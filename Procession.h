//
// Created by Ark Zenith on 16/6/3.
//

#ifndef SOUCHINSERVER_PROCESSION_H
#define SOUCHINSERVER_PROCESSION_H

#include "Acception.h"
#include "ParamenterPtr.h"

class Procession {
public:
    virtual void doProcess(struct ParamenterPtr *ptr)=0;
};

class MessageProcessioin : public Procession {

public:
    virtual void doProcess(struct ParamenterPtr *ptr) override {
        char buff[512];
        recv(*(ptr->conn),buff,512,0);
        printf("got message: %s", buff);
        close(*(ptr->conn));
    }
};


#endif //SOUCHINSERVER_PROCESSION_H
