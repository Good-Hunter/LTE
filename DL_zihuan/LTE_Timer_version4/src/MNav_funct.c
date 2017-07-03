/***********************************************/
/*MNav_funct.c                                 */
/*π¶ƒ‹£∫QM≈‰÷√                                 */
/***********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "MNav_funct.h"

/* This function programs a QM memory region. */
void set_memory_region(unsigned short regn, unsigned int addr, unsigned int indx, unsigned int setup)
{
  unsigned int *reg;

  reg = (unsigned int *)(QM_DESC_REGION + QM_REG_MEM_REGION_BASE + (regn * 16));
 *reg = addr;

  reg = (unsigned int *)(QM_DESC_REGION + QM_REG_MEM_REGION_INDEX + (regn * 16));
 *reg = indx;

  /* See register description for programming values. */
  reg = (unsigned int *)(QM_DESC_REGION + QM_REG_MEM_REGION_SETUP + (regn * 16));
 *reg = setup;
}


/* This function programs a QM Link RAM. */
void set_link_ram(unsigned short ram, unsigned int addr, unsigned int count)
{
  unsigned int *reg;

  reg = (unsigned int *)(QM_CTRL_REGION + QM_REG_LINKRAM_0_BASE + (ram * 8));
 *reg = addr;

  if (ram == 0)
  {
    reg = (unsigned int *)(QM_CTRL_REGION + QM_REG_LINKRAM_0_SIZE);
   *reg = count;
  }
}


/* This function pushes descriptor info to a queue.
 * mode parameter:  1 = write reg D only.
 *                  2 = write regs C and D.
 */
void push_queue(unsigned short qn, unsigned char mode, unsigned int c_val, unsigned int d_val)
{
  unsigned int *reg;

  if (mode == 2)
  {
    reg = (unsigned int *)(QM_QMAN_REGION + QM_REG_QUE_REG_C + (qn * 16));
   *reg = c_val;
  }

  reg = (unsigned int *)(QM_QMAN_REGION + QM_REG_QUE_REG_D + (qn * 16));
 *reg = d_val;
}


/* This function pops a descriptor from a queue. */
unsigned int pop_queue(unsigned short qn)
{
  unsigned int *reg;
  unsigned int  value;

  reg = (unsigned int *)(QM_QMAN_REGION + QM_REG_QUE_REG_D + (qn * 16));
  value = *reg;

  return(value);
}


/* This function moves a source queue to a destination queue.  If
 * headtail = 0, the source queue is appended to the tail of the
 * dest queue.  If 1, it is appended at the head. */
void divert_queue(unsigned short src_qn, unsigned short dest_qn, unsigned char headtail)
{
  unsigned int *reg;
  unsigned int  value;

  reg = (unsigned int *)(QM_CTRL_REGION + QM_REG_QUE_DIVERSION);

  value = (headtail << 31) + (dest_qn << 16) + src_qn;
 *reg = value;

  return;
}


/* This function pops a queue until it is empty. If *list is not NULL,
 * it will return the list of descriptor addresses and the count. */
void empty_queue(unsigned short qn, unsigned int *list, unsigned int *listCount)
{
  unsigned int *reg;
  unsigned int  value;
  unsigned short  idx;
  unsigned int  count;

  reg = (unsigned int *)(QM_QMAN_REGION + QM_REG_QUE_REG_A + (qn * 16));
  count = *reg; //read the descriptor count
 *listCount = count;

  reg = (unsigned int *)(QM_QMAN_REGION + QM_REG_QUE_REG_D + (qn * 16));

  for (idx = 0; idx < count; idx ++)
  {
    value = *reg;
    if (list != NULL)
    {
      list[idx] = value;
    }
  }
}


/* This function returns the byte count of a queue. */
unsigned int get_byte_count(unsigned short qn)
{
  unsigned int *reg;
  unsigned int  count;

  reg = (unsigned int *)(QM_PEEK_REGION + QM_REG_QUE_REG_B + (qn * 16));
  count = *reg;

  return(count);
}


/* This function returns the descriptor count of a queue. */
unsigned int get_descriptor_count(unsigned short qn)
{
  unsigned int *reg;
  unsigned int  count;

  reg = (unsigned int *)(QM_PEEK_REGION + QM_REG_QUE_REG_A + (qn * 16));
  count = *reg;

  return(count);
}


/* This function sets a queue threshold for triggering PKTDMA channels. */
void set_queue_threshold(unsigned short qn, unsigned int value)
{
  unsigned int *reg;

  reg = (unsigned int *)(QM_PEEK_REGION + QM_REG_QUE_STATUS_REG_D + (qn * 16));
 *reg = value;
}

/* This function programs base addresses for the four logical
 * queue managers that a PKTDMA may setup.  Use a value of 0xffff
 * if you don't want to set value into QM base addr reg. N. */
