#pragma once
#include <fstream>
#include <cstring>
#include "..\mp2-lab3-stack\TStack.h"

#define MAX_SIZE 80
struct TNode
{
	char str[MAX_SIZE];
	TNode* pNext, * pDown;
	TNode(const char str[] = "", TNode* pNext = nullptr, TNode* pDown = nullptr) : pNext(pNext), pDown(pDown) {
		if (str == nullptr)
			*this->str = '\0';
		else
			strcpy_s(this->str, str);
	}
};

class TText
{
	TNode* pFirst;
	TNode* pCurr;
	TStack<TNode*> st;
public:
	TText(){}
	void GoNextLine() {
		if (pCurr != nullptr) {
			st.Push(pCurr);
			pCurr = pCurr->pNext;
		}
	}
	void GoDownLine() {
		if (pCurr != nullptr) {
			st.Push(pCurr);
			pCurr = pCurr->pDown;
		}
	}
	void GoFirst() {
		pCurr = pFirst;
		st.Clear();
	}
	void InsNextLine(char str[]) { //insert next element on the same level
		if (pCurr != nullptr) {
			TNode* newStr = new TNode(str, pCurr->pNext);
			pCurr->pNext = newStr;
		}
	}
	void InsDownLine(char str[]) { //insert next element on the same level
		if (pCurr != nullptr) {
			TNode* newStr = new TNode(str, pCurr->pDown);
			pCurr->pDown = newStr;
		}
	}
	void InsNextSection(char str[]) {
		if (pCurr != nullptr) {
			TNode* newStr = new TNode(str, nullptr, pCurr->pNext);
			pCurr->pNext = newStr;
		}
	}
	void InsDownSection(char str[]) {
		if (pCurr != nullptr) {
			TNode* newStr = new TNode(str, nullptr, pCurr->pDown);
			pCurr->pDown = newStr;
		}
	}
	void DelNextLine() {
		if (pCurr != nullptr) {
			if (pCurr->pNext != nullptr) {
				TNode* pDel = pCurr->pNext;
				pCurr->pNext = pDel->pNext;
				delete pDel;
			}
		}
	}
	void DelDownLine() {
		if (pCurr != nullptr) {
			if (pCurr->pDown != nullptr) {
				TNode* pDel = pCurr->pDown;
				pCurr->pDown = pDel->pNext;
				delete pDel;
			}
		}
	}
	TNode* ReadRec(std::ifstream& ifs) {
		TNode* pHead = NULL, * pTemp;
		char buf[MAX_SIZE];
		while (!ifs.eof()) {
			ifs.getline(buf, MAX_SIZE, '\n');
			if (buf[0] == '}')
				break;
			else if (buf[0] == '{')
				pTemp->pDown = ReadRec(ifs);
			else {
				TNode* p = new TNode(buf);
				if (pHead == nullptr)
					pHead = pTemp = p;
				else {
					pTemp->pNext = p;
					pTemp = p;
				}
			}
		}
		return pHead;
	}
	void ReadFromFile(char const* filename) {
		std::ifstream ifs(filename);
		if (ifs.is_open()) {
			pFirst = ReadRec(ifs);
		}
	}
	void PrintRec(std::ofstream& ofs, TNode* p)
	{
		if (p != nullptr) {
			ofs << p->str << std::endl;
			if (p->pDown != nullptr)
				ofs << '{' << std::endl;
			PrintRec(ofs, p->pDown);
			if (p->pDown != nullptr)
				ofs << '}' << std::endl;
			PrintRec(ofs, p->pNext);
		}
	}
	void Print(char const* filename) {
		std::ofstream ofs(filename);
		if (ofs.is_open()) {
			PrintRec(ofs, pFirst);
		}
	}
};