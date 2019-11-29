#include <stdlib.h>
#include "stack.h"

StackNode *Push(StackNode *stack, Position pos) {
	//스택에 푸시하는 함수
	StackNode *newNode; //새로 생성한 노드
	StackNode *pushNode; //푸시할 위치의 전 노드

	newNode = (StackNode *)malloc(sizeof(StackNode));
	//새로 생성한 노드에 푸시하고자하는 위치값을 저장
	(newNode->elem).x = pos.x;
	(newNode->elem).y = pos.y;
	//새로 생성한 노드의 다음은 널 포인터를 가리켜 마지막을 알 수 있게함
	newNode->next = NULL;

	if (stack == NULL)
		//스택이 비어있다면 새로 생성한 노드를 리턴
		return newNode;
	pushNode = stack;

	//스택이 비어있지않다면 현재의 마지막 노드로 이동하여 새로 생성한 노드를 마지막에 오도록 추가
	while (pushNode->next != NULL)
		pushNode = pushNode->next;
	pushNode->next = newNode;

	return stack;
}

Position Pop(StackNode **stack) {
	//스택의 노드를 팝하는 함수
	Position poppedElem; //팝할 노드의 원소를 담을 변수
	StackNode *popNode; //메모리 해제를 하기 위해 팝할 노드를 가리킬 변수

						//팝할 노드의 원소와 노드를 저장
	poppedElem = (*stack)->elem;
	popNode = *stack;

	*stack = (*stack)->next;
	free(popNode);
	return poppedElem;
}

int IsEmptyStack(StackNode *stack) {
	//스택이 비어있는지 확인하는 함수
	if (stack == NULL)
		//스택이 비어있다면 TRUE를 리턴
		return true;
	//스택이 비어있지않다면 FALSE를 리턴
	return false;
}
