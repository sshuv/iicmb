/*******************************************************************************
**                                                                             *
**    Project: IIC Multiple Bus Controller (IICMB)                             *
**                                                                             *
**    File:    IRQ driven C driver.                                            *
**    Version:                                                                 *
**             1.0,     March 25, 2023                                         *
**                                                                             *
**    Author:  Andreas Kaeberlein, (andreas.kaeberlein@siemens.com)            *
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



/** Includes **/
/* Standard libs */
#include <stdint.h>     // defines fixed data types: int8_t...
#include <stddef.h>     // various variable types and macros: size_t, offsetof, NULL, ...
/* Self */
#include "iicmb.h"  // related definitions



/**
 *  @defgroup IICMB_PRINTF_EN
 *
 *  redirect iicmb_printf to printf
 *
 *  @{
 */
#ifdef IICMB_PRINTF_EN
    #include <stdio.h>  // allow outputs in unit test
    #define iicmb_printf(...) printf(__VA_ARGS__)
#else
    #define iicmb_printf(...)
#endif
/** @} */   // DEBUG



/**
 *  @defgroup FALL_THROUGH
 *
 *  @brief Fallthrough
 *
 *  Defines fallthrough only for newer compiler
 *  avoids warning 'error: empty declaration __attribute__((fallthrough))'
 *
 *  @since  2023-02-24
 *  @see https://stackoverflow.com/questions/45349079/how-to-use-attribute-fallthrough-correctly-in-gcc
 */
#if defined(__GNUC__) && __GNUC__ >= 7
    #define FALL_THROUGH __attribute__ ((fallthrough))
#else
    #define FALL_THROUGH ((void)0)
#endif /* __GNUC__ >= 7 */
/** @} */   // FALL_THROUGH



/**
 *  @brief startbit
 *
 *  send start bit on i2c bus
 *
 *  @param[in,out]  self                driver handle
 *  @return         void
 *  @since          2023-02-24
 *  @author         Andreas Kaeberlein
 */
static inline void iicmb_start_bit(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* set start bit */
    self->iicmb->CMDR = IICMB_CMD_START;
}



/**
 *  @brief stopbit
 *
 *  send stop bit on i2c bus
 *
 *  @param[in,out]  self                driver handle
 *  @return         void
 *  @since          2023-02-24
 *  @author         Andreas Kaeberlein
 */
static inline void iicmb_stop_bit(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* set stop bit */
    self->iicmb->CMDR = IICMB_CMD_STOP;
}



/**
 *  @brief I2C enable
 *
 *  enables IICM core
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-09
 *  @author         Andreas Kaeberlein
 */
static int iicmb_enable(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* modify */
    self->iicmb->CSR |= IICMB_CSR_IICM_ENA; // enable core
    /* check */
    if ( 0 == (self->iicmb->CSR & IICMB_CSR_IICM_ENA) ) {
        return -1;
    }
    /* normal end */
    return 0;
}



/**
 *  @brief IICMB disable
 *
 *  disables IICM core
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-09
 *  @author         Andreas Kaeberlein
 */
static int iicmb_disable(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* modify */
    self->iicmb->CSR &= (uint8_t) ~IICMB_CSR_IICM_ENA;  // clear core enable
    /* check, for bit clear */
    if ( 0 != (self->iicmb->CSR & IICMB_CSR_IICM_ENA) ) {
        return -1;  // Error
    }
    /* graceful end */
    return 0;
}



/**
 *  @brief IRQ enable
 *
 *  enables IRQ at status register change
 *  cleared (reset to '0') by reading CMDR register.
 *  When a command is completed, one of the four status bits (DON, NAK, AL or ERR) becomes
 *  '1', depending on the completion results. In the same moment, the interrupt output
 *  is activated
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-09
 *  @author         Andreas Kaeberlein
 */
