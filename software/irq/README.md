# [IICMB](/software/irq/iicmb.c) IRQ driver

Interrupt driven dataflow for IICMB.


## API

To interact with HDL _IICMB_ can following API used.


### Init

Initializes the _IICMB_ driver.
 * _*self_ : common storage handle
 * _iicmbAdr_: base address pointer to IICMB
 * _bus_: selected I2C channel

```c
int iicmb_init(t_iicmb *self, void* iicmbAdr, uint8_t bus);
```


### Wait

Waits until _IICMB_ reaches busy state. This would allow to run this driver in a poll loop.
 * _*self_ : common storage handle

```c
int iicmb_busy_wait(t_iicmb *self);
```


### ISR

_IICMB_ fsm. This function needs to be called by the processors Interrupt handler.
 * _*self_ : common storage handle

```c
void iicmb_fsm(t_iicmb *self);
```


### Busy

Checks if the _IICMB_ is ready for new requests.
 * _*self_ : common storage handle

```c
int iicmb_busy(t_iicmb *self);
```


### Error

Checks if the last transfer ended with an error.
 * _*self_ : common storage handle

```c
int iicmb_is_error(t_iicmb *self);
```


### Write

Writes data packet to I2C slave.
 * _*self_ : common storage handle
 * _adr7_: 7bit slave address
 * _*data_: pointer to write data
 * _len_: number of bytes in _*data_

```c
int iicmb_write(t_iicmb *self, uint8_t adr7, void* data, uint16_t len);
```


### Read

Reads data packet from I2C slave.
 * _*self_ : common storage handle
 * _adr7_: 7bit slave address
 * _*data_: pointer to read data
 * _len_: number of bytes in _*data_

```c
int iicmb_read(t_iicmb *self, uint8_t adr7, void* data, uint16_t len);
```


### Write-Read

Writes to I2C slave, sends a repeated start condition and starts with read.
Read and write data takes place in the same buffer. That means the read data
will overwrite the write data.
 * _*self_ : common storage handle
 * _adr7_: 7bit slave address
 * _*data_: pointer to read data
 * _wrLen_: number of write bytes in _*data_
 * _rdLen_: number of read bytes to capture in _*data_

```c
int iicmb_wr_rd(t_iicmb *self, uint8_t adr7, void* data, uint16_t wrLen, uint16_t rdLen);
```


### Example

The code snippet below shows the integration of the driver into a user application.

```c
#include <stdlib.h> // EXIT codes, malloc
#include <stdio.h>  // f.e. printf
#include <stdint.h> // defines fixed data types: int8_t...
#include "iicmb.h"  // IICMB driver

t_iicmb g_iicmb;  // handle for IICMB driver

/**
 *  isr
 *    ISR IICMB Macro
 */
void processors_isr(void)
{
  iicmb_fsm(&g_iicmb);
}

/**
 *  main
 */
int main ()
{
  /* variables */
  uint8_t i2c[10];  // I2C packet to interact

  /* init IICMB
   *   base address 'NULL', change to physical address of IICMB
   *   traffic on I2C channel 0
   */
  iicmb_init(&g_iicmb, (void*) NULL, 0);

  /* issue transfer
   *   access I2C slave with address 0x12
   *   write 5 bytes to slave
   *   read 8 bytes from slave
   */
  iicmb_wr_rd(&g_iicmb, 0x12, &i2c, 5, 8);

  /* wait for transfer */
  while ( iicmb_busy(&g_iicmb) ) {
    __asm("nop");
  }

  /* normal end */
  exit(0);
}
```

For exemplary compile:
```bash
gcc -c -O iicmb.c -o iicmb.o
gcc -c -O main.c -o main.o
gcc iicmb.o main.o -lm -o main
```


## [Test](/software/irq/test/iicmb_test.c)

Githubs [CI/CD](/.github/workflows/c.yml) executes the [Makefile](/software/irq/Makefile) and performs some simple tests.

