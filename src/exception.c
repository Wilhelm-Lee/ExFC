#include "exception.h"

void
exception_addexcep(char *excep_name, char *description);

void
exception_removeexcep_byname(char *excep_name);

void
exception_removeexcep_byid(excep_e id);

char **
exception_getallexcep();

bool
exception_findexcep_byname(char *excep_name);

bool
exception_findexcep_byid(excep_e id);
