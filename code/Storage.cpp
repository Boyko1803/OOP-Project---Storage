#include "Storage.h"

Storage::ProductChange::ProductChange(const Product& product_, const Date& changeDate_, size_t quantity_, char action_) :
	product(product_),
	changeDate(changeDate_),
	quantity(quantity_),
	action(action_)
{}

void Storage::ProductChange::write(std::ostream& os) const
{
	os << std::endl;
	os << changeDate << ' ';
	os << action << ' ';
	os << quantity << ' ';
	product.write(os, 1);
}

void Storage::ProductChange::read(std::istream& is)
{
	is >> changeDate;
	is >> action;
	is >> quantity;
	product.read(is, 1);
}

void Storage::expandProducts()
{
	Product** temp = new Product*[2 * limitOfProducts];
	for (size_t i = 0; i < limitOfProducts; i++) temp[i] = products[i];
	for (size_t i = limitOfProducts; i < 2 * limitOfProducts; i++) temp[i] = nullptr;
	delete[] products;
	products = temp;
	limitOfProducts *= 2;
}

void Storage::expandChanges()
{
	ProductChange* temp = new ProductChange[2 * limitOfChanges];
	for (size_t i = 0; i < numberOfChanges; i++) temp[i] = changes[i];
	delete[] changes;
	changes = temp;
	limitOfChanges *= 2;
}

void Storage::addToChanges(size_t productIndex, size_t quantity, char action)
{
	if (action == 'N') return;
	if (numberOfChanges >= limitOfChanges) expandChanges();

	size_t temp = products[productIndex]->quantity;
	products[productIndex]->quantity = quantity;
	changes[numberOfChanges] = ProductChange{ *products[productIndex], Date::getCurrentDate(), quantity, action };
	products[productIndex]->quantity = temp;
	for (size_t i = 0; i < quantity; i++) changes[numberOfChanges].product.positions[i] = products[productIndex]->positions[temp - quantity + i];
	numberOfChanges++;
}

size_t Storage::findInstanceStartingPlace(size_t place, size_t shelf, size_t section) const
{

	Product* product = sites[section][shelf][place];
	if (product->size == 1) return place;

	for (size_t i = 0; i < product->quantity; i++)
	{
		if (product->positions[i].section == section && product->positions[i].shelf == shelf)
		{
			if (place >= product->positions[i].place && (place - product->positions[i].place) < product->size) return product->positions[i].place;
		}
	}
	return 0;
}

size_t Storage::findInstanceIndex(size_t place, size_t shelf, size_t section) const
{
	Product* product = sites[section][shelf][place];

	for (size_t i = 0; i < product->quantity; i++)
	{
		if (product->positions[i].section == section && product->positions[i].shelf == shelf)
		{
			if (place >= product->positions[i].place && (place - product->positions[i].place) < product->size) return i;
		}
	}
	return 0;
}

