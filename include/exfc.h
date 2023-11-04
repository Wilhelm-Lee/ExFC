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
 * @file exfc.h
 * @brief ExFC header file. Contains declarations to exceptions and operational
 *        functionalities.
 * @version Alpha 0.0.0
 * @author William Lee
 */

#ifndef EXFC_H
# define EXFC_H

# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "dependency.h"

#ifndef CARRAY_H
# error Ensure Array control is completely modularied from current fixture.
#endif /* NO CARRAY_H */

# ifdef __cplusplus
__BEGIN_DECLS
# endif /* __cplusplus */

/* par1="Exception"=EXCEPTION;
   par2="File"=__FILE__;
   par3="Line"=__LINE__;
   par4="Function"=__FUNCTION__ */
# define EXCEPT_FMT "Threw the %s:\n\tat %s:%ld, func %s\n\"%s\"\n"
# define DEF_EXCEPT_FMT "Threw the %s\n"

# ifndef EXCEP_BUFF_MAX
#  define EXCEP_BUFF_MAX 4096
# endif /* NO EXCEP_BUFF_MAX */

# ifndef EXCEP_ARRAY_MAX
#  define EXCEP_ARRAY_MAX 511
# endif /* NO EXCEP_ARRAY_MAX */

# ifndef EXCEP_ID_OFFSET
#  define EXCEP_ID_OFFSET 1
# endif /* NO EXCEP_ID_OFFSET */

/**
 * @enum An enumeration indicates the results of
 *       exceptions' comparisons.
 */
typedef CommonComparison excep_comparison;

/**
 * @enum An enumeration indicates the result to common operations to
 *       exceptions.
 */
typedef CommonReturn excep_result;

/**
 * @enum An enumeration declares all predefined exceptions.
 */
enum Except_t {
  UnknownException = EXCEP_ID_OFFSET,
  InstanceFailureException,
  IllegalMemoryAccessException,
  InvalidArgumentException,
  OutOfBoundException,
  InvalidNullPointerException,
  OutOfMemoryException,
  BufferOverflowException  
};

/**
 * \struct Exception src/exfc.h exfc.h
 */
typedef struct
{
  const char *_name;
  const char *_description;
  Except_t _id;
} __attribute__((aligned(32))) Exception;

Exception E_OutOfMemoryException =
  {"OutOfMemoryException", "", OutOfMemoryException};

Exception _exceptions[] = {
	{"Exception", "", UnknownException},
	{"InstanceFailureException", "", InstanceFailureException},
	{"IllegalMemoryAccessException", "", IllegalMemoryAccessException},
	{"InvalidArgumentException", "", InvalidArgumentException},
	{"OutOfBoundException", "", OutOfBoundException},
	{"InvalidNullPointerException", "", InvalidNullPointerException},
	{"OutOfMemoryException", "", OutOfMemoryException},
	{"BufferOverflowException", "", BufferOverflowException}
};

static inline Exception*
_read_from_array_exceptions(int idx)
{
  if (idx <= EXCEP_ID_OFFSET)
    return NULL;
  
  return (&_exceptions[idx - EXCEP_ID_OFFSET]);
}

//static const EXCEPTION excep_null = (EXCEPTION){"", "", 0};
//static const EXCEPTION *excep_nullptr = &excep_null;

/* TODO: Replace this macro with using Class */
# define excep_null ((Exception){"", "", 0})
/* TODO: Disqualify this */
# define excep_nullptr (&excep_null)

static Exception _excep_arr[EXCEP_ARRAY_MAX] = {};
static const int _excep_arr_len = EXCEP_ARRAY_MAX;

/**
 * @brief Compare two exception by their ID;
 * @param a The first exception to be compared.
 * @param b The second exception to be compared.
 * @note Fails once any given parameter was null;
 * @return @b IDENTICAL once $A = $B;\n
 * @return @b GREATER   once $A > $B;\n
 * @return @b LESS      once $A < $B;
 */
