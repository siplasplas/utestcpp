#include <string>
    struct Complex {
    double re, im;
    Complex(double re, double im): re(re),im(im) {}
    bool operator==(const Complex &other) const {
        return re==other.re && im==other.im;
    }
};
namespace std {
    inline string to_string(Complex c){
        return to_string(c.re)+","+to_string(c.im);
    }
}