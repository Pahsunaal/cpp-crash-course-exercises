#include <cstdio>
#include <stdexcept>
#include <cstddef>

template<size_t size>
struct UnsignedBigInt {
	UnsignedBigInt() : data{} {
		data[0] = '0';
	}

	UnsignedBigInt(const uint8_t input_data[size]) {
		for (int i{}; i < size; i++) data[i] = input_data[i];
	}

	UnsignedBigInt(int value) {
		for (int i{}; i < size; i++) {
			data[i] = value % 10;
			value /= 10;
		}
		if (value > 0) throw std::overflow_error("Input too large for UnsignedBigInt!");
	}

	UnsignedBigInt operator+(const UnsignedBigInt& other) const {
		int carry{};
		uint8_t result[size]{};
		for (int i{}; i < size; i++) {
			result[i] = data[i] + other.data[i] + carry;
			result[i] = result[i] % 10;
			carry = result[i] / 10;
		}
		if (carry) throw std::overflow_error("Result too large for UnsignedBigInt!");
		return UnsignedBigInt(result);
	}

	UnsignedBigInt operator-(const UnsignedBigInt& other) const {
		uint8_t result[size]{};
		for (int i{size-1}; i >= 0; i--) {
			if (data[i] < other.data[i]) throw std::underflow_error("Result negative!");
			result[i] = data[i] - other.data[i];
		}
		return UnsignedBigInt(result);
	}

	operator int() {
		int result{};
		int power{1};
		for (int i{}; i < size; i++) {
			result += data[i] * power;
			power *= 10;
		}
		return result;
	}

	uint8_t get_byte(size_t index) {
		if (index < size) return data[index];
		else return 0;
	}

	uint8_t data[size];
};

int main() {
	UnsignedBigInt<8> test{ 134 };
	UnsignedBigInt<8> result{ test - test };
	int test2{ test };
	int result2{ result };
	printf("%d\n", test2);
	printf("%d\n", result2);
}