static int iicmb_irq_enable(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* modify */
    self->iicmb->CSR |= IICMB_CSR_IRQ_ENA;  // set IRQ enable bit
    /* check */
    if ( 0 == (self->iicmb->CSR & IICMB_CSR_IRQ_ENA) ) {
        return -1;
    }
    /* normal end */
    return 0;
}



/**
 *  @brief IRQ disable
 *
 *  disables IRQ on status register change
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-10
 *  @author         Andreas Kaeberlein
 */
static int iicmb_irq_disable(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* modify */
    self->iicmb->CSR &= (uint8_t) ~IICMB_CSR_IRQ_ENA;   // clear IRQ enable bit
    /* check */
    if ( 0 != (self->iicmb->CSR & IICMB_CSR_IRQ_ENA) ) {
        return -1;
    }
    /* graceful end */
    return 0;
}



/**
 *  @brief Satus decode
 *
 *  Decodes IICMB last transfer status
 *
 *  @param[in,out]  self                driver handle
 *  @param[in]      cmdReg              read command register value
 *  @return         int                 state
 *  @retval         0                   OKAY
 *  @retval         -1                  FAIL, leave ISR with error
 *  @since          2023-02-25
 *  @author         Andreas Kaeberlein
 */
static int iicmb_status_decode(t_iicmb *self, uint8_t cmdReg)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* Check for Eros in last transfer */
    switch (cmdReg & IICMB_RSP) {
        /* normal: last command done */
        case IICMB_RSP_DONE:
            iicmb_printf("  INFO:CMDR: done\n");
            break;
        /* normal: last byte nck */
        case IICMB_RSP_NAK:
            iicmb_printf("  INFO:CMDR: data NCK\n");
            break;
        /* exception: arbitration lost */
        case IICMB_RSP_ARB_LOST:
            iicmb_printf("  ERROR:CMDR: arbitration lost\n");
            self->error = IICMB_E_ARBLOST;  // arbitration lost
            self->fsm = IICMB_WT_IDLE;
            return -1;
        /* exception: IICMB unknown error */
        case IICMB_RSP_ERR:
            iicmb_printf("  ERROR:CMDR: IICMB unkown error\n");
            self->error = IICMB_E_IICMB;    // I2C controller runs into error
            self->fsm = IICMB_IDLE;
            return -1;
        /* all okay */
        default:
            iicmb_printf("  ERROR:CMDR: soft FSM unknown error\n");
            self->error = IICMB_E_UNKNOWN;  // unknown error in IICMB
            return -1;
    }
    /* normal end */
    return 0;
}



/**
 *  iicmb_set_bus
 *    set bus number
 */
int iicmb_set_bus(t_iicmb *self, uint8_t num)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* set bus number */
    self->iicmb->DPR = (uint8_t) (num & IICMB_CSR_BUS); // preload data/parameter register
    self->iicmb->CMDR = IICMB_CMD_SET_BUS;              // set bus id
    /* check for setting */
    if ( num != (self->iicmb->CSR & IICMB_CSR_BUS) ) {
        return -1;
    }
    /* graceful end */
    return 0;
}


/**
 *  iicmb_init
 *    init sw handle and IICMB core
 */
int iicmb_init(t_iicmb *self, void* iicmbAdr, uint8_t bus)
{
    /** Variables **/
    int ret = 0;    // common return value

    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* Init driver handle */
    self->iicmb = (t_iicm_reg*) iicmbAdr;   // register set of IICMB
    self->fsm = IICMB_IDLE;     // Soft I2C state machine
    self->uint8WrRd = 0;        // no write/read interaction requested
    self->error = IICMB_E_NO;   // Driver runs without error
    self->uint16WrByteLen = 0;  // Total Number of Bytes to transfer
    self->uint16WrByteIs = 0;   // Number of Bytes processed (Sent/Receive)
    self->uint8PtrData = NULL;  // Read/Write Buffer Pointer
    /* init core */
    ret |= iicmb_disable(self);         // core disable
    ret |= iicmb_set_bus(self, bus);    // init with bus desired bus number
    ret |= iicmb_irq_enable(self);      // enable IRQs
    ret |= iicmb_enable(self);          // enable IICMB
    /* end */
    return ret;
}


