
# SpellChecker CLI

SpellChecker CLI is a command-line interface application designed to identify and correct misspellings in text. Utilizing advanced algorithms and a comprehensive dictionary, it offers efficient spell-checking capabilities for both interactive and batch processing modes



## Data Strucures & Algorithms
- Prefix Trie: Easily store all words in dictonary with easy and efficient lookups 
- Levenshtine Distance: Give an edit score to covnert one word into another
- Cosine Similarity: Give a number indicating how similar two words are
- HeapSort: Normal Sorting algorithm

## Features
SpellChecker CLI comes with a suite of features designed to cater to a wide range of spelling correction needs:

- **REPL Mode**: An interactive Read-Eval-Print Loop mode that allows users to enter text and receive immediate spelling corrections and suggestions
- **File Scanning**: Process text files to identify and correct misspellings, with options to output the corrected text to a specified file
- **Accuracy Reporting**: Analyze documents to report the percentage of correctly spelled words and identify misspellings. Supports batch files
- **Interactive (Windows Only)**: A simple text editting mode in the termianl that provides real-time suggestions as you type, can save to file when you are done 


## Prerequisites
- A C++ compiler (e.g., GCC, Clang, MSVC)
- Git (for cloning the repository)
- Make (for macOS/Linux)

## Installation

Follow these steps to install SpellChecker CLI on your machine:

1. **Clone the Repository**

   Use Git to clone the SpellChecker CLI repository to your local machine:

   ```bash
   git clone https://github.com/yourusername/spellchecker-cli.git
   ```

2. **Build the Project**

   Depending on your operating system, use the appropriate command to build SpellChecker CLI:

   - **Windows**: Run `build.bat` from the command line
   - **Mac/Linux**: Execute `make` in your terminal

3. **Add the Executable to Your PATH**

   To use SpellChecker CLI from any directory, add the path to the executable to your system's PATH environment variable. This step varies by operating system, so consult your OS documentation for instructions.

## Usage


##### REPL
```bash
   spellcheck -repl 
```

##### File Checker
```bash
   spellcheck -file [filename] -o [filename(optional)] 
```

##### File(s) Accuracy Score
```bash
   spellcheck -acc [filename/directory]
```

##### Interactive Mode
```bash
   spellcheck -i
```

## Contributing
