//Dev-C++ -std=c++20
//曾品元11027233/江庭瑄11027235

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

#define MAXthread    3    //for multithreading

LARGE_INTEGER nFreq;
LARGE_INTEGER nBeginTime;
LARGE_INTEGER nEndTime;

void startTime() //start timing
{
    QueryPerformanceFrequency(&nFreq);
    QueryPerformanceCounter(&nBeginTime);
}

double getTime() //get timing
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


//內嵌函式(to speed up, cause it's short and often used)
inline bool compare(department&a, department&b) //compare rule (sortrule
{
    return a.numofgraduate > b.numofgraduate;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//in my algorithm, I use indexvec like a kind of iterator.
//we usually swap the whole data, which cause lots of data transfer
//indexvec means vec at first, so that I only have to copy the vec once after sort
template<typename v, typename T>
void bubblesort(vector<v>&vec, T sortrule) //bubble sort 602ms
{
    int indexvec[vec.size()];              //declare an array whose size same to the vec

    for(int i = 0; i < vec.size(); i++)    //store the index in them
        indexvec[i] = i;

    for(int i = vec.size() - 1; i > 0; i--)
        for(int j = 0; j < i; j++)
            if(!sortrule(vec[indexvec[j]], vec[indexvec[j + 1]]))
                swap(indexvec[j], indexvec[j + 1]);

//reduction data
    vector<v> copyvec;

    copyvec.assign(vec.begin(), vec.end());//copy vec to the copyvec then clear vec
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
        tempvec.assign(indexvec.begin(), indexvec.end()); //copy indexvec to tempvec
        indexvec.clear();                                 //clear indexvec
        for(int i = 0 ; i < tempvec.size() ; i += r * 2)
        {
            int x = i, y = i + r;
            while(x < i + r && y < i + r * 2 && y < tempvec.size() && x < tempvec.size()) //push smaller one to indexvec
            {
                if(sortrule(vec[tempvec[x]], vec[tempvec[y]]))
                    indexvec.push_back(tempvec[x++]);
                else
                    indexvec.push_back(tempvec[y++]);
            }

            while(x < i + r && x < tempvec.size()) //push the ramaining thing to indexvec
                indexvec.push_back(tempvec[x++]);
            while(y < i + r * 2 && y < tempvec.size())
                indexvec.push_back(tempvec[y++]);
        }
    }

    //reduction data
    vector<v> copyvec;

    copyvec.assign(vec.begin(), vec.end());
    vec.clear();
    for(int i = 0; i < indexvec.size(); i++)
        vec.push_back(copyvec[indexvec[i]]);
}

