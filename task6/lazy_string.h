#include <string>
#include <iostream>
#include <ostream>
#include <assert.h>
#pragma once
using namespace std;

class lazy_char;

class lazy_string {
	char* s;
	size_t len;
	bool has_reference;	
public:
	lazy_string(char *s, size_t len, bool);
	lazy_string(std::string);
	lazy_string(const lazy_string& );
	size_t size();
	size_t length();
	lazy_char at(size_t);
	lazy_char operator[](size_t);
	lazy_string substr(size_t pos, size_t len);
	operator std::string();
	lazy_string& operator=(lazy_string &source);
	~lazy_string();
	void out() {cerr << "out: " << s << ' ' << len << ' ' << has_reference << endl;}
		// printf("out: %p %d %d %p\n", s, len, has_reference, origin);}
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
	~lazy_char();
	friend std::ostream& operator<<(std::ostream&, lazy_char);
};

