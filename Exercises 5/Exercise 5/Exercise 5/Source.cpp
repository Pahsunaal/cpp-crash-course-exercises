#include <cstdio>

struct Logger {
	virtual ~Logger() = default;
	virtual void log_transfer(long from, long to, double amount) = 0;
};

struct AccountDatabase {
	virtual ~AccountDatabase() = default;
	virtual double get_amount(long id) = 0;
	virtual bool set_amount(long id, double amount) = 0;
};

struct ConsoleLogger : Logger{
	ConsoleLogger() {
	}

	ConsoleLogger(const char* tag) : prepend{ tag } {
	}

	void log_transfer(long from, long to, double amount) override {
		if (prepend) printf("[cons %s] %ld -> %ld: %f\n", prepend, from, to, amount);
		else printf("[cons] %ld -> %ld: %f\n", from, to, amount);
	}
	
private:
	const char* prepend = nullptr;
};

struct FileLogger : Logger {
	void log_transfer(long from, long to, double amount) override {
		printf("[file] %ld -> %ld: %f\n", from, to, amount);
	}
};

struct InMemoryAccountDatabase : AccountDatabase {
	InMemoryAccountDatabase(size_t max_accounts) : current_amount{}, max_accounts { max_accounts }, accounts{ new double[max_accounts] }, ids{ new long[max_accounts] } {
	}

	double get_amount(long id) override {
		for (int i = 0; i < max_accounts;i++) {
			if (ids[i] == id) return accounts[i];
		}
		return 0.0f;
	}

	bool set_amount(long id, double amount) override {
		for (int i = 0; i < max_accounts; i++) {
			if (ids[i] == id) {
				accounts[i] = amount;
				return true;
			}
		}
		ids[current_amount] = id;
		accounts[current_amount++] = amount;
		return false;
	}

	~InMemoryAccountDatabase() {
		delete[] accounts;
		delete[] ids;
	}

private:
	size_t current_amount;
	size_t max_accounts;
	double* accounts;
	long* ids;
};

struct Bank {
	Bank(Logger* logger, AccountDatabase& db) : logger{ logger }, db{ db } {
	}

	void set_logger(Logger* new_logger) {
		logger = new_logger;
	}

	void make_transfer(long from, long to, double amount) {
		if (logger) {
			double fromAmount = db.get_amount(from);
			double toAmount = db.get_amount(to);
			if (fromAmount > amount) {
				db.set_amount(from,fromAmount - amount);
				db.set_amount(to, toAmount + amount);
				logger->log_transfer(from, to, amount);
			}
		}
	}

private:
	Logger* logger;
	AccountDatabase& db;
};

int main() {
	ConsoleLogger console_logger{ "test" };
	FileLogger file_logger;
	InMemoryAccountDatabase db{ 100 };
	db.set_amount(1000, 50.00);
	Bank bank{ &console_logger, db };
	bank.make_transfer(1000, 2000, 49.95);
	bank.set_logger(&file_logger);
	bank.make_transfer(2000, 4000, 20.00);
	printf("%ld : %f\n", 1000, db.get_amount(1000L));
	printf("%ld : %f\n", 2000, db.get_amount(2000L));
}