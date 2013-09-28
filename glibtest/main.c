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
gint conncmp(gconstpointer data1,gconstpointer data2)
{
    return ((txr_conn_p)data1)->sockfd - ((txr_conn_p)data2)->sockfd;
}
int main(int argc,char* argv[])
{
#define LEN_OF_CONNLIST 10
    GArray *connarr=g_array_sized_new(FALSE, TRUE, sizeof(txr_conn_t),LEN_OF_CONNLIST);
    txr_conn_p conn = NULL;
    for (int i=0; i<LEN_OF_CONNLIST; i++) {
        conn = &g_array_index(connarr, txr_conn_t, i);
        conn->sockfd=rand() % 100;
    }
    for (int i=0; i<LEN_OF_CONNLIST; i++) {
        conn = &g_array_index(connarr, txr_conn_t, i);
        printf("%d\n",conn->sockfd);
    }
    g_array_sort(connarr, (GCompareFunc)conncmp);
    for (int i=0; i<LEN_OF_CONNLIST; i++) {
        conn = &g_array_index(connarr, txr_conn_t, i);
        printf("%d\n",conn->sockfd);
    }
    return 0;
}


