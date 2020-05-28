#pragma once

#include "Product.h"

class StorageFile;

class Storage
{
private:
	const static size_t DEFAULT_NUMBER_OF_PLACES = 50;
	const static size_t DEFAULT_NUMBER_OF_SHELFS = 5;
	const static size_t DEFAULT_NUMBER_OF_SECTIONS = 100;
	const static size_t INITIAL_LIMIT_OF_PRODUCTS = 16;
	const static size_t INITIAL_LIMIT_OF_CHANGES = 16;

	struct ProductChange
	{
		Product product;
		Date changeDate;
		size_t quantity;
		char action;

		ProductChange(const Product& product = Product(), const Date& changeDate = Date::getCurrentDate(), size_t quantity = 0, char actionSymbol = '\0');

		void write(std::ostream& os) const;
		void read(std::istream& is);
	};

	size_t numberOfPlaces;
	size_t numberOfShelfs;
	size_t numberOfSections;
	size_t numberOfProducts;
	size_t limitOfProducts;
	Product**** sites;
	Product** products;

	ProductChange* changes;
	size_t numberOfChanges;
	size_t limitOfChanges;


	void expandProducts();
	void expandChanges();
	void addToChanges(size_t productIndex, size_t quantity, char actionSymbol);

	size_t findInstanceStartingPlace(size_t place, size_t shelf, size_t section) const;
	size_t findInstanceIndex(size_t place, size_t shelf, size_t section) const;
	size_t fillSites(Product& other);

public:
	Storage();
	Storage(size_t numberOfPlaces, size_t numberOfShelfs, size_t numberOfSections, size_t expectedNumberOfProducts = 0);
	~Storage();

	size_t getNumberOfPlaces() const;
	size_t getNumberOfShelfs() const;
	size_t getNumberOfSections() const;
	size_t getNumberOfProducts() const;
	size_t getSimilarProductsQuantity(const char* name) const;
	void printExpirationDates(const char* name, std::ostream& os = std::cout) const;
	void printLastChanges(size_t numberOfChanges, std::ostream& os = std::cout) const;
	void printAllChanges(std::ostream& os = std::cout) const;
	void printProducts(std::ostream& os = std::cout) const;
	void printProductsByName(std::ostream& os = std::cout) const;

	void reduceProductQuantity(size_t productIndex, size_t quantity, char actionSymbol = 'R');
	void removeProduct(size_t productIndex, char actionSymbol = 'X');
	bool removeProductInstance(size_t place, size_t shelf, size_t section, char actionSymbol = 'I');
	void reduceProductsQuantityByName(const char* name, size_t quantity);
	size_t cleanProducts(const Date& = Date::getCurrentDate(), char actionSymbol = 'C');
	bool addProduct(const Product& product, char actionSymbol = 'A');

	friend std::ostream& operator<<(std::ostream& os, const ProductChange& product);

	friend class StorageFile;
};
