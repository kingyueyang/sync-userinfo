/*
 * =====================================================================================
 *
 *       Filename:  g_mobile.h
 *
 *    Description:  get sid by mobile NO.
 *
 *        Version:  1.0
 *        Created:  07/10/2012 04:44:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>

#include "utils.h"

#define	UID_LEN 13			/* uid size */
#define _FMT_SIZE_T "lu"
#define	REQUEST_XML_BASE "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"\
    "<fetion><function>getUserInfoByUid""</function>"\
    "<body><uid>%ld</uid></body></fetion>"  /* Request mobile NO. XML */

char *get_mobile(long unsigned int uid);
int send_str(char *input, char **output);
static size_t parser_xml(void *ptr, size_t size, size_t nmemb, void *stream);

