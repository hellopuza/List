/*------------------------------------------------------------------------------
    * File:        ListConfig.h                                                *
    * Description: List congigurations which define different list data types  *
                   and errors                                                  *
    * Created:     21 aug 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#ifndef LIST_CONFIG_H_INCLUDED
#define LIST_CONFIG_H_INCLUDED


#include "../Types.h"
#include <stdlib.h>
#include <time.h>


#if defined (__GNUC__) || defined (__clang__) || defined (__clang_major__)
    #define __FUNC_NAME__   __PRETTY_FUNCTION__
    #define PRINT_PTR       "%p"

#elif defined (_MSC_VER)
    #define __FUNC_NAME__   __FUNCSIG__
    #define PRINT_PTR       "0x%p"

#else
    #define __FUNC_NAME__   __FUNCTION__
    #define PRINT_PTR       "%p"

#endif


char const * const DUMP_NAME         = "graph.dot";
char const * const DUMP_PICT_NAME    = "graph.png";
char const * const DEFAULT_BASE_NAME = "Base.dat";
char const * const LIST_LOGNAME      = "list.log";


enum ListErrors
{
    LIST_NOT_OK = -1                                                ,
    LIST_OK = 0                                                     ,
    LIST_NO_MEMORY                                                  ,

    LIST_DESTRUCTED                                                 ,
    LIST_DESTRUCTOR_REPEATED                                        ,
    LIST_EMPTY_LIST                                                 ,
    LIST_INPUT_DATA_POISON                                          ,
    LIST_MEM_ACCESS_VIOLATION                                       ,
    LIST_NOT_CONSTRUCTED                                            ,
    LIST_NULL_INPUT_LIST_PTR                                        ,
    LIST_NULL_LIST_PTR                                              ,
    LIST_WRONG_INPUT_LIST_NAME                                      ,
    LIST_WRONG_NEXT_NODE                                            ,
    LIST_WRONG_PREV_NODE                                            ,
    LIST_WRONG_SIZE                                                 ,
};

char const * const list_errstr[] =
{
    "ERROR"                                                         ,
    "OK"                                                            ,
    "Failed to allocate memory"                                     ,

    "List already destructed"                                       ,
    "List destructor repeated"                                      ,
    "List is empty"                                                 ,
    "Input data is poison"                                          ,
    "Memory access violation"                                       ,
    "List did not constructed, operation is impossible"             ,
    "The input value of the tree pointer turned out to be zero"     ,
    "The pointer to the list is null, list lost"                    ,
    "Wrong input list name"                                         ,
    "Wrong pointer to next node found"                              ,
    "Wrong pointer to previous node found"                          ,
    "Wrong list size"                                               ,
};


#endif // LIST_CONFIG_H_INCLUDED
