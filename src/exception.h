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

#ifdef __cplusplus
__BEGIN_DECLS
#endif /* __cplusplus */

/* par1="Exception"=_excep_e;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
static const char *__restrict__ _EXCEP_FMT =
"Threw the %s:\n\tat %s:%ld, func %s\n";
static const char *__restrict__ _DEF_EXCEP_FMT = "Threw the %s\n";

typedef enum _excep_e
{
  InstanceFailureException = 5000,
  IllegalMemoryAccessException,
  InvalidArgumentException,
  OutOfBoundException,
  InvalidNullPointerException,
  OutOfMemoryException,
  BufferOverflowException
} excep_e;

/* These exceptions would become a super class which is so-called
   "Abstract Class". */
static __inline__ const char *
__excep_etos(excep_e e)
{
  switch(e)
    {
      /* Once malloc returns NULL, this exception could stop the program
      for good. */
      case InstanceFailureException:
        return "InstanceFailureException";
      /* When in "_var.h" and "_array.h" ..., it is common to have problems
         related to bounding restrictions. Therefor, to protect a programme
         by throwing this exception would be a better solution than just
         having a "Segmentation Fault (Core dumped)" in the end of the day. */
      case IllegalMemoryAccessException:
        return "IllegalMemoryAccessException";
      /* When passing through a function with given parameters, it is critical
         to check whether they are qualified for the function to take in.
         For example, if function "void f(int a)" requires parameter "a"
         NOT to be negative, then we need to throw this exception for safety.
         This exception is called on first once InvalidNullPointerException
         involves.
      */
      case InvalidArgumentException:
        return "InvalidArgumentException";
      /* When in an array, a pointer must NOT go out of the scheduled
         bound that limits the actual physical sizes on the memory.
         This exception is kind of similarity of IllegalMemoryAccessException.
         However, when using "_var.h" and "_array.h", there would be more
         complex situations that puts variables into a conception of
         generalisation for different size for one single variable.
         This requires non-system-builtin but systematic check mechanics to
         kick in. */
      case OutOfBoundException:
        return "OutOfBoundException";
      /* In some particular cases, you would meet on some scenarios
         that restricts the target to NOT be nulled. */
      case InvalidNullPointerException:
        return "InvalidNullPointerException";
      /* When allocating with allocating functions, usually those who returns
      NULL means failure of allocations. Out of memory specifically is the major
      caution. And we use this exception to identify them. */
      case OutOfMemoryException:
        return "OutOfMemoryException";
      /* Thrown once internal buffer size was exceeded. */
      case BufferOverflowException:
        return "BufferOverflowException";
      /* To be able to throw this exception, you must have typo in
         the name of targeting exception, or simply you just passed an
         unknown exception into this function. Therefor, it is no longer
         the responsibility for this function to "guess" which exception
         you were meant to throw. And UnknownException it is to be thrown. */
      default:
        return "UnknownException";
    }
}

/* Example:
      Required Macros:                ___M1___  ___M2___  _____M3_____

      THROW(InstanceFailureException, __FILE__, __LINE__, __FUNCTION__,
                                      ~~~^^~~~  ~~~^^~~~  ~~~~^^^^~~~~
           "Errored when instancing %s.\nGiven options is illegal:\n\
           %d, %lf", opt1, opt2); */
static __inline__ void
THROW(excep_e e, const char *__restrict__ __file__, long int __line__,
      const char *__restrict__ __function__, const char *__restrict__ _FMT, ...)
{
  if (_FMT == NULL)
    {
      fprintf(stderr, _DEF_EXCEP_FMT, __excep_etos(e));
      exit(e);
    }

  /* Output secondary description about the thrown exception. */
  va_list _vlist;
  va_start(_vlist, _FMT);
  fprintf(stderr, ((__file__ == NULL && __line__ == -1 && __function__ == NULL)
                   ? _DEF_EXCEP_FMT
                   /* Ignore _FMT when outputting the exception title.
                      Use _EXCEP_FMT instead. */
                   : _EXCEP_FMT), __excep_etos(e), __file__, __line__, __function__);
  vfprintf(stderr, _FMT, _vlist);
  va_end(_vlist);

  exit(e);
}

/* Throws InvalidNullPointerException */
#define nullchk(o) {if (o == NULL) THROW(InvalidNullPointerException,\
__FILE__, __LINE__, __FUNCTION__, "Threw the %s:\n\tat %s:%ld, func %s\n\t\
#o should NOT be nulled as NULL being represented as a value of invalidation\
 in COOL.");}

# ifdef EXCEP_BUFF_MAX
#  undef EXCEP_BUFF_MAX
# endif /* EXCEP_BUFF_MAX */
# define EXCEP_BUFF_MAX 4096L

typedef struct _excep_S
{
  char _name[EXCEP_BUFF_MAX];
  char _description[EXCEP_BUFF_MAX];
  int id;
} _excep_t;

/* By specifying the name & the description, an exception can be added
   once none exception has the same name.
   Fails once any given parameter was null.
   Throws BufferOverflowException */
void
exception_addexcep(char *excep_name, char *description);

/* By specifying the name, an exception can be removed once it exists.
   Fails once any given parameter was null.
   Throws BufferOverflowException */
void
exception_removeexcep_byname(char *excep_name);

/* By specifying the id, an exception can be removed once it exists.
   Fails once any given parameter was null.
   Throws BufferOverflowException */
void
exception_removeexcep_byid(excep_e id);

/* Returns the name to all exceptions */
char **
exception_getallexcep();

/* Find desired exception with their names;
   Returns true once found;
           false once NOT found or null was given as EXCEP_NAME;
   Throws BufferOverflowException */
bool
exception_findexcep_byname(char *excep_name);

/* Find desired exception with their IDs;
   Returns true once found;
           false once NOT found or null was given as EXCEP_NAME;
   Throws BufferOverflowException */
bool
exception_findexcep_byid(excep_e id);

#ifdef __cplusplus
__END_DECLS
#endif /* __cplusplus */

/* Make _EXCEP_FMT public */
# define _EXCEP_FMT EXCEP_FMT
/* Make _DEF_EXCEP_FMT public */
# define _DEF_EXCEP_FMT DEFAULT_EXCEP_FMT

#endif /* NO _EXCEPTION_H */
