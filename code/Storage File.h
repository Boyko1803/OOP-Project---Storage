#pragma once

#include "Storage.h"

#include <fstream>

class StorageFile
{
private:
	Storage* currentStorage;
	char* storageAddress;
	char* historyAddress;
	size_t writtenChanges;
	bool openFlag;

	char* deduceHistoryAddress(const char* storageAddress) const;
	void copyFileContent(const char* destinationAddress, char* sourceAddress) const;

public:
	StorageFile();
	~StorageFile();

	bool isOpen() const;
	const char* const getStorageAddress() const;
	const char* const getHistoryAddress() const;
	char printStorageChanges(const Date& fromDate, const Date& toDate, std::ostream& os = std::cout) const;
	size_t getChangedProductsQuantity(const char* name, const Date& fromDate, const Date& toDate, char actionSymbol) const;

	Storage& getStorage();
	char writeStorageAsFile(const char* storageAddress = nullptr, const char* historyAddress = nullptr);
	char openNewFile(const char* storageAddress, const char* historyAddress = nullptr);
	char createNewFile(size_t places, size_t shelfs, size_t sections, const char* storageAddress, const char* historyAddress = nullptr);
	bool closeCurrentFile();
};