int
exfc_cmp(Exception *a, Exception *b);

/**
 * @brief By specifying the name & the description & the ID, an exception can
 *        be added once no same exception exists in advance.
 * @param name Name to the exception being added.
 * @param description Description to the exception being added.
 * @param id ID to the exception being added.
 * @note Fails once any given parameter was null, except $id.
 * @return Index to the exception being added;\n
 * @return @b CONDITIONAL once $_excep_arr was full;\n
 * @return @b DUPLICATED  once $_excep_arr had a same element;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
exfc_addexcep(const char *name, const char *description, int id);

/* For test only. */
int
_exfc_addexcep_test(const void *name, const void *description, int id);

/**
 * @brief By specifying the name, an exception can be removed once it exists.
 * @param name The name used to search for desired exception to be removed.
 * @note Fails once any given parameter was null.
 * @return Index to the exception being removed;\n
 * @return @b MISSING     once $_excep_arr had no desired exception;\n
 * @return @b CONDITIONAL once $_excep_arr was empty;\n
 * @return @b FAILED      once $id < 0;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
exfc_removeexcep_byname(const char *name);

/**
 * @brief By specifying the id, an exception can be removed once it exists.
 * @param id The ID used to search for desired exception to be removed.
 * @return Index to the exception being removed;\n
 * @return @b MISSING     once $_excep_arr had no desired exception;\n
 * @return @b CONDITIONAL once $_excep_arr was empty;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
exfc_removeexcep_byid(int id);

/**
 * @brief Return all exceptions.
 * @return A pointer towards $_excep_arr;\n
 * @return $excep_nullptr once _excep_arr is NULL;
 * @exception InvalidNullPointerException
 */
Exception *
exfc_getallexcep();

/**
 * @brief Find desired exception with its name;
 * @param name The name used to search for desired exception.
 * @note Fails once any given parameter was null.
 * @return Index of the exception being found;\n
 * @return @b MISSING  once NOT found;\n
 * @return @b ABNORMAL once $_excep_arr was null;
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
exfc_getindex_byname(const char *name);

/**
 * @brief Find desired exception with its ID;
 * @param name The ID used to search for desired exception.
 * @note Fails once any given parameter was null.
 * @return Index of the exception being found;\n
 * @return @b MISSING  once NOT found;\n
 * @return @b ABNORMAL once $_excep_arr was null;
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
exfc_getindex_byid(int id);

/**
 * @brief Iterate through every element in $_excep_arr, until find specified
 *        exception.
 * @param e The exception used to search for desired exception.
 * @note Fails once any given parameter was null.
 * @return Index of matched exception;\n
 * @return @b CONDITIONAL once $e was excep_null, which is not allowed to
 *                        operate on it;\n
 * @return @b MISSING     once NOT found;\n
 * @return @b FAILED      once id < 0;
 *                        once operating target is $excep_null;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception InvalidNullPointerException
 */
int
exfc_getindex_byexcep(Exception e);

/**
 * @brief Iterate through every element in $_excep_arr, until find the last
 *        exception.
 * @return Index of matched exception;\n
 * @return @b CONDITIONAL once $_excep_arr was empty;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception InvalidNullPointerException
 */
int
_exfc_iteration_last();

/**
 * @brief Iterate through every element in $_excep_arr, until find the first
 *        exception.
 * @return Index of matched exception;\n
 * @return @b CONDITIONAL once $_excep_arr was empty;\n
 * @return @b ABNORMAL    once $_excep_arr was null;
 * @exception InvalidNullPointerException
 */
int
_exfc_iteration_first();

/**
 * @brief Rearrange whole array to make all the elements listed near-by.
 * @return Real length of _excep_arr after rearrangement;\n
 * @return @b ABNORMAL once $_excep_arr was null;
 * @exception InvalidNullPointerException
 */
