#pragma once
#include <cstring>
#include "..\mp2-lab3-stack\TStack.h"

#define MAX_SIZE 10000
struct TNode
{
	char str[MAX_SIZE];
	TNode* pNext, * pDown;
	TNode(const char str[] = "", TNode* pNext = nullptr, TNode* pDown = nullptr) : pNext(pNext), pDown(pDown) {
		if (str == nullptr)
			*this->str = '\0';
		else
			strcpy(this->str, str);
	}
};

class TText
{
	TNode* pFirst;
	TNode* pCurr;
	TStack<TNode*> st;
public:
	TText(){}
};