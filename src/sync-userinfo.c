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
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: primary process startup -- failed");
        return -1;
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: primary process startup -- successful");

    /*
     *Load configure files
     *initate server
     */
    if(initServerConfig()) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: initiate server configure -- failed");
        return -3;
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: initiate server configure -- successful");

    /*
     *Create daemon
     *if error return process
     */
    if(xdaemon()) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: process into daemon state -- failed");
        return -2;
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: process into daemon state -- successful");

    /*
     *Create queue
     *store receiver item
     */
    if(create_queue()) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: create queue -- failed");
        return -4;
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: create queue -- successful");

    /*
     *create dump file
     *when MySQL lost connect
     */
    if(create_dump()) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: create dump file -- failed");
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: create queue -- successful");

    /*
     *Create some process include:
     *one receiver
     *many mysql-connector 
     */
    create_thread();

    /*
     * Process never run to here
     */

    return EXIT_SUCCESS;
}

int
initServerConfig(void) {
    server.receiverIP = "0.0.0.0";
    server.receiverPort = 8080;
    server.dump_file = "/tmp/community_sync.dump";

    server.mysqlIP = "192.168.142.10";
    server.mysqlUser = "ci_user";
    server.mysqlPasswd = "y4hrthjUggg";
    server.db = "ci";
    server.mysqlPort = 3306;
    server.mysqlThread = 2;

    get_conf("../conf/configure.cfg");

    /*printf ( "%s\n", server.receiverIP );*/
    /*printf ( "%d\n", server.receiverPort );*/
    /*printf("%s\n", server.dump_file);*/

    /*printf("%s\t", server.mysqlIP);*/
    /*printf("%s\t", server.mysqlUser);*/
    /*printf("%s\t", server.mysqlPasswd);*/
    /*printf("%s\t", server.db);*/
    /*printf("%d\t", server.mysqlPort);*/
    /*printf("%d\n", server.mysqlThread);*/

    return 0;
}

int
xdaemon(void) {
    /*FIXME: debug modue*/
    /*return 0;*/
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
create_dump(void) {
    server.dump_file_handler= fopen("/tmp/mysql.dump", "a");
    if(NULL == server.dump_file_handler) {
        return 1;
    }

    fprintf(server.dump_file_handler, "\n");
    fflush(server.dump_file_handler);

    return 0;
}

void
create_thread(void) {
    int rc;
    void *receiver_rc = 0, *request_rc[50] = {0};
    pthread_t receiver_tid, request_tid[50];

    rc = pthread_create(&receiver_tid, NULL, receiver, NULL);
    if(0 != rc) {
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_FATAL,
                "sync: create receiver thread-- failed");
        exit(-5);
    }
    log4c_category_log(
            log_handler, LOG4C_PRIORITY_DEBUG,
            "sync: create receiver thread -- successful");

    int count;
    for(count = 0; count < server.mysqlThread; count++) {
        rc = pthread_create(&request_tid[count], NULL, mysql_connector, NULL);
        if(0 != rc) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_FATAL,
                    "sync: create mysql_connect thread: %u -- failed", count);
            exit(-5);
        }
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_DEBUG,
                "sync: create mysql_connect thread: %u -- successful", count);
    }

    while(1) {
        /*FIXME:moniter thread restat*/
        /*
         *moniter of receiver thread
         */
        /*pthread_join(receiver_tid, &receiver_rc);*/
        /*log4c_category_log(*/
                /*log_handler, LOG4C_PRIORITY_ERROR,*/
                /*"sync: will check thread");*/

        pthread_tryjoin_np(receiver_tid, &receiver_rc);
        if(receiver_rc != 0) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_ERROR,
                    "sync: receiver thread abnormal termination, %d", receiver_rc);
            exit(-1);
        }

        for(count = 0; count < server.mysqlThread; count++) {
            pthread_tryjoin_np(request_tid[count], &request_rc[count]);
            if(request_rc[count] != 0) {
                log4c_category_log(
                        log_handler, LOG4C_PRIORITY_ERROR,
                        "sync: mysql connector threads\
                        abnormal termination, %d:%d", count, receiver_rc);
                exit(-1);
            }
        }
        /*log4c_category_log(*/
                /*log_handler, LOG4C_PRIORITY_ERROR,*/
                /*"sync: checked thread done");*/

        sleep(1);
    }

    return ;
}

