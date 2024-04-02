#include "include/score.h"
#include "include/heapsort.h"


double combinedScore(const std::string& inputWord, const std::string& candidateWord) {
/**
 *  Given two input strings, give a score based on different algorithms that
 *  shows how similar they are to each other
 *
 *
 * @param inputWord - First input string
 * @param candidateWord - Second input string
 * @return double - Similarity score of the words
 */


    /* const int lcsScore = LCS(inputWord, candidateWord); */
    const double levDistance = levenshtine_distance(inputWord, candidateWord);
    const double cosineSimScore = cosineSimilarityScore(inputWord, candidateWord);

    // Weights to show how much we want each comparison to effect the score
    /* const double lcsWeight = 1.0; // Adjust based on relative importance */
    const double levWeight = 2.0; // Higher weight penalizes distance more
    const double cosineWeight = 1.5; // Adjust to emphasize semantic similarity

    // Calculate combined score
    /* double score = (lcsWeight * lcsScore) - (levWeight * levDistance) + (cosineWeight * cosineSimScore); */


    double score = (levWeight * levDistance) - (cosineWeight * cosineSimScore);

    return score;
}


std::vector<WordScore> get_best_words(PrefixTrie* pt, const std::string& input) {
/**
 * Given an input word, using hte prefix trie narrow down the words to look through
 * give each word a score based on how similar it is to the input string
 * sort by those given scores
 *
 * It narrows down the words by only getting the words that start with either
 * of the first 2 letters of the input string
 *
 *
 * @param pt - Prefix Trie containing all the words from the loaded dictionary
 * @param input - Word to comparse against 
 * @return std::vector<WordScore> sorted list of words by how close they are to the input
*/
    
    std::vector<std::string> found_words;
    std::vector<WordScore> word_scores;

    if(input.empty()){
      return word_scores; // Empty input
    }


    std::string prefix = input.substr(0, 1);


    // Check the original prefix
    std::vector<std::string> originalWords = pt->get_words_from_prefix(prefix);
    found_words.insert(found_words.end(), originalWords.begin(), originalWords.end());

    // Check an alternative prefix using the next letter in the input
    if (input.size() > 1) {
        char nextLetter = input[1];
        std::string alternativePrefix = std::string(1, nextLetter);
        std::vector<std::string> alternativeWords = pt->get_words_from_prefix(alternativePrefix);
        found_words.insert(found_words.end(), alternativeWords.begin(), alternativeWords.end());
    }

    // Calculate scores for each found word
    for (auto& candidateWord : found_words) {
        double score = combinedScore(input, candidateWord);
        word_scores.emplace_back(candidateWord, score);
    }

    // Sort the vector of WordScore by the score in descending order
    Sort::heapSort(word_scores);

    return word_scores;
}
