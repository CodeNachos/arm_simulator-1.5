#ifndef _EXCPT_HDLR_H_
#define _EXCPT_HDLR_H_

void reset();

void undefined_instruction();

void prefetch_abort();

void data_abort();

void irq();

void fiq();

#endif