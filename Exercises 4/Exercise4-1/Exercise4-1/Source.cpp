#include <cstdio>
#include <ctime>
#include <chrono>
#include <stdexcept>

struct TimerClass {
	std::chrono::time_point<std::chrono::system_clock> timestamp{};
	bool moved{ false };
	const char* tag;

	TimerClass() : tag{ nullptr } {
		timestamp = std::chrono::system_clock::now();
	}

	TimerClass(const char* tagVal) : tag{ tagVal } {
		timestamp = std::chrono::system_clock::now();
	}

	TimerClass(const TimerClass& other) : timestamp{ other.timestamp }, tag{ other.tag } {
	}

	TimerClass& operator=(const TimerClass& other) {
		if (this == &other) return *this;
		timestamp = other.timestamp;
		tag = other.tag;
		return *this;
	}

	TimerClass(TimerClass&& other) noexcept : timestamp{ other.timestamp }, tag{ other.tag } {
		other.moved = true;
	}

	TimerClass& operator=(TimerClass&& other) noexcept{
		if (this == &other) return *this;
		other.moved = true;
		timestamp = other.timestamp;
		tag = other.tag;
		return *this;
	}

	~TimerClass() {
		if (!moved) {
			auto end = std::chrono::system_clock::now();
			if (tag == nullptr) {
				printf("%lld us\n", std::chrono::duration_cast<std::chrono::microseconds>(end - timestamp).count());
			}
			else {
				printf("%s: %lld us\n", tag, std::chrono::duration_cast<std::chrono::microseconds>(end - timestamp).count());
			}
		}
	}
};

struct SimpleString {
	SimpleString(size_t max_size) : max_size{ max_size }, length{} {
		if (max_size == 0) {
			throw std::runtime_error{ "Max size lower than 1 is not allowed." };
		}
		buffer = new char[max_size];
		buffer[0] = 0;
	}

	SimpleString(const SimpleString& other) : max_size{ other.max_size }, length{ other.length }, buffer{ new char[other.max_size] }{
		std::strncpy(buffer, other.buffer, max_size);
	}

	SimpleString& operator=(const SimpleString& other) {
		if (this == &other) return *this;
		delete[] buffer;
		buffer = new char[other.max_size];
		max_size = other.max_size;
		length = other.max_size;
		std::strncpy(buffer, other.buffer, max_size);
		return *this;
	}

	SimpleString(SimpleString&& other) noexcept : max_size { other.max_size }, length{ other.length }, buffer{ new char[other.max_size] }{
		std::strncpy(buffer, other.buffer, max_size);
		delete[] other.buffer;
		other.buffer = nullptr;
		other.length = 0;
		other.max_size = 0;
	}

	SimpleString& operator=(SimpleString&& other) noexcept {
		if (this == &other) return *this;
		delete[] buffer;
		buffer = new char[other.max_size];
		max_size = other.max_size;
		length = other.max_size;
		std::strncpy(buffer, other.buffer, max_size);
		other.length = 0;
		delete[] other.buffer;
		other.buffer = nullptr;
		other.max_size = 0;
		return *this;
	}

	~SimpleString() {
		delete[] buffer;
	}

	void print(const char* tag) {
		printf("%s : %s", tag, buffer);
	}

	bool append_line(const char* str) {
		const auto len = strlen(str);
		if (len + length + 2 > max_size) return false;
		std::strncpy(buffer + length, str, max_size - length);
		length += len;
		buffer[length++] = '\n';
		buffer[length] = 0;
		return true;
	}
private:
	size_t max_size;
	char* buffer;
	size_t length;
};

int main() {
	TimerClass tc("test");
	printf("Time for stuff to happen\n");
	TimerClass tc2;
	printf("Time for more stuff to happen\n");
	tc2 = tc;//std::move(tc);

	SimpleString string { 10 };
	string.append_line("Hiya!");
	string.print("A");
	SimpleString string2 { 10 };
	string2 = string;
	string2.print("B");
}