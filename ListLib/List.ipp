/*------------------------------------------------------------------------------
    * File:        List.ipp                                                    *
    * Description: Functions for lists.                                        *
    * Created:     21 aug 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>::ListNode () { }

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>::List () : errCode_ (LIST_NOT_CONSTRUCTED) { }

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>::List (char* list_name) :
    name_         (list_name),
    id_           (list_id++),
    errCode_      (LIST_OK)
{}

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>::List (char* list_name, char* base_filename) :
    name_         (list_name),
    id_           (list_id++),
    errCode_      (LIST_OK)
{
    LIST_ASSERTOK((list_name == nullptr), LIST_WRONG_INPUT_LIST_NAME);

    Text base(base_filename);

    ListNode<TYPE>* node = nullptr;
    ListNode<TYPE>* prev = nullptr;

    for (int i = 0; i < base.num_; ++i)
    {
        if (base.lines_[i].len == 0) continue;

        size_cur_++;

        node = new ListNode<TYPE>;
        sscanf(base.lines_[i].str, PRINT_FORMAT<TYPE>, &node->data_);
        if constexpr (std::is_same<TYPE, char*>::value) node->is_string_ = true;

        if (head_ == nullptr)
            head_ = node;

        if (prev != nullptr)
            prev->next_ = node;
        
        node->prev_ = prev;
        prev = node;
    }
    
    tail_ = node;

    LIST_CHECK;
}

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>::List (const List& obj)
{
    *this = obj;
}

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>& List<TYPE>::operator = (const List& obj)
{
    name_ = obj.name_;
    size_cur_ = obj.size_cur_;

    Clean();

    ListNode<TYPE>* node_cur = obj.head_;
    ListNode<TYPE>* node = nullptr;
    ListNode<TYPE>* prev = nullptr;

    while (node_cur != nullptr)
    {
        node = new ListNode<TYPE>;

        node->data_ = node_cur->data_;
        node->is_string_ = node_cur->is_string_;

        if (head_ == nullptr)
            head_ = node;

        if (prev != nullptr)
            prev->next_ = node;

        node->prev_ = prev;
        prev = node;
        node_cur = node_cur->next_;
    }

    tail_ = node;

    return *this;
}

//------------------------------------------------------------------------------

template <typename TYPE>
List<TYPE>::~List ()
{
    if (errCode_ == LIST_NOT_CONSTRUCTED) {}

    else if (errCode_ != LIST_DESTRUCTED)
    {
        ListNode<TYPE>* node_cur = head_;
        while (node_cur != nullptr)
        {
            ListNode<TYPE>* node = node_cur;
            node_cur = node_cur->next_;
            delete node;
        }

        head_ = nullptr;
        tail_ = nullptr;

        size_cur_ = 0;

        errCode_ = LIST_DESTRUCTED;
    }
    else
    {
        LIST_ASSERTOK(LIST_DESTRUCTOR_REPEATED, LIST_DESTRUCTOR_REPEATED);
    }
}

//------------------------------------------------------------------------------

template <typename TYPE>
void List<TYPE>::Clean ()
{
    LIST_CHECK;

    ListNode<TYPE>* node_cur = head_;
    while (node_cur != nullptr)
    {
        ListNode<TYPE>* node = node_cur;
        node_cur = node_cur->next_;

        delete node;
    }

    size_cur_ = 0;

    head_ = nullptr;
    tail_ = nullptr;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>::ListNode (const ListNode& obj)
{
    *this = obj;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>& ListNode<TYPE>::operator = (const ListNode& obj)
{
    if constexpr (std::is_same<TYPE, char*>::value)
    {
        if (is_string_)
            delete [] data_;

        if (obj.is_string_)
            data_ = new char[strlen(obj.data_) + 2] {};

        strcpy(data_, obj.data_);
    }
    else data_ = obj.data_;

    is_string_ = obj.is_string_;

    return *this;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>::~ListNode ()
{
    prev_ = nullptr;
    next_ = nullptr;

    if constexpr (std::is_same<TYPE, char*>::value)
        if (is_string_)
            delete [] data_;

    is_string_ = false;

    data_ = POISON<TYPE>;
}

//------------------------------------------------------------------------------

template <typename TYPE>
void List<TYPE>::Dump (const char* dumpname)
{
    assert(dumpname != nullptr);

    FILE* dump = fopen(dumpname, "w");
    assert(dump != nullptr);

    fprintf(dump, "digraph G{\n" "rankdir = HR;\n node[shape=box];\n");

    ListNode<TYPE>* node_cur = head_;
    while (node_cur != nullptr)
    {
        fprintf(dump, "\t \"this: " PRINT_PTR "\\n data: [", node_cur);
        TypePrint(dump, node_cur->data_);
        fprintf(dump, "]\\n prev: " PRINT_PTR " | next: " PRINT_PTR "\\n", node_cur->prev_, node_cur->next_);
        fprintf(dump, "\" [shape = box, style = filled, color = black, fillcolor = lightskyblue]\n");

        if (node_cur->prev_ != nullptr)
        {
            fprintf(dump, "\t \"this: " PRINT_PTR "\\n data: [", node_cur);
            TypePrint(dump, node_cur->data_);
            fprintf(dump, "]\\n prev: " PRINT_PTR " | next: " PRINT_PTR "\\n", node_cur->prev_, node_cur->next_);

            fprintf(dump, "\" -> \"");

            fprintf(dump, "this: " PRINT_PTR "\\n data: [", node_cur->prev_);
            TypePrint(dump, node_cur->prev_->data_);
            fprintf(dump, "]\\n prev: " PRINT_PTR " | next: " PRINT_PTR "\\n", node_cur->prev_->prev_, node_cur->prev_->next_);
            fprintf(dump, "\" [label=\"prev\"]\n");
        }

        if (node_cur->next_ != nullptr)
        {
            fprintf(dump, "\t \"this: " PRINT_PTR "\\n data: [", node_cur);
            TypePrint(dump, node_cur->data_);
            fprintf(dump, "]\\n prev: " PRINT_PTR " | next: " PRINT_PTR "\\n", node_cur->prev_, node_cur->next_);

            fprintf(dump, "\" -> \"");

            fprintf(dump, "this: " PRINT_PTR "\\n data: [", node_cur->next_);
            TypePrint(dump, node_cur->next_->data_);
            fprintf(dump, "]\\n prev: " PRINT_PTR " | next: " PRINT_PTR "\\n", node_cur->next_->prev_, node_cur->next_->next_);
            fprintf(dump, "\" [label=\"next\"]\n");
        }

        node_cur = node_cur->next_;
    }

    fprintf(dump, "\tlabelloc=\"t\";"
                  "\tlabel=\"list name: %s\\nType is %s\\nSize is %lu\";"
                  "}\n", name_, PRINT_TYPE<TYPE>, size_cur_);

    fclose(dump);

    char command[128] = "";

#if defined(WIN32)

    sprintf(command, "win_iconv -f 1251 -t UTF8 \"%s\" > \"new%s\"", dumpname, dumpname);

#elif defined(__linux__)

    sprintf(command, "iconv -f CP1251 -t UTF8 \"%s\" -o \"new%s\"", dumpname, dumpname);

#else
#error Program is only supported by linux or windows platforms
#endif

    int err = system(command);

    sprintf(command, "dot -Tpng -o %s new%s", DUMP_PICT_NAME, dumpname);
    if (!err) err = system(command);

#if defined(WIN32)

    sprintf(command, "del new%s", dumpname);

#elif defined(__linux__)

    sprintf(command, "rm new%s", dumpname);

#else
#error Program is only supported by linux or windows platforms
#endif

    if (!err) err = system(command);
}

//------------------------------------------------------------------------------

template <typename TYPE>
void List<TYPE>::Write (const char* basename)
{
    LIST_CHECK;

    FILE* base = fopen(basename, "w");
    assert(base != nullptr);

    ListNode<TYPE>* node_cur = head_;
    while (node_cur != nullptr)
    {
        TypePrint(base, node_cur->data_);
        fprintf(base, "\n");
        node_cur = node_cur->next_;
    }

    fclose(base);
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::push_front (TYPE data)
{
    LIST_CHECK;

    ListNode<TYPE>* node = new ListNode<TYPE>;

    node->data_ = data;
    size_cur_++;

    node->next_ = head_;
    if (head_ == nullptr)
        tail_ = node;
    else
        head_->prev_ = node;

    head_ = node;

    return head_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::emplace_front (TYPE data)
{
    LIST_CHECK;

    if (head_ != nullptr) head_->data_ = data;

    return head_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
TYPE List<TYPE>::pop_front ()
{
    LIST_CHECK;

    TYPE data = POISON<TYPE>;
    if (head_ != nullptr)
    {
        data = head_->data_;
        size_cur_--;

        ListNode<TYPE>* node = head_;

        head_ = node->next_;
        if (head_ != nullptr) head_->prev_ = nullptr;
        delete node;
    }

    return data;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::push_back (TYPE data)
{
    LIST_CHECK;

    ListNode<TYPE>* node = new ListNode<TYPE>;

    node->data_ = data;
    size_cur_++;

    node->prev_ = tail_;
    if (tail_ == nullptr)
        head_ = node;
    else
        tail_->next_ = node;

    tail_ = node;

    return tail_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::emplace_back (TYPE data)
{
    LIST_CHECK;

    if (tail_ != nullptr) tail_->data_ = data;

    return tail_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
TYPE List<TYPE>::pop_back ()
{
    LIST_CHECK;

    TYPE data = POISON<TYPE>;
    if (tail_ != nullptr)
    {
        data = tail_->data_;
        size_cur_--;

        ListNode<TYPE>* node = tail_;

        tail_ = node->prev_;
        if (tail_ != nullptr) tail_->next_ = nullptr;
        delete node;
    }

    return data;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::FindElem (size_t index)
{
    LIST_CHECK;

    ListNode<TYPE>* node_cur = head_;

    for (size_t i = 0; i < index; ++i)
    {
        if (node_cur == nullptr) return nullptr;
        node_cur = node_cur->next_;
    }

    return node_cur;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::FindVal (TYPE data, int (*CompareFunc)(const void*, const void*))
{
    LIST_CHECK;

    ListNode<TYPE>* node_cur = head_;

    while (node_cur != nullptr)
    {
        if (CompareFunc(&data, &node_cur->data_) == 0) break;
        node_cur = node_cur->next_;
    }

    return node_cur;
}

//------------------------------------------------------------------------------

template <typename TYPE>
bool List<TYPE>::DelElem (size_t index)
{
    LIST_CHECK;

    ListNode<TYPE>* node = FindElem(index);
    if (node == nullptr) return false;

    if (node->prev_ != nullptr)
    {
        if (node->next_ == nullptr)
            tail_ = node->prev_;
        else
            node->next_->prev_ = node->prev_;

        node->prev_->next_ = node->next_;
    }
    else
    {
        if (node->next_ == nullptr)
        {
            head_ = nullptr;
            tail_ = nullptr;
        }
        else
            node->next_->prev_ = nullptr;

        head_ = node->next_;
    }

    size_cur_--;

    delete node;

    return true;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::insert_before (TYPE data, size_t index)
{
    LIST_CHECK;

    ListNode<TYPE>* found_node = FindElem(index);
    if (found_node == nullptr) return nullptr;

    if (found_node->prev_ == nullptr) return push_front(data);

    ListNode<TYPE>* new_node = new ListNode<TYPE>;
    new_node->data_ = data;
    new_node->next_ = found_node;
    new_node->prev_ = found_node->prev_;
    found_node->prev_->next_ = new_node;
    found_node->prev_ = new_node;

    size_cur_++;

    return new_node;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::insert_after (TYPE data, size_t index)
{
    LIST_CHECK;

    ListNode<TYPE>* found_node = FindElem(index);
    if (found_node == nullptr) return nullptr;

    if (found_node->next_ == nullptr) return push_back(data);

    ListNode<TYPE>* new_node = new ListNode<TYPE>;
    new_node->data_ = data;
    new_node->prev_ = found_node;
    new_node->next_ = found_node->next_;
    found_node->next_->prev_ = new_node;
    found_node->next_ = new_node;

    size_cur_++;

    return new_node;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::emplace (TYPE data, size_t index)
{
    LIST_CHECK;

    ListNode<TYPE>* found_node = FindElem(index);
    if (found_node == nullptr) return nullptr;

    found_node->data_ = data;

    return found_node;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::head_element ()
{
    return head_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
ListNode<TYPE>* List<TYPE>::tail_element ()
{
    return tail_;
}

//------------------------------------------------------------------------------

template <typename TYPE>
void ListNode<TYPE>::setData (TYPE data)
{
    if constexpr (std::is_same<TYPE, char*>::value) if (is_string_) delete [] data_;
    is_string_ = false;

    data_ = data;
}

//------------------------------------------------------------------------------

template <typename TYPE>
const TYPE& ListNode<TYPE>::getData ()
{
    return data_;
}

//------------------------------------------------------------------------------

template<typename TYPE>
bool isPOISON (List<TYPE> list)
{
    return ( (list.name_    == nullptr) &&
             (list.head_    == nullptr) &&
             (list.tail_    == nullptr) &&
             (list.id_      == 0)       &&
             (list.errCode_ == 0) );
}

//------------------------------------------------------------------------------

template<typename TYPE>
void TypePrint (FILE* fp, const List<TYPE>& list)
{
    fprintf(fp, "%s", list.name_);
}

//------------------------------------------------------------------------------

template <typename TYPE>
int List<TYPE>::Check ()
{
    int err = LIST_OK;
    size_t size = 0;

    ListNode<TYPE>* node_cur = head_;
    while (node_cur != nullptr)
    {
        if (node_cur->prev_ != nullptr) if (node_cur->prev_->next_ != node_cur) err = LIST_WRONG_NEXT_NODE;
        if (node_cur->next_ != nullptr) if (node_cur->next_->prev_ != node_cur) err = LIST_WRONG_PREV_NODE;

        node_cur = node_cur->next_;

        size++;
    }
    if (size != size_cur_) err = LIST_WRONG_SIZE;

    errCode_ = err;

    return err;
}

//------------------------------------------------------------------------------

template <typename TYPE>
void List<TYPE>::PrintError (const char* logname, const char* file, int line, const char* function, int err)
{
    assert(function != nullptr);
    assert(logname  != nullptr);
    assert(file     != nullptr);

    FILE* log = fopen(logname, "a");
    assert(log != nullptr);

    fprintf(log, "********************************************************************************\n");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(log, "TIME: %d-%02d-%02d %02d:%02d:%02d\n\n",
            tm.tm_year + 1900,
            tm.tm_mon + 1,
            tm.tm_mday,
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec);

    fprintf(log, "ERROR: file %s  line %d  function %s\n\n", file, line, function);
    fprintf(log, "%s\n", list_errstr[err + 1]);
    fprintf(log, "You can look list dump in %s\n\n", DUMP_PICT_NAME);
    fclose(log);

    ////

    printf("ERROR: file %s  line %d  function %s\n", file, line, function);
    printf("%s\n\n", list_errstr[err + 1]);
    printf (     "You can look list dump in %s\n\n", DUMP_PICT_NAME);
}

//------------------------------------------------------------------------------
