
/*******************************************************************************
**                                                                             *
**    Project: IIC Multiple Bus Controller (IICMB)                             *
**                                                                             *
**    File:    Several basic functions for IICMB core.                         *
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

#include <stdio.h>
#include <io.h>

#include "iicmb.h"


/* Print responses */
void print_rsp(rsp_tt r)
{
  switch (r)
  {
    case rsp_done :
      printf("Done");
      break;
    case rsp_nak :
      printf("No acknowledge");
      break;
    case rsp_arb_lost :
      printf("Arbitration lost");
      break;
    case rsp_err :
      printf("Error");
      break;
  }
}

/* Initialize IICMB core */
void iicmb_init(void)    { IICMB_REG_WRITE(IICMB_CSR, IICMB_CSR_ENABLE); }
/* Disable IICMB core */
void iicmb_disable(void) { IICMB_REG_WRITE(IICMB_CSR, 0x00u); }

/* Waiting for a response after issuing a command */
rsp_tt iicmb_wait_response(void)
{
  int tmp;
  do
  {
    tmp = IICMB_REG_READ(IICMB_CMDR);
  } while ((tmp & IICMB_RSP_COMPLETED) == 0);

  if (tmp & IICMB_RSP_NAK)      { return rsp_nak; }
  if (tmp & IICMB_RSP_ARB_LOST) { return rsp_arb_lost; }
  if (tmp & IICMB_RSP_ERR)      { return rsp_err; }

  return rsp_done;
}

/* 'Wait' command */
rsp_tt iicmb_cmd_wait(unsigned char n)
{
  IICMB_REG_WRITE(IICMB_DPR, ((unsigned int)n & 0x000000FFu));
  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_WAIT);
  return iicmb_wait_response();
}

/* 'Write' command */
rsp_tt iicmb_cmd_write(unsigned char n)
{
  IICMB_REG_WRITE(IICMB_DPR, ((unsigned int)n & 0x000000FFu));
  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_WRITE);
  return iicmb_wait_response();
}

/* 'Read With Ack' command */
rsp_tt iicmb_cmd_read_ack(unsigned char * n)
{
  rsp_tt ret;

  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_READ_ACK);
  ret = iicmb_wait_response();
  if (ret == rsp_done) { *n = (unsigned char)IICMB_REG_READ(IICMB_DPR); }

  return ret;
}

/* 'Read With Nak' command */
rsp_tt iicmb_cmd_read_nak(unsigned char * n)
{
  rsp_tt ret;

  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_READ_NAK);
  ret = iicmb_wait_response();
  if (ret == rsp_done) { *n = (unsigned char)IICMB_REG_READ(IICMB_DPR); }

  return ret;
}

/* 'Start' command */
rsp_tt iicmb_cmd_start(void)
{
  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_START);
  return iicmb_wait_response();
}

/* 'Stop' command */
rsp_tt iicmb_cmd_stop(void)
{
  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_STOP);
  return iicmb_wait_response();
}

/* 'Set Bus' command */
rsp_tt iicmb_cmd_set_bus(unsigned char n)
{
  IICMB_REG_WRITE(IICMB_DPR, ((unsigned int)n & 0x000000FFu));
  IICMB_REG_WRITE(IICMB_CMDR, IICMB_CMD_SET_BUS);
  return iicmb_wait_response();
}


/* Read a single byte */
rsp_tt iicmb_read_bus(unsigned char sa, unsigned char a, unsigned char * d)
{
  rsp_tt ret;

  /* Start condition */
  ret = iicmb_cmd_start();
  if (ret != rsp_done) return ret;

  do {
    /* Write slave address and write bit */
    ret = iicmb_cmd_write((sa << 1) | 0x00u);
    if (ret != rsp_done) break;
    /* Write byte address */
    ret = iicmb_cmd_write(a);
    if (ret != rsp_done) break;
    /* Repeated start */
    ret = iicmb_cmd_start();
    if (ret != rsp_done) return ret;
    /* Write slave address and read bit */
    ret = iicmb_cmd_write((sa << 1) | 0x01u);
    if (ret != rsp_done) break;
    /* Read byte of data with not-acknowledge */
    ret = iicmb_cmd_read_nak(d);
    if (ret != rsp_done) return ret;
  } while (0);

  /* Stop condition */
  (void)iicmb_cmd_stop();

  return ret;
}

