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

#include <stdio.h>
#include <curl/curl.h>

int 
notifu_rt(char *type, long uid) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headerlist = NULL;

    headerlist = curl_slist_append(headerlist, "Content-Type: text/html");

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "127.0.0.1:8888");
        /*curl_easy_setopt(curl, CURLOPT_URL, "http://10.10.140.94:8080/realtime/jservice/recommondation");*/
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "S001,407902377");
        /*同校S001 同班S002 同乡S003*/

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        curl_slist_free_all (headerlist);
        curl_easy_cleanup(curl);
    }
    return 0;
}

