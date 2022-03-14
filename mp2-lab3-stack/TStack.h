#pragma once

#include "TException.h"

template <class T>

class TStack
{
	T* Stack;
	int Size, NumLast;

public:
	TStack(int _size = 10) : Size(_size) 
	{
		Stack = new T[Size];
		NumLast = -1;
	}

	~TStack() { delete[] Stack; }

	TStack& operator=(const TStack<T>& s)
	{
		if (Size != s.Size) {
			Size = s.Size;
			delete[] Stack;
			Stack = new T[Size];
		}

		NumLast = s.NumLast;

		for (int i = 0; i <= NumLast; i++)
			Stack[i] = s.Stack[i];
	}

	bool IsEmpty() const { return (NumLast == -1); }

	bool IsFull() const { return (NumLast == (Size - 1)); }

	void Clear() { NumLast = -1; }

	void Push(const T& element)
	{
		if (IsFull())
			throw TException("Stack is full");
		Stack[++NumLast] = element;
	}

	T Pop()
	{
		if (IsEmpty())
			throw TException("Stack is empty");
		return Stack[NumLast--];
	}

	T Top()
	{
		if (IsEmpty())
			throw TException("Stack is empty");
		return Stack[NumLast];
	}
};
