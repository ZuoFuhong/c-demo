// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include "zhelper.h"
#include <stdio.h>

void zmq_req_client() {
    printf("Connecting to hello world server...\n");
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://127.0.0.1:5555");

    char message[] = "hello server";
    zmq_send(socket, message, strlen(message), 0);

    char *str = s_recv(socket);
    printf("Received: %s, strlen: %lu\n", str, strlen(str));

    zmq_close(socket);
    zmq_ctx_destroy(context);
}
