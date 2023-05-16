//11027233 曾品元,11027263 黃致穎
//IDE Dev-C++, vscode, sublime, codeblock or even notepad
//Compiler gcc mingw https://github.com/Vuniverse0/mingwInstaller/releases/download/1.2.0/mingwInstaller.exe
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//https://blog.csdn.net/qq_50285142/article/details/122930647
#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

//#define debug    ;

LARGE_INTEGER nFreq;
LARGE_INTEGER nBeginTime;
LARGE_INTEGER nEndTime;

void startTime() //start timing
{
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);
}

double getTime() //get timing (second)
{
    QueryPerformanceCounter(&nEndTime);
    return (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
}

class pairs_bin
{
public:
    char putid[10] = "";
    char getid[10] = "";
    float weight   = 0.0;
    //ifstream, istream, ofstream, ostream are friends of person_bin's, so they can acess private members in person_bin
    friend ifstream &operator>>(ifstream&s, pairs_bin&p);
    friend ofstream &operator<<(ofstream&s, pairs_bin&p);
};


ifstream &operator>>(ifstream&s, pairs_bin&p) //person_bin output file stream Overload
{
    if(!s.read((char*)&p, sizeof(p)))         //copy block
        s.clear(ios::failbit);                //set failbit if read failed
    return s;
}

ofstream &operator<<(ofstream&s, pairs_bin&p) //person_bin output file stream Overload
{
    s.write((char*)&p, sizeof(p));            //copy block
    return s;
}

class node
{
public:
    string id;
    vector<pair<int, float> > nexts;

    node()
    {
    }

    node(string s)
    {
        id = s;
    }

    void putnext(int& i, float& j) // to push back nexts
    {
        nexts.push_back(make_pair(i, j));
    }
};


class dgraph
{
public:
    string input_filename;

    set<string> idset;                          //all id here
    vector<tuple<string, string, float> > dvec; //Directed_graph
    vector<node> nodevec;                       //node
    map<string, int> indexmap;                  //id to index
    void read(string locate)                    //constructer  read the file
    {
        ifstream input;

        idset.clear();
        dvec.clear();

        //再一律加上input和.txt
        //input.open("input" + locate + ".bin", ios::binary);     //open file "inputxxx.txt"

        for(;; cout << "\nCan't open file, typenew: ", cin >> locate)
        {
            input_filename = locate;
            input.open("pairs" + locate + ".bin", ios::binary); //try input bin
            if(input)
            {
                pairs_bin p;
                while(input >> p)
                {
                    dvec.push_back(make_tuple(string(p.putid), string(p.getid), p.weight));
                    idset.insert(p.getid);
                    idset.insert(p.putid);
                    #ifdef debug
                    cout << p.putid << " " << p.getid << " " << p.weight << "\n";
                    #endif
                }

                input.close();
                return;
            }
            input.close();
        }
    }

    void adjlist() //mission 1
    {
        indexmap.clear();
        nodevec.clear();
        for(auto&s:idset) //build nodevec
        {
            indexmap[s] = nodevec.size();
            nodevec.push_back(node(s));
        }
        for(auto&d:dvec) //save Directed_graph by node
            nodevec[indexmap[get<0>(d)]].putnext(indexmap[get<1>(d)], get<2>(d));


        ofstream out("pairs" + input_filename + ".adj", ios::trunc);
        for(int i = 0 ; i < nodevec.size(); i++) // write file
        {
            out << "[" << i + 1 << "] " << nodevec[i].id << ":" << "\n";
            #ifdef debug
            cout << "[" << i + 1 << "] " << nodevec[i].id << ":" << "\n";
            #endif
            int count = 0;
            for(auto&p:nodevec[i].nexts)
            {
                out << "   (" << ++count << ") " << nodevec[p.first].id << " " << p.second << "\n";
                #ifdef debug
                cout << "   (" << ++count << ") " << nodevec[p.first].id << " " << p.second << "\n";
                #endif
            }
        }
        out.close();
    }

    class connectclass
    {
public:
        string id;
        vector<string> idvec;
    };

    void connect()
    {
        ofstream             out("pairs" + input_filename + ".cnt", ios::trunc);
        vector<connectclass> totalvec;

        for(int i = 0 ; i < nodevec.size(); i++)
        {
            set<int>   nextset; // to check repeat
            queue<int> tempque; //nexvec->now checking, tempvec->next time checking

            nextset.insert(i);
            tempque.push(i); //start at index i

            while(tempque.size())
            {
                int n = tempque.front();
                tempque.pop();
                for(auto j:nodevec[n].nexts | ranges::views::transform([](pair<int, float> p){
                    return p.first;
                }) | ranges::views::filter([&nextset](int x){
                    return nextset.count(x) == 0;     //not exist in nextset
                }))
                {
                    nextset.insert(j);
                    tempque.push(j);
                }
            }

            nextset.erase(i);

            /*
             * out << "[" << i + 1 << "] " << nodevec[i].id << "(" << nextset.size() << ")" << ":" << "\n";
             #ifdef debug
             * cout << "[" << i + 1 << "] " << nodevec[i].id << "(" << nextset.size() << ")" << ":" << "\n";
             #endif
             * int count = 0;
             * for(auto&j:nextset)
             * {
             *  out << "   (" << ++count << ") " << nodevec[j].id << "\n";
             #ifdef debug
             *  cout << "   (" << ++count << ") " << nodevec[j].id << "\n";
             #endif
             * }
             */

            totalvec.push_back({});
            totalvec[totalvec.size() - 1].id = nodevec[i].id;
            for(auto&j:nextset)
                totalvec[totalvec.size() - 1].idvec.push_back(nodevec[j].id);
        }

        sort(totalvec.begin(), totalvec.end(), []( connectclass&a, connectclass&b){
            return a.idvec.size() > b.idvec.size();
        });

        int i = 0;

        for(auto&n:totalvec)
        {
            out << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #ifdef debug
            cout << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #endif
            int count = 0;
            for(auto&j:n.idvec)
            {
                out << "   (" << ++count << ") " << j << "\n";
                #ifdef debug
                cout << "   (" << ++count << ") " << j << "\n";
                #endif
            }
            i++;
        }
        out.close();
    }

    void inf()
    {
        float threshold;

        cout << "type threshold:";
        cin >> threshold;


        ofstream             out("pairs" + input_filename + ".inf", ios::trunc);
        vector<connectclass> totalvec;

        for(int i = 0 ; i < nodevec.size(); i++)
        {
            set<int>   nextset;   // to check repeat
            stack<int> nextstack; //nexvec->now checking, tempvec->next time checking

            nextset.insert(i);
            nextstack.push(i); //start at index i

            while(nextstack.size())
            {
                auto n = nextstack.top();
                nextstack.pop();
                for(auto&j:nodevec[n].nexts | ranges::views::filter([&nextset, &threshold](pair<int, float>&p){
                    return (nextset.count(get<0>(p)) == 0) && get<1>(p) >= threshold;     //not exist in nextset
                }))
                {
                    nextset.insert(get<0>(j));
                    nextstack.push(get<0>(j));
                }
            }

            nextset.erase(i);

            totalvec.push_back({});
            totalvec[totalvec.size() - 1].id = nodevec[i].id;
            for(auto&j:nextset)
                totalvec[totalvec.size() - 1].idvec.push_back(nodevec[j].id);
        }

        sort(totalvec.begin(), totalvec.end(), []( connectclass&a, connectclass&b){
            return a.idvec.size() > b.idvec.size();
        });

        int i = 0;

        for(auto&n:totalvec)
        {
            if(n.idvec.size() == 0)
                continue;
            out << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #ifdef debug
            cout << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #endif
            int count = 0;
            for(auto&j:n.idvec)
            {
                out << "   (" << ++count << ") " << j << "\n";
                #ifdef debug
                cout << "   (" << ++count << ") " << j << "\n";
                #endif
            }
            i++;
        }
        out.close();
    }

    void pro()
    {
        srand(time(NULL));
        ofstream             out("pairs" + input_filename + ".pro", ios::trunc);
        vector<connectclass> totalvec;

        for(int i = 0 ; i < nodevec.size(); i++)
        {
            set<int>   nextset;   // to check repeat
            stack<int> nextstack; //nexvec->now checking, tempvec->next time checking

            nextset.insert(i);
            nextstack.push(i); //start at index i

            while(nextstack.size())
            {
                auto n = nextstack.top();
                nextstack.pop();
                for(auto&j:nodevec[n].nexts | ranges::views::filter([&nextset](pair<int, float>&p){
                    return nextset.count(get<0>(p)) == 0 && get<1>(p) >= (float)(rand() % 10000) / 10000;      //not exist in nextset
                }))
                {
                    nextset.insert(get<0>(j));

                    nextstack.push(get<0>(j));
                }
            }

            nextset.erase(i);

            totalvec.push_back({});
            totalvec[totalvec.size() - 1].id = nodevec[i].id;
            for(auto&j:nextset)
                totalvec[totalvec.size() - 1].idvec.push_back(nodevec[j].id);
        }

        sort(totalvec.begin(), totalvec.end(), []( connectclass&a, connectclass&b){
            return a.idvec.size() > b.idvec.size();
        });

        int i = 0;

        for(auto&n:totalvec)
        {
            if(n.idvec.size() == 0)
                continue;
            out << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #ifdef debug
            cout << "[" << i + 1 << "] " << n.id << "(" << n.idvec.size() << ")" << ":" << "\n";
            #endif
            int count = 0;
            for(auto&j:n.idvec)
            {
                out << "   (" << ++count << ") " << j << "\n";
                #ifdef debug
                cout << "   (" << ++count << ") " << j << "\n";
                #endif
            }
            i++;
        }
        out.close();
    }
};

int main()
{
    int k = 100;

    while(k != 0)
    {
        cout << "\n";
        cout << "**  data operate system           **" << "\n"; //print menu
        cout << "* 0. Quit                          *" << "\n";
        cout << "* 1. adj list & connect counts     *" << "\n";
        cout << "* & inf & pro                     **" << "\n";
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

        if(k != 0 && k != 1 && k != 2)
        {
            cout << "\nCommand does not exist!\n";
            continue; //skip this loop
        }

        if(k == 1)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            dgraph dg;
            dg.read(locate);
            dg.adjlist();
            startTime();
            dg.connect();
            cout << "connect time=" << getTime() << "s" << "\n";
            dg.inf();
            dg.pro();
        }
    }
}
