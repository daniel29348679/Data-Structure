//11027233 曾品元,11027263 黃致穎
//IDE Dev-C++, vscode, sublime, codeblock or even notepad
//Compiler gcc mingw https://github.com/Vuniverse0/mingwInstaller/releases/download/1.2.0/mingwInstaller.exe
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//https://blog.csdn.net/qq_50285142/article/details/122930647
#include <bits/stdc++.h>
using namespace std;

//#define debug    ; use if want to debug
struct person;
struct person_bin
{
    char          sid[10]   = "";
    char          sname[10] = "";
    unsigned char score[6]  = {0, 0, 0, 0, 0, 0};
    float         mean      = 0.0;
    void operator=(const person&p);
};

struct person
{
public:
    string sid;
    string sname;
    int    score[6] = {0, 0, 0, 0, 0, 0};
    float  mean     = 0.0;

    void operator=(const person_bin&p) //person_bin->person
    {
        sid   = p.sid;                 //sid
        sname = p.sname;               //sname
        for(int i = 0 ; i < 6 ; i++)   //score
            score[i] = p.score[i];
        mean = p.mean;                 //mean;
    }
};


void person_bin::operator=(const person&p) //define here because must after person defined
{
    strcpy(sid, p.sid.c_str());            //sid
    strcpy(sname, p.sname.c_str());        //sname
    for(int i = 0 ; i < 6 ; i++)           //score
        score[i] = p.score[i];
    mean = p.mean;                         //mean;
}

ifstream &operator>>(ifstream&s, person&p)
{
    s >> p.sid;

    s >> p.sname;
    if(p.sname.size() < 3)
    {
        string str;
        s >> str;
        p.sname += str;
    }
    s >> p.score[0] >> p.score[1] >> p.score[2] >> p.score[3] >> p.score[4] >> p.score[5] >> p.mean;
    return s;
}

ostream &operator<<(ostream&s, person&p)
{
    s << p.sid << '\t' << p.sname << '\t' << p.score[0] << '\t' << p.score[1] << '\t' << p.score[2] << '\t' << p.score[3] << '\t' << p.score[4] << '\t' << p.score[5] << '\t' << p.mean;
    return s;
}

ifstream &operator>>(ifstream&s, person_bin&p) //person_bin output file stream Overload
{
    if(!s.read((char*)&p, sizeof(p)))          //copy block
        s.clear(ios::failbit);                 //set failbit if read failed
    return s;
}

ofstream &operator<<(ofstream&s, person_bin&p) //person_bin output file stream Overload
{
    s.write((char*)&p, sizeof(p));             //copy block
    return s;
}

class sheet
{
public:
    int numofdata = 0;
    vector<person> person_vec;
    string input_filename;
    vector<tuple<string, string, float, int> > hashtable_qua;
    vector<tuple<string, string, float, int> > hashtable_double;

    bool run_1 = 0;
    bool run_2 = 0;
    int step;

    void read(string locate)     //constructer  read the file
    {
        run_1 = 0;
        run_2 = 0;
        ifstream input;

        person_vec.clear();

        //再一律加上input和.txt
        //input.open("input" + locate + ".bin", ios::binary);     //open file "inputxxx.txt"

        for(;; cout << "\nCan't open file, typenew: ", cin >> locate)
        {
            input_filename = locate;
            input.open("input" + locate + ".bin", ios::binary); //try input bin
            if(input)
            {
                person_bin pb;
                person     p;
                while(input >> pb) //read bin
                {
                    p = pb;        //bin to normal
                    person_vec.push_back(p);

                    #ifdef debug
                    cout << "binary->" << p << endl;
                    #endif
                }
                input.close();
                return;
            }
            input.open("input" + locate + ".txt");//try input txt
            if(input)
            {
                ofstream   output("input" + locate + ".bin", ios::binary);//save file
                person_bin pb;
                person     p;

                while(input >> p) //read normal
                {
                    person_vec.push_back(p);
                    pb = p;       //normal to bin
                    output << pb; //save in bin

                    #ifdef debug
                    cout << p << endl;
                    #endif
                }
                output.close();
                input.close();
                return;
            }
            input.close();
        }
    }

