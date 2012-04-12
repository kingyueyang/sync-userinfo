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
    /*
     *
     * Load logging module
     * if error return process
     */
    if(logging()) {
        log4c_category_log(log_handler, LOG4C_PRIORITY_FATAL, "primary process startup -- failed");
        return -1;
    }
    log4c_category_log(log_handler, LOG4C_PRIORITY_DEBUG, "primary process startup -- successful");

    /*
     *Create daemon
     *if error return process
     */
    if(xdaemon()) {
        log4c_category_log(log_handler, LOG4C_PRIORITY_FATAL, "process into daemon state -- failed");
        return -2;
    }
    log4c_category_log(log_handler, LOG4C_PRIORITY_DEBUG, "process into daemon state -- successful");

    /*
     *Load configure files
     *initate server
     */
    if(initServerConfig()) {
        log4c_category_log(log_handler, LOG4C_PRIORITY_FATAL, "initiate server configure -- failed");
        return -3;
    }
    log4c_category_log(log_handler, LOG4C_PRIORITY_DEBUG, "initiate server configure -- successful");

    /*
     *Create queue
     *store receiver item
     */
    if(create_queue()) {
        log4c_category_log(log_handler, LOG4C_PRIORITY_FATAL, "create queue -- failed");
        return -4;
    }
    log4c_category_log(log_handler, LOG4C_PRIORITY_DEBUG, "create queue -- successful");

    /*  Create there process: receiver, queue, mysql-connector */
    createthread();

    /*
     * Server fall into Looooop
     */

    return EXIT_SUCCESS;
}

int
initServerConfig(void) {
    server.receiverIP = "127.0.0.1";
    server.receiverPort = 8080;

    return 0;
}

int
xdaemon(void) {
    return 0;
    return daemon(0, 0);
}

int
create_queue(void) {
    apr_status_t rv;
    if(apr_pool_initialize()) {
        return 1;
    }
    if(apr_pool_create(&pool, NULL)) {
        return 2;
    }
    if(apr_queue_create(&queue, QUEUE_SIZE, pool)) {
        return 3;
    }

    return 0;
}

int 
createthread(void) {
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

