#include "include/features.h"
#include "include/prifixTrie.h"
#include "include/score.h"
#include <algorithm>
#include <iomanip>
#include <string>
#include <filesystem>

#ifdef _WIN32
#include "include/windowsTerminal.h"
#endif 

namespace fs = std::filesystem;


std::string toLowerCase(const std::string& input) {
    std::string result;
    for (char ch : input) {
        result += std::tolower((unsigned char)ch); 
    }
    return result;
}

std::string cleanWord(std::string word) {

    //  So like apparently containers have a .begin and .end which can be used 
    //  kinda like iterators for algorithms in the std lib which is nice if you ask me
    //  also why do anonymous functions need the [] at the front? I'll have to look into it
    //  This just removes and punctuation from a string becuase our spellechker does not support it
    word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char ch) {
        return std::ispunct(ch);
    }), word.end());
    return toLowerCase(word);
}


void print_file_accuracy(const fs::path& filePath, PrefixTrie* pt) {
    unsigned int totalWords = 0, misspelledWords = 0;

    // Read in the input file
    std::fstream in_file(filePath, std::fstream::in);
    if (!in_file.is_open()) {
        std::cout << RED << "File could not open input file: " << filePath.generic_string() << RESET << std::endl;
        return;
    }

    // Count the words and misspellings
    std::string currentWord;
    while (in_file >> currentWord) {
        totalWords++;
        currentWord = cleanWord(currentWord);
        if (!pt->does_word_exist(currentWord)) misspelledWords++;
    }

    double percentMisspelled = totalWords > 0 ? 100.0 * misspelledWords / totalWords : 0;

    // Display Results to User
    std::cout << CYAN << "File: " << filePath.generic_string() << RESET << "\n";
    std::cout << std::setfill('=') << std::setw(filePath.generic_string().size() + 6 + 4) << "\n";
    std::cout << YELLOW << "Total Words: " << totalWords << RESET << "\n";
    std::cout << RED << "Misspelled Words: " << misspelledWords << RESET << "\n";
    std::cout << GREEN << "Accuracy: " << (100 - percentMisspelled) << "%" << RESET << "\n\n";
}

void start_file_accuracy(PrefixTrie* pt, std::string user_path){
  fs::path path = user_path;


  try {
    if(!fs::exists(path)){
      std::cout << RED << "ERROR: " << RESET << "Path '" << RED << path.generic_string() << RESET << "' does not exist";
      return;
    }

        if (fs::is_directory(path)) {
            // Iterate over each item in the directory and process if it's a file
            for (const auto& entry : fs::directory_iterator(path)) {
                if (fs::is_regular_file(entry)) {
                    print_file_accuracy(entry.path(), pt);
                }
            }
        } else if (fs::is_regular_file(path)) {
            // Process a single file
            print_file_accuracy(path, pt);
        } else {
            std::cout << "Provided path is neither a file nor a directory.\n";
        }


  } catch (fs::filesystem_error& e) {
      std::cout << RED << "ERROR: " << RESET << e.what() << "\n";
  
  }
}


void start_repl(PrefixTrie* pt){
  while(true){
    std::string input = "";
    std::cout << BOLD_CYAN << "> " << RESET;
    std::cin >> input;


    //Convert input to lowercase
    input = toLowerCase(input);

    // Check for exit command
    if (input == "q" || input == "exit") {
        std::cout << CYAN << "Exiting program." << RESET << std::endl;
        break;
    }


    //Check if word is already correct
    if(pt->does_word_exist(input)){
      std::cout << GREEN << "Word found in dictionary" << RESET << std::endl;
    }


    std::vector<WordScore> word_scores = get_best_words(pt, input);

    int count = 0;
    for (auto &ws : word_scores) {
        if (count++ >= 10) break; // Limit to top suggestions
        std::cout << YELLOW << ws.word << " " << ws.score <<  RESET << "\n";
    }
  }

}




void file_scan_prompt(std::string word, std::vector<WordScore>& word_scores, int word_score_offset, std::queue<std::string>& context) {
    std::cout << BOLD_RED << "Potential Misspelling: " << RESET;

     // Display context
    std::queue<std::string> tempQueue = context;
    while (!tempQueue.empty()) {
        std::cout << tempQueue.front();
        tempQueue.pop();
    } 

    std::cout << RED <<  "'" << BOLD_YELLOW <<   word << RED << "'" << RESET << "\n";
    std::cout << BOLD_CYAN << "s<Skip> r<Retry> #<Replace> q<Quit>\n" << RESET;

    // Show word recomendations
    for (int i = word_score_offset; i < word_score_offset + 5 && i < word_scores.size() - 1; i++) {
        std::cout << BOLD_MAGENTA << i - word_score_offset << ". " << RESET << word_scores[i].word << RESET << "\n";
    }

    // Show prompt
    std::cout << BOLD_CYAN << "> " << RESET;
}


