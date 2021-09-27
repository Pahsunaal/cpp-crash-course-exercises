#include <cstdio>

enum class Operation {
	Add,
	Subtract,
	Multiply,
	Divide
};

struct Calculator {
	Calculator(Operation operation) {
		op = operation;
	}

	int calculate(int a, int b) {
		switch (op) {
		case (Operation::Add): {
			return a + b;
		} break;
		case (Operation::Subtract): {
			return a - b;
		} break;
		case (Operation::Multiply): {
			return a * b;
		} break;
		case (Operation::Divide): {
			return a / b;
		} break;
		default: {
			printf("Error! No operation selected!");
		}
		}
	}

	private:
		Operation op{};
};

int main() {
	Calculator calc{ Operation::Multiply };
	printf("%d",calc.calculate(1, 2));
}