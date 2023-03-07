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

    int no;            //編號


    //ifstream, istream, ofstream, ostream are friends of department's, so they can acess private members in department
    friend ifstream &operator>>(ifstream&s, department&d);
    friend istream &operator>>(istream&s, department&d);
    friend ofstream &operator<<(ofstream&s, department d);
    friend ostream &operator<<(ostream&s, department d);


    auto operator<=>(const department& other) const
    {
        return numofgraduate - other.numofgraduate;
    }
};

class mindepartment
{
public:
    int no;            //編號
    int numofgraduate; //上學年度畢業生數
    mindepartment()
    {
    }

    mindepartment(department d)
    {
        no            = d.no;
        numofgraduate = d.numofgraduate;
    }

    auto operator<=>(const mindepartment& other) const
    {
        return numofgraduate - other.numofgraduate;
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

ostream &operator<<(ostream&s, mindepartment d)//department output stream Overload
{
    s << "[" << d.no << "] " << d.numofgraduate;
    return s;
}

ofstream &operator<<(ofstream&s, mindepartment d) //department output file stream Overload
{
    s << "[" << d.no << "] " << d.numofgraduate;
    return s;
}

class sheet
{
public:
    vector<mindepartment> heap; //vector store department the sheet has
    string inputlocate;         //where input from e.g. 201       (also name)
    sheet()                     //constructor let inherit to use
    {
    };

    sheet(string locate)     //constructer  read the file
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


        while(input >> d)    //while input available, then input department
        {
            mindepartment m(d);
            push(m);
        }
        input.close();
    }

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

    mindepartment root() //return root
    {
        return *heap.begin();
    }

    mindepartment bottom() //return bottom
    {
        return *heap.rbegin();
    }

    mindepartment leftmost()           //return leftmost
    {
        int i = 0;                     //root

        while(i * 2 + 1 < heap.size()) //not end
            i = i * 2 + 1;             //left
        return heap[i];
    }

    void push(mindepartment i)
    {
        i.no = heap.size() + 1;          //set no
        heap.push_back(i);               //pushback
        checkandchange(heap.size() - 1); //call checkandchange func
    }

    void checkandchange(int i)
    {
        int r = (i - 1) / 2; //root(parent)

        if(r < 0 || i == r)  //if i is root
            return;

        if(!(heap[r] <= heap[i]))   //if not legal
        {
            swap(heap[r], heap[i]); //swap
            checkandchange(r);      //check parent
        }
    }
};


bool maxlevel(int i) //return is in max level
{
    bool bo = 1;

    for(int l = 1 ; 1 ; l *= 2)
    {
        if(l - 1 > i)
            return bo;

        bo = !bo;
    }
}

bool minlevel(int i) //return is in min level
{
    bool bo = 0;

    for(int l = 1 ; 1 ; l *= 2)
    {
        if(l - 1 > i)
            return bo;

        bo = !bo;
    }
}

class sheet_2 : public sheet
{
public:
    vector<department> devec;
    sheet_2(string locate)     //constructer  read the file
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
            push(d);
        input.close();
    }

    void push(department i)
    {
        i.no = heap.size() + 1;
        devec.push_back(i);
        heap.push_back(i);
        int n = heap.size() - 1;
        if(n == 0)
            return;

        int r = (n - 1) / 2;          //parent node

        if(minlevel(r))               // root in min level
        {
            if(!(heap[r] <= heap[n])) //not compliance min heap->change
            {
                swap(heap[r], heap[n]);
                checkandchange_min_grand(r);
                return;
            }


            // already compliance min heap
            if(r == 0)
                return;       //no grandparent

            r = (r - 1) / 2;  //r = grandparent
            if(!(heap[r] >= heap[n]))
            {
                swap(heap[r], heap[n]);
                checkandchange_max_grand(r);
            }
            return;
        }


        if(maxlevel(r))               // root in min level
        {
            if(!(heap[r] >= heap[n])) //not compliance min heap->change
            {
                swap(heap[r], heap[n]);
                checkandchange_max_grand(r);
                return;
            }


            // already compliance max heap
            if(r == 0)
                return;       //no grandparent

            r = (r - 1) / 2;  //r = grandparent
            if(!(heap[r] <= heap[n]))
            {
                swap(heap[r], heap[n]);
                checkandchange_min_grand(r);
            }
        }
    }

