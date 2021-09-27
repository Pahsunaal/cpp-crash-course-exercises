#include <cstdio>
#include <cstdint>
constexpr char pos_A{ 65 }, pos_Z{ 90 }, pos_a{ 97 }, pos_z{ 122 };
constexpr bool within_AZ(char x) { return pos_A <= x && pos_Z >= x; }
constexpr bool within_az(char x) { return pos_a <= x && pos_z >= x; }

struct CharNumHistogram {
	CharNumHistogram(int size) : size{ size } {
		counts = new size_t[size]{};
	}

	~CharNumHistogram() {
		delete counts;
	}

	void ingest(const char* x, int num) {
		size_t index{};
		while (const auto c = x[index]) {
			counts[num - 1]++;
			index++;
		}
	}

	void print(char** argv) const {
		for (size_t index{}; index < size; index++) {
			printf("%s: ", argv[index+1]);
			auto n_asterisks = counts[index];
			while (n_asterisks--) printf("*");
			printf("\n");
		}
	}
private:
	size_t* counts{};
	int size;
};





int main(int argc, char** argv) {
	CharNumHistogram hist{argc-1};
	for (int i{ 1 }; i < argc; i++) {
		hist.ingest(argv[i],i);
	}
	hist.print(argv);
}