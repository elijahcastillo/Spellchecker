#include <iostream> 
#include <string> 
#include <fstream>
#include "include/prifixTrie.h"
#include "include/features.h"
#define DICTIONARY_FILE "src/words/50000.txt"



void printHelp() {
    std::cout << "Usage:\n"
              << "  -repl\t\tStarts the application in REPL mode\n"
              << "  -file [filename]\tStarts file scanning on the specified file. Use -o to specify output file.\n"
              << "  -o [output_filename]\tSpecifies the output file for file scanning (must be used with -file)\n"
              << "  -help\t\tDisplays this help message\n";
}


int main(int argc, char** argv){

    if (argc == 1) {
        printHelp();
        return 1;
    }

  std::fstream file(DICTIONARY_FILE);
    if(!file.is_open()){
      std::cout << "Could not open file: " << DICTIONARY_FILE << std::endl;
      exit(1);
  }



  // Fill in prefix tree
  PrefixTrie pt;
  std::string word;
  while(file >> word){
    bool success = pt.insert_word(word);
    if(!success){
      std::cout << "ERROR: Unable to add word '" << word << "'" << "\n";
    }
  }

  for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];

      if (arg == "-help") {
          printHelp();
          return 1;

      } else if (arg == "-i"){
        interactive(&pt);
        return 1;
      }
      else if (arg == "-acc"){
        i++;

        std::string inputPath = argv[i];
        start_file_accuracy(&pt, inputPath);
        return 1;
      }
      else if (arg == "-repl") {
          start_repl(&pt);
          return 1;

      } else if (arg == "-file") {
        i++; // Skip past -file
        if(i < argc){
          std::string inputFile = argv[i];
          std::string outputFile = "out.txt";

          i++; // Skip past input file
          if( (i + 1 < argc) && std::string(argv[i]) == "-o"  ){
            outputFile = argv[i + 1];
          }
          file_scan_start(&pt, inputFile, outputFile);
          return 1;

        } else {
          std::cout << "Unknown command or missing arguments\n";
          printHelp();
        }

      } else {
          std::cout << "Unknown command or missing arguments\n";
          printHelp();
          return 1;
      }
  }





}
