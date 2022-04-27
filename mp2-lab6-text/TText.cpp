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

void TText::GoNextLine() const 
{
	if (pCurr != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pNext;
	}
}

void TText::GoDownLine() const {
	if (pCurr != nullptr) {
		st.Push(pCurr);
		pCurr = pCurr->pDown;
	}
}

const char* TText::GetCurrent() const
{
	if (pCurr)
		return pCurr->str;
	return "";
}

TNode* TText::ReadRec(std::ifstream& ifs) const
{
	TNode* pHead = nullptr, * pTemp = nullptr;
	char buf[MAX_STRING_SIZE];
	while (!ifs.eof()) {
		ifs.getline(buf, MAX_STRING_SIZE, '\n');
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

void TText::PrintRec(std::ostream& os, TNode* p, int level_count, int& line_number, bool print_to_file) const
{
	const int space_count = level_count * 2;
	bool is_head = p == pFirst;
	if (p != nullptr) {
		if (!is_head)
			os << std::endl;
		if (!print_to_file)
			os << line_number++ << ' ';
		 os << std::string(space_count, ' ') << p->str;
		if (p->pDown != nullptr)
			os << std::endl << std::string(space_count + 2, ' ') << '{';
		PrintRec(os, p->pDown, level_count + 1, line_number, print_to_file);
		if (p->pDown != nullptr)
			os << std::endl << std::string(space_count + 2, ' ') << '}';
		PrintRec(os, p->pNext, level_count, line_number, print_to_file);
	}
}

void TText::GoFirst() const 
{
	pCurr = pFirst;
	st.Clear();
}

void TText::InsNextLine(const char* str) //insert next element on the same level
{ 
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, pCurr->pNext);
		pCurr->pNext = newStr;
	}
}

void TText::InsDownLine(const char* str) //insert next element on the same level
{ 
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, pCurr->pDown);
		pCurr->pDown = newStr;
	}
}

void TText::InsNextSection(const char* str)
{
	if (pCurr != nullptr) {
		TNode* newStr = CreateNode(str, nullptr, pCurr->pNext);
		pCurr->pNext = newStr;
	}
}

void TText::InsDownSection(const char* str) 
{
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

void TText::ReadFromFile(char const* filename) {
	std::ifstream ifs(filename);
	if (ifs.is_open()) {
		pFirst = ReadRec(ifs);
	}
	else
	{
		std::cout << filename << " doesn't exist\n\n";
		return;
	}
}

void TText::Reset() const 
{
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

void TText::PrintToFile(char const* filename) const
{
	std::ofstream ofs(filename);
	int line_num = 0;
	if (ofs.is_open()) {
		PrintRec(ofs, pFirst, 0, line_num, true);
	}
}

void TText::GoNext() const
{
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

bool TText::IsEnd() const { return pCurr == nullptr || st.IsEmpty(); }

//void TText::SetFlag() const { pCurr->isNotGarbage = true; }

TText* TText::Copy() const { return new TText(CopyNode(pFirst)); }

TNode* TText::CopyNode(TNode* p) const
{
	TNode* pN = nullptr, * pD = nullptr, * res;
	if (p->pNext)
		pN = CopyNode(p->pNext);
	if (p->pDown)
		pD = CopyNode(p->pDown);
	res = CreateNode(p->str, pN, pD);
	return res;
}

void TText::SetFlag() const { pCurr->isNotGarbage = true; }

TNode* TText::CreateNode(const char str[], TNode* pNext, TNode* pDown) const
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

void TNode::CleanMem(const TText& txt, bool should_i_remove_text) 
{
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

bool TText::operator==(const TText& other) const 
{ 
	for (this->Reset(), other.Reset(); !this->IsEnd() && !other.IsEnd(); this->GoNext(), other.GoNext())
	{
		const char* this_cur = this->GetCurrent();
		const char* other_cur = other.GetCurrent();
		if (strcmp(this_cur, other_cur))
			return false;
	}
	if (!this->IsEnd() || !other.IsEnd())
		return false;
	return true;
}

std::ostream& operator<<(std::ostream& os, const TText& text)
{
	int line_num = 0;
	text.PrintRec(os, text.pFirst, 0, line_num, false);
	return os;
}