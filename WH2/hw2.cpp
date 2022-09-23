#include <bits/stdc++.h>
using namespace std;


//學校代碼  學校名稱    科系代碼    科系名稱        日間／進修別  等級別 學生數 教師數 上學年度畢業生數    縣市名稱    體系別
//0002  國立清華大學  520114  資訊工程學系  D 日 B       學士       565     43     117             18 新竹市  1 一般

class department
{
public:
    string data[8];     //學校代碼 學校名稱 科系代碼 科系名稱 日間／進修別  等級別  縣市名稱 體系別
    int numofstudent;   //學生數
    int numofteacher;   //教師數
    int numofgraduate;  //上學年度畢業生數
    bool readsuccess = true; //input successed or not


    friend ifstream &operator>>(ifstream&s, department&d);
    friend istream &operator>>(istream&s, department&d);
    friend ofstream &operator<<(ofstream&s, department d);
    friend ostream &operator<<(ostream&s, department d);
};


ifstream &operator>>(ifstream&s, department&d) //department input file stream Overload
{
    string str;


    getline(s, str); //read one line
    if (str.size() < 3)
    {
        d.readsuccess = false;
        return s;
    }
    int i = 0;


    for (int j = 0; j < 8; j++)
        d.data[j] = "";
    for (int j = 0; j < 6; j++)
    {
        for (; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
            d.data[j] += str[i];
        i++; //skip '\t'
    }

    string num; //string to store number


    for (; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
        num += str[i];
    i++; //skip '\t'
    d.numofstudent = stoi(num); //convert string to int

    num = "";
    for (; i < str.size() && str[i] != '\t'; i++)
        num += str[i];
    i++;
    d.numofteacher = stoi(num);

    num = "";
    for (; i < str.size() && str[i] != '\t'; i++)
        num += str[i];
    i++;
    d.numofgraduate = stoi(num);

    for (int j = 6; j < 8; j++)
    {
        for (; i < str.size() && str[i] != '\t'; i++)
            d.data[j] += str[i];
        i++;
    }
    return s;
}

istream &operator>>(istream&s, department&d) //department input stream Overload
{
    string str;


    getline(s, str);
    if (str.size() < 3)
    {
        d.readsuccess = false;
        return s;
    }
    int i = 0;


    for (int j = 0; j < 8; j++)
        d.data[j] = "";
    for (int j = 0; j < 6; j++)
    {
        for (; i < str.size() && str[i] != '\t'; i++)
            d.data[j] += str[i];
        i++;
    }

    string num;


    for (; i < str.size() && str[i] != '\t'; i++)
        num += str[i];
    i++;
    d.numofstudent = stoi(num);

    num = "";
    for (; i < str.size() && str[i] != '\t'; i++)
        num += str[i];
    i++;
    d.numofteacher = stoi(num);

    num = "";
    for (; i < str.size() && str[i] != '\t'; i++)
        num += str[i];
    i++;
    d.numofgraduate = stoi(num);

    for (int j = 6; j < 8; j++)
    {
        for (; i < str.size() && str[i] != '\t'; i++)
            d.data[j] += str[i];
        i++;
    }
    return s;
}

ostream &operator<<(ostream&s, department d)//department output stream Overload
{
    for (int i = 0; i < 6; i++)
        s << d.data[i] << '\t'; //optput data and use '\t' to separate data
    s << d.numofstudent << '\t';
    s << d.numofteacher << '\t';
    s << d.numofgraduate << '\t';
    for (int i = 6; i < 8; i++)
        s << d.data[i] << '\t';
    return s;
}

ofstream &operator<<(ofstream&s, department d) //department output file stream Overload
{
    for (int i = 0; i < 6; i++)
        s << d.data[i] << '\t'; //optput data and use '\t' to separate data
    s << d.numofstudent << '\t';
    s << d.numofteacher << '\t';
    s << d.numofgraduate << '\t';
    for (int i = 6; i < 8; i++)
        s << d.data[i] << '\t';
    return s;
}

class sheet
{
public:
    vector <department> departmentvec; //vector store department the sheet has
    string inputlocate; //where input from e.g. 201       (also name)

    sheet(string locate, bool state) //constructer
    {
        ifstream input;
        int skiplines; //lines to skip in file's begin

        if (!state) //file named inputxxx.txt or xxx
        {
            skiplines = 3;
            if (locate.size() < 10) //length("xxx") < length("input" +".txt")
                inputlocate = locate; //store xxx
            else
            {
                inputlocate = locate.substr(5); //"inputxxx.txt" to "xxx.txt"
                inputlocate = inputlocate.substr(0, inputlocate.size() - 4); //"xxx.txt" to "xxx"
            }

            input.open("input" + inputlocate + ".txt"); //open file "inputxxx.txt"
            while (!input) // if "inputxxx.txt" can't be opened
            {
                cout << "\nCan't open file, typenew: ";
                cin >> locate;
                if (locate.size() < 10)            //"input" +".txt"
                    inputlocate = locate;
                else
                {
                    inputlocate = locate.substr(5);
                    inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
                }
                input.open("input" + inputlocate + ".txt"); //reopen
            }
        }
        else //file named copyxxx.txt or xxx
        {
            skiplines = 0;
            if (locate.size() < 9)
                inputlocate = locate;
            else
            {
                inputlocate = locate.substr(4);
                inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
            }
            input.open("copy" + inputlocate + ".txt");

            while (!input)
            {
                cout << "\nCan't open file, typenew: ";
                cin >> locate;
                if (locate.size() < 9)
                    inputlocate = locate;
                else
                {
                    inputlocate = locate.substr(4);;
                    inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
                }
                input.open("copy" + inputlocate + ".txt");
            }
        }


        string str;
        for (int i = 0; i < skiplines; i++) //input lines to skip useless information
            getline(input, str);


        department d;


        while (input >> d && d.readsuccess == 1) // input department while input available
            //cout<<d<<endl;
            departmentvec.push_back(d);
    }

    sheet(sheet s1, sheet s2) //construct use two sheet
    {
        //put s1.departmentvec and s2.departmentvec into departmentvec
        for (auto i:s1.departmentvec)
            departmentvec.push_back(i);
        for (auto i:s2.departmentvec)
            departmentvec.push_back(i);
        sort(departmentvec.begin(), departmentvec.end(), [](department d1, department d2){
            return d1.data[0] < d2.data[0];
        }); //sort departmentvec use schoolcode
        inputlocate = s1.inputlocate + "_" + s2.inputlocate; //new inputlocate(name) = "201"+"_"+"202"="201_202"
    }

    void print() //print departmentvec
    {
        for (auto i:departmentvec)
            cout << i << '\n';
    }

    void eraseif(int student, int graduate) //find all data which doesn't satisfied the claim and erase it
    {
        while (find_if(departmentvec.begin(), departmentvec.end(), [student, graduate](department d){
            return d.numofstudent < student || d.numofgraduate < graduate;
        }) != departmentvec.end())
            departmentvec.erase(find_if(departmentvec.begin(), departmentvec.end(), [student, graduate](department d){
                return d.numofstudent < student || d.numofgraduate < graduate;
            }));
    }

    int size() //return the total number of department
    {
        return departmentvec.size();
    }

    void writefile(bool state)
    {
        ofstream output((state?"output":"copy") + inputlocate + ".txt", ios::trunc);
        //open new output file stream output201.txt or copy201.txt
        //clear the file is file already exist bt using ios::trunc


        for (auto i:departmentvec) //output
            output << i << '\n';
        output.close(); //close file
    }
};


int main()
{
    //sheet s("input201.txt");


    int k = 100;


    while (k != 0)
    {
        cout << "\n";
        cout << "**  data operate system  **" << "\n"; //print menu
        cout << "* 0. Quit                 *" << "\n";
        cout << "* 1. read and write file  *" << "\n";
        cout << "* 2. read and select file *" << "\n";
        cout << "* 3. read and merge file  *" << "\n";
        cout << "***************************" << "\n";
        cout << "Input a choice(0, 1, 2, 3): ";
        cin >> k;
        if (cin.fail())
        {
            cout << "\nCommand does not exist!\n";
            cin.clear();  //set cin stream to good(work) situation
            cin.ignore(); //skip next char in input stream

            k = -1;       //let k != 0 to keep loop continue
            continue;     //skip this loop
        }

        if (k != 0 && k != 1 && k != 2 && k != 3)
        {
            cout << "\nCommand does not exist!\n";
            continue; //skip this loop
        }
        if (k == 1)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet firstsheet(locate, 0);

            cout << "Total number of records = " << firstsheet.size() << endl;
            firstsheet.print();
            firstsheet.writefile(0);
        }

        if (k == 2)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet firstsheet(locate, 1);
            int   student, graduate;
            cout << "Threshold of student: ";
            cin >> student;
            cout << "Threshold of graduates: ";
            cin >> graduate;
            firstsheet.eraseif(student, graduate);

            cout << "Total number of records = " << firstsheet.size() << endl;
            firstsheet.print();
            firstsheet.writefile(0);
        }

        if (k == 3)
        {
            cout << "Input 1st file: ";
            string locate;
            cin >> locate;
            sheet firstsheet(locate, 1);

            cout << "Input 2nd file: ";
            cin >> locate;
            sheet secondsheet(locate, 1);

            sheet totalsheet(firstsheet, secondsheet);

            cout << "Total number of records = " << totalsheet.size() << endl;
            totalsheet.print();
            totalsheet.writefile(1);
        }
    }
}
