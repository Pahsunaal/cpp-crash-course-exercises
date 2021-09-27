#include <cstdio>

template<typename T>
struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(T& from, T& to, double amount) = 0;
};

template<typename T>
struct ConsoleLogger : Logger<T> {
	ConsoleLogger() {
	}

	ConsoleLogger(const char* tag) : prepend{ tag } {
	}

	void log_transfer(T& from, T& to, double amount) override {
		if (prepend) printf("[cons %s] %ld -> %ld: %f\n", prepend, from.get_id(), to.get_id(), amount);
		else printf("[cons] %ld -> %ld: %f\n", from.get_id(), to.get_id(), amount);
	}

private:
	const char* prepend = nullptr;
};

template<typename T>
struct FileLogger : Logger<T> {
	void log_transfer(T& from, T& to, double amount) override {
		printf("[file] %ld -> %ld: %f\n", from.get_id(), to.get_id(), amount);
	}
};

struct Account {
	virtual ~Account() = default;

	virtual long get_id() = 0;

	virtual double get_balance() = 0;

	virtual void set_balance(double newBalance) = 0;
};

struct CheckingAccount : Account {
	CheckingAccount(long id) : balance{}, id{ id }{
	}

	CheckingAccount(long id, double startBalance) : balance{ startBalance }, id{ id }{
	}
	long get_id() {
		return id;
	}

	double get_balance() {
		return balance;
	}

	void set_balance(double newBalance) {
		balance = newBalance;
	}

private:
	double balance;
	long id;
};

struct SavingsAccount : Account {
	SavingsAccount(long id) : balance{}, id{ id }{
	}

	SavingsAccount(long id, double startBalance) : balance{ startBalance }, id{ id }{
	}
	long get_id() {
		return id;
	}

	double get_balance() {
		return balance;
	}

	void set_balance(double newBalance) {
		balance = newBalance;
	}

private:
	double balance;
	long id;
};

template<typename T>
struct Bank {
	Bank(Logger<T>* logger) : logger{ logger } {
	}

	~Bank() = default;

	void set_logger(Logger<T>* new_logger) {
		logger = new_logger;
	}

	void make_transfer(T& from, T& to, double amount) {
		if (logger) {
			double fromAmount = from.get_balance();
			double toAmount = to.get_balance();
			if (fromAmount > amount) {
				from.set_balance(fromAmount - amount);
				to.set_balance(toAmount + amount);
				logger->log_transfer(from, to, amount);
			}
		}
	}

private:
	Logger<T>* logger;
};

int main() {
	ConsoleLogger<Account> console_logger{ "test" };
	FileLogger<Account> file_logger;
	Bank<Account> bank{ &console_logger};

	SavingsAccount ac1{100L,100};
	CheckingAccount ac2{200L};
	bank.make_transfer(ac1, ac2, 30);
	printf("%ld : %f\n", ac1.get_id(), ac1.get_balance());
	printf("%ld : %f\n", ac2.get_id(), ac2.get_balance());
}