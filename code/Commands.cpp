#include "Commands.h"

void Commands::expandString(char*& string, size_t& limit)
{
	char* newString = new char[2 * limit + 1];
	strncpy(newString, string, limit);
	limit *= 2;
	delete[] string;
	string = newString;
}

char* Commands::inputContent(std::istream& is, const char endSymbol, const char fillerSymbol)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT;
	char* temp = new char[limit + 1];
	char character;

	character = is.get();
	while (character != endSymbol && character == fillerSymbol) is.get(character);
	while (character != endSymbol)
	{
		if (size == limit) expandString(temp, limit);
		temp[size] = character;
		size++;
		is.get(character);
	}
	while (size > 0 && temp[size - 1] == fillerSymbol) size--;
	temp[size] = '\0';

	return temp;
}

char* Commands::inputCommand(std::istream& is, bool& endOfLine)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT;
	char* temp = new char[limit + 1];
	char character;

	character = is.get();
	while (character != ' ' && character != '\n')
	{
		if (size == limit) expandString(temp, limit);
		temp[size] = character;
		size++;
		is.get(character);
	}
	temp[size] = '\0';

	if (character == '\n') endOfLine = 1;
	else endOfLine = 0;

	return temp;
}

int Commands::inputNonnegativeInteger(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol, const char fillerSymbol)
{
	size_t size = 0, limit = INITIAL_COMMAND_READ_LIMIT, result;
	char temp[INTEGER_DIGITS_LIMIT + 1];
	char character;

	character = is.get();
	while (character != endSymbol && character == fillerSymbol) is.get(character);
	if (character >= '0' && character <= '9')
	{
		while (character >= '0' && character <= '9')
		{
			if (size < INTEGER_DIGITS_LIMIT)
			{
				temp[size] = character;
				size++;
			}
			is.get(character);
		}
		temp[size] = '\0';
		if (character == fillerSymbol)
		{
			validInput = 1;
			endOfLine = 0;
		}
		else if (character == endSymbol)
		{
			validInput = 1;
			endOfLine = 1;
		}
		else
		{
			validInput = 0;
			endOfLine = 0;
		}

		if (size == INTEGER_DIGITS_LIMIT) validInput = 0;

		result = atoi(temp);
		return result;
	}
	else
	{
		endOfLine = (character == '\n');
		validInput = 0;
		return 0;
	}
}

Date Commands::inputDate(std::istream& is, bool& validInput, bool& endOfLine, const char endSymbol, const char fillerSymbol)
{
	size_t day = 0, month = 0, year = 0;
	char character;

	validInput = 0;
	endOfLine = 0;
	character = is.get();
	while (character != endSymbol && character == fillerSymbol) is.get(character);
	if (character >= '0' && character <= '9')
	{
		year += character - '0';
		for (size_t i = 0; i < 3; i++)
		{
			is.get(character);
			if (character >= '0' && character <= '9')
			{
				year *= 10;
				year += character - '0';
			}
			else
			{
				endOfLine = (character == '\n');
				return Date();
			}
		}

		is.get(character);
		if (character != '-')
		{
			endOfLine = (character == '\n');
			return Date();
		}

		for (size_t i = 0; i < 2; i++)
		{
			is.get(character);
			if (character >= '0' && character <= '9')
			{
				month *= 10;
				month += character - '0';
			}
			else
			{
				endOfLine = (character == '\n');
				return Date();
			}
		}

		is.get(character);
		if (character != '-')
		{
			endOfLine = (character == '\n');
			return Date();
		}

		for (size_t i = 0; i < 2; i++)
		{
			is.get(character);
			if (character >= '0' && character <= '9')
			{
				day *= 10;
				day += character - '0';
			}
			else
			{
				endOfLine = (character == '\n');
				return Date();
			}
		}

		validInput = 1;
		return Date(day, month, year);
	}
	else
	{
		endOfLine = (character == '\n');
		return Date();
	}
}

