/***********************************************************************
 @copyright     : Siemens AG
 @license       : BSDv3
 @author        : Andreas Kaeberlein
 @address       : Clemens-Winkler-Strasse 3, 09116 Chemnitz

 @maintainer    : Andreas Kaeberlein
 @telephone     : +49 371 4810-2108
 @email         : andreas.kaeberlein@siemens.com

 @file          : iicmb.h
 @date          : June 6, 2022
 @see           : https://github.com/sshuv/iicmb

 @brief         : iicm driver
                  abstraction layer for IICM
***********************************************************************/



//--------------------------------------------------------------
// Define Guard
//--------------------------------------------------------------
#ifndef __IICMB_H
#define __IICMB_H


/**
 * @defgroup IICMB_ADDRESS base and register offsets
 *
 * IICMB base register and offset register defintion
 *
 * @{
 */
#ifndef IICMB_BASE
    #define IICMB_BASE      (0xF0000000)    /**<  Base address pointer          */
#endif
#define IICMB_CSR           (0x00)          /**<  Control/Status Register   R/W */
#define IICMB_DPR           (0x01)          /**<  Data/Parameter Register   R/W */
#define IICMB_CMDR          (0x02)          /**<  Command Register          R/W */
#define IICMB_FSMR          (0x03)          /**<  FSM States Register       RO  */
/** @} */



/**
 * @defgroup IICMB_CSR register bits
 *
 * Control/Status Register bit defintions
 *
 * @{
 */
#define IICMB_CSR_IICM_ENA  (0x80)      /**<  Enable. Effectively resets IICMB core.                                R/W */
#define IICMB_CSR_IRQ_ENA   (0x40)      /**<  Interrupt Enable.                                                     R/W */
#define IICMB_CSR_BB        (0x20)      /**<  Bus Busy. Indicates selected bus state.                               RO  */
#define IICMB_CSR_BC        (0x10)      /**<  Bus Captured. Indicates when IICMB has captured the selected bus.     RO  */
#define IICMB_CSR_BUS       (0x0F)      /**<  Bus ID. Indicates selected bus ID.                                    RO  */
/** @} */



/**
 * @defgroup IICMB_CMDR register bits
 *
 * Command Register defintions
 *
 * @{
 */
#define IICMB_CMD_WAIT      (0x00)      /**<  WO    Do nothing for specified amount of time in milliseconds */
#define IICMB_CMD_WRITE     (0x01)      /**<  WO    Transmit the byte given as a parameter */
#define IICMB_CMD_READ_ACK  (0x02)      /**<  WO    Receive a byte with acknowledge */
#define IICMB_CMD_READ_NAK  (0x03)      /**<  WO    Receive a byte with not-acknowledge */
#define IICMB_CMD_START     (0x04)      /**<  WO    If bus is not captured yet: issue Start Condition; If bus captured: issue Repeated Start Condition */
#define IICMB_CMD_STOP      (0x05)      /**<  WO    Issue Stop Condition and free selected bus */
#define IICMB_CMD_SET_BUS   (0x06)      /**<  WO    Connect to the specified bus (select bus) */

#define IICMB_RSP           (0xF0)      /**<        Bit Mask for selecting response Bits */
#define IICMB_RSP_COMPLETED (0x00)      /**<  RO    Command completed. */
#define IICMB_RSP_DONE      (0x80)      /**<  RO    Done. Indicates command completion */
#define IICMB_RSP_NAK       (0x40)      /**<  RO    Write command was not acknowledged */
#define IICMB_RSP_ARB_LOST  (0x20)      /**<  RO    Arbitration was lost during command execution */
#define IICMB_RSP_ERR       (0x10)      /**<  RO    Last command terminated with an error */
/** @} */



/**
 * @defgroup I2C_DIR
 *
 * Direction (Read or Write) of I2C communication
 *
 * @{
 */
#define IICMB_I2C_WR    (0x00)      /**<  I2C Write Bit */
#define IICMB_I2C_RD    (0x01)      /**<  I2C Read Bit */
/** @} */



/**
 * @defgroup IICMB_FSM
 *
 * State FSM
 *
 * @{
 */
