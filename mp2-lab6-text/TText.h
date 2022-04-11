#pragma once
#include <fstream>
#include <cstring>
#include "TStack.h"

#define MAX_SIZE 80
struct TMem 
{
	TNode* pFirst; // pointer at the begin
	TNode* pFree; // pointer at the first free node
	TNode* pLast; // pointer at the last node
};
struct TNode
{
	char str[MAX_SIZE];
	TNode* pNext, * pDown;
	static TMem mem;
	bool flag = false;
	TNode(const char str[] = "", TNode* pNext = nullptr, TNode* pDown = nullptr) : pNext(pNext), pDown(pDown) 
	{
		if (str == nullptr)
			*this->str = '\0';
		else
			strcpy_s(this->str, str);
	}
	void* operator new(size_t size) 
	{
		TNode* res = mem.pFree;
		if (mem.pFree)
			mem.pFree = mem.pFree->pNext;
		return res;
	}
	void operator delete(void* del) 
	{
		if (del) 
		{
			TNode* del_node = (TNode*)del;
			TNode* pLastFree = mem.pFree;
			mem.pFree = del_node;
			mem.pFree->pNext = pLastFree;
		}
	}
	static void InitMem(int s = 100)
	{
		mem.pFirst = (TNode*) new char[sizeof(TNode) * s];
		mem.pFree = mem.pFirst;
		mem.pLast = mem.pFirst + s - 1;
		TNode* p = mem.pFirst;
		for (int i = 0; i < s - 1; i++, p++)
		{
			p->pNext = p + 1;
			p->str[0] = '\0';
			p->flag = false;
		}
		p->pNext = nullptr;
	}
	static void CleanMem(TText& txt) {
		TNode* p = mem.pFree;
		while (p) {
			p->flag = true;
			p = p->pNext;
		}
		for (txt.Reset(); !txt.IsEnd(); txt.GoNext()) {
			// to do: write TText::SetFlag that sets flag & pCurr
			txt.SetFlag();
		}
		for (p = mem.pFirst; p <= mem.pLast; p++) {
			if (!p->flag) {
				delete p;
				p->flag = true;
			}
		}
	}
	static void PrintFree() {

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
	void Reset() {
		st.Clear();
		pCurr = pFirst;
		st.Push(pCurr);
		if (pCurr) {
			if (pCurr->pNext)
				st.Push(pCurr->pNext);
			if (pCurr->pDown)
				st.Push(pCurr->pDown);
		}
	}
	void GoNext() {
		pCurr = st.Pop();
		if(pCurr != pFirst){
			if (pCurr) {
				if (pCurr->pNext)
					st.Push(pCurr->pNext);
				if (pCurr->pDown)
					st.Push(pCurr->pDown);
			}
		}
	}
	bool IsEnd() { return st.IsEmpty(); }
	void SetFlag() {}
	TNode* CopyNode(TNode* p) {

	}
};