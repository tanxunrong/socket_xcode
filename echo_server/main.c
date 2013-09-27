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
    char rbuf[MAXLINE];
    char wbuf[MAXLINE];
} txr_conn_t;
typedef txr_conn_t * txr_conn_p;
static int listen_fd;
static GMainContext *liscontext;
static GMainLoop *lisloop;
static GSource *lissource;
static GList *connlist=NULL;

void display_list(GList *ll)
{
    GList *it=NULL;
    for (it = ll; it != NULL;it = it->next) {
        txr_conn_p conn=(txr_conn_p)(it->data);
        printf("%d\t",conn->sockfd);
    }
    printf("\n");
}

gboolean accept_cb(gpointer *data)
{
    GList *connlist=(GList *)data;
    int sockfd=accept(listen_fd,NULL,NULL);
    if (sockfd < 0)
        return FALSE;
    txr_conn_p newconn = (txr_conn_p)g_slice_alloc(sizeof(txr_conn_t));
    memset(newconn,0,sizeof(txr_conn_t));
    newconn->sockfd=sockfd;
    
    connlist = g_list_prepend(connlist, (gpointer)newconn);
    if (connlist == NULL)
        return FALSE;
    display_list(connlist);
//    g_print("new fd %d ,list count %d \n",sockfd,g_list_length(connlist));
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
    connlist = g_list_alloc();
    g_source_set_callback(lissource, (GSourceFunc)accept_cb, connlist, NULL);
    g_source_attach(lissource, liscontext);
    g_main_loop_run(lisloop);
    g_main_loop_unref(lisloop);
    
    return 0;
}

