// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.
#include <sys/event.h>

// MacOS 不支持epoll，但有一个类似epoll的kqueue
// 官方文档：https://developer.apple.com/library/archive/documentation/Darwin/Conceptual/FSEvents_ProgGuide/KernelQueues/KernelQueues.html
void learn_kqueue() {
    // TODO: kqueue-example
    // https://github.com/yedf/kqueue-example/blob/master/main.cc
}