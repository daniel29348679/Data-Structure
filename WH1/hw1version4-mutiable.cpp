//Dev-C++
#include <bits/stdc++.h>
using namespace std;


int          level;       //the largest recursive level
int          arrivecount; //count the amount about arrived recursive
map <int,int> allowedmap;  //storge the number that can be used


void findnext(vector <int> nowvec)           // nowvec stroged the previous number array
{
    if (nowvec.size() == level)              // if nowvec already had enough number(already went through enough recursive)
    {
        cout << "[" << ++arrivecount << "]"; //print the order of this recursive
        for (auto i:nowvec)                  // print this number array
            cout << " " << i;
        cout << "\n";
        return; //quit this recursive
    }

    for (auto i:allowedmap)                                        //find new number to pushback
        if (count(nowvec.begin(), nowvec.end(), i.first) < i.second) //if the number has not exist in nowvec
        {
            vector <int> t(nowvec.begin(), nowvec.end());          //copy nowvec to new array
            t.push_back(i.first);                                        //pushback the new number
            findnext(t);                                           // recursive
        }
}

int main()
{
    ios_base::sync_with_stdio(false); //sync_with_stdio makes sure the printf and cout won't clash because the other, set it to false to Improve efficiency

    int k = -1;                       //choice number


    while (k != 0)
    {
        cout << "\n";
        cout << "** Permutation Generator **" << "\n"; //print menu
        cout << "* 0. Quit                 *" << "\n";
        cout << "* 1. N numbers from 1..N  *" << "\n";
        cout << "* 2. M numbers from input *" << "\n";
        cout << "* 3. M numbers from 1..9  *" << "\n";
        cout << "***************************" << "\n";
        cout << "Input a choice(0, 1, 2, 3): ";
        cin >> k;

        if (k != 0 && k != 1 && k != 2 && k != 3)
            cout << "\nCommand does not exist!\n";
        if (k == 1)
        {
            cout << "\nInput a number: ";
            cin >> level;                  //N numbers means N recursive level
            while (level < 1 || level > 9) // make sure N is in [1,9]
            {
                cout << "\n### It is NOT in [1,9] ###\n\nInput a number: ";
                cin >> level;
            }
            arrivecount = 0;                 //reset arrivecount
            allowedmap.clear();              //reset allowedmap

            for (int i = 1; i <= level; i++) //put 1,2,3,,,,,N into allowedmap
                allowedmap[i]++;

            findnext({});                                              //start recursive

            cout << "Mission 1: " << arrivecount << " permutations\n"; //print total permutations
            cout << "L = " << level << "\n";                           //print level
        }


        if (k == 2)
        {
            cout << "\nInput a number: ";
            cin >> level;                   //N numbers means N recursive level
            while (level < 2 || level > 9)  // make sure N is in [2,9]
            {
                cout << "\n### It is NOT in [2,9] ###\n\nInput a number: ";
                cin >> level;
            }
            arrivecount = 0;                //reset arrivecount
            allowedmap.clear();             //reset allowedmap

            for (int i = 0; i < level; i++) //scan new number and put into allowedmap
            {
                int t;
                cout << "\nInput a number: ";
                cin >> t;
                allowedmap[t]++;
            }

            int time = clock(); //time storge recursive start at when

            findnext({});       //start recursive


            cout << "Mission 2: " << arrivecount << " permutations\n"; //print total permutations
            cout << "T = " << clock() - time << " ms" << "\n";         //print time consuming
        }

        if (k == 3)
        {
            cout << "\nInput a number: ";
            cin >> level;                  //N numbers means N recursive level
            while (level < 1 || level > 9) // make sure N is in [1,9]
            {
                cout << "\n### It is NOT in [1,9] ###\n\nInput a number: ";
                cin >> level;
            }

            arrivecount = 0;               //reset arrivecount
            allowedmap.clear();            //reset allowedmap
            for (int i = 1; i < 10; i++)
                allowedmap[i]++;   //put 1,2,3,4,5,6,7,8,9 into allowedmap

            int time = clock();

            findnext({});


            cout << "Mission 3: " << arrivecount << " permutations\n"; //print total permutations
            cout << "T = " << clock() - time << " ms" << "\n";         //print time consuming
        }

    }
}