    void checkandchange_min_grand(int i)
    {
        int r = (i - 1) / 2; //parent

        if(r == 0)
            return;


        r = (r - 1) / 2; //praent's parent(grand)

        if(r < 0 || i == r)
            return;

        if(!(heap[r] <= heap[i]))
        {
            swap(heap[r], heap[i]);
            checkandchange_min_grand(r);
        }
    }

    void checkandchange_max_grand(int i)
    {
        int r = (i - 1) / 2;

        if(r == 0)
            return;


        r = (r - 1) / 2;

        if(r < 0 || i == r)
            return;

        if(!(heap[r] >= heap[i]))
        {
            swap(heap[r], heap[i]);
            checkandchange_max_grand(r);
        }
    }

    void recheckandchange(int r)
    {
        int c0 = r * 2 + 1, c1 = r * 2 + 2;                                         //child
        int cc0 = c0 * 2 + 1, cc1 = c0 * 2 + 2, cc2 = c1 * 2 + 1, cc3 = c1 * 2 + 2; //grandchild
        int goal = -1;                                                              //to swap

        if(c0 >= heap.size())
            return;

        if(cc0 >= heap.size())//no grandchild
        {
            goal = c0;
            if(c1 < heap.size() && heap[goal] > heap[c1])
                goal = c1;
            if(heap[r] > heap[goal])
                swap(heap[r], heap[goal]);
            //recheckandchange(goal);
            return;
        }
        goal = cc0;
        if(cc1 < heap.size() && heap[goal] > heap[cc1])
            goal = cc1;
        if(cc2 < heap.size() && heap[goal] > heap[cc2])
            goal = cc2;
        if(cc3 < heap.size() && heap[goal] > heap[cc3])
            goal = cc3;
        //find smallest
        if(heap[r] > heap[goal])
        {
            swap(heap[r], heap[goal]);
            if(heap[goal] > heap[(goal - 1) / 2]) //recheck parent
                swap(heap[goal], heap[(goal - 1) / 2]);

            recheckandchange(goal);
        }
    }

    void printsmallest(int n)
    {
        n = min(n, int(heap.size()));
        int i = 0;
        while(n--)
        {
            cout << "top " << ++i << ":" << "[" << devec[heap[0].no - 1].no << "] " << devec[heap[0].no - 1].data[1] << " " << devec[heap[0].no - 1].data[3] << " " << devec[heap[0].no - 1].data[4] << " " << devec[heap[0].no - 1].data[5] << " " << devec[heap[0].no - 1].numofgraduate << endl;
            swap(heap[0], heap[heap.size() - 1]);
            heap.pop_back();
            recheckandchange(0);
        }
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
        cout << "* 2. min-max heap                  *" << "\n";
        cout << "* 3. smallest in min-max heap     **" << "\n";

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

            sheet sh(locate);  //input file locate
            cout << "<min heap>\n";
            cout << "root: [" << sh.root().no << "] " << sh.root().numofgraduate << endl;
            cout << "bottom: [" << sh.bottom().no << "] " << sh.bottom().numofgraduate << endl;
            cout << "leftmost: [" << sh.leftmost().no << "] " << sh.leftmost().numofgraduate << endl;
        }
        if(k == 2)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;

            sheet_2 sh(locate);  //input file locate
            cout << "<min-max heap>\n";
            cout << "root: [" << sh.root().no << "] " << sh.root().numofgraduate << endl;
            cout << "bottom: [" << sh.bottom().no << "] " << sh.bottom().numofgraduate << endl;
            cout << "leftmost: [" << sh.leftmost().no << "] " << sh.leftmost().numofgraduate << endl;
        }
        if(k == 3)
        {
            cout << "Input: ";
            string locate;
            cin >> locate;
            sheet_2 sh(locate);  //input file locate
            while(1)
            {
                cout << "how many do you want(-1 to quit): ";
                int n;
                cin >> n;
                if(n < 0)
                    break;
                sh.printsmallest(n);
            }
        }
    }
}
