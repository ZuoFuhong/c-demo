// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <hiredis/hiredis.h>
#include <stdio.h>
#include <string.h>

// 安装依赖：brew install hiredis
void operate_redis() {
    redisContext *ctx = redisConnect("127.0.0.1", 6379);
    if (ctx == NULL || ctx->err) {
        if (ctx) {
            printf("Error: %s\n", ctx->errstr);
            // handle error
        } else {
            printf("Can't allocate redis context\n");
        }
    }
    // 1.AUTH
    const char *authCmd = "AUTH 123456";
    redisReply *reply = redisCommand(ctx, authCmd);
    if (reply == NULL) {
        printf("command execute failure\n");
        redisFree(ctx);
        return;
    }
    if (!(reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0)) {
        printf("command execute failure:%s\n", authCmd);
        freeReplyObject(reply);
        redisFree(ctx);
        return;
    }
    freeReplyObject(reply);
    printf("AUTH success\n");

    // 2.SET val
    const char *setCmd = "SET name dazuo";
    reply = redisCommand(ctx, setCmd);
    if (reply == NULL) {
        printf("command execute failure\n");
        redisFree(ctx);
        return;
    }
    if (!(reply->type == REDIS_REPLY_STATUS && strcmp(reply->str, "OK") == 0)) {
        printf("command execute failure:%s\n", setCmd);
        freeReplyObject(reply);
        redisFree(ctx);
        return;
    }
    freeReplyObject(reply);
    printf("%s execute success\n", setCmd);

    // 3.GET val
    const char *getCmd = "GET name";
    reply = (struct redisReply *)redisCommand(ctx, getCmd);
    if (reply->type != REDIS_REPLY_STRING) {
        printf("command execute failure:%s\n", getCmd);
        freeReplyObject(reply);
        redisFree(ctx);
        return;
    }
    printf("GET val: %s\n", reply->str);
    freeReplyObject(reply);
    redisFree(ctx);
}
