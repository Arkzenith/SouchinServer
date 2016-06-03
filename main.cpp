#include <iostream>

#include "SCServer.h"

using namespace std;


int main(int argc,char const ** argv) {


    SCServer *server =  new SCServer();

    server->setAcception(new ThreadAcception());

    server->start();

    return 0;
}