//11027233 曾品元,11027263 黃致穎
//IDE Dev-C++, vscode, sublime, codeblock or even notepad
//Compiler gcc mingw https://github.com/Vuniverse0/mingwInstaller/releases/download/1.2.0/mingwInstaller.exe
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//https://blog.csdn.net/qq_50285142/article/details/122930647
#include <bits/stdc++.h>
using namespace std;


class department
{
public:
    string data[8];    //學校代碼 學校名稱 科系代碼 科系名稱 日間／進修別  等級別  縣市名稱 體系別
    int numofstudent;  //學生數
    int numofteacher;  //教師數
    int numofgraduate; //上學年度畢業生數

    int no = -1;       //編號


    //ifstream, istream, ofstream, ostream are friends of department's, so they can acess private members in department
    friend ifstream &operator>>(ifstream&s, department&d);
    friend istream &operator>>(istream&s, department&d);
    friend ofstream &operator<<(ofstream&s, department d);
    friend ostream &operator<<(ostream&s, department d);


    auto operator<=>(const department& other) const
    {
        return data[1] <=> other.data[1];
    }

    void operator=(const department& other)
    {
        for(int i = 0 ; i < 8 ; i++)
            data[i] = other.data[i];
        numofstudent  = other.numofstudent;
        numofteacher  = other.numofteacher;
        numofgraduate = other.numofgraduate;
        no            = other.no;
    }
};

class department_23
{
public:
    string data;
    int no;
    void operator=(const department& other)
    {
        data = other.data[1];
        no   = other.no;
    }

    auto operator<=>(const department_23& other) const
    {
        return data <=> other.data;

        //return strcmp(data[1].c_str(), other.data[1].c_str());
    }

    bool operator==(const department_23& other) const
    {
        return data == other.data;
    }
};

class department_avl
{
public:
    int numofgraduate;
    int no;
    void operator=(const department& other)
    {
        numofgraduate = other.numofgraduate;
        no            = other.no;
    }

    auto operator<=>(const department_avl& other) const
    {
        return numofgraduate <=> other.numofgraduate;

        //return strcmp(data[1].c_str(), other.data[1].c_str());
    }

    bool operator==(const department_avl& other) const
    {
        return numofgraduate == other.numofgraduate;
    }
};

ifstream &operator>>(ifstream&s, department&d) //department input file stream Overload, return what it read
{
    string str;

    getline(s, str);           //read one line
    if(str.size() < 3)         //too short = error
    {
        s.clear(ios::failbit); //set failbit to true
        return s;
    }
    int i = 0;

    for(int j = 0; j < 8; j++)                       //clear up the data[8]
        d.data[j] = "";
    for(int j = 0; j < 6; j++)                       //read in 學校代碼~等級別
    {
        for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
            d.data[j] += str[i];
        i++;                                         //skip '\t'
    }

    string num; //string to store number

    //read in 學生人數
    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    d.numofstudent = stoi(num);                                //convert string to int

    //read in 教師人數
    num = "";
    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    d.numofteacher = stoi(num);                                //convert string to int

    //read in 畢業人數
    num = "";
    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    d.numofgraduate = stoi(num);                               //convert string to int

    //read in 縣市名稱~體系別
    for(int j = 6; j < 8; j++)
    {
        for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
            d.data[j] += str[i];
        i++;                                         //skip '\t'
    }

    return s;
}

ostream &operator<<(ostream&s, department d)//department output stream Overload
{
    for(int i = 0; i < 6; i++)
        s << d.data[i] << '\t'; //optput data and use '\t' to separate data
    s << d.numofstudent << '\t';
    s << d.numofteacher << '\t';
    s << d.numofgraduate << '\t';
    for(int i = 6; i < 8; i++)
        s << d.data[i] << '\t';
    return s;
}

ofstream &operator<<(ofstream&s, department d) //department output file stream Overload
{
    for(int i = 0; i < 6; i++)
        s << d.data[i] << '\t'; //optput data and use '\t' to separate data
    s << d.numofstudent << '\t';
    s << d.numofteacher << '\t';
    s << d.numofgraduate << '\t';
    for(int i = 6; i < 8; i++)
        s << d.data[i] << '\t';
    return s;
}