std::string file_scan_input_loop(const std::string& currentWord, std::vector<WordScore>& word_scores, int& offset, std::queue<std::string>& context) {
    std::string input;

    // Debug print to show the contents of the context_queue
    /* std::cout << "Context Queue: "; */
    /* std::queue<std::string> tempQueue = context; */
    /* while (!tempQueue.empty()) { */
    /*     std::cout << tempQueue.front(); */
    /*     tempQueue.pop(); */
    /* } */
    /* std::cout << "\n"; */
    
    do {
        file_scan_prompt(currentWord, word_scores, offset, context);
        std::cin >> input;
        
        if (input == "s") {
            // Skip
            break;
        } else if (input == "q") {
            // Quit
            std::cout << CYAN << "Exiting program." << RESET << std::endl;
            exit(1);
        } else if (input == "r") {
            offset += 5;  // Move to the next set of suggestions
        } else {
            // Takes input from user
            break;
        }
    } while (true);

    std::cout << "\n";

    return input;
}




void updateContextQueue(std::queue<std::string>& context_queue, const std::string item) {

    // Keep context queue at a ceratin size so that it does
    // not fill the screen
    context_queue.push(item);
    if (context_queue.size() > CONTEXT_QUEUE_SIZE) {
        context_queue.pop();
    }
}

void processWord(std::ofstream& out_file, PrefixTrie* pt, std::string& word, std::queue<std::string>& context_queue) {
    if (word.empty()) {
        return;
    }


    // Convert word to lowercase so no false positives
    std::string lowercaseWord = toLowerCase(word);

    // Only prompt for words not in dictionary
    if (!pt->does_word_exist(lowercaseWord)) {
        //Keep track of offset for recomeded words
        int recommended_offset = 0;

        // Grab best word recommendations
        std::vector<WordScore> word_scores = get_best_words(pt, lowercaseWord);

        // Promt user for action
        std::string input = file_scan_input_loop(word, word_scores, recommended_offset, context_queue);
        bool has_only_digits = (input.find_first_not_of( "0123456789" ) == std::string::npos);

        if (has_only_digits) {
            // User picked word reccomendation from list
            int recommendation_index = std::stoi(input);
            std::string recommended_word = word_scores[recommended_offset + recommendation_index].word; 
            out_file << recommended_word; 
            updateContextQueue(context_queue, recommended_word);
        } else if (input == "s") {
            // User skipped mispelling
            out_file << word;
            updateContextQueue(context_queue, word);
        } else {
             // Otherwise use input from user for correction
            out_file << input;
            updateContextQueue(context_queue, input);
        }

    } else {
        //Word is correctly spelled so apppend to output
        out_file << word;
        updateContextQueue(context_queue, word);
    }

    word.clear(); // Reset word for next use
}


void file_scan_start(PrefixTrie* pt, const std::string& filename, const std::string& output_filename) {

    // Read in the input file
    std::fstream in_file(filename, std::fstream::in);
    if(!in_file.is_open()){
      std::cout << RED << "File could not open input file: " << filename <<  RESET << std::endl;
      exit(1);
    }

    // Open the output file
    std::ofstream out_file(output_filename);
    if(!out_file.is_open()){
      std::cout << RED << "File could not open output file: " << output_filename <<  RESET << std::endl;
      exit(1);
    }

    char ch;
    std::string word = "";

    // Keeps track of the words before the misspelling
    // to give context to the user
    std::queue<std::string> context_queue;

    while (in_file >> std::noskipws >> ch) {

        //Add current charcter to word
        // isaplha function makes sure
        // that character is one that should
        // be in a word
        if (isalpha(ch)) {
            word += ch;
            continue;
        }

        processWord(out_file, pt, word, context_queue);

        // Also add single characters to context
        updateContextQueue(context_queue, std::string(1, ch));

        // Make sure to write single characters like spaces and puncuation
        // to output file as well
        out_file << ch;
    }

    // Success
    std::cout << GREEN << "Sucesfully updated spellings to " << YELLOW << output_filename << RESET << "\n";

}


