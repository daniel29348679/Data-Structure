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
class person;

class person_bin
{
public:
    char sid[10]           = "";
    char sname[10]         = "";
    unsigned char score[6] = {0, 0, 0, 0, 0, 0};
    float mean             = 0.0;

    void operator=(const person&p);

    //ifstream, istream, ofstream, ostream are friends of person_bin's, so they can acess private members in person_bin
    friend ifstream &operator>>(ifstream&s, person_bin&p);
    friend ofstream &operator<<(ofstream&s, person_bin&p);
};

class person
{
public:
    string sid;
    string sname;
    int score[6] = {0, 0, 0, 0, 0, 0};
    float mean   = 0.0;

    void operator=(const person_bin&p) //person_bin->person
    {
        sid   = p.sid;                 //sid
        sname = p.sname;               //sname
        for(int i = 0 ; i < 6 ; i++)   //score
            score[i] = p.score[i];
        mean = p.mean;                 //mean;
    }

    //ifstream, istream, ofstream, ostream are friends of person_bin's, so they can acess private members in person_bin
    friend ifstream &operator>>(ifstream&s, person&p);
    friend ostream &operator<<(ostream&s, person&p);
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
    void read(string locate)     //constructer  read the file
    {
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
                while(input >> pb)
                {
                    p = pb;
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
                ofstream   output("input" + locate + ".bin", ios::binary);
                person_bin pb;
                person     p;

                while(input >> p)
                {
                    person_vec.push_back(p);
                    pb = p;
                    output << pb;

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
        vector<bool> bvec(100000, 1);

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
        vector<bool> bvec(100000, 1);

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
        int haxnum = gethaxnum();

        vector<int> haxvec;
        vector<tuple<string, string, float, int> > hashtable(haxnum, {"", "", 0.0, -1});


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
            for(int j = 0; j < haxnum; j++)
            {
                int a = h + j * j;
                a %= haxnum;
                if(get<3>(hashtable[a]) == -1)
                {
                    hashtable[a] = make_tuple(person_vec[i].sid, person_vec[i].sname, person_vec[i].mean, h);
                    break;
                }
            }
        }

        ofstream out("quadratic" + input_filename + ".txt", ios::trunc);

        for(int i = 0; i < hashtable.size(); i++)
        {
            out << "Hash code:" << i << "\t";
            if(get<3>(hashtable[i]) != -1)
                out << get<0>(hashtable[i]) << '\t' << get<1>(hashtable[i]) << '\t' << get<2>(hashtable[i]) << '\t' << get<3>(hashtable[i]);
            else
                out << "NULL!";
            out << endl;
        }
        out.close();


        //find average
        int count = 0;

        for(int num = 0; num < hashtable.size(); num++) //not exist
        {
            if(get<3>(hashtable[num]) == -1)
                continue;
            for(int j = 1; j < haxnum; j++)
            {
                int a = num + j * j;
                a %= haxnum;
                count++;
                if(get<3>(hashtable[a]) == -1)
                    break;
            }
        }

        cout << "not exist:" << setprecision(4) << (float)count / hashtable.size() << endl;


        count = 0;
        for(int i = 0 ; i < person_vec.size(); i++)  //exist
        {
            int h = haxvec[i];
            count++;
            if(get<0>(hashtable[h]) == person_vec[i].sid)
                continue;
            for(int j = 1; j < haxnum; j++)
            {
                int a = h + j * j;
                a %= haxnum;
                count++;
                if(get<0>(hashtable[a]) == person_vec[i].sid)
                    break;
            }
        }
        cout << "exist:" << (float)count / person_vec.size() << endl;
    }

    void doublehash()
    {
        int         haxnum = gethaxnum();
        int         step = getstep();
        vector<int> haxvec, stepvec;
        vector<tuple<string, string, float, int> > hashtable(haxnum, {"", "", 0.0, -1});

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
                if(get<3>(hashtable[a]) == -1)
                {
                    hashtable[a] = make_tuple(person_vec[i].sid, person_vec[i].sname, person_vec[i].mean, h);
                    break;
                }
                a += st;
                a %= haxnum;
            }
        }

        ofstream out("double" + input_filename + ".txt", ios::trunc);

        for(int i = 0; i < hashtable.size(); i++)
        {
            out << "Hash code:" << i << "\t";
            if(get<3>(hashtable[i]) != -1)
                out << get<0>(hashtable[i]) << '\t' << get<1>(hashtable[i]) << '\t' << get<2>(hashtable[i]) << '\t' << get<3>(hashtable[i]);
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
                if(get<0>(hashtable[a]) == person_vec[i].sid)
                    break;
            }
        }
        cout << "exist:" << (float)count / person_vec.size() << endl;
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
        cout << "* 2. Double hashing                **" << "\n";

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

            sheet sh;
            sh.read(locate);
            sh.quadratic();
        }
        if(k == 2)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet sh;
            sh.read(locate);
            sh.doublehash();
        }
    }
}
