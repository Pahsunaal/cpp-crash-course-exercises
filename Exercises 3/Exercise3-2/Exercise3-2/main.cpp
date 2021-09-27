#include <cstdio>

bool bounds_check(int pos, int size) {
	if (pos > size || pos < 0) {
		printf("\nError! Out of bounds!\n");
		return false;
	}
	return true;
}

// Read from array bounded by supplied size
char read_from(char* toRead, int pos, int size) {
	if (bounds_check(pos,size)) return toRead[pos];
	else return ' ';
}

// Write to array bounded by supplied size
void write_to(char* toWriteTo, int pos, char charToWrite, int size) {
	if (bounds_check(pos, size)) {
		toWriteTo[pos] = charToWrite;
	}
}

int main() {
	char lower[] = "abc?e";
	int lowerSize = sizeof(lower) / sizeof(char);
	char upper[] = "ABC?E";
	int upperSize = sizeof(upper) / sizeof(char);

	char* upper_ptr = upper; // Equivalent: &upper[0]
	lower[3] = 'd'; // lower now contains a b c d e \0
	upper_ptr[3] = 'D'; // upper now contains A B C D E \0
	char letter_d = lower[3]; // letter_d equals 'd'
	char letter_D = upper_ptr[3]; // letter_D equals 'D'

	printf("lower: %s\nupper: %s", lower, upper);
	//lower[7] = 'g'; // Super bad. You must never do this.
	write_to(lower, 7, 'g', lowerSize);
}