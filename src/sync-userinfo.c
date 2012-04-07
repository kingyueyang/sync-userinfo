/*
 * =====================================================================================
 *
 *       Filename:  sync-userinfo.c
 *
 *    Description:  sync user infornation
 *
 *        Version:  1.0
 *        Created:  03/30/2012 02:43:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "sync-userinfo.h"

struct syncServer server;

apr_pool_t *arp_pool = NULL;
apr_queue_t *apr_queue;

int
main( int argc, char *argv[] ) {
    /* Load configure files, initate server */
    initServerConfig();

    /* Create one new process by daemon module, if 0 former process return 0 */
    xdaemon();

    /*  Create there process: receiver, queue, mysql-connector */
    dofork(3, 0);

    /*
     * Server fall into Looooop
     */

    return EXIT_SUCCESS;
}

void
initServerConfig( void ) {
    server.receiverIP = "127.0.0.1";
    server.receiverPort = 8080;

    return ;
}

void
xdaemon( void ) {

    return ;
}

int 
dofork( size_t fork_no, int flag ) {
    pid_t pid=0;
    int rt;

    pid = fork( );
    if( pid < 0 )
        return -1;
    else if( 0 == pid ) {
        printf("I am child\n");
        rt = receiver();
        return rt;
    } else {
        /* Wirte down the PID */
        printf("I am farther,child is %d\n",pid);
    }

    pid = fork( );
    if( pid < 0 )
        return -1;
    else if( 0 == pid ) {
        printf("I am child\n");
        rt = msg_queue_server();
        return rt;
    } else {
        /* Wirte down the PID */
        printf("I am farther,child is %d\n",pid);
    }

    return 0;
}