    int gethaxnum()
    {
        vector<bool> bvec(100000, 1); //set if Prime number

        for(int i = 2; i < 100000; i++)
        {
            if(bvec[i] && i > person_vec.size() * 1.2)
                return i;

            if(bvec[i])
                for(int j = i * 2; j < 100000; j += i)
                    bvec[j] = 0;
        }
        return -1;
    }

    int getstep()
    {
        vector<bool> bvec(100000, 1); //set if Prime number

        for(int i = 2; i < 100000; i++)
        {
            if(bvec[i] && i > person_vec.size() / 3)
                return i;

            if(bvec[i])
                for(int j = i * 2; j < 100000; j += i)
                    bvec[j] = 0;
        }
        return -1;
    }

    void quadratic()
    {
        run_1 = 1;
        int haxnum = gethaxnum();

        vector<int> haxvec;

        //vector<tuple<string, string, float, int> > hashtable_qua(haxnum, {"", "", 0.0, -1});
        hashtable_qua.clear();
        for(int i = 0 ; i < haxnum; i++)
            hashtable_qua.push_back({"", "", 0.0, -1});


        for(int i = 0 ; i < person_vec.size(); i++)
        {
            char c[10];
            strcpy(c, person_vec[i].sid.c_str());
            int h = 1; //hash number
            for(int j = 0; j < 10 && c[j] != '\0'; j++)
            {
                h *= c[j];
                h %= haxnum;
            }
            haxvec.push_back(h);

            //put into hashtable
            for(int j = 0; j < haxnum; j++)
            {
                int a = h + j * j;
                a %= haxnum;
                if(get<3>(hashtable_qua[a]) == -1)
                {
                    hashtable_qua[a] = make_tuple(person_vec[i].sid, person_vec[i].sname, person_vec[i].mean, h);
                    break;
                }
            }
        }

        ofstream out("quadratic" + input_filename + ".txt", ios::trunc); //output file

        for(int i = 0; i < hashtable_qua.size(); i++)
        {
            out << "Hash code:" << i << "\t";
            if(get<3>(hashtable_qua[i]) != -1)
                out << get<0>(hashtable_qua[i]) << '\t' << get<1>(hashtable_qua[i]) << '\t' << get<2>(hashtable_qua[i]) << '\t' << get<3>(hashtable_qua[i]);
            else
                out << "NULL!";
            out << endl;
        }
        out.close();


        //find average
        int count = 0;

        for(int num = 0; num < hashtable_qua.size(); num++) //not exist
        {
            if(get<3>(hashtable_qua[num]) == -1)
                continue;
            for(int j = 1; j < haxnum; j++)
            {
                int a = num + j * j;
                a %= haxnum;
                count++;
                if(get<3>(hashtable_qua[a]) == -1)
                    break;
            }
        }

        cout << "not exist:" << setprecision(4) << (float)count / hashtable_qua.size() << endl;


        count = 0;
        for(int i = 0 ; i < person_vec.size(); i++)  //exist
        {
            int h = haxvec[i];
            count++;
            if(get<0>(hashtable_qua[h]) == person_vec[i].sid)
                continue;
            for(int j = 1; j < haxnum; j++)
            {
                int a = h + j * j;
                a %= haxnum;
                count++;
                if(get<0>(hashtable_qua[a]) == person_vec[i].sid)
                    break;
            }
        }
        cout << "exist:" << (float)count / person_vec.size() << endl;
    }

