#include <iostream>
#include <bits/stdc++.h>
#include <limits.h>
#include <algorithm>
#include <string>
using namespace std;
typedef multimap<int, string> mmap;
typedef pair<int, string> mmap_pair;
//PROGRAM README
/*This program takes in four "cost" inputs and two words
and uses a dictionary to find the shortest path between them.
The four costs are: Cost to add a letter, delete a letter, change a letter, and take an anagram.
The second input it takes are two words in the dictionary you wish to find the shortest cost between.

Example input:
1 3 1 5
health
hands

Example output:
7

Why:
health-> heath-> heats -> hents-> hends-> hands
So... delete a letter, change a letter, change a letter, change a letter,change a letter
for a cost of 3+1+1+1+1 to do these steps.

It uses dijkstra's to accomplish the shortest path, and I update in real time rather than building the paths first.

*/


bool is_anagram4(const string &s1, const string &s2){
    int arr[256] = {};
    if (s1.length() != s2.length()) return false;
    for(string::size_type i = 0; i < s1.length(); i++)
    {
    arr[(unsigned)s1[i]]++;
    arr[(unsigned)s2[i]]--;
    }
    for(auto i : arr)
    {
    if (i) return false;
    }
    return true;
}



void toLowerCase(string &str, string &str2){
	const int length = str.length();
	for(int i=0; i < length; ++i)
	{
		str[i] = tolower(str[i]);
	}
	const int length2 = str2.length();
	for(int x=0; x < length2; ++x)
	{
		str2[x] = tolower(str2[x]);
	}
}

bool EditDistance(string s1, string s2, int& result, int Costs[]){
    int m = s1.length(), n = s2.length();
    if(m == n){
        if(is_anagram4(s1,s2)){
            result = Costs[3];//ANAGRAM
            return true;
        }
    }
    if (abs(m - n) > 1)
        return false;

    int count = 0; // Count of edits

    int i = 0, j = 0;
    while (i < m && j < n){
        // If current characters don't match
        if (s1[i] != s2[j])
        {
            if (count == 1){
                result = 100;
                return false;
            }

            // If length of one string is
            // more, then only possible edit
            // is to remove a character
            if (m > n){
                i++;
            }
            else if (m< n){
                j++;
            }
            else //Iflengths of both strings is same
            {
                result = Costs[2];//CHANGE
                i++;
                j++;
            }

            // Increment count of edits
            count++;
        }

        else // If current characters match
        {
            i++;
            j++;
        }
    }
    if (i < m || j < n){
        count++;
    }
    if(count == 1){//{addcost,subcost,changecost,anagramcost}
        if(m > n){result = Costs[1];}//SUBBING
        else if(n > m){result = Costs[0];}//ADDING
        return true;
    }
}

mmap LengthMap(string dicttoparse, set<string> &UnVisited){//Process a dictionary into a map of lengths, where each string
    mmap m;
    string word;
    ifstream ifp(dicttoparse);
      while(ifp >> word){//Put each word of our dictionary into a multimap with its length as its key
        m.insert(mmap_pair(word.length(), word));
        UnVisited.insert(word);
    }
    return m;
}

map<string,int> CreateNeighbors(string Word, mmap m, int Costs[]){
    int Cost=0;
    map<string,int> Neighbors;
    int LENGTH_MINUS = Word.length() -1;
    for(int i = 0; i <= 2; i++){
        pair<mmap::iterator, mmap::iterator> range;
        range = m.equal_range(LENGTH_MINUS+i); //SET OUR RANGE UP USING OUR LENGTH TO CHECK FOR ANAGRAMS ECT
        for(mmap::iterator it = range.first; it != range.second; ++it){//FOR EACH OF OUR STRINGS THAT COULD BE ONE EDIT AWAY
                string CHILDRENTOCHECK = it->second;
                if(Word != CHILDRENTOCHECK){
                    if(EditDistance(Word,CHILDRENTOCHECK,Cost,Costs)){
                        Neighbors.insert (pair<string,int>(CHILDRENTOCHECK,Cost));
                    }
                }
        }
    }
    return Neighbors;
}


