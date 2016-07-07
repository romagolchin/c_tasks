#include "lazy_string.h"
#define DEBUG 1
#undef DEBUG
using namespace std;

lazy_char::lazy_char(lazy_string* hs, size_t pos) :
host_str(hs), pos(pos)
{}

lazy_char::~lazy_char() {

}

lazy_char::operator char() {
	return *(host_str->s + pos);
}

lazy_char& lazy_char::operator=(char c) {
	//printf("%s %c\n", __func__, c);
	if(host_str->has_reference) {
		host_str->s = new char[host_str->len + 1];
		for(size_t i = 0; i < host_str->len; ++i)
			host_str->s[i] = host_str->origin->s[i];
		host_str->has_reference = 0;
		host_str->origin = 0;	
	}
	*(host_str->s + pos) = c;
	host_str->s[host_str->len] = '\0';
	//
	return *this;
}

std::ostream& operator<<(std::ostream& stream, lazy_char lc) {
	stream << (char) lc;
	return stream;
}

lazy_string::lazy_string(std::string str) {
	//cerr << __func__ << endl;
	//cerr << __func__ << endl;
	cerr << "from std::string" << endl;
	//fflush(stdout);

	len = str.length();
	s = new char[len + 1];
	assert(s);
	for(size_t i = 0; i < len; ++i)
		s[i] = str[i];
	s[len] = '\0';
	has_reference = 0;
	// origin = 0;
}

lazy_string::lazy_string(char *s, size_t len, bool has_reference, lazy_string* origin):
s(s), len(len), has_reference(has_reference), origin(origin)
{//cerr << __func__ << endl;
//cerr << has_reference << endl;
//this->out();
}

lazy_string::lazy_string(const lazy_string& source) {
	using namespace std;
	// #ifdef DEBUG 
	cerr << "copy constructor" << endl;
	//fflush(stdout);
	// #endif
	s = source.s;
	len = source.len;
	has_reference = 1;
	lazy_string* ptr = source;
	// origin = &source;
	// origin = ptr;
}

lazy_string& lazy_string::operator=(lazy_string& source) {
	using namespace std;
		cerr << __func__ << endl;

	#ifdef DEBUG 
	//fflush(stdout);

	this->out();
	//fflush(stdout);
	source.out();
	//fflush(stdout);

	#endif
	// #ifdef DEBUG 
	// if(!s)
	// 	//printf("null\n");
	// else
	// 	//printf("%p\n", s);
	// #endif
	// delete [] s;
	if(s) {
		delete [] s;
	}
	s = source.s;
	len = source.len;
	has_reference = 1;
	origin = &source;
	this->out();
	//fflush(stdout);
	//printf("%s\n", s);
	//fflush(stdout);

	return *this;
}

size_t lazy_string::length() {return len; }
size_t lazy_string::size() {return len; }


lazy_char lazy_string::operator[](size_t pos) {
	return lazy_char(this, pos);
}

lazy_char lazy_string::at(size_t pos) {
	if(pos >= len)
		throw out_of_range(std::to_string(pos) + " is out of range\n");
	return (*this)[pos];
}

// lazy_string::operator std::string() {
// 	cerr << "cast to string" << endl;
// 	return std::string(s).substr(0, len);
// }

lazy_string lazy_string::substr(size_t pos, size_t len) {
	//printf("%p %d %d %p\n", s, pos, len, this);
	cerr << __func__ << endl;
	if(len > this->len)
		throw length_error("failed to take substring longer than string itself\n");
	lazy_string ret(s + pos, len, 1, this);
	return ret;
}

lazy_string::~lazy_string() {
	#ifdef DEBUG 
	//cerr << __func__ << endl;
	//fflush(stdout);
	//printf("%p\n", s);
	//fflush(stdout);

	#endif
	if(!has_reference){
		delete [] s;
	}
}
