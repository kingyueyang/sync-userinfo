/*
 * =====================================================================================
 *
 *       Filename:  logging.c
 *
 *    Description:  log module
 *
 *        Version:  1.0
 *        Created:  04/12/2012 04:50:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "logging.h"
 
log4c_category_t* log_handler = NULL;

int
logging(void) {
    log4c_init();
    log_handler = log4c_category_get("test-project");
    if(NULL == log_handler) {
        fprintf(stderr, "log module load error.\n" );
        return -1;
    }

    log4c_category_log(log_handler, LOG4C_PRIORITY_DEBUG, "logging mudule startup");

    /*if ( log4c_fini()){*/
        /*printf("log4c_fini() failed");*/
    /*}*/

    return 0;
}

/*[>* fatal <]	LOG4C_PRIORITY_FATAL	= 000, */
/*[>* alert <]	LOG4C_PRIORITY_ALERT	= 100, */
/*[>* crit <]	      	LOG4C_PRIORITY_CRIT	= 200, */
/*[>* error <]	LOG4C_PRIORITY_ERROR	= 300, */
/*[>* warn <]	      	LOG4C_PRIORITY_WARN	= 400, */
/*[>* notice <]	LOG4C_PRIORITY_NOTICE	= 500, */
/*[>* info <]	      	LOG4C_PRIORITY_INFO	= 600, */
/*[>* debug <]	LOG4C_PRIORITY_DEBUG	= 700,*/
/*[>* trace <]	LOG4C_PRIORITY_TRACE	= 800,*/
/*[>* notset <]	LOG4C_PRIORITY_NOTSET	= 900,*/
/*[>* unknown <]	LOG4C_PRIORITY_UNKNOWN	= 1000*/

