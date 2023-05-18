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

  _exception_nullchk();

  if (strlen((const char *)buff) > EXCEP_BUFF_MAX)
    {
      THROW((_excep_t){"BufferOverflowException", "", BufferOverflowException},
            __FILE__, __LINE__, __FUNCTION__, EXCEP_FMT);
    }

  return NORMAL;
}

excep_return_e
exception_addexcep(char *excep_name, char *description, int id)
{
  if (id < 0)
    {
      return FAILED;
    }

  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);
  fail(description, FAILED);
  fail(&id, FAILED);

  _exception_nullchk();

  _exception_buffersize_chk(excep_name);
  _exception_buffersize_chk(description);

  /* Found the duplication, exit. */
  if (exception_findexcep_byname(excep_name) != CONDITIONAL
      || exception_findexcep_byid(id) != CONDITIONAL)
    {
      return DUPLICATED;
    }

  int index = _exception_rearrangement();

  /* Array was full */
  if (index == _excep_arr_len)
    {
      return CONDITIONAL;
    }

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

  _exception_nullchk();

  _exception_buffersize_chk(excep_name);

  /* Find the desired exception */
  int index = exception_findexcep_byname(excep_name);

  /* Not found */
  if (index == MISSING)
    {
      return MISSING;
    }

  /* Assign */
  _excep_arr[index]._name = NULL;
  _excep_arr[index]._description = NULL;
  _excep_arr[index]._id = 0;

  return index;
}

int
exception_removeexcep_byid(int id)
{
  if (id < 0)
    {
      return FAILED;
    }

  fail(_excep_arr, ABNORMAL);
  fail(&id, FAILED);

  _exception_nullchk();

  /* Find the desired exception */
  int index = exception_findexcep_byid(id);

  /* Not found */
  if (index == MISSING)
    {
      return MISSING;
    }

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

  _exception_nullchk();

  /* Ensure no gaps exists */
  int rtnlen = _exception_rearrangement();

  _excep_t rtnarr[rtnlen];
  _excep_t *rtn = rtnarr;

  for (register int i = 0; i < rtnlen; i ++)
    {
      rtnarr[i] = _excep_arr[i];
    }

  return rtn;
}

excep_return_e
exception_findexcep_byname(char *excep_name)
{
  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);

  _exception_nullchk();

  _exception_buffersize_chk(excep_name);

  for (register int i = 0; i < _excep_arr_len - 1; i ++)
    {
      /* Once matched, return the index. */
      if (_exception_quick_match_str(excep_name, _excep_arr[i]._name, true))
        {
          return i;
        }
    }
  return MISSING;
}

excep_return_e
exception_findexcep_byid(int id)
{
  fail(_excep_arr, ABNORMAL);

  _exception_nullchk();

  if (id < 0)
    {
      return FAILED;
    }

  for (register int i = 0; i < _excep_arr_len; i ++)
    {
      /* Once matched, return the index. */
      if (id == _excep_arr[i]._id)
        {
          return i;
        }
    }
  return MISSING;
}

excep_return_e
exception_findexcep_byit(_excep_t e)
{
  fail(_excep_arr, ABNORMAL);
  fail(&e, FAILED);

  _exception_nullchk();

  if (exception_cmp(&e, (_excep_t*)excep_nullptr))
    {
      return EXCEP_NULL;
    }

  for (register int i = 0; i < _excep_arr_len - 1; i ++)
    {
      /* Once matched, return the index. */
      if (_exception_quick_match_str(e._name, _excep_arr[i]._name, true)
          && (e._id == _excep_arr[i]._id))
        {
          return i;
        }
    }
  return MISSING;
}

excep_return_e
_exception_iteration_last()
{
  fail(_excep_arr, ABNORMAL);

  _exception_nullchk();

  int rtn = -1;
  for (register int i = _excep_arr_len - 1; i >= 0; i --)
    {
      /* Finding non-null elements, mark on. */
      if (&_excep_arr[i] != NULL)
        {
          rtn = i;
        }
    }
  /* Once reached at the beginning, return the index. */
  return rtn;
}

