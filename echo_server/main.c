//
//  main.c
//  echo_server
//
//  Created by tanxunrong on 13-9-26.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#include "../include/common.h"
#include <glib.h>

typedef struct txr_conn {
    int sockfd;
    gchar *rbuf;
    gsize rlen;
    GError *err;
} txr_conn_t;
typedef txr_conn_t * txr_conn_p;
static int listen_fd;
static GMainContext *liscontext;
static GMainLoop *lisloop;
static GSource *lissource;
static GList *connlist;
void display_list(GList *ll)
{
    GList *it=NULL;
    for (it = ll; it->next != NULL;it = ll->next) {
        txr_conn_p conn=(txr_conn_p)it->data;
        assert(conn->sockfd != 0);
        printf("%d\t",conn->sockfd);
    }
    printf("\n");
}

gboolean work_cb(GIOChannel *channel,GIOCondition condition,gpointer data)
{
    printf("iofunc called\n");
    GIOStatus ioret;
    txr_conn_p conn = (txr_conn_p)data;
    if ( condition == G_IO_IN) {
        ioret = g_io_channel_read_to_end(channel, &(conn->rbuf), &(conn->rlen), &(conn->err));
        printf("read %lu chars\n",conn->rlen);
        if (ioret == G_IO_STATUS_ERROR) {
            return FALSE;
        }
    }
    
    if ( condition == G_IO_OUT)
    {
       ioret = g_io_channel_write_chars(channel, conn->rbuf, conn->rlen, NULL, &(conn->err));
        printf("write %lu chars\n",conn->rlen);
        if (ioret == G_IO_STATUS_ERROR) {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean accept_cb(gpointer *data)
{
//    assert(data == NULL);
    int sockfd=accept(listen_fd,NULL,NULL);
    if (sockfd < 0)
        return FALSE;
    txr_conn_p newconn = (txr_conn_p)malloc(sizeof(txr_conn_t));
    memset(newconn,0,sizeof(txr_conn_t));
    newconn->sockfd=sockfd;
    
    connlist = g_list_append(connlist, (gpointer)newconn);
    assert(connlist != NULL);
    
    GIOChannel *newchan = g_io_channel_unix_new(sockfd);
    g_io_add_watch(newchan, G_IO_IN | G_IO_OUT, (GIOFunc)work_cb ,(gpointer)newconn);
    g_print("new fd %d \n",sockfd);
    return TRUE;
}

int main(int argc,char* argv[])
{
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(3789);
    if( inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr) <= 0)
        err_quit ("inet_pton");
    

    if ((listen_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        err_sys("create socket");
    
    //bind & listen
    if (bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
        err_sys("bind");
    if (listen(listen_fd,LISTEN_QUEUE) < 0)
        err_sys("listen");
    setfdnonblock(listen_fd);
    
    liscontext = g_main_context_new();
    lisloop = g_main_loop_new(liscontext, FALSE);

    GIOChannel *lischan=g_io_channel_unix_new(listen_fd);
    lissource=g_io_create_watch(lischan, G_IO_IN);

    g_source_set_callback(lissource, (GSourceFunc)accept_cb, NULL, NULL);
    g_source_attach(lissource, liscontext);
    g_main_loop_run(lisloop);
    g_main_loop_unref(lisloop);
    
    return 0;
}