bool Commands::detectExtraCharacters(std::istream& is, const char endSymbol, const char fillerSymbol)
{
	char character;

	is.get(character);
	while (character == fillerSymbol) is.get(character);
	if (character == endSymbol) return 0;
	while (is.get() != endSymbol);
	return 1;
}

void Commands::help()
{
	std::cout << "The following commands are supported:" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "open <file>" << "opens <file>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "create <sections> <shelfs> <places> <file>" << "creates a new file with the given parameters in <file>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "close" << "closes the currently opened file" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "save" << "saves the currently opened file" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "saveas <file>" << "saves the currently opened file in <file>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "add" << "opens a dialogue to add a new product" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "remove" << "opens a dialogue to remove a product" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "clean" << "cleans the products that exceed their expiration date" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "log <from> <to>" << "prints all changes in the period <from> - <to>" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "print" << "prints all the products in the storage by their name" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "printall" << "prints all the products and their partides in the storage" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "lost" << "opens a dialog to calculate product loses due to cleaning" << std::endl;
	std::cout << std::left << std::setw(HELP_OUTPUT_WIDTH) << "exit" << "exits the program" << std::endl;
}

void Commands::openFile(StorageFile& file)
{
	char* address = inputContent(std::cin);
	char state = file.openNewFile(address);

	switch (state)
	{
	case 'O':
		std::cout << "The current file should be closed before a new one can be opened!" << std::endl;
		break;
	case 'L':
		std::cout << "The file could not be found! Check if you entered a correct address." << std::endl;
		break;
	case 'H':
		std::cout << "The history file is missing!" << std::endl;
		break;
	case 'S':
		std::cout << "File successfully opened!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occurred! " << std::endl;
	}

	if (state != 'S' && state != 'O') file.closeCurrentFile();
	delete[] address;
}

void Commands::createFile(StorageFile& file)
{
	if (file.isOpen())
	{
		std::cout << "The current file should be closed before a new one can be created!" << std::endl;
		while (std::cin.get() != '\n');
		return;
	}

	char* address;
	char state;
	size_t sections, shelfs, places;
	bool validInput, endOfLine;

	sections = inputNonnegativeInteger(std::cin, validInput, endOfLine);
	if (!validInput || endOfLine)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	shelfs = inputNonnegativeInteger(std::cin, validInput, endOfLine);
	if (!validInput || endOfLine)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	places = inputNonnegativeInteger(std::cin, validInput, endOfLine);
	if (!validInput || endOfLine)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	address = inputContent(std::cin);

	if (sections == 0 || shelfs == 0 || places == 0)
	{
		if (sections == 0) std::cout << "Cannot have zero sections!" << std::endl;
		if (shelfs == 0) std::cout << "Cannot have zero shelfs per section!" << std::endl;
		if (places == 0) std::cout << "Cannot have zero places per shelf!" << std::endl;
		return;
	}

	state = file.createNewFile(places, shelfs, sections, address);

	switch (state)
	{
	case 'O':
		std::cout << "The current file should be closed before a new one can be created!" << std::endl;
		break;
	case 'L':
		std::cout << "The given address is invalid!" << std::endl;
		break;
	case 'H':
		std::cout << "The given history address is invalid!" << std::endl;
		break;
	case 'S':
		std::cout << "New file successfully created!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occurred! " << std::endl;
	}

	delete[] address;
}

void Commands::closeFile(StorageFile& file)
{
	if (file.closeCurrentFile()) std::cout << "The file was successfully closed!" << std::endl;
	else std::cout << "No file is currently opened!" << std::endl;
}

void Commands::saveAsFile(StorageFile& file)
{
	char* address = inputContent(std::cin);

	char state = file.writeStorageAsFile(address);

	switch (state)
	{
	case 'N':
		std::cout << "No file is currently opened!" << std::endl;
		break;
	case 'L':
		std::cout << "The given address is invalid!" << std::endl;
		break;
	case 'S':
		std::cout << "File successfully saved!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occured!" << std::endl;
	}

	delete[] address;
}

