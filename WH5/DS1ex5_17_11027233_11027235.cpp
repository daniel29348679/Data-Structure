//Dev-C++ -std=c++20
//曾品元11027233/江庭瑄11027235

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

#define MAXthread    3
mutex mu, mus[MAXthread], mustar[MAXthread];


LARGE_INTEGER nFreq;
LARGE_INTEGER nBeginTime;
LARGE_INTEGER nEndTime;

void startTime()
{
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);
}

double getTime()
{
    QueryPerformanceCounter(&nEndTime);
    return (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
}

class department
{
public:
    string data[8];          //學校代碼 學校名稱 科系代碼 科系名稱 日間／進修別  等級別  縣市名稱 體系別
    int numofstudent;        //學生數
    int numofteacher;        //教師數
    int numofgraduate;       //上學年度畢業生數


    //ifstream, istream, ofstream, ostream are friends of department's, so they can acess private members in department
    friend ifstream &operator>>(ifstream&s, department&d);
    friend istream &operator>>(istream&s, department&d);
    friend ofstream &operator<<(ofstream&s, department d);
    friend ostream &operator<<(ostream&s, department d);
};

inline bool compare(department& a, department& b)
{
    return a.numofgraduate < b.numofgraduate;
}

template<typename v, typename T>
void bubblesort(vector<v>&vec, T sortrule) //bubble sort 602ms
{
    int indexvec[vec.size()];

    for(int i = 0; i < vec.size(); i++)
        indexvec[i] = i;

    for(int i = vec.size() - 1; i > 0; i--)
        for(int j = 0; j < i; j++)
            if(!sortrule(vec[indexvec[j]], vec[indexvec[j + 1]]))
                swap(indexvec[j], indexvec[j + 1]);

    vector<v> copyvec;

    copyvec.assign(vec.begin(), vec.end());
    vec.clear();
    for(int i = 0; i < copyvec.size(); i++)
        vec.push_back(copyvec[indexvec[i]]);
}

template<typename v, typename T>
void mergesort(vector<v>&vec, T sortrule) //mergesort 1524ms
{
    vector<int> tempvec;
    vector<int> indexvec;

    for(int i = 0; i < vec.size(); i++)
        indexvec.push_back(i);

    for(int r = 1; r < vec.size(); r *= 2)
    {
        tempvec.clear();
        tempvec.assign(indexvec.begin(), indexvec.end());
        indexvec.clear();
        for(int i = 0 ; i < tempvec.size() ; i += r * 2)
        {
            int x = i, y = i + r;
            while(x < i + r && y < i + r * 2 && y < tempvec.size() && x < tempvec.size())
            {
                if(sortrule(vec[tempvec[x]], vec[tempvec[y]]))
                    indexvec.push_back(tempvec[x++]);
                else
                    indexvec.push_back(tempvec[y++]);
            }

            while(x < i + r && x < tempvec.size())
                indexvec.push_back(tempvec[x++]);
            while(y < i + r * 2 && y < tempvec.size())
                indexvec.push_back(tempvec[y++]);
        }
    }


    vector<v> copyvec;

    copyvec.assign(vec.begin(), vec.end());
    vec.clear();
    for(int i = 0; i < indexvec.size(); i++)
        vec.push_back(copyvec[indexvec[i]]);
}

vector<int> indexvec;
template<typename v, typename T>
void quicksort(vector<v>&vec, T sortrule, int l = 0, int r = -2)    //quick sort 4ms
{
    if(r == -2)
    {
        for(int i = 0; i < vec.size(); i++)
            indexvec.push_back(i);
        quicksort(vec, sortrule, l, vec.size());
        vector<v> copyvec;

        copyvec.assign(vec.begin(), vec.end());
        vec.clear();
        for(int i = 0; i < indexvec.size(); i++)
            vec.push_back(copyvec[indexvec[i]]);
        indexvec.clear();
        return;
    }


    if(l >= r)
        return;

    int i = l, j = r - 1;
    while(i != j)
    {
        while(sortrule(vec[indexvec[l]], vec[indexvec[j]]) && i < j)
            j--;
        while(!(sortrule(vec[indexvec[l]], vec[indexvec[i]])) && i < j)
            i++;
        if(i < j)
            swap(indexvec[i], indexvec[j]);
    }
    swap(indexvec[i], indexvec[l]);
    quicksort(vec, sortrule, l, i);
    quicksort(vec, sortrule, i + 1, r);
}

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

inline int powten(int k)
{
    int i = 1;

    while(k--)
        i *= 10;
    return i;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

vector<int>        indexvecc;
int                minn;
vector<department> departmentvecc;
int                f[MAXthread], e[MAXthread];
bool               star[MAXthread];
bool               clos = 0;


int getminn(int x = -1, bool mandatory = 0)
{
    lock_guard<std::mutex> lg(mu);

    if(x == -1)
        return minn;

    if(!compare(departmentvecc[indexvecc[minn]], departmentvecc[indexvecc[x]]))
        minn = x;
    if(mandatory)
        minn = x;
    return 0;
}

int getf(int k, int x = -1)
{
    lock_guard<std::mutex> lg(mus[k]);

    if(x == -1)
        return f[k];

    f[k] = x;
    return 0;
}

int gete(int k, int x = -1)
{
    lock_guard<std::mutex> lg(mus[k]);

    if(x == -1)
        return e[k];

    e[k] = x;
    return 0;
}

bool getstar(int k)
{
    lock_guard<std::mutex> lg(mustar[k]);

    return star[k];
}

void changestar(int k, bool x)
{
    lock_guard<std::mutex> lg(mustar[k]);

    star[k] = x;
}

void partsort(int k)  //selection sort 32ms
{
    while(1)
    {
        if(getstar(k))
        {
            //cout << "run!" << f[k] << " " << e[k] << endl;
            int l = getf(k), r = gete(k);
            int min = l;
            for(int j = l + 1 ; j < r; j++)
                if(!compare(departmentvecc[indexvecc[minn]], departmentvecc[indexvecc[j]]))
                {
                    min = j;

                    //cout << "minn=" << minn << endl;
                }
            getminn(min);
            changestar(k, 0);
        }
        if(clos)
            return;
    }
}

void selectionsort() //selection sort 32ms
{
    for(int i = 0; i < departmentvecc.size(); i++)
        indexvecc.push_back(i);
    thread threads[MAXthread];
    clos = 0;
    for(int j = 0 ; j < MAXthread; j++)
    {
        changestar(j, 0);
        threads[j] = thread(partsort, j);
    }
    startTime();

    for(int i = 0; i + 1 < departmentvecc.size(); i++)
    {
        getminn(i, 1);
        for(int j = 0 ; j < MAXthread; j++)
        {
            getf(j, i + (departmentvecc.size() - i) * j / MAXthread);
            gete(j, i + (departmentvecc.size() - i) * (j + 1) / MAXthread);
            changestar(j, 1);
        }
        while(1)
        {
            bool b = 1;
            for(int j = 0 ; j < MAXthread; j++)
                if(getstar(j) == 1)
                    b = 0;
            if(b)
                break;
        }
        swap(indexvecc[getminn()], indexvecc[i]);
    }
    clos = 1;
    for(int j = 0 ; j < MAXthread; j++)
        threads[j].join();
    vector<department> copyvec;

    copyvec.assign(departmentvecc.begin(), departmentvecc.end());
    departmentvecc.clear();
    for(int i = 0; i < copyvec.size(); i++)
        departmentvecc.push_back(copyvec[indexvecc[i]]);
    indexvecc.clear();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


class sheet
{
public:
    vector<department> departmentvec; //vector store department the sheet has
    string inputlocate;               //where input from e.g. 201       (also name)

    sheet(string locate)              //constructer  read the file
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


        while(input >> d)   //while input available, then input department
            departmentvec.push_back(d);
        input.close();
    }

    void reread()
    {
        ifstream input("input" + inputlocate + ".txt");;
        int      skiplines;        // lines to skip in file's begin
        string   str;

        for(int i = 0; i < 3; i++) //input lines to skip useless information
            getline(input, str);
        departmentvec.clear();
        // anounce a new vector
        department d;


        while(input >> d)   //while input available, then input department
            departmentvec.push_back(d);
        input.close();
    }

    void print()  // print departmentvec
    {
        return;

        for(auto i:departmentvec)
            cout << i << '\n';
    }

    void writefile(string sorttype) //output
    {
        return;

        ofstream output(sorttype + inputlocate + ".txt", ios::trunc);


        //open new output file stream output201.txt or copy201.txt
        //clear the file is file already exist bt using ios::trunc


        for(auto i:departmentvec)  //output
            output << i << '\n';
        output.close();            //close file
    }

    void selectionsortt()
    {
        reread();
        departmentvecc.assign(departmentvec.begin(), departmentvec.end());
        startTime();

        selectionsort();
        departmentvec.clear();
        departmentvec.assign(departmentvecc.begin(), departmentvecc.end());
        departmentvecc.clear();
        cout << "selection sort:" << getTime() * 1000 << "ms\n";
        writefile("selectionsort");
    }

    void bubblesortt()
    {
        reread();
        startTime();

        bubblesort(departmentvec, compare);
        cout << "bubble sort:" << getTime() * 1000 << "ms\n";
        writefile("bubblesort");
    }

    void mergesortt()
    {
        reread();
        startTime();

        mergesort(departmentvec, compare);
        cout << "merge sort:" << getTime() * 1000 << "ms\n";
        writefile("mergesort");
    }

    void quicksortt()
    {
        reread();
        startTime();

        quicksort(departmentvec, compare);
        cout << "quick sort:" << getTime() * 1000 << "ms\n";
        writefile("quicksort");
    }

    void radixsortt() //1ms
    {
        reread();
        vector<int> indexvec;

        for(int i = 0; i < departmentvec.size(); i++)
            indexvec.push_back(i);

        startTime();
        queue<int> que[10];

        for(int i = 0 ; i < 10; i++)
        {
            for(int j = 0 ; j < departmentvec.size(); j++)
                que[(departmentvec[indexvec[j]].numofgraduate / powten(i) % 10)].push(indexvec[j]);
            indexvec.clear();
            for(int j = 0; j < 10; j++)
                while(que[j].size())
                {
                    indexvec.push_back(que[j].front());
                    que[j].pop();
                }
        }
        vector<department> copyvec;

        copyvec.assign(departmentvec.begin(), departmentvec.end());
        departmentvec.clear();
        for(int i = 0; i < indexvec.size(); i++)
            departmentvec.push_back(copyvec[indexvec[i]]);

        cout << "radix sort:" << getTime() * 1000 << "ms\n";
        writefile("radixsort");
    }
};


int main()
{
    //ios_base::sync_with_stdio(false);
    //cin.tie(0);
    int k = 100;


    while(k != 0)
    {
        cout << "\n";
        cout << "**  data operate system           **" << "\n"; //print menu
        cout << "* 0. Quit                          *" << "\n";
        cout << "* 1. read and sort and write file  *" << "\n";
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

        if(k != 0 && k != 1)
        {
            cout << "\nCommand does not exist!\n";
            continue; //skip this loop
        }

        if(k == 1)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet sh(locate);  //input file locate

            sh.selectionsortt();
            sh.bubblesortt();
            sh.mergesortt();
            sh.quicksortt();
            sh.radixsortt();
            sh.print();
        }
    }
}