/* Reading several bytes */
rsp_tt iicmb_read_bus_mul(unsigned char sa, unsigned char a, unsigned char * d, int n)
{
  rsp_tt ret;
  int i;

  /* Start condition */
  ret = iicmb_cmd_start();
  if (ret != rsp_done) return ret;

  do {
    /* Write slave address and write bit */
    ret = iicmb_cmd_write((sa << 1) | 0x00u);
    if (ret != rsp_done) break;
    /* Write byte address */
    ret = iicmb_cmd_write(a);
    if (ret != rsp_done) break;
    /* Repeated start */
    ret = iicmb_cmd_start();
    if (ret != rsp_done) return ret;
    /* Write slave address and read bit */
    ret = iicmb_cmd_write((sa << 1) | 0x01u);
    if (ret != rsp_done) break;
    for (i = 0; i < (n - 1); i++)
    {
      /* Read byte of data with acknowledge */
      ret = iicmb_cmd_read_ack(d + i);
      if (ret != rsp_done) return ret;
    }
    /* Read byte of data with not-acknowledge */
    ret = iicmb_cmd_read_nak(d + i);
    if (ret != rsp_done) return ret;
  } while (0);

  /* Stop condition */
  (void)iicmb_cmd_stop();

  return ret;
}

/* Write single byte */
rsp_tt iicmb_write_bus(unsigned char sa, unsigned char a, unsigned char d)
{
  rsp_tt ret;

  /* Start condition */
  ret = iicmb_cmd_start();
  if (ret != rsp_done) return ret;

  do {
    /* Write slave address and write bit */
    ret = iicmb_cmd_write((sa << 1) | 0x00000000);
    if (ret != rsp_done) break;
    /* Write byte address */
    ret = iicmb_cmd_write(a);
    if (ret != rsp_done) break;
    /* Write byte of data */
    ret = iicmb_cmd_write(d);
    if (ret != rsp_done) break;
  } while (0);

  /* Stop condition */
  (void)iicmb_cmd_stop();

  return ret;
}

/* Writing several bytes */
rsp_tt iicmb_write_bus_mul(unsigned char sa, unsigned char a, unsigned char * d, int n)
{
  rsp_tt ret;
  int i;

  /* Start condition */
  ret = iicmb_cmd_start();
  if (ret != rsp_done) return ret;

  do {
    /* Write slave address and write bit */
    ret = iicmb_cmd_write((sa << 1) | 0x00000000);
    if (ret != rsp_done) break;
    /* Write byte address */
    ret = iicmb_cmd_write(a);
    if (ret != rsp_done) break;
    for (i = 0; i < n; i++)
    {
      /* Write byte of data */
      ret = iicmb_cmd_write(*(d + i));
      if (ret != rsp_done) break;
    }
  } while (0);

  /* Stop condition */
  (void)iicmb_cmd_stop();

  return ret;
}

