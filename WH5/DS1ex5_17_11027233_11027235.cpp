//Dev-C++ -std=c++20
//曾品元11027233/江庭瑄11027235

#include <bits/stdc++.h>
using namespace std;


template<typename v, typename T>
void choosesort(vector<v>&vec, T sortrule) //shell sort
{
    for(int i = 0; i < vec.size(); i++)
    {
        int max = i;
        for(int j = i + 1; j < vec.size(); j++)
            if(!sortrule(vec[max], vec[j]))
                max = j;
        swap(vec[max], vec[i]);
    }
}

template<typename v, typename T>
void bubblesort(vector<v>&vec, T sortrule) //shell sort
{
    for(int i = vec.size() - 1; i > 0; i--)
        for(int j = 0; j < i; j++)
            if(!sortrule(vec[j], vec[j + 1]))
                swap(vec[j], vec[j + 1]);
}

template<typename v, typename T>
void mergesort(vector<v>&vec, T sortrule)
{
    for(int r = 1; r < vec.size(); r *= 2)
        for(int i = 0 ; i < vec.size() ;)
        {
            int       x = i, y = i + r;
            vector<v> tempvec;
            while(x < i + r && y < i + r * 2 && y < vec.size())
            {
                if(sortrule(vec[x], vec[y]))
                    tempvec.push_back(vec[x++]);
                else
                    tempvec.push_back(vec[y++]);
            }

            while(x < i + r)
                tempvec.push_back(vec[x++]);
            while(y < i + r * 2 && y < vec.size())
                tempvec.push_back(vec[y++]);

            for(auto t:tempvec)
                vec[i++] = t;
        }
}

template<typename v, typename T>
void quicksort(vector<v>&vec, T sortrule)
{
    function<void(int, int)> sortpart = [&](int f, int e) -> void
                                        {
                                            if(e == f + 1)
                                                return;

                                            int i = f + 1, j = e - 1;

                                            while(i <= j)
                                            {
                                                while(sortrule(vec[i], vec[f]) && i < e && i < j)
                                                    i++;
                                                while(sortrule(vec[f], vec[j]) && f < j && i < j)
                                                    j--;
                                                if(i == j)
                                                {
                                                    if(!sortrule(vec[f], vec[i]))
                                                        swap(vec[f], vec[i]);
                                                    sortpart(f, i);
                                                    sortpart(i, e);
                                                    break;
                                                }
                                                else
                                                    swap(vec[i], vec[j]);
                                            }
                                        };

    sortpart(0, vec.size());
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

bool compare(department a, department b)
{
    return a.numofgraduate < b.numofgraduate;
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

int powten(int k)
{
    int i = 1;

    while(k--)
        i *= 10;
    return i;
}

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

    void print()  // print departmentvec
    {
        for(auto i:departmentvec)
            cout << i << '\n';
    }

    void writefile(string sorttype) //output
    {
        ofstream output(sorttype + inputlocate + ".txt", ios::trunc);


        //open new output file stream output201.txt or copy201.txt
        //clear the file is file already exist bt using ios::trunc


        for(auto i:departmentvec)  //output
            output << i << '\n';
        output.close();            //close file
    }

    void choosesortt()
    {
        int t = clock();

        choosesort(departmentvec, compare);
        cout << "choose sort:" << clock() - t << "ms\n";
        writefile("choosesort");
    }

    void bubblesortt()
    {
        int t = clock();

        bubblesort(departmentvec, compare);
        cout << "bubble sort:" << clock() - t << "ms\n";
        writefile("bubblesort");
    }

    void mergesortt()
    {
        int t = clock();

        mergesort(departmentvec, compare);
        cout << "merge sort:" << clock() - t << "ms\n";
        writefile("mergesort");
    }

    void quicksortt()
    {
        int t = clock();

        quicksort(departmentvec, compare);
        cout << "quick sort:" << clock() - t << "ms\n";
        writefile("quicksort");
    }

    void radixsortt()
    {
        int t = clock();
        queue<department> que[10];

        for(int i = 0 ; i < 10; i++)
        {
            for(int j = 0 ; j < departmentvec.size(); j++)
                que[(departmentvec[j].numofgraduate / powten(i) % 10)].push(departmentvec[j]);
            departmentvec.clear();
            for(int j = 0; j < 10; j++)
                while(que[j].size())
                {
                    departmentvec.push_back(que[j].front());
                    que[j].pop();
                }
        }

        cout << "radix sort:" << clock() - t << "ms\n";
        writefile("radixsort");
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
            sh.choosesortt();
            sh.bubblesortt();
            sh.mergesortt();
            sh.quicksortt();
            sh.radixsortt();
            sh.print();
        }
    }
}