excep_return_e
_exception_iteration_first()
{
  fail(_excep_arr, ABNORMAL);

  _exception_nullchk();

  int rtn = -1;
  for (register int i = 0; i < _excep_arr_len; i ++)
    {
      /* Finding non-null elements, mark on. */
      if (&_excep_arr[i] != NULL)
        {
          rtn = i;
        }
    }
  /* Once reached at the end, return the index. */
  return rtn;
}

int
_exception_rearrangement()
{
  fail(_excep_arr, ABNORMAL);

  _exception_nullchk();

  _excep_t tmp[_excep_arr_len];

  int tmp_index = 0;
  for (int arr_index = 0; arr_index < _excep_arr_len; arr_index ++)
    {
      if (exception_cmp(&_excep_arr[arr_index], (_excep_t *)excep_nullptr) == 0)
        {
          continue;
        }

      /* Not null. Write this element into array tmp. */
      tmp[tmp_index] = _excep_arr[arr_index];
      tmp_index += 1;
    }
  /* Apply tmp on _excep_arr */
  for (register int i = 0; i < _excep_arr_len; i ++)
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
        {
          _excep_arr[i] = tmp[i];
        }
      else /* Erase elements out of range [0, tmp_index] */
        {
          _excep_arr[i] = excep_null;
        }
    }
  return tmp_index;
}

int
_exception_rearrangement_inplace()
{
  fail(_excep_arr, ABNORMAL);

  _exception_nullchk();

  /* Ensure target is not just a whole empty array, or a full filled array. */
  int first = _exception_iteration_first();
  int last = _exception_iteration_last();
  /* If _excep_arr is just empty on the whole, or have been full filled,
     return normally. */
  if (first == CONDITIONAL || last == CONDITIONAL)
    {
      return NORMAL;
    }

  /* Start rearranging */
  int cnt = 0;
  int record_before_following = -1;
  for (register int i = 1; i < _excep_arr_len; i ++)
    {
      /* A not empty element */
      int compare_this = exception_cmp(&_excep_arr[i], (_excep_t *)excep_nullptr);
      int compare_prev = exception_cmp(&_excep_arr[i - 1],
                                       (_excep_t *)excep_nullptr);

      /* While the previous element is empty. */
      if (compare_this != 0 && compare_prev == 0)
        {
          /* Move this element backwards for one. */
          _exception_swap(&_excep_arr[i], &_excep_arr[i - 1]);
          /* Record current index */
          record_before_following = i;
          /* Follow up */
          i -= 1;
        }

      /* While ALL of those two were NOT empty. */
      if (compare_this != 0 && compare_prev != 0)
        {
          /* Go back to marked index if possible. */
          if (record_before_following > i)
            {
              i = record_before_following;
            }

          continue;
        }

      /* While ALL of those two were empty. */
      if (compare_this == 0 && compare_prev == 0)
        {
          /* If it was because of reaching at the end. */
          if (i == _excep_arr_len)
            {
              /* End the process */
              break;
              /* If it was NOT because of reaching at the end. */
              continue;
            }
        }

      /* While this element is empty. */
      if (compare_this == 0 && compare_prev != 0)
        {
          cnt += 1;
        }
    }
  return cnt;
}

excep_return_e
_exception_quick_match_str(char *a, char *b, bool capital_restricted)
{
  fail(_excep_arr, ABNORMAL);
  fail(a, FAILED);
  fail(b, FAILED);
  fail(&capital_restricted, FAILED);

  _exception_nullchk();

  const unsigned long lenA = strlen(a);
  const unsigned long lenB = strlen(b);

  /* Check length */
  if (lenA != lenB)
    {
      return false;
    }

  for (register unsigned long i = 0; i < lenA; i ++)
    {
      if (_exception_capital_check(a[i], b[i], capital_restricted) == NORMAL)
        {
          return false;
        }
    }
  return true;
}

excep_return_e
_exception_capital_check(char a, char b, bool capital_restricted)
{
  fail(_excep_arr, ABNORMAL);

  return ((capital_restricted) ? (a == b) : ((a - b) == ('a' - 'A')));
}
