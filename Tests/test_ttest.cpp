#include "gtest.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <list>
#include <array>
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

TEST(TText, can_read_and_print_text)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.PrintToFile("tests/test_out.txt");
	ASSERT_EQ(FileName("tests/test_in.txt"), FileName("tests/test_out.txt"));
}

TEST(TText, can_iterate_through_text)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE1", "TEXT", "TITLE2", "TEXT" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}
	
	ASSERT_EQ(expected_result, actual_result);
}


TEST(TText, can_go_first_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	std::string res = text.GetCurrent();
	std::string exp = "MAIN_TITLE";
	ASSERT_EQ(exp, res);
}

TEST(TText, can_go_next_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	text.GoNextLine();
	std::string res = text.GetCurrent();
	std::string exp = "TITLE2";
	ASSERT_EQ(exp, res);
}

TEST(TText, can_go_down_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	std::string res = text.GetCurrent();
	std::string exp = "TITLE1";
	ASSERT_EQ(exp, res);
}

TEST(TText, can_insert_next_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.InsNextLine("MAIN_TITLE1");

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE1", "TEXT", "TITLE2", "TEXT", "MAIN_TITLE1" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_insert_down_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.InsDownLine("TITLE0");

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE0", "TITLE1", "TEXT", "TITLE2", "TEXT" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_insert_new_next_section)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.InsNextSection("SECTION");

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE1", "TEXT", "TITLE2", "TEXT", "SECTION" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_insert_new_down_section)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.InsDownSection("SECTION");

	std::list<std::string> expected_result = { "MAIN_TITLE", "SECTION", "TITLE1", "TEXT", "TITLE2", "TEXT" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_delete_next_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	text.DelNextLine();

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE1", "TEXT" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_delete_down_line)
{
	TText text;
	text.ReadFromFile("tests/test_in.txt");
	text.GoFirst();
	text.GoDownLine();
	text.DelDownLine();

	std::list<std::string> expected_result = { "MAIN_TITLE", "TITLE1", "TITLE2", "TEXT" };
	std::list<std::string> actual_result;
	for (text.Reset(); !text.IsEnd(); text.GoNext())
	{
		actual_result.push_back(text.GetCurrent());
	}

	ASSERT_EQ(expected_result, actual_result);
}

TEST(TText, can_copy_text)
{
	TText expected_text;
	expected_text.ReadFromFile("tests/test_in.txt");
	std::unique_ptr<TText> actual_text(expected_text.Copy());

	ASSERT_EQ(expected_text, *actual_text);
}

TEST(TText, throws_bad_alloc_because_out_of_memory)
{
	TText text;
	ASSERT_THROW(text.ReadFromFile("tests/memory_test_in.txt"), std::bad_alloc);
}

TEST(TText, can_create_and_delete_several_objects)
{
	ASSERT_NO_THROW(
		for (int i = 0; i < 100; ++i)
		{
			TText text;
			text.ReadFromFile("tests/memory_test2_in.txt");
		});
}

TEST(TText, can_insert_into_several_elements_and_desrtoy_them_after_that)
{
	constexpr int size = 3;
	std::array<TText*, size> texts;
	ASSERT_NO_THROW(
		for (int count = 0; count < 100; ++count)
		{
			for (int i = 0; i < size; ++i)
			{
				texts[i] = new TText();
				texts[i]->GoFirst();
			}
			for (int i = 0; i < MAX_MEMORY_SIZE / size; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					texts[j]->InsNextLine("Test");
				}
			}
			for (int i = 0; i < 3; ++i)
			{
				delete texts[i];
			}
		});
}

TEST(TText, can_create_new_text_after_exception)
{
	try {
		TText text;
		text.ReadFromFile("tests/memory_test_in.txt");
		ASSERT_TRUE(false); // exception expected
	}
	catch (const std::bad_alloc&)
	{
		ASSERT_NO_THROW(
			for (int i = 0; i < 100; ++i)
			{
				TText text;
				text.ReadFromFile("tests/memory_test2_in.txt");
			});
	}
	
}