size_t Storage::fillSites(Product& product)
{
	size_t freePlaces, filled = 0;
	bool sameExpireDate, differentExpireDate;

	for (int i = numberOfSections - 1; i >= 0; i--)
	{
		for (int j = numberOfShelfs - 1; j >= 0; j--)
		{
			freePlaces = 0;
			sameExpireDate = 0;
			differentExpireDate = 0;
			for (int k = numberOfPlaces - 1; k >= 0; k--)
			{
				if (sites[i][j][k] != nullptr)
				{
					freePlaces = 0;
					if ((product == (*sites[i][j][k])) == 1) differentExpireDate = 1;
					else if ((product == (*sites[i][j][k])) == 2)
					{
						sameExpireDate = 1;
						differentExpireDate = 0;
					}
					else
					{
						sameExpireDate = 0;
						differentExpireDate = 0;
					}
				}
				else
				{
					if (!differentExpireDate && sameExpireDate) freePlaces++;
					if (freePlaces == product.size)
					{
						if (k == 0 || sites[i][j][k - 1] == nullptr || ((*sites[i][j][k - 1]) == product) != 1)
						{
							freePlaces = 0;
							product.positions[filled] = { (size_t)k, (size_t)j, (size_t)i };
							for (size_t l = 0; l < product.size; l++) sites[i][j][k + l] = &product;
							filled++;
						}
					}
					if (filled == product.quantity)
					{
						i = 0;
						j = 0;
						k = 0;
					}
				}
			}
		}
	}

	if (filled == product.quantity) return filled;

	for (size_t i = 0; i < numberOfSections; i++)
	{
		for (size_t j = 0; j < numberOfShelfs; j++)
		{
			freePlaces = 0;
			sameExpireDate = 0;
			differentExpireDate = 0;
			for (size_t k = 0; k < numberOfPlaces; k++)
			{
				if (sites[i][j][k] != nullptr)
				{
					freePlaces = 0;
					if ((product == (*sites[i][j][k])) == 1) differentExpireDate = 1;
					else if ((product == (*sites[i][j][k])) == 2)
					{
						sameExpireDate = 1;
						differentExpireDate = 0;
					}
					else
					{
						sameExpireDate = 0;
						differentExpireDate = 0;
					}
				}
				else
				{
					if (!differentExpireDate && sameExpireDate) freePlaces++;
					if (freePlaces == product.size)
					{
						if (k == numberOfPlaces - 1 || sites[i][j][k + 1] == nullptr || ((*sites[i][j][k + 1]) == product) != 1)
						{
							freePlaces = 0;
							product.positions[filled] = { k - product.size + 1, j, i };
							for (size_t l = 0; l < product.size; l++) sites[i][j][k - l] = &product;
							filled++;
						}
					}
					if (filled == product.quantity)
					{
						i = numberOfSections;
						j = numberOfShelfs;
						k = numberOfPlaces;
					}
				}
			}
		}
	}

	if (filled == product.quantity) return filled;

	for (size_t i = 0; i < numberOfSections; i++)
	{
		for (size_t j = 0; j < numberOfShelfs; j++)
		{
			freePlaces = 0;
			sameExpireDate = 0;
			differentExpireDate = 0;
			for (size_t k = 0; k < numberOfPlaces; k++)
			{
				if (sites[i][j][k] != nullptr)
				{
					freePlaces = 0;
					if ((product == (*sites[i][j][k])) == 1) differentExpireDate = 1;
					else differentExpireDate = 0;
				}
				else
				{
					if (differentExpireDate) differentExpireDate = 0;
					else
					{
						freePlaces++;
						if (freePlaces == product.size)
						{
							if (k == numberOfPlaces - 1 || sites[i][j][k + 1] == nullptr || ((*sites[i][j][k + 1]) == product) != 1)
							{
								freePlaces = 0;
								product.positions[filled] = { k - product.size + 1, j, i };
								for (size_t l = 0; l < product.size; l++) sites[i][j][k - l] = &product;
								filled++;
							}
						}
						if (filled == product.quantity)
						{
							i = numberOfSections;
							j = numberOfShelfs;
							k = numberOfPlaces;
						}
					}
				}
			}
		}
	}

	return filled;
}

Storage::Storage() :
	numberOfPlaces(DEFAULT_NUMBER_OF_PLACES),
	numberOfShelfs(DEFAULT_NUMBER_OF_SHELFS),
	numberOfSections(DEFAULT_NUMBER_OF_SECTIONS),
	numberOfProducts(0),
	limitOfProducts(INITIAL_LIMIT_OF_PRODUCTS),
	numberOfChanges(0),
	limitOfChanges(INITIAL_LIMIT_OF_CHANGES)
{
	products = new Product*[limitOfProducts];
	for (size_t i = 0; i < limitOfProducts; i++) products[i] = nullptr;

	changes = new ProductChange[limitOfChanges];

	sites = new Product***[DEFAULT_NUMBER_OF_SECTIONS];
	for (size_t i = 0; i < DEFAULT_NUMBER_OF_SECTIONS; i++)
	{
		sites[i] = new Product**[DEFAULT_NUMBER_OF_SHELFS];
		for (size_t j = 0; j < DEFAULT_NUMBER_OF_SHELFS; j++)
		{
			sites[i][j] = new Product*[DEFAULT_NUMBER_OF_PLACES];
			for (size_t k = 0; k < DEFAULT_NUMBER_OF_PLACES; k++) sites[i][j][k] = nullptr;
		}
	}
}

Storage::Storage(size_t numberOfPlaces_, size_t numberOfShelfs_, size_t numberOfSections_, size_t expectedNumberOfProducts) :
	numberOfPlaces(numberOfPlaces_),
	numberOfShelfs(numberOfShelfs_),
	numberOfSections(numberOfSections_),
	numberOfProducts(0),
	numberOfChanges(0),
	limitOfChanges(INITIAL_LIMIT_OF_CHANGES)
{
	limitOfProducts = (2 * expectedNumberOfProducts > INITIAL_LIMIT_OF_PRODUCTS) ? (2 * expectedNumberOfProducts) : INITIAL_LIMIT_OF_PRODUCTS;
	products = new Product*[limitOfProducts];
	for (size_t i = 0; i < limitOfProducts; i++) products[i] = nullptr;

	changes = new ProductChange[limitOfChanges];

	sites = new Product***[numberOfSections];
	for (size_t i = 0; i < numberOfSections; i++)
	{
		sites[i] = new Product**[numberOfShelfs];
		for (size_t j = 0; j < numberOfShelfs; j++)
		{
			sites[i][j] = new Product*[numberOfPlaces];
			for (size_t k = 0; k < numberOfPlaces; k++) sites[i][j][k] = nullptr;
		}
	}
}

