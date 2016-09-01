#include "lazy_string.h"

std::mutex share_stream::mx;

void bp(int n) {
	// std::cout << n << std::endl;
	fflush(stdout);
}

rw_lock::rw_lock(): n_readers(0)
{}

rw_lock::rw_lock(const rw_lock&): n_readers(0)
{}

void rw_lock::read_lock() {
	mutex.lock();
	n_readers++;
	mutex.unlock();
}

void rw_lock::read_unlock() {
	n_readers--;
}

void rw_lock::write_lock() {
	mutex.lock();
	while (n_readers.load() > 0) {
		std::this_thread::yield();
	}
}

void rw_lock::write_unlock() {
	this->mutex.unlock();
}

lazy_string::lazy_char::lazy_char(lazy_string* hs, size_t pos) :
	host_str(hs), pos(pos)
{}

lazy_string::lazy_char::operator char() const {
	return (*host_str->str)[host_str->start + pos];
}

lazy_string::lazy_char& lazy_string::lazy_char::operator=(char c) {
	bp(0);
	host_str->lock->write_lock();
	bp(1);
	std::shared_ptr<rw_lock> old_lock = host_str->lock;
	bp(2);
	if (host_str->str.use_count() > 1) {
		bp(-1);
		// host_str->str = std::make_shared<std::string>(
		// 	lazy_string(host_str->str, host_str->lock, host_str->start, host_str->len)
		// 	.str->substr(host_str->start, host_str->len));
		host_str->str = std::make_shared<std::string>(
		                    host_str->str->substr(host_str->start, host_str->len));
		host_str->start = 0;
		host_str->lock = std::make_shared<rw_lock>();
	}
	(*host_str->str)[host_str->start + pos] = c;
	bp(3);
	old_lock->write_unlock();
	bp(4);

	return *this;
}

lazy_string::lazy_string(std::string str) :
	start(0), len(str.length()),
	lock(std::make_shared<rw_lock>()),
	str(std::make_shared<std::string>(str))
{}

lazy_string::lazy_string(std::shared_ptr<std::string> str, std::shared_ptr<rw_lock> lock, size_t start, size_t len):
	str(str), len(len), start(start), lock(lock)
{}



size_t lazy_string::length() const {
	this->lock->read_lock();
	size_t l = this->len;
	this->lock->read_unlock();
	return l;
}

size_t lazy_string::size() const {
	lock->read_lock();
	size_t l = len;
	lock->read_unlock();
	return l;
}


lazy_string::lazy_char lazy_string::operator[](size_t pos) {
	return lazy_char(this, pos);
}

lazy_string::lazy_char lazy_string::at(size_t pos) {
	lock->read_lock();
	if (pos >= len)
		throw std::out_of_range(std::to_string(pos) + " is out of range\n");
	lazy_char lc(this, pos);
	lock->read_unlock();
	return lc;
}

lazy_string::operator std::string() const {
	// std::cout << *str << ' ' << len << std::endl;
	lock->read_lock();
	std::string res = str->substr(start, len);
	lock->read_unlock();
	return res;
}

lazy_string lazy_string::substr(size_t pos, size_t len) const {
	lock->read_lock();
	if (pos >= this->len)
		throw std::out_of_range("start position of substring is out of range\n");
	lazy_string res(str, lock, start + pos, std::min(this->len - pos, len));
	lock->read_unlock();
	return res;
}


std::istream& operator>>(std::istream& stream, lazy_string& ls) {
	ls.lock->write_lock();
	std::shared_ptr<std::string> string = std::make_shared<std::string>();
	stream >> *string;
	ls.str = string;
	ls.len = string->length();
	std::shared_ptr<rw_lock> old_lock = ls.lock;
	ls.lock = std::make_shared<rw_lock>();
	old_lock->write_unlock();
	return stream;
}

std::ostream& operator<< (std::ostream& stream, lazy_string& ls) {
	ls.lock->read_lock();
	stream << static_cast<std::string>(ls);
	ls.lock->read_unlock();
	return stream;
}

