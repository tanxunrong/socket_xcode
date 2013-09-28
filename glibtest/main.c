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
int main(int argc,char* argv[])
{
    GHashTable *connht=g_hash_table_new_full(g_str_hash, g_str_equal,NULL,free);
#define LEN_OF_CONNLIST 10
    txr_conn_p conn[LEN_OF_CONNLIST];
    for (int i=0; i<LEN_OF_CONNLIST; i++) {
        conn[i]=(txr_conn_p)malloc(sizeof(txr_conn_t));
        memset(conn[i], 0, sizeof(txr_conn_t));
        conn[i]->sockfd=rand();
        sprintf(conn[i]->key, "conn list %d",i);
        printf("key %d : %s\n",i,conn[i]->key);
        g_hash_table_insert(connht, conn[i]->key, conn[i]);
    }
    assert(10 == g_hash_table_size(connht));
    txr_conn_p connfind = g_hash_table_lookup(connht, "conn list 8");
    assert(connfind != NULL);
    assert(TRUE == g_hash_table_contains(connht, "conn list 7"));
    g_hash_table_destroy(connht);
    return 0;
}


