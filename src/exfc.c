/**
 *     This file is part of project <https://github.com/Wilhelm-Lee/ExFC>
 *     Copyright (C) 2022 - 2023  William Lee
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
 *
 * @version Alpha 0.0.0
 * @author William Lee
 */

#include "exfc.h"

int
exfc_cmp(_excep_t *a, _excep_t *b)
{
  fails(a, FAILED);
  fails(b, FAILED);

  return ((a->_id > b->_id)
         ? GREATER : (a->_id == b->_id)
            ? IDENTICAL : LESS);
}

int
exfc_addexcep(const char *name, const char *description, int id)
{
  if (id < 0)
    {
      return CONDITIONAL;
    }

  /* TEST: seek out for $_excep_arr[$i]._name */
  (void)fprintf(stdout, "addexcep: %p->%s\n", &_excep_arr[0]._name,
                _excep_arr[0]._name);
  /* TEST OVER */

  fails(_excep_arr, ABNORMAL);
  fails(name, FAILED);
  fails(description, FAILED);

  _exfc_buffersize_chk((char *)name);
  _exfc_buffersize_chk((char *)description);

  const int byname = exfc_getindex_byname(name);
  trans(byname, FAILED);

  const int byid = exfc_getindex_byid(id);
  trans(byid, ABNORMAL);

  /* Found the duplication, exit. */
  if (byname != MISSING || byid != MISSING)
    {
      return DUPLICATED;
    }

  /* Ensure no gaps exists */
  int rearrange = _exfc_rearrangement();

  /* Array was full */
  if (rearrange == _excep_arr_len)
    {
      return CONDITIONAL;
    }

  /* Assign */
  _excep_arr[rearrange]._name = (char *)name;
  _excep_arr[rearrange]._description = (char *)description;
  _excep_arr[rearrange]._id = id;

  return rearrange;
}

int
_exfc_addexcep_test(const void *name, const void *description, int id)
{
  const char *_name = (const char *)name;
  const char *_description = (const char *)description;

  /* TEST: seek $_excep_arr[$i]._name */
  (void)fprintf(stdout, "%s\n%s\n%d\n_excep_arr[0]:\
", _name, _description, id);
  /* TEST OVER */

  fails(_excep_arr, ABNORMAL);
  fails(name, FAILED);
  fails(description, FAILED);

  _exfc_buffersize_chk((char *)name);
  _exfc_buffersize_chk((char *)description);

  const int byname = exfc_getindex_byname(name);
  trans(byname, FAILED);

  const int byid = exfc_getindex_byid(id);
  trans(byid, ABNORMAL);

  /* Found the duplication, exit. */
  if (byname != MISSING || byid != MISSING)
    {
      return DUPLICATED;
    }

  /* Ensure no gaps exists */
  int rearrange = _exfc_rearrangement();

  /* Array was full */
  if (rearrange == _excep_arr_len)
    {
      return CONDITIONAL;
    }

  /* Assign */
  _excep_arr[rearrange]._name = (char *)name;
  _excep_arr[rearrange]._description = (char *)description;
  _excep_arr[rearrange]._id = id;

  return rearrange;

}

int
exfc_removeexcep_byname(const char *name)
{
  fails(_excep_arr, ABNORMAL);
  fails(name, FAILED);

  _exfc_buffersize_chk((char *)name);

  /* Find the desired exception */
  int byname = exfc_getindex_byname(name);

  /* Not found */
  trans(byname, MISSING);

  /* Assign */
  _excep_arr[byname]._name = NULL;
  _excep_arr[byname]._description = NULL;
  _excep_arr[byname]._id = 0;

  return byname;
}

int
exfc_removeexcep_byid(int id)
{
  if (id < 0)
    {
      return FAILED;
    }

  fails(_excep_arr, ABNORMAL);

  /* Find the desired exception */
  int byid = exfc_getindex_byid(id);

  /* Not found */
  trans(byid, MISSING);

  /* Assign */
  _excep_arr[byid]._name = NULL;
  _excep_arr[byid]._description = NULL;
  _excep_arr[byid]._id = 0;

  return byid;
}

_excep_t *
exfc_getallexcep()
{
  fails(_excep_arr, (_excep_t*)excep_nullptr);

  /* Ensure no gaps exists */
  int rearrange = _exfc_rearrangement();

  _excep_t rtnarr[rearrange];
  _excep_t *rtn = rtnarr;

  for (register int i = 0; i < rearrange; i ++)
    {
      rtnarr[i] = _excep_arr[i];
    }

  return rtn;
}

int
exfc_getindex_byname(const char *name)
{
  fails(_excep_arr, ABNORMAL);
  fails(name, FAILED);

  // _exfc_buffersize_chk((char *)name);

  /* TEST: seek $_excep_arr[$i]._name */
  (void)fprintf(stdout, "getindex_byname: %s\n", _excep_arr[0]._name);
  /* TEST OVER */

  for (register int i = 0; i < _excep_arr_len - 1; i ++)
    {
      /* TEST: seek $i, $_excep_arr[$i]._name */
      (void)fprintf(stdout, "%d\n%s", i, _excep_arr[i]._name);
      /* TEST OVER */

      const int match
        = _exfc_quick_match_str(name, _excep_arr[i]._name, true);

      /* Once failed, fail. */
      trans(match, FAILED);

      /* Once matched, return the index. */
      if (match == IDENTICAL)
        {
          return i;
        }
    }
  return MISSING;
}

