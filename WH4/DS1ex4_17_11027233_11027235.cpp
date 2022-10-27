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
    int startcooking;
    int abort     = 0;
    int delay     = 0;
    int departure = 0;

    string stat = "";
    string cid;
};


ostream &operator<<(ostream&s, order&i)
{
    s << i.oid << '\t'
      << i.arrival << '\t'
      << i.duration << '\t'
      << i.timeout << '\t'
      << i.stat << '\t'
      << i.cid << '\t'
      << i.abort << '\t'
      << i.delay << '\t'
      << i.departure;
    return s;
}

vector<order> ordervec;
vector<order> cancelvec;
//vector<order> finishvec;
int nowtime = 0;
void vecclear()
{
    ordervec.clear();
    cancelvec.clear();
}

class chef
{
public:

    int cheftime = 0;
    string cid;
    int ordqueue[3];
    int index    = 0;
    int size     = 0;
    bool cooking = 0;
    int ing;
    chef(string s)
    {
        cid = s;
    }

    void push(int o)
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

    int top()
    {
        if(size < 1)
            cout << "ERROR!!!!!!!!!!!!";
        return ordqueue[(index) % 3];
    }

    void cook()
    {
        if(cooking)
            if(nowtime >= ordervec[ing].startcooking + ordervec[ing].duration)
            {
                //煮完
                if(ordervec[ing].startcooking + ordervec[ing].duration > ordervec[ing].timeout)
                {
                    ordervec[ing].stat  = "timeout";
                    ordervec[ing].cid   = cid;
                    ordervec[ing].delay = nowtime - ordervec[ing].arrival - ordervec[ing].duration;
                    ordervec[ing].departure = nowtime;
                    cancelvec.push_back(ordervec[ing]);
                }
                else
                {
                    ordervec[ing].stat = "success";
                    ordervec[ing].cid  = cid;
                }
                cooking = 0;
            }

        if(!cooking) //收新的
            while(size && !cooking)
            {
                ing = top();
                pop();
                if(nowtime >= ordervec[ing].arrival&&nowtime < ordervec[ing].timeout)
                {
                    cooking = 1;
                    ordervec[ing].startcooking = nowtime;
                }
                else //if(nowtime > ordervec[ing].timeout)
                {
                    ordervec[ing].stat  = "abort";
                    ordervec[ing].cid   = cid;
                    ordervec[ing].delay = nowtime - ordervec[ing].arrival ;
                    ordervec[ing].abort = nowtime;
                    cancelvec.push_back(ordervec[ing]);
                }
            }
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
            order o;
            int   time = clock();
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
            cout << "input:\n";
            cin >> name;
            ifstream in("sort" + name + ".txt");
            if(!in)
                cout << "error";
            getline(in, s);
            order o;
            while(in >> o)
                ordervec.push_back(o);

            int  totalsize = ordervec.size();
            chef chef1("chef1");
            int  i = 0;
            for(nowtime = 0 ; nowtime < 1000 ; nowtime++)
            {
                chef1.cook();
                while(i < ordervec.size() && ordervec[i].arrival == nowtime)
                {
                    if(!chef1.cooking)
                    {
                        chef1.push(i);
                        chef1.cook();
                    }

                    else if(chef1.size < 3)
                        chef1.push(i);
                    else
                    {
                        ordervec[i].stat  = "abort";
                        ordervec[i].cid   = "0";
                        ordervec[i].abort = ordervec[i].arrival;
                        cancelvec.push_back(ordervec[i]);
                    }
                    i++;
                }

                chef1.cook();
            }


            for(auto i:ordervec)
                cout << i << '\n';

            string ans   = "";
            int    delay = 0;

            ans += "[Abort List]\n\tOID\tDelay\tAbort\n";
            int ab = 0;
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "abort";
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t" + to_string(x.delay) + "\t" + to_string(x.abort) + "\n";
                delay += x.delay;
            }


            ans += "[Timeout List]\n\tOID\tDelay\tDeparture\n";
            int timo = 0;
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "timeout";
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t" + to_string(x.delay) + "\t" + to_string(x.departure) + "\n";
                delay += x.delay;
            }


            ans += "[Total Delay]\n" + to_string(delay) + " min.\n";
            ans += "[Failure Perecentage]\n" + to_string((float)100*(ab + timo) / totalsize).substr(0,to_string((float)100*(ab + timo) / totalsize).find(".")+3) + " %\n";

            cout << ans;
            ofstream out("one" + name + ".txt", ios::trunc);
            out<<ans;
            vecclear();
        }

        if(k == 3)
        {
            string name, s;
            cout << "input:\n";
            cin >> name;
            ifstream in("sort" + name + ".txt");
            if(!in)
                cout << "error";
            getline(in, s);
            order o;
            while(in >> o)
                ordervec.push_back(o);

            int  totalsize = ordervec.size();
            chef chef1("1");
            chef chef2("2");
            int  i = 0;
            for(nowtime = 0 ; nowtime < 1000 ; nowtime++)
            {
                chef1.cook();
                chef2.cook();
                while(i < ordervec.size() && ordervec[i].arrival == nowtime)
                {
                    if(!chef1.cooking)
                    {
                        chef1.push(i);
                        chef1.cook();
                    }
                    else if(!chef2.cooking)
                    {
                        chef2.push(i);
                        chef2.cook();
                    }
                    else if(chef1.size<chef2.size)
                        chef1.push(i);
                    else if(chef1.size>chef2.size)
                        chef2.push(i);
                    else if(chef1.size==chef2.size&&chef1.size < 3)
                        chef1.push(i);
                    else
                    {
                        ordervec[i].stat  = "abort";
                        ordervec[i].cid   = "0";
                        ordervec[i].abort = ordervec[i].arrival;
                        cancelvec.push_back(ordervec[i]);
                    }
                    i++;
                }

                chef1.cook();
                chef2.cook();
            }


            for(auto i:ordervec)
                cout << i << '\n';

            string ans   = "";
            int    delay = 0;

            ans += "[Abort List]\n\tOID\tCID\tDelay\tAbort\n";
            int ab = 0;
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "abort";
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t"+x.cid+ "\t" + to_string(x.delay) + "\t" + to_string(x.abort) + "\n";
                delay += x.delay;
            }


            ans += "[Timeout List]\n\tOID\tCID\tDelay\tDeparture\n";
            int timo = 0;
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "timeout";
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t"+x.cid+"\t" + to_string(x.delay) + "\t" + to_string(x.departure) + "\n";
                delay += x.delay;
            }


            ans += "[Total Delay]\n" + to_string(delay) + " min.\n";
            ans += "[Failure Perecentage]\n" + to_string((float)100*(ab + timo) / totalsize).substr(0,to_string((float)100*(ab + timo) / totalsize).find(".")+3) + " %\n";

            cout << ans;

            ofstream out("two" + name + ".txt", ios::trunc);
            out<<ans;
            vecclear();
        }
    }
}
