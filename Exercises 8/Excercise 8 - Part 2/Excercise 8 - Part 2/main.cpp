#include "Fib.h"
#include "Prime.h"
#include <cstdio>

int main() {
	for (const auto i : Fib::FibonacciRange{ 5000 }) {
		printf("%d ", i);
	}
	printf("\n");
	for (const auto i : Prime::PrimeNumberRange{ 5000000 }) {
		printf("%d ", i);
	}
}