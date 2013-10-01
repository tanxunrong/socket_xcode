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
    sleep(2);
    printf("%d add\n",upnum);
    sum += upnum;
}
int main(int argc,char* argv[])
{
    GError *err;
    GThreadPool *sumpool = g_thread_pool_new(sumtohuge,NULL,10,TRUE,&err);
    int *sand = (int *)malloc(sizeof(int) * 100);
    int upnum = 0;
    for (int i=0; i<100; i++) {
        upnum = rand() % 100;
        memcpy(sand + i, &upnum, sizeof(int));
        g_thread_pool_push(sumpool, sand+i, &err);
        printf("%d \t",*(sand+i));
        assert(err == NULL);
    }
    printf("%d unprocessed\n",g_thread_pool_unprocessed(sumpool));
    g_thread_pool_free(sumpool, FALSE, TRUE);
    free(sand);
    printf("sum %d\n",sum);
    return 0;
}


