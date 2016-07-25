#include <stdio.h>
#include <sstream>
#include "format.cpp"


int main() {
    try {
        string f = "aba%%caba%100dxyz";
        string res = format(f);
        cout << res << endl;
    }
    catch (const exception& e) {
        cout << e.what() << std::endl;
    }
    catch(string s) {
        cout << s << endl;
    }
    // cout << format("Characters: %c %c \n", 'a', 65);
    // cout << format("Decimals: %d %ld\n", 1977, 650000L);
    // cout << format("Preceding with blanks: %10d \n", 1977);
    // cout << format("Preceding with blanks: %010d \n", 1977);
    // cout << format("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    // cout << format("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    // cout << format("Width trick: %*d \n", 5, 10);
    // cout << format("%s \n", "A string");
}
