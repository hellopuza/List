/*------------------------------------------------------------------------------
    * File:        List.h                                                      *
    * Description: Declaration of functions and data types used for lists.     *
    * Created:     21 aug 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#include "../StringLib/StringLib.h"
#include "ListConfig.h"
#include <type_traits>
#include <assert.h>
#include <limits.h>
#include <new>


#define LIST_CHECK if (Check ())                        \
                   {                                    \
                     Dump(DUMP_NAME);                   \
                     LIST_ASSERTOK(errCode_, errCode_); \
                   } //


#define LIST_ASSERTOK(cond, err) if (cond)                                                                  \
                                       {                                                                    \
                                         PrintError(LIST_LOGNAME , __FILE__, __LINE__, __FUNC_NAME__, err); \
                                         exit(err);                                                         \
                                       } //

static int list_id = 0;

#define newList(NAME, LIST_TYPE) \
        List<LIST_TYPE> NAME ((char*)#NAME);

#define newList_base(NAME, base, LIST_TYPE) \
        List<LIST_TYPE> NAME ((char*)#NAME, base);


template <typename TYPE>
class List;

template<typename TYPE> const char* const PRINT_TYPE<List<TYPE>> = "List";
template<typename TYPE> const List<TYPE>  POISON    <List<TYPE>> = {};

template<typename TYPE> bool isPOISON  (List<TYPE> list);
template<typename TYPE> void TypePrint (FILE* fp, const List<TYPE>& list);


template <typename TYPE>
class ListNode
{
    friend class List<TYPE>;

    TYPE data_      = POISON<TYPE>;
    bool is_string_ = false;

public:

    ListNode* next_ = nullptr;
    ListNode* prev_ = nullptr;

//------------------------------------------------------------------------------
/*! @brief   Node default constructor.
*/

    ListNode ();

//------------------------------------------------------------------------------
/*! @brief   Node destruction.
 *
 *  @note    All nodes must be created by operator new!!!
 */

    ~ListNode ();

//------------------------------------------------------------------------------
/*! @brief   Safe change node data.
 *
 *  @param   data        Data to change
 */

    void setData (TYPE data);

//------------------------------------------------------------------------------
/*! @brief   Get node data.
 *
 *  @return  node data
 */

    const TYPE& getData ();

//------------------------------------------------------------------------------
/*! @brief   Node copy constructor.
 *
 *  @param   obj         Source node
 */

    ListNode (const ListNode& obj);

    ListNode& operator = (const ListNode& obj);

//------------------------------------------------------------------------------
};


template <typename TYPE>
class List
{
    char* name_ = nullptr;

    size_t size_cur_ = 0;

    ListNode<TYPE>* head_ = nullptr;
    ListNode<TYPE>* tail_ = nullptr;

    int id_ = 0;
    int errCode_ = 0;

public:

//------------------------------------------------------------------------------
/*! @brief   List default constructor.
*/

    List ();

//------------------------------------------------------------------------------
/*! @brief   List constructor with name.
 *
 *  @param   list_name   List variable name
 */

    List (char* list_name);

//------------------------------------------------------------------------------
/*! @brief   List constructor with base.
 *
 *  @param   list_name   List variable name
 *  @param   base_name   Base filename
 */

    List (char* list_name, char* base_name);

//------------------------------------------------------------------------------
/*! @brief   List destructor.
 */

    ~List ();

//------------------------------------------------------------------------------
/*! @brief   List copy constructor.
 *
 *  @param   obj         Source list
 */

    List (const List& obj);

    List& operator = (const List& obj);

//------------------------------------------------------------------------------
/*! @brief   Clean list.
 */

    void Clean ();

//------------------------------------------------------------------------------
/*! @brief   Print the contents of the list like a graphviz dot file.
 *
 *  @param   dumpname    Name of the dump file
 */

    void Dump (const char* dumpname = DUMP_NAME);

//------------------------------------------------------------------------------
/*! @brief   Write the list data to the base file.
 *
 *  @param   basename    Base file name
 */

