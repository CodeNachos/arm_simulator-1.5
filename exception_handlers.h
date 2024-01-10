#ifndef _EXCPT_HDLR_H_
#define _EXCPT_HDLR_H_

#include "arm_core.h"

void reset(arm_core p);

void undefined_instruction(arm_core p);

void prefetch_abort(arm_core p);

void data_abort(arm_core p);

void irq(arm_core p);

void fiq(arm_core p);

#endif