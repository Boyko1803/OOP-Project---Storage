#include "Storage File.h"

char* StorageFile::deduceHistoryAddress(const char* storageFileAddress) const
{
	char* temp = new char[strlen(storageFileAddress) + 11];
	strcpy(temp, storageFileAddress);
	size_t index = strlen(temp) - 1;
	while (temp[index] != '.') index--;

	temp[index] = '\0';
	strcat(temp, "_history.txt");
	index += 12;
	temp[index] = '\0';

	return temp;
}

void StorageFile::copyFileContent(const char* destinationAddress, char* sourceAddress) const
{
	if (!strcmp(destinationAddress, sourceAddress)) return;

	std::ofstream destination(destinationAddress, std::ios::trunc | std::ios::out);
	std::ifstream source(sourceAddress, std::ios::in);

	char character;
	source.get(character);
	while (!source.eof())
	{
		destination << character;
		source.get(character);
	}

	destination.close();
	source.close();
}

StorageFile::StorageFile() :
	currentStorage(nullptr),
	storageAddress(nullptr),
	historyAddress(nullptr),
	writtenChanges(0),
	openFlag(0)
{}

StorageFile::~StorageFile()
{
	if (openFlag)
	{
		delete currentStorage;
		delete[] storageAddress;
		delete[] historyAddress;
	}
}

bool StorageFile::isOpen() const
{
	return openFlag;
}

const char* const StorageFile::getStorageAddress() const
{
	return storageAddress;
}

const char* const StorageFile::getHistoryAddress() const
{
	return historyAddress;
}

char StorageFile::printStorageChanges(const Date& fromDate, const Date& toDate, std::ostream& os) const
{
	if (!openFlag) return 'N';

	std::ifstream historyFile(historyAddress, std::ios::in);
	if (!historyFile) return 'H';

	Storage::ProductChange change;

	while (!historyFile.eof())
	{
		change.read(historyFile);
		historyFile.get(); historyFile.get();
		if ((change.changeDate > fromDate || change.changeDate == fromDate) && (change.changeDate < toDate || change.changeDate == toDate)) os << change << std::endl;
	}

	historyFile.close();

	for (size_t i = 0; i < currentStorage->numberOfChanges; i++)
	{
		if ((currentStorage->changes[i].changeDate > fromDate || currentStorage->changes[i].changeDate == fromDate) && (currentStorage->changes[i].changeDate < toDate || currentStorage->changes[i].changeDate == toDate)) os << currentStorage->changes[i];
	}

	return 'S';
}

size_t StorageFile::getChangedProductsQuantity(const char* name, const Date& fromDate, const Date& toDate, char actionSymbol) const
{
	if (!openFlag) return 0;

	Storage::ProductChange change;
	size_t quantity = 0;
	std::ifstream historyFile(historyAddress, std::ios::in);
	if (historyFile)
	{
		while (!historyFile.eof())
		{
			change.read(historyFile);
			historyFile.get(); historyFile.get();
			if ((change.action == actionSymbol) && (change.product == name) && (change.changeDate > fromDate || change.changeDate == fromDate) && (change.changeDate < toDate || change.changeDate == toDate)) quantity += change.quantity;
		}
	}

	historyFile.close();

	for (size_t i = 0; i < currentStorage->numberOfChanges; i++)
	{
		if ((currentStorage->changes[i].action == actionSymbol) && (currentStorage->changes[i].product == name) && (currentStorage->changes[i].changeDate > fromDate || currentStorage->changes[i].changeDate == fromDate) && (currentStorage->changes[i].changeDate < toDate || currentStorage->changes[i].changeDate == toDate)) quantity += currentStorage->changes[i].quantity;
	}

	return quantity;
}

Storage& StorageFile::getStorage()
{
	return *currentStorage;
}

char StorageFile::writeStorageAsFile(const char* storageFileAddress, const char* historyFileAddress)
{
	if (!openFlag) return 'N';
	std::ofstream storageFile;
	std::ofstream historyFile;

	if (storageFileAddress == nullptr) storageFile.open(storageAddress, std::ios::out | std::ios::trunc);
	else storageFile.open(storageFileAddress, std::ios::out | std::ios::trunc);
	if (!storageFile) return 'L';

	if (historyFileAddress != nullptr)
	{
		historyFile.open(historyFileAddress, std::ios::out | std::ios::app);
		if (historyFile)
		{
			historyFile.close();
			copyFileContent(historyFileAddress, historyAddress);
			historyFile.open(historyFileAddress, std::ios::out | std::ios::app);
		}
	}
	else if (storageFileAddress == nullptr) historyFile.open(historyAddress, std::ios::out | std::ios::app);
	else
	{
		char* temp = deduceHistoryAddress(storageFileAddress);
		historyFile.open(temp, std::ios::out | std::ios::app);
		if (historyFile)
		{
			historyFile.close();
			copyFileContent(temp, historyAddress);
			historyFile.open(temp, std::ios::out | std::ios::app);
		}
		delete[] temp;
	}
	if (!historyFile) return 'H';

	storageFile << currentStorage->numberOfSections << ' ' << currentStorage->numberOfShelfs << ' ' << currentStorage->numberOfPlaces << ' ' << currentStorage->numberOfProducts << std::endl;
	for (size_t i = 0; i < currentStorage->numberOfProducts; i++) currentStorage->products[i]->write(storageFile, 0);
	storageFile << std::endl;
	for (size_t section = 0; section < currentStorage->numberOfSections; section++)
	{
		for (size_t shelf = 0; shelf < currentStorage->numberOfShelfs; shelf++)
		{
			for (size_t place = 0; place < currentStorage->numberOfPlaces; place++)
			{
				if (currentStorage->sites[section][shelf][place] == nullptr) storageFile << 0 << ' ';
				else storageFile << currentStorage->sites[section][shelf][place]->identificationIndex << ' ';
			}
			storageFile << std::endl;
		}
		storageFile << std::endl;
	}

	for (size_t i = (storageFileAddress == nullptr) ? writtenChanges : 0; i < currentStorage->numberOfChanges; i++) currentStorage->changes[i].write(historyFile);
	if (storageFileAddress == nullptr) writtenChanges = currentStorage->numberOfChanges;

	storageFile.close();
	historyFile.close();

	return 'S';
}

