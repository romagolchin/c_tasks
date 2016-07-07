#include "lazy_string.cpp"

int main(int argc, char const *argv[]) {
	using namespace std;
	string str = "abacaba";
	string astr = "e";
	lazy_string s(str);
	s.out();
	lazy_string t(astr);
	// printf("%p\n", t.get_s());
	t = s;
	lazy_char lc = s[1];
	// for(int i = 0; i < t.size(); ++i)
	// 	printf("%c", (char) t[i]);
	// printf("\n");

	// cout << t[1] << endl;
	// printf("%p %p\n", t.get_s(), s.get_s());
	t[3] = 'd';
	// printf("%p %p\n", t.get_s(), s.get_s());
	// (t.substr(1, 4)).out();

	// try {
	// 	lazy_string r(t.substr(1, 4));
	// }
	// catch (const length_error le) {
	// 	cerr << le.what() << endl;
	// }
	cerr << "Expected" << endl;
	(lazy_string(t.get_s() + 1, 4, 1, &t)).out();
	cerr << "Reality" << endl;
	// lazy_string r = t;
	// const lazy_string& ref = t.substr(1, 4);
	// lazy_string r (ref);
	/*lazy_string r = */t.substr(1, 4);
	// r.out();

	// printf("%p %p\n", t.get_s(), r.get_s());
	// printf("%c\n", (char) t[2]);
	// printf("%c\n", (char) t[3]);
	// printf("%d\n", (int) t.size());

	// string my_str = (string) r;
	// cout << my_str << endl;
	// r.out();
	// printf("%c\n", (char) r[3]);
	// try {
	// 	printf("%c\n", (char) r.at(4));
	// }
	// catch(const out_of_range& oor) {
	// 	cerr << oor.what() << endl;
	// }
	return 0;
}