#include "Prime.h"
#include <math.h>

using namespace Prime;

bool PrimeNumberIterator::operator!=(int x) const {
    return x >= current;
}

PrimeNumberIterator& PrimeNumberIterator::operator++() {
    const auto tmp = current;
    size_t div(ceil(sqrt(current)));
    bool notFound{ true };
    bool isPrime{ true };
    while (notFound) {
        
        current++;
        for (size_t i{2}; i <= div;i++) {
            if (current % i == 0) {
                isPrime = false;
            }
        }
        if (isPrime) {
            notFound = false;
        }
        isPrime = true;
    }
    last = tmp;
    return *this;
}

int PrimeNumberIterator::operator*() const {
    return current;
}

PrimeNumberRange::PrimeNumberRange(int max) : max{ max } {
}

PrimeNumberIterator PrimeNumberRange::begin() const {
    return PrimeNumberIterator{};
}
int PrimeNumberRange::end() const {
    return max;
}