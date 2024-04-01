#pragma once

#include <queue> 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "prifixTrie.h"
#include "score.h"

// Define ANSI color codes
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";
const std::string RESET = "\033[0m";
const std::string BOLD_RED = "\033[1;31m";
const std::string BOLD_GREEN = "\033[1;32m";
const std::string BOLD_YELLOW = "\033[1;33m";
const std::string BOLD_BLUE = "\033[1;34m";
const std::string BOLD_MAGENTA = "\033[1;35m";
const std::string BOLD_CYAN = "\033[1;36m";
const std::string BOLD_WHITE = "\033[1;37m";
const std::string BOLD_LIGHT_GRAY = "\033[1;97m";
const std::string BOLD_GREEN_YELLOW = "\033[1;93m";
const std::string BOLD_GRAY = "\033[1;90m";
const std::string BOLD_PURPLE = "\033[1;95m";
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#define CONTEXT_QUEUE_SIZE 8
#define INTERACTIVE_SUGGESTION_LEN 4

void start_repl(PrefixTrie* pt);
void start_file_accuracy(PrefixTrie* pt, std::string user_path);
void file_scan_prompt(std::string word, std::vector<WordScore>& word_scores, int word_score_offset, std::queue<std::string>& context);
std::string file_scan_input_loop(const std::string& currentWord, std::vector<WordScore>& word_scores, int& offset, std::queue<std::string>& context);
void file_scan_start(PrefixTrie* pt, const std::string& filename, const std::string& output_filename);

void interactive(PrefixTrie* pt); // WINDOWS ONLY


