#pragma once

#include "Storage File.h"

#include <iomanip>

class Commands
{
private:
	const static size_t INITIAL_COMMAND_READ_LIMIT = 64;
	const static size_t HELP_OUTPUT_WIDTH = 48;
	const static size_t INTEGER_DIGITS_LIMIT = 8;

	Commands();

	static void expandString(char*& string, size_t& stringLimit);
	static char* inputContent(std::istream& is, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static char* inputCommand(std::istream& is, bool& endOfLine);
	static int inputNonnegativeInteger(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static Date inputDate(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol = '\n', const char fillerSymbol = ' ');
	static bool detectExtraCharacters(std::istream& is, const char endSymbol = '\n', const char fillerSymbol = ' ');
public:
	Commands(const Commands&) = delete;
	Commands& operator=(const Commands&) = delete;

	static void openFile(StorageFile& file);
	static void createFile(StorageFile& file);
	static void closeFile(StorageFile& file);
	static void saveAsFile(StorageFile& file);
	static void saveFile(StorageFile& file);
	static void help();
	static void exit(StorageFile& file);

	static void add(StorageFile& file);
	static void remove(StorageFile& file);
	static void clean(StorageFile& file);
	static void log(StorageFile& file);
	static void print(StorageFile& file);
	static void printAll(StorageFile& file);
	static void calculateLoses(StorageFile& file);

	static bool menu(StorageFile& file);
};
