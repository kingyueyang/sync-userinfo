/*
 * =====================================================================================
 *
 *       Filename:  inf_header_info.c
 *
 *    Description:  Sync user head portrait information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 06:34:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_header_info.h"

void
post_SHI_cb(struct evhttp_request *req, void *arg) {
    fprintf(stdout, "SHI\n");
    evhttp_send_reply(req, 200, "OK", NULL);
    return ;
}

