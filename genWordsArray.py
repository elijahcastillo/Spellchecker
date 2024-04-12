with open("src/words/50000.txt", "r") as input_file:
    lines = input_file.readlines()

    with open("src/include/exportedWords.h", "w") as output_file:
        # Write the start of the array
        output_file.write("const char* dict[] = {\n    ")

        # Process each word, remove the newline, and write it to the file on the same line
        for word in lines:
            clean_word = word.strip()  # Remove whitespace characters like `\n` at the start and end
            output_file.write(f'"{clean_word}", ')

        # Write the end of the array with a NULL pointer to signify the end of the array
        output_file.write("nullptr\n};\n")
