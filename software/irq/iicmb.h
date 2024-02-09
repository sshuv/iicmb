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



//--------------------------------------------------------------
// Define Guard
//--------------------------------------------------------------
#ifndef __IICMB_H
#define __IICMB_H


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
 * @defgroup I2C_SW_FUNC
 *
 * I2C driver function exit codes
 *
 * @{
 */
#define IICMB_EXIT_OK       (0)     /**<  Function ended normally */
#define IICMB_EXIT_BUSY     (1<<0)  /**<  IICMB busy with last request, please wait */
#define IICMB_EXIT_OCC      (1<<1)  /**<  Occupied by another master, please wait */

#define IICMB_EXIT_ERROR    (-1)    /**<  Something went wrong */
/** @} */




/** C++ compatibility **/
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



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
    IICMB_IDLE,         /**<  Idle: Nothing to to */
    IICMB_WT_IDLE,      /**<  Idle: Wait for execution of stopbit */
    IICMB_WR_ADR_SET,   /**<  Write: Slave Address */
    IICMB_WR_ADR_CHK,   /**<  Write: slave responsible? */
    IICMB_WR_BYTE,      /**<  Write: Sent Databyte */
    IICMB_RD_ADR_SET,   /**<  Read: Write Slave Address */
    IICMB_RD_ADR_CHK,   /**<  Read: slave responsible? */
    IICMB_RD_BYTE       /**<  Read: Read byte from slave */
} t_iicmb_fsm;



/**
 *  @typedef t_iicmb_ero;
 *
 *  @brief  Errors
 *
 *  Encoutered errors while execution
 *
 *  @since  2023-02-24
 *  @author Andreas Kaeberlein
 */
typedef enum
{
    IICMB_E_NO,         /**<  No Error */
    IICMB_E_NOSLAVE,    /**<  Slave not responding */
    IICMB_E_ARBLOST,    /**<  Arbitration lost */
    IICMB_E_IICMB,      /**<  I2C controller encoutered issue */
    IICMB_E_ICTF,       /**<  Transfer incomplete */
    IICMB_E_FSM,        /**<  non designed path of FSM used */
    IICMB_E_BUSOCC,     /**<  I2C bus occupied by another slave */
    IICMB_E_UNKNOWN     /**<  Something went wrong */
} t_iicmb_ero;



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
    volatile uint8_t        CSR;    /**<  Control/Status Register   R/W */
    volatile uint8_t        DPR;    /**<  Data/Parameter Register   R/W */
    volatile uint8_t        CMDR;   /**<  Command Register          R/W */
    volatile const uint8_t  FSMR;   /**<  FSM States Register       RO  */

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
    t_iicm_reg*             iicmb;              /**<  pointer to IICMB hardware registers */
    volatile t_iicmb_fsm    fsm;                /**<  Soft I2C state machine @see IICMB_FSM */
    volatile t_iicmb_ero    error;              /**<  Encoutered errors while exec, #t_iicmb_ero */
    uint8_t                 uint8WrRd;          /**<  Flag: Write/Read Interaction, allows to use first Write, then read part of FSM */
    uint8_t                 uint8Adr;           /**<  I2C slave address */
    uint16_t                uint16WrByteLen;    /**<  Total number of bytes to write */
    volatile uint16_t       uint16WrByteIs;     /**<  Current number of bytes written */
    uint16_t                uint16RdByteLen;    /**<  Total number of bytes to read */
    volatile uint16_t       uint16RdByteIs;     /**<  Current number of bytes readen */
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



/**
 *  @brief init
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



/**
 *  @brief close
 *
 *  make SW handle to IICMB core invalid
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   OK
 *  @since          2023-03-25
 *  @author         Andreas Kaeberlein
 */
int iicmb_close(t_iicmb *self);



/** @brief busy wait
 *
 *  busy wait for completion of last command
 *
 *  @param[in,out]  self                driver handle
 *  @return         int                 state
 *  @retval         0                   IDLE
 *  @since          2022-06-10
 *  @author         Andreas Kaeberlein
 */
int iicmb_busy_wait(t_iicmb *self);



/** @brief FSM
 *
 *  IICMB state machine
 *
 *  @param[in,out]  self                storage element
 *  @return         void                state
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
void iicmb_fsm(t_iicmb *self);



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
int iicmb_busy(t_iicmb *self);



/** @brief error?
 *
 *  check if an error is occured while transmission, #t_iicmb_ero
 *
 *  @param[in,out]  self                storage element
 *  @return         int                 state
 *  @retval         0                   OKAY
 *  @retval         -1                  Error, use decode function for details
 *  @since          2022-06-16
 *  @author         Andreas Kaeberlein
 */
int iicmb_is_error(t_iicmb *self);



/** @brief I2C bus state
 *
 *  checks I2C bus state
 *
 *  @param[in,out]  self                storage element
 *  @return         int                 state, #I2C_SW_FUNC
 *  @retval         IICMB_EXIT_OK       I2C Bus is free
 *  @retval         IICMB_EXIT_BUSY     I2C bus is busy caused by IICMB
 *  @retval         IICMB_EXIT_OCC      I2C bus is occupied by another master
 *  @since          2024-02-09
 *  @author         Andreas Kaeberlein
 */
int iicmb_bus_state(t_iicmb *self);



/** @brief write
 *
 *  write to I2C slave
 *
 *  @param[in,out]  self                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[in]      *data               data buffer
 *  @param[in]      len                 size of *data in byte
 *  @return         int                 state
 *  @retval         IICMB_EXIT_OK       OK: Transfer request accepted
 *  @retval         IICMB_EXIT_BUSY     FAIL: Transfer request not accepted, wait for finish before next request
 *  @retval         IICMB_EXIT_OCC      FAIL: I2C bus is occupied by another master
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicmb_write(t_iicmb *self, uint8_t adr7, void* data, uint16_t len);



/** @brief read
 *
 *  read from I2C slave
 *
 *  @param[in,out]  self                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[out]     *data               data buffer
 *  @param[in]      len                 size of *data in byte
 *  @return         int                 state
 *  @retval         IICMB_EXIT_OK       OK: Transfer request accepted
 *  @retval         IICMB_EXIT_BUSY     FAIL: Transfer request not accepted, wait for finish before next request
 *  @retval         IICMB_EXIT_OCC      FAIL: I2C bus is occupied by another master
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicmb_read(t_iicmb *self, uint8_t adr7, void* data, uint16_t len);



/** @brief write-read
 *
 *  perform I2C write, repeated start condition and I2C read
 *
 *  @param[in,out]  this                storage element
 *  @param[in]      adr7                Slave address (7bit)
 *  @param[in,out]  *data               data buffer, write and read takes place in the same buffer, in consequence is the write data by the read data overwritten
 *  @param[in]      wrLen               number of bytes to write in *data
 *  @param[in]      rdLen               number of bytes to read in *data
 *  @return         int                 state
 *  @retval         IICMB_EXIT_OK       OK: Transfer request accepted
 *  @retval         IICMB_EXIT_BUSY     FAIL: Transfer request not accepted, wait for finish before next request
 *  @retval         IICMB_EXIT_OCC      FAIL: I2C bus is occupied by another master
 *  @retval         IICMB_EXIT_ERROR    FAIL: RSomething went wrong
 *  @since          2022-06-14
 *  @author         Andreas Kaeberlein
 */
int iicmb_wr_rd(t_iicmb *self, uint8_t adr7, void* data, uint16_t wrLen, uint16_t rdLen);



#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __IICMB_H
