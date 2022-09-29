//Dev-C++
#include <bits/stdc++.h>
using namespace std;

vector <char> allowedcharvec = {'0', '1', '2', '3', '4', '5', '6',
								'7', '8', '9', '+', '-', '*', '/',
								'(', ')', ' '}; // all allowed char in legal algorithm


list<char> &operator<<(list<char> &li,char c)
{
    li.push_back(c);
    return li;
}


list<char> &operator<<(list<char> &li,int i)
{
	for(auto c:to_string(i))
    	li.push_back(c);
    return li;
}

list<char> &operator<<(list<char> &li,string s)
{
	for(auto c:s)
    	li.push_back(c);
    return li;
}

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
			int	   numofparenthesis = 0; //number of '( - number of ')
			int	   lasttype			= -2; //1->operand,2->operator
			bool   lastisnumer		= 0; //true means last char is number
			bool   allgood			= 1; //no error
			string str;
			cin.get();                   //after cin>>k left a '\n' so use cin.get() to clear it
			cout << "Input:";
			getline(cin, str);           //read line
			for(auto i:str)
			{
				if(find(allowedcharvec.begin(), allowedcharvec.end(), i) == allowedcharvec.end()) //i must in allowedcharvec
				{
					cout << "Error 1: " << i << " is not a legitimate character.";
					allgood = 0; //set allgod to false
					break;       //if found one so that don't have to fine another
				}


				//deal with parenthesis
				if(i == '('){
					numofparenthesis++;
					lastisnumer=0;
				}


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

				if(nowtype != -1)                           //if nowtype == -1 -> no new type found
				{
					if(nowtype == lasttype && nowtype == 1) //operand after operand
					{
						cout << "Error 2: there is one extra operand.";
						allgood = 0;
						break;
					}
					if(nowtype == 2 && (lasttype==2||lasttype==-2)) //operator after operator
					{
						cout << "Error 2: there is one extra operator.";
						allgood = 0;
						break;
					}
					lasttype = nowtype; //if found new type than refresh lasttype
				}
			}
			if(lasttype == 2 && allgood )//'(' - ')' after all must = 0
			{
				cout << "Error 2: there is one extra operator.";
				allgood = 0;
			}
			if(numofparenthesis > 0 && allgood )//'(' - ')' after all must = 0
			{
				cout << "Error 2: there is one extra open parenthesis.";
				allgood = 0;
			}
			if(allgood) //no error cout right
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
			list<char> outputlist;
			int	 n					  = 0;
			bool isint				  = 0; //last is operand
			bool istimesordibide	  = 0;
			bool barckettimesordibide = 0;
			for(auto i:str)
			{
				if(i == ' ')
					continue;                 //skip ' '
				if('0' <= i && i <= '9')      //if i is number
				{
					n	  = n * 10 + i - '0'; //add to n
					isint = 1;                //set isint to true
				}
				if(i == '(')                  //operator
				{
					operatorst.push(i);       //push operator to stack
					if(istimesordibide)
					{
						barckettimesordibide = 1;
						istimesordibide		 = 0;
					}
				}
				if(i == '+' || i == '-' || i == '*' || i == '/') //operator
				{
					if(isint)
					{
						cout << n << ','; //cout operand
						n	  = 0;
						isint = 0;
						if(istimesordibide)
						{
							outputlist << operatorst.top() << ',';
							operatorst.pop();
							istimesordibide = 0;
						}
					}
					operatorst.push(i); //push operator to stack
				}

				if(i == ')') //if find ')'
				{
					if(isint)
					{
						outputlist << n << ',';
						n	  = 0;
						isint = 0;
						if(istimesordibide)
						{
							outputlist << operatorst.top() << ',';
							operatorst.pop();
							istimesordibide = 0;
						}
					}

					while(operatorst.top() != '(') //output stack until '('
					{
						outputlist << operatorst.top() << ',';
						operatorst.pop();
					}
					operatorst.pop(); //pop '('
					if(barckettimesordibide)
					{
						barckettimesordibide = 0;
						outputlist << operatorst.top() << ',';
						operatorst.pop();
					}
				}


				if(i == '*' || i == '/')
					istimesordibide = 1;
			}

			if(isint) //output remained operand
			{
				outputlist << n << ',';
				n	  = 0;
				isint = 0;
			}
			while(operatorst.size()) //output remained operator
			{
				outputlist << operatorst.top() << ',';
				operatorst.pop();
			}
			outputlist.pop_back(); //clean last ','

			for(auto c:outputlist) cout<<c;
		}


		if(k == 3)
		{
			string str;
			cin.get();
			cout << "Input:";
			getline(cin, str);
			stack <int> operandst;
			list<char> intputlist;
			intputlist<<str;
			int	 n	   = 0;
			bool isint = 0;
			for(auto i:intputlist)
			{
				if(i == ' ')
					continue;
				if('0' <= i && i <= '9')
				{
					n	  = n * 10 + i - '0';
					isint = 1;
				}

				if(i == ',' && isint == 1) //last is number
				{
					operandst.push(n);     //push opreand to stack
					n	  = 0;
					isint = 0;
				}
				if(i == '+' || i == '-' || i == '*' || i == '/')
				{
					//get a and b
					int b = operandst.top(); //stack:1,2,3,4,5,6,a,b
					operandst.pop();
					int a = operandst.top(); //stack:1,2,3,4,5,6,a
					operandst.pop();

					//push a +-*/ b back into stack
					if(i == '+')
						operandst.push(a + b);
					if(i == '-')
						operandst.push(a - b);
					if(i == '*')
						operandst.push(a * b);
					if(i == '/')
					{
						if(b != 0) // b can't be 0
							operandst.push(a / b);
						else
						{
							cout << "ERROR!!!! can't do /0";
							goto quit; //continue the while(k!=0)
						}
					}
				}
			}

			cout << "Answer: " << operandst.top(); //cout answer
quit:       ;                                      //quit at if(k==3) end
		}
	}
}