template<typename v, typename T>
void quicksort(vector<v>&vec, T sortrule, int l = 0, int r = -2)    //quick sort 4ms
{
    static vector<int> indexvec;

    if(r == -2)
    {
        for(int i = 0; i < vec.size(); i++)
            indexvec.push_back(i);
        quicksort(vec, sortrule, l, vec.size()); //recursive


        //reduction data
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
        while(sortrule(vec[indexvec[l]], vec[indexvec[j]]) && i < j)    //find last j which doesn't conform the rule
            j--;
        while(!(sortrule(vec[indexvec[l]], vec[indexvec[i]])) && i < j) //find first i which doesn't conform the rule
            i++;
        if(i < j)
            swap(indexvec[i], indexvec[j]);
    }
    swap(indexvec[i], indexvec[l]);
    quicksort(vec, sortrule, l, i);     //recursive
    quicksort(vec, sortrule, i + 1, r); //recursive
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

inline int powten(int k) //get 10**k((10的k次
{
    int i = 1;

    while(k--)
        i *= 10;
    return i;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
mutex              mu_minn, mus_f[MAXthread], mus_e[MAXthread], mus_star[MAXthread]; //mutex to lock the thread
vector<int>        indexvecc;                                                        //indexvec for selection sort
int                minn;                                                             //min
vector<department> departmentvecc;                                                   //departmentvec for selection sort
int                f[MAXthread], e[MAXthread];                                       //first and end
bool               star[MAXthread];                                                  //control thread to run
bool               clos = 0;                                                         //control thread to close itself
//use mutex to lock different thread so that data won't get wrong
//because it will cause lost of time to explain mutex,
//you can read https://shengyu7697.github.io/std-mutex/

void partsort(int k)  //selection find the minnest int the specify part(f->e)
{
    bool keeprunning;
    int  l, r, min;

    //announce hear to avoid announce repeated

    while(1)
    {
        mus_star[k].lock();
        keeprunning = star[k]; //tell thread start running or not
        mus_star[k].unlock();
        if(keeprunning)
        {
            mus_f[k].lock();
            l = f[k]; //get first
            mus_f[k].unlock();

            mus_e[k].lock();
            r = e[k]; //get end
            mus_e[k].unlock();


            min = l;
            for(int j = l + 1 ; j < r; j++) //find the minnest
                if(!compare(departmentvecc[indexvecc[min]], departmentvecc[indexvecc[j]]))
                    min = j;


            mu_minn.lock();
            if(!compare(departmentvecc[indexvecc[minn]], departmentvecc[indexvecc[min]]))
                minn = min; //change if departmentvecc[indexvecc[min]] < departmentvecc[indexvecc[minn]]

            mu_minn.unlock();

            mus_star[k].lock();
            star[k] = 0; //pause
            mus_star[k].unlock();
        }
        if(clos)
            return;
    }
}

void selectionsort() //selection sort 32ms
{
    for(int i = 0; i < departmentvecc.size(); i++)
        indexvecc.push_back(i);
    thread threads[MAXthread]; //new thread
    clos = 0;
    for(int j = 0 ; j < MAXthread; j++)
    {
        mus_star[j].lock();
        star[j] = 0;                      //pause
        mus_star[j].unlock();
        threads[j] = thread(partsort, j); //give thread thing to run
    }
    startTime();

    for(int i = 0; i + 1 < departmentvecc.size(); i++)
    {
        mu_minn.lock();
        minn = i; //set minn
        mu_minn.unlock();

        for(int j = 0 ; j < MAXthread; j++)
        {
            mus_f[j].lock();
            f[j] = i + (departmentvecc.size() - i) * j / MAXthread;
            mus_f[j].unlock();

            mus_e[j].lock();
            e[j] = (departmentvecc.size() - i) * (j + 1) / MAXthread;
            mus_e[j].unlock();
            //give thread range to run

            mus_star[j].lock();
            star[j] = 1; //start thread
            mus_star[j].unlock();
        }

        bool b;
        while(1)
        {
            b = 1;
            for(int j = 0 ; j < MAXthread; j++)
            {
                mus_star[j].lock();
                if(star[j] == 1)
                    b = 0;
                mus_star[j].unlock();
            }

            if(b) //if all thread paused means all finished
                break;
        }
        mu_minn.lock();
        swap(indexvecc[minn], indexvecc[i]); //swap
        mu_minn.unlock();
    }
    clos = 1;
    for(int j = 0 ; j < MAXthread; j++)
        threads[j].join();

    //reduction data
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
                que[(departmentvec[indexvec[j]].numofgraduate / powten(i) % 10)].push(indexvec[j]);  //push into x%(10**n)
            indexvec.clear();
            for(int j = 9; j >= 0; j--)                                                              //get all int the que
                while(que[j].size())
                {
                    indexvec.push_back(que[j].front());
                    que[j].pop();
                }
        }

        //reduction data
        vector<department> copyvec;

        copyvec.assign(departmentvec.begin(), departmentvec.end());
        departmentvec.clear();
        for(int i = 0; i < indexvec.size(); i++)
            departmentvec.push_back(copyvec[indexvec[i]]);

        cout << "radix sort:" << getTime() * 1000 << "ms\n";
        writefile("radixsort");
    }

    auto selectionsortt_notime()
    {
        reread();
        departmentvecc.assign(departmentvec.begin(), departmentvec.end());
        startTime();

        selectionsort();
        departmentvec.clear();
        departmentvec.assign(departmentvecc.begin(), departmentvecc.end());
        departmentvecc.clear();
        return getTime() * 1000;
    }

    auto bubblesortt_notime()
    {
        reread();
        startTime();

        bubblesort(departmentvec, compare);
        return getTime() * 1000;
    }

    auto mergesortt_notime()
    {
        reread();
        startTime();

        mergesort(departmentvec, compare);
        return getTime() * 1000;
    }

    auto quicksortt_notime()
    {
        reread();
        startTime();

        quicksort(departmentvec, compare);
        return getTime() * 1000;
    }

    auto radixsortt_notime() //1ms
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
                que[(departmentvec[indexvec[j]].numofgraduate / powten(i) % 10)].push(indexvec[j]);  //push into x%(10**n)
            indexvec.clear();
            for(int j = 9; j >= 0; j--)                                                              //get all int the que
                while(que[j].size())
                {
                    indexvec.push_back(que[j].front());
                    que[j].pop();
                }
        }

        //reduction data
        vector<department> copyvec;

        copyvec.assign(departmentvec.begin(), departmentvec.end());
        departmentvec.clear();
        for(int i = 0; i < indexvec.size(); i++)
            departmentvec.push_back(copyvec[indexvec[i]]);

        return getTime() * 1000;
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
        cout << "* 2. Comparisons on five methods  **" << "\n";
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

            sheet sh(locate);  //input file locate

            sh.selectionsortt();
            sh.bubblesortt();
            sh.mergesortt();
            sh.quicksortt();
            sh.radixsortt();
            //sh.print();
        }
        if(k == 2)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;
            ofstream out;
            ifstream in("sort_time.txt");


            if(!in)
            {
                out.open("sort_time.txt");
                out << "檔案編號  選擇排序    氣泡排序    合併排序    快速排序    基數排序\n";
            }
            else
            {
                in.close();
                out.open("sort_time.txt", ios::app);
            }

            sheet sh(locate);  //input file locate
            out << sh.inputlocate << '\t';
            out << sh.selectionsortt_notime() << '\t';
            out << sh.bubblesortt_notime() << '\t';
            out << sh.mergesortt_notime() << '\t';
            out << sh.quicksortt_notime() << '\t';
            out << sh.radixsortt_notime() << '\n';
            //sh.print();
        }
    }
}