/**
 *  iicmb_close
 *    make driver invalid
 */
int iicmb_close(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* Disable IRQ */
    iicmb_irq_disable(self);    // mask all IRQs
    iicmb_disable(self);        // core disable
    /* make handle invalid */
    self->iicmb = NULL; // invalid register handle
    /* end */
    return 0;
}


/**
 *  iicmb_busy_wait
 *    wait for completion of last command with busy wait
 */
int iicmb_busy_wait(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* poll until completion */
    uint8_t uint8Rsp = (uint8_t) ~IICMB_RSP;
    while ( IICMB_RSP_COMPLETED == (uint8Rsp & IICMB_RSP) ) {
        uint8Rsp = self->iicmb->CMDR;
    }
    /* graceful end */
    return 0;
}


/**
 *  iicmb_fsm()
 *    IICMB fsm, triggered by ISR
 */
void iicmb_fsm(t_iicmb *self)
{
    /* Function call message */
    iicmb_printf("__FUNCTION__ = %s\n", __FUNCTION__);
    /* read command register */
    uint8_t uint8CmdReg = self->iicmb->CMDR;    // clears IRQ, and read data
    /* read/write/idle */
    switch (self->fsm) {
        /*
         *  IDLE States
         *    nothing to do
         */
        case IICMB_IDLE:
            iicmb_status_decode(self, uint8CmdReg);
            return; // clears only last IRQ after stopbit
        /* stop bit succesfull sent? */
        case IICMB_WT_IDLE:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error leave
            }
            /* check for complete transfer */
            if ( !((self->uint16WrByteLen == self->uint16WrByteIs) && (self->uint16RdByteLen == self->uint16RdByteIs)) ) {
                self->error = IICMB_E_ICTF; // transfer not complete
            }
            self->fsm = IICMB_IDLE; // transfer done
            /* leave */
            return;
        /*
         *  WRITE States
         *    sent slave address
         */
        case IICMB_WR_ADR_SET:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error exit
            }
            /* iicm_byte_write */
            self->iicmb->DPR = (uint8_t) (self->uint8Adr | IICMB_I2C_WR);   // assemble write address
            self->iicmb->CMDR = IICMB_CMD_WRITE;    // ICMB command
            /* Update FSM */
            self->fsm = IICMB_WR_ADR_CHK;   // go one with data transfer
            return; // wait for next IRQ
        /* slave responsible? */
        case IICMB_WR_ADR_CHK:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error exit
            }
            /* slave address ack? */
            if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                self->error = IICMB_E_NOSLAVE;  // NCK on address byte
                self->fsm = IICMB_WT_IDLE;
                (void) iicmb_stop_bit(self);
                return;
            }
            self->fsm = IICMB_WR_BYTE;  // go one with data transfer
            FALL_THROUGH;   // slave respobsible, write first data byte
        /* data transmission */
        case IICMB_WR_BYTE:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error exit
            }
            /* last byte succesfull? */
            if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                /* prepare IDLE on bus */
                self->fsm = IICMB_WT_IDLE;
                (void) iicmb_stop_bit(self);
                return;  // leave byte write FSM
            }
            /* last byte sent */
            if ( self->uint16WrByteIs == self->uint16WrByteLen ) {
                /* last byte sent */
                self->fsm = IICMB_WT_IDLE;
                /* write-read access? */
                if ( 0 != self->uint8WrRd ) {
                    self->fsm = IICMB_RD_ADR_SET;   // go in FSM read path
                    (void) iicmb_start_bit(self);
                } else {
                    self->fsm = IICMB_WT_IDLE;  // last byte sent, go in idle
                    (void) iicmb_stop_bit(self);
                }
                return; // leave, trigger with next IRQ
            }
            /* write next byte to IICMB */
            self->iicmb->DPR = (self->uint8PtrData)[self->uint16WrByteIs];
            self->iicmb->CMDR = IICMB_CMD_WRITE;
            /* data pointer update */
            ++(self->uint16WrByteIs);
            return; // leave, trigger with next IRQ
        /*
         *  READ States
         *    sent slave address
         */
        /* Read: sent slave address */
        case IICMB_RD_ADR_SET:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error exit
            }
            /* iicm_byte_write */
            self->iicmb->DPR = (uint8_t) (self->uint8Adr | IICMB_I2C_RD);  // assemble read address
            self->iicmb->CMDR = IICMB_CMD_WRITE;    // IICMB command
            /* update FSM */
            self->fsm = IICMB_RD_ADR_CHK;   // check slave is responsible
            return; // leave, trigger with next IRQ
        /* Read: slave responsible? */
        case IICMB_RD_ADR_CHK:
            /* IICMB encoutered error? */
            if ( 0 != iicmb_status_decode(self, uint8CmdReg) ) {
                return; // error exit
            }
            /* Slave Not responsible */
            if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                self->error = IICMB_E_NOSLAVE;  // NCK on address byte
                self->fsm = IICMB_WT_IDLE;
                (void) iicmb_stop_bit(self);
                return;
            }
            /* next state read byte */
            self->fsm = IICMB_RD_BYTE;
            /* Request =1Byte */
            if ( 1 == self->uint16RdByteLen ) { // only one byte requested, read NCK
                self->iicmb->CMDR = IICMB_CMD_READ_NAK;
                return; // leave ISR, wait for transfer
            }
            /* Request >1Byte */
            self->iicmb->CMDR = IICMB_CMD_READ_ACK;
            return; // leave ISR, wait for transfer
        /* Read: Byte Request */
        case IICMB_RD_BYTE:
            /* capture value */
            (self->uint8PtrData)[self->uint16RdByteIs] = self->iicmb->DPR;
            ++(self->uint16RdByteIs);
            /* last byte sent */
            if ( self->uint16RdByteIs == self->uint16RdByteLen ) {
                /* last byte sent */
                self->fsm = IICMB_WT_IDLE;
                (void) iicmb_stop_bit(self);
                return; // leave ISR
            }
            /* More Bytes Pending, Read with ACK */
            if ( (self->uint16RdByteIs) < ((self->uint16RdByteLen)-1) ) {
                self->iicmb->CMDR = IICMB_CMD_READ_ACK;
                return; // leave ISR, wait for transfer
            }
            /* Last Byte Pending, Read with NCK */
            self->iicmb->CMDR = IICMB_CMD_READ_NAK;
            return; // leave ISR, trigger with next IRQ
        /* something unexpedted */
        default:
            self->error = IICMB_E_FSM;  // non designed path of FSM used
            return;
    }
    return; // leave ISR
}



