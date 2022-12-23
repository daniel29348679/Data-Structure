//Dev-C++ -std=c++20
//曾品元11027233/江庭瑄11027235

#include <bits/stdc++.h>
using namespace std;


class pokemon //Name	Type 1	Type 2	Total	HP	Attack	Defense	Sp. Atk	Sp. Def	Speed	Generation	Legendary
{
public:
	int no;
	string name;
	string type1;
	string type2;
	int total;
	int hp;
	int attack;
	int defense;
	int spatk;
	int spdefence;
	int speed;
	int generation;
	string legendary;

	auto operator==(const pokemon&p) const
	{
		return hp == p.hp;
	}

	auto operator<=>(const pokemon&p) const
	{
		return hp - p.hp;
	}

	auto operator<=>(const int&p) const
	{
		return hp - p;
	}

	//friend ifstream &operator>>(ifstream&s, pokemon&d);
};


ifstream &operator>>(ifstream&s, pokemon&d) //department input file stream Overload, return what it read
{
	string str;

	getline(s, str);           //read one line
	if(str.size() < 3)         //too short = error
	{
		s.clear(ios::failbit); //set failbit to true
		return s;
	}
	int i = 0;

	string num;                                                //string to store number

	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.no = stoi(num);                                          //convert string to int

	d.name = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		d.name += str[i];
	i++;                                         //skip '\t'


	d.type1 = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		d.type1 += str[i];
	i++;

	d.type2 = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		d.type2 += str[i];
	i++;

	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.total = stoi(num);                                       //convert string to int

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.hp = stoi(num);                                          //convert string to int

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.attack = stoi(num);                                      //convert string to int

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.defense = stoi(num);

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.spatk = stoi(num);

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.spdefence = stoi(num);

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.speed = stoi(num);

	num = "";
	for(; i < str.size() && str[i] != '\t'; i++)               //if str[i] == '\t' -> one complete data
		num += str[i];
	i++;                                                       //skip '\t'
	num.erase(remove(num.begin(), num.end(), '"'), num.end()); //erase "
	d.generation = stoi(num);


	d.legendary = "";
	for(; i < str.size() && str[i] != '\t'; i++) //if str[i] == '\t' -> one complete data
		d.legendary += str[i];
	i++;
	return s;
}


ostream &operator<<(ostream&s, pokemon d) //department output file stream Overload
{
	s << d.no << '\t';
	s << d.name << '\t';
	s << d.type1 << '\t';
	s << d.type2 << '\t';
	s << d.total << '\t';
	s << d.hp << '\t';
	s << d.attack << '\t';
	s << d.defense << '\t';
	s << d.spatk << '\t';
	s << d.spdefence << '\t';
	s << d.speed << '\t';
	s << d.generation << '\t';
	s << d.legendary;
	return s;
}


class node
{
public:
	int left  = -1;
	int right = -1;
	int child = -1;
	pokemon pok;      //this node's pokemon
	node(pokemon& p)  //constructor:set the pokemon of this node
	{
		pok = p;
	}
};


vector<node> nodevec;  //the tree in a vector form

int			 head		  = -1;
int			 successcount = 0;

void addnode(int& index, int n)
{
	if(index == -1)
	{
		index = n;
		return;
	}
	if(nodevec[index].pok < nodevec[n].pok)
	{
		addnode(nodevec[index].right, n);
		return;
	}
	if(nodevec[index].pok == nodevec[n].pok)
	{
		addnode(nodevec[index].child, n);
		return;
	}
	if(nodevec[index].pok > nodevec[n].pok)
	{
		addnode(nodevec[index].left, n);
		return;
	}
}


void printallnode(int index = head)       //for mission1
{
	if(index == -1)
		return;

	printallnode(nodevec[index].right);
	cout << "[" << setw(3) << ++successcount << "]" << nodevec[index].pok << endl;
	printallnode(nodevec[index].child);
	printallnode(nodevec[index].left);
}