Storage::~Storage()
{
	for (size_t i = 0; i < numberOfProducts; i++) if (products[i] != nullptr) delete products[i];
	delete[] products;
	delete[] changes;

	for (size_t i = 0; i < numberOfSections; i++)
	{
		for (size_t j = 0; j < numberOfShelfs; j++) delete[] sites[i][j];
		delete[] sites[i];
	}
	delete[] sites;
}

size_t Storage::getNumberOfPlaces() const
{
	return numberOfPlaces;
}

size_t Storage::getNumberOfShelfs() const
{
	return numberOfShelfs;
}

size_t Storage::getNumberOfSections() const
{
	return numberOfSections;
}

size_t Storage::getNumberOfProducts() const
{
	return numberOfProducts;
}

size_t Storage::getSimilarProductsQuantity(const char* name) const
{
	size_t quantity = 0;

	for (size_t i = 0; i < numberOfProducts; i++) if ((*products[i]) == name) quantity += products[i]->quantity;

	return quantity;
}

void Storage::printExpirationDates(const char* name, std::ostream& os) const
{
	for (size_t i = 0; i < numberOfProducts; i++)
	{
		if ((*products[i]) == name)
		{
			os << "Expiration date - " << (*products[i]).expireDate << " ; " << "Quantity - " << (*products[i]).quantity << std::endl;
		}
	}
}

void Storage::printLastChanges(size_t number, std::ostream& os) const
{
	number = (numberOfChanges < number) ? numberOfChanges : number;

	for (size_t i = numberOfChanges - number; i < numberOfChanges; i++) os << changes[i] << std::endl;
}

void Storage::printAllChanges(std::ostream& os) const
{
	printLastChanges(numberOfChanges, os);
}

void Storage::printProducts(std::ostream& os) const
{
	for (size_t i = 0; i < numberOfProducts; i++) os << (*products[i]) << std::endl;
}

void Storage::printProductsByName(std::ostream& os) const
{
	size_t totalQuantity, locationIndex;
	bool* visited = new bool[numberOfProducts];
	for (size_t i = 0; i < numberOfProducts; i++) visited[i] = 0;

	for (size_t i = 0; i < numberOfProducts; i++)
	{
		if (visited[i]) continue;

		totalQuantity = 0;
		for (size_t j = i; j < numberOfProducts; j++)
		{
			if ((*products[i]) == (*products[j]))
			{
				totalQuantity += products[j]->quantity;
				visited[j] = 1;
			}
		}

		os << "Name: " << products[i]->name << std::endl;
		os << "Quantity: " << totalQuantity << std::endl;

		locationIndex = 1;
		if (totalQuantity == 1) os << "Location: " << std::endl;
		else os << "Locations: " << std::endl;
		for (size_t j = i; j < numberOfProducts; j++)
		{
			if ((*products[i]) == (*products[j]))
			{
				for (size_t k = 0; k < products[j]->quantity; k++)
				{
					os << locationIndex << " - ";
					os << "Section " << products[j]->positions[k].section + 1;
					os << " ; Shelf " << products[j]->positions[k].shelf + 1;
					os << " ; Place " << products[j]->positions[k].place + 1;
					os << std::endl;
					locationIndex++;
				}
			}
		}
		os << std::endl;
	}

	delete[] visited;
}

void Storage::reduceProductQuantity(size_t productIndex, size_t quantity, char actionSymbol)
{
	if (quantity >= products[productIndex]->quantity)
	{
		removeProduct(productIndex, actionSymbol);
		return;
	}

	size_t section, shelf, place;

	for (size_t i = products[productIndex]->quantity - quantity; i < products[productIndex]->quantity; i++)
	{
		section = products[productIndex]->positions[i].section;
		shelf = products[productIndex]->positions[i].shelf;
		place = products[productIndex]->positions[i].place;
		for (size_t j = 0; j < products[productIndex]->size; j++) sites[section][shelf][place + j] = nullptr;
	}

	addToChanges(productIndex, quantity, actionSymbol);

	products[productIndex]->quantity -= quantity;
}

