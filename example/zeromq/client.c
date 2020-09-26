// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <zmq.h>
#include <stdio.h>

int zeromq_client() {
    printf ("Connecting to hello world server...\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://127.0.0.1:5555");

    printf ("Sending Hello\n");
    zmq_send (requester, "Hello", 5, 0);

    char buffer [10];
    zmq_recv (requester, buffer, 10, 0);
    printf ("Received World\n");

    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