char StorageFile::openNewFile(const char* storageFileAddress, const char* historyFileAddress)
{
	if (openFlag) return 'O';

	std::ifstream storageFile(storageFileAddress, std::ios::in);
	if (!storageFile) return 'L';

	openFlag = 1;
	storageAddress = new char[strlen(storageFileAddress) + 1];
	strcpy(storageAddress, storageFileAddress);

	if (historyFileAddress == nullptr) historyAddress = deduceHistoryAddress(storageAddress);
	else
	{
		historyAddress = new char[strlen(historyFileAddress) + 1];
		strcpy(historyAddress, historyFileAddress);
	}
	bool openedHistory = 0;
	std::ifstream historyFile(historyAddress, std::ios::in);
	if (historyFile) openedHistory = 1;
	historyFile.close();

	size_t numberOfSections, numberOfShelfs, numberOfPlaces, numberOfProducts, productIndex, temp;

	storageFile >> numberOfSections >> numberOfShelfs >> numberOfPlaces >> numberOfProducts;
	currentStorage = new Storage(numberOfPlaces, numberOfShelfs, numberOfSections, numberOfProducts);
	size_t* filledProductInstances = new size_t[numberOfProducts];
	for (size_t i = 0; i < numberOfProducts; i++) filledProductInstances[i] = 0;

	for (size_t i = 0; i < numberOfProducts; i++)
	{
		currentStorage->products[i] = new Product;
		currentStorage->products[i]->read(storageFile, 0);
		currentStorage->products[i]->identificationIndex = i + 1;
	}

	for (size_t section = 0; section < numberOfSections; section++)
	{
		for (size_t shelf = 0; shelf < numberOfShelfs; shelf++)
		{
			for (size_t place = 0; place < numberOfPlaces; place++)
			{
				storageFile >> productIndex;
				if (productIndex > 0)
				{
					currentStorage->products[productIndex - 1]->positions[filledProductInstances[productIndex - 1]] = { place, shelf, section };
					filledProductInstances[productIndex - 1]++;
					for (size_t i = 0; i < currentStorage->products[productIndex - 1]->size; i++)
					{
						currentStorage->sites[section][shelf][place] = currentStorage->products[productIndex - 1];
						place++;
					}
					for (size_t i = 0; i < currentStorage->products[productIndex - 1]->size - 1; i++) storageFile >> temp;
					place--;
				}
				else currentStorage->sites[section][shelf][place] = nullptr;
			}
		}
	}
	currentStorage->numberOfProducts = numberOfProducts;

	delete[] filledProductInstances;
	storageFile.close();

	if (openedHistory) return 'S';
	else return 'H';
}

char StorageFile::createNewFile(size_t places, size_t shelfs, size_t sections, const char* storageFileAddress, const char* historyFileAddress)
{
	if (openFlag) return 'O';

	std::ofstream storageFile(storageFileAddress, std::ios::out | std::ios::trunc);
	if (!storageFile) return 'L';

	storageAddress = new char[strlen(storageFileAddress) + 1];
	strcpy(storageAddress, storageFileAddress);

	if (historyFileAddress == nullptr) historyAddress = deduceHistoryAddress(storageAddress);
	else
	{
		historyAddress = new char[strlen(historyFileAddress) + 1];
		strcpy(historyAddress, historyFileAddress);
	}
	std::ofstream historyFile(historyAddress, std::ios::out | std::ios::trunc);
	if (!historyFile) return 'H';
	historyFile.close();
	storageFile.close();

	currentStorage = new Storage(places, shelfs, sections);
	openFlag = 1;
	writeStorageAsFile();

	return 'S';
}

bool StorageFile::closeCurrentFile()
{
	if (!openFlag) return 0;
	writtenChanges = 0;
	openFlag = 0;
	delete currentStorage;
	delete[] storageAddress;
	delete[] historyAddress;
	currentStorage = nullptr;
	storageAddress = nullptr;
	historyAddress = nullptr;
	return 1;
}