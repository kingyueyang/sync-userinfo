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
    config_setting_t *receiver, *mysql_server;

    /* Init server inf struct */
    receiver_cfg receiver_conf;
    mysql_cfg mysql_conf;

    config_init(&cfg);

    /* Read configure file, and handle error */
    if (! config_read_file(&cfg, conf_file)) {
        fprintf(stderr, "open configure file(%s) error - %s .\n",
                config_error_file(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit (EXIT_FAILURE);
    }

    /* Get configure mdul name */
    if (config_lookup_string(&cfg, "module_name", &module_name)) {
        printf("%s\n", module_name);
    } else {
        fprintf(stderr, "No name int configure file.\n");
    }

    /* Get server base information */
    receiver = config_lookup(&cfg, "receiver");
    if(receiver != NULL) {
        int count = config_setting_length(receiver);

        /* Load receiver configure */
        if(!(config_setting_lookup_string(receiver,
                        "server_ip", &receiver_conf.receiver_ip))) {
            printf("%s\n", receiver_conf.receiver_ip);
        } else {
            printf ( "no ip\n" );
        }

    }
    config_destroy(&cfg);
    return 0;
}

int
main( int argc, char *argv[]) {
    get_conf("../conf/configure.cfg");

    /* test code */
    /*printf("%s\n", feiliao_conf.sub_srv.get_url);*/
    /*printf("%s\n", feiliao_conf.sub_srv.get_userpwd);*/
    /*printf("%s\n", feiliao_conf.sub_srv.get_path);*/
    /*printf("%s\n", feiliao_conf.sub_srv.local_cached_path);*/
    /*printf("%s\n", feiliao_conf.sub_srv.put_url);*/
    /*printf("%s\n", feiliao_conf.sub_srv.put_userpwd);*/
    /*printf("%d\n", feiliao_conf.sub_srv.retry);*/
    /*printf("%s\n", feiliao_conf.sub_srv.set_day);*/
    /*printf("%s\n", feiliao_conf.sub_srv.phone_no);*/
    /*printf("\n");*/

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


