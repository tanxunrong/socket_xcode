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
    char buf[MAXLINE];
}txr_conn_t;
typedef txr_conn_t * txr_conn_p;
void seq_del(gpointer *data)
{
    printf("seq element del\n");
}
void seq_foreach(gpointer data)
{
    txr_conn_p conn = (txr_conn_p)data;
    printf("%d\n",conn->sockfd);
}
static inline gint seq_comp(gconstpointer data1,gconstpointer data2,gpointer userdata)
{
    return ((txr_conn_p)data1)->sockfd -
        ((txr_conn_p)data2)->sockfd;
}
int main(int argc,char* argv[])
{
    GSequence *connlist=g_sequence_new((GDestroyNotify)seq_del);
#define LEN_OF_CONNLIST 10
    txr_conn_p conn[LEN_OF_CONNLIST];
    for (int i=0; i<LEN_OF_CONNLIST; i++) {
        conn[i]=(txr_conn_p)malloc(sizeof(txr_conn_t));
        memset(conn[i], 0, sizeof(txr_conn_t));
        conn[i]->sockfd=rand();
        g_sequence_append(connlist, conn[i]);
    }
    printf("length %d\n",g_sequence_get_length(connlist));
    g_sequence_foreach(connlist, (GFunc)seq_foreach, NULL);
    g_sequence_sort(connlist,seq_comp,NULL);
    g_sequence_foreach(connlist, (GFunc)seq_foreach, NULL);
    return 0;
}


