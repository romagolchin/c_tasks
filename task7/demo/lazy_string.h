#ifndef ITMO_CPP_LAZY_STRING_H
#define ITMO_CPP_LAZY_STRING_H
#include <string>
#include <iostream>
#include <ostream>
#include <assert.h>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>
#include <algorithm>
#include <atomic>


class rw_lock {
	std::mutex mutex;
	std::atomic<int> n_readers;
public:
	rw_lock();
	rw_lock(const rw_lock&);
	void read_lock();
	void read_unlock();
	void write_lock();
	void write_unlock();
};

class lazy_string {
	struct lazy_char {
		lazy_string* host_str;
		size_t pos;
		lazy_char(lazy_string*, size_t);
		operator char() const;
		lazy_char& operator=(char c) ;
	};
	std::shared_ptr<std::string> str;
	std::shared_ptr<rw_lock> lock;
	size_t start;
	size_t len;
	lazy_string(std::shared_ptr<std::string>, std::shared_ptr<rw_lock>,
		size_t, size_t len);
public:
	lazy_string(std::string);
	lazy_string(const lazy_string& );
	size_t size() const;
	size_t length() const;
	lazy_char at(size_t);
	lazy_char operator[](size_t);
	lazy_string substr(size_t pos, size_t len) const;
	operator std::string() const;
	friend class lazy_char;
	friend std::istream& operator>>(std::istream&, lazy_string&);
	friend std::ostream& operator<<(std::ostream&, lazy_string&);
};

class share_stream {
	static std::mutex mx;
	friend std::ostream& operator<<(std::ostream&, lazy_string&);
	friend std::istream& operator>>(std::istream&, lazy_string&);
};

#endif //ITMO_CPP_LAZY_STRING_H