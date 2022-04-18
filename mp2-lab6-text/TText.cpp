#include <fstream>
#include <iostream>
#include "TStack.h"
#include "TText.h"

TMem::TMem()
	: pFirst(nullptr), pFree(nullptr), pLast(nullptr)
{
	TNode::InitMem();
}

TMem TNode::mem;

TText::TText() = default;

TText::TText(TNode* _pFirst) : pFirst(_pFirst) {}

TText::~TText()
{
	TNode::CleanMem(*this, true);
}

void TText::GoNextLine() {
	if (pCurr != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pNext;
	}
}

void TText::GoDownLine() {
	if (pCurr != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pDown;
	}
}

const char* TText::GetCurrent()
{
	if (pCurr)
		return pCurr->str;
	return "";
}

void TText::GoFirst() {
	pCurr = pFirst;
	st.Clear();
}

void TText::InsNextLine(char str[]) { //insert next element on the same level
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, pCurr->pNext);
		pCurr->pNext = newStr;
	}
}

void TText::InsDownLine(char str[]) { //insert next element on the same level
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, pCurr->pDown);
		pCurr->pDown = newStr;
	}
}

void TText::InsNextSection(char str[]) {
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, nullptr, pCurr->pNext);
		pCurr->pNext = newStr;
	}
}

void TText::InsDownSection(char str[]) {
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, nullptr, pCurr->pDown);
		pCurr->pDown = newStr;
	}
}

void TText::DelNextLine() {
	if (pCurr != nullptr) {
		if (pCurr->pNext != nullptr) {
			TNode* pDel = pCurr->pNext;
			pCurr->pNext = pDel->pNext;
			delete pDel;
		}
	}
}

void TText::DelDownLine() {
	if (pCurr != nullptr) {
		if (pCurr->pDown != nullptr) {
			TNode* pDel = pCurr->pDown;
			pCurr->pDown = pDel->pNext;
			delete pDel;
		}
	}
}

TNode* TText::ReadRec(std::ifstream& ifs) {
	TNode* pHead = nullptr, * pTemp = nullptr;
	char buf[MAX_SIZE];
	while (!ifs.eof()) {
		ifs.getline(buf, MAX_SIZE, '\n');
		if (buf[0] == '}')
			break;
		else if (buf[0] == '{')
			pTemp->pDown = ReadRec(ifs);
		else {
			TNode* p = CreateNode(buf);
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

void TText::ReadFromFile(char const* filename) {
	std::ifstream ifs(filename);
	if (ifs.is_open()) {
		pFirst = ReadRec(ifs);
	}
}

void TText::PrintRec(std::ofstream& ofs, TNode* p, int level_count)
{
	const int space_count = level_count * 2;
	if (p != nullptr) {
		ofs << std::string(space_count, ' ') << p->str << std::endl;
		if (p->pDown != nullptr)
			ofs << std::string(space_count, ' ') << '{' << std::endl;
		PrintRec(ofs, p->pDown, level_count + 1);
		if (p->pDown != nullptr)
			ofs << std::string(space_count, ' ') << '}' << std::endl;
		PrintRec(ofs, p->pNext, level_count);
	}
}

void TText::PrintToFile(char const* filename) {
	std::ofstream ofs(filename);
	if (ofs.is_open()) {
		PrintRec(ofs, pFirst);
	}
}

void TText::Reset() {
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

void TText::GoNext() {
	pCurr = st.Pop();
	if (pCurr != pFirst) {
		if (pCurr) {
			if (pCurr->pNext)
				st.Push(pCurr->pNext);
			if (pCurr->pDown)
				st.Push(pCurr->pDown);
		}
	}
}

bool TText::IsEnd() { return pCurr == nullptr || st.IsEmpty(); }

void TText::SetFlag() { pCurr->isNotGarbage = true; }

TNode* TText::CopyNode(TNode* p) {
	TNode* pN = nullptr, * pD = nullptr, * res;
	if (p->pNext)
		pN = CopyNode(p->pNext);
	if (p->pDown)
		pN = CopyNode(p->pDown);
	res = CreateNode(p->str, pN, pD);
	return res;
}

TText* TText::Copy() { return new TText(CopyNode(pFirst)); }

TNode* TText::CreateNode(const char str[], TNode* pNext, TNode* pDown)
{
	TNode* res = new TNode(str, pNext, pDown);
	if (res == nullptr)
	{
		TNode::CleanMem(*this);
		res = new TNode(str, pNext, pDown);
		if (res == nullptr)
		{
			throw std::bad_alloc();
		}
	}
	
	return res;
}

void TNode::CleanMem(TText& txt, bool should_i_remove_text) {
	TNode* p = mem.pFree;
	while (p) {
		p->isNotGarbage = true;
		p = p->pNext;
	}
	if (!should_i_remove_text)
	{
		txt.Reset();
		if (txt.IsEnd())
		{
			throw std::bad_alloc();
		}
		for (; !txt.IsEnd(); txt.GoNext()) {
			// to do: write TText::SetFlag that sets pCurr->isNotGarbage
			txt.SetFlag();
		}
	}
	for (p = mem.pFirst; p <= mem.pLast; p++) {
		if (!p->isNotGarbage) {
			delete p;
			p->isNotGarbage = true;
		}
	}
}