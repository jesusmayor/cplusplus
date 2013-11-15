/*
 * TransitionTable.cpp
 *
 *  Created on: Nov 15, 2013
 *      Author: kasios
 */

#include "TransitionTable.h"

TransitionTable::TransitionTable() {
	// TODO Auto-generated constructor stub

}

TransitionTable::~TransitionTable() {
	// TODO Auto-generated destructor stub
}

static const Table_Entry    my_table[] =
{
    //  Current   Transition     Next
    //  State ID    Letter     State ID
    {    0,          'A',        1},
    {    1,          'A',        0},
};

static const unsigned int  TABLE_SIZE =
    sizeof(my_table) / sizeof(my_table[0]);


Table_Entry const *
table_begin(void)
{
    return &my_table[0];
}


Table_Entry const *
table_end(void)
{
    return &my_table[TABLE_SIZE];
}
