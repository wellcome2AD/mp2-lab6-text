#include <iostream>
#include "TDStack.h"
#include <string>

#define cout std::cout
#define string std::string

void main()
{
	TDStack<int> mas(10);

	try
	{
		for (int i = 0; i < 5; i++)
			mas.St1_Push(i);
	}
	catch (...)
	{
		cout << '\n' << "Stack is full" << '\n';
		return;
	}

	try
	{
		for (int i = 0; i < 5; i++)
			mas.St2_Push(2*i);
	}
	catch (...)
	{
		cout << '\n' << "Stack is full" << '\n';
		return;
	}
	

	if (mas.IsFull())
		cout << '\n' << "Array is full" << '\n';
	else
		cout << '\n' << "Array is not full" << '\n';


	try
	{
		cout << '\n' << "Stack ¹1" << '\n';
		for (int i = 0; i < 5; i++)
			cout << mas.St1_Pop() << ' ';
		cout << '\n';
	}
	catch (...)
	{
		cout << '\n' << "Stack ¹1 is empty" << '\n';
		return;
	}

	try
	{
		cout << '\n' << "Stack ¹2" << '\n';
		for (int i = 0; i < 5; i++)
			cout << mas.St2_Pop() << ' ';
		cout << '\n';
	}
	catch (...)
	{
		cout << '\n' << "Stack ¹2 is empty" << '\n';
		return;
	}

	if (mas.St1_IsEmpty())
		cout << '\n' << "Stack ¹1 is empty" << '\n';

	if (mas.St2_IsEmpty())
		cout << '\n' << "Stack ¹2 is empty" << '\n';
}