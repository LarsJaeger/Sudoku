/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stack.h
 * Author: Praxis
 *
 * Created on 13. Dezember 2021, 12:25
 */

#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef void stackElement_t;
typedef struct 
{
    stackElement_t *bottom; 
    stackElement_t *top; 
    stackElement_t *pStack;
    size_t elementSize;
    size_t maxAmount;
} myStack_t;

typedef struct 
{
    int line;
    int row;
} position;

myStack_t *StackNew (size_t i_elementSize, size_t i_maxAmount);
int StackDestroy (myStack_t *io_pStack);
int Pop (myStack_t *io_pStack, stackElement_t *io_pValue);
int Push (myStack_t *io_pStack, stackElement_t *io_pValue);
int IsStackEmpty(myStack_t *io_pStack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */

