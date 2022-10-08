//Dev-C++ -std=c++14
//曾品元11027233/江庭瑄11027235
#include <bits/stdc++.h>
using namespace std;


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


ifstream &operator>>(ifstream&s, department&d) //department input file stream Overload, return what it read
{
	string str;

	getline(s, str); //read one line
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
	for(; i < str.size() && str[i] != '\t'; i++)  //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                          //skip '\t'
	d.numofstudent = stoi(num);                   //convert string to int

	//read in 教師人數
	num = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                         //skip '\t'
	d.numofteacher = stoi(num);                  //convert string to int

	//read in 畢業人數
	num = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                         //skip '\t'
	d.numofgraduate = stoi(num);                 //convert string to int

	//read in 縣市名稱~體系別
	for(int j = 6; j < 8; j++)
	{
		for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
			d.data[j] += str[i];
		i++;                                         //skip '\t'
	}

	s.get(); //clear remained '\n'
	return s;
}

istream &operator>>(istream&s, department&d) //department input stream Overload
{
	string str;


	getline(s, str); //read one line
	int i = 0;


	for(int j = 0; j < 8; j++)                       //clear up the data[8]
		d.data[j] = "";
	for(int j = 0; j < 6; j++)                       //read in 學校代碼~等級別
	{
		for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
			d.data[j] += str[i];
		i++;                                         //skip '\t'
	}

	string num;

	//read in 學生人數
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                         //skip '\t'
	d.numofstudent = stoi(num);                  //convert string to int

	//read in 教師人數
	num = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                         //skip '\t'
	d.numofteacher = stoi(num);                  //convert string to int

	//read in 畢業人數
	num = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                         //skip '\t'
	d.numofgraduate = stoi(num);                 //convert string to int

	//read in 縣市名稱~體系別
	for(int j = 6; j < 8; j++)
	{
		for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
			d.data[j] += str[i];
		i++;                                         //skip '\t'
	}

	s.get(); //clear remained '\n'
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
	vector <department> departmentvec; //vector store department the sheet has
	string inputlocate;                //where input from e.g. 201       (also name)

	sheet(string locate, bool state)   //constructer  read the file
	//((process the file name，decide if we should skip the first three lines，read the content
	{
		ifstream input;
		int		 skiplines; // lines to skip in file's begin


		// state = 0 : file named inputxxx.txt or xxx
		if(!state)
		{
			skiplines = 3;

			//全部都存成xxx
			if(locate.size() < 10)                                           // length("xxx") < length("input" +".txt")
				inputlocate = locate;                                        // store xxx
			else                                                             // inputxxx.txt
			{
				inputlocate = locate.substr(5);                              //"inputxxx.txt" to "xxx.txt"
				inputlocate = inputlocate.substr(0, inputlocate.size() - 4); //"xxx.txt" to "xxx"
			}

			//再一律加上input和.txt
			input.open("input" + inputlocate + ".txt"); //open file "inputxxx.txt"


			//if "inputxxx.txt" can't be opened, input the filename again and repeat the steps of processing above
			while(!input)
			{
				cout << "\nCan't open file, typenew: ";
				cin >> locate;
				if(locate.size() < 10)             //"input" +".txt"
					inputlocate = locate;
				else
				{
					inputlocate = locate.substr(5);
					inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
				}
				input.open("input" + inputlocate + ".txt"); //reopen
			}
		}


		//state = 1 : file named copyxxx.txt or xxx
		else
		{
			skiplines = 0;

			// 全部都存成xxx
			if(locate.size() < 9)                                            //xxx
				inputlocate = locate;                                        //store xxx

			else                                                             //copyxxx.txt
			{
				inputlocate = locate.substr(4);                              //"copyxxx.txt" to "xxx.txt"
				inputlocate = inputlocate.substr(0, inputlocate.size() - 4); //"xxx.txt" to "xxx"
			}

			// 再一律加上copy和.txt
			input.open("copy" + inputlocate + ".txt");  //open file "inputxxx.txt"

			//if "copyxxx.txt" can't be opened, input the filename again and repeat the steps of processing above
			while(!input)
			{
				cout << "\nCan't open file, typenew: ";
				cin >> locate;
				if(locate.size() < 9)
					inputlocate = locate;
				else
				{
					inputlocate = locate.substr(4);
					inputlocate = inputlocate.substr(0, inputlocate.size() - 4);
				}
				input.open("copy" + inputlocate + ".txt"); //reopen
			}
		}


		string str;


		for(int i = 0; i < skiplines; i++)  //input lines to skip useless information
			getline(input, str);

		// anounce a new vector
		department d;


		while(!input.eof() && input >> d) //while input available, then input department
			departmentvec.push_back(d);
		input.close();
	}

	sheet(sheet s1, sheet s2) //construct use two sheet
	{
		//merge two vec : s1.departmentvec and s2.departmentvec -> departmentvec
		for(auto i:s1.departmentvec)
			departmentvec.push_back(i);
		for(auto i:s2.departmentvec)
			departmentvec.push_back(i);
		sort(departmentvec.begin(), departmentvec.end(), [](department d1, department d2){
			return d1.data[0] < d2.data[0];
		});                                                  //sort departmentvec use schoolcode
		inputlocate = s1.inputlocate + "_" + s2.inputlocate; //new inputlocate(name) = "201"+"_"+"202"="201_202"
	}

	void print()  // print departmentvec
	{
		for(auto i:departmentvec)
			cout << i << '\n';
	}

	void eraseif(int student, int graduate) //find all data which doesn't satisfied the claim and erase it. the filter
	{
		while(find_if(departmentvec.begin(), departmentvec.end(), [student, graduate](department d){
			return d.numofstudent < student || d.numofgraduate < graduate;
		}) != departmentvec.end())
			departmentvec.erase(find_if(departmentvec.begin(), departmentvec.end(), [student, graduate](department d){
				return d.numofstudent < student || d.numofgraduate < graduate;
			})); //while there exist a department which doesn't satisfy the request, erase it
	}

	int size() //return the total number of department
	{
		return departmentvec.size();
	}

	void writefile(bool state) //output
	{
		ofstream output((state?"output":"copy") + inputlocate + ".txt", ios::trunc);


		//open new output file stream output201.txt or copy201.txt
		//clear the file is file already exist bt using ios::trunc


		for(auto i:departmentvec)  //output
			output << i << '\n';
		output.close();            //close file
	}
};


int main()
{
	int k = 100;


	while(k != 0)
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
			cin >> locate;               //input file locate(name)

			sheet firstsheet(locate, 0); //use the constructor to read the file

			cout << "Total number of records = " << firstsheet.size() << endl;
			firstsheet.print();
			firstsheet.writefile(0);  //0->copy
		}

		if(k == 2)
		{
			cout << "Input: ";
			string locate;
			cin >> locate;

			sheet firstsheet(locate, 1);  //input file locate(name)
			int	  student, graduate;
			cout << "Threshold of student: ";
			cin >> student;
			cout << "Threshold of graduates: ";
			cin >> graduate;
			firstsheet.eraseif(student, graduate);  //select the vector

			cout << "Total number of records = " << firstsheet.size() << endl;
			firstsheet.print();
			firstsheet.writefile(0);    //0->copy
		}

		if(k == 3)
		{
			cout << "Input 1st file: ";
			string locate;
			cin >> locate;
			sheet firstsheet(locate, 1);  //input file locate(name)

			cout << "Input 2nd file: ";
			cin >> locate;
			sheet secondsheet(locate, 1);              //input file locate(name)

			sheet totalsheet(firstsheet, secondsheet); //construct using two sheet

			cout << "Total number of records = " << totalsheet.size() << endl;
			totalsheet.print();
			totalsheet.writefile(1);    //1->output
		}
	}
}
