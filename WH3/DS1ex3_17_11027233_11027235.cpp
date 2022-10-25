//Dev-C++ -std=c++14
//曾品元11027233/江庭瑄11027235
#include <bits/stdc++.h>
using namespace std;

//#define err

template<class T>
class stackk
{
private:
    int s = 0; //size
    struct node
    {
        T                t;
        shared_ptr<node> father;
    };

    shared_ptr<node> nowptr;   //auto release ptr
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
        shared_ptr<node> n = make_shared<node>();
        n->t      = t;
        n->father = nowptr;
        nowptr    = n;
    }

    void pop()
    {
        nowptr = nowptr->father;
        s--;
    }

    T top()
    {
        return nowptr->t;
    }
};

template<class T>
class listt
{
public:
    int s = 0; //size
    struct node
    {
        T                t;
        shared_ptr<node> next; //auto release ptr
    };

    shared_ptr<node> headptr;


    void push_back(T t)
    {
        s++;
        if(headptr == NULL)
        {
            headptr    = make_shared<node>();
            headptr->t = t;
            return;
        }
        auto x = headptr;
        while(x->next != NULL)
            x = x->next;
        x->next = make_shared<node>();
        x       = x->next;
        x->t    = t;
    }

    shared_ptr<node> begin()
    {
        return headptr;
    }
};


vector<char> allowedcharvec = {'0', '1', '2', '3', '4', '5', '6',
                               '7', '8', '9', '+', '-', '*', '/',
                               '(', ')', ' '};  // all allowed char in legal algorithm
struct node
{
    bool operandoroperator; //node is operand or operator
    int  operand;
    char opertor;           //operator (operator can't use)
};


list<node> &operator<<(list<node>&li, char c)
{
    li.push_back({1, 0, c});
    return li;
}

list<node> &operator<<(list<node>&li, int i)
{
    li.push_back({0, i, '\0'});
    return li;
}

listt<node> &operator<<(listt<node>&li, char c)
{
    li.push_back({1, 0, c});
    return li;
}

listt<node> &operator<<(listt<node>&li, int i)
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