class sheet_23
{
public:
    int numofdata = 0;
    string inputlocate;         //where input from e.g. 201       (also name)
    vector<department> dvec;
    sheet_23()                  //constructor let inherit to use
    {
    };

    sheet_23(string locate)     //constructer  read the file
    //((process the file name，decide if we should skip the first three lines，read the content
    {
        ifstream input;
        int      skiplines; // lines to skip in file's begin

        skiplines = 3;

        //全部都存成xxx
        if(locate.size() < 10)                                               // length("xxx") < length("input" +".txt")
            inputlocate = locate;                                            // store xxx
        else                                                                 // inputxxx.txt
        {
            inputlocate = locate.substr(5);                                  //"inputxxx.txt" to "xxx.txt"
            inputlocate = inputlocate.substr(0, inputlocate.size() - 4);     //"xxx.txt" to "xxx"
        }

        //再一律加上input和.txt
        input.open("input" + inputlocate + ".txt");     //open file "inputxxx.txt"


        //if "inputxxx.txt" can't be opened, input the filename again and repeat the steps of processing above
        while(!input)
        {
            cout << "\nCan't open file, typenew: ";
            cin >> locate;
            if(locate.size() < 10)                 //"input" +".txt"
                inputlocate = locate;
            else
            {
                inputlocate = locate.substr(5);
                inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
            }
            input.open("input" + inputlocate + ".txt");     //reopen
        }


        string str;


        for(int i = 0; i < skiplines; i++)  //input lines to skip useless information
            getline(input, str);

        // anounce a new vector
        department d;


        while(input >> d)    //while input available, then input department
        {
            d.no = ++numofdata;
            dvec.push_back(d);
            department_23 dd;
            dd = d;
            push(dd, head);
        }
        input.close();
    }

    class smallist
    {
public:
        list<department_23> li;
        ~smallist()
        {
            //cout << "destroy smallist:" << *(li.begin()) << endl;
        }
    };
    class node_23
    {
public:
        vector<shared_ptr<smallist> > value {nullptr, nullptr, nullptr};
        shared_ptr<node_23> root = nullptr;
        vector<shared_ptr<node_23> > child {nullptr, nullptr, nullptr, nullptr};
        void sortt()
        {
            sort(value.begin(), value.end(), [](shared_ptr<smallist> a, shared_ptr<smallist> b)
            {
                if(a == nullptr)
                    return false;

                if(b == nullptr)
                    return true;

                return *(a->li.begin()) < *(b->li.begin());
            });

            sort(child.begin(), child.end(), [] (shared_ptr<node_23> a, shared_ptr<node_23> b)
            {
                if(a == nullptr)
                    return false;

                if(b == nullptr)
                    return true;

                return *(a->value[0]->li.begin()) < *(b->value[0]->li.begin());
            });
        }

        int valuesize()
        {
            int x = 0;

            for(int i = 0; i < 3 ; i++)
                if(value[i] != nullptr)
                    x++;
            return x;
        }

        int childsize()
        {
            int x = 0;

            for(int i = 0; i < 4 ; i++)
                if(child[i] != nullptr)
                    x++;
            return x;
        }
    };
    shared_ptr<node_23> head = nullptr;
    void checkandpushup(shared_ptr<node_23>& originode, shared_ptr<node_23>& newnode, shared_ptr<smallist>& valuetopass)
    {
        if(originode->root == nullptr)
        {
            originode->root = make_shared<node_23>();
            shared_ptr<node_23> r = originode->root;
            r->child[0] = originode;
            r->child[1] = newnode;
            r->value[0] = valuetopass;
            r->sortt();
            newnode->root = r;
            head          = r;
            return;
        }

        shared_ptr<node_23> r = originode->root;
        r->child[3]   = newnode;
        newnode->root = r;
        r->value[2]   = valuetopass;
        r->sortt();
        if(r->valuesize() == 3)
        {
            shared_ptr<node_23> nnode = make_shared<node_23>();
            nnode->value[0] = r->value[2];
            r->value[2]     = nullptr;

            nnode->child[0]       = r->child[2];
            r->child[2]           = nullptr;
            nnode->child[0]->root = nnode;

            nnode->child[1]       = r->child[3];
            r->child[3]           = nullptr;
            nnode->child[1]->root = nnode;
            shared_ptr<smallist> nroot = r->value[1];
            r->value[1] = nullptr;
            checkandpushup(r, nnode, nroot);
        }
    }