/**
 *  iicmb_busy
 *    checks if IICMB FSM is active
 */
int iicmb_busy(t_iicmb *self)
{
    if ( IICMB_IDLE == self->fsm ) {
        return 0;
    }
    return -1;
}



/**
 *  icmb_is_error
 *    return ero code
 */
int iicmb_is_error(t_iicmb *self)
{
    if ( IICMB_E_NO == self->error ) {
        return 0;
    }
    return -1;
}



/**
 *  iicm_write
 *    I2C write
 */
int iicmb_write(t_iicmb *self, uint8_t adr7, void* data, uint16_t len)
{
    /* check for empty data set */
    if ( 0 == len ) {
        return IICMB_EXIT_OK;
    }
    /* check for active transfer */
    if ( IICMB_IDLE != self->fsm ) {
        return IICMB_EXIT_BUSY; // iicmb is busy with last request
    }
    /* set-up next request */
    self->error = IICMB_E_NO;
    self->uint8Adr = (uint8_t) (adr7 << 1); // prepare address for Read/Write bit set
    self->uint16WrByteLen = len;
    self->uint16WrByteIs = 0;
    self->uint8PtrData = (uint8_t*) data;
    self->uint8WrRd = 0;    // only read is performed
    self->fsm= IICMB_WR_ADR_SET;
    /* check for free I2C bus */
    if ( 0 == (self->iicmb->CSR & IICMB_CSR_BB) ) { // bus is free
        (void) iicmb_start_bit(self);   // sent start bit, triggers first IRQ
        return IICMB_EXIT_OK;   // normale end
    }
    /* bus not free, wait is needed */
    self->fsm = IICMB_IDLE;
    return IICMB_EXIT_I2C_OCC;  // i2c by other master occupied
}



