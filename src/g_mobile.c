/*
 * =====================================================================================
 *
 *       Filename:  g_sid_mobile.c
 *
 *    Description:  get sid by mobile NO.
 *
 *        Version:  1.0
 *        Created:  07/10/2012 04:22:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "g_mobile.h"

static size_t
parser_xml(void *ptr, size_t size, size_t nmemb, void *stream) {
    xmlDocPtr doc = NULL;
    xmlNodePtr cur = NULL;
    xmlNodePtr child = NULL;

    char *mobile_value = NULL;

    xmlKeepBlanksDefault(0);

    doc = xmlParseMemory((char *)ptr, strlen(ptr));
    if(NULL == doc) {
        xmlCleanupParser();
        return 1;
    }

    cur = xmlDocGetRootElement(doc);
    if(NULL == cur) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return 1;
    }

    if(cur->type != XML_ELEMENT_NODE) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return 1;
    }

    if(xmlStrcmp(cur->name, BAD_CAST "fetion") != 0) {
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return 1;
    }

    child = cur->xmlChildrenNode;
    if(child) {
        child = child->next;

        for(child = child->xmlChildrenNode; child; child = child->next) {
            if(0 == xmlStrcmp(BAD_CAST "mobileNo", child->name)) {
                mobile_value = (char *)xmlNodeGetContent(child);
                snprintf(stream, 128, "%s", mobile_value);
		xmlFree(mobile_value);
            }
            /*
             *skip unsed item
             */
        }

    }
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return (nmemb*size);
}

char *
get_mobile(long unsigned int uid) {
    char *request = NULL;
    char *_data = NULL;

    request = (char *)xmalloc(sizeof(REQUEST_XML_BASE) + UID_LEN);
    _data = xmalloc(1024);

    memset(request, 0, sizeof(REQUEST_XML_BASE) + UID_LEN);
    memset(_data, 0, 1024);

    if(NULL == request || NULL == _data) {
        xfree(request);
        xfree(_data);
        return (char *)0;
    }

    snprintf(request, sizeof(REQUEST_XML_BASE) + UID_LEN,
            REQUEST_XML_BASE, uid);

    send_str(request, &_data);
printf("parser: %s\n", _data);

    xfree(request);

    return (char *)_data;
}		/* -----  end of function get_mobile  ----- */

int 
send_str(char *input,  char **output) {
    CURL *curl;
    CURLcode res = 0;

    struct curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, "Content-Type: application/xml");
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://172.21.4.225/oxp/jservice/oxpServer");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, input);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parser_xml);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, *output);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        if(res) {
            /*log4c_category_log(*/
            /*log_handler, LOG4C_PRIORITY_ERROR,*/
            /*"notify: notify to real time error: %s", type);*/
        } else {
            /*log4c_category_log(*/
            /*log_handler, LOG4C_PRIORITY_TRACE,*/
            /*"notify: notify to real time: %s", type);*/
        }

        curl_easy_cleanup(curl);
        //parser_xml(base, 512, 1, *output);
    }
    curl_slist_free_all (headerlist);

    return (int)res;
}

/* 
int
main( int argc, char *argv[]) {
    char *value = get_mobile(204757783);
    printf("%s\n", value);
    xfree(value);

    return EXIT_SUCCESS;
}

 */