int place(node n)
{
    if(n.opertor == '+' || n.opertor == '-')
        return 1;

    if(n.opertor == '*' || n.opertor == '/')
        return 2;

    return 0;
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
        cout << "* 3. challenge                  *" << "\n";
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
            int    numofparenthesis = 0;  //number of '(' - number of ')'
            int    lasttype         = -2; //1->operand,2->operator
            bool   lastisnumer      = 0;  //true means last char is number
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
                if(i == ' ')
                    continue;                     //skip ' '

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
                        lasttype    = 1;
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
                    nowtype     = 2;
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

                #ifdef err
                cout << "now:" << i << endl;
                #endif
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
            stackk<char> operatorst;           //stack store operator
            listt<node>  outputlist;           //
            int          n               = 0;  //store number
            bool         isint           = 0;  //last is operand
            bool         istimesordibide = 0;  //last is '*' or '/'
            for(auto i:str)
            {
                if(i == ' ')
                    continue;                     //skip ' '
                if('0' <= i && i <= '9')          //if i is number
                {
                    n     = n * 10 + i - '0';     //add to n
                    isint = 1;                    //set isint to true
                    continue;
                }
                if(isint)     //if stored number than output
                {
                    outputlist << n;
                    n     = 0;
                    isint = 0;
                    if(istimesordibide)
                    {
                        outputlist << operatorst.top();
                        operatorst.pop();
                        istimesordibide = 0;
                    }
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
                    if(i == '+' || i == '-')
                        if(operatorst.size() && operatorst.top() != '(')
                        {
                            outputlist << operatorst.top();
                            operatorst.pop();
                        }
                    if(i == '*' || i == '/')
                        istimesordibide = 1;
                    operatorst.push(i); //push operator to stack
                }

                if(i == ')')                       //if find ')'
                {
                    while(operatorst.top() != '(') //output stack until '('
                    {
                        outputlist << operatorst.top();
                        operatorst.pop();
                    }
                    operatorst.pop();                                                             //pop '('
                    if(operatorst.size() && (operatorst.top() == '*' || operatorst.top() == '/')) //'*' or '/' before '(' must output when '(' remove
                    {
                        outputlist << operatorst.top();
                        operatorst.pop();
                    }
                }
            }

            if(isint)                //output remained operand
            {
                outputlist << n;     //store into outputlist
                n     = 0;           //reset n
                isint = 0;           //reset isint
            }
            while(operatorst.size()) //output remained operator
            {
                outputlist << operatorst.top();
                operatorst.pop();
            }

            for(auto ptr = outputlist.begin(); ptr != nullptr; ptr = ptr->next) //auto c:outputlist
            {
                auto c = ptr->t;
                cout << c << ',';  //print list
            }

            cout << '\b' << ' ';   //delete last ','

            //mission 3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            stackk<int> operandst;
            n = 0;
            for(auto ptr = outputlist.begin(); ptr != nullptr; ptr = ptr->next) //auto i:outputlist
            {
                auto i = ptr->t;
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
quit2:      ;                                        //quit at if(k==2) end
        }

        if(k == 3)
        {
            string str;
            cin.get();
            cout << "Input:";
            getline(cin, str);
            list<node> nodli;
            nodli << '(';
            bool isint = 0;
            int  n     = 0;
            for(auto i:str) //convert strint into useful data
            {
                if(i == ' ')
                    continue;
                if('0' <= i && i <= '9')
                {
                    n     = n * 10 + i - '0';     //add to n
                    isint = 1;                    //set isint to true
                    continue;
                }
                if(isint)
                {
                    nodli << n;
                    n     = 0;
                    isint = 0;
                }
                nodli << i;
            }
            if(isint)
            {
                nodli << n;
                n     = 0;
                isint = 0;
            }
            nodli << ')';

            vector<node> ansvec;
            stack<node>  operatorst;

            for(auto ii = nodli.rbegin(); ii != nodli.rend(); ii++) //auto i:nodli but reverse
            {
                auto i = *ii;
                if(i.operandoroperator == 0)
                {
                    ansvec.insert(ansvec.begin(),i);
                    continue;
                }

                if(i.opertor == '+' || i.opertor == '-' || i.opertor == '*' || i.opertor == '/')
                {
                    while(place(operatorst.top()) > place(i))
                    {
                        ansvec.insert(ansvec.begin(),operatorst.top());
                        operatorst.pop();
                    }
                    operatorst.push(i);
                }

                if(i.opertor == '(')
                {
                    while(operatorst.top().opertor != ')')
                    {
                        ansvec.insert(ansvec.begin(),operatorst.top());
                        operatorst.pop();
                    }
                    operatorst.pop();
                }
                if(i.opertor == ')')
                    operatorst.push(i);
            }
            cout << "preorder:";
            for(auto t:ansvec)
                cout << t << ',';
            cout << '\b' << ' ';

            while(ansvec.size() > 1)
            {
                int last = 0;
                int ans;
                for(int i = 0; i < ansvec.size() - 2; i++)
                    if(ansvec[i].operandoroperator == 1 && ansvec[i + 1].operandoroperator == 0 && ansvec[i + 2].operandoroperator == 0)
                        last = i;

                int b = ansvec[last + 2].operand;
                int a = ansvec[last + 1].operand;

                //push a +-*/ b back into stack
                if(ansvec[last ].opertor == '+')
                    ans = (a + b);     //stack:1,2,3,4,5,6,a+b
                if(ansvec[last ].opertor == '-')
                    ans = (a - b);
                if(ansvec[last ].opertor == '*')
                    ans = (a * b);
                if(ansvec[last].opertor == '/')
                {
                    if(b != 0)     // b can't be 0
                        ans = (a / b);
                    else
                    {
                        cout << "ERROR!!!! can't do /0";
                        goto quit3;     //quit this mission
                    }
                }

                ansvec.erase(ansvec.begin()+last + 2);
                ansvec.erase(ansvec.begin()+last + 1);
                ansvec[last]={0, ans, '\0'};
            }
            cout<<"\nAns:"<<ansvec[0];
quit3:      ;
        }
    }
}
