#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <ctime>

class Date
{
private:
	size_t day;
	size_t month;
	size_t year;

	bool longYear() const;
public:
	Date();
	Date(size_t day, size_t month, size_t year);

	size_t getDay() const;
	size_t getMonth() const;
	size_t getYear() const;
	bool validDate() const;

	void setDay(size_t);
	void setMonth(size_t);
	void setYear(size_t);

	bool operator==(const Date& other) const;
	bool operator<(const Date& other) const;
	bool operator>(const Date& other) const;

	friend std::ostream& operator<<(std::ostream& os, const Date& date);
	friend std::istream& operator>>(std::istream& os, Date& date);

	static Date getCurrentDate();
};