    void push(department_23 i, shared_ptr<node_23>& node, shared_ptr<node_23> p = nullptr)
    {
        if(node == nullptr)//為空
        {
            node           = make_shared<node_23>();
            node->value[0] = make_shared<smallist>();
            node->value[0]->li.push_back(i);
            node->root = p;
            return;
        }
        for(int j = 0 ; j < 3; j++)
            if(node->value[j] != nullptr)
                if(*(node->value[j]->li.begin()) == i)
                {
                    node->value[j]->li.push_back(i);
                    return;
                }

        if(node->childsize() == 0) //為最下面的node
        {
            if(node->valuesize() == 1)
            {
                node->value[1] = make_shared<smallist>();
                node->value[1]->li.push_back(i);
                node->sortt();
                return;
            }
            if(node->valuesize() == 2)//滿了，要往上丟
            {
                node->value[2] = make_shared<smallist>();
                node->value[2]->li.push_back(i);
                node->sortt();

                shared_ptr<smallist> newroot = node->value[1];
                node->value[1] = nullptr;

                shared_ptr<node_23> newnode = make_shared<node_23>();
                newnode->value[0] = node->value[2];
                node->value[2]    = nullptr;
                checkandpushup(node, newnode, newroot);
                return;
            }
        }
        int j = 0;
        for(; j < node->valuesize(); j++)
            if(node->value[j] != nullptr && i < *(node->value[j]->li.begin()))
            {
                push(i, node->child[j], node);
                return;
            }
        push(i, node->child[j], node);
    }

    void printroot()
    {
        if(head == nullptr)
        {
            cout << "nullptr!!" << endl;
            return;
        }
        vector<department_23> v;
        for(int i = 0 ; i < 2; i++)
            if(head->value[i] != nullptr)
                for(auto&d:(head->value[i]->li))
                    v.push_back(d);
        sort(v.begin(), v.end(), [](department_23 a, department_23 b)
        {
            return a.no < b.no;
        });
        int k = 0;
        for(auto&q:v)
        {
            auto d = dvec[q.no - 1];
            cout << ++k << ": [" << d.no << "]" << " " << d.data[1] << " " << d.data[3] << " " << d.data[4] << " " << d.data[5] << " " << d.numofgraduate << endl;
        }
    }

    int gethei()
    {
        return hei(head);
    }

    int hei(shared_ptr<node_23> node)
    {
        if(node == nullptr)
            return 0;

        return max(hei(node->child[0]), max(hei(node->child[1]), hei(node->child[2]))) + 1;
    }

    int getnumofnode()
    {
        return numofnode(head);
    }

    int numofnode(shared_ptr<node_23> node)
    {
        if(node == nullptr)
            return 0;

        return 1 + numofnode(node->child[0]) + numofnode(node->child[1]) + numofnode(node->child[2]);
    }
};


class sheet_avl
{
public:
    int numofdata = 0;
    string inputlocate;         //where input from e.g. 201       (also name)
    vector<department> dvec;
    sheet_avl()                 //constructor let inherit to use
    {
    };

