/*******************************************************************************
**                                                                             *
**    Project: IIC Multiple Bus Controller (IICMB)                             *
**                                                                             *
**    File:    Unittest for IRQ based driver for IICMB                         *
**    Version:                                                                 *
**             1.0,     Feb 24, 2023                                           *
**                                                                             *
**    Author:  Andreas Kaeberlein, (andreas.kaeberlein@siemens.com)            *
**                                                                             *
********************************************************************************
********************************************************************************
** Copyright (c) 2023, Sergey Shuvalkin                                        *
** All rights reserved.                                                        *
**                                                                             *
** Redistribution and use in source and binary forms, with or without          *
** modification, are permitted provided that the following conditions are met: *
**                                                                             *
** 1. Redistributions of source code must retain the above copyright notice,   *
**    this list of conditions and the following disclaimer.                    *
** 2. Redistributions in binary form must reproduce the above copyright        *
**    notice, this list of conditions and the following disclaimer in the      *
**    documentation and/or other materials provided with the distribution.     *
**                                                                             *
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  *
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE    *
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         *
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        *
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     *
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     *
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  *
** POSSIBILITY OF SUCH DAMAGE.                                                 *
*******************************************************************************/



/** Standard libs **/
#include <stdio.h>          // f.e. printf
#include <stdlib.h>         // defines four variables, several macros,
                            // and various functions for performing
                            // general functions
#include <stdint.h>         // defines fiexd data types, like int8_t...
#include <unistd.h>         // system call wrapper functions such as fork, pipe and I/O primitives (read, write, close, etc.).
#include <string.h>         // string handling functions
#include <stdarg.h>         // variable parameter lists
#include <fcntl.h>          // manipulate file descriptor
#include <ctype.h>          // used for testing and mapping characters

/** User Libs **/
#include "iicmb.h"	// self



/**
 *  prints register of iicmb
 */
void print_reg_iicmb ( uint8_t* iicmb )
{
	/* print to console */
	printf (  	"INFO:iicmb_reg:dump:\n"
				"  CSR  = 0x%02x\n"
				"  DPR  = 0x%02x\n"
				"  CMDR = 0x%02x\n"
				"  FSMR = 0x%02x\n"
				,
				iicmb[0],
				iicmb[1],
				iicmb[2],
				iicmb[3]
			);
}



/**
 *  Main
 *  ----
 */
int main ()
{
    /** Variables **/
	uint8_t		uint8RegIICMB[4] = {0xff, 0x00, 0x80, 0x00};	// register handle for IICMB: CSR, DPR, CMDR, FSMR
	t_iicmb  	iicm;											// handle for IICMB driver
	
	
	
	/* entry message */
	printf("INFO:%s: unit test started\n", __FUNCTION__);
	
	/* iicm_init */
	printf("INFO:%s:iicm_init\n", __FUNCTION__);
	uint8RegIICMB[0] = 0xf5;	// CSR, checked by init
	if ( 0 != iicmb_init(&iicm, (void*) &uint8RegIICMB, 5) ) {
		printf("ERROR:%s:iicm_init: failed\n", __FUNCTION__);
		print_reg_iicmb( (uint8_t*) &uint8RegIICMB);
		goto ERO_END;
	}
	







	
	/* end register dump */
	print_reg_iicmb((uint8_t*) &uint8RegIICMB);

	/* avoid warning */
	goto OK_END;
    /* gracefull end */
    OK_END:
		printf("INFO:%s: Module test SUCCESSFUL :-)\n", __FUNCTION__);
		exit(EXIT_SUCCESS);

	/* avoid warning */
	goto ERO_END;
    /* abnormal end */
    ERO_END:
        printf("FAIL:%s: Module test FAILED :-(\n", __FUNCTION__);
        exit(EXIT_FAILURE);

}
