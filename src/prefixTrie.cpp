
#include "include/prifixTrie.h"


void PrefixTrie::destroy(TrieNode* current) {
/**
 * Recursivly destory the prefix trie from the first passed in node
 *
 * @param current - Node to recursivly delete from
 */

    // Base Case
    if (current == nullptr) return;

    for (int i = 0; i < CHILDREN_PER_NODE; i++) {
        destroy(current->children[i]); 
    }

    delete current;

}



bool PrefixTrie::insert_word(std::string word){
  /**
 * Insert the given word into the prefix trie
 *
 * @param word - Word to be inserted
 */

  TrieNode* current = root;

  //Loop over each character in the word
  for(int i = 0; i < word.size(); i++){
    char current_character = word[i];

    // Convert character to index in alphabet 1 - 26
    // I guess becuase characters are just ASCII values it works
    int alphabet_index = (int)current_character - 'a'; 

    //Error unregognized character
    if(alphabet_index < 0 || alphabet_index >= 26) return false;


    //Check if character already exists in current children
    if(current->children[alphabet_index] == nullptr){
      //Character does not exist in children so make the node
      current->children[alphabet_index] = new TrieNode(current_character);
    }

    // Move the current pointer to the node containing the current_character
    current = current->children[alphabet_index];

  }

  //We reached the end of the word so mark current node as such
  current->is_end_of_word = true;

  return true;
}


void PrefixTrie::find_words(TrieNode* node, std::string currentWord, std::vector<std::string>& words) {
  /**
   * Recursive helper function to find all words with prefix
   */

    // Base Case
    if (node == nullptr) return;

    //Current Node marks the end of a word
    //But it could also be part of a bigger word
    //so keep looking through its children
    if (node->is_end_of_word) {
        words.push_back(currentWord);
    }

    //Loop through children
    for (int i = 0; i < CHILDREN_PER_NODE; i++) {
        if (node->children[i] != nullptr) {
            char nextChar = 'a' + i; // Convert index back to character (Weird hack I found)
            find_words(node->children[i], currentWord + nextChar, words);
        }
    }
}


std::vector<std::string> PrefixTrie::get_words_from_prefix(std::string prefix){
  /**
 *  Given a prefix return a vector of all of the words stored in the 
 *  prefix trie that start with that prefix
 *
 * @param prefix - What all returned words must start with
 * @return vector<string> - All words with passed in prefix stored in trie
 */
    TrieNode* current = root;
    std::vector<std::string> found_words;

    //Loop over each character in the prefix
    for(int i = 0; i < prefix.size(); i++){
      // Convert character to index in alphabet 1 - 26
      char current_character = prefix[i];
      int alphabet_index = (int)current_character - 'a';

      //Error unregognized character
      if(alphabet_index < 0 || alphabet_index >= 26) return found_words; //Empty Vector


      //Check if character already exists in current children
      if(current->children[alphabet_index] != nullptr){
        current = current->children[alphabet_index];
      }
    }

    find_words(current, prefix, found_words);
    return found_words;

}
  

bool PrefixTrie::does_word_exist(std::string word){
  /**
 *  Deternmines if the word exists inside of the prefix trie
 *
 * @param word - First input string
 * @return bool - Does the word exists in the trie
 */

  TrieNode* current = root;

  //Loop over each character in the word
  for(int i = 0; i < word.size(); i++){
    // Convert character to index in alphabet 1 - 26
    char current_character = word[i];
    int alphabet_index = (int)current_character - 'a';

    //Error unregognized character
    if(alphabet_index < 0 || alphabet_index >= 26) return false;

    //Check if character already exists in current children
    if(current->children[alphabet_index] == nullptr){
      return false; // Character not found in path
    } else {
      current = current->children[alphabet_index];
    }
  }

  return true;

}