    void doublehash()
    {
        if(!run_1)
        {
            cout << "run 1 needed!!!!\n";
            return;
        }
        run_2 = 1;
        int haxnum = gethaxnum();
        step = getstep();
        vector<int> haxvec, stepvec;

        //vector<tuple<string, string, float, int> > hashtable_double(haxnum, {"", "", 0.0, -1});
        hashtable_double.clear();
        for(int i = 0 ; i < haxnum; i++)
            hashtable_double.push_back({"", "", 0.0, -1});
        for(int i = 0 ; i < person_vec.size(); i++)
        {
            char c[10];
            strcpy(c, person_vec[i].sid.c_str());
            int h = 1;
            for(int j = 0; j < 10 && c[j] != '\0'; j++)
            {
                h *= c[j];
                h %= haxnum;
            }
            haxvec.push_back(h);

            //put into hashtable
            int hh = 1;
            for(int j = 0; j < 10 && c[j] != '\0'; j++)
            {
                hh *= c[j];
                hh %= step;
            }
            int st = step - (hh % step);
            stepvec.push_back(st);
            for(int a = h; ;)
            {
                if(get<3>(hashtable_double[a]) == -1)
                {
                    hashtable_double[a] = make_tuple(person_vec[i].sid, person_vec[i].sname, person_vec[i].mean, h);
                    break;
                }
                a += st;
                a %= haxnum;
            }
        }

        ofstream out("double" + input_filename + ".txt", ios::trunc); //output file

        for(int i = 0; i < hashtable_double.size(); i++)
        {
            out << "Hash code:" << i << "\t";
            if(get<3>(hashtable_double[i]) != -1)
                out << get<0>(hashtable_double[i]) << '\t' << get<1>(hashtable_double[i]) << '\t' << get<2>(hashtable_double[i]) << '\t' << get<3>(hashtable_double[i]);
            else
                out << "NULL!";
            out << endl;
        }
        out.close();

        //find average
        int count = 0;

        for(int i = 0 ; i < person_vec.size(); i++)  //exist
        {
            int h  = haxvec[i];
            int st = stepvec[i];
            for(int a = h; ; a += st, a %= haxnum)
            {
                count++;
                if(get<0>(hashtable_double[a]) == person_vec[i].sid)
                    break;
            }
        }
        cout << "exist:" << (float)count / person_vec.size() << endl;
    }

    void search(string id)
    {
        if(!(run_1 && run_2))
        {
            cout << "run 1&2 needed!!\n";
            return;
        }
        int h = 1;

        for(auto&c:id)
        {
            h *= c;
            h %= hashtable_double.size();
        }

        int s = 1;

        for(auto&c:id)
        {
            s *= c;
            s %= step;
        }
        s = step - s;

        for(int i = 0 ;; i++)
        {
            if(get<0>(hashtable_qua[(h + i * i) % hashtable_qua.size()]) == id)
            {
                cout << "find {" << get<0>(hashtable_qua[(h + i * i) % hashtable_qua.size()]) << "," << get<1>(hashtable_qua[(h + i * i) % hashtable_qua.size()]) << "," << get<2>(hashtable_qua[(h + i * i) % hashtable_qua.size()]) << "} success in qua, " << i + 1 << "times\n";
                break;
            }
            if(get<3>(hashtable_qua[(h + i * i) % hashtable_qua.size()]) == -1)
            {
                cout << "find " << id << " unsuccess in qua, " << i + 1 << "times\n";
                break;
            }
        }
        for(int i = 0;; h += s, h %= hashtable_double.size(), i++)
        {
            if(get<0>(hashtable_double[h]) == id)
            {
                cout << "find {" << get<0>(hashtable_double[h]) << "," << get<1>(hashtable_double[h]) << "," << get<2>(hashtable_double[h]) << "} success in double, " << i + 1 << "times\n";
                break;
            }
            if(get<3>(hashtable_double[h]) == -1)
            {
                cout << "find " << id << " unsuccess in double, " << i + 1 << "times\n";
                break;
            }
        }
    }
};

int main()
{
    int   k = 100;
    sheet sh;

    cout << fixed << setprecision(4);

    while(k != 0)
    {
        cout << "\n";
        cout << "**  data operate system           **" << "\n"; //print menu
        cout << "* 0. Quit                          *" << "\n";
        cout << "* 1. Quadratic                     *" << "\n";
        cout << "* 2. Double hashing                *" << "\n";
        cout << "* 3. Search                       **" << "\n";

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
            sh.read(locate);
            sh.quadratic();
        }
        if(k == 2)
            sh.doublehash();
        if(k == 3)
        {
            string id;
            cout << "id: ";
            cin >> id;
            sh.search(id);
        }
    }
}