/**
 *  iicm_read
 *    I2C read
 */
int iicmb_read(t_iicmb *self, uint8_t adr7, void* data, uint16_t len)
{
    /* check for empty data set */
    if ( 0 == len ) {
        return IICMB_EXIT_OK;
    }
    /* check for active transfer */
    if ( IICMB_IDLE != self->fsm ) {
        return IICMB_EXIT_BUSY; // iicmb is busy with last request
    }
    /* set-up next request */
    self->error = IICMB_E_NO;
    self->uint8Adr = (uint8_t) (adr7 << 1);
    self->uint16RdByteLen = len;
    self->uint16RdByteIs = 0;
    self->uint8PtrData = (uint8_t*) data;
    self->uint8WrRd = 0;    // only read is performed
    self->fsm = IICMB_RD_ADR_SET;
    /* check for free I2C bus */
    if ( 0 == (self->iicmb->CSR & IICMB_CSR_BB) ) { // bus is free
        (void) iicmb_start_bit(self);   // sent start bit, triggers first IRQ
        return IICMB_EXIT_OK;   // normal end
    }
    /* bus not free, wait is needed */
    self->fsm = IICMB_IDLE;
    return IICMB_EXIT_I2C_OCC;  // i2c by other master occupied
}



/**
 *  iicmb_wr_rd
 *    perform I2C write, repeated start condition and I2C read
 */
int iicmb_wr_rd(t_iicmb *self, uint8_t adr7, void* data, uint16_t wrLen, uint16_t rdLen)
{
    /* check for empty data set */
    if ( (0 == wrLen) && (0 == rdLen) ) {
        return IICMB_EXIT_OK;
    }
    /* check for active transfer */
    if ( IICMB_IDLE != self->fsm ) {
        return IICMB_EXIT_BUSY; // iicmb is busy with last request
    }
    /* except only write-read transfers, otherwise use dedicated function */
    if ( !((0 != wrLen) && (0 != rdLen)) ) {
        return IICMB_EXIT_ERROR;
    }
    /* set-up next request */
    self->error = IICMB_E_NO;
    self->uint8Adr = (uint8_t) (adr7 << 1); // define address
    /* write/read request */
    self->uint8PtrData = (uint8_t*) data;
    self->uint16WrByteLen = wrLen;
    self->uint16WrByteIs = 0;
    self->uint16RdByteLen = rdLen;
    self->uint16RdByteIs = 0;
    self->uint8WrRd = 1;    // read after write is performed
    self->fsm = IICMB_WR_ADR_SET;
    /* check for free I2C bus */
    if ( 0 == (self->iicmb->CSR & IICMB_CSR_BB) ) { // bus is free
        (void) iicmb_start_bit(self);   // sent start bit, triggers first IRQ
        return IICMB_EXIT_OK;   // normale end
    }
    /* bus not free, wait is needed */
    self->fsm = IICMB_IDLE;
    return IICMB_EXIT_I2C_OCC;  // i2c by other master occupied
}
