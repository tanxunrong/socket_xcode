//
//  main.c
//  glibtest
//
//  Created by tanxunrong on 13-9-27.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#include "../include/common.h"
#include <glib.h>

#define TXR_MALLOC(type) ((type *)malloc(sizeof(type)))

typedef struct txr_conn{
    int sockfd;
    char key[20];
    char buf[MAXLINE];
}txr_conn_t;
typedef txr_conn_t * txr_conn_p;
static int sum = 0;
void sumtohuge(gpointer data,gpointer userdata)
{
    int upnum = *(int *)data;
    printf("%d add\n",upnum);
    sum += upnum;
    int sltime = rand() % 10;
    sleep(sltime);
}
int main(int argc,char* argv[])
{
    GError *err;
    GThreadPool *sumpool = g_thread_pool_new(sumtohuge,NULL,10,TRUE,&err);
    assert(err == NULL);
//    for (int i=0; i<100; i++) {
//        int upnum = rand() % 100;
//        printf("upnum %d\t",upnum);
//        g_thread_pool_push(sumpool, &upnum, &err);
//        assert(err == NULL);
//    }
    for (int i=0; i<100; i++) {
        int upnum = rand() % 100;
        g_thread_pool_push(sumpool, &upnum, &err);
        assert(err == NULL);
    }
    printf("%d unprocessed\n",g_thread_pool_unprocessed(sumpool));
    g_thread_pool_free(sumpool, FALSE, TRUE);
    printf("sum %d\n",sum);
    return 0;
}


