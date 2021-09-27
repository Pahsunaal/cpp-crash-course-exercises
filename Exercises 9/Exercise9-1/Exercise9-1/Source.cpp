#include <cstdio>
#include <algorithm>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
	if (length == 0) {
		return initial;
	}
	return function(input[0], fold(function, &input[1], length - 1, initial));
}

int main() {
	int data[]{ 100, 200, 300, 400, 500 };
	size_t data_len = 5;
	auto sum = fold([](auto x, auto y) { return x + y; }, data, data_len,
		0);
	printf("Sum: %d\n", sum);
	auto maximum = fold([](auto x, auto y) { return std::max(x, y);  }, data, data_len, 0);
	printf("Maximum: %d\n", maximum);
	auto minimum = fold([](auto x, auto y) { return std::min(x, y);  }, data, data_len, 100000);
	printf("Minimum: %d\n", minimum);
	auto numGreaterThan200 = fold([](auto x, auto y) { auto tmp = (x > 200) ? 1 + y : 0 + y; return tmp; }, data, data_len, 0);
	printf("Number Greater Than 200: %d\n", numGreaterThan200);
}