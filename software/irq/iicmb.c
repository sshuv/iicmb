/***********************************************************************
 @copyright     : Siemens AG
 @license       : BSDv3
 @author        : Andreas Kaeberlein
 @address       : Clemens-Winkler-Strasse 3, 09116 Chemnitz

 @maintainer    : Andreas Kaeberlein
 @telephone     : +49 371 4810-2108
 @email         : andreas.kaeberlein@siemens.com

 @file          : iicmb.c
 @date          : June 6, 2022

 @brief         : iicm driver
                  abstraction layer for IICM
***********************************************************************/



/** Includes **/
/* Standard libs */
#include <stdint.h>     // defines fixed data types: int8_t...
#include <stddef.h>     // various variable types and macros: size_t, offsetof, NULL, ...
/* Self */
#include "iicmb.h"  // related definitions





/**
 *  iicm_start_bit()
 *    send start bit on i2c bus
 */
inline void iicm_start_bit(void)
{
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_START;
}


/**
 *  iicm_stop_bit()
 *    send stop bit on i2c bus
 */
inline void iicm_stop_bit(void)
{
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_STOP;
}


/**
 *  iicm_enable()
 *    enables I2C core
 */
int iicm_enable(void)
{
    /* enable core */
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) |= IICMB_CSR_IICM_ENA;
    /* check */
    if ( IICMB_CSR_IICM_ENA != ((*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) & IICMB_CSR_IICM_ENA) ) {
        return IICMB_ERROR;
    }
    /* normal end */
    return IICMB_OKAY;
}


/**
 *  iicm_disable()
 *    disables I2C core
 */
int iicm_disable(void)
{
    /* enable core */
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) &= ~IICMB_CSR_IICM_ENA;
    /* check */
    if ( IICMB_CSR_IICM_ENA != ((*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) & IICMB_CSR_IICM_ENA) ) {
        return IICMB_ERROR;
    }
    /* graceful end */
    return IICMB_OKAY;
}


/**
 *  iicm_irq_enable()
 *    enables IRQ
 */
int iicm_irq_enable(void)
{
    /* enable core */
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) |= IICMB_CSR_IRQ_ENA;
    /* check */
    if ( IICMB_CSR_IRQ_ENA != ((*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) & IICMB_CSR_IRQ_ENA) ) {
        return IICMB_ERROR;
    }
    /* normal end */
    return IICMB_OKAY;
}


/**
 *  iicm_irq_disable()
 *    disables IRQ
 */
int iicm_irq_disable(void)
{
    /* enable core */
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) &= ~IICMB_CSR_IRQ_ENA;
    /* check */
    if ( IICMB_CSR_IRQ_ENA != ((*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) & IICMB_CSR_IRQ_ENA) ) {
        return IICMB_ERROR;
    }
    /* graceful end */
    return IICMB_OKAY;
}


/**
 *  iicmb_set_bus
 *    set bus number
 */
int iicmb_set_bus(uint8_t busNum)
{
    /* set bus number */
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR))   = (busNum & IICMB_CSR_BUS); // preload data register
    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR))  = IICMB_CMD_SET_BUS;        // set bus id
    /* check for setting */
    if ( (busNum & IICMB_CSR_BUS) != ((*(volatile uint8_t*) (IICMB_BASE+IICMB_CSR)) & IICMB_CSR_BUS) ) {
        return IICMB_ERROR;
    }
    /* graceful end */
    return IICMB_OKAY;
}


/**
 *  iicm_init
 *    init sw handle and IICMB core
 */
int iicm_init(t_iicm *this)
{
    /** Variables **/
    int ret = 0;    // common return value

    /* Init driver handle */
    this->uint8FSM = IICMB_FSM_IDLE;    // Soft I2C state machine
    this->uint8WrRd = 0;                // no write/read interaction requested
    this->uint8Error = IICMB_OKAY;      // no error in transfer
    this->uint16WrByteLen = 0;          // Total Number of Bytes to transfer
    this->uint16WrByteIs = 0;           // Number of Bytes processed (Sent/Receive)
    this->uint8PtrWrBuf = NULL;         // Write Buffer Pointer
    this->uint8PtrRdBuf = NULL;         // Read Buffer Pointer
    this->regPtr = NULL;                // IICMB handle not mapped

    /* init core */
    ret |= iicm_disable();      // core disable
    ret |= iicmb_set_bus(0);    // default bus 0
    ret |= iicm_irq_enable();   // enable IRQs
    ret |= iicm_enable();       // enable IICMB

    /* end */
    return ret;
}


