#include <iostream>
#include <string>
#include "TCalculator.h"
#include "TStack.h"
#include "TException.h"

using namespace std;

int main()
{
	double res;
	string expr;
	TCalculator calc;
	
	while (true)
	{
		std::getline(std::cin, expr);
		calc.SetExpr(expr);

		if (expr != "0" && calc.CheckBracketsNum())
		{
			try
			{
				calc.ToPostfix();
				cout << "Postfix: " << calc.GetPostfix() << '\n';
			}
			catch (...)
			{
				cout << "Cannot convert to postfix" << '\n';
				return 0;
			}

			try
			{			
				res = calc.CalcPostfix();
				cout << "Result of calculation with postfix: " << res << '\n';
			}
			catch (const TException&)
			{
				cout << "Cannot calculate postfix expression" << '\n';
				return 0;
			}
			
			try
			{
				res = calc.Calc();
				cout << "Result of calculation without postfix: " << res << '\n';
			}
			catch (const TException& ex)
			{
				cout << "Cannot calculate" << '\n';
				return 0;
			}
		}
		else
			break;

		cout << '\n';
	}
}