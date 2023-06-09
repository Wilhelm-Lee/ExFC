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

# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# ifdef __cplusplus
__BEGIN_DECLS
# endif /* __cplusplus */

/* par1="Exception"=_excep_t;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
static const char *__restrict__ EXCEP_FMT =
"Threw the %s:\n\tat %s:%ld, func %s\n\"%s\"\n";
static const char *__restrict__ DEF_EXCEP_FMT = "Threw the %s\n";

# ifndef EXCEP_BUFF_MAX
#  define EXCEP_BUFF_MAX 4096
# endif /* NO EXCEP_BUFF_MAX */

# ifndef EXCEP_ARRAY_MAX
#  define EXCEP_ARRAY_MAX 511
# endif /* NO EXCEP_ARRAY_MAX */

# ifndef EXCEP_ID_OFFSET
#  define EXCEP_ID_OFFSET 1
# endif /* NO EXCEP_ID_OFFSET */

typedef enum excep_compare_E
{
  LESS          = -4,
  GREATER       = -3,
  DIFFERENT     = -2,
  IDENTICAL     = -1,
} excep_compare_e;

typedef enum excep_return_E
{
  /* Target is not found. */
  MISSING       = -600,
  /* Certain conditions were not satisfied. */
  CONDITIONAL   = -500,
  /* Failed passing through macro "fail". */
  FAILED        = -400,
  /* Target holds the same element. */
  DUPLICATED    = -300,
  /* Has error occurred. */
  ABNORMAL      = -200,
  /* Normally proceeded with no error occurred */
  NORMAL        = -100,

} excep_return_e;

# define fail(o, rtn) {if ((o) == NULL) return (rtn);}
# define trans(v, e) {if ((v) == (e)) return (e);}

typedef enum _excep_E
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
} __attribute__((packed)) _excep_t;

static const _excep_t excep_null = (_excep_t){"", "", 0};
static const _excep_t *excep_nullptr = &excep_null;

static _excep_t _excep_arr[EXCEP_ARRAY_MAX] = {  };
static const int _excep_arr_len = EXCEP_ARRAY_MAX;;

/* Compare two exception by their ID;
   Fails once any given parameter was null;
   Returns |IDENTICAL once A = B;
           |GREATER   once A > B;
           |LESS      once A < B; */
int
exception_cmp(_excep_t *a, _excep_t *b);

/* By specifying the name & the description & the ID, an exception can be added
     once no same exception exists in advance.
   Fails once any given parameter was null, except $id.
   Returns |index to the exception being added;
           |CONDITIONAL once _excep_arr was full;
           |DUPLICATED once _excep_arr had a same element;
           |ABNORMAL once $_excep_arr was null;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
exception_addexcep(const char *excep_name, const char *description, int id);

/* By specifying the name, an exception can be removed once it exists.
   Fails once any given parameter was null.
   Returns |index to the exception being removed;
           |MISSING once _excep_arr had no desired exception;
           |CONDITIONAL once _excep_arr was empty;
           |FAILED once id < 0;
           |ABNORMAL once $_excep_arr was null;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
exception_removeexcep_byname(const char *excep_name);

/* By specifying the id, an exception can be removed once it exists.
   Returns |index to the exception being removed;
           |MISSING once _excep_arr had no desired exception;
           |CONDITIONAL once _excep_arr was empty;
           |ABNORMAL once $_excep_arr was null;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
exception_removeexcep_byid(int id);

/* Returns |all exceptions;
           |$excep_nullptr once _excep_arr is NULL;
   Throws InvalidNullPointerException; */
_excep_t *
exception_getallexcep();

/* Find desired exception with its name;
   Fails once any given parameter was null.
   Returns |index of the exception being found;
           |MISSING once NOT found;
           |ABNORMAL once $_excep_arr was null;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
exception_findexcep_byname(const char *excep_name);

/* Find desired exception with its ID;
   Fails once any given parameter was null.
   Returns |index of the exception being found;
           |MISSING once NOT found;
           |ABNORMAL once $_excep_arr was null;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
exception_findexcep_byid(int id);

/* Iterate through every element in _excep_arr, util find specified exception.
   Fails once any given parameter was null.
   Returns |index of matched exception.
           |EXCEP_NULL once $e was excep_null,
            which is not allowed to operate on it;
           |MISSING once NOT found;
           |FAILED once id < 0
                   once operating target is $excep_null;
           |ABNORMAL once $_excep_arr was null;
   Throws InvalidNullPointerException; */