/* Report registers */
void iicmb_report_registers(FILE *fp)
{
  int uuu;

  fprintf(fp, "\n--------------------------------------------------------\n");
  fprintf(fp, "IICMB state:\n\n");

  uuu = IICMB_REG_READ(IICMB_CSR);
  fprintf(fp, "CSR  = 0x%02x\n", uuu);
  fprintf(fp, "  Core is ");
  if (uuu & 0x00000080) { fprintf(fp, "enabled\n"); } else { fprintf(fp, "disabled\n"); }
  fprintf(fp, "  Interrupts are ");
  if (uuu & 0x00000040) { fprintf(fp, "enabled\n"); } else { fprintf(fp, "disabled\n"); }
  fprintf(fp, "  Selected bus #%d is ", uuu & 0x0000000F);
  if (uuu & 0x00000020) { fprintf(fp, "busy and "); } else { fprintf(fp, "free and "); }
  if (uuu & 0x00000010) { fprintf(fp, "captured\n"); } else { fprintf(fp, "not captured\n"); }

  uuu = IICMB_REG_READ(IICMB_DPR);
  fprintf(fp, "DPR  = 0x%02x\n", uuu);
  uuu = IICMB_REG_READ(IICMB_CMDR);
  fprintf(fp, "CMDR = 0x%02x\n", uuu);
  fprintf(fp, "  Last command was ");
  switch (uuu & 0x0000000F)
  {
    case IICMB_CMD_WAIT     : fprintf(fp, "'Wait'");          break;
    case IICMB_CMD_WRITE    : fprintf(fp, "'Write'");         break;
    case IICMB_CMD_READ_ACK : fprintf(fp, "'Read with Ack'"); break;
    case IICMB_CMD_READ_NAK : fprintf(fp, "'Read with Nak'"); break;
    case IICMB_CMD_START    : fprintf(fp, "'Start'");         break;
    case IICMB_CMD_STOP     : fprintf(fp, "'Stop'");          break;
    case IICMB_CMD_SET_BUS  : fprintf(fp, "'Set bus'");       break;
    default                 : fprintf(fp, "'STRANGE!!!'");    break;
  }
  fprintf(fp, " and response is: ");
  switch (uuu & 0x000000F0)
  {
    case IICMB_RSP_DONE     : fprintf(fp, "'Done'\n");             break;
    case IICMB_RSP_NAK      : fprintf(fp, "'No acknowledge'\n");   break;
    case IICMB_RSP_ARB_LOST : fprintf(fp, "'Arbitration lost'\n"); break;
    case IICMB_RSP_ERR      : fprintf(fp, "'Error'\n");            break;
    default                 : fprintf(fp, "STRANGE!!!\n");         break;
  }

  uuu = IICMB_REG_READ(IICMB_FSMR);
  fprintf(fp, "FSMR = 0x%02x\n", uuu);
  fprintf(fp, "  Byte-level FSM state is ");
  switch (uuu & 0x000000F0)
  {
    case 0x00000000 : fprintf(fp, "'(Idle)'\n");           break;
    case 0x00000010 : fprintf(fp, "'(Bus is taken)'\n");   break;
    case 0x00000020 : fprintf(fp, "'Start is pending'\n"); break;
    case 0x00000030 : fprintf(fp, "'Start'\n");            break;
    case 0x00000040 : fprintf(fp, "'Stop'\n");             break;
    case 0x00000050 : fprintf(fp, "'Byte Write'\n");       break;
    case 0x00000060 : fprintf(fp, "'Byte Read'\n");        break;
    case 0x00000070 : fprintf(fp, "'Waiting'\n");          break;
    default         : fprintf(fp, "'STRANGE!!!'\n");       break;
  }
  fprintf(fp, "  Bit-level FSM state is ");
  switch (uuu & 0x0000000F)
  {
    case 0x00000000 : fprintf(fp, "'(Idle)'\n");           break;
    case 0x00000001 : fprintf(fp, "'Start A'\n");          break;
    case 0x00000002 : fprintf(fp, "'Start B'\n");          break;
    case 0x00000003 : fprintf(fp, "'(Start C)'\n");        break;
    case 0x00000004 : fprintf(fp, "'Read/Write A'\n");     break;
    case 0x00000005 : fprintf(fp, "'Read/Write B'\n");     break;
    case 0x00000006 : fprintf(fp, "'Read/Write C'\n");     break;
    case 0x00000007 : fprintf(fp, "'Read/Write D'\n");     break;
    case 0x00000008 : fprintf(fp, "'(Read/Write E)'\n");   break;
    case 0x00000009 : fprintf(fp, "'Stop A'\n");           break;
    case 0x0000000a : fprintf(fp, "'Stop B'\n");           break;
    case 0x0000000b : fprintf(fp, "'Stop C'\n");           break;
    case 0x0000000c : fprintf(fp, "'Repeated Start A'\n"); break;
    case 0x0000000d : fprintf(fp, "'Repeated Start B'\n"); break;
    case 0x0000000e : fprintf(fp, "'Repeated Start C'\n"); break;
    default         : fprintf(fp, "'STRANGE!!!'\n");       break;
  }
  fprintf(fp, "--------------------------------------------------------\n");
  fprintf(fp, "\n");
}

