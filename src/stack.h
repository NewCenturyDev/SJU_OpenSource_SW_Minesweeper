#ifndef __STACK_H__
#define __STACK_H__

#include "game.h"

typedef struct StackNode { //스택 구조를 사용하기 위한 구조체
	Position elem;
	struct StackNode *next;
}StackNode;

StackNode *Push(StackNode *stack, Position pos);
Position Pop(StackNode **stack);
int IsEmptyStack(StackNode *stack);

#endif
