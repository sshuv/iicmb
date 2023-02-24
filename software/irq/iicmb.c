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
 *  @defgroup SFCB_PRINTF_EN
 *
 *  redirect sfcb_printf to printf
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
    self->iicmb = (volatile t_iicm_reg*) iicmbAdr;  // register set of IICMB


    self->uint8FSM = IICMB_FSM_IDLE;    // Soft I2C state machine
    self->uint8WrRd = 0;                // no write/read interaction requested
    self->uint8Error = IICMB_OKAY;      // no error in transfer
    self->uint16WrByteLen = 0;          // Total Number of Bytes to transfer
    self->uint16WrByteIs = 0;           // Number of Bytes processed (Sent/Receive)
    self->uint8PtrData = NULL;          // Read/Write Buffer Pointer

    /* init core */
    ret |= iicmb_disable(self);         // core disable
    ret |= iicmb_set_bus(self, bus);    // init with bus desired bus number
    ret |= iicmb_irq_enable(self);      // enable IRQs
    ret |= iicmb_enable(self);          // enable IICMB
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
void iicm_fsm(t_iicmb *this)
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
                    (void) iicmb_stop_bit(this);
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
                        (void) iicmb_stop_bit(this);
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
                        (void) iicmb_stop_bit(this);
                        break;  // leave byte write FSM
                    }
                    /* last byte sent */
                    if ( this->uint16WrByteIs == this->uint16WrByteLen ) {
                        /* last byte sent */
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        /* write-read access? */
                        if ( 0 != this->uint8WrRd ) {
                            this->uint8FSM = IICMB_FSM_RD_ADR_SET;  // go in FSM read path
                            (void) iicmb_start_bit(this);
                        } else {
                            this->uint8FSM = IICMB_FSM_WT_IDLE;     // last byte sent, go in idle
                            (void) iicmb_stop_bit(this);
                        }
                        break;  // leave, trigger with next IRQ
                    }
                    /* write next byte to IICMB */
                    (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR)) = this->uint8PtrData[this->uint16WrByteIs];
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
                    (void) iicmb_stop_bit(this);
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
                        (void) iicmb_stop_bit(this);
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
                    this->uint8PtrData[this->uint16RdByteIs] = (*(volatile uint8_t*) (IICMB_BASE+IICMB_DPR));
                    this->uint16RdByteIs = this->uint16RdByteIs + 1;
                    /* last byte sent */
                    if ( this->uint16RdByteIs == this->uint16RdByteLen ) {
                        /* last byte sent */
                        this->uint8FSM = IICMB_FSM_WT_IDLE;
                        (void) iicmb_stop_bit(this);

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
int iicm_busy(t_iicmb *this)
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
int iicm_error(t_iicmb *this)
{
    return (int) this->uint8Error;
}



/**
 *  iicm_write
 *    I2C write
 */
int iicm_write(t_iicmb *this, uint8_t adr7, uint8_t buf[], uint16_t len)
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
    this->uint8PtrData = buf;
    this->uint8WrRd = 0;    // only read is performed
    this->uint8FSM = IICMB_FSM_WR_ADR_SET;
    (void) iicmb_start_bit(this);    // sent start bit, triggers first IRQ
    /* normal end */
    return IICMB_OK;
}



/**
 *  iicm_read
 *    I2C read
 */
int iicm_read(t_iicmb *this, uint8_t adr7, uint8_t buf[], uint16_t len)
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
    this->uint8PtrData = buf;
    this->uint8WrRd = 0;    // only read is performed
    this->uint8FSM = IICMB_FSM_RD_ADR_SET;
    (void) iicmb_start_bit(this);    // sent start bit, triggers first IRQ
    /* normal end */
    return IICMB_OK;
}



/**
 *  iicm_wr_rd
 *    perform I2C write, repeated start condition and I2C read
 */
int iicm_wr_rd(t_iicmb *this, uint8_t adr7, uint8_t wr[], uint16_t wrLen, uint8_t rd[], uint16_t rdLen)
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
        this->uint8PtrData = wr;
    } else {
        this->uint8PtrData = NULL;
    }
    /* read requested? */
    this->uint16RdByteLen = rdLen;
    this->uint16RdByteIs = 0;
    if ( 0 != rdLen ) {
        this->uint8PtrData = rd;
    } else {
        this->uint8PtrData = NULL;
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
    (void) iicmb_start_bit(this);
    /* normal end */
    return IICMB_OK;
}
