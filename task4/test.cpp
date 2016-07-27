#include <stdio.h>
#include <sstream>

#include "format.cpp"


int main() {
    try {
        string f = "aba%%caba%100dxyz";
        string res = format(f, 42);
        cout << res << endl;
        cout << format("Characters: %c %c \n", 'a', 65);
        cout << format("Decimals: %d %ld\n", 1977, 650000L);
        cout << format("Preceding with blanks: %10d \n", 1977);
        cout << format("Preceding with blanks: %010d \n", 1977);
        cout << format("Some different radices: %d %x %o %#o %#x \n", 100, 100, 100, 100, 100);
        cout << format("floats: %4.2f %+.100e %E \n", 3.1416, 3.1416, 3.1416);
        cout << format("Width trick: %*d %d\n", -5, 10, 11);
        cout << format("%s \n", string("A string"));
        cout << format(string("a%+0%100d "), 123);
    }
    catch (const exception& e) {
        cout << e.what() << std::endl;
    }
    catch(string s) {
        cout << s << endl;
    }
}