void Storage::removeProduct(size_t productIndex, char actionSymbol)
{
	size_t section, shelf, place;

	for (size_t i = 0; i < products[productIndex]->quantity; i++)
	{
		section = products[productIndex]->positions[i].section;
		shelf = products[productIndex]->positions[i].shelf;
		place = products[productIndex]->positions[i].place;
		for (size_t j = 0; j < products[productIndex]->size; j++) sites[section][shelf][place + j] = nullptr;
	}

	addToChanges(productIndex, products[productIndex]->quantity, actionSymbol);

	Product* temp = products[productIndex];
	products[productIndex] = products[numberOfProducts - 1];
	products[productIndex]->identificationIndex = productIndex + 1;
	products[numberOfProducts - 1] = nullptr;
	numberOfProducts--;
	delete temp;
}

bool Storage::removeProductInstance(size_t place, size_t shelf, size_t section, char actionSymbol)
{
	Product* product = sites[section][shelf][place];
	if (product == nullptr) return 0;

	size_t positionIndex = findInstanceIndex(place, shelf, section);
	place = product->positions[positionIndex].place;

	Product::Position temp = product->positions[positionIndex];
	product->positions[positionIndex] = product->positions[product->quantity - 1];
	product->positions[product->quantity - 1] = temp;

	reduceProductQuantity(product->identificationIndex - 1, 1, actionSymbol);

	return 1;
}

void Storage::reduceProductsQuantityByName(const char* name, size_t quantity)
{
	size_t numberOfSimilarProducts = 0, productIndex = 0, similarProductIndex = 0;
	size_t productQuantity;

	for (size_t i = 0; i < numberOfProducts; i++) if ((*products[i]) == name) numberOfSimilarProducts++;
	Product** productLocations = new Product*[numberOfSimilarProducts + 2];
	for (size_t i = 0; i < numberOfSimilarProducts; i++) productLocations[i] = products[0];

	while (similarProductIndex < numberOfSimilarProducts)
	{
		if ((*products[productIndex]) == name)
		{
			productLocations[similarProductIndex] = products[productIndex];
			similarProductIndex++;
		}
		productIndex++;
	}

	Product* temp;
	for (size_t i = 0; i < numberOfSimilarProducts; i++)
	{
		for (size_t j = i + 1; j < numberOfSimilarProducts; j++)
		{
			if (productLocations[i]->expireDate > productLocations[j]->expireDate)
			{
				temp = productLocations[i];
				productLocations[i] = productLocations[j];
				productLocations[j] = temp;
			}
		}
	}

	for (size_t i = 0; i < numberOfSimilarProducts; i++)
	{
		productQuantity = productLocations[i]->quantity;
		reduceProductQuantity(productLocations[i]->identificationIndex - 1, quantity);
		if (productQuantity >= quantity) break;
		else quantity -= productQuantity;
	}

	delete[] productLocations;
}

size_t Storage::cleanProducts(const Date& limitDate, char actionSymbol)
{
	size_t cleanedProducts = 0, productIndex = 0;

	while (productIndex < numberOfProducts)
	{
		if (products[productIndex]->expireDate < limitDate || products[productIndex]->expireDate == limitDate)
		{
			removeProduct(productIndex, 'C');
			cleanedProducts++;
		}
		else productIndex++;
	}

	return cleanedProducts;
}

bool Storage::addProduct(const Product& product, char actionSymbol)
{
	size_t filled;
	Product* temp = new Product(product);

	temp->identificationIndex = numberOfProducts + 1;
	products[numberOfProducts] = temp;
	numberOfProducts++;

	filled = fillSites(*temp);
	if (filled == product.quantity)
	{
		addToChanges(numberOfProducts - 1, products[numberOfProducts - 1]->quantity, actionSymbol);
		return 1;
	}
	temp->quantity = filled;
	removeProduct(numberOfProducts - 1, 'N');
	return 0;
}

std::ostream& operator<<(std::ostream& os, const Storage::ProductChange& change)
{
	os << "Change date: " << change.changeDate << std::endl;
	os << "Action: ";
	switch (change.action)
	{
	case 'A': os << "Added"; break;
	case 'R': os << "Removed quantity"; break;
	case 'I': os << "Removed instance"; break;
	case 'X': os << "Removed product"; break;
	case 'C': os << "Cleaned"; break;
	default: os << "Unknown";
	}
	os << std::endl;
	os << change.product;

	return os;
}