#include "include/comparisonAlgorithms.h"

// 2D Array to keep track of subproblems
// Pre-allocated for speed
int dp_array[MAX_WORD_LEN][MAX_WORD_LEN] = {0};


int levenshtine_distance(std::string word1, std::string word2){

  // Exmaple for strings 'car' & 'cat'
  // ---------------------------------
  //     ""  c   a   r
  //  "" 0   1   2   3  <-- Row is initialized at start
  //  c  1   0   1   2
  //  a  2   1   0   1
  //  t  3   2   1   1  <-- This last value is the edit distance
  //     ^
  //    Column is Initialized at start

  // Reset 2d array back to all 0's
  memset(dp_array, 0, sizeof(dp_array));

  // Initialize the first row and column of the array
  // representing the number of operations needed to match an empty string with substrings of `word1` and `word2`.
  for (int i = 0; i <= word1.size(); i++) {
    dp_array[i][0] = i; // Cost of deletions from word1 to match an empty string
  }
  for (int j = 0; j <= word2.size(); j++) {
    dp_array[0][j] = j; // Cost of insertions into an empty string to match word2
  }

  // Iterate over each character of both strings
  for (int idx1 = 1; idx1 <= word1.size(); idx1++) {
    for (int idx2 = 1; idx2 <= word2.size(); idx2++) {

      // If current characters match, no operation is needed; take the previous best result.
      if (word1[idx1 - 1] == word2[idx2 - 1]) {
        dp_array[idx1][idx2] = dp_array[idx1 - 1][idx2 - 1];
      } else {
        // Calculate costs of the 3 operation tha you could make to a single charcter operations
        const int replaceCost = dp_array[idx1 - 1][idx2 - 1]; 
        const int insertCost = dp_array[idx1][idx2 - 1];
        const int deleteCost = dp_array[idx1 - 1][idx2]; 

        // The cost of the current operation is 1 plus the minimum of the three costs
        dp_array[idx1][idx2] = 1 + std::min({replaceCost, insertCost, deleteCost});
      }
    }
  }

  // The final cell in the bottom right corner of 2d array is the Levenshtein distance between the strings
  return dp_array[word1.size()][word2.size()];
}




int LCS(const std::string& str1, const std::string& str2) {

  // Example 'cat' & 'hat'
  // -----------------------------------
  //     ""  c   a   t
  //  "" 0   0   0   0  <-- Row for LCS with "" and prefixes of cat
  //  h  0   0   0   0  <-- 'h' does not match any letter in cat so carry over the previous values
  //  a  0   0   1   1  <-- 'a' matches in both increment diagonal by 1
  //  t  0   0   1   2  <-- 't' matches in both increment diagonal by 1

  // Reset 2d array back to all 0's
  memset(dp_array, 0, sizeof(dp_array));

  // Loop through all characters of str1 and str2
  for (size_t i = 1; i <= str1.size(); ++i) {
      for (size_t j = 1; j <= str2.size(); ++j) {

          // If we find matching letters in both words at our current position,
          // we add 1 to our score because we've found a common part.
          // We look diagonally back in our scoreboard for the previous score,
          // and add one to that.
          if (str1[i - 1] == str2[j - 1]) {
            dp_array[i][j] = dp_array[i - 1][j - 1] + 1;
          } else {
            // If the letters don't match, we take the best score from either
            // just above or to the left of our current position.
            // This means we either skip a letter from word1 or word2 and see
            // which option keeps our score higher (more matching found so far)
            dp_array[i][j] = std::max(dp_array[i - 1][j], dp_array[i][j - 1]);
          }
      }
  }

  // The bottom-right cell contains the length of the LCS
  return dp_array[str1.size()][str2.size()];
}


std::pair<std::vector<int>, std::vector<int>> convertWordsToVector(const std::string& str1, const std::string& str2){
    // Get max lengh of both words
    int vec_length = std::max({str1.size(), str2.size()});
    
    std::vector<int> vec1;
    std::vector<int> vec2;
    
    // Create vector for str1
    for(int i = 0; i < vec_length; i++){
       if(i < str1.size()){
           vec1.push_back((int)str1[i]);
          continue;
       }

       vec1.push_back(0);
    }

    // Create vector for str2
     for(int i = 0; i < vec_length; i++){
 
        if(i < str2.size()){
           vec2.push_back((int)str2[i]);
           continue;
       }

       vec2.push_back(0);
    }

    return {vec1, vec2};    
}

double vectorMagnitude(std::vector<int>& vec){
  int sumOfSquare = 0;
  for(int i = 0; i < vec.size(); i++){
    sumOfSquare += vec[i] * vec[i];
  }
  return sqrt(sumOfSquare);
}


int dotProduct(std::vector<int>& vec1, std::vector<int>& vec2){
  int dotProduct = 0;
  for(int i = 0; i < vec1.size(); i++){
    dotProduct += vec1[i] * vec2[i];
  }

  return dotProduct;
}


double cosineSimilarity(std::vector<int>& vec1, std::vector<int>& vec2) {
  int dot = dotProduct(vec1, vec2);
  double mag1 = vectorMagnitude(vec1);
  double mag2 = vectorMagnitude(vec2);

  // Ensure magnitudes are not zero to avoid division by zero errors
  if (mag1 == 0 || mag2 == 0) return 0;

  return dot / (mag1 * mag2);
}

// Returns the cosine of the angle between the two word vectors
double cosineSimilarityScore(const std::string& input, const std::string& test){
  if (input == "" || test == "") return 0; // No empty strings
  std::pair<std::vector<int>, std::vector<int>> vectors = convertWordsToVector(input, test);
  double similarity = cosineSimilarity(vectors.first, vectors.second);
  return similarity;
}


