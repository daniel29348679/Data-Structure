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

#define INF    0x3f3f3f3f


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
    vector<pair<int, float> > nexts;

    node()
    {
    }

    void putnext(const int& i, const float& j) // to push back nexts
    {
        nexts.push_back(make_pair(i, j));
    }
};


class dgraph
{
public:
    string input_filename;
    float threshold;

    set<string> idset;                          //all id here
    vector<tuple<string, string, float> > dvec; //Directed_graph
    vector<node> nodevec;                       //node
    map<string, int> indexmap;                  //id to index
    vector<string> idvec;


    void readandrewrite(string locate)          //constructer  read the file
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
                break;
            }
            input.close();
        }

        for(const auto&id:idset)
        {
            indexmap[id] = idvec.size();
            idvec.push_back(id);
            nodevec.push_back({});
        }

        cout << "type threshold:";
        cin >> threshold;

        for(const auto&d:dvec | views::filter([&](const tuple<string, string, float> t){
            return get<2>(t) <= threshold;
        }))
        {
            nodevec[indexmap[get<0>(d)]].putnext(indexmap[get<1>(d)], get<2>(d));
            nodevec[indexmap[get<1>(d)]].putnext(indexmap[get<0>(d)], get<2>(d));
        }

        ofstream out("pairs" + input_filename + "_" + to_string(threshold) + ".adj", ios::trunc);

        int un = 0;
        for(int i = 0 ; i < nodevec.size() ; i++)
            if(nodevec[i].nexts.size() == 0)
                un++;


        out << nodevec.size() - un << "\n";
        for(int i = 0 ; i < nodevec.size() ; i++)
        {
            if(nodevec[i].nexts.size() == 0)
                continue;
            sort(nodevec[i].nexts.begin(), nodevec[i].nexts.end(), [](const tuple<int, float>&t0, const tuple<int, float>&t1)
            {
                return get<0>(t0) < get<0>(t1);
            });

            #ifdef debug
            cout << "id:" << idvec[i] << "\n";
            for(const auto&p:nodevec[i].nexts)
                cout << "\t" << idvec[get<0>(p)] << "," << get<1>(p) << "\n";
            #endif

            out << idvec[i] << "\n";
            out << nodevec[i].nexts.size() << "\n";
            for(const auto&p:nodevec[i].nexts)
                out << idvec[get<0>(p)] << "\n" << get<1>(p) << "\n";
        }
        out.close();
    }

    void read()
    {
        ifstream input("!!!!!!");

        while(1)
        {
            cout << "type name(e.x. 501):";
            cin >> input_filename;
            cout << "type threshold(e.x. 0.5):";
            cin >> threshold;
            input.open("pairs" + input_filename + "_" + to_string(threshold) + ".adj"); //try input bin
            if(input)
                break;
            cout << (string)"failed to open " + "pairs" + input_filename + "_" + to_string(threshold) + ".adj\n";
        }


        int    total;
        string id;

        input >> total;
        #ifdef debug
        cout << "total:" << total << "\n";
        #endif
        while(total--)
        {
            input >> id;
            indexmap[id] = idvec.size();
            idvec.push_back(id);
            nodevec.push_back({});

            #ifdef debug
            cout << "id:" << id << "\n";
            #endif

            int k;
            input >> k;
            while(k--)
                input >> id >> id;
        }


        input.close();
        input.open("pairs" + input_filename + "_" + to_string(threshold) + ".adj");

        input >> total;
        while(total--)
        {
            input >> id;

            int k;
            input >> k;
            string id2;
            float  thr;
            while(k--)
            {
                input >> id2 >> thr;
                nodevec[indexmap[id]].putnext(indexmap[id2], thr);
            }
        }

        #ifdef debug
        for(int i = 0 ; i < nodevec.size() ; i++)
        {
            cout << "id:" << idvec[i] << "\n";
            for(const auto&p:nodevec[i].nexts)
                cout << "\t" << idvec[get<0>(p)] << "," << get<1>(p) << "\n";
        }
        #endif
    }

    void ccandsp()
    {
        vector<bool>         alreadyvec(nodevec.size(), 0);
        vector<vector<int> > ccvec;


        for(int i = 0 ; i < alreadyvec.size(); i++)
        {
            if(alreadyvec[i])
                continue;
            vector<int> newcc;
            stack<int>  dfsstack;

            dfsstack.push(i);

            while(dfsstack.size())
            {
                int index = dfsstack.top();
                dfsstack.pop();
                if(alreadyvec[index])
                    continue;
                alreadyvec[index] = 1;
                newcc.push_back(index);

                for(const auto&p:nodevec[index].nexts)
                    dfsstack.push(get<0>(p));
            }
            sort(newcc.begin(), newcc.end());
            ccvec.push_back(newcc);
        }

        sort(ccvec.begin(), ccvec.end(), [](const vector<int>&v0, const vector<int>&v1)
        {
            return v0.size() > v1.size();
        });

        ofstream out("pairs" + input_filename + "_" + to_string(threshold) + ".cc", ios::trunc);

        for(auto& v : ccvec)
        {
            cout << "cc size:" << v.size() << "\n";
            out << "cc size:" << v.size() << "\n";
            for(auto& i : v)
            {
                cout << idvec[i] << "\n";
                out << idvec[i] << "\n";
            }
        }
        out.close();


        out.open("pairs" + input_filename + "_" + to_string(threshold) + ".ds", ios::trunc);

        string idtosearch;

        while(1)
        {
            cout << "type id to search(-1 for quit): ";
            cin >> idtosearch;
            if(idtosearch == "-1")
                break;

            vector<float> distancesvec(nodevec.size(), 999999);

            distancesvec[indexmap[idtosearch]] = 0;
            vector<int> nowvec, tempvec;
            nowvec.push_back(indexmap[idtosearch]);
            while(nowvec.size())
            {
                for(const auto& i :nowvec)
                    for(const auto&j:nodevec[i].nexts)
                        if(distancesvec[i] + get<1>(j) < distancesvec[get<0>(j)])
                        {
                            distancesvec[get<0>(j)] = distancesvec[i] + get<1>(j);
                            tempvec.push_back(get<0>(j));
                        }

                nowvec = tempvec;
                tempvec.clear();
            }

            cout << "start from:" << idtosearch << "\n";
            out << "start from:" << idtosearch << "\n";
            for(int i = 0 ; i < distancesvec.size() ; i++)
                if(distancesvec[i] < 999999)
                {
                    cout << "\t" << idvec[i] << ", dis= " << distancesvec[i] << "\n";
                    out << "\t" << idvec[i] << ", dis= " << distancesvec[i] << "\n";
                }
        }
    }

    void mintree()
    {
        ifstream input("pairs" + input_filename + "_" + to_string(threshold) + ".cc");

        if(!input)
        {
            cout << (string)"failed to open " + "pairs" + input_filename + "_" + to_string(threshold) + ".adj\n";
            return;
        }


        string idtosearch;

        char ch;

        while(input >> ch >> ch >> ch >> ch >> ch >> ch >> ch)
        {
            int t;
            input >> t;
            cout << "cc size=" << t << '\n';
            while(t--)
            {
                input >> idtosearch;
                //cout << idtosearch << '\n';
            }


            vector<float> distancesvec(nodevec.size(), 999999);
            vector<float> weightvec(nodevec.size(), 0);

            distancesvec[indexmap[idtosearch]] = 0;
            vector<int> nowvec, tempvec;
            nowvec.push_back(indexmap[idtosearch]);
            while(nowvec.size())
            {
                for(const auto& i :nowvec)
                    for(const auto&j:nodevec[i].nexts)
                        if(distancesvec[i] + get<1>(j) < distancesvec[get<0>(j)])
                        {
                            distancesvec[get<0>(j)] = distancesvec[i] + get<1>(j);
                            weightvec[get<0>(j)]    = get<1>(j);
                            tempvec.push_back(get<0>(j));
                        }

                nowvec = tempvec;
                tempvec.clear();
            }

            float total = 0;
            for_each(weightvec.begin(), weightvec.end(), [&](float& n) {
                total += n;
            });
            cout << "total=" << total << '\n';
        }
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
        cout << "* 1. cc and sp                     *" << "\n";
        cout << "* 99. mission 0                   **" << "\n";
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

        if(k != 0 && k != 1 && k != 2 && k != 99)
        {
            cout << "\nCommand does not exist!\n";
            continue; //skip this loop
        }

        if(k == 99)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            dgraph dg;
            dg.readandrewrite(locate);
        }

        if(k == 1)
        {
            dgraph dg;
            dg.read();
            dg.ccandsp();
        }
        if(k == 2)
        {
            dgraph dg;
            dg.read();
            dg.mintree();
        }
    }
}
