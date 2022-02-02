/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stack.h"

myStack_t *StackNew (size_t i_elementSize, size_t i_maxAmount)
{
    myStack_t *pStack = malloc (sizeof (myStack_t));
    if (NULL == pStack) return NULL;
    pStack->top= malloc (i_elementSize *i_maxAmount);
    if (NULL == pStack->top)
    {
        free (pStack);
        return NULL;
    }
    pStack->bottom = pStack->top + (i_elementSize * i_maxAmount);
    pStack->pStack = pStack->bottom;
    pStack->elementSize = i_elementSize;
    pStack->maxAmount = i_maxAmount;
    return pStack;
}

int IsStackEmpty(myStack_t *io_pStack)
{
    if (io_pStack->bottom == io_pStack->pStack)
        return 1;
    else return 0;
}

int StackDestroy (myStack_t *io_pStack)
{
    if (NULL == io_pStack) return EXIT_FAILURE;
    free (io_pStack->top);
    io_pStack->bottom = NULL;
    io_pStack -> top=NULL;
    free (io_pStack);
    return EXIT_SUCCESS;
}

int Pop (myStack_t *io_pStack, stackElement_t *io_pValue)
{
    if ((NULL != io_pStack) && (io_pStack->pStack != io_pStack->bottom))
    {
        memcpy (io_pValue, io_pStack->pStack, io_pStack->elementSize);
        io_pStack->pStack += io_pStack->elementSize;
        return EXIT_SUCCESS;
    }
    else return EXIT_FAILURE;
}

int Push (myStack_t *io_pStack, stackElement_t *io_pValue)
{

    if ((NULL != io_pStack) && 
         (io_pStack->pStack >= 
         ((io_pStack->bottom)-(io_pStack->elementSize * io_pStack->maxAmount))))
    {
        io_pStack->pStack -= io_pStack->elementSize;
        memcpy (io_pStack->pStack, io_pValue, io_pStack->elementSize);
        
        return EXIT_SUCCESS;
    }
    else return EXIT_FAILURE;
}