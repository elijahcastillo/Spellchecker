#include "include/score.h"


double combinedScore(const std::string& inputWord, const std::string& candidateWord) {
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
    std::sort(word_scores.begin(), word_scores.end(), WordScore::compare);

    return word_scores;
}