#define IICMB_FSM_IDLE          (0x00)      /**<  Idle: Nothing to to */
#define IICMB_FSM_WT_IDLE       (0x01)      /**<  Idle: Wait for execution of stopbit */
#define IICMB_FSM_CL            (0xC0)      /**<  Class: FSM Class Bit selector, divides ISR FSM into Read/Write Path */
#define IICMB_FSM_CL_WR         (0x40)      /**<  Class: Write */
#define IICMB_FSM_WR_ADR_SET    (0x41)      /**<  Write: Slave Address */
#define IICMB_FSM_WR_ADR_CHK    (0x42)      /**<  Write: slave responsible? */
#define IICMB_FSM_WR_DAT        (0x43)      /**<  Write: Sent Databyte */
#define IICMB_FSM_CL_RD         (0x80)      /**<  Class: Read */
#define IICMB_FSM_RD_ADR_SET    (0x81)      /**<  Read: Write Slave Address */
#define IICMB_FSM_RD_ADR_CHK    (0x82)      /**<  Read: slave responsible? */
#define IICMB_FSM_RD_BYTE       (0x83)      /**<  Read: Read byte from slave */
/** @} */



/**
 *  @typedef t_iicmb_fsm;
 *
 *  @brief  FSM
 *
 *  Execution state of IICMB core
 *
 *  @since  2023-02-24
 *  @author Andreas Kaeberlein
 */
typedef enum
{
    IDLE,           /**<  Idle: Nothing to to */
    WT_IDLE,        /**<  Idle: Wait for execution of stopbit */
    WR_ADR_SET,     /**<  Write: Slave Address */
    WR_ADR_CHK,     /**<  Write: slave responsible? */
    WR_BYTE,        /**<  Write: Sent Databyte */
    RD_ADR_SET,     /**<  Read: Write Slave Address */
    RD_ADR_CHK,     /**<  Read: slave responsible? */
    RD_BYTE         /**<  Read: Read byte from slave */
} t_iicmb_fsm;



/**
 * @defgroup IICMB_ERO
 *
 * State FSM
 *
 * @{
 */
#define IICMB_OKAY              (0x00)      /**<  No Error */
#define IICMB_ERROR             (-1)        /**<  General Error */
#define IICMB_ERO_NOSLAVE       (0x01)      /**<  Slave not responding */
#define IICMB_ERO_ARBLOST       (0x02)      /**<  Arbitration lost */
#define IICMB_ERO_IICMB         (0x04)      /**<  Something went wrong on IICMB */
#define IICMB_ERO_TF_INC        (0x08)      /**<  Transfer incomplete */
#define IICMB_ERO_FSM           (0x40)      /**<  FSM Error */
#define IICMB_ERO_UNKNOWN       (0x80)      /**<  Unknown Error */
/** @} */


/**
 * @defgroup IICMB_RW
 *
 * Return values of iicm_write, iicm_read, iicm_wr_rd
 *
 * @{
 */
#define IICMB_OK            (0x00)      /**<  No Error */
#define IICMB_BUSY          (0x01)      /**<  No Error */
/** @} */



/** C++ compatibility **/
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



/**
 *  @typedef t_iicm_reg
 *
 *  @brief  IICMB Registers
 *
 *  IICMB Hardare Registers
 *
 *  @see https://github.com/sshuv/iicmb/blob/master/doc/iicmb_mb.pdf
 *
 *  @since  Novembre, 2022
 *  @author Andreas Kaeberlein
 */
typedef struct {
    uint8_t        CSR;    /**<  Control/Status Register   R/W */
    uint8_t        DPR;    /**<  Data/Parameter Register   R/W */
    uint8_t        CMDR;   /**<  Command Register          R/W */
    const uint8_t  FSMR;   /**<  FSM States Register       RO  */

} __attribute__((packed)) t_iicm_reg;



/**
 *  @typedef t_iicmb
 *
 *  @brief  IICMB driver handle
 *
 *  handles IICMB macro related information
 *  for data control and flow
 *
 *  @since  June 13, 2022
 *  @author Andreas Kaeberlein
 */
typedef struct t_iicmb {
    volatile t_iicm_reg*    iicmb;              /**<  pointer to IICMB hardware registers */
    uint8_t                 uint8FSM;           /**<  Soft I2C state machine @see IICMB_FSM */
    uint8_t                 uint8WrRd : 1;      /**<  Flag: Write/Read Interaction, allows to use first Write, then read part of FSM */
    uint8_t                 uint8Adr;           /**<  I2C slave address */
    uint8_t                 uint8Error;         /**<  Error in Transfer @see IICMB_ERO */
    uint16_t                uint16WrByteLen;    /**<  Total number of bytes to write */
    uint16_t                uint16WrByteIs;     /**<  Current number of bytes written */
    uint16_t                uint16RdByteLen;    /**<  Total number of bytes to read */
    uint16_t                uint16RdByteIs;     /**<  Current number of bytes readen */
    uint8_t*                uint8PtrData;       /**<  Read/Write data buffer */
} t_iicmb;