// Windows only stuff
void interactive(PrefixTrie* pt){


#ifdef _WIN32

  
    TerminalMode terminal;
    terminal.enableRawMode();
    terminal.clearScreen();

    // Where we are on screen
    int currentX = 0;
    int currentY = 0;

    std::string currentWord = "";
    std::string input; // Stores the current data shown on screen
    std::string lastTwoChars; // Helps with save and exiting
    int c;

    int selectedSuggestion = -1; // Initially no suggestion is selected

    // Get terminal dimensions
    auto dim = terminal.getTerminalDimensions();
    int width = dim.first;
    int height = dim.second;

    std::vector<WordScore> word_scores;


    try {
        while (true) {
            terminal.clearScreen();

            // Move to next line
            if(currentX >= width){
              currentX = 0;
              currentY += 1;
            }

            
            size_t suggestionsSize = std::min(word_scores.size(), (size_t)INTERACTIVE_SUGGESTION_LEN);


            // Find the length of the longest word in the suggestions
            // Make the little dropdown look nice
            size_t maxLength = 0;
            for (size_t i = 0; i < suggestionsSize; ++i) {
                maxLength = std::max(maxLength, word_scores[i].word.size());
            }


            // Display suggestions
            for (size_t i = 0; i < suggestionsSize; ++i) {

                terminal.moveCursor(currentX, currentY + (int)i + 1); // Adjust starting line as needed

                // Highlight the selected suggestion
                if (static_cast<int>(i) == selectedSuggestion) {
                    terminal.setTextAndBackgroundColor(TERMINAL_RED, BACKGROUND_LIGHT_GREY);
                } else {
                    terminal.setTextAndBackgroundColor(TERMINAL_BLACK, BACKGROUND_LIGHT_GREY);
                }

              // Pad the word so that all are the same length
              std::string paddedWord = word_scores[i].word;
              paddedWord += std::string(maxLength - paddedWord.size(), ' '); // Pad with spaces
              std::cout << paddedWord;
                
                terminal.setTextAndBackgroundColor(DEFAULT_FOREGROUND_COLOR, DEFAULT_BACKGROUND_COLOR); // Reset color
            }

            // Show the already written text
            terminal.moveCursor(0, 0);
            std::cout << input;

            // Move cursor back to the start of the input line
            terminal.moveCursor(currentX, currentY);

            // Read a single key press
            c = terminal.readKey();


            // Check for Control + W to save and exit
            if(c == 23){
                terminal.clearScreen();
                terminal.disableRawMode();
                
                std::string filename = "default_input.txt"; // Default filename

                // Prompt the user for a filename
                std::cout << "Enter filename to save input: ";
                std::getline(std::cin, filename); // Use getline to allow spaces in filename

                std::ofstream outFile(filename);
                if (outFile.is_open()) {
                    outFile << input; // Write the input to the file
                    outFile.close();
                    std::cout << "Input saved to " << filename << std::endl;
                } else {
                    std::cerr << "Failed to open file for writing." << std::endl;
                }

                return; // Exit after saving
            }


            if (c == 27) { // ESC sequence for arrow keys
                c = terminal.readKey(); // Skip [
                switch (terminal.readKey()) { // Get the direction
                    case 'A': // Up arrow
                        selectedSuggestion = (selectedSuggestion - 1 + suggestionsSize) % suggestionsSize;
                        break;
                    case 'B': // Down arrow
                        selectedSuggestion = (selectedSuggestion + 1) % suggestionsSize;
                        break;
                }
            } else if (c == 127) { // Handle backspace
                if (!input.empty()) {
                    input.pop_back();
                    currentX = currentX - 1;
                    if (!currentWord.empty()) currentWord.pop_back();
                }
            } else if ((c == '\n' || c == '\r') && selectedSuggestion != -1) { // Exit on enter key
                // Erase current word from input
                size_t startPos = input.size() - currentWord.size();
                input.erase(startPos, currentWord.size());

                // Add suggested word
                std::string selectedWord = word_scores[selectedSuggestion].word;
                input += selectedWord + " "; // Adding a space after the suggestion

                // Update Cursor position directly based on the length of the input string
                currentX = static_cast<int>(input.size() - (currentY * width));
                currentWord.clear();

            } else if (c == ' '){
                // Handle space
                input += " ";
                currentX += 1;
                currentWord = ""; // Reset currentWord after space

            } else if (c >= 32 && c <= 126) { // Add character to input
                input += static_cast<char>(c);
                currentWord += static_cast<char>(c);
                currentX += 1;


            }



            word_scores = get_best_words(pt, currentWord);
        }
    } catch (std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }

    terminal.clearScreen();
    terminal.disableRawMode();



  #else
    std::cout << "Interactive Mode for Windows OS only\n"; 
  #endif // DEBUG

} 

