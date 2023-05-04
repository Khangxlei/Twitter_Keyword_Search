#include<iostream>
#include<vector>
#include<map>
#include<string>
#include <unordered_set>
using namespace std;

class InvertedIndex {
  public:
    map<string, vector<int>> Dictionary; //Word associated with tweet numbers
    map<int, int> similarity_score; //Tweet number associated with score
    map<int, string> tweet; //Tweet number associated with tweet itself
    vector<string> filelist;
  
    void addfile(string filename);
    void create_list(string word, unordered_set<int>& tweets);
    void search(string word);
    vector<int> topten(vector<int> list);
    vector<int> relevant_tweets(vector<int> list); //display all relevant tweets in order
};