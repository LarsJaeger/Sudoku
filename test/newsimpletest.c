/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   newsimpletest.c
 * Author: Praxis
 *
 * Created on 13. Dezember 2021, 13:00
 */

#include <stdio.h>
#include <stdlib.h>
#include <stack.h>

/*
 * Simple C Test Suite
 */


void test2() {
    printf("newsimpletest teststack 2\n");
    
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
       printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=First Pop failed\n");
    }
    if (IsStackEmpty(pMyStack)) 
    {
        printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=StackEmpty failed\n");
    }
    position next;
    Pop (pMyStack,&next);
    if ((88 != next.line) || (next.row != -1024))
    {
        printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=Second Pop failed\n");
    }
    position last;
    Pop(pMyStack,&last);
    if ((10 !=last.line) || (last.row != 3))
    {
       printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=Third Pop failed\n");
    }   
    if (! IsStackEmpty(pMyStack))
    {
       printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=StackIsEmpty failed\n");
    }
    int result=StackDestroy(pMyStack);
    if (EXIT_FAILURE == result)
    {
      printf("%%TEST_FAILED%% time=0 testname=test2 (newsimpletest) message=StackDestroy failed\n");
    }
  
}

int main(int argc, char** argv) {
    printf("%%SUITE_STARTING%% newsimpletest\n");
    printf("%%SUITE_STARTED%%\n");

 

    printf("%%TEST_STARTED%% test2 (newsimpletest)\n");
    test2();
    printf("%%TEST_FINISHED%% time=0 test2 (newsimpletest) \n");

    printf("%%SUITE_FINISHED%% time=0\n");

    return (EXIT_SUCCESS);
}
