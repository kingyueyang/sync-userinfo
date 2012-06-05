/*
 * =====================================================================================
 *
 *       Filename:  notify.c
 *
 *    Description:  notify real time compute
 *
 *        Version:  1.0
 *        Created:  04/12/2012 02:08:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "notify.h"

/* int64_t(20) + type(6) */
#define	CONTENT_LEN 26			/*  */

int 
notify_rt(char *type, char *uid) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;

    headerlist = curl_slist_append(headerlist, "Content-Type: text/html");

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://172.21.73.131:8080/realtime/jservice/recommondation");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);

        /*同校S001 同班S002 同乡S003*/
        char *content = xmalloc(CONTENT_LEN);
        sprintf(content, "%s,%s", type, uid);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        if(res) {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_ERROR,
                    "notify: notify to real time error: %s", type);
        } else {
            log4c_category_log(
                    log_handler, LOG4C_PRIORITY_TRACE,
                    "notify: notify to real time: %s", type);
        }

        xfree(content);
        curl_slist_free_all (headerlist);
        curl_easy_cleanup(curl);
    }
    return (int)res;
}

