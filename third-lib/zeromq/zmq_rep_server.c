// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include "zhelper.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

void zmq_rep_server() {
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(socket, "tcp://127.0.0.1:5555");
    assert(rc == 0);

    while(1){
        char *str = s_recv(socket);
        printf("Received: %s, strlen: %lu\n", str, strlen(str));
        sleep(1);

        char message[] = "hello client";
        zmq_send(socket, message, strlen(message), 0);
    }
}
