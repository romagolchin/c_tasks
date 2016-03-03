#include <stdlib.h>

class rational {
    int num, denom;
    int gcd(int a, int b);
public:
    rational(int n);

    rational(int num, int denom);

    int getNum();

    int getDenom();

    rational operator+(rational &frac);

    rational operator-(rational &frac);

    rational operator*(rational &frac);

    rational operator/(rational &frac);
};

rational::rational(int n) {
    num = n;
    denom = 1;
}

rational::rational(int num, int denom) {
    int d = gcd(num, denom);
    this->num = num / d;
    this->denom = denom / d;
}

int rational::getNum() {
    return num;
}

int rational::getDenom() {
    return denom;
}

rational rational::operator+(rational &frac) {
    return rational(this->num * frac.denom + this->denom * frac.num, frac.num * this->num);
}

rational rational::operator-(rational &frac) {
    return rational(this->num * frac.denom - this->denom * frac.num, frac.num * this->num);
}

rational rational::operator*(rational &frac) {
    return rational(this->num * frac.num, this->denom * frac.denom);
}

rational rational::operator/(rational &frac) {
    return rational(this->num * frac.denom, this->denom * frac.num);
}

int rational::gcd(int a, int b) {
    a = abs(a);
    b = abs(b);
    return a ? gcd(a, b % a) : b;
}
