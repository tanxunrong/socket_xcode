//
//  main.c
//  echo_server
//
//  Created by tanxunrong on 13-9-26.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#include "../include/common.h"
#include <event2/event.h>
#include <assert.h>

typedef struct sock_conn {
    int sockfd;
    int status;
    char rbuf[MAXLINE];
    char wbuf[MAXLINE];
    
} sock_conn_t;
typedef sock_conn_t * sock_conn_p;
static  evutil_socket_t listen_fd=0;
static  struct event_base *evbase,*evlisten;
#define CONN_MAXFD 65536
static sock_conn_t conn_table[CONN_MAXFD] = {0};
static sig_atomic_t toshut=0;
void callback_accept(evutil_socket_t fd,short what,void *arg)
{
    evutil_socket_t sockfd=accept(fd, NULL, NULL);
    conn_table[sockfd].status=1;
    
}

int main(int argc,char* argv[])
{
    //server socket addr
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
    
    struct event_config *evcfg;
    evcfg = event_config_new();
    assert(evcfg != NULL);
    event_config_avoid_method(evcfg, "select");
    struct event_base *evbase = event_base_new_with_config(evcfg);
    assert(evbase != NULL);
    event_config_free(evcfg);
    
    evlisten=event_base_new();
    assert(evlisten != NULL);
    
    struct event *evaccept=event_new(evlisten, listen_fd, EV_READ, callback_accept, NULL);
    assert(evaccept != NULL);
    
    
    return 0;
}

