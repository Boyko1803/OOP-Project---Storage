#pragma once

#include "Date.h"

#include <string.h>

class Storage;
class StorageFile;

class Product
{
private:
	struct Position
	{
		size_t place;
		size_t shelf;
		size_t section;
	};

	Date arriveDate;
	Date expireDate;
	Position* positions;
	char* name;
	char* producer;
	char* comment;
	size_t size;
	size_t quantity;
	size_t identificationIndex;
	char measureUnit;

	void copyData(const Product& other);
	void deleteData();

public:
	Product();
	Product(char* name, char* producer, char* comment, size_t size, size_t quantity, char measureUnit, const Date& expireDate, const Date& arriveDate = Date::getCurrentDate());
	Product(const Product& other);
	Product& operator=(const Product& other);
	~Product();

	int operator==(const Product& other) const;
	bool operator==(const char* name) const;

	void write(std::ostream& os, bool readPositions = 0) const;
	void read(std::istream& is, bool readPositions = 0);

	friend std::ostream& operator<<(std::ostream& os, const Product& other);

	friend class Storage;
	friend class StorageFile;
};