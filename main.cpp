/*------------------------------------------------------------------------------
    * File:        main.cpp                                                    *
    * Description: Program uses list.                                          *
    * Created:     21 aug 2021                                                 *
    * Author:      Artem Puzankov                                              *
    * Email:       puzankov.ao@phystech.edu                                    *
    * GitHub:      https://github.com/hellopuza                                *
    * Copyright © 2021 Artem Puzankov. All rights reserved.                    *
    *///------------------------------------------------------------------------

#include "ListLib/List.h"

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    newList(list, int);

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    list.push_front(2);
    list.pop_back();
    list.insert_after(1, 1);
    list.Dump();
    list.Write();

    return 0;
}