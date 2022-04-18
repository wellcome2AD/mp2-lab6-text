#include "gtest.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include "..\mp2-lab6-text\TText.h"

struct FileName
{
	std::string name;
	FileName(std::string _name) : name(_name) {}
	bool operator==(const FileName& other) const
	{
		std::ifstream files[2] = { std::ifstream(name), std::ifstream(other.name) };
		std::string files_contents[2];
		for(int i=0; i<2; i++)
		{
			if (!files[i].is_open())
				throw std::runtime_error("Can't open file");
			files_contents[i] = { std::istreambuf_iterator<char>(files[i]), std::istreambuf_iterator<char>() };

			std::string regex_string = R"(\s+)";
			std::string res;
			std::regex pattern(regex_string);
			std::regex_replace(std::back_inserter(res), files_contents[i].begin(), files_contents[i].end(), pattern, " ");
			files_contents[i] = res;
		}
		return files_contents[0] == files_contents[1];
	}

};

TEST(TTest, can_read_and_print_text)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.PrintToFile("tests/test_out.txt");
	ASSERT_EQ(FileName("tests/test_in.txt"), FileName("tests/test_out.txt"));
}

TEST(TTest, can_go_first_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	const char* res = text.GetCurrent();
	ASSERT_EQ(res, "MAIN_TITLE");
}

TEST(TTest, can_go_next_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	text.GoNextLine();
	const char* res = text.GetCurrent();
	ASSERT_EQ(res, "TITLE2");
}

TEST(TTest, can_go_down_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	const char* res = text.GetCurrent();
	ASSERT_EQ(res, "TITLE1");
}