/** @brief set active bus
 *
 *  set active bus number
 *
 *  @param[in,out]  self                driver handle
 *  @param[in]      num                 active I2C bus number 0..15
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-10
 *  @author         Andreas Kaeberlein
 */
int iicmb_set_bus(t_iicmb *self, uint8_t num);



/** @brief init
 *
 *  init sw handle and IICMB core
 *
 *  @param[in,out]  self                driver handle
 *  @param[in,out]  iicmbAdr            base address of IICMB core
 *  @param[in,out]  bus                 default used I2C bus
 *  @return         int                 state
 *  @retval         0                   OK
 *  @retval         -1                  FAIL
 *  @since          2022-06-13
 *  @author         Andreas Kaeberlein
 */
int iicmb_init(t_iicmb *self, void* iicmbAdr, uint8_t bus);



/** @brief busy wait
 *
 *  busy wait for completion of last command
 *
 *  @return         int                 state
 *  @retval         0                   OK
 *  @since          2022-06-10
 *  @author         Andreas Kaeberlein
 */
int iicm_completion_busy_wait(void);



/** @brief FSM
 *
 *  IICMB state machine
 *
 *  @param[in,out]  this                storage element
 *  @return         void                state
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
void iicm_fsm(t_iicmb *this);



/** @brief busy
 *
 *  checks if the FSM is busy
 *
 *  @param[in,out]  this                storage element
 *  @return         int                 state
 *  @retval         0                   IDLE
 *  @retval         -1                  BUSY
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicm_busy(t_iicmb *this);



/** @brief busy
 *
 *  checks if the FSM is busy
 *
 *  @param[in,out]  this                storage element
 *  @return         int                 state
 *  @retval         IICMB_ERO_NO        All okay
 *  @retval         IICMB_ERO_NOSLAVE   Slave not responding on address
 *  @retval         IICMB_ERO_ARBLOST   Arbitration lost
 *  @retval         IICMB_ERO_IICMB     Something went wrong on IICMB
 *  @retval         IICMB_ERO_FSM       FSM Error, non designed path used
 *  @retval         IICMB_ERO_UNKNOWN   Unknown error on IICMB
 *  @since          2022-06-16
 *  @author         Andreas Kaeberlein
 */
int iicm_error(t_iicmb *this);



/** @brief write
 *
 *  write to I2C slave
 *
 *  @param[in,out]  this                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[in]      buf[]               data buffer
 *  @param[in]      len                 data buffer length
 *  @return         int                 state
 *  @retval         IICMB_OK            Next Transfer started
 *  @retval         IICMB_BUSY          Transfer active, wait for finish before next request
 *  @retval         IICMB_ERROR         IICMB FSM Crash, recovery routine necessary
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicm_write(t_iicmb *this, uint8_t adr7, uint8_t buf[], uint16_t len);



/** @brief read
 *
 *  read from I2C slave
 *
 *  @param[in,out]  this                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[out]     buf[]               data buffer
 *  @param[in]      len                 data buffer length
 *  @return         int                 state
 *  @retval         IICMB_OK            Next Transfer started
 *  @retval         IICMB_BUSY          Transfer active, wait for finish before next request
 *  @retval         IICMB_ERROR         IICMB FSM Crash, recovery routine necessary
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicm_read(t_iicmb *this, uint8_t adr7, uint8_t buf[], uint16_t len);



/** @brief write-read
 *
 *  perform I2C write, repeated start condition and I2C read
 *
 *  @param[in,out]  this                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[in]      wr[]                data written to I2C slave
 *  @param[in]      wrLen               number of written bytes
 *  @param[out]     rd[]                data readen from I2C slave
 *  @param[in]      rdLen               number of read requested bytes
 *  @return         int                 state
 *  @retval         IICMB_OK            Next Transfer started
 *  @retval         IICMB_BUSY          Transfer active, wait for finish before next request
 *  @retval         IICMB_ERROR         IICMB FSM Crash, recovery routine necessary
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicm_wr_rd(t_iicmb *this, uint8_t adr7, uint8_t wr[], uint16_t wrLen, uint8_t rd[], uint16_t rdLen);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __IICMB_H