/**
 *  iicm_completion_busy_wait()
 *    wait for completion of last command with busy wait
 */
int iicm_completion_busy_wait(void)
{
    /** Variables **/
    uint8_t uint8Rsp;

    /* poll until completion */
    uint8Rsp = (uint8_t) ~IICMB_RSP;
    while ( IICMB_RSP_COMPLETED == (uint8Rsp & IICMB_RSP) ) {
        uint8Rsp = (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR));
    }
    /* graceful end */
    return IICMB_OKAY;
}


/**
 *  iicm_fsm()
 *    IICMB fsm, triggered by ISR
 */
void iicm_fsm(t_iicm *this)
{
    /** Variables **/
    uint8_t     uint8CmdReg;


    /* read command register */
    uint8CmdReg = (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR));   // clears IRQ

    /* read/write/idle */
    switch (this->uint8FSM & IICMB_FSM_CL) {
        /* idle category */
        case IICMB_FSM_IDLE:
            /* Idle: in idle or wait for stop */
            switch (this->uint8FSM) {
                /* stop bit succesfull sent? */
                case IICMB_FSM_WT_IDLE:
                    switch (uint8CmdReg & IICMB_RSP) {
                        /* All Done */
                        case IICMB_RSP_DONE:
                            /* check for complete transfer */
                            if ( (this->uint16WrByteLen == this->uint16WrByteIs) || (this->uint16RdByteLen == this->uint16RdByteIs) ) {
                                this->uint8Error &= (uint8_t) ~IICMB_ERO_TF_INC;    // clear incomplete flag
                            }
                            break;
                        case IICMB_RSP_ERR:
                            this->uint8Error |= IICMB_ERO_IICMB;
                            break;
                        default:
                            this->uint8Error |= IICMB_ERO_UNKNOWN;
                            break;
                    }
                    this->uint8FSM = IICMB_FSM_IDLE;
                    return; // leave ISR
                /* nothing to do */
                case IICMB_FSM_IDLE:
                    break;  // clears only last IRQ after stopbit
                /* something unexpedted */
                default:
                    this->uint8Error |= IICMB_ERO_FSM;  // non designed path of FSM used
                    break;
            }
            return; // leave ISR
        /* write category */
        case IICMB_FSM_CL_WR:
            /* Check for Eros in last transfer */
            switch (uint8CmdReg & IICMB_RSP) {
                /* go one with normal FSM */
                case IICMB_RSP_DONE: break; // last command done
                case IICMB_RSP_NAK: break;  // last byte nck
                /* exception: arbitration lost */
                case IICMB_RSP_ARB_LOST:
                    this->uint8Error |= IICMB_ERO_ARBLOST;  // arbitration lost
                    this->uint8FSM = IICMB_FSM_WT_IDLE;
                    (void) iicm_stop_bit();
                    return;
                /* exception: IICMB unknown error */
                case IICMB_ERO_IICMB:
                    this->uint8Error |= IICMB_ERO_IICMB;    // arbitration lost
                    this->uint8FSM = IICMB_FSM_IDLE;
                    return;
                /* all okay */
                default:
                    this->uint8Error |= IICMB_ERO_UNKNOWN;  // unknown error in IICMB
                    return;
            }
            /* Byte Write FSM */
            switch (this->uint8FSM) {
                /* sent slave address */
                case IICMB_FSM_WR_ADR_SET:
                    /* iicm_byte_write */
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR)) = (uint8_t) (this->uint8Adr | IICMB_I2C_WR);  // assemble write address
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_WRITE;   // ICMB command
                    /* Update FSM */
                    this->uint8FSM = IICMB_FSM_WR_ADR_CHK;      // go one with data transfer
                    break;;                                     // wait for next IRQ
                /* slave responsible? */
                case IICMB_FSM_WR_ADR_CHK:
                    if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                        this->uint8Error |= IICMB_ERO_NOSLAVE;  // NCK on address byte
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        (void) iicm_stop_bit();
                        break;
                    }
                    this->uint8FSM = IICMB_FSM_WR_DAT;  // go one with data transfer
                    __attribute__ ((fallthrough));      // slave respobsible, write first data byte
                /* data transmission */
                case IICMB_FSM_WR_DAT:
                    /* last byte succesfull? */
                    if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                        /* prepare IDLE on bus */
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        (void) iicm_stop_bit();
                        break;  // leave byte write FSM
                    }
                    /* last byte sent */
                    if ( this->uint16WrByteIs == this->uint16WrByteLen ) {
                        /* last byte sent */
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        /* write-read access? */
                        if ( 0 != this->uint8WrRd ) {
                            this->uint8FSM = IICMB_FSM_RD_ADR_SET;  // go in FSM read path
                            (void) iicm_start_bit();
                        } else {
                            this->uint8FSM = IICMB_FSM_WT_IDLE;     // last byte sent, go in idle
                            (void) iicm_stop_bit();
                        }
                        break;  // leave, trigger with next IRQ
                    }
                    /* write next byte to IICMB */
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR)) = this->uint8PtrWrBuf[this->uint16WrByteIs];
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_WRITE;
                    /* data pointer update */
                    this->uint16WrByteIs = this->uint16WrByteIs + 1;
                    break;
                /* something strange happend */
                default:
                    this->uint8Error |= IICMB_ERO_FSM;  // non designed path of FSM used
                    break;
            }
            return; // leave ISR, called with next IRQ
        /* read category */
        case IICMB_FSM_CL_RD:
            /* Check for Eros in last transfer */
            switch (uint8CmdReg & IICMB_RSP) {
                /* go one with normal FSM */
                case IICMB_RSP_DONE: break; // last command done
                case IICMB_RSP_NAK: break;  // last byte nck
                /* exception: arbitration lost */
                case IICMB_RSP_ARB_LOST:
                    this->uint8Error |= IICMB_ERO_ARBLOST;  // arbitration lost
                    this->uint8FSM = IICMB_FSM_WT_IDLE;
                    (void) iicm_stop_bit();
                    return;
                /* exception: IICMB unknown error */
                case IICMB_ERO_IICMB:
                    this->uint8Error |= IICMB_ERO_IICMB;    // arbitration lost
                    this->uint8FSM = IICMB_FSM_IDLE;
                    return;
                /* all okay */
                default:
                    this->uint8Error |= IICMB_ERO_UNKNOWN;  // unknown error in IICMB
                    return;
            }
            /* Byte Read FSM */
            switch (this->uint8FSM) {
                /* Read: sent slave address */
                case IICMB_FSM_RD_ADR_SET:
                    /* iicm_byte_write */
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR)) = (uint8_t) (this->uint8Adr | IICMB_I2C_RD);  // assemble read address
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_WRITE;   // ICMB command
                    /* update FSM */
                    this->uint8FSM = IICMB_FSM_RD_ADR_CHK;      // check slave is responsible
                    break;
                /* Read: slave responsible? */
                case IICMB_FSM_RD_ADR_CHK:
                    /* Slave Not responsible */
                    if ( IICMB_RSP_NAK == (uint8CmdReg & IICMB_RSP) ) {
                        this->uint8Error |= IICMB_ERO_NOSLAVE;  // NCK on address byte
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        (void) iicm_stop_bit();
                        break;
                    }
                    /* next state read byte */
                    this->uint8FSM = IICMB_FSM_RD_BYTE;
                    /* Request =1Byte */
                    if ( 1 == this->uint16RdByteLen ) { // only one byte requested, read NCK
                        (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_READ_NAK;
                        break;  // leave ISR, wait for transfer
                    }
                    /* Request >1Byte */
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_READ_ACK;
                    break; // leave ISR, wait for transfer
                /* Read: Byte Request */
                case IICMB_FSM_RD_BYTE:
                    /* capture value */
                    this->uint8PtrRdBuf[this->uint16RdByteIs] = (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR));
                    this->uint16RdByteIs = this->uint16RdByteIs + 1;
                    /* last byte sent */
                    if ( this->uint16RdByteIs == this->uint16RdByteLen ) {
                        /* last byte sent */
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        (void) iicm_stop_bit();

                        break; // leave ISR
                    }
                    /* More Bytes Pending, Read with ACK */
                    if ( (this->uint16RdByteIs) < (this->uint16RdByteLen-1) ) {
                        (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_READ_ACK;
                    /* Last Byte Pending, Read with NCK */
                    } else {
                        (*(volatile uint8_t*) (IICMB_BASE+IICMB_CMDR)) = IICMB_CMD_READ_NAK;
                    }
                    break;  // wait for byte transfer on mext IRQ
                /* something strange happend */
                default:
                    this->uint8Error |= IICMB_ERO_FSM;  // non designed path of FSM used
                    break;
            }
            return; // leave ISR, called with next IRQ
        /* default */
        default:
            this->uint8Error |= IICMB_ERO_FSM;  // non designed path of FSM used
            break;
    }
    /* normal end */
    return;
}



