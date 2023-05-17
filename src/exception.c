/*
 *     This file is part of project <https://github.com/WilliamPascal/ExFC>
 *     Copyright (C) 2023  William Pascal
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If NOT, see <https://www.gnu.org/licenses/>.
 */

#include "exception.h"

excep_return_e
_exception_buffersize_chk(char *buff)
{
  fail(buff, FAILED);

  if (strlen((const char *)buff) > EXCEP_BUFF_MAX)
    {
      THROW((_excep_t){"BufferOverflowException", "", BufferOverflowException},
            __FILE__, __LINE__, __FUNCTION__, _EXCEP_FMT);
    }

  return NORMAL;
}

excep_return_e
exception_addexcep(char *excep_name, char *description, int id)
{
  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);
  fail(description, FAILED);
  fail(&id, FAILED);

  _exception_buffersize_chk(excep_name);
  _exception_buffersize_chk(description);

  /* Found the duplication, exit. */
  if (exception_findexcep_byname(excep_name) != CONDITIONAL
      || exception_findexcep_byid(id) != CONDITIONAL)
    return DUPLICATED;

  int index = _exception_rearrangement();

  /* Array was full */
  if (index == _excep_arr_len)
    return CONDITIONAL;

  /* Assign */
  _excep_arr[index]._name = excep_name;
  _excep_arr[index]._description = description;
  _excep_arr[index]._id = id;

  return index;
}

int
exception_removeexcep_byname(char *excep_name)
{
  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);

  _exception_buffersize_chk(excep_name);

  /* Find the desired exception */
  int index = exception_findexcep_byname(excep_name);

  /* Not found */
  if (index == MISSING)
    return MISSING;

  /* Assign */
  _excep_arr[index]._name = NULL;
  _excep_arr[index]._description = NULL;
  _excep_arr[index]._id = 0;

  return index;
}

int
exception_removeexcep_byid(int id)
{
  fail(_excep_arr, ABNORMAL);
  fail(&id, FAILED);

  /* Find the desired exception */
  int index = exception_findexcep_byid(id);

  /* Not found */
  if (index == MISSING)
    return MISSING;

  /* Assign */
  _excep_arr[index]._name = NULL;
  _excep_arr[index]._description = NULL;
  _excep_arr[index]._id = 0;

  return index;
}

_excep_t *
exception_getallexcep()
{
  fail(_excep_arr, (_excep_t*)excep_nullptr);

  /* Ensure no gaps exists */
  int rtnlen = _exception_rearrangement();

  _excep_t rtnarr[rtnlen];
  _excep_t *rtn = rtnarr;

  for (int i = 0; i < rtnlen; i ++)
    {
      rtnarr[i] = _excep_arr[i];
    }

  return rtn;
}

excep_return_e
exception_findexcep_byname(char *excep_name)
{
  /* YOU LEFT HERE */
}

excep_return_e
exception_findexcep_byid(int id);

excep_return_e
exception_findexcep_byit(_excep_t e)
{
  fail(_excep_arr, ABNORMAL);
  fail(&e, FAILED);

  for (int i = 0; i < _excep_arr_len - 1; i ++)
    {
      /* Once matched, return the index. */
      if (_exception_quick_match_str(e._name, _excep_arr[i]._name, true)
          && (e._id == _excep_arr[i]._id))
        return i;
    }
  return MISSING;
}

excep_return_e
_exception_iteration_last()
{
  fail(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (int i = _excep_arr_len - 1; i >= 0; i --)
    {
      /* Finding non-null elements, mark on. */
      if (&_excep_arr[i] != NULL)
        rtn = i;
    }
  /* Once reached at the beginning, return the index. */
  return rtn;
}

excep_return_e
_exception_iteration_first()
{
  fail(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (int i = 0; i < _excep_arr_len; i ++)
    {
      /* Finding non-null elements, mark on. */
      if (&_excep_arr[i] != NULL)
        rtn = i;
    }
  /* Once reached at the end, return the index. */
  return rtn;
}

int
_exception_rearrangement()
{
  fail(_excep_arr, ABNORMAL);

  _excep_t tmp[_excep_arr_len];

  int tmp_index = 0;
  for (int arr_index = 0; arr_index < _excep_arr_len; arr_index ++)
    {
      if (&_excep_arr[arr_index] == NULL
          || excep_cmp(_excep_arr[arr_index], excep_null) == 0)
        continue;
      /* Not null. Write this element into array tmp. */
      else
        {
          tmp[tmp_index] = _excep_arr[arr_index];
          tmp_index += 1;
        }
    }
  /* Apply tmp on _excep_arr */
  for (int i = 0; i < _excep_arr_len; i ++)
    {
      /*
         _excep_arr: (len = 19)
         [1_23456___78_9A__BC] -> real length == 12 elem
         tmp_arr: (len = 19)
         [123456789ABC_______] -> real length == 12 elem
                     ~^ (12th)
                                  tmp_index == 12
      */
      /* Overwrite previous elements by the ones in array tmp. */
      if (i <= tmp_index)
        _excep_arr[i] = tmp[i];
      else /* Erase elements out of range [0, tmp_index] */
        _excep_arr[i] = excep_null;
    }
  return tmp_index;
}

/* Optional extra */
int
_exception_rearrangement_inplace();

excep_return_e
_exception_quick_match_str(char *a, char *b, bool capital_restricted)
{
  fail(_excep_arr, ABNORMAL);
  fail(a, FAILED);
  fail(b, FAILED);
  fail(&capital_restricted, FAILED);

  int lenA = strlen(a);
  int lenB = strlen(b);

  /* Check length */
  if (lenA != lenB)
    return false;

  for (int i = 0; i < lenA; i ++)
    {
      if (_exception_capital_check(a[i], b[i], capital_restricted != NORMAL))
        return false;
    }
  return true;
}

excep_return_e
_exception_capital_check(char a, char b, bool capital_restricted)
{
  fail(_excep_arr, ABNORMAL);

  return ((capital_restricted) ? (a == b) : ((a - b) == 32));
}
