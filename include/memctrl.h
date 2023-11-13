#ifndef MEMCTRL_H
# define MEMCTRL_H 1

# include <stdbool.h>
# include <string.h>

# define MAX_MEMCTRL_STACK 65536

static void *MEMCTRL_STACK[MAX_MEMCTRL_STACK] = {0};

bool __MEMCTRL_STATUS_LOCK = false;

void **_memctrl_p;

static inline void
memctrl_initmemstk()
{
  /* Lock up status */
  __MEMCTRL_STATUS_LOCK = true;
  
  /* Initialise memstack */
  memset(MEMCTRL_STACK, 0, MAX_MEMCTRL_STACK);
}

static inline void
memctrl_resetmemstk()
{
  
}

void
memctrl_push(void *addr);

void
memctrl_pop();

bool
memctrl_empty();

bool
memctrl_full();

bool
memctrl_exist(void *addr);

void
_memctrl_mvp(unsigned int idx);

void *
_memctrl_get();

void
_memctrl_set(void *addr);

# define PROGBEGIN memctrl_initmemstk()
# define PROGEND memctrl_resetmemstk()

#endif /* NO MEMCTRL_H */