/**
 *  iicm_busy
 *    checks if IICMB FSM is active
 */
int iicm_busy(t_iicm *this)
{
    if ( IICMB_FSM_IDLE == this->uint8FSM ) {
        return IICMB_OKAY;
    }
    return IICMB_ERROR;
}



/**
 *  iicm_error
 *    return ero code
 */
int iicm_error(t_iicm *this)
{
    return (int) this->uint8Error;
}



/**
 *  iicm_write
 *    I2C write
 */
int iicm_write(t_iicm *this, uint8_t adr7, uint8_t buf[], uint16_t len)
{
    /* check for empty data set */
    if ( 0 == len ) {
        return IICMB_OK;
    }
    /* check for eros and wip */
    switch (this->uint8FSM) {
        case IICMB_FSM_IDLE: break;
        default: return IICMB_BUSY;
    }
    /* set-up next request */
    this->uint8Error = IICMB_ERO_TF_INC;    // set transfer as incomplete, cleared at end of succesfull transfer
    this->uint8Adr = (uint8_t) (adr7 << 1); // prepare address for Read/Write bit set
    this->uint16WrByteLen = len;
    this->uint16WrByteIs = 0;
    this->uint8PtrWrBuf = buf;
    this->uint8WrRd = 0;    // only read is performed
    this->uint8FSM = IICMB_FSM_WR_ADR_SET;
    (void) iicm_start_bit();    // sent start bit, triggers first IRQ
    /* normal end */
    return IICMB_OK;
}



