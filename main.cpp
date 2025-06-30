#include <iostream>
#include "multifit.h"
#include <vector>
#include <fstream>

int main()
{
	int threads;
	std::cout << "Max threads: ";
	std::cin >> threads;

	std::ifstream path_to_files("input.txt");
	if (!path_to_files.is_open())
	{
		throw std::runtime_error("Could not open file");
	}
	std::string temp;
	std::vector<Task<std::string>> files;
	while (path_to_files >> temp)
	{
		std::string name = temp;
		path_to_files >> temp;
		long long size = std::stol(temp);
		files.push_back(Task<std::string>{ name, size });
	}
	path_to_files.close();

	std::vector<std::vector<Task<std::string>>> a;
	
	std::string res = "";
	Multifit::multifit(files, threads, &a, 100);
	res = res + "Number of threads neccessary: " + std::to_string(a.size()) + "\n";
	//res = res + "Maximum time for a thread to finish working: " + std::to_string(s.get_max_load()) + "\n";
	int q = 1;
	for (auto x : a)
	{
		long long sum = 0;
		res = res + "Thread No. " + std::to_string(q) + "; Items: " + std::to_string(x.size()) + "\n";
		for (auto y : x)
		{
			res = res + "\t" + y.id + ": " + std::to_string(y.duration) + "\n";
			sum += y.duration;
		}
		res = res +  "Total: " + std::to_string(sum) + "\n";
		++q;
	}
	std::cout << res << std::endl;
	
	system("pause");
	return 0;
}