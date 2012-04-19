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

conf
get_conf(const char *conf_file) {
    const char *title;
    config_t cfg;
    config_setting_t *setting_srv, *setting_inf;

    /* Init server inf struct */
    srv_conf server_conf;
    inf_conf interface_conf[INFCNT];
    conf all_conf;

    config_init(&cfg);

    /* Read configure file, and handle error */
    if (! config_read_file(&cfg, conf_file)) {
        fprintf(stderr, "open configure file(%s) error - %s .\n",
                config_error_file(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit (EXIT_FAILURE);
    }

    /* Get configure title */
    if (config_lookup_string(&cfg, "name", &title)) {
        printf("%s\n", title);
    } else {
        fprintf(stderr, "No name int configure file.\n");
    }

    /* Get server base information */
    setting_srv = config_lookup(&cfg, "feiliao.server");
    if (setting_srv != NULL) {
        int count = config_setting_length(setting_srv);
        /* Loop range in count, but here have only ONE item*/
        for (int i=0; i < count; ++i) {
            config_setting_t *server_srv = config_setting_get_elem(setting_srv, i);

            /* Load server inf context */
            if (!(config_setting_lookup_string(server_srv,
                            "get-ftp-url", &server_conf.get_url) &&
                        config_setting_lookup_string(server_srv,
                            "get-ftp-userpwd", &server_conf.get_userpwd) &&
                        config_setting_lookup_string(server_srv,
                            "get-ftp-path", &server_conf.get_path) &&
                        config_setting_lookup_string(server_srv,
                            "local-cached-path", &server_conf.local_cached_path) &&
                        config_setting_lookup_string(server_srv,
                            "put-ftp-url", &server_conf.put_url) &&
                        config_setting_lookup_string(server_srv,
                            "put-ftp-userpwd", &server_conf.put_userpwd) &&
                        config_setting_lookup_string(server_srv,
                            "set-day", &server_conf.set_day) &&
                        config_setting_lookup_string(server_srv,
                            "sms-gateway", &server_conf.sms_gateway) &&
                        config_setting_lookup_string(server_srv,
                            "phone-no", &server_conf.phone_no) &&
                        config_setting_lookup_int(server_srv,
                            "retry", &server_conf.retry)
                 )) { continue; }

        }
    }

    /* Get per-interface inf */
    setting_inf = config_lookup(&cfg, "feiliao.interface");
    if (setting_inf != NULL) {
        int count = config_setting_length(setting_inf);
        /* Loop range in count, but here have only ONE item*/
        for (int i=0; i < count; ++i) {
            config_setting_t *server_inf = config_setting_get_elem(setting_inf, i);

            /* Load every inferface inf context */
            if (!(config_setting_lookup_string(server_inf,
                            "name", &interface_conf[i].name) &&
                        config_setting_lookup_string(server_inf,
                            "flag", &interface_conf[i].flag) &&
                        config_setting_lookup_string(server_inf,
                            "code", &interface_conf[i].code) &&
                        config_setting_lookup_string(server_inf,
                            "server", &interface_conf[i].server) &&
                        config_setting_lookup_int(server_inf,
                            "item_no", &interface_conf[i].item_no) &&
                        config_setting_lookup_string(server_inf,
                            "put-ftp-path", &interface_conf[i].put_ftp_path)
                 )) { continue; }

        }
    }

    /* Prepare return configure struct */
    all_conf.sub_srv = server_conf;

    for (int i=0; i<INFCNT; ++i) {
        all_conf.sub_inf[i] = interface_conf[i];
    }

    config_destroy(&cfg);

    return all_conf;
}

int
main ( int argc, char *argv[] ) {
    conf feiliao_conf;
    feiliao_conf = get_conf("../conf/config.cfg");

    /* test code */
    printf("%s\n", feiliao_conf.sub_srv.get_url);
    printf("%s\n", feiliao_conf.sub_srv.get_userpwd);
    printf("%s\n", feiliao_conf.sub_srv.get_path);
    printf("%s\n", feiliao_conf.sub_srv.local_cached_path);
    printf("%s\n", feiliao_conf.sub_srv.put_url);
    printf("%s\n", feiliao_conf.sub_srv.put_userpwd);
    printf("%d\n", feiliao_conf.sub_srv.retry);
    printf("%s\n", feiliao_conf.sub_srv.set_day);
    printf("%s\n", feiliao_conf.sub_srv.phone_no);
    printf("\n");


    for (int i=0; i<INFCNT; ++i) {
        printf("%s\n", feiliao_conf.sub_inf[i].name);
        printf("%s\n", feiliao_conf.sub_inf[i].flag);
        printf("%s\n", feiliao_conf.sub_inf[i].code);
        printf("%s\n", feiliao_conf.sub_inf[i].server);
        printf("%d\n", feiliao_conf.sub_inf[i].item_no);
        printf("%s\n", feiliao_conf.sub_inf[i].put_ftp_path);
        printf("\n");
    }

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