    void Write (const char* basename = DEFAULT_BASE_NAME);

//------------------------------------------------------------------------------
/*! @brief   Push element to the front.
 *
 *  @param   data        Value of the element
 * 
 *  @return  pointer to head node
 */

    ListNode<TYPE>* push_front (TYPE data);

//------------------------------------------------------------------------------
/*! @brief   Emplace element at the front.
 *
 *  @param   data        Value of the element
 *
 *  @return  pointer to head node
 */

    ListNode<TYPE>* emplace_front (TYPE data);

//------------------------------------------------------------------------------
/*! @brief   Pop element from the front.
 *
 *  @return  Value of the element
 */

    TYPE pop_front ();

//------------------------------------------------------------------------------
/*! @brief   Push element to the back.
 *
 *  @param   data        Value of the element
 *
 *  @return  pointer to tail node
 */

    ListNode<TYPE>* push_back (TYPE data);

//------------------------------------------------------------------------------
/*! @brief   Emplace element at the back.
 *
 *  @param   data        Value of the element
 *
 *  @return  pointer to tail node
 */

    ListNode<TYPE>* emplace_back (TYPE data);

//------------------------------------------------------------------------------
/*! @brief   Pop element from the back.
 *
 *  @return  Value of the element
 */

    TYPE pop_back ();

//------------------------------------------------------------------------------
/*! @brief   Find element by index.
 *
 *  @param   index       Index of the element
 *
 *  @return  pointer to node if found, else nullptr
 */

    ListNode<TYPE>* FindElem (size_t index);

//------------------------------------------------------------------------------
/*! @brief   Find element by value.
 *
 *  @param   data         Value of the element
 *  @param   CompareFunc  Comparison function
 *
 *  @return  pointer to node if found, else nullptr
 */

    ListNode<TYPE>* FindVal (TYPE data, int (*CompareFunc)(const void*, const void*));

//------------------------------------------------------------------------------
/*! @brief   Delete element.
 *
 *  @param   index       Index of the element
 *
 *  @return  true if element deleted, else false
 */

    bool DelElem (size_t index);

//------------------------------------------------------------------------------
/*! @brief   Insert element before chosen element.
 *
 *  @param   data        Value of the element
 *  @param   index       Index of chosen element
 *
 *  @return  pointer to node if inserted, else nullptr
 */

    ListNode<TYPE>* insert_before (TYPE data, size_t index);

//------------------------------------------------------------------------------
/*! @brief   Insert element after chosen element.
 *
 *  @param   data        Value of the element
 *  @param   index       Index of chosen element
 *
 *  @return  pointer to node if inserted, else nullptr
 */

    ListNode<TYPE>* insert_after (TYPE data, size_t index);

//------------------------------------------------------------------------------
/*! @brief   Emplace chosen by index element.
 *
 *  @param   data        Value of the element
 *  @param   index       Index of chosen element
 *
 *  @return  pointer to node if emplaced, else nullptr
 */

    ListNode<TYPE>* emplace (TYPE data, size_t index);

//------------------------------------------------------------------------------
/*! @brief   Get head element of the list.
 *
 *  @return  head element
 */

    ListNode<TYPE>* head_element ();

//------------------------------------------------------------------------------
/*! @brief   Get tail element of the list.
 *
 *  @return  tail element
 */

    ListNode<TYPE>* tail_element ();

private:

//------------------------------------------------------------------------------
/*! @brief   Check list for problems.
 *
 *  @return  error code
 */

    int Check ();

//------------------------------------------------------------------------------
/*! @brief   Print error explanations to log file and to console.
 *
 *  @param   logname     Name of the log file
 *  @param   file        Name of the file from which this function was called
 *  @param   line        Line of the code from which this function was called
 *  @param   function    Name of the function from which this function was called
 *  @param   err         Error code
 */

    void PrintError (const char* logname, const char* file, int line, const char* function, int err);

//------------------------------------------------------------------------------
};

#include "List.ipp"

#endif // LIST_H_INCLUDED