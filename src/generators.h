#pragma once

double erlang(int len, double lambda);
double normal(double mu, double sigma);
double exponential(double lambda);
double poisson(double lambda);
int uniform(int low, int high);

void erlang_to_file(const char*, int, double, int size = 1000);
void exponential_to_file(const char*, double, int size = 1000);
void normal_to_file(const char*, double, double, int size = 1000);