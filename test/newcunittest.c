/*
 * File:   newcunittest.c
 * Author: Praxis
 *
 * Created on 13.12.2021, 12:06:01
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../stack.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

myStack_t* StackNew(size_t i_elementSize, size_t i_maxAmount);

void testStack() {
    size_t i_elementSize=sizeof(position);
    size_t i_maxAmount=10;
    myStack_t* pMyStack = StackNew(i_elementSize, i_maxAmount);
    position first; first.line=10; first.row=3;
    position second; second.line=88; second.row=-1024;
    position third;
    third.line=5;
    third.row=2;
    Push(pMyStack,&first);
    Push(pMyStack,&second);
    Push(pMyStack,&third);
    position top;
    Pop (pMyStack, &top); 
    if ((top.line !=5)|| (top.row != 2)) 
    {
        CU_ASSERT(0);
    }
    if (IsStackEmpty(pMyStack)) 
    {
        CU_ASSERT(0);
    }
    position next;
    Pop (pMyStack,&next);
    if ((88 != next.line) || (next.row != -1024))
    {
        CU_ASSERT(0);
    }
    position last;
    Pop(pMyStack,&last);
    if ((10 !=last.line) || (last.row != 3))
    {
        CU_ASSERT(0);
    }   
    if (! IsStackEmpty(pMyStack))
    {
        CU_ASSERT(0);
    }
    int result=StackDestroy(pMyStack);
    if (EXIT_FAILURE == result)
    {
        CU_ASSERT(0);
    }
 /*   if ()
    {
        CU_ASSERT(0);
    }
    if ()
    {
        CU_ASSERT(0);
    }
    if ()
    {
        CU_ASSERT(0);
    }
  * */
}
/*
int StackDestroy(myStack_t* io_pStack);

void testStackDestroy() {
    myStack_t* io_pStack;
    int result = StackDestroy(io_pStack);
    if (1 ) {
        CU_ASSERT(0);
    }
}

int Pop(myStack_t* io_pStack, stackElement_t* io_pValue);

void testPop() {
    myStack_t* io_pStack;
    stackElement_t* io_pValue;
    int result = Pop(io_pStack, io_pValue);
    if (1 ) {
        CU_ASSERT(0);
    }
}
*/

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("newcunittest", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testStack", testStack)) /*||
            (NULL == CU_add_test(pSuite, "testStackDestroy", testStackDestroy)) ||
            (NULL == CU_add_test(pSuite, "testPop", testPop)) */ ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
