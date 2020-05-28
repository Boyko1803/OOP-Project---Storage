#include "Product.h"

void Product::copyData(const Product& other)
{
	arriveDate = other.arriveDate;
	expireDate = other.expireDate;
	size = other.size;
	quantity = other.quantity;
	measureUnit = other.measureUnit;

	if (other.name != nullptr)
	{
		name = new char[strlen(other.name) + 1];
		strcpy(name, other.name);
	}
	else name = nullptr;

	if (other.producer != nullptr)
	{
		producer = new char[strlen(other.producer) + 1];
		strcpy(producer, other.producer);
	}
	else producer = nullptr;

	if (other.comment != nullptr)
	{
		comment = new char[strlen(other.comment) + 1];
		strcpy(comment, other.comment);
	}
	else comment = nullptr;

	identificationIndex = 0;
	if (quantity > 0) positions = new Position[quantity];
	else positions = nullptr;
}

void Product::deleteData()
{
	if (positions != nullptr) delete[] positions;
	if (name != nullptr) delete[] name;
	if (producer != nullptr) delete[] producer;
	if (comment != nullptr) delete[] comment;
}

Product::Product() :
	arriveDate(),
	expireDate(),
	positions(nullptr),
	name(nullptr),
	producer(nullptr),
	comment(nullptr),
	size(0),
	quantity(0),
	identificationIndex(0),
	measureUnit('U')
{}

Product::Product(char* name_, char* producer_, char* comment_, size_t size_, size_t quantity_, char unit, const Date& expire, const Date& arrive) :
	arriveDate(arrive),
	expireDate(expire),
	size(size_),
	quantity(quantity_),
	identificationIndex(0),
	measureUnit(unit)
{
	name = new char[strlen(name_) + 1];
	strcpy(name, name_);
	producer = new char[strlen(producer_) + 1];
	strcpy(producer, producer_);
	comment = new char[strlen(comment_) + 1];
	strcpy(comment, comment_);

	if (quantity > 0) positions = new Position[quantity];
	else positions = nullptr;
}

Product::Product(const Product& other)
{
	copyData(other);
}

Product& Product::operator=(const Product& other)
{
	if (this != &other)
	{
		deleteData();
		copyData(other);
	}

	return *this;
}

Product::~Product()
{
	deleteData();
}

int Product::operator==(const Product& other) const
{
	if (name == nullptr || other.name == nullptr || strcmp(name, other.name)) return 0;
	else if (expireDate == other.expireDate) return 2;
	return 1;
}

bool Product::operator==(const char* otherName) const
{
	if (name == nullptr || otherName == nullptr || strcmp(name, otherName)) return 0;
	return 1;
}

void Product::write(std::ostream& os, bool includePositions) const
{
	os << arriveDate << ' ' << expireDate << ' ';
	if (name != nullptr) os << strlen(name) + 1 << ' ' << name << ' ';
	if (producer != nullptr) os << strlen(producer) + 1 << ' ' << producer << ' ';
	if (comment != nullptr) os << strlen(comment) + 1 << ' ' << comment << ' ';
	os << size << ' ';
	os << quantity << ' ';
	os << measureUnit << std::endl;

	if (includePositions)
	{
		for (size_t i = 0; i < quantity; i++) os << positions[i].section << ' ' << positions[i].shelf << ' ' << positions[i].place << std::endl;
	}
}

void Product::read(std::istream& is, bool readPositions)
{
	int requiredSpace;

	deleteData();

	is >> arriveDate >> expireDate;

	is >> requiredSpace;
	is.ignore();
	name = new char[requiredSpace];
	is.read(name, requiredSpace);
	name[requiredSpace - 1] = '\0';

	is >> requiredSpace;
	is.ignore();
	producer = new char[requiredSpace];
	is.read(producer, requiredSpace);
	producer[requiredSpace - 1] = '\0';

	is >> requiredSpace;
	is.ignore();
	comment = new char[requiredSpace];
	is.read(comment, requiredSpace);
	comment[requiredSpace - 1] = '\0';

	is >> size;
	is >> quantity;
	is >> measureUnit;

	positions = new Position[quantity];
	if (readPositions)
	{
		for (size_t i = 0; i < quantity; i++) is >> positions[i].section >> positions[i].shelf >> positions[i].place;
	}
}

std::ostream& operator<<(std::ostream& os, const Product& product)
{
	os << "Name: " << product.name << std::endl;
	os << "Producer: " << product.producer << std::endl;
	os << "Arrive date: " << product.arriveDate << std::endl;
	os << "Expire date: " << product.expireDate << std::endl;
	os << "Size of product: " << product.size << std::endl;
	os << "Quantity: " << product.quantity << std::endl;
	os << "Measurement unit: ";
	switch (product.measureUnit)
	{
	case 'K': os << "Kilograms"; break;
	case 'L': os << "Litres"; break;
	default: os << "None";
	}

	if (product.quantity == 1) os << std::endl << "Location: " << std::endl;
	else os << std::endl << "Locations: " << std::endl;
	for (size_t i = 0; i < product.quantity; i++)
	{
		os << i + 1 << " - ";
		os << "Section " << product.positions[i].section + 1;
		os << " ; Shelf " << product.positions[i].shelf + 1;
		os << " ; Place " << product.positions[i].place + 1;
		os << std::endl;
	}

	return os;
}