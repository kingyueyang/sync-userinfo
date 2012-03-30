/*
 * =====================================================================================
 *
 *       Filename:  inf_basic_info.c
 *
 *    Description:  Sync user basic information call back
 *
 *        Version:  1.0
 *        Created:  03/30/2012 05:53:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "inf_basic_info.h"

void
post_SBI_cb(struct evhttp_request *req, void *arg) {
    fprintf(stdout, "SBI\n");
    evhttp_send_reply(req, 200, "OK", NULL);
    return ;
}

