#include "lazy_string.h"


lazy_string::lazy_char::lazy_char(lazy_string* hs, size_t pos) :
	host_str(hs), pos(pos)
{}

lazy_string::lazy_char::operator char() const {
	return (*host_str->str)[host_str->start + pos];
}

lazy_string::lazy_char& lazy_string::lazy_char::operator=(char c) {
	if (host_str->str.use_count() > 1) {
		host_str->str = std::make_shared<std::string>(
			lazy_string(host_str->str, host_str->start, host_str->len)
			.str->substr(host_str->start, host_str->len));
		host_str->start = 0;
	}
	(*host_str->str)[host_str->start + pos] = c;
	return *this;
}

lazy_string::lazy_string(std::string str) :
	start(0), len(str.length()),
	str(std::make_shared<std::string>(str))
{}

lazy_string::lazy_string(std::shared_ptr<std::string> str, size_t start, size_t len):
	str(str), len(len), start(start)
{}



size_t lazy_string::length() const {
	return len;
}

size_t lazy_string::size() const {
	return len;
}


lazy_string::lazy_char lazy_string::operator[](size_t pos) {
	return lazy_char(this, pos);
}

lazy_string::lazy_char lazy_string::at(size_t pos) {
	if (pos >= len)
		throw std::out_of_range(std::to_string(pos) + " is out of range\n");
	lazy_char lc(this, pos);
	return lc;
}

lazy_string::operator std::string() const {
	// std::cout << *str << ' ' << len << std::endl;
	std::string res = str->substr(start, len);
	return res;
}

lazy_string lazy_string::substr(size_t pos, size_t len) const {
	if (pos >= this->len)
		throw std::out_of_range("start position of substring is out of range\n");
	lazy_string res(str, start + pos, std::min(this->len - pos, len));
	return res;
}


std::istream& operator>>(std::istream& stream, lazy_string& ls) {
	std::shared_ptr<std::string> string = std::make_shared<std::string>();
	stream >> *string;
	ls.str = string;
	ls.len = string->length();
	return stream;
}

std::ostream& operator<< (std::ostream& stream, lazy_string& ls) {
	stream << static_cast<std::string>(ls);
	return stream;
}