void Commands::saveFile(StorageFile& file)
{
	char state = file.writeStorageAsFile(nullptr);

	switch (state)
	{
	case 'N':
		std::cout << "No file is currently opened!" << std::endl;
		break;
	case 'S':
		std::cout << "File successfully saved!" << std::endl;
		break;
	default:
		std::cout << "An unknown error has occured!" << std::endl;
	}
}

void Commands::exit(StorageFile& file)
{
	file.closeCurrentFile();
	std::cout << "Successfully exited the program!" << std::endl;
}

void Commands::add(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	Date expireDate;
	char *name, *producer, *comment, *content;
	size_t size, quantity;
	char measureUnit;
	bool validInput, endOfLine;

	std::cout << "Name: ";
	name = inputContent(std::cin);
	std::cout << "Producer: ";
	producer = inputContent(std::cin);
	std::cout << "Comment: ";
	comment = inputContent(std::cin);

	std::cout << "Expire date: ";
	while (1)
	{
		expireDate = inputDate(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter the date in the format YYYY-MM-DD" << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the date!" << std::endl;
			else
			{
				if (!expireDate.validDate()) std::cout << "The given date does not exist!" << std::endl;
				else break;
			}
		}
	}

	std::cout << "Measurement unit: ";
	while (1)
	{
		content = inputContent(std::cin);
		if (!strcmp(content, "kg"))
		{
			measureUnit = 'K';
			break;
		}
		else if (!strcmp(content, "L") || !strcmp(content, "l"))
		{
			measureUnit = 'L';
			break;
		}
		else
		{
			delete[] content;
			std::cout << "Invalid measurement unit! Enter \"kg\" for kilograms and \"L\" for litres." << std::endl;
		}
	}
	delete[] content;

	std::cout << "Size: ";
	while (1)
	{
		size = inputNonnegativeInteger(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter a single positive integer." << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the size!" << std::endl;
			else
			{
				if (size == 0) std::cout << "Cannot have a product that does not need any space!" << std::endl;
				else if (size > file.getStorage().getNumberOfPlaces())
				{
					std::cout << "Products are too big to fit in a single shelf! The size of a shelf is only " << file.getStorage().getNumberOfPlaces() << ". Do you want to stop adding the item? ( yes / no ) : ";
					bool stopAdding = 0;
					while (1)
					{
						content = inputContent(std::cin);
						if (!strcmp(content, "yes"))
						{
							stopAdding = 1;
							break;
						}
						else if (!strcmp(content, "no")) break;
						else
						{
							std::cout << "Invalid input! Enter \"yes\" or \"no\"! : ";
							delete[] content;
						}
					}
					delete[] content;

					if (stopAdding)
					{
						delete[] name;
						delete[] producer;
						delete[] comment;
						return;
					}
					else std::cout << "Enter new size for the item: ";
				}
				else break;
			}
		}
	}

	std::cout << "Quantity: ";
	while (1)
	{
		quantity = inputNonnegativeInteger(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter a single positive integer." << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the quantity!" << std::endl;
			else
			{
				if (quantity == 0) std::cout << "Cannot have a product with no quantity!" << std::endl;
				else break;
			}
		}
	}

	if (file.getStorage().addProduct(Product(name, producer, comment, size, quantity, measureUnit, expireDate))) std::cout << "Product successfully added to the storage!" << std::endl;
	else std::cout << "Not enough free space in the storage to add the product!" << std::endl;

	delete[] name;
	delete[] producer;
	delete[] comment;
}

