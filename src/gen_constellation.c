/*
 * =====================================================================================
 *
 *       Filename:  gen_constellation.c
 *
 *    Description:  generate constellation
 *
 *        Version:  1.0
 *        Created:  07/10/2012 11:17:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "gen_constellation.h"

unsigned int
generate_constellation (unsigned int month, unsigned int day) {
    unsigned int constellation = 0;

    if( month < 1 || month > 12 ) {
        return constellation;
    }

    if( (1 == month || 3 == month || 5 == month || 7 == month 
                || 8 == month || 10 == month || 12 == month)
            && (day < 1 || day > 31) ) {
        return constellation;
    }

    if( (4 == month || 6 == month || 9 == month || 11 == month) &&
            (day < 1 || day > 30) ) {
        return constellation;
    }

    if( (2 == month ) && (day < 1 || day > 29) ) {
        return constellation;
    }

    unsigned int months[] = {20, 19, 21, 21, 21, 22, 23, 23, 23, 23, 22, 22};

    constellation = (month - (day < months[month - 1]? 1: 0))
        ? month - (day < months[month - 1]? 1: 0) : 12;

    return constellation;
}		/* -----  end of function generate_constellation  ----- */

