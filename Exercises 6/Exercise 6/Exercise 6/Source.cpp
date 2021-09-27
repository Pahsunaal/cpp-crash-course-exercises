#include <cstdio>
#include <map>

template<typename T, size_t Length>
T mean(T (&values)[Length]) {
	T result{};
	for (size_t i{}; i < Length; i++) {
		result += values[i];
	}
	return result / Length;
}

template<typename I>
I mode(const I* values, size_t length) {
	static_assert(std::is_integral<I>(), "Type must be an Integer");
	//if (!length) return 0;
	I result{};
	size_t max_num{};
	I mode{};
	std::map<I, I> counter{};
	for (size_t i{}; i < length; i++) {
		counter[values[i]]++;
		if (counter[values[i]] > max_num) {
			max_num = counter[values[i]];
			mode = values[i];
		}
		//printf("%d: %d\n", values[i], counter[values[i]]);
	}
	for (size_t i{}; i < length; i++) {
		if (values[i] != mode && counter[values[i]] == max_num) {
			return 0;
		}
	}
	return mode;
}

int main() {
	int values[]{ 1,2,3,4,5,6,7,1,1,2 };
	auto result = mode(values, 10);
	printf("mode: %d", result);
	double values2[]{ 1,2,3,4,5,6,7,8 };
	auto result2 = mean(values2);
	printf("mean: %f", result2);
}