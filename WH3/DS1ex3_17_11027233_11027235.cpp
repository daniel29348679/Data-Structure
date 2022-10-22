//Dev-C++ -std=c++14
//曾品元11027233/江庭瑄11027235
#include <bits/stdc++.h>
using namespace std;

template <class T>
class stackk
{
private:
	int s = 0;
	struct node
	{
		T	  t;
		node *father;
	};

	node *nowptr = NULL;
public:
	bool empty()
	{
		return s == 0;
	}

	int size()
	{
		return s;
	}

	void push(T t)
	{
		s++;
		node *n = new node;
		n->t	  = t;
		n->father = nowptr;
		nowptr	  = n;
	}

	void pop()
	{
		unique_ptr<node> i ( nowptr ); //call auto release ptr
		nowptr = nowptr->father;
		s--;
	}

	T top()
	{
		return nowptr->t;
	}
};


vector <char> allowedcharvec = {'0', '1', '2', '3', '4', '5', '6',
								'7', '8', '9', '+', '-', '*', '/',
								'(', ')', ' '}; // all allowed char in legal algorithm
struct node
{
	bool operandoroperator; //node is operand or operator
	int	 operand;
	char opertor;           //operator (operator can't use)
};


list <node> &operator<<(list <node>&li, char c)
{
	li.push_back({1, 0, c});
	return li;
}

list <node> &operator<<(list <node>&li, int i)
{
	li.push_back({0, i, '\0'});
	return li;
}

ostream &operator<<(ostream&out, node n)
{
	if(n.operandoroperator)
		out << n.opertor;
	else
		out << n.operand;
	return out;
}

