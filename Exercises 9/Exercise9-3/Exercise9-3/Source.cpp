#include <cstdio>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
	if (length == 0) {
		return initial;
	}
	return function(input[0], fold(function, &input[1], length - 1, initial));
}

template <typename Fn, typename In, typename Out>
constexpr bool all(Fn function, In* input, size_t length) {
	return fold([function](auto x, auto y) { return function(x) && y; }, input, length, 100000);
} 

int main() {
	int data[]{ 100, 200, 300, 400, 500 };
	size_t data_len = 5;
	auto func = [](auto x) { return x > 100; };
	auto all_gt100 = all<decltype(func),int,bool>(func, data, data_len);
	if (all_gt100) printf("All elements greater than 100.\n");
}