int getthrrheight(int index = head)     //for every mission
{
	if(index == -1)
		return 0;

	return 1 + max(getthrrheight(nodevec[index].left), getthrrheight(nodevec[index].right));
}


int printalllarge(const int&threshold, int index = head)
{
	if(index == -1 || nodevec[index].pok < threshold)
		return 0;

	int count = 1;
	count += printalllarge(threshold, nodevec[index].right);
	cout << "[" << setw(3) << ++successcount << "]" << setw(4) << nodevec[index].pok.no << setw(15) << nodevec[index].pok.name << setw(10) << nodevec[index].pok.type1
		 << setw(10) << nodevec[index].pok.total << setw(10) << nodevec[index].pok.hp << setw(10) << nodevec[index].pok.attack << setw(10) << nodevec[index].pok.defense << endl;
	printalllarge(threshold, nodevec[index].child);
	count += printalllarge(threshold, nodevec[index].left);
	return count;
}


void deletelargest(int&index = head)  //for mission3
{
	if(index == -1)
		return;

	if(nodevec[index].right != -1)
	{
		deletelargest(nodevec[index].right);
		return;
	}
	cout << nodevec[index].pok << endl;
	if(nodevec[index].child != -1)
	{
		nodevec[nodevec[index].child].left = nodevec[index].left;
		index = nodevec[index].child;
		return;
	}
	index = nodevec[index].left;
}


int main()
{
	int k = 100;

	//cout<<std::boolalpha;

	while(k != 0)
	{
		cout << "\n";
		cout << "*** Binary Search Tree on Pokemon ***" << "\n"; //print menu
		cout << "* 0. Quit                           *" << "\n";
		cout << "* 1. Read one file to build BST     *" << "\n";
		cout << "* 2. Threshold search on one column *" << "\n";
		cout << "* 3. Delete the max on one column   *" << "\n";
		cout << "*************************************" << "\n";
		cout << "Input a choice(0, 1, 2, 3):\n";
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
			cout << "Input: \n";
			string locate, inputlocate;
			cin >> locate;
			ifstream input;
			int		 skiplines; // lines to skip in file's begin

			skiplines = 1;

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


			string str;


			for(int i = 0; i < skiplines; i++) //input lines to skip useless information
				getline(input, str);

			// anounce a new vector
			pokemon d;

			nodevec.clear();
			head = -1;
			while(input >> d) //while input available, then input department
				nodevec.push_back(node(d));
			input.close();


			for(int j = 0 ; j < nodevec.size(); j++)
				addnode(head, j);
			cout << "        #           Name    Type 1        HP    Attack   Defense\n";
			//printallnode();
			for(int j = 0 ; j < nodevec.size(); j++)
				cout << "[" << setw(3) << j + 1 << "]" << setw(4) << nodevec[j].pok.no << setw(15) << nodevec[j].pok.name << setw(10) << nodevec[j].pok.type1
					 << setw(10) << nodevec[j].pok.hp << setw(10) << nodevec[j].pok.attack << setw(10) << nodevec[j].pok.defense << endl;

			cout << "HP tree height = " << getthrrheight() << endl;
		}


		if(k == 2)
		{
			int threshold;
			cin >> threshold;
			while(threshold < 1 || threshold > 80)
			{
				cout << "### It is NOT in [1,80]. ###\nTry again:";
				cin >> threshold;
			}
			cout << "        #           Name    Type 1     Total        HP    Attack   Defense\n";
			successcount = 0;
			int k = printalllarge(threshold);
			cout << "Number of visited nodes =" << k << endl;
		}
		if(k == 3)
		{
			if(!getthrrheight())
			{
				cout << "nothing to delete!!!!!!!!\n\n";
				continue;
			}
			cout << "Name	Type 1	Type 2	Total	HP	Attack	Defense	Sp. Atk	Sp. Def	Speed	Generation	Legendary\n";
			deletelargest();
			cout << "HP tree height = " << getthrrheight() << endl;
		}

		if(k == 4)
			for(auto&i:nodevec)
				cout << i.pok << endl;
	}
}
