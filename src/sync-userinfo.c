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

int
main( int argc, char *argv[] ) {
    /* Load configure files, initate server */
    initServerConfig();

    /* Create one new process by daemon module, if 0 former process return 0 */
    xdaemon();

    /*  Create there process: receiver, queue, mysql-connector */
    createthread();

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
createthread( ) {
    /*rt = receiver();*/
    /*rt = msg_queue_server();*/
    int rc;
    pthread_t receiver_tid, request_tid;

    rc = pthread_create(&request_tid, NULL, receiver, NULL);
    assert(0 == rc);



    return 0;
}

