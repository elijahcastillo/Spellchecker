#pragma once
#include <string>
#include <vector>

// Since we have 26 english letters, we need 26 children per node
#define CHILDREN_PER_NODE 26

struct TrieNode{
  char character;
  bool is_end_of_word;

  //Initialize Chidlren to null
  TrieNode* children[CHILDREN_PER_NODE] = {nullptr};

  TrieNode(char character){
    this->character = character;
    this->is_end_of_word = false;
  }
};


class PrefixTrie {
  TrieNode* root = nullptr;

  // Helper Method to cleanup 
  // allocated data for tree
  void destroy(TrieNode* current);

  // Helper function to recursively find all words with a given prefix
  void find_words(TrieNode* node, std::string currentWord, std::vector<std::string>& words);

public:

  PrefixTrie(){
    this->root = new TrieNode('\0');
  }

  ~PrefixTrie(){
    destroy(root);
  }

  // Inserts a new word into the tree
  // Adds flag to last charatcter indicating an end of word
  bool insert_word(std::string word);

  // Returns a vector of words with a given prefix
  std::vector<std::string> get_words_from_prefix(std::string prefix);
  
  // Checks if word is present in the prefix tree
  bool does_word_exist(std::string word);
};

