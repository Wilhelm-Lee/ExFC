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

int
exception_addexcep(const char *excep_name, const char *description, int id)
{
  if (id < 0)
    {
      return FAILED;
    }

  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);
  fail(description, FAILED);
  fail(&id, FAILED);

  _exception_buffersize_chk((char *)excep_name);
  _exception_buffersize_chk((char *)description);

  const int byname = exception_findexcep_byname(excep_name);
  const int byid = exception_findexcep_byid(id);

  trans(byname, FAILED);
  trans(byid, ABNORMAL);

  /* Found the duplication, exit. */
  if (byname != MISSING || byid != MISSING)
    {
      return DUPLICATED;
    }

  /* Ensure no gaps exists */
  int rearrange = _exception_rearrangement();

  /* Array was full */
  if (rearrange == _excep_arr_len)
    {
      return CONDITIONAL;
    }

  /* Assign */
  _excep_arr[rearrange]._name = (char *)excep_name;
  _excep_arr[rearrange]._description = (char *)description;
  _excep_arr[rearrange]._id = id;

  return rearrange;
}

int
exception_removeexcep_byname(const char *excep_name)
{
  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);

  _exception_buffersize_chk((char *)excep_name);

  /* Find the desired exception */
  int byname = exception_findexcep_byname(excep_name);

  /* Not found */
  trans(byname, MISSING);

  /* Assign */
  _excep_arr[byname]._name = NULL;
  _excep_arr[byname]._description = NULL;
  _excep_arr[byname]._id = 0;

  return byname;
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

  /* Find the desired exception */
  int byid = exception_findexcep_byid(id);

  /* Not found */
  trans(byid, MISSING);

  /* Assign */
  _excep_arr[byid]._name = NULL;
  _excep_arr[byid]._description = NULL;
  _excep_arr[byid]._id = 0;

  return byid;
}

_excep_t *
exception_getallexcep()
{
  fail(_excep_arr, (_excep_t*)excep_nullptr);

  /* Ensure no gaps exists */
  int rearrange = _exception_rearrangement();

  _excep_t rtnarr[rearrange];
  _excep_t *rtn = rtnarr;

  for (register int i = 0; i < rearrange; i ++)
    {
      rtnarr[i] = _excep_arr[i];
    }

  return rtn;
}

int
exception_findexcep_byname(const char *excep_name)
{
  fail(_excep_arr, ABNORMAL);
  fail(excep_name, FAILED);

  _exception_buffersize_chk((char *)excep_name);

  for (register int i = 0; i < _excep_arr_len - 1; i ++)
    {
      int match = _exception_quick_match_str(excep_name, _excep_arr[i]._name,
                                             true);
      /* Once failed, fail. */
      if (match == FAILED)
        {
          return FAILED;
        }

      /* Once matched, return the index. */
      if (match == IDENTICAL)
        {
          return i;
        }
    }
  return MISSING;
}

int
exception_findexcep_byid(int id)
{
  fail(_excep_arr, ABNORMAL);

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

int
exception_findexcep_byit(_excep_t e)
{
  fail(_excep_arr, ABNORMAL);
  fail(&e, FAILED);

  if (exception_cmp(&e, (_excep_t*)excep_nullptr) == IDENTICAL)
    {
      return FAILED;
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

int
_exception_iteration_last()
{
  fail(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (register int i = _excep_arr_len - 1; i >= 0; i --)
    {
      /* Finding non-null elements, mark on. */
      if (exception_cmp(&_excep_arr[i], (_excep_t *)excep_nullptr) != IDENTICAL)
        {
          rtn = i;
        }
    }
  /* Once reached at the beginning, return the index. */
  return rtn;
}

int
_exception_iteration_first()
{
  fail(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (register int i = 0; i < _excep_arr_len; i ++)
    {
      /* Finding non-null elements, mark on. */
      if (exception_cmp(&_excep_arr[i], (_excep_t*)excep_nullptr) != IDENTICAL)
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

  _excep_t tmp[_excep_arr_len];

  int tmp_index = 0;
  for (int arr_index = 0; arr_index < _excep_arr_len; arr_index ++)
    {
      /* Continue once empty */
      if (exception_cmp(&_excep_arr[arr_index],
                        (_excep_t *)excep_nullptr) == IDENTICAL)
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

  /* Ensure target is not just a whole empty array, or a full filled array. */
  int first = _exception_iteration_first();
  int last = _exception_iteration_last();

  trans(first, FAILED);
  trans(first, ABNORMAL);
  trans(last, FAILED);
  trans(last, ABNORMAL);

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
      int compare_this = exception_cmp(&_excep_arr[i],
                                       (_excep_t *)excep_nullptr);
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
            }
          /* If it was NOT because of reaching at the end. */
          continue;
        }

      /* While this element is empty. */
      if (compare_this == 0 && compare_prev != 0)
        {
          cnt += 1;
        }
    }
  return cnt;
}

int
_exception_quick_match_str(const char *a, const char *b,
                           bool capital_restricted)
{

  fail(_excep_arr, ABNORMAL);
  fail(a, FAILED);
  fail(b, FAILED);

  const unsigned long lenA = strlen(a);
  const unsigned long lenB = strlen(b);

  /* Check length */
  if (lenA != lenB)
    {
      return DIFFERENT;
    }

  for (register unsigned long i = 0; i < lenA; i ++)
    {
      if (_exception_capital_check(a[i], b[i], capital_restricted) == NORMAL)
        {
          return DIFFERENT;
        }
    }
  return IDENTICAL;
}

int
_exception_capital_check(char a, char b, bool capital_restricted)
{
  fail(_excep_arr, ABNORMAL);

  return ((capital_restricted) ? (a == b) : ((a - b) == ('a' - 'A'))
          ? IDENTICAL
          : DIFFERENT);
}

int
_exception_buffersize_chk(char *buff)
{
  fail(buff, FAILED);

  if (strlen((const char *)buff) > EXCEP_BUFF_MAX)
    {
      THROW(&(_excep_t){"BufferOverflowException", "", BufferOverflowException},
            __FILE__, __LINE__, __FUNCTION__, EXCEP_FMT);
    }

  return NORMAL;
}

int
_exception_swap(_excep_t *a, _excep_t *b)
{
  fail(a, FAILED);
  fail(b, FAILED);

  _excep_t *c = a;
  a = b;
  b = c;

  return NORMAL;
}
