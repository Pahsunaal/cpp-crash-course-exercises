#pragma once

namespace Prime {
	struct PrimeNumberIterator {
		bool operator!=(int x) const;
		PrimeNumberIterator& operator++();
		int operator*() const;

	private:
		int current{ 1 }, last{ 1 };
	};

	struct PrimeNumberRange {
		PrimeNumberRange(int max);
		PrimeNumberIterator begin() const;

		int end() const;

	private:
		const int max;
	};
}
