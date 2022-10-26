#include <bits/stdc++.h>
using namespace std;

template<typename v, typename T>
void shellsort(vector<v>&vec, T sortrule)
{
    int range = 1;

    while(range * 2 <= vec.size())
        range *= 2;

    for(; range > 0; range /= 2)
        for(int i = 0; i + range < vec.size(); i++)
            for(int j = 1; i + range * j < vec.size(); j++)
                if(!sortrule(vec[i], vec[i + range * j]))
                    swap(vec[i], vec[i + range * j]);
}

class order
{
public:
    int oid;
    int arrival;
    int duration;
    int timeout;
};


vector<order> ordervec;
vector<order> finishvec;
int           nowtime = 0;
void vecclear()
{
    ordervec.clear();
    finishvec.clear();
}

class chef
{
public:

    int cheftime       = 0;
    string cid;
    order ordqueue[3];
    int index = 0;
    int size  = 0;
    chef(string s)
    {
        cid = s;
    }

    void push(order o)
    {
        if(size > 2)
            cout << "ERROR!!!!!!!!!!!!";
        size++;
        ordqueue[(index + size - 1) % 3] = o;
    }

    void pop()
    {
        if(size < 1)
            cout << "ERROR!!!!!!!!!!!!";
        size--;
        index++;
    }

    order top(order)
    {
        if(size < 1)
            cout << "ERROR!!!!!!!!!!!!";
        return ordqueue[(index) % 3];
    }

    void cook()
    {
    }

    int finishtime()
    {
        return 0;
    }
};

ifstream &operator>>(ifstream&s, order&o)
{
    string str;

    getline(s, str);           //read one line
    if(str.size() < 3)         //too short = error
    {
        s.clear(ios::failbit); //set failbit to true
        return s;
    }
    int    i = 0;
    string num;

    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    o.oid = stoi(num);
    num   = "";                                                //convert string to int

    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    o.arrival = stoi(num);
    num       = "";

    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    o.duration = stoi(num);
    num        = "";

    for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
        num += str[i];
    i++;                                                       //skip '\t'
    num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
    o.timeout = stoi(num);
    return s;
}

int main()
{
    int k = -1;

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
            string name, s;
            cout << "input:";
            cin >> name;
            ifstream in("input" + name + ".txt");
            if(!in)
                cout << "error";
            getline(in, s);
            order         o;
            int           time = clock();
            while(in >> o)
                ordervec.push_back(o);
            cout << "Sorting data:" << clock() - time << "ms\n";
            time = clock();
            shellsort(ordervec, [](order a, order b){
                return a.arrival != b.arrival?a.arrival < b.arrival:a.oid < b.oid;
            });
            cout << "Reading data:" << clock() - time << "ms\n";
            time = clock();
            cout << "OID  Arrival Duration    Timeout\n";
            ofstream out("sort" + name + ".txt", ios::trunc);
            out << "OID  Arrival Duration    Timeout\n";
            for(auto i:ordervec)
            {
                cout << i.oid << '\t' << i.arrival << '\t' << i.duration << '\t' << i.timeout << '\n';
                out << i.oid << '\t' << i.arrival << '\t' << i.duration << '\t' << i.timeout << '\n';
            }

            cout << "Writing data:" << clock() - time << "ms\n";
            time = clock();
            vecclear();
        }

        if(k == 2)
        {
            string name, s;
            cout << "input:";
            cin >> name;
            ifstream in("sort" + name + ".txt");
            if(!in)
                cout << "error";
            getline(in, s);
            order o;
            int   time = clock();
            while(in >> o)
                ordervec.push_back(o);
        }
    }
}
