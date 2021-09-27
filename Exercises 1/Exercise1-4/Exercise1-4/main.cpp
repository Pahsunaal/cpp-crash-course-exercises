#include <cstdio>
int absolute_value(int x) {
	if (x < 0)return x * -1;
	else return x;
}

int sum(int x, int y) {
	return x + y;
}

int main() {
	int my_num = -10;
	int my_num2 = -1;
	printf("The sum of %d and %d is %d.\n", my_num, my_num2,
		sum(my_num, my_num2));
}