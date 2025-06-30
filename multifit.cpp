#include "multifit.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

bool Multifit::first_fit_impl(const Tasks<int>& tasks, int number_of_threads, long long capacity, ThreadsWithTasks<int>* result)
{
	std::vector<long long> bin_fullness(number_of_threads, 0);
	std::vector<std::vector<Task<int>>> bins(number_of_threads, std::vector<Task<int>>(0));
	for (const auto& task : tasks)
	{
		bool placed = false;
		for (int i = 0; i < number_of_threads; ++i)
		{
			if (bin_fullness[i] + task.duration <= capacity)
			{
				bin_fullness[i] += task.duration;
				bins[i].push_back(task);
				placed = true;
				break;
			}
		}
		if (!placed)
			return false;
	}
	if (result)
	{
		*result = bins;
	}
	return true;
	
}


void Multifit::multifit_impl(const Tasks<int>& tasks, int number_of_threads, ThreadsWithTasks<int>* result, int precision)
{
	auto sorted_files = tasks;
	std::sort(sorted_files.begin(), sorted_files.end(), [](const Task<int>& x, const Task<int>& y){return x.duration > y.duration; });
	long long size_sum = Multifit::sum_of_sizes(tasks);
	long long size_max = Multifit::max_of_sizes(tasks);

	long long lower_bound = std::max(size_sum / number_of_threads, size_max);
	long long upper_bound = std::max(2 * size_sum / number_of_threads, size_max);


	for (int i = 0; i < precision; ++i)
	{
		long long capacity = lower_bound + (upper_bound - lower_bound) / 2;
		auto temp = Multifit::first_fit_impl(sorted_files, number_of_threads, capacity);
		if (temp)
			upper_bound = capacity;
		else
			lower_bound = capacity;
	}

	Multifit::first_fit_impl(sorted_files, number_of_threads, upper_bound, result);

	while (result->back().empty())
	{
		result->pop_back();
	}
}


long long Multifit::sum_of_sizes(const Tasks<int>& tasks)
{
	long long res = 0;
	for (const auto& task : tasks)
	{
		res += task.duration;
	}
	return res;
}

long long Multifit::max_of_sizes(const Tasks<int>& tasks)
{
	long long res = -1;
	for (const auto& task : tasks)
	{
		if (res < task.duration)
			res = task.duration;
	}
	return res;
}