void Commands::remove(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	char *name, *content;
	size_t totalQuantity, removeQuantity;
	bool validInput, endOfLine;

	std::cout << "Name: ";
	name = inputContent(std::cin);

	totalQuantity = file.getStorage().getSimilarProductsQuantity(name);
	if (totalQuantity == 0)
	{
		std::cout << "There are no products with that name!" << std::endl;
		delete[] name;
		return;
	}

	std::cout << "Quantity: ";
	while (1)
	{
		removeQuantity = inputNonnegativeInteger(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter a single positive integer." << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the quantity!" << std::endl;
			else break;
		}
	}

	if (removeQuantity == 0) std::cout << "Cannot remove zero quantity!" << std::endl;
	else
	{
		if (removeQuantity >= totalQuantity)
		{
			std::cout << "The total quantity of the product is " << totalQuantity << ":" << std::endl;
			file.getStorage().printExpirationDates(name);

			bool continueRemoving = 0;
			std::cout << "Do you want to remove all of the product? ( yes / no ) : ";
			while (1)
			{
				content = inputContent(std::cin);
				if (!strcmp(content, "yes"))
				{
					continueRemoving = 1;
					break;
				}
				else if (!strcmp(content, "no")) break;
				else
				{
					std::cout << "Invalid input! Enter \"yes\" or \"no\"! : ";
					delete[] content;
				}
			}
			delete[] content;

			if (!continueRemoving)
			{
				delete[] name;
				return;
			}
		}

		file.getStorage().reduceProductsQuantityByName(name, removeQuantity);
		std::cout << "Successfully removed the desired quantity!" << std::endl;
	}

	delete[] name;
}

void Commands::clean(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	size_t cleaned = file.getStorage().cleanProducts();
	std::cout << "Successfully cleaned " << cleaned << " products:" << std::endl;
	file.getStorage().printLastChanges(cleaned);
}

void Commands::log(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	Date fromDate, toDate;
	bool validInput, endOfLine;

	fromDate = inputDate(std::cin, validInput, endOfLine);
	if (!validInput || endOfLine)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	toDate = inputDate(std::cin, validInput, endOfLine);
	if (!validInput)
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		if (!endOfLine) while (std::cin.get() != '\n');
		return;
	}

	if (!endOfLine && detectExtraCharacters(std::cin))
	{
		std::cout << "The given arguments are invalid!" << std::endl;
		return;
	}

	if (!fromDate.validDate() || !toDate.validDate())
	{
		if (!fromDate.validDate()) std::cout << "The date " << fromDate << " is invalid!" << std::endl;
		if (!toDate.validDate()) std::cout << "The date " << toDate << " is invalid!" << std::endl;
		return;
	}

	if (fromDate > toDate)
	{
		std::cout << "The date " << fromDate << " is after " << toDate << "! No changes to show." << std::endl;
		return;
	}

	std::cout << "The following changes were made between " << fromDate << " and " << toDate << ":" << std::endl;
	file.printStorageChanges(fromDate, toDate);
}

void Commands::print(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	if (file.getStorage().getNumberOfProducts() > 0)
	{
		std::cout << "The following products are currently in the storage:" << std::endl;
		file.getStorage().printProductsByName();
	}
	else std::cout << "Currently there are no products in the storage." << std::endl;
}

void Commands::printAll(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	if (file.getStorage().getNumberOfProducts() > 0)
	{
		std::cout << "The following products are currently in the storage:" << std::endl;
		file.getStorage().printProducts();
	}
	else std::cout << "Currently there are no products in the storage." << std::endl;
}

