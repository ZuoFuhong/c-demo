// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <zmq.h>
#include <strings.h>

// 从ZMQ套接字中接收字符串，并转换为C语言的字符串（末尾补\0）
static char *s_recv (void *socket) {
    enum {
        cap = 256
    };
    char buffer [cap];
    int size = zmq_recv(socket, buffer, cap - 1, 0);
    if (size == -1) {
        return NULL;
    }
    buffer[size < cap ? size : cap - 1] = '\0';
    return strdup(buffer);
}
