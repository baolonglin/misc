#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <cstdlib>
#include <set>

bool word_include(const std::string& word, const std::string& chars)
{
  std::map<char, int> char_map;
  for(int i = 0; i < chars.length(); i++) {
    char_map[chars[i]]++;
  }
  for(int i = 0; i < word.length(); i++) {
    std::map<char,int>::iterator iter = char_map.find(word[i]);
    if(iter == char_map.end() || iter->second == 0) {
      return false;
    }
    iter->second--;
  }
  return true;
}

bool two_word_in(const std::string& word1, const std::string& word2, const std::string& chars)
{
  std::map<char, int> char_map;
  for(int i = 0; i < chars.length(); i++) {
    char_map[chars[i]]++;
  }

  std::map<char, int> word_map;
  for(int i = 0; i < word1.length(); i++) {
    word_map[word1[i]]++;
  }
  for(int i = 0; i < word2.length(); i++) {
    word_map[word2[i]]++;
  }

  return char_map == word_map;
}

void get_reachable_map(const std::string& chars, std::map<char, std::set<char> >& reachable_map)
{
  /*
   0 [1, 3, 4]
   1 [0, 2, 3, 4, 5]
   2 [1, 4, 5]
   3 [0, 1, 4, 6, 7]
   4 [0, 1, 2, 3, 5, 6, 7, 8]
   5 [1, 2, 4, 7, 8]
   6 [3, 4, 7]
   7 [3, 4, 5, 6, 8]
   8 [4, 5, 7]
[][][]
[][][]
[][][]
  */
  int k = 0;
  char reg[3][3];
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      reg[i][j] = chars[k];
      k++;
    }
  }

  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      char curr_char = reg[i][j];
      if(j+1 < 3) {
        reachable_map[curr_char].insert(reg[i][j+1]);
      }
      if(i+1 < 3) {
        reachable_map[curr_char].insert(reg[i+1][j]);
      }
      if(j+1 < 3 && i+1 < 3) {
        reachable_map[curr_char].insert(reg[i+1][j+1]);
      }
      if(i-1 >= 0) {
        reachable_map[curr_char].insert(reg[i-1][j]);
      }
      if(j-1 >= 0) {
        reachable_map[curr_char].insert(reg[i][j-1]);
      }
      if(i-1 >= 0 && j-1 >= 0) {
        reachable_map[curr_char].insert(reg[i-1][j-1]);
      }
      if(i-1 >=0 && j+1 < 3) {
        reachable_map[curr_char].insert(reg[i-1][j+1]);
      }
      if(i+1 < 3 && j-1 >= 0) {
        reachable_map[curr_char].insert(reg[i+1][j-1]);
      }
    }
  }
  
}

bool is_reachable(const std::string& word, const std::map<char, std::set<char> >& reachable_map)
{
  for(int i = 0; i < word.length() - 1; i++) {
    std::map<char, std::set<char> >::const_iterator iter = reachable_map.find(word[i]);
    if(iter->second.find(word[i+1]) == iter->second.end()) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> filter_word(const std::vector<std::string>& words,
                 const std::string& chars)
{
  std::map<char, std::set<char> > reachable_map;
  get_reachable_map(chars, reachable_map);

  for(std::map<char, std::set<char> >::iterator iter = reachable_map.begin();
      iter != reachable_map.end(); ++iter) {
    std::cout << iter->first << ":";
    for(std::set<char>::iterator i2 = iter->second.begin(); i2 != iter->second.end(); ++i2) {
      std::cout << *i2 << " ";
    }
    std::cout << std::endl;
  }
  std::vector<std::string> valid_word;
  for(int i = 0; i < words.size(); i++) {
    if(is_reachable(words[i], reachable_map)) {
      valid_word.push_back(words[i]);
    }
  }

  return valid_word;
}

void filter_result(const std::vector<std::string>& first_word_matched,
                   const std::vector<std::string>& second_word_matched,
                   const std::string& chars)
{
  for(std::vector<std::string>::const_iterator iter = first_word_matched.begin(); iter != first_word_matched.end(); ++iter) {
    std::cout << *iter << ' ';
  }
  std::cout << std::endl;

  for(std::vector<std::string>::const_iterator iter = second_word_matched.begin(); iter != second_word_matched.end(); ++iter) {
    std::cout << *iter << ' ';
  }
  std::cout << std::endl;

  std::vector<std::string> filtered_first_word = filter_word(first_word_matched, chars);
  for(std::vector<std::string>::const_iterator iter = filtered_first_word.begin(); iter != filtered_first_word.end(); ++iter) {
    std::cout << *iter << ' ';
  }
  std::cout << std::endl;

  for(std::vector<std::string>::const_iterator iter = filtered_first_word.begin(); iter != filtered_first_word.end(); ++iter) {
      for(std::vector<std::string>::const_iterator iter2 = second_word_matched.begin(); iter2 != second_word_matched.end(); ++iter2) {
        if(two_word_in(*iter, *iter2, chars)) {
          std::cout << "Solved!! first word: " << *iter << " second word: " << *iter2 << std::endl;
          break;
        }
      }
  }

  
}

int main(int argc, char* argv[])
{
  std::vector<std::string> db;
  std::ifstream infile("words.txt");
  std::string line;
  while (std::getline(infile, line)) {
    db.push_back(line);
  }

  std::string chars = argv[1];
  int word_num = atoi(argv[2]);
  int first_word_char_num = atoi(argv[3]);
  int second_word_char_num = atoi(argv[4]);

  std::vector<std::string> first_word_matched;
  std::vector<std::string> second_word_matched;
  for(int i = 0; i < db.size(); i++) {
    int len = db[i].length();
    if(len == first_word_char_num) {
      if(word_include(db[i], chars)) {
        first_word_matched.push_back(db[i]);
      }
    }
    if(len == second_word_char_num) {
      if(word_include(db[i], chars)) {
        second_word_matched.push_back(db[i]);
      }
    }
  }

  filter_result(first_word_matched, second_word_matched, chars);
}
