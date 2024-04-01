#pragma once
#include <string> 
#include <vector> 
#include <cstring> // memset
#include <algorithm>
#include <cmath>
#define MAX_WORD_LEN 15

// Algorithms
int levenshtine_distance(std::string word1, std::string word2);
int LCS(const std::string& str1, const std::string& str2);
double cosineSimilarityScore(const std::string& input, const std::string& test);



