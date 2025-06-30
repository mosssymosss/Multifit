#ifndef MULTIFIT_H___
#define MULTIFIT_H___


/*
Implementation of the MULTIFIT ALGORITHM originally developed for the identical-machines scheduling problem.
https://en.wikipedia.org/wiki/Multifit_algorithm

Identical-machines scheduling is an optimization problem. 
We are given n jobs of varying processing times, which need to be scheduled on m identical machines, 
such that a certain objective function is optimized, for example, the makespan is minimized.

The input to the multifit algorithm is a set S of numbers, and a parameter n.
The required output is a partition of S into n subsets, such that the largest subset sum (also called the makespan) is as small as possible.

The algorithm uses as a subroutine, an algorithm called first-fit-decreasing bin packing (FFD)
Multifit runs FFD multiple times, each time with a different capacity C, until it finds some C such that FFD with capacity C packs S into at most n bins.

To find it, it uses binary search as follows.
1. Let L := max ( sum(S) / n, max(S) )
2. Let U := max ( 2 sum(S) / n, max(S) )
3. Iterate k times (where k is a precision parameter)
	- Let C := (L+U)/2. Run FFD on S with capacity C
	- If FFD needs at most n bins, then decrease U by letting U := C
	- If FFD needs more than n bins, then increase L by letting L := C
4. Run FFD with capacity U. It is guaranteed to use at most n bins

The scheduling returned by Multifit has makespan at most 13/11 + (1/2)^k times the optimal makespan.

*/


#include <vector>
#include <string>
#include <algorithm>
#include <utility>

//the ID filed is something that distinguishes the Tasks from each other
//it can be the name of the file or something other the user uses to differentiate the files
//in some cases an integer is used in the code to make working with the Tasks easier
template <typename ID>
struct Task
{
	ID id;
	long long duration;
};

class Multifit
{
public:

	template <typename File>
	using ThreadsWithTasks = std::vector<std::vector<Task<File>>>;

	template <typename File>
	using Tasks = std::vector<Task<File>>;

	//the algorithm returns a vector of vectors that shows which file should go in which thread
	//the size of the result vector is the amount of threads necessary to complete all the tasks in the optimal time
	//the size of the result vector may be smaller than the number of threades available 
	//the precision parameter shows how optimal the solution is
	//the scheduling returned by Multifit has makespan at most 13/11 + (1/2)^precision times the optimal makespan
	//it is obvious that precision does not need to be more that 10, because (1/2)^10 does not have much effect to the number 13/11,
	//and a number bigger than 10 is unnecessary 
	template <typename File>
	static void multifit(const Tasks<File>& tasks, int number_of_threads, ThreadsWithTasks<File>* result, int precision = 10)
	{
		std::vector<Task<int>> ids(tasks.size());
		std::vector<File> names(tasks.size());
		int count = 0;
		for (const auto& task : tasks)
		{
			ids[count] = Task<int>{count, task.duration};
			names[count] = task.id;
			++count;
		}
		std::vector<std::vector<Task<int>>> temp;
		result->resize(temp.size());
		multifit_impl(ids, number_of_threads, &temp, precision);

		for (int i = 0; i < temp.size(); ++i)
		{
			std::vector<Task<File>> tmp;
			for (const auto& elem : temp[i])
			{
				int idx = elem.id;
				tmp.push_back(Task<File>{ names[idx], elem.duration });
			}
			result->push_back(tmp);
		}
	}

private:

	//The FFD algorithm takes as input the same set S of numbers, and a bin-capacity c. 
	//It heuristically packs numbers into bins such that the sum of numbers in each bin is at most C, aiming to use as few bins as possible. 

	//first-fit algorithm, we pass the files that need to be distributed pre-sorted so it works as first-fit-decreasing (FFD)
	//the algorithm just checks if it is possible to distribute the tasks into number_of_threads amount of threads with given capacity, 
	//but optionaly we can save the result of the distribution
	static bool first_fit_impl(const Tasks<int>& tasks, int number_of_threads, long long capacity, ThreadsWithTasks<int>* result = nullptr);

	//the algorithm uses binary search and ffd to find the best value vor the bin capacity
	
	static void multifit_impl(const Tasks<int>& tasks, int number_of_threads, ThreadsWithTasks<int>* result, int precision);

	static long long sum_of_sizes(const Tasks<int>& tasks);
	static long long max_of_sizes(const Tasks<int>& tasks);
};


#endif //MULTIFIT_H___