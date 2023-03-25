
/*******************************************************************************
**                                                                             *
**    Project: IIC Multiple Bus Controller (IICMB)                             *
**                                                                             *
**    File:    Definitions of several basic functions for IICMB core.          *
**    Version:                                                                 *
**             1.0,     May 25, 2016                                           *
**                                                                             *
**    Author:  Sergey Shuvalkin, (sshuv2@opencores.org)                        *
**                                                                             *
********************************************************************************
********************************************************************************
** Copyright (c) 2016, Sergey Shuvalkin                                        *
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


#ifndef __IICMB_H__
#define __IICMB_H__

/* IICMB controller base address: */
#define IICMB_BASE_ADDR      (0x00800000)

/* IICMB register offsets: */
#define IICMB_CSR            (0x00)
#define IICMB_DPR            (0x01)
#define IICMB_CMDR           (0x02)
#define IICMB_FSMR           (0x03)

/* Bits of CSR register */
#define IICMB_CSR_ENABLE     (0x80)
#define IICMB_CSR_IRQ_ENABLE (0x40)

/* Response codes in CMDR register: */
#define IICMB_RSP_DONE       (0x80)
#define IICMB_RSP_NAK        (0x40)
#define IICMB_RSP_ARB_LOST   (0x20)
#define IICMB_RSP_ERR        (0x10)
#define IICMB_RSP_COMPLETED  (0xF0)

/* Responses */
typedef enum
{
  rsp_done,
  rsp_nak,
  rsp_arb_lost,
  rsp_err
} rsp_tt;

/* Print responses */
void print_rsp(rsp_tt r);

/* Command codes in CMDR register: */
#define IICMB_CMD_WAIT       (0x00)
#define IICMB_CMD_WRITE      (0x01)
#define IICMB_CMD_READ_ACK   (0x02)
#define IICMB_CMD_READ_NAK   (0x03)
#define IICMB_CMD_START      (0x04)
#define IICMB_CMD_STOP       (0x05)
#define IICMB_CMD_SET_BUS    (0x06)

/* Commands */
typedef enum
{
  cmd_wait,
  cmd_write,
  cmd_read_ack,
  cmd_read_nak,
  cmd_start,
  cmd_stop,
  cmd_set_bus
} cmd_tt;


/* IICMB controller register read/write primitives: */
#define IICMB_REG_WRITE(off, val) IOWR_8DIRECT(IICMB_BASE_ADDR, (off), (val))
#define IICMB_REG_READ(off)       IORD_8DIRECT(IICMB_BASE_ADDR, (off))


void iicmb_init(void);            /* Initialize IICMB core */
void iicmb_disable(void);         /* Disable IICMB core */

/* Generic Interface commands: ***********************************************/
rsp_tt iicmb_cmd_wait(unsigned char n);       /* Wait          */
rsp_tt iicmb_cmd_write(unsigned char n);      /* Write         */
rsp_tt iicmb_cmd_read_ack(unsigned char * n); /* Read with Ack */
rsp_tt iicmb_cmd_read_nak(unsigned char * n); /* Read with Nak */
rsp_tt iicmb_cmd_start(void);                 /* Start         */
rsp_tt iicmb_cmd_stop(void);                  /* Stop          */
rsp_tt iicmb_cmd_set_bus(unsigned char n);    /* Set Bus       */


/* High-level operations: ****************************************************/

/* Read a single byte
 * Parameters:
 *    unsigned char    sa   -- I2C Slave address (7-bit)
 *    unsigned char    a    -- Byte address
 *    unsigned char *  d    -- Pointer to a storage for received data
 * Returns:
 *    rsp_tt                -- Response
 */
rsp_tt iicmb_read_bus(unsigned char sa, unsigned char a, unsigned char * d);

/* Read several bytes
 * Parameters:
 *    unsigned char    sa   -- I2C Slave address (7-bit)
 *    unsigned char    a    -- Byte address
 *    unsigned char *  d    -- Pointer to a storage for received data
 *    int              n    -- Number of bytes to read
 * Returns:
 *    rsp_tt                -- Response
 */
rsp_tt iicmb_read_bus_mul(unsigned char sa, unsigned char a, unsigned char * d, int n);


/* Write a single byte
 * Parameters:
 *    unsigned char    sa   -- I2C Slave address (7-bit)
 *    unsigned char    a    -- Byte address
 *    unsigned char    d    -- Data byte to write
 * Returns:
 *    rsp_tt                -- Response
 */
rsp_tt iicmb_write_bus(unsigned char sa, unsigned char a, unsigned char d);


/* Write several bytes
 * Parameters:
 *    unsigned char    sa   -- I2C Slave address (7-bit)
 *    unsigned char    a    -- Byte address
 *    unsigned char *  d    -- Pointer to a storage with data to write
 *    int              n    -- Number of bytes to write
 * Returns:
 *    rsp_tt                -- Response
 */
rsp_tt iicmb_write_bus_mul(unsigned char sa, unsigned char a, unsigned char * d, int n);

/* Report IICMB registers */
void iicmb_report_registers(FILE *fp);

#endif /* __IICMB_H__ */

