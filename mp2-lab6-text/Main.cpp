#include <iostream>
#include <cassert>
#include <fstream>
#include "TText.h"

int textlen(const TText& text)
{
	int size = 0;
	for (text.Reset(); !text.IsEnd(); text.GoNext(), size++);
	return size;
}

void editMenu(TText& text)
{
	int command = 0;
	int line_num = -1;
	while (command != 8)
	{
		std::cout << std::endl << text << std::endl << std::endl;
		std::cout << "----AVAILABLE COMMANDS----" << std::endl << std::endl;
		std::cout << "1. insert next line" << std::endl;
		std::cout << "2. insert down line" << std::endl;
		std::cout << "3. insert new next section" << std::endl;
		std::cout << "4. insert new down section" << std::endl;
		std::cout << "5. delete next line" << std::endl;
		std::cout << "6. delete down line" << std::endl;
		std::cout << "7. print free" << std::endl;
		std::cout << "8. exit from editing" << std::endl << std::endl;
		std::cout << "\n--input command: ";
		std::cin >> command;
		std::cout << std::endl;
		if (command < 0 || command > 8)
		{
			std::cout << "---incorrect command number" << std::endl;
			continue;
		}
		if (command != 7 && command != 8)
		{
			std::cout << "--input number of line, that you want to edit: ";
			std::cin >> line_num;
			std::cout << std::endl;
			if (line_num >= textlen(text) || line_num < 0)
			{
				std::cout << "---incorrect line_num" << std::endl << std::endl;
				continue;
			}
		}
		switch (command)
		{
		case 1:
		{
			char* str = new char[1000];
			std::cout << "---input the string for inserting: ";
			std::cin >> str;
			std::cout << std::endl;
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.InsNextLine(str);
			break;
		}
		case 2:
		{
			char* str = new char[1000];
			std::cout << "---input the string for inserting: ";
			std::cin >> str;
			std::cout << std::endl;
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.InsDownLine(str);
			break;
		}
		case 3:
		{
			char* str = new char[1000];
			std::cout << "---input the name of new next section: ";
			std::cin >> str;
			std::cout << std::endl;
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.InsNextSection(str);
			break;
		}
		case 4:
		{
			char* str = new char[1000];
			std::cout << "---input the name of new down section: ";
			std::cin >> str;
			std::cout << std::endl;
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.InsDownSection(str);
			break;
		}
		case 5:
		{
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.DelNextLine();
			break;
		}
		case 6:
		{
			text.Reset();
			for (int i = 0; i < line_num && !text.IsEnd(); i++, text.GoNext());
			text.DelDownLine();
			break;
		}
		case 7:
			TNode::PrintFree();
			break;
		case 8:
			return;
		}
	}
}

void menu()
{
	int command = 0;
	TText text;
	char* filename = new char[100];
	while (command != 5)
	{
		std::cout << "----AVAILABLE COMMANDS----" << std::endl << std::endl;;
		std::cout << "1. upload text from file" << std::endl;
		std::cout << "2. save text to file" << std::endl;
		std::cout << "3. print text" << std::endl;
		std::cout << "4. start editing text in console" << std::endl;
		std::cout << "5. exit" << std::endl;
		std::cout << "\n--input command: ";
		std::cin >> command;
		std::cout << std::endl;
		switch (command)
		{
		case 1:
		{
			std::cout << "---input file name: ";
			std::cin >> filename;
			std::cout << std::endl;
			text.ReadFromFile(filename);
			break;
		}
		case 2:
		{
			std::cout << "---input file name: ";
			std::cin >> filename;
			std::cout << std::endl;
			text.PrintToFile(filename);
			break;
		}
		case 3:
		{
			std::cout << std::endl << text << std::endl << std::endl;
			break;
		}
		case 4:
		{
			system("cls");
			editMenu(text);
			system("cls");
			break;
		}
		case 5:
			return;
		default:
		{
			std::cout << "--incorrect command number" << std::endl << std::endl;
			break;
		}
		}
	}
}

int main()
{
	menu();
}