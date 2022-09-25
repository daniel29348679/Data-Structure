//Dev-C++
#include <bits/stdc++.h>
using namespace std;

vector <char> allowedcharvec = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '-', '*', '/', '(', ')', ' '};


int main()
{
	int k = 100;


	while(k != 0)
	{
		cout << "\n\n";
		cout << "**  Function                   **" << "\n"; //print menu
		cout << "* 0. Quit                       *" << "\n";
		cout << "* 1. legal algorithm detection  *" << "\n";
		cout << "* 2. inorder to postifx         *" << "\n";
		cout << "* 3. postifx to answer          *" << "\n";
		cout << "*********************************" << "\n";
		cout << "Input a choice(0, 1, 2, 3): ";
		cin >> k;
		if(cin.fail())
		{
			cout << "\nCommand does not exist!\n";
			cin.clear();  //set cin stream to good(work) situation
			cin.ignore(); //skip next char in input stream

			k = -1;       //let k != 0 to keep loop continue
			continue;     //skip this loop
		}

		if(k != 0 && k != 1 && k != 2 && k != 3)
		{
			cout << "\nCommand does not exist!\n";
			continue; //skip this loop
		}
		if(k == 1)
		{
			int	   numofparenthesis = 0;
			int	   lasttype			= 2;
			bool   lastisnumer		= 0;
			bool   allgood			= 1;
			string str;
			cin.get();
			cout << "Input:";
			getline(cin, str);
			for(auto i:str)
			{
				if(find(allowedcharvec.begin(), allowedcharvec.end(), i) == allowedcharvec.end())
				{
					cout << "Error 1: " << i << " is not a legitimate character.";
					allgood = 0;
					break;
				}


				//deal with parenthesis
				if(i == '(')
					numofparenthesis++;
				if(i == ')')
				{
					numofparenthesis--;
					if(lastisnumer)
					{
						lastisnumer = 0;
						lasttype	= 1;
					}
				}

				if(numofparenthesis < 0) //number of '(' must always >= ')'
				{
					cout << "Error 2: there is one extra close parenthesis.";
					allgood = 0;
					break;
				}


				int nowtype = -1;

				if('0' <= i && i <= '9')
				{
					if(!lastisnumer)
						nowtype = 1;
					lastisnumer = 1;
				}

				if(i == '+' || i == '-' || i == '*' || i == '/')
				{
					nowtype		= 2;
					lastisnumer = 0;
				}

				if(i == ' ')
					lastisnumer = 0;

				if(nowtype != -1)
				{
					if(nowtype == lasttype && nowtype == 1)
					{
						cout << "Error 2: there is one extra operand.";
						allgood = 0;
						break;
					}
					if(nowtype == lasttype && nowtype == 2)
					{
						cout << "Error 2: there is one extra operator.";
						allgood = 0;
						break;
					}
					lasttype = nowtype;
				}
			}

			if(numofparenthesis > 0)
			{
				cout << "Error 2: there is one extra open parenthesis.";
				allgood = 0;
				continue;
			}
			if(allgood)
				cout << "It is a legitimate infix expression";

			//(29+101)*33/25
			//24*7770^(55+30*2)
			//(90+(70*(68-55/10))
			//(24*1111)19/(55+30*2)
		}

		if(k == 2)
		{
			string str;
			cin.get();
			cout << "Input:";
			getline(cin, str);
			stack <char> operatorst;

			int n = 0;
			for(auto i:str)
			{
				if(i == ' ')
					continue;
				if('0' <= i && i <= '9')
					n = n * 10 + i - '0';

				if(i == '+' || i == '-' || i == '*' || i == '/' || i == '(')
				{
					if(n > 0)
					{
						cout << n << ',';
						n = 0;
					}
					operatorst.push(i);
				}

				if(i == ')')
				{
					if(n > 0)
					{
						cout << n << ',';
						n = 0;
					}
					char c = operatorst.top();
					operatorst.pop();
					while(c != '(')
					{
						cout << c << ',';
						c = operatorst.top();
						operatorst.pop();
					}
				}
			}
			if(n > 0)
			{
				cout << n << ',';
				n = 0;
			}
			while(operatorst.size())
			{
				cout << operatorst.top() << ',';
				operatorst.pop();
			}
			cout << '\b' << " ";
		}

		if(k == 3)
		{
			string str;
			cin.get();
			cout << "Input:";
			getline(cin, str);
			stack <int> operand;

			int n = 0;
			bool isint =0;
			for(auto i:str)
			{
				if(i == ' ')
					continue;
				if('0' <= i && i <= '9'){
					n = n * 10 + i - '0';
					isint=1;
				}

				if(i == ','&&isint==1)
				{
					operand.push(n);
					n = 0;
					isint=0;
				}
				if(i == '+' || i == '-' || i == '*' || i == '/')
				{
					int b = operand.top();
					operand.pop();
					int a = operand.top();
					operand.pop();

					if(i == '+')
						operand.push(a + b);
					if(i == '-')
						operand.push(a - b);
					if(i == '*')
						operand.push(a * b);
					if(i == '/'){
						if(b!=0)
							operand.push(a / b);
						else
						{
							cout<<"ERROR!!!! can't do /0";
							goto quit;
						}
					}
				}
			}

			cout <<"Answer: "<< operand.top();
			while(operand.size())operand.pop();
		quit:;
		}
	}
}
