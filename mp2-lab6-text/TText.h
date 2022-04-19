#pragma once
#include "TStack.h"
#include <cstring>

#define MAX_STRING_SIZE 80
#define MAX_MEMORY_SIZE 80

struct TNode;
class TText;

struct TMem
{
	TNode* pFirst; // pointer at the begin
	TNode* pFree; // pointer at the first free node
	TNode* pLast; // pointer at the last node

	TMem();
};

class TText {
	TNode* pFirst = nullptr;
	mutable TNode* pCurr = nullptr;
	mutable TStack<TNode*> st;
public:
	TText();
	TText(TNode* _pFirst);
	~TText();
	bool operator==(const TText& other) const;
	
	void InsNextLine(const char* str);
	void InsDownLine(const char* str);
	void InsNextSection(const char* str);
	void InsDownSection(const char* str);

	void DelNextLine();
	void DelDownLine();

	void ReadFromFile(char const* filename);
	void PrintToFile(char const* filename) const;

	void Reset() const;
	void GoNext() const;
	bool IsEnd() const;
	void GoNextLine() const;
	void GoDownLine() const;
	void GoFirst() const;
	const char* GetCurrent() const;

	TText* Copy() const;
private:
	friend TNode;
	TNode* CreateNode(const char str[] = "", TNode* pNext = nullptr, TNode* pDown = nullptr) const;
	TNode* ReadRec(std::ifstream& ifs) const;
	void PrintRec(std::ofstream& ofs, TNode* p, int level_count = 0) const;
	void SetFlag() const;
	TNode* CopyNode(TNode* p) const;
};

struct TNode {
	char str[MAX_STRING_SIZE];
	TNode* pNext, * pDown;
	static TMem mem;
	bool isNotGarbage = false;
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
	static void InitMem(int s = MAX_MEMORY_SIZE)
	{
		mem.pFirst = (TNode*) new char[sizeof(TNode) * s];
		mem.pFree = mem.pFirst;
		mem.pLast = mem.pFirst + s - 1;
		TNode* p = mem.pFirst;
		for (int i = 0; i < s - 1; i++, p++)
		{
			p->pNext = p + 1;
			p->str[0] = '\0';
			p->isNotGarbage = false;
		}
		p->pNext = nullptr;
	}
	static void PrintFree() {
		TNode* p = mem.pFree;
		std::cout << "Free nodes:" << std::endl;
		while (p) {
			std::cout << p->str << std::endl;
			p = p->pNext;
		}
	}
private:
	friend TText;
	static void CleanMem(const TText& txt, bool should_i_remove_text = false);
};