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
#include <arpa/inet.h>

struct syncServer server;

apr_pool_t *pool = NULL;
apr_queue_t *queue;

int
main(int argc, char *argv[]) {
    /* Create one new process by daemon module, if 0 former process return 0 */
    xdaemon();

    /* Load configure files, initate server */
    initServerConfig();

    /* Create global queue */
    create_queue();

    /*  Create there process: receiver, queue, mysql-connector */
    createthread();

    /*
     * Server fall into Looooop
     */

    return EXIT_SUCCESS;
}

void
initServerConfig(void) {
    server.receiverIP = "127.0.0.1";
    server.receiverPort = 8080;

    return ;
}

void
xdaemon(void) {

    return ;
}

void
create_queue(void) {
    apr_status_t rv;
    if(apr_pool_initialize()) {
        exit(1);
    }
    if(apr_pool_create(&pool, NULL)) {
        exit(1);
    }
    if(apr_queue_create(&queue, QUEUE_SIZE, pool)) {
        exit(1);
    }

    return;
}

int 
createthread() {
    int rc;
    pthread_t receiver_tid, request_tid;

    rc = pthread_create(&request_tid, NULL, receiver, NULL);
    assert(0 == rc);

    int mysql_thread_no = 10;
    int count;
    for(count = 0; count < mysql_thread_no; count++) {
        rc = pthread_create(&request_tid, NULL, mysql_connector, NULL);
        assert(0 == rc);
    }

    while(1) {
        sleep(5);
    }

    return 0;
}

