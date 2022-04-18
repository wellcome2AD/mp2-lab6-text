#include <iostream>
#include <fstream>
#include  "TText.h"
using namespace std;



int main()
{
    for (int i = 0; i < 100; ++i)
    {
        TText text;
        text.ReadFromFile("test.txt");
        text.Print("output.txt");
    }
}