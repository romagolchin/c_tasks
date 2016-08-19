#ifndef ITMO_CPP_LAZY_STRING_H
#define ITMO_CPP_LAZY_STRING_H
#include <string>
#include <iostream>
#include <ostream>
#include <assert.h>
#include <stdexcept>
using namespace std;

class lazy_char;

class lazy_string {
	char* s;
	size_t len;
	bool has_reference;	//true iff this->s points to memory of another instance
	lazy_string(char *s, size_t len, bool);
public:
	lazy_string(std::string);
	lazy_string(const lazy_string& );
	size_t size();
	size_t length();
	lazy_char at(size_t);
	lazy_char operator[](size_t) const;
	lazy_string substr(size_t pos, size_t len);
	operator std::string();
	lazy_string& operator=(const lazy_string &source);
	~lazy_string();
	friend class lazy_char;
	friend istream& operator>>(istream&, lazy_string&);
	friend ostream& operator<<(ostream&, lazy_string&);
};

struct lazy_char {
	lazy_string* host_str;
	size_t pos;
	lazy_char(lazy_string*, size_t);
	operator char();
	lazy_char& operator=(char c);
	friend std::ostream& operator<<(std::ostream&, lazy_char);
};

#endif //ITMO_CPP_LAZY_STRING_H