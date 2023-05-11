#include "exception.h"

int
exception_addexcep(char *excep_name, char *description);

void
exception_addexcep_id(char *excep_name, char *description, int id);

char *
exception_removeexcep_byname(char *excep_name);

int
exception_removeexcep_byid(excep_e id);

_excep_t **
exception_getallexcep();

int
exception_findexcep_byname(char *excep_name);

int
exception_findexcep_byid(excep_e id);

int
_exception_iteration(_excep_t e)
{

  fail_rtn(&e);

  for (int i = 0; i < _excep_arr_len - 1; i ++)
    {
      /* Once matched, return the index. */
      if (_exception_quick_match_str(e._name, _excep_arr[i]._name, true)
          && (e._id == _excep_arr[i]._id))
          return i;
    }
  return -1;
}

int
_exception_quick_match_str(char *a, char *b, bool capital_restricted)
{
  fail_rtn(a);
  fail_rtn(b);
  fail_rtn(&capital_restricted);

  int lenA = strlen(a);
  int lenB = strlen(b);

  /* Check length */
  if (lenA != lenB)
    return false;

  for (int i = 0; i < lenA; i ++)
    {
      if (!_exception_capital_check(a[i], b[i], capital_restricted))
        return false;
    }
  return true;
}

int
_exception_capital_check(char a, char b, bool capital_restricted)
{
  fail_rtn(&a);
  fail_rtn(&b);
  fail_rtn(&capital_restricted);

  return ((capital_restricted) ? (a == b) : ((a - b) == 32));
}
