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

#ifndef _EXCEPTION_H
# define _EXCEPTION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <stdbool.h>

# ifdef __cplusplus
__BEGIN_DECLS
# endif /* __cplusplus */

/* par1="Exception"=_excep_t;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
static const char *__restrict__ _EXCEP_FMT =
"Threw the %s:\n\tat %s:%ld, func %s\n";
static const char *__restrict__ _DEF_EXCEP_FMT = "Threw the %s\n";

# define fail(o, rtn) {if (o == NULL) return (rtn);}
# define voidfail(o) {if (o == NULL) return;}

# ifndef EXCEP_BUFF_MAX
#  define EXCEP_BUFF_MAX 4096L
# endif /* NO EXCEP_BUFF_MAX */

# ifndef EXCEP_ARRAY_MAX
#  define EXCEP_ARRAY_MAX 1024L
# endif /* NO EXCEP_ARRAY_MAX */

# ifndef EXCEP_ID_OFFSET
#  define EXCEP_ID_OFFSET 1000L
# endif /* NO EXCEP_ID_OFFSET */

typedef enum _excep_e
{
  Exception = EXCEP_ID_OFFSET,
  InstanceFailureException,
  IllegalMemoryAccessException,
  InvalidArgumentException,
  OutOfBoundException,
  InvalidNullPointerException,
  OutOfMemoryException,
  BufferOverflowException
} excep_e;

typedef struct _excep_S
{
  char *_name;
  char *_description;
  int _id;
} _excep_t;

static _excep_t excep_null = (_excep_t){0, 0, 0};
static _excep_t *__restrict__ excep_nullptr = &excep_null;

static _excep_t _excep_arr[EXCEP_ARRAY_MAX] = {};
static const int _excep_arr_len = EXCEP_ARRAY_MAX;

/* Compare two exception:_excep_t
   Fails once any given parameter was null.
   Returns 0 once same;
           1 once A > B;
          -1 once A < B;
          -2 once failed. */
static __inline__ int
excep_cmp(_excep_t a, _excep_t b)
{
   fail(&a, -2);
   fail(&b, -2);

   return ((a._id > b._id) ? 1 : (a._id == b._id) ? 0 : -1);
}

/* This function specifically throw BufferOverflowException once given
   BUFF is longer than EXCEP_BUFF_MAX.
   Fails once any given parameter was null.
   Throws BufferOverflowException */
void
_exception_buffersize_chk(char *buff);

/* By specifying the name & the description, an exception can be added
   once none exception has the same name nor the ID.
   Fails once any given parameter was null.
   Returns index to the exception added;
           -1 once _excep_arr was full;
           -2 once failed;
           -3 once _excep_arr had a same exception.
   Throws BufferOverflowException */
int
exception_addexcep(char *excep_name, char *description);

/* By specifying the name & the description, an exception can be added
   once none exception has the same name nor the ID.
   Fails once any given parameter was null;
   Returns index to the exception added;
           -1 once _excep_arr was full;
           -2 once failed;
           -3 once _excep_arr had a same exception.
   Throws BufferOverflowException */
int
exception_addexcep_id(char *excep_name, char *description, int id);

/* By specifying the name, an exception can be removed once it exists.
   Fails once any given parameter was null.
   Returns index to the exception removed;
           -1 once _excep_arr was empty;
           -2 once failed;
           -3 once _excep_arr had no desired exception.
   Throws BufferOverflowException */
int
exception_removeexcep_byname(char *excep_name);

/* By specifying the id, an exception can be removed once it exists.
   Fails once any given parameter was null.
   Returns index to the exception removed;
           -1 once _excep_arr was empty;
           -2 once failed;
           -3 once _excep_arr had no desired exception.
   Throws BufferOverflowException */
int
exception_removeexcep_byid(int id);

/* Returns all exceptions */
_excep_t *
exception_getallexcep();

/* Find desired exception with their names;
   Fails once any given parameter was null.
   Returns index of the exception found;
           -1 once NOT found.
   Throws BufferOverflowException */
int
exception_findexcep_byname(char *excep_name);

/* Find desired exception with their IDs;
   Fails once any given parameter was null.
   Returns index of the exception found;
           -1 once NOT found;
           -2 once failed.
   Throws BufferOverflowException */
int
exception_findexcep_byid(int id);

/* Iterate through every element in _excep_arr, util find specified exception.
   Fails once any given parameter was null.
   Returns the index of matched exception.
           -1 once NOT found;
           -2 once failed. */
int
exception_findexcep_byit(_excep_t e);

/* Iterate through every element in _excep_arr, util find the last exception.
   Returns the index of matched exception.
           -1 once the whole array was empty. */
int
_exception_iteration_last();

/* Iterate through every element in _excep_arr, util find the first exception.
   Returns the index of matched exception.
           -1 once the whole array was empty. */
int
_exception_iteration_first();

/* Rearrange whole array to make all the elements listed in near-by. */
void
_exception_rearrangement();

/* Compare A and B in a quick way.
   That is, once single character does not match, it stops following operations.
   Fails once any given parameter was null.
   Returns matched or not;
           false once failed. */
bool
_exception_quick_match_str(char *a, char *b, bool capital_restricted);

/* Check wether the characters are exactly the same by comparing ASCII value;
   Fails once any given parameter was null.
   Returns true once A is exactly the same as B.
           false once failed. */
bool
_exception_capital_check(char a, char b, bool capital_restricted);

/* Example:
      Required Macros:                ___M1___  ___M2___  _____M3_____

      THROW(InstanceFailureException, __FILE__, __LINE__, __FUNCTION__,
                                      ~~~^^~~~  ~~~^^~~~  ~~~~^^^^~~~~
           "Errored when instancing %s.\nGiven options is illegal:\n\
           %d, %lf", opt1, opt2); */
static __inline__ void
THROW(_excep_t e, const char *__restrict__ __file__, long int __line__,
      const char *__restrict__ __function__, const char *__restrict__ _FMT, ...)
{
  if (_FMT == NULL)
    {
      fprintf(stderr, _DEF_EXCEP_FMT, e._name);
      exit(e._id);
    }

  /* Output secondary description about the thrown exception. */
  va_list _vlist;
  va_start(_vlist, _FMT);
  fprintf(stderr, ((__file__ == NULL && __line__ == -1 && __function__ == NULL)
                   ? _DEF_EXCEP_FMT
                   /* Ignore _FMT when outputting the exception title.
                      Use _EXCEP_FMT instead. */
                   : _EXCEP_FMT), e._name, __file__, __line__, __function__);
  vfprintf(stderr, _FMT, _vlist);
  va_end(_vlist);

  exit(e._id);
}

# ifdef __cplusplus
__END_DECLS
# endif /* __cplusplus */

#endif /* NO _EXCEPTION_H */