int RunDjikstras(string StartingWord, string EndingWord, int Costs[],set<string> &UnVisited, mmap m){

    //Variable Prep
    int MinimumDistance = INT_MAX;
    unordered_map<string,int> NodeAndCost;//Tracks each node and its current minimal distance
    //set<string> UnVisited;
    set<string> Visited;//Make our visited nodes tracker
    set<string> Checked;

    //Assign our first Node to be 0 and visit it
    NodeAndCost.insert(pair<string,int>(StartingWord,0));//Assign 0 to our current NODE
    Visited.insert(StartingWord);//Add our first word to our visited set.
    UnVisited.erase (StartingWord);//Remove our first word from our unvisited set
    string CurrentWord = StartingWord;//Set up our CurrentWord for future use

        while(true){
            string LoopStartingWord = CurrentWord;
            //Consider all the current node unvisited neighbors and calculate distance to current node+distance to each neighbor
            //If this is less than their current tentative distance
            //Replace it with a new value
            ///START OF FUNCTION
            map<string,int> Neighbors = CreateNeighbors(CurrentWord, m, Costs);//So now set has all our neighbors
            //Find unvisited neighbors
            for(auto& sm_pair : Neighbors){
                string CurrentNeighbor = sm_pair.first;
                int CurrentCost = sm_pair.second;
                if(UnVisited.count(CurrentNeighbor) > 0){//We have an unvisited neighbor
                    if(Checked.count(CurrentNeighbor)==0){
                        Checked.insert(CurrentNeighbor);
                        NodeAndCost[CurrentNeighbor] = INT_MAX;
                    }
                    if(NodeAndCost[CurrentWord]+CurrentCost < NodeAndCost[CurrentNeighbor]){
                        //If our new path is better than our old path to our current node
                        NodeAndCost[CurrentNeighbor] = NodeAndCost[CurrentWord] + CurrentCost;
                    }
                }
            }

            //Mark current node as visited
            //Remove from unvisited set
            Visited.insert(CurrentWord);//Add our current to our visited set.
            UnVisited.erase (CurrentWord);//Remove our current from our unvisited set

            //If the destination has been marked visited, finish
            if(Visited.count(EndingWord) != 0){
                int DistanceToEnd = NodeAndCost.at(EndingWord);
                cout << "Shortest path takes: " << DistanceToEnd << endl;
                return DistanceToEnd;
            }

            //Set the unvisited node with the smallest tentative distance as the next current node and go back to 3
           int Min = INT_MAX;
           for(auto& sm_pair : NodeAndCost){
                string CurrentBestWord = sm_pair.first;
                int CurrentWordBestCost = sm_pair.second;
                if(CurrentBestWord != CurrentWord && UnVisited.count(CurrentBestWord) > 0 && CurrentWordBestCost < Min){

                    Min = CurrentWordBestCost;
                    CurrentWord = CurrentBestWord;
                }
           }
           if(CurrentWord == LoopStartingWord){
                return -1;
           }
        }
}


int main()
{
    //Prepare necessary storage for useful information to make our algorithm run
    set<string>Unvisited;
    int addcost; int subcost; int changecost; int anagramcost;
    string Start; string End;

    //Replace 20k.txt with whatever dictionary you are using, place it in the same folder
    mmap m = LengthMap("20k.txt", Unvisited);

    //Take in our input//
    cin>> addcost >> subcost >> changecost >> anagramcost >> Start >> End;
    if(cin.fail()){//Didn't obey the rules, proper input is four ints and two string
        cout << "\nInvalid input per specified parameters! Please run me again.\nInput is four ints representing costs followed by two strings.\n";
    }
    else if((Start.find_first_not_of( "0123456789" ) == string::npos) || (End.find_first_not_of( "0123456789" ) == string::npos)){//Numbers aren't in the dictionary, at least in mine
        cout << "\nOne of your inputs is a number!\nThat's not fair.\n";
    }
    else{//Valid input
        toLowerCase(Start, End); //Handle capitals
        int Costs[] = {addcost,subcost,changecost,anagramcost};//Simplify our costs a bit
        return RunDjikstras(End, Start, Costs, Unvisited, m);//Run Djikstra's algorithm
    }
}