int main()
{
	int k = 100;


	while(k != 0)
	{
		cout << "\n\n";
		cout << "**  Mission                    **" << "\n"; //print menu
		cout << "* 0. Quit                       *" << "\n";
		cout << "* 1. legal algorithm detection  *" << "\n";
		cout << "* 2. inorder to postifx and     *" << "\n";
		cout << "* postifx to answer(mission2&3) *" << "\n";
		cout << "*********************************" << "\n";
		cout << "Input a choice(0, 1, 2): ";
		cin >> k;
		if(cin.fail())
		{
			cout << "\nCommand does not exist!\n";
			cin.clear();  //set cin stream to good(work) situation
			cin.ignore(); //skip next char in input stream

			k = -1;       //let k != 0 to keep loop continue
			continue;     //skip this loop
		}

		if(k != 0 && k != 1 && k != 2)
		{
			cout << "\nCommand does not exist!\n";
			continue; //skip this loop
		}
		if(k == 1)
		{
			int	   numofparenthesis = 0;  //number of '(' - number of ')'
			int	   lasttype			= -2; //1->operand,2->operator
			bool   lastisnumer		= 0;  //true means last char is number
			string str;
			cin.get();                    //after cin>>k left a '\n' so use cin.get() to clear it
			cout << "Input:";
			getline(cin, str);            //read line
			for(auto i:str)
			{
				if(find(allowedcharvec.begin(), allowedcharvec.end(), i) == allowedcharvec.end()) //i must in allowedcharvec
				{
					cout << "Error 1: " << i << " is not a legitimate character.";
					goto quit1;      //if found one so that don't have to fine another
				}


				//deal with parenthesis
				if(i == '(')
				{
					numofparenthesis++;
					lastisnumer = 0;
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
					goto quit1;
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


				if(nowtype != -1)                           //if nowtype == -1 -> no new type found
				{
					if(nowtype == lasttype && nowtype == 1) //operand after operand
					{
						cout << "Error 2: there is one extra operand.";
						goto quit1;
					}
					if(nowtype == 2 && (lasttype == 2 || lasttype == -2)) //operator after operator
					{
						cout << "Error 2: there is one extra operator.";
						goto quit1;
					}
					lasttype = nowtype; //if found new type than refresh lasttype
				}
			}
			if(lasttype == 2)  //last input must be operand
			{
				cout << "Error 2: there is one extra operator.";
				goto quit1;
			}
			if(numofparenthesis > 0)  //'(' - ')' after all must = 0
			{
				cout << "Error 2: there is one extra open parenthesis.";
				goto quit1;
			}
			cout << "It is a legitimate infix expression";//no error cout right

quit1:      ;
		}

		if(k == 2)
		{
			string str;
			cin.get();
			cout << "Input:";
			getline(cin, str);
			stackk <char> operatorst;          //stack store operator
			list <node>	  outputlist;          //
			int			  n				  = 0; //store number
			bool		  isint			  = 0; //last is operand
			bool		  istimesordibide = 0; //last is '*' or '/'
			for(auto i:str)
			{
				if(i == ' ')
					continue;                     //skip ' '
				if('0' <= i && i <= '9')          //if i is number
				{
					n	  = n * 10 + i - '0';     //add to n
					isint = 1;                    //set isint to true
				}
				if(i == '(')                      //operator
				{
					operatorst.push(i);           //push operator to stack
					if(istimesordibide)           //if there is '*' or '/' before '('
					{
						istimesordibide = 0;      //reset istimesordibide
					}
				}
				if(i == '+' || i == '-' || i == '*' || i == '/') //operator
				{
					if(isint)                                    //if stored number than output
					{
						outputlist << n;                         //store operand
						n	  = 0;                               //reset n
						isint = 0;                               //reset isint
						if(istimesordibide)                      //if there is '*' or '/' before operand
						{
							outputlist << operatorst.top();      //output operator
							operatorst.pop();                    //pop operator
							istimesordibide = 0;                 //reset istimesordibide
						}
					}
					operatorst.push(i); //push operator to stack
				}

				if(i == ')')  //if find ')'
				{
					if(isint) //if stored number than output
					{
						outputlist << n;
						n	  = 0;
						isint = 0;
						if(istimesordibide)
						{
							outputlist << operatorst.top();
							operatorst.pop();
							istimesordibide = 0;
						}
					}

					while(operatorst.top() != '(') //output stack until '('
					{
						outputlist << operatorst.top();
						operatorst.pop();
					}
					operatorst.pop();                                                               //pop '('
					if(!operatorst.empty() && (operatorst.top() == '*' || operatorst.top() == '/')) //'*' or '/' before '(' must output when '(' remove
					{
						outputlist << operatorst.top();
						operatorst.pop();
					}
				}

				if(i == '*' || i == '/')
					istimesordibide = 1;
			}

			if(isint)                //output remained operand
			{
				outputlist << n;     //store into outputlist
				n	  = 0;           //reset n
				isint = 0;           //reset isint
			}
			while(operatorst.size()) //output remained operator
			{
				outputlist << operatorst.top();
				operatorst.pop();
			}

			for(auto c:outputlist)
				cout << c << ','; //print list
			cout << '\b' << '\0';

			//mission 3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			stackk <int> operandst;
			n = 0;
			for(auto i:outputlist)
			{
				if(i.operandoroperator == 0) //if i is operand
					operandst.push(i.operand);
				if(i.operandoroperator == 1) //if i is operator
				{
					//get a and b
					int b = operandst.top(); //stack:1,2,3,4,5,6,a,b
					operandst.pop();
					int a = operandst.top(); //stack:1,2,3,4,5,6,a
					operandst.pop();

					//push a +-*/ b back into stack
					if(i.opertor == '+')
						operandst.push(a + b); //stack:1,2,3,4,5,6,a+b
					if(i.opertor == '-')
						operandst.push(a - b);
					if(i.opertor == '*')
						operandst.push(a * b);
					if(i.opertor == '/')
					{
						if(b != 0) // b can't be 0
							operandst.push(a / b);
						else
						{
							cout << "ERROR!!!! can't do /0";
							goto quit2; //quit this mission
						}
					}
				}
			}
			cout << "\nAnswer: " << operandst.top(); //cout answer
quit2:      ;                                      //quit at if(k==2) end
		}
	}
}
