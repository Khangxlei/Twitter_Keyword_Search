#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<sstream>
#include <unordered_set>
#include <algorithm>

using namespace std;

class InvertedIndex {
  public:
    map<string, vector<int>> Dictionary; //Word associated with tweet numbers
    map<int, int> similarity_score; //Tweet number associated with score
    map<int, string> tweet; //Tweet number associated with tweet itself
    vector<string> filelist;

    //for better similarity computations, we should use a better algorithm that compares two sentences with each other.
    // we compare each tweets by accessing it with the tweet map with the search phrase, and put the score into 
    // similarity_score map. 

    //https://stackoverflow.com/questions/955110/similarity-string-comparison-in-java

  
    void addfile(string filename);
    void create_list(string word, vector<int>& tweets);
    void search(string word);
    vector<int> topten(vector<int> list);
    vector<int> relevant_tweets(vector<int> list); //display all relevant tweets in order
};

void InvertedIndex::addfile(string filename) {
  ifstream input;
  input.open(filename,ios::in);

  if(!input){
    cout<<"File Not Found\n";
    return;
  }

  string input_line;
  string word;
  int tweet_no = 0;

  while(getline(input, input_line)) {
    stringstream s(input_line);
    tweet_no++;
    similarity_score[tweet_no] = 0; //setting similarity score to 0
    tweet[tweet_no] = input_line; //setting the tweet number to its actual tweet

    while(s>>word){
      Dictionary[word].push_back(tweet_no);
    }
  }
  input.close();
}

// Function calculates top 10 most similar tweets given a vector of all the tweets that contains at least a similarity score of 1
vector<int> InvertedIndex::topten(vector<int> list){
  vector<int> top_ten;

  for (unsigned int i=0; i< list.size(); ++i){
    if (top_ten.size() == 0) top_ten.push_back(list[i]);
    
    else if (top_ten.size() < 10){
      int index = 0;
      for (unsigned int j=0; j<top_ten.size(); ++j){
        if (similarity_score[list[i]] < similarity_score[top_ten[j]]) break;
        index++;
      }

      top_ten.insert(top_ten.begin() + index, list[i]);
    }
    
    else {
      if (similarity_score[list[i]] < similarity_score[top_ten[0]]) continue;
      else {
        int index = 0;
        for (unsigned int j=0; j<top_ten.size(); ++j){
          if (similarity_score[list[i]] < similarity_score[top_ten[j]]) break;
          index++;
        }

        top_ten.insert(top_ten.begin() + index, list[i]);
        top_ten.erase(top_ten.begin());
      }
    }
  }
  return top_ten;
}

vector<int> InvertedIndex::relevant_tweets(vector<int> list){
  vector<int> result; //vector will contain all relevant tweets in order, starting from least to most
  
  for (unsigned int i=0; i < list.size(); ++i){
    int ind = 0;
    for (unsigned int j=0; j < result.size(); ++j){
      if (similarity_score[list[i]] < similarity_score[result[j]]) break;
      ind++;
    }
    //cout << "ind: " << ind << endl;
    result.insert(result.begin() + ind, list[i]);
  }
  return result;
}

// Function creates a list of tweets that contains the word, as well as setting up the similarity scores

// MAKE TWEETS AN UNORDERED SET SO IT DOESN'T HAVE DUPLICATES
void InvertedIndex::create_list(string word, vector<int>& tweets) {
  for (unsigned int i=0; i < Dictionary[word].size(); ++i){
    tweets.push_back(Dictionary[word][i]);
    similarity_score[Dictionary[word][i]] += 1;
  }
}

void parse_input (string& input, vector<string>& result){
  
  string word;

  for (auto x: input){
    if (x == ' '){
        if (word.size() > 0) result.push_back(word);
        word = "";
    }
    else{
        word = word + x;
    }
  }
  result.push_back(word);
}


int main(int argc, char*argv[]) {
  InvertedIndex Data;
  Data.addfile(argv[1]);

  // user inputs
  vector<string> input_words;
  string input;
  cout << "Please enter search: ";
  getline(cin, input);

  // parses the keywords that the user inputs into vectors of words
  parse_input(input, input_words);

  // go through the user's query of keywords and add it into 2 separate maps, one for similarity score associated with tweet
  // number and the other for the tweet itself associated with tweet number. 
  // tweets vector will hold the tweet numbers that shares similarities with at least one word with the user's query
  vector<int> tweets;
  for (unsigned int i=0; i<input_words.size(); ++i){
    Data.create_list(input_words[i], tweets);
  }

  // add all the tweets that shares similarities with the user's query into a set to remove duplicates
  unordered_set<int> tweets_set;
  for (unsigned int i=0; i <tweets.size(); ++i){
    tweets_set.insert(tweets[i]);
  }

  // add everything from the set back into the vector for easy iteration and data access
  vector<int> tweets_non_dup;
  unordered_set<int>::iterator it;
  for (it = tweets_set.begin();  it != tweets_set.end(); ++it){
    tweets_non_dup.push_back(*it);
  }

  // relevant vector will hold all the tweets in ascending order of similarity score
  vector<int> relevant;
  relevant = Data.relevant_tweets(tweets_non_dup);

  // prints out top 10 similar tweets
  cout << "Printing Top 10 Similar Tweets:" << endl;
  int i = relevant.size();
  unsigned int count = 0;
  while (i >= 0 && count < 10){
    cout << Data.tweet[relevant[i]] << endl;
    count++;
    i--;
  }
  return 0;
}