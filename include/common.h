//
//  common.h
//  unix_socket
//
//  Created by tanxunrong on 13-9-25.
//  Copyright (c) 2013年 tanxunrong. All rights reserved.
//

#ifndef unix_socket_common_h
#define unix_socket_common_h

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>

#define MAXLINE 1000
#define LISTEN_QUEUE 10

void err_quit(char* str)
{
    fprintf(stderr,"%s",str);
    exit(EXIT_FAILURE);
}

void err_sys(char *str)
{
    perror(str);
    exit(errno);
}

void setfdnonblock(int fd)
{
    int fctl = fcntl(fd,F_GETFL);
    fctl |= O_NONBLOCK;
    fcntl(fd,F_SETFL,fctl);
}

#endif