void Commands::calculateLoses(StorageFile& file)
{
	if (!file.isOpen())
	{
		std::cout << "No file is currently opened!" << std::endl;
		return;
	}

	Date fromDate, toDate;
	double price;
	char *name, *content;
	size_t quantity;
	bool validInput, endOfLine, realNumber, comma;

	std::cout << "Name: ";
	name = inputContent(std::cin);

	std::cout << "Price per product instance: ";
	while (1)
	{
		content = inputContent(std::cin);
		if (!strcmp(content, ""))
		{
			delete[] content;
			continue;
		}
		realNumber = 1;
		comma = 0;
		for (size_t i = 0; i < strlen(content); i++)
		{
			if (content[i] == '.')
			{
				if (!comma) comma = 1;
				else
				{
					realNumber = 0;
					break;
				}
			}
			else if (content[i] < '0' || content[i] > '9')
			{
				realNumber = 0;
				break;
			}
		}

		if (realNumber) price = atof(content);
		delete[] content;
		if (realNumber) break;
		else std::cout << "Enter a positive real number!" << std::endl;
	}

	std::cout << "From date: ";
	while (1)
	{
		fromDate = inputDate(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter the date in the format YYYY-MM-DD" << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the date!" << std::endl;
			else
			{
				if (!fromDate.validDate()) std::cout << "The given date does not exist!" << std::endl;
				else break;
			}
		}
	}

	std::cout << "To date: ";
	while (1)
	{
		toDate = inputDate(std::cin, validInput, endOfLine);
		if (!validInput)
		{
			std::cout << "Invalid input! Enter the date in the format YYYY-MM-DD" << std::endl;
			if (!endOfLine) while (std::cin.get() != '\n');
		}
		else
		{
			if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "Do not enter any additional input after the date!" << std::endl;
			else
			{
				if (!toDate.validDate()) std::cout << "The given date does not exist!" << std::endl;
				else break;
			}
		}
	}

	if (fromDate > toDate)
	{
		std::cout << "The date " << fromDate << " is after " << toDate << "! No changes to show." << std::endl;
		return;
	}

	quantity = file.getChangedProductsQuantity(name, fromDate, toDate, 'C');

	std::cout << "From " << fromDate << " to " << toDate << " a total of " << quantity << " instances of the product were cleaned, amounting to a loss of " << quantity * price << std::endl;

	delete[] name;
}

bool Commands::menu(StorageFile& file)
{
	char* command;
	bool endOfLine;

	std::cout << '>';

	command = inputCommand(std::cin, endOfLine);

	if (!strcmp(command, "open"))
	{
		if (endOfLine) std::cout << "\"open\" should take arguments!" << std::endl;
		else openFile(file);
	}
	else if (!strcmp(command, "create"))
	{
		if (endOfLine) std::cout << "\"create\" should take arguments!" << std::endl;
		else createFile(file);
	}
	else if (!strcmp(command, "close"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"close\" does not take any arguments!" << std::endl;
		else closeFile(file);
	}
	else if (!strcmp(command, "saveas"))
	{
		if (endOfLine) std::cout << "\"saveas\" should take arguments!" << std::endl;
		else saveAsFile(file);
	}
	else if (!strcmp(command, "save"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"save\" does not take any arguments!" << std::endl;
		else saveFile(file);
	}
	else if (!strcmp(command, "help"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"help\" does not take any arguments!" << std::endl;
		else help();
	}
	else if (!strcmp(command, "add"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"add\" does not take any arguments!" << std::endl;
		else add(file);
	}
	else if (!strcmp(command, "remove"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"remove\" does not take any arguments!" << std::endl;
		else remove(file);
	}
	else if (!strcmp(command, "clean"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"clean\" does not take any arguments!" << std::endl;
		else clean(file);
	}
	else if (!strcmp(command, "log"))
	{
		if (endOfLine) std::cout << "\"log\" should take arguments!" << std::endl;
		else log(file);
	}
	else if (!strcmp(command, "print"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"print\" does not take any arguments!" << std::endl;
		else print(file);
	}
	else if (!strcmp(command, "printall"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"printall\" does not take any arguments!" << std::endl;
		else printAll(file);
	}
	else if (!strcmp(command, "lost"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"lost\" does not take any arguments!" << std::endl;
		else calculateLoses(file);
	}
	else if (!strcmp(command, "exit"))
	{
		if (!endOfLine && detectExtraCharacters(std::cin)) std::cout << "\"exit\" does not take any arguments!" << std::endl;
		else
		{
			exit(file);
			return 0;
		}
	}
	else
	{
		if (!endOfLine) while (std::cin.get() != '\n');
		std::cout << "Unknown command!" << std::endl;
	}

	return 1;
}