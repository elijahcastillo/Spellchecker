#pragma once

#include "prifixTrie.h"
#include "comparisonAlgorithms.h"
#include <iostream> 
#include <string>
#include <vector>

// Define a structure to hold words and their scores together for sorting 
struct WordScore {
    std::string word;
    double score;
    WordScore(std::string word, double score) : word(word), score(score) {}

    // Comparator for sorting in accending order of score
    // Static allows you to call without createing instance of object first
    static bool compare(const WordScore& a, const WordScore& b) {
        return a.score < b.score;
    }
};

// Scoring based on algorithms
double combinedScore(const std::string& inputWord, const std::string& candidateWord);
std::vector<WordScore> get_best_words(PrefixTrie* pt, const std::string& input);