int
exception_findexcep_byit(_excep_t e);

/* Iterate through every element in _excep_arr, util find the last exception.
   Returns |index of matched exception.
           |CONDITIONAL once the whole array was empty;
           |ABNORMAL once $_excep_arr was null;
   Throws InvalidNullPointerException; */
int
_exception_iteration_last();

/* Iterate through every element in _excep_arr, util find the first exception.
   Returns |index of matched exception.
           |CONDITIONAL once the whole array was empty;
           |ABNORMAL once $_excep_arr was null;
   Throws InvalidNullPointerException; */
int
_exception_iteration_first();

/* Rearrange whole array to make all the elements listed in near-by.
   Returns |real length of _excep_arr after rearrangement;
           |ABNORMAL once $_excep_arr was null;
   Throws InvalidNullPointerException; */
int
_exception_rearrangement();

/* Rearrange whole array to make all the elements listed in near-by without
     extra space used.
   Returns |real length of _excep_arr after rearrangement;
           |ABNORMAL once $_excep_arr was null;
   Throws InvalidNullPointerException; */
int
_exception_rearrangement_inplace();

/* Compare A and B in a quick way.
   That is, once single character does not match, it stops following operations.
   Fails once any given parameter was null, except $capital_restricted.
   Transacts |FAILED   from _exception_capital_check(char, char, bool);
             |ABNORMAL from _exception_capital_check(char, char, bool);
   Returns |IDENTICAL once matched;
           |DIFFERENT once did not match. */
int
_exception_quick_match_str(const char *a, const char *b,
                           bool capital_restricted);

/* Check whether the characters are exactly the same by comparing their ASCII
     values;
   Returns |IDENTICAL once A is exactly the same as B;
           |DIFFERENT once A is not exactly the same as B; */
int
_exception_capital_check(char a, char b, bool capital_restricted);

/* This function specifically throw BufferOverflowException once given
     BUFF is longer than EXCEP_BUFF_MAX.
   Fails once any given parameter was null.
   Returns |FAILED   once failed passing through macro "fail";
           |ABNORMAL once $_excep_arr was null;
           |NORMAL   once normally proceeded with no error occurred;
   Throws BufferOverflowException;
          InvalidNullPointerException; */
int
_exception_buffersize_chk(char *buff);

/* Swap specified two element from $_excep_arr.
   Fails once any given parameter was null.
   Returns |FAILED once failed passing through macro "fail";
           |NORMAL normally proceeded with no error occurred; */
int
_exception_swap(_excep_t *a, _excep_t *b);

/* Example:
      Required Macros:                ___M1___  ___M2___  _____M3_____

      THROW(InstanceFailureException, __FILE__, __LINE__, __FUNCTION__,
                                      ~~~^^~~~  ~~~^^~~~  ~~~~^^^^~~~~
           "Errored when instancing %s.\nGiven options is illegal:\n\
           %d, %lf", opt1, opt2); */
static inline void
THROW(_excep_t *e, const char *__restrict__ _file_, long int _line_,
      const char *__restrict__ _function_, const char *__restrict__ FMT)
{
  if (FMT == NULL)
    {
      (void)fprintf(stderr, DEF_EXCEP_FMT, e->_name);
      exit(e->_id);
    }

  (void)fprintf(stderr, ((_file_ == NULL && _line_ == -1 && _function_ == NULL)
                   ? DEF_EXCEP_FMT
                   /* Ignore _FMT when outputting the exception title.
                      Use _EXCEP_FMT instead. */
                   : EXCEP_FMT), e->_name, _file_, _line_, _function_,
                                 e->_description);

  exit(e->_id);
}

# ifdef __cplusplus
__END_DECLS
# endif /* __cplusplus */

#endif /* NO _EXCEPTION_H */
