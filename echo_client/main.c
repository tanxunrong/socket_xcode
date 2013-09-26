//
//  main.c
//  echo_client
//
//  Created by tanxunrong on 13-9-26.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#include "../include/common.h"

static int rcount = 0;
static int sockfd = 0;
void dealSig(int signum)
{
    printf("read from server count %d\n",rcount);
    close(sockfd);
    exit(0);
}

int main(int argc,char* argv[])
{
    
    char recvline[MAXLINE];
    struct sockaddr_in servaddr;
    
    memset(recvline,0,sizeof(recvline));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(3789);
    if (inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr) <= 0)
        err_sys("IPv4 host addr false");
    
    if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
        err_quit("create socket err");
    if ( connect(sockfd,(struct sockaddr *)&servaddr,sizeof(struct sockaddr_in)) < 0)
        err_sys("connect error");
    char *msg="client msg";
    
    alarm(100);
    signal(SIGALRM,(void*)dealSig);
    signal(SIGPIPE,(void*)dealSig);
    
    while (1)
    {
        write(sockfd,msg,strlen(msg)+1);
        memset(recvline,0,MAXLINE);
        read(sockfd,recvline,MAXLINE);
        rcount++;
    }
    
    return 0;
}

