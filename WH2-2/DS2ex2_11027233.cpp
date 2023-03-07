//11027233 曾品元
//IDE Dev-C++, vscode, sublime, codeblock or even notepad
//Compiler gcc mingw https://github.com/Vuniverse0/mingwInstaller/releases/download/1.2.0/mingwInstaller.exe
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//!!!! USE C++20 to compile or you may get an error!!!!!!!!
//https://blog.csdn.net/qq_50285142/article/details/122930647
#include <bits/stdc++.h>
using namespace std;


class department
{
public:
    string data[8];    //學校代碼 學校名稱 科系代碼 科系名稱 日間／進修別  等級別  縣市名稱 體系別
    int numofstudent;  //學生數
    int numofteacher;  //教師數
    int numofgraduate; //上學年度畢業生數

    int no = -1;       //編號


    //ifstream, istream, ofstream, ostream are friends of department's, so they can acess private members in department
    friend ifstream &operator>>(ifstream&s, department&d);
    friend istream &operator>>(istream&s, department&d);
    friend ofstream &operator<<(ofstream&s, department d);
    friend ostream &operator<<(ostream&s, department d);


    auto operator<=>(const department& other) const
    {
        return data[1] <=> other.data[1];
    }

    void operator=(const department& other)
    {
        for(int i = 0 ; i < 8 ; i++)
            data[i] = other.data[i];
        numofstudent  = other.numofstudent;
        numofteacher  = other.numofteacher;
        numofgraduate = other.numofgraduate;
        no            = other.no;
    }

    ~department()
    {
        cout << "destrouct!!" << no << endl;
    }
};


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

class sheet
{
public:
    vector<department> heap; //vector store department the sheet has
    string inputlocate;      //where input from e.g. 201       (also name)
    sheet()                  //constructor let inherit to use
    {
    };


    void print()  // print heap
    {
        for(const auto&i:heap)
            cout << i << '\n';
    }

    void writefile(string sorttype) //output
    {
        return;

        ofstream output(sorttype + inputlocate + ".txt", ios::trunc);


        //open new output file stream output201.txt or copy201.txt
        //clear the file is file already exist bt using ios::trunc


        for(auto i:heap)  //output
            output << i << '\n';
        output.close();   //close file
    }
};

class sheet_23 : public sheet
{
public:
    class node_23
    {
        shared_ptr<department> de[2];
        shared_ptr<node_23> child[3];
    };
    sheet_23(string locate) //constructer  read the file
    {                       //((process the file name，decide if we should skip the first three lines，read the content
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


        while(input >> d)    //while input available, then input department

            push(d);
        input.close();
    }

    void push(department i)
    {
        i.no = heap.size() + 1;          //set no
        heap.push_back(i);               //pushback
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
        cout << "* 1. min heap                      *" << "\n";
        cout << "* 2. max heap                     **" << "\n";
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

            //numofstudentsheet sh(locate);  //input file locate
        }
    }
}
