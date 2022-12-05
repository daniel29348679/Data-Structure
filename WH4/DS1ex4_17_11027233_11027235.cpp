//Dev-C++ -std=c++20
//曾品元11027233/江庭瑄11027235
//you can download compiler from
//https://github.com/niXman/mingw-builds-binaries/releases/download/12.2.0-rt_v10-rev1/x86_64-12.2.0-release-posix-seh-rt_v10-rev1.7z

#include <bits/stdc++.h>
using namespace std;

//#define error

template<typename v, typename T>
void shellsort(vector<v>&vec, T sortrule) //shell sort
{
    int range = 1;                        //spacing between to node

    while(range * 2 <= vec.size())
        range *= 2;

    for(; range > 0; range /= 2)
        for(int i = 0; i + range < vec.size(); i++)
            if(!sortrule(vec[i], vec[i + range]))
            {
                swap(vec[i], vec[i + range]);
                for(int j = i - range; j >= 0 && !sortrule(vec[j], vec[j + range]); j -= range)
                    swap(vec[j], vec[j + range]);
            }
}

class order
{
public:
    int oid;   //order id??????????????????????????????????????????????//
    int arrival;
    int duration;
    int timeout;
    int startcooking;    //time of start cooking
    int abort     = 0;
    int delay     = 0;
    int departure = 0; //????????????????????????????????????
    int finishout = -1;

    string stat = "";  //order status
    string cid;        //cook id??????????????????????????????????????????????
};


ostream &operator<<(ostream&s, order&i) //for debugger
{
    s << i.oid << '\t'
      << i.arrival << '\t'
      << i.duration << '\t'
      << i.timeout << '\t'
      << i.stat << '\t'
      << i.cid << '\t'
      << i.abort << '\t'
      << i.delay << '\t'
      << i.departure << '\t'
      << i.finishout;
    return s;
}

vector<order> ordervec;  //all order
vector<order> cancelvec; //canceled order

int nowtime = 0;         //now time that thef can know
void vecclear()          //vec clear
{
    ordervec.clear();
    cancelvec.clear();
}

int totaldealed;
class chef
{
public:
    string cid;       //cook id
    int ordqueue[3];  //queue
    int index    = 0; //queue front index
    int size     = 0; //queue size
    bool cooking = 0; //chef state
    int ing;          //cooking index
    int stopuntil = -1;
    chef()
    {
        return;
    }

    chef(string s)
    {
        cid = s;
    }

    void push(int o) //push to back of the queue
    {
        if(size > 2)
            cout << "ERROR!!!!!!!!!!!!";
        ordqueue[(index + size++) % 3] = o;
    }

    void pop()   //remove sth out of the queue
    {
        if(size < 1)
            cout << "ERROR!!!!!!!!!!!!";
        size--;  //size-=1
        index++; //move to next int
    }

    int top()
    {
        if(size < 1)
            cout << "ERROR!!!!!!!!!!!!";
        return ordqueue[(index) % 3]; //return front int
    }

