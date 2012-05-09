/*
 * =====================================================================================
 *
 *       Filename:  config.c
 *
 *    Description:  load configure file
 *
 *        Version:  1.0
 *        Created:  12/21/2011 04:56:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */


#include "config.h"

int
get_conf(const char *conf_file) {
    const char *module_name;
    config_t cfg;
    config_setting_t *receiver_server, *mysql_server;

    config_init(&cfg);

    /* Read configure file, and handle error */
    if (! config_read_file(&cfg, conf_file)) {
        if (! config_read_file(&cfg, "/etc/configure.cfg")) {
            fprintf(stderr, "open configure file(%s) error - %s .\n",
                    config_error_file(&cfg), config_error_text(&cfg));
            config_destroy(&cfg);
            exit (EXIT_FAILURE);
        }
    }

    /* Get configure mdul name */
    if (config_lookup_string(&cfg, "module_name", &module_name)) {
        ;
    } else {
        fprintf(stderr, "No name int configure file.\n");
    }

    /* Get receiver information */
    receiver_server = config_lookup(&cfg, "config.receiver");
    if(receiver_server != NULL) {
        int count = config_setting_length(receiver_server);
        if(count != 1) {
            return -1;
        }

        /* Load receiver_server configure */
        config_setting_t *receiver = config_setting_get_elem(receiver_server, 0);
        if(!(config_setting_lookup_string(receiver, "server_ip", &server.receiverIP)
                    && config_setting_lookup_int(receiver, "server_port", &server.receiverPort)
                    && config_setting_lookup_string(receiver, "dump_file", &server.dump_file) )) {
            return -2;
        }
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_INFO,
                "config: receiverIP:%s, receiverPort:%d, dump_file:%s",
                server.receiverIP, server.receiverPort, server.dump_file);

    } else {
        fprintf(stderr, "no item reveiver_server.\n");
    }

    /* Get mysql information */
    mysql_server = config_lookup(&cfg, "config.mysql");
    if(mysql_server != NULL) {
        int count = config_setting_length(mysql_server);
        if(count != 1) {
            return -1;
        }

        /* Load mysql_server configure */
        config_setting_t *conn_mysql = config_setting_get_elem(mysql_server, 0);
        if(!(config_setting_lookup_string(conn_mysql, "ip", &server.mysqlIP)
                    && config_setting_lookup_string(conn_mysql,
                        "user", &server.mysqlUser)
                    && config_setting_lookup_string(conn_mysql,
                        "passwd", &server.mysqlPasswd)
                    && config_setting_lookup_string(conn_mysql,
                        "db", &server.db)
                    && config_setting_lookup_int(conn_mysql,
                        "port", &server.mysqlPort)
                    && config_setting_lookup_int(conn_mysql,
                        "thread_count", &server.mysqlThread)
            )) {
            return -3;
        }
        log4c_category_log(
                log_handler, LOG4C_PRIORITY_INFO,
                "config: mysqlIP:%s, mysqlUser:%s, mysqlPasswd:%s,\
                db:%s, mysqlPort:%d, mysqlThread:%d",
                server.mysqlIP, server.mysqlUser, server.mysqlPasswd,
                server.db, server.mysqlPort, server.mysqlThread);

    } else {
        fprintf(stderr, "no item mysql_server.\n");
    }

    /*config_destroy(&cfg);*/
    return 0;
}