    sheet_avl(string locate)     //constructer  read the file
    //((process the file name，decide if we should skip the first three lines，read the content
    {
        ifstream input;
        int      skiplines; // lines to skip in file's begin

        skiplines = 3;

        //全部都存成xxx
        if(locate.size() < 10)                                               // length("xxx") < length("input" +".txt")
            inputlocate = locate;                                            // store xxx
        else                                                                 // inputxxx.txt
        {
            inputlocate = locate.substr(5);                                  //"inputxxx.txt" to "xxx.txt"
            inputlocate = inputlocate.substr(0, inputlocate.size() - 4);     //"xxx.txt" to "xxx"
        }

        //再一律加上input和.txt
        input.open("input" + inputlocate + ".txt");     //open file "inputxxx.txt"


        //if "inputxxx.txt" can't be opened, input the filename again and repeat the steps of processing above
        while(!input)
        {
            cout << "\nCan't open file, typenew: ";
            cin >> locate;
            if(locate.size() < 10)                 //"input" +".txt"
                inputlocate = locate;
            else
            {
                inputlocate = locate.substr(5);
                inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
            }
            input.open("input" + inputlocate + ".txt");     //reopen
        }


        string str;


        for(int i = 0; i < skiplines; i++)  //input lines to skip useless information
            getline(input, str);

        // anounce a new vector
        department d;


        while(input >> d)    //while input available, then input department
        {
            d.no = ++numofdata;
            dvec.push_back(d);
            department_avl dd;
            dd = d;
            push(dd, head);
        }
        input.close();
    }

    class node_avl
    {
public:
        int h = 0;
        list<department_avl> li;
        shared_ptr<node_avl> root = nullptr;
        vector<shared_ptr<node_avl> > child {nullptr, nullptr};

        void checkandchangeh()
        {
            h = 1 + max((child[0] == nullptr)? -1:child[0]->h, (child[1] == nullptr)? -1:child[1]->h);
        }

        int heightl_r()
        {
            return ((child[0] == nullptr)? -1:child[0]->h) - ((child[1] == nullptr)? -1:child[1]->h);
        }
    };

    shared_ptr<node_avl> head = nullptr;

    void rotater(shared_ptr<node_avl> node)
    {
        if(node == nullptr || (node->child[0] == nullptr && node->child[1] == nullptr))
            return;

        shared_ptr<node_avl> nnode = make_shared<node_avl>();
        swap(node->li, nnode->li);
        if(node->child[0] != nullptr)
        {
            swap(node->li, node->child[0]->li);
            nnode->child[0] = node->child[0]->child[1];
            if(nnode->child[0] != nullptr)
                nnode->child[0]->root = nnode;
        }
        nnode->child[1] = node->child[1];
        if(nnode->child[1] != nullptr)
            nnode->child[1]->root = nnode;
        if(node->child[0] != nullptr)
        {
            node->child[0] = node->child[0]->child[0];
            if(node->child[0] != nullptr)
                node->child[0]->root = node;
        }
        node->child[1] = nnode;
        nnode->root    = node;
        nnode->checkandchangeh();
    }

    void rotatel(shared_ptr<node_avl> node)
    {
        if(node == nullptr || (node->child[0] == nullptr && node->child[1] == nullptr))
            return;

        shared_ptr<node_avl> nnode = make_shared<node_avl>();
        swap(node->li, nnode->li);
        if(node->child[1] != nullptr)
        {
            swap(node->li, node->child[1]->li);
            nnode->child[1] = node->child[1]->child[0];
            if(nnode->child[1] != nullptr)
                nnode->child[1]->root = nnode;
        }
        nnode->child[0] = node->child[0];
        if(nnode->child[0] != nullptr)
            nnode->child[0]->root = nnode;
        if(node->child[1] != nullptr)
        {
            node->child[1] = node->child[1]->child[1];
            if(node->child[1] != nullptr)
                node->child[1]->root = node;
        }
        node->child[0] = nnode;
        nnode->root    = node;
        nnode->checkandchangeh();
    }

    void checkandrotate(shared_ptr<node_avl> node)
    {
        if(node == nullptr)
            return;

        int h = node->h;

        if(node->heightl_r() > 1) //right rotate
        {
            if(node->child[0]->heightl_r() < 0)
                rotatel(node->child[0]);
            rotater(node);
        }

        if(node->heightl_r() < -1) //left rotate
        {
            if(node->child[1]->heightl_r() > 0)
                rotater(node->child[1]);
            rotatel(node);
        }

        node->checkandchangeh();
        if(node->h != h)
            checkandrotate(node->root);
    }

