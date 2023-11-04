#include <stdlib.h>
#include "../../ExFC/src/exfc.h"
#include "memctrl.h"

#define AUTO_MEM_CTRL 1

void
memctrl_push(void *addr)
{
  if (memctrl_stkfull())
    THROW(StackOverflowException, __FILE__, __LINE__, __FUNCTION__, EXCEP_FMT);
}

bool
memctrl_stkfull()
{
  return (memctrl_stklen() == MAX_MEMCTRL_STACK);
}

int
memctrl_stklen()
{
  memlen()
}
