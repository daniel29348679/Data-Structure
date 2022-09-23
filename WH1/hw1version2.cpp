//Dev-C++
#include <bits/stdc++.h>
using namespace std;


int          level;       //the largest recursive level
int          arrivecount; //count the amount about arrived recursive
vector <int> allowedvec;  //storge the number that can be used


void findnext(vector <int> nowvec, int already)           // nowvec stroged the previous number array
{
    //int already = count_if(nowvec.begin(), nowvec.end(),[](int x){ return x!=-1;});
    if (already == level)                    // if nowvec already had enough number(already went through enough recursive)
    {
        cout << "[" << ++arrivecount << "]"; //print the order of this recursive
        for (int i = 0 ; i < level ; i++)    // print this number array
            cout << " " << allowedvec[find(nowvec.begin(), nowvec.end(), i) - nowvec.begin()];
        cout << "\n";
        return; //quit this recursive
    }

    for (auto i = nowvec.begin(); i != nowvec.end(); i++)
        if (*i == -1)
        {
            *i = already;
            findnext(nowvec, already + 1);
            *i = -1;
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
            allowedvec.clear();              //reset allowedvec

            for (int i = 1; i <= level; i++) //put 1,2,3,,,,,N into allowedvec
                allowedvec.push_back(i);
            vector <int> firstvec;
            for (int i = 0 ; i < allowedvec.size(); i++)
                firstvec.push_back(-1);
            findnext(firstvec, 0);                                     //start recursive

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
            allowedvec.clear();             //reset allowedvec

            for (int i = 0; i < level; i++) //scan new number and put into allowedvec
            {
                int t;
                cout << "\nInput a number: ";
                cin >> t;

                while (find(allowedvec.begin(), allowedvec.end(), t) != allowedvec.end()) //new number must be new
                {
                    cout << "\n### Duplicate! Still need " << level - i << " numbers! ###\n";
                    cout << "\nInput a number: ";
                    cin >> t;
                }
                allowedvec.push_back(t);
            }

            int          time = clock(); //time storge recursive start at when
            vector <int> firstvec;
            for (int i = 0 ; i < allowedvec.size(); i++)
                firstvec.push_back(-1);
            findnext(firstvec, 0);       //start recursive


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
            allowedvec.clear();            //reset allowedvec
            for (int i = 1; i < 10; i++)
                allowedvec.push_back(i);   //put 1,2,3,4,5,6,7,8,9 into allowedvec

            int          time = clock();
            vector <int> firstvec;
            for (int i = 0 ; i < allowedvec.size(); i++)
                firstvec.push_back(-1);
            findnext(firstvec, 0);


            cout << "Mission 3: " << arrivecount << " permutations\n"; //print total permutations
            cout << "T = " << clock() - time << " ms" << "\n";         //print time consuming
        }
    }
}