    void push(department_avl i, shared_ptr<node_avl>& node)
    {
        if(node == nullptr)
        {
            node = make_shared<node_avl>();
            node->li.push_back(i);
            return;
        }
        if(i < *node->li.begin())
        {
            if(node->child[0] != nullptr)
            {
                push(i, node->child[0]);
                return;
            }
            node->child[0]       = make_shared<node_avl>();
            node->child[0]->root = node;
            node->child[0]->li.push_back(i);
            node->child[0]->checkandchangeh();
            checkandrotate(node);
            return;
        }
        if(i == *node->li.begin())
        {
            node->li.push_back(i);
            return;
        }
        if(i > *node->li.begin())
        {
            if(node->child[1] != nullptr)
            {
                push(i, node->child[1]);
                return;
            }
            node->child[1]       = make_shared<node_avl>();
            node->child[1]->root = node;
            node->child[1]->li.push_back(i);
            node->child[1]->checkandchangeh();
            checkandrotate(node);
            return;
        }
    }

    void printlargest(const int& numoflargest)
    {
        int count = 0;

        printlargest(numoflargest, count, head);
    }

    void printlargest(const int& numoflargest, int& count, shared_ptr<node_avl> node)
    {
        if(node == nullptr)
            return;

        printlargest(numoflargest, count, node->child[1]);
        if(count < numoflargest)
            for(auto& q:node->li)
            {
                auto d = dvec[q.no - 1];
                cout << ++count << ": [" << d.no << "]" << " " << d.data[1] << " " << d.data[3] << " " << d.data[4] << " " << d.data[5] << " " << d.numofgraduate << endl;
            }
        if(count < numoflargest)
            printlargest(numoflargest, count, node->child[0]);
    }

    void printroot()
    {
        if(head == nullptr)
        {
            cout << "nullptr!!" << endl;
            return;
        }
        int k = 0;
        for(auto&q:head->li)
        {
            auto d = dvec[q.no - 1];
            cout << ++k << ": [" << d.no << "]" << " " << d.data[1] << " " << d.data[3] << " " << d.data[4] << " " << d.data[5] << " " << d.numofgraduate << endl;
        }
    }

    int size()
    {
        return dvec.size();
    }

    int gethei()
    {
        return hei(head);
    }

    int hei(shared_ptr<node_avl> node)
    {
        if(node == nullptr)
            return 0;

        return max(hei(node->child[0]), hei(node->child[1])) + 1;
    }

    int getnumofnode()
    {
        return numofnode(head);
    }

    int numofnode(shared_ptr<node_avl> node)
    {
        if(node == nullptr)
            return 0;

        return 1 + numofnode(node->child[0]) + numofnode(node->child[1]);
    }
};


int main()
{
    int k = 100;

    sheet_avl shavl;

    while(k != 0)
    {
        cout << "\n";
        cout << "**  data operate system           **" << "\n"; //print menu
        cout << "* 0. Quit                          *" << "\n";
        cout << "* 1. 23tree                        *" << "\n";
        cout << "* 2. avltree                       *" << "\n";
        cout << "* 3. largest in avltree           **" << "\n";
        cout << ":";
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
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet_23 sh(locate);
            cout << "Tree height = " << sh.gethei() << endl;
            cout << "Number of nodes = " << sh.getnumofnode() << endl;
            sh.printroot();
            //numofstudentsheet sh(locate);  //input file locate
        }
        if(k == 2)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            shavl = sheet_avl(locate);
            cout << "Tree height = " << shavl.gethei() << endl;
            cout << "Number of nodes = " << shavl.getnumofnode() << endl;
            shavl.printroot();
            //numofstudentsheet sh(locate);  //input file locate
        }
        if(k == 3)
        {
            if(shavl.inputlocate == "")
            {
                cout << "run mission 2 first!!\n";
                continue;
            }
            int k;
            while(1)
            {
                cout << "top-K (-1 to quit): ";
                cin >> k;
                if(k == -1)
                    break;
                while(k > shavl.size())
                {
                    cout << "too much, retury top-K (-1 to quit): ";
                    cin >> k;
                }
                shavl.printlargest(k);
            }
        }
    }
}
