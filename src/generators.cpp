#include "generators.h"

#include <random>
#include <fstream>

double uniform() {
	return (double)rand() / (double)RAND_MAX;
}

void erlang_to_file(const char* file_name, int len, double lambda, int size) {
	std::ofstream os(file_name);
	for (int i = 0; i < size; i++) {
		os << erlang(len, lambda) << std::endl;
	}

	os.close();
}
void exponential_to_file(const char* file_name, double lambda, int size) {

	std::ofstream os(file_name);
	for (int i = 0; i < size; i++) {
		os << exponential(lambda) << std::endl;
	}

	os.close();

}
void normal_to_file(const char* file_name, double mu, double sigma, int size) {

	std::ofstream os(file_name);
	for (int i = 0; i < size; i++) {
		os << normal(mu, sigma) << std::endl;
	}

	os.close();
}

double erlang(int len, double lambda)
{
	double sum = 0.0;

	for (int i = 0; i < len; i++)
	{
		sum += exponential(lambda);
	}

	return sum;
}

double normal(double mu, double sigma)
{
	double r1 = rand() / double(RAND_MAX + 1.);
	double r2 = rand() / double(RAND_MAX + 1.);

	return mu + sigma * cos(2 * 3.14 * r1) * sqrt(-log(r2));
}

double exponential(double lambda)
{
	double r = uniform();
	return -log(r) / lambda;
}

double poisson(double lambda)
{
	return exponential(lambda);
}


int uniform(int low, int high)
{
	double r = rand() / (RAND_MAX + 1.);
	int range = high - low + 1;
	return (r * range) + low;
}