    void cook() //廚師的工作:從queue拿東西來做, 判斷是否完成逾時((過了下單時間還是可拿來做((本來就是arrival到才可以進queue
    {
        if(cooking)
        {
            if(ordervec[ing].startcooking < ordervec[ing].timeout && ordervec[ing].timeout < ordervec[ing].startcooking + ordervec[ing].duration)
            {
                totaldealed++;
                ordervec[ing].stat      = "timeout";
                ordervec[ing].cid       = cid;
                ordervec[ing].delay     = ordervec[ing].startcooking - ordervec[ing].arrival;
                ordervec[ing].departure = ordervec[ing].arrival + ordervec[ing].duration + ordervec[ing].delay;
                cancelvec.push_back(ordervec[ing]);
                cooking   = 0;
                stopuntil = ordervec[ing].startcooking + ordervec[ing].duration;
            }
            else if(nowtime >= ordervec[ing].startcooking + ordervec[ing].duration)             //cook finish
            {
                if(ordervec[ing].startcooking + ordervec[ing].duration > ordervec[ing].timeout) //already timeout
                {
                    totaldealed++;
                    ordervec[ing].stat      = "timeout";
                    ordervec[ing].cid       = cid;
                    ordervec[ing].delay     = nowtime - ordervec[ing].arrival - ordervec[ing].duration;
                    ordervec[ing].departure = nowtime;
                    cancelvec.push_back(ordervec[ing]);
                }
                else //cook success
                {
                    totaldealed++;
                    ordervec[ing].stat = "success";
                    ordervec[ing].cid  = cid;
                }
                cooking = 0;
            }
        }
        if(nowtime < stopuntil)
            return;

        if(!cooking)                //enable to recive new order, find sth to do from the queue
            while(size && !cooking) //queue isn't empty but the chef isn't cooking
            {
                ing = top();
                pop();
                if(nowtime >= ordervec[ing].arrival && nowtime < ordervec[ing].timeout) //not exist time limit
                {
                    cooking = 1;
                    ordervec[ing].startcooking = nowtime;
                }
                else //if(nowtime > ordervec[ing].timeout)
                {
                    totaldealed++;
                    ordervec[ing].stat  = "abort";
                    ordervec[ing].cid   = cid;
                    ordervec[ing].delay = nowtime - ordervec[ing].arrival;
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
        cout << "* 1. sort                       *" << "\n";
        cout << "* 2. one chef                   *" << "\n";
        cout << "* 3. two chef                   *" << "\n";
        cout << "* 4. N chef                     *" << "\n";
        cout << "*********************************" << "\n";
        cout << "Input a choice(0, 1, 2, 3, 4): ";
        cin >> k;
        if(cin.fail())
        {
            cout << "\nCommand does not exist!\n";
            cin.clear();  //set cin stream to good(work) situation
            cin.ignore(); //skip next char in input stream

            k = -1;       //let k != 0 to keep loop continue
            continue;     //skip this loop
        }

        if(k != 0 && k != 1 && k != 2 && k != 3 && k != 4)
        {
            cout << "\nCommand does not exist!\n";
            continue; //skip this loop
        }

        if(k == 1)
        {
            string name, s;
            cout << "input:";
            cin >> name;
            ifstream in("input" + name + ".txt"); //open file stream
            while(!in)                            //open error
            {
                cout << "read file error, input:";
                cin >> name;
                in.open("input" + name + ".txt"); //retry
            }

            getline(in, s);                       //skip first line
            order o;
            int   time = clock();
            while(in >> o)
                ordervec.push_back(o); //read data

            cout << "Reading data:" << clock() - time << "ms\n";

            time = clock();
            shellsort(ordervec, [](order a, order b){
                return a.arrival != b.arrival?a.arrival < b.arrival:a.oid < b.oid;
            }); //sort
            cout << "Sorting data:" << clock() - time << "ms\n";

            time = clock();
            cout << "OID  Arrival Duration    Timeout\n";
            ofstream out("sort" + name + ".txt", ios::trunc);
            out << "OID  Arrival Duration    Timeout\n";
            for(auto i:ordervec)
            {
                //print and write the data into the file
                cout << i.oid << '\t' << i.arrival << '\t' << i.duration << '\t' << i.timeout << '\n';
                out << i.oid << '\t' << i.arrival << '\t' << i.duration << '\t' << i.timeout << '\n';
            }

            cout << "Writing data:" << clock() - time << "ms\n";
            vecclear(); //clear all vector
        }

        if(k == 2)
        {
            string name, s;
            cout << "input:\n";
            cin >> name;
            ifstream in("sort" + name + ".txt"); //open file stream
            while(!in)                           //open error
            {
                cout << "read file error, input:";
                cin >> name;
                in.open("sort" + name + ".txt");
            }
            getline(in, s); //skip first line
            order o;
            while(in >> o)
                ordervec.push_back(o);   //read data

            int totalsize = ordervec.size();
            totaldealed = 0;
            chef chef1("chef1");
            int  i = 0;
            for(nowtime = 0 ; totaldealed < totalsize ; nowtime++)           //keep working until all order finished
            {
                chef1.cook();                                                //cook and reflash                                                //cook
                while(i < ordervec.size() && ordervec[i].arrival == nowtime) //if has order to input
                {
                    if(!chef1.cooking)
                    {
                        chef1.push(i);
                        chef1.cook();
                    }

                    else if(chef1.size < 3)
                        chef1.push(i);
                    else //chef has no queue to store order
                    {
                        totaldealed++;
                        ordervec[i].stat  = "abort";
                        ordervec[i].cid   = "0";
                        ordervec[i].abort = ordervec[i].arrival;
                        cancelvec.push_back(ordervec[i]);
                    }
                    i++;
                }
            }

            #ifdef error //debugger?????????????????????????????????????????????????????????????
            for(auto i:ordervec)
                cout << i << '\n';
            #endif
            string ans   = ""; //strint to store output
            int    delay = 0;

            ans += "[Abort List]\n\tOID\tDelay\tAbort\n"; //title
            int ab = 0;                                   //abort count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "abort";                 //only "abort" can pass
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t" + to_string(x.delay) + "\t" + to_string(x.abort) + "\n";
                delay += x.delay;
            } //aborted order


            ans += "[Timeout List]\n\tOID\tDelay\tDeparture\n"; //title
            int timo = 0;                                       //timeout count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "timeout";                     //only "timeout" can pass
            }))
            {
                ans   += "[" + to_string(++timo) + "]\t" + to_string(x.oid) + "\t" + to_string(x.delay) + "\t" + to_string(x.departure) + "\n";
                delay += x.delay;
            } //timeout order