int
_exfc_rearrangement();

/**
 * @brief Rearrange whole array to make all the elements listed near-by without
 *   an extra array used.
 * @return Real length of $_excep_arr after rearrangement;\n
 * @return @b ABNORMAL once $_excep_arr was null;
 * @exception InvalidNullPointerException
 */
int
_exfc_rearrangement_inplace();

/**
 * @brief Compare string $A and string $B in a quick way.\n
 * @param a The first string to be compared.
 * @param b The second string to be compared.
 * @param capital_restricted Specify whether to restrict on capitalisation.
 * That is, once single character does not match, it stops following operations.
 * @note Fails once any given parameter was null. except $capital_restricted.
 * @note Transacts FAILED    from _exfc_capital_check(char, char, bool);\n
 *                 ABNORMAL  from _exfc_capital_check(char, char, bool);
 * @return @b IDENTICAL once matched;\n
 * @return @b DIFFERENT once did not match.
 */
int
_exfc_quick_match_str(const char *a, const char *b, bool capital_restricted);

/**
 * @brief Check whether the characters are exactly the same by comparing their
 *        ASCII values.
 * @param a The first letter to be checked.
 * @param b The second letter to be checked.
 * @param capital_restricted Specify whether to restrict on capitalisation.
 * @return @b IDENTICAL once $A is exactly the same as $B;\n
 * @return @b DIFFERENT once $A is not exactly the same as $B;
 */
int
_exfc_capital_check(char a, char b, bool capital_restricted);

/**
 * @brief This function specifically throw BufferOverflowException once given
 *        BUFF is longer than $EXCEP_BUFF_MAX.
 * @param buff The buffer to be checked.
 * @note Fails once any given parameter was null.
 * @return @b FAILED   once failed passing through macro "fail";\n
 * @return @b ABNORMAL once $_excep_arr was null;\n
 * @return @b NORMAL   once normally proceeded with no error occurred;\n
 * @exception BufferOverflowException
 * @exception InvalidNullPointerException
 */
int
_exfc_buffersize_chk(char *buff);

/**
 * @brief Swap specified two element from $_excep_arr;
 * @param a The first partial exception to be swapped.
 * @param a The second partial exception to be swapped.
 * @note Fails once any given parameter was null;
 * @return @b FAILED once failed passing through macro "fail";\n
 * @return @b NORMAL normally proceeded with no error occurred;
 */
int
_exfc_swap(Exception *a, Exception *b);

/**
 * @brief The one of operations to the exceptions, THROW.
 * @param e The exception specified to be thrown.
 * @param _file_ The macro __file__ provided under promise on calling.
 * @param _line_ The macro __line__ provided under promise on calling.
 * @param _function_ The macro __function__ provided under promise on calling.
 * @param FMT The format to variadic list used on outputting.
 */
__attribute__((noreturn))
static inline void
THROW(Except_t *e, const char *__restrict__ _file_, long int _line_,
      const char *__restrict__ _function_, const char *__restrict__ FMT)
{
  if (FMT == NULL)
    FMT = DEF_EXCEPT_FMT

  Exception *E = _read_from_array_exceptions(e);
  
  (void)fprintf(stderr, ((_file_ == NULL && _line_ == -1 && _function_ == NULL)
                         ? DEF_EXCEPT_FMT
                         /* Ignore _FMT when outputting the exception title.
                             Use _EXCEPT_FMT instead. */
                         : EXCEPT_FMT), e->_name, _file_, _line_, _function_,
                                       e->_description);

  exit(e->_id);  // Try using memctl (credit: Wilhelm-Lee@github.com) to
                 // solve such issues by retracing back to caller.
                 // Direct usage of exit(int):void is NOT recommanded. It 
                 // damages thead-safe in long-term consideration.

}

# ifdef __cplusplus
__END_DECLS
# endif /* __cplusplus */

#endif /* NO EXFC_H */