int
exfc_getindex_byid(int id)
{
  fails(_excep_arr, ABNORMAL);

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
exfc_getindex_byexcep(_excep_t e)
{
  fails(_excep_arr, ABNORMAL);
  fails(&e, FAILED);

  if (exfc_cmp(&e, excep_nullptr) == IDENTICAL)
    {
      return FAILED;
    }

  for (register int i = 0; i < _excep_arr_len - 1; i ++)
    {
      const int match = _exfc_quick_match_str(e._name, _excep_arr[i]._name,
                                                   true);

      /* Once matched, return the index. */
      if (match == IDENTICAL && (e._id == _excep_arr[i]._id))
        {
          return i;
        }
    }
  return MISSING;
}

int
_exfc_iteration_last()
{
  fails(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (register int i = _excep_arr_len - 1; i >= 0; i --)
    {
      const int cmp = exfc_cmp(&_excep_arr[i], excep_nullptr);

      trans(cmp, FAILED);

      /* Finding non-null elements, mark on. */
      if (cmp != IDENTICAL)
        {
          rtn = i;
        }
    }
  /* Once reached at the beginning, return the index. */
  return rtn;
}

int
_exfc_iteration_first()
{
  fails(_excep_arr, ABNORMAL);

  int rtn = -1;
  for (register int i = 0; i < _excep_arr_len; i ++)
    {
      const int cmp = exfc_cmp(&_excep_arr[i], excep_nullptr);

      trans(cmp, FAILED);

      /* Finding non-null elements, mark on. */
      if (cmp != IDENTICAL)
        {
          rtn = i;
        }
    }
  /* Once reached at the end, return the index. */
  return rtn;
}

int
_exfc_rearrangement()
{
  fails(_excep_arr, ABNORMAL);

  _excep_t tmp[_excep_arr_len];

  int tmp_index = 0;
  for (int arr_index = 0; arr_index < _excep_arr_len; arr_index ++)
    {
      const int cmp = exfc_cmp(&_excep_arr[arr_index], excep_nullptr);

      trans(cmp, FAILED);

      /* Continue once empty */
      if (cmp == IDENTICAL)
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
_exfc_rearrangement_inplace()
{
  fails(_excep_arr, ABNORMAL);

  /* Ensure target is not just a whole empty array, or a full filled array. */
  const int first = _exfc_iteration_first();
  const int last = _exfc_iteration_last();

//  trans(first, FAILED);
//  trans(first, ABNORMAL);
//  trans(last, FAILED);
//  trans(last, ABNORMAL);

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
      int cmp_this = exfc_cmp(&_excep_arr[i], excep_nullptr);
      int cmp_prev = exfc_cmp(&_excep_arr[i - 1], excep_nullptr);

      /* While the previous element is empty. */
      if (cmp_this != 0 && cmp_prev == 0)
        {
          /* Move this element backwards for one. */
          const int swap = _exfc_swap(&_excep_arr[i], &_excep_arr[i - 1]);

          trans(swap, FAILED);

          /* Record current index */
          record_before_following = i;
          /* Follow up */
          i -= 1;
        }

      /* While ALL of those two were NOT empty. */
      if (cmp_this != 0 && cmp_prev != 0)
        {
          /* Go back to marked index if possible. */
          if (record_before_following > i)
            {
              i = record_before_following;
            }

          continue;
        }

      /* While ALL of those two were empty. */
      if (cmp_this == 0 && cmp_prev == 0)
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
      if (cmp_this == 0 && cmp_prev != 0)
        {
          cnt += 1;
        }
    }
  return cnt;
}

int
_exfc_quick_match_str(const char *a, const char *b,
                      bool capital_restricted)
{

  /* TEST: seek $_excep_arr[$i]._name */
  (void)fprintf(stdout, "quick_match_str: %s\n", _excep_arr[0]._name);
  /* TEST OVER */

  fails(_excep_arr, ABNORMAL);
  fails(a, FAILED);
  fails(b, FAILED);

  const unsigned long lenA = strlen(a);
  const unsigned long lenB = strlen(b);

  /* Check length */
  if (lenA != lenB)
    {
      return DIFFERENT;
    }

  for (register unsigned long i = 0; i < lenA; i ++)
    {
      const int check = _exfc_capital_check(a[i], b[i], capital_restricted);

      trans(check, ABNORMAL);

      if (check == NORMAL)
        {
          return DIFFERENT;
        }
    }
  return IDENTICAL;
}

int
_exfc_capital_check(char a, char b, bool capital_restricted)
{
  fails(_excep_arr, ABNORMAL);

  return ((capital_restricted) ? (a == b) : ((a - b) == ('a' - 'A'))
          ? IDENTICAL
          : DIFFERENT);
}

int
_exfc_buffersize_chk(char *buff)
{
  fails(buff, FAILED);

  if (strlen((const char *)buff) > EXCEP_BUFF_MAX)
    {
      THROW(&(_excep_t){"BufferOverflowException", "", BufferOverflowException},
            __FILE__, __LINE__, __FUNCTION__, EXCEP_FMT);
    }

  return NORMAL;
}

int
_exfc_swap(_excep_t *a, _excep_t *b)
{
  _excep_t *c = a;
  a = b;
  b = c;

  return NORMAL;
}