/**
 *  iicm_read
 *    I2C read
 */
int iicm_read(t_iicm *this, uint8_t adr7, uint8_t buf[], uint16_t len)
{
    /* check for empty data set */
    if ( 0 == len ) {
        return IICMB_OK;
    }
    /* check for eros and wip */
    switch (this->uint8FSM) {
        case IICMB_FSM_IDLE: break;
        default: return IICMB_BUSY;
    }
    /* set-up next request */
    this->uint8Error = IICMB_ERO_TF_INC;    // set transfer as incomplete, cleared at end of succesfull transfer
    this->uint8Adr = (uint8_t) (adr7 << 1);
    this->uint16RdByteLen = len;
    this->uint16RdByteIs = 0;
    this->uint8PtrRdBuf = buf;
    this->uint8WrRd = 0;    // only read is performed
    this->uint8FSM = IICMB_FSM_RD_ADR_SET;
    (void) iicm_start_bit();    // sent start bit, triggers first IRQ
    /* normal end */
    return IICMB_OK;
}



/**
 *  iicm_wr_rd
 *    perform I2C write, repeated start condition and I2C read
 */
int iicm_wr_rd(t_iicm *this, uint8_t adr7, uint8_t wr[], uint16_t wrLen, uint8_t rd[], uint16_t rdLen)
{
    /* check for empty data set */
    if ( (0 == wrLen) && (0 == rdLen) ) {
        return IICMB_OK;
    }
    /* check for eros and wip */
    switch (this->uint8FSM) {
        case IICMB_FSM_IDLE: break;
        default: return IICMB_BUSY;
    }
    /* set-up next request */
    this->uint8Error = IICMB_ERO_TF_INC;    // set transfer as incomplete, cleared at end of succesfull transfer
    this->uint8Adr = (uint8_t) (adr7 << 1); // define address
    /* write requested? */
    this->uint16WrByteLen = wrLen;
    this->uint16WrByteIs = 0;
    if ( 0 != wrLen ) {
        this->uint8PtrWrBuf = wr;
    } else {
        this->uint8PtrWrBuf = NULL;
    }
    /* read requested? */
    this->uint16RdByteLen = rdLen;
    this->uint16RdByteIs = 0;
    if ( 0 != rdLen ) {
        this->uint8PtrRdBuf = rd;
    } else {
        this->uint8PtrRdBuf = NULL;
    }
    /* determine type of interaction */
    if ( 0 != wrLen ) {
        this->uint8FSM = IICMB_FSM_WR_ADR_SET;
        if ( 0 != rdLen ) {
            this->uint8WrRd = 1;    // read after write is performed
        } else {
            this->uint8WrRd = 0;    // only write is performed
        }
    } else {
        this->uint8FSM = IICMB_FSM_RD_ADR_SET;  // start with read
        this->uint8WrRd = 0;    // only read is performed
    }
    /* sent start bit, triggers first IRQ */
    (void) iicm_start_bit();
    /* normal end */
    return IICMB_OK;
}
