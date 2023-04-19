//11027233 曾品元,11027263 黃致穎
//IDE Dev-C++, vscode, sublime, codeblock or even notepad
//Compiler gcc mingw https://github.com/Vuniverse0/mingwInstaller/releases/download/1.2.0/mingwInstaller.exe
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//https://blog.csdn.net/qq_50285142/article/details/122930647
#include <bits/stdc++.h>
using namespace std;
//#define debug    ;

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

    void putnext(int& i, float& j)
    {
        nexts.push_back(make_pair(i, j));
    }
};


class dgraph
{
public:
    string input_filename;

    set<string> idset;
    vector<tuple<string, string, float> > dvec;
    vector<node> nodevec;
    map<string, int> indexmap;
    void read(string locate)     //constructer  read the file
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
                    cout << p.putid << " " << p.getid << " " << p.weight << endl;
                    #endif
                }

                input.close();
                return;
            }
            input.close();
        }
    }

    void adjlist()
    {
        indexmap.clear();
        nodevec.clear();
        for(auto&s:idset)
        {
            indexmap[s] = nodevec.size();
            nodevec.push_back(node(s));
        }
        for(auto&d:dvec)
            nodevec[indexmap[get<0>(d)]].putnext(indexmap[get<1>(d)], get<2>(d));


        ofstream out("pairs" + input_filename + ".adj", ios::trunc);
        for(int i = 0 ; i < nodevec.size(); i++)
        {
            out << "[" << i + 1 << "] " << nodevec[i].id << ":" << endl;
            #ifdef debug
            cout << "[" << i + 1 << "] " << nodevec[i].id << ":" << endl;
            #endif
            int count = 0;
            for(auto&p:nodevec[i].nexts)
            {
                out << "   (" << ++count << ") " << nodevec[p.first].id << " " << p.second << endl;
                #ifdef debug
                cout << "   (" << ++count << ") " << nodevec[p.first].id << " " << p.second << endl;
                #endif
            }
        }
        out.close();
    }

    void connect()
    {
        ofstream out("pairs" + input_filename + ".cnt", ios::trunc);

        for(int i = 0 ; i < nodevec.size(); i++)
        {
            set<int>    nextset;
            vector<int> tempvev, nexvec;

            nextset.insert(i);
            nexvec.push_back(i);

            while(nexvec.size())
            {
                for(auto&n:nexvec)
                    for(auto j:nodevec[n].nexts | ranges::views::transform([](pair<int, float> p){
                        return p.first;
                    }) | ranges::views::filter([&nextset](int x){
                        return nextset.count(x) == 0;
                    }))
                    {
                        nextset.insert(j);
                        tempvev.push_back(j);
                    }
                nexvec = tempvev;
                tempvev.clear();
            }

            nextset.erase(i);

            out << "[" << i + 1 << "] " << nodevec[i].id << "(" << nextset.size() << ")" << ":" << endl;
            #ifdef debug
            cout << "[" << i + 1 << "] " << nodevec[i].id << "(" << nextset.size() << ")" << ":" << endl;
            #endif
            int count = 0;
            for(auto&j:nextset)
            {
                out << "   (" << ++count << ") " << nodevec[j].id << endl;
                #ifdef debug
                cout << "   (" << ++count << ") " << nodevec[j].id << endl;
                #endif
            }
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
        cout << "* 1. adj list & connect counts    **" << "\n";
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
            dg.connect();
            cout << "success!" << endl;
        }
    }
}
