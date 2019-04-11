#pragma once
// Quick copy paste guard to roughly measure perfomance.
// Usage: copy-paste into method to have local definition (since statics are used) and instantiate
//
// Leaving multithread adjustment up to user choice as a quick solution thread_local could be used to have separate value per thread. (beware that thread_local was/is considered relatively slow)
// This dirty code is based on https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
// It's advised for reading especially if you want to make more precise measurements.

class perf_guard
{
public:
	explicit perf_guard(uint64_t print_threashold)
		: start_(0)
		, print_threshold_(print_threashold)
	{
		int temp[4];
		__cpuid(temp, 0);
		start_ = __rdtsc();
	}
	
	~perf_guard()
	{
		unsigned int tmp;
		uint64_t end = __rdtscp(&tmp);
		int temp[4];
		__cpuid(temp, 0);

		static uint64_t sum = 0;
		static uint64_t prev_sum = 0;
		static uint64_t counter = 0;
		sum += end - start_;
		counter++;
		if ((counter % print_threshold_) == 0)
		{
			ITRACE(
				T_WARN
				, IFMT(
					"Performance stats s: %Iu c: %Iu a: %Iu, local a: %Iu"
					, sum
					, counter
					, sum / counter
					, (sum - prev_sum) / print_threshold_
				)
			);
			prev_sum = sum;
		}
	}
private:
	uint64_t start_;
	const uint64_t print_threshold_;
};
