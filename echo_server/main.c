//
//  main.c
//  echo_server
//
//  Created by tanxunrong on 13-9-26.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#include "../include/common.h"
#include <glib.h>
#include <event2/event.h>

typedef struct txr_conn {
    evutil_socket_t  sockfd;
    size_t rlen;
    char rbuf[MAXLINE];
    struct event *event;
} txr_conn_t;
typedef txr_conn_t * txr_conn_p;
static evutil_socket_t listen_fd;
static struct event_base *mainbase;
static GHashTable *connlist;
static char *servermsg = "server msg";
gboolean connequal(gconstpointer a,gconstpointer b);

void callback_work(evutil_socket_t fd,short what,void *data)
{
    txr_conn_p conn = g_hash_table_lookup(connlist, &fd);
    assert(conn != NULL);
    if (what & EV_READ) {
        ssize_t n = read(fd, conn->rbuf, MAXLINE);
        conn->rlen = n;
        if ( n < 0 & errno != EAGAIN && errno !=EWOULDBLOCK) {
            goto FREE_EVENT;
        }
    }
    if (what & EV_WRITE) {
        ssize_t n = write(fd, servermsg, strlen(servermsg)+1);
        if ( n < 0 & errno != EAGAIN && errno !=EWOULDBLOCK) {
            goto FREE_EVENT;
        }
    }
FREE_EVENT:
    g_hash_table_remove(connlist, &fd);
    event_free(conn->event);
    free(conn);
    close(fd);
}

void callback_lis(evutil_socket_t fd,short what,void *data)
{
    assert(what & EV_READ);
    evutil_socket_t connfd=accept(fd, NULL, NULL);
    assert(connfd > 0);
    txr_conn_p newconn = (txr_conn_p)malloc(sizeof(txr_conn_t));
    memset(newconn, 0, sizeof(txr_conn_t));
    newconn->sockfd=connfd;
    g_hash_table_insert(connlist, &(newconn->sockfd), (gpointer)newconn);
    struct event *newconn_event=event_new(mainbase, connfd, EV_READ | EV_WRITE, (event_callback_fn)callback_work, NULL);
    newconn->event = newconn_event;
}

gboolean connequal(gconstpointer a,gconstpointer b)
{
    return ((txr_conn_p)a)->sockfd == ((txr_conn_p)b)->sockfd;
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
    
    connlist = g_hash_table_new(g_int_hash, (GEqualFunc)connequal);
    assert(connlist != NULL);
    
    mainbase = event_base_new();
    assert(mainbase != NULL);
    struct event *lisevent = event_new(mainbase, listen_fd, EV_READ, (event_callback_fn)callback_lis, NULL);
    assert(lisevent != NULL);
    event_add(lisevent, NULL);
    event_base_dispatch(mainbase);
    event_free(lisevent);
    event_base_free(mainbase);
    close(listen_fd);
    return 0;
}

