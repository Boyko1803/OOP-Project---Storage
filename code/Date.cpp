#include "Date.h"

bool Date::longYear() const
{
	if (year % 400 == 0) return 1;
	if (year % 100 == 0) return 0;
	if (year % 4 == 0) return 1;
	return 0;
}

Date::Date() :
	day(1),
	month(1),
	year(1900)
{}

Date::Date(size_t day_, size_t month_, size_t year_) :
	day(day_),
	month(month_),
	year(year_)
{}

size_t Date::getDay() const
{
	return day;
}

size_t Date::getMonth() const
{
	return month;
}

size_t Date::getYear() const
{
	return year;
}

bool Date::validDate() const
{
	if (month <= 0 || month > 12) return 0;
	if (day <= 0 || day > 31) return 0;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day == 31) return 0;
	if (month == 2)
	{
		if (day > 29) return 0;
		if (!longYear() && day == 29) return 0;
	}

	return 1;
}

void Date::setDay(size_t day_)
{
	day = day_;
}

void Date::setMonth(size_t month_)
{
	month = month_;
}

void Date::setYear(size_t year_)
{
	year = year_;
}

bool Date::operator==(const Date& other) const
{
	return (day == other.day && month == other.month && year == other.year);
}

bool Date::operator<(const Date& other) const
{
	if (year < other.year) return 1;
	else if (year > other.year) return 0;

	if (month < other.month) return 1;
	else if (month > other.month) return 0;

	if (day < other.day) return 1;
	else return 0;
}

bool Date::operator>(const Date& other) const
{
	return other<(*this);
}

Date Date::getCurrentDate()
{
	time_t sec = time(0);
	tm* now = localtime(&sec);
	return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
	os << date.year << '-';
	if (date.month < 10) os << 0;
	os << date.month << '-';
	if (date.day < 10) os << 0;
	os << date.day;

	return os;
}

std::istream& operator>>(std::istream& is, Date& date)
{
	is >> date.year;
	is.get();
	is >> date.month;
	is.get();
	is >> date.day;

	return is;
}