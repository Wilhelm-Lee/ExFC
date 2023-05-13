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

void
_exception_buffersize_chk(char *buff)
{
  voidfail(buff);

  if (strlen((const char *)buff) > EXCEP_ARRAY_MAX)
    {
      THROW((_excep_t){"BufferOverflowException", "", BufferOverflowException},
            __FILE__, __LINE__, __FUNCTION__, _EXCEP_FMT);
    }
}

int
exception_addexcep(char *excep_name, char *description)
{
  fail(excep_name, -2);
  fail(description, -2);

  _exception_buffersize_chk(excep_name);
  _exception_buffersize_chk(description);

  /* Found the duplication, exit. */
  if (exception_findexcep_byname(excep_name) != -1)
    return -3;

  _exception_rearrangement();
  int pivot = _exception_iteration_first();

  /* Array was full */
  if (pivot == -1)
    return -1;

  /* Assign */
  _excep_arr[pivot]._name = excep_name;
  _excep_arr[pivot]._description = description;
  _excep_arr[pivot]._id = pivot + EXCEP_ID_OFFSET;

  return pivot;
}

int
exception_addexcep_id(char *excep_name, char *description, int id)
{
  fail(excep_name, -2);
  fail(description, -2);
  fail(&id, -2);

  _exception_buffersize_chk(excep_name);
  _exception_buffersize_chk(description);

  /* Found the duplication, exit. */
  if (exception_findexcep_byname(excep_name) != -1
      || exception_findexcep_byid(id) != -1)
    return -3;

  _exception_rearrangement();
  int pivot = _exception_iteration_first();

  /* Array was full */
  if (pivot == -1)
    return -1;

  /* Assign */
  _excep_arr[pivot]._name = excep_name;
  _excep_arr[pivot]._description = description;
  _excep_arr[pivot]._id = id;

  return pivot;
}

int
exception_removeexcep_byname(char *excep_name);/* YOU LEFT HERE */

int
exception_removeexcep_byid(int id);

_excep_t *
exception_getallexcep();

int
exception_findexcep_byname(char *excep_name);

int
exception_findexcep_byid(int id);

int
exception_findexcep_byit(_excep_t e)
{
  fail(&e, -1);

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
_exception_iteration_last()
{
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

int
_exception_iteration_first()
{
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

void
_exception_rearrangement()
{
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
      /* Overwrite previous element by the ones in array tmp. */
      if (i <= tmp_index)
        _excep_arr[i] = tmp[i];
      else /* Erase elements out of range [0, tmp_index] */
        _excep_arr[i] = excep_null;
    }
}

bool
_exception_quick_match_str(char *a, char *b, bool capital_restricted)
{
  fail(a, false);
  fail(b, false);
  fail(&capital_restricted, false);

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

bool
_exception_capital_check(char a, char b, bool capital_restricted)
{
  fail(&a, false);
  fail(&b, false);
  fail(&capital_restricted, false);

  return ((capital_restricted) ? (a == b) : ((a - b) == 32));
}
