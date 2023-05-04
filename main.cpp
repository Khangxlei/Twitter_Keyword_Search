#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<sstream>
#include <unordered_set>
#include <algorithm>

#include "inverted_index.h"

using namespace std;




// parses string of a sentence into vector of words
vector<string> parse_input (string& input){
  vector<string> result;
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
  return result;
}


int main(int argc, char*argv[]) {

  InvertedIndex Data;


  // user inputs
  string input;
  cout << "Please enter input file name: ";
  getline(cin, input);
  Data.addfile(input);

  string outputfile_name = "output_" + input;
  ofstream outfile;
  outfile.open(outputfile_name);
  
  vector<string> input_words;
  
  cout << "Please enter search: ";
  getline(cin, input);
  transform(input.begin(), input.end(), input.begin(), ::tolower);

  // parses the keywords that the user inputs into vectors of words
  input_words = parse_input(input);

  
  unordered_set<int> tweets_set; // hold the tweet numbers that shares similarities with at least one word with the user's query
  for (unsigned int i=0; i<input_words.size(); ++i){
    Data.create_list(input_words[i], tweets_set); // go through the user's query of words and add it into 2 separate maps, one for tweet number: similarity score
                                                  // and the other for the tweet number: tweet. 
  }

  outfile << "\nTweet IDs that contains at least one word in the given query" << endl;
  for ( unordered_set<int>::iterator it = tweets_set.begin(); it != tweets_set.end(); ++it){
    outfile << *it << ' ';
  }
  outfile << "\n\n";

  // add everything from the set back into the vector for easy iteration and data access
  vector<int> tweets_non_dup;
  unordered_set<int>::iterator it;
  for (it = tweets_set.begin();  it != tweets_set.end(); ++it){
    tweets_non_dup.push_back(*it);
  }

  outfile << "\nTweet IDs associated with their similarity scores" << endl;
  for (unsigned int i=0; i<tweets_non_dup.size(); ++i){
    outfile << "ID: " << tweets_non_dup[i] << "  Similarity Score: " << Data.similarity_score[tweets_non_dup[i]] << endl;
  }
  outfile << "\n\n";

  vector<int> relevant; // hold all the tweets in ascending order of similarity score
  relevant = Data.relevant_tweets(tweets_non_dup); // calculates and sorts the tweets in ascending order in terms of similarity score

  // prints out top 10 similar tweets
  outfile << "Printing Top 10 Similar Tweets:" << endl;
  int i = relevant.size()-1;
  unsigned int count = 0;
  while (i >= 0 && count < 10){
    outfile << count +1 << ". ";
    outfile << Data.tweet[relevant[i]] << endl;
    count++;
    i--;
  }
  return 0;
  
}