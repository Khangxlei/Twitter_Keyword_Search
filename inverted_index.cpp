#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<fstream>
#include<sstream>
#include <unordered_set>
#include "inverted_index.h"

using namespace std;

string toRawString(std::string const& in){
   std::string ret = in;
   auto p = ret.find('\t');
   if ( p != ret.npos )
   {
      ret.replace(p, 1, "\\t");
   }

   return ret;
}

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
    stringstream s(toRawString(input_line));
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
    result.insert(result.begin() + ind, list[i]);
  }
  return result;
}

// Function creates a list of tweets that contains the word, as well as setting up the similarity scores

// MAKE TWEETS AN UNORDERED SET SO IT DOESN'T HAVE DUPLICATES
void InvertedIndex::create_list(string word, unordered_set<int>& tweets) {
  for (unsigned int i=0; i < Dictionary[word].size(); ++i){
    tweets.insert(Dictionary[word][i]);
    similarity_score[Dictionary[word][i]] += 1;
  }
}