void config_pktdma_qm(unsigned int base, unsigned short *physical_qnum)
{
  unsigned short  idx;
  unsigned int  qm_base;
  unsigned int *reg;

  for (idx = 0; idx < 4; idx ++)
  {
    if (physical_qnum[idx] != 0xffff)
    {
      qm_base = QM_QMAN_VBUSM_REGION + (physical_qnum[idx] * 16);

      reg = (unsigned int *)(base + PKTDMA_REG_QM0_BASE_ADDR + (idx * 4));
     *reg = qm_base;
    }
  }
}


/* This function disables a TX PKTDMA channel, then configures Reg B. */
void config_tx_chan(unsigned int base, unsigned short chan, unsigned int reg_b)
{
  unsigned int *reg;

  reg = (unsigned int *)(base + PKTDMA_REG_TX_CHAN_CFG_A + (chan * 32));
 *reg = 0; //disable the channel

  reg = (unsigned int *)(base + PKTDMA_REG_TX_CHAN_CFG_B + (chan * 32));
 *reg = reg_b;
}


/* This function configures priority a TX PKTDMA channel */
void config_tx_sched(unsigned int base, unsigned short chan, unsigned int priority)
{
  unsigned int *reg;

  reg = (unsigned int *)(base + PKTDMA_REG_TX_SCHED_CHAN_CFG + (chan * 4));
 *reg = priority;
}


/* This function configures a RX PKTDMA channel flow. */
void config_rx_flow(unsigned int base, unsigned short flow,
                    unsigned int a, unsigned int b, unsigned int c, unsigned int d,
                    unsigned int e, unsigned int f, unsigned int g, unsigned int h)
{
  unsigned int *reg;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_A + (flow * 32));
 *reg = a;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_B + (flow * 32));
 *reg = b;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_C + (flow * 32));
 *reg = c;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_D + (flow * 32));
 *reg = d;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_E + (flow * 32));
 *reg = e;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_F + (flow * 32));
 *reg = f;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_G + (flow * 32));
 *reg = g;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_FLOW_CFG_H + (flow * 32));
 *reg = h;
}


/* This function writes a RX PKTDMA channel's enable register. */
void enable_rx_chan(unsigned int base, unsigned short chan, unsigned int value)
{
  unsigned int *reg;

  reg = (unsigned int *)(base + PKTDMA_REG_RX_CHAN_CFG_A + (chan * 32));
 *reg = value;
}


/* This function writes a TX PKTDMA channel's enable register. */
void enable_tx_chan(unsigned int base, unsigned short chan, unsigned int value)
{
  unsigned int *reg;

  reg = (unsigned int *)(base + PKTDMA_REG_TX_CHAN_CFG_A + (chan * 32));
 *reg = value;
}

/* This function turns on/off PKTDMA loopback mode for IP test. 0x0 disable, 0x80000000 enable */
void enable_disable_loopback(unsigned int addr, unsigned int value)
{
  unsigned int *reg;

  reg = (unsigned int *)(addr + 0x04);
 *reg = 0x00080000; //set write arbitration FIFO depth to 8
 
  reg = (unsigned int *)(addr + 0x08);
 *reg = value;
}

/* This function reads a QMSS INTD Status Register.
 * group parameter:  0 = high priority interrupts.
 *                   1 = low priority interrupts.
 *                   4 = PKTDMA starvation interrupts.
 *
 * If the chan parameter = 0xffffffff, the entire register contents
 * are returned.  Otherwise, chan is expected to be a channel number,
 * and the return value will be a 0 or 1 for that channel's status.
 */
unsigned int read_status(unsigned short group, unsigned int chan)
{
  unsigned int *reg;
  unsigned int  value;
  unsigned int  mask;

  reg = (unsigned int *)(QM_INTD_REGION + QM_REG_INTD_STATUS + (group * 4));
  value = *reg;

  if (chan != 0xffffffff)
  {
    mask = 1 << (chan & 0x001f);
    if ((value & mask) == 0)
      value = 0;
    else
      value = 1;
  }

  return(value);
}


/* This function writes a QMSS INTD Status Register.
 * group parameter:  0 = high priority interrupts.
 *                   1 = low priority interrupts.
 *                   4 = PKTDMA starvation interrupts.
 */
void set_status(unsigned short group, unsigned int chan)
{
  unsigned int *reg;
  unsigned int  value;
  unsigned int  mask;

  reg = (unsigned int *)(QM_INTD_REGION + QM_REG_INTD_STATUS + (group * 4));
  value = *reg;

  mask = 1 << (chan & 0x001f);
  value |= mask;
 *reg = value;
}


/* This function writes a QMSS INTD Status Clear Register.
 * group parameter:  0 = high priority interrupts.
 *                   1 = low priority interrupts.
 *                   4 = PKTDMA starvation interrupts.
 */
void clear_status(unsigned short group, unsigned int chan)
{
  unsigned int *reg;
  unsigned int  value;
  unsigned int  mask;

  reg = (unsigned int *)(QM_INTD_REGION + QM_REG_INTD_STATUS_CLEAR + (group * 4));
  value = *reg;

  mask = 1 << (chan & 0x001f);
  value |= mask;
 *reg = value;
}

