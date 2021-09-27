#include "Fib.h"
#include <cstdio>

int main() {
	for (const auto i : Fib::FibonacciRange{ 5000 }) {
		printf("%d ", i);
	}
}