            ans += "[Total Delay]\n" + to_string(delay) + " min.\n";
            ans += "[Failure Perecentage]\n" + to_string((float)100 * (ab + timo) / totalsize).substr(0, to_string((float)100 * (ab + timo) / totalsize).find(".") + 3) + " %\n";

            //print and write the data into the file
            cout << ans;
            ofstream("one" + name + ".txt", ios::trunc) << ans;
            vecclear();  //clear all vector
        }

        if(k == 3)
        {
            string name, s;
            cout << "input:\n";
            cin >> name;
            ifstream in("sort" + name + ".txt");     //open file stream
            while(!in)                               //open error
            {
                cout << "read file error, input:";
                cin >> name;
                in.open("sort" + name + ".txt");
            }
            getline(in, s);                          //skip first line
            order o;
            while(in >> o)                           //read data
                ordervec.push_back(o);

            int totalsize = ordervec.size();
            totaldealed = 0;
            chef chef1("1");
            chef chef2("2");
            int  i = 0;
            for(nowtime = 0 ; totaldealed < totalsize ; nowtime++)           //keep working until all order finished
            {
                chef1.cook();                                                //cook and reflash
                chef2.cook();
                while(i < ordervec.size() && ordervec[i].arrival == nowtime) //if has order to input
                {
                    if(!chef1.cooking)                                       //not cooking get order first
                    {
                        chef1.push(i);
                        chef1.cook();
                    }
                    else if(!chef2.cooking)  //chef1 first and then chef2
                    {
                        chef2.push(i);
                        chef2.cook();
                    }
                    else if(chef1.size < chef2.size) //than who has less order get new order
                        chef1.push(i);
                    else if(chef1.size > chef2.size)
                        chef2.push(i);
                    else if(chef1.size == chef2.size && chef1.size < 3) //same chef1 get
                        chef1.push(i);
                    else //no one get ->"abort"
                    {
                        totaldealed++;
                        ordervec[i].stat  = "abort";
                        ordervec[i].cid   = "0";
                        ordervec[i].abort = ordervec[i].arrival;
                        cancelvec.push_back(ordervec[i]);
                    }
                    i++;
                }
            }


            #ifdef error
            for(auto i:ordervec)
                cout << i << '\n';
            #endif

            string ans   = "";  //strint to store output
            int    delay = 0;

            ans += "[Abort List]\n\tOID\tCID\tDelay\tAbort\n"; //title
            int ab = 0;                                        //abort count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "abort";                      //only "abort" can pass
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t" + x.cid + "\t" + to_string(x.delay) + "\t" + to_string(x.abort) + "\n";
                delay += x.delay;
            } //Abort order

            ans += "[Timeout List]\n\tOID\tCID\tDelay\tDeparture\n"; //title
            int timo = 0;                                            //timeout count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "timeout";                          //only "timeout" can pass
            }))
            {
                ans   += "[" + to_string(++timo) + "]\t" + to_string(x.oid) + "\t" + x.cid + "\t" + to_string(x.delay) + "\t" + to_string(x.departure) + "\n";
                delay += x.delay;
            }  //timeout order


            ans += "[Total Delay]\n" + to_string(delay) + " min.\n";
            ans += "[Failure Perecentage]\n" + to_string((float)100 * (ab + timo) / totalsize).substr(0, to_string((float)100 * (ab + timo) / totalsize).find(".") + 3) + " %\n";

            //print and write the data into the file
            cout << ans;
            ofstream("two" + name + ".txt", ios::trunc) << ans;
            vecclear(); //clear all vector
        }


        if(k == 4)
        {
            string name, s;
            cout << "input:\n";
            cin >> name;
            ifstream in("sort" + name + ".txt");     //open file stream
            while(!in)                               //open error
            {
                cout << "read file error, input:";
                cin >> name;
                in.open("sort" + name + ".txt");
            }
            getline(in, s);                          //skip first line
            order o;
            while(in >> o)                           //read data
                ordervec.push_back(o);

            int totalsize = ordervec.size();
            totaldealed = 0;

            cout << "number of chef:";
            int n;
            cin >> n;
            chef chefs[n];
            for(int i = 0; i < n; i++)
                chefs[i].cid = to_string(i + 1);
            int i = 0;
            for(nowtime = 0 ; totaldealed < totalsize ; nowtime++)           //keep working until all order finished
            {
                for(int j = 0; j < n; j++)                                   //cook and reflash
                    chefs[j].cook();
                while(i < ordervec.size() && ordervec[i].arrival == nowtime) //if has order to input
                {
                    bool success = 0;
                    for(int j = 0; j < n && !success; j++)
                        if(!chefs[j].cooking)
                        {
                            chefs[j].push(i);
                            chefs[j].cook();
                            success = 1;
                        }

                    for(int j = 0; j < n && !success; j++)
                        if(chefs[j].size == 0)
                        {
                            chefs[j].push(i);
                            chefs[j].cook();
                            success = 1;
                        }

                    for(int j = 0; j < n && !success; j++)
                        if(chefs[j].size == 1)
                        {
                            chefs[j].push(i);
                            chefs[j].cook();
                            success = 1;
                        }
                    for(int j = 0; j < n && !success; j++)
                        if(chefs[j].size == 2)
                        {
                            chefs[j].push(i);
                            chefs[j].cook();
                            success = 1;
                        }
                    if(!success) //no one get ->"abort"
                    {
                        totaldealed++;
                        ordervec[i].stat  = "abort";
                        ordervec[i].cid   = "0";
                        ordervec[i].abort = ordervec[i].arrival;
                        cancelvec.push_back(ordervec[i]);
                    }
                    i++;
                }
            }


            #ifdef error
            for(auto i:ordervec)
                cout << i << '\n';
            #endif

            string ans   = "";  //strint to store output
            int    delay = 0;

            ans += "[Abort List]\n\tOID\tCID\tDelay\tAbort\n"; //title
            int ab = 0;                                        //abort count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "abort";                      //only "abort" can pass
            }))
            {
                ans   += "[" + to_string(++ab) + "]\t" + to_string(x.oid) + "\t" + x.cid + "\t" + to_string(x.delay) + "\t" + to_string(x.abort) + "\n";
                delay += x.delay;
            } //Abort order


            ans += "[Timeout List]\n\tOID\tCID\tDelay\tDeparture\n"; //title
            int timo = 0;                                            //timeout count
            for(auto x:cancelvec | ranges::views::filter([](order o){
                return o.stat == "timeout";                          //only "timeout" can pass
            }))
            {
                ans   += "[" + to_string(++timo) + "]\t" + to_string(x.oid) + "\t" + x.cid + "\t" + to_string(x.delay) + "\t" + to_string(x.departure) + "\n";
                delay += x.delay;
            }  //timeout order


            ans += "[Total Delay]\n" + to_string(delay) + " min.\n";
            ans += "[Failure Perecentage]\n" + to_string((float)100 * (ab + timo) / totalsize).substr(0, to_string((float)100 * (ab + timo) / totalsize).find(".") + 3) + " %\n";

            //print and write the data into the file
            cout << ans;
            ofstream("more" + name + ".txt", ios::trunc) << ans;
            vecclear(); //clear all vector
        }
    }
}
