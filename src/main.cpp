#include <iostream>
#include <iomanip>
#include <queue> 
#include <fstream>
#include <string>

#include "generators.h"
#include "metrics.h"
#include "events.h"

#define SIMULATION_TIME 500

#define ARRIVED_L1 0
#define ARRIVED_L2 1
#define DEPART 2

#define MAX 1000            

int next_event(double events[], int size, double *now);
void report (Metrics &metrics, std::queue<Event*> queue);
const char* state(int, Metrics&);

void print_header();

int main() {

	std::queue<Event*> queue;

	double futureEvents[3] = { erlang(3, 0.25), poisson(0.2), SIMULATION_TIME + 1 };

	Metrics metrics;

	double now = 0; // laiks tagadejais      
	int nextEvent = -1;

	int i = 0;

	print_header();
	    
	do {
		metrics.update(now);

		switch (nextEvent) {
		case ARRIVED_L1:
			
			futureEvents[0] = now + erlang(3, 0.25);
			if (metrics.is_server_busy()) {
				queue.push(new Event_L1(now));
			}
			else
			{
				metrics.server_busy(L1);
				futureEvents[2] = now + normal(15.0, 2.0);
				metrics.increase_counter_without_queue_L1();
			}
			metrics.increase_counter_L1();
			break;
		case ARRIVED_L2:
			futureEvents[1] = now + poisson(0.2);
			if (metrics.is_server_busy()) {
				queue.push(new Event_L2(now));
			}
			else
			{
				metrics.server_busy(L2);
				futureEvents[2] = now + exponential(1.0 / 3.0);
				metrics.increase_counter_without_queue_L2();
			}
			metrics.increase_counter_L2();
			break;
		case DEPART:
			metrics.server_idle();
			if (queue.empty()) 
			{
				futureEvents[2] = MAX;
			}
			else
			{
				Event *pEvent = queue.front();
				queue.pop();

				if (pEvent->equals(L1)) 
				{
					metrics.server_busy(L1);
					futureEvents[2] = now + normal(15.0, 2.0);
					metrics.increase_counter_from_queue_L1();
				}
				else
				{
					metrics.server_busy(L2);
					futureEvents[2] = now + exponential(1.0 / 3.0);
					metrics.increase_counter_from_queue_L2();
				}
			}
			break;
		}

		const char pp = ' ';

		std::cout
			<< std::left << std::setw(5) << std::setfill(pp) << i++
			<< std::left << std::setw(15) << std::setfill(pp) << state(nextEvent, metrics)
			<< std::left << std::setw(10) << std::setfill(pp) << now
			<< std::left << std::setw(15) << std::setfill(pp) << futureEvents[0]
			<< std::left << std::setw(15) << std::setfill(pp) << futureEvents[1]
			<< std::left << std::setw(10) << std::setfill(pp) << futureEvents[2]
			<< std::left << std::setw(10) << std::setfill(pp) << metrics.is_server_busy()
			<< std::left << std::setw(10) << std::setfill(pp) << queue << std::endl;

		nextEvent = next_event(futureEvents, 3, &now);

	} while (now < SIMULATION_TIME + 1);

	now = SIMULATION_TIME;

	metrics.update(now);

	report(metrics, queue);

	exponential_to_file("exp.txt", 1.0 / 3.0);
	erlang_to_file("erlang.txt", 3, 0.25);
	normal_to_file("normal.txt", 15.0, 2.0);

	return 0;
}

const char* state(int next_event, Metrics& metrics) {           
	if (next_event < 0) return "Start";

	if (next_event == 0) return "L1";

	if (next_event == 1) return "L2";

	if (metrics.event_type == 0) return "process L1";
	if (metrics.event_type == 1) return "process L2";

	return "unhandled";
}

int next_event(double events[], int size, double *now) {
	int index = 0;

	for (int i = 1; i < size; i++) {
		if (events[i] < events[index]) {
			index = i;
		}
	}

	*now = events[index];

	return index;
}

	
void report( Metrics &m, std::queue<Event*> queue) {
		
	double serverIdleTime = SIMULATION_TIME - m.serverBusyTime;
	std::cout << std::endl;
	std::cout << std::setfill('.') << std::setw(80) << std::left << "Server idle time: " << serverIdleTime << std::endl;
	std::cout << std::setw(80) << std::left << "Server idle coeff: " << serverIdleTime / SIMULATION_TIME << std::endl << std::endl;


	std::cout << std::setw(80) << std::left << "L1 without queue: " << m.l1WithoutQ << std::endl;
	std::cout << std::setw(80) << std::left << "L2 without queue: " << m.l2WithoutQ << std::endl;
	std::cout << std::setw(80) << std::left << "Total without queue: " << m.l1WithoutQ + m.l2WithoutQ << std::endl;
	std::cout << std::setw(80) << std::left << "L1 count: " << m.l1Count << std::endl;
	std::cout << std::setw(80) << std::left << "L2 count: " << m.l2Count << std::endl;
	std::cout << std::setw(80) << std::left << "Total count: " << m.l1Count + m.l2Count << std::endl << std::endl;

	std::cout << std::setw(80) << std::left << "Probability without queue: " << (m.l1WithoutQ + m.l2WithoutQ) / double(m.l1Count + m.l2Count) << std::endl << std::endl;

	int l1RemainInQ = 0;
	int l2RemainInQ = 0;

	while (!queue.empty()) {
		Event* pEvent = queue.front();
		queue.pop();

		if (pEvent->equals(L1)) {
			l1RemainInQ++;
		}
		else {
			l2RemainInQ++;
		}
	}
	std::cout << std::setw(80) << std::left << "L1 remains in FIFO: " << l1RemainInQ << std::endl;
	std::cout << std::setw(80) << std::left << "L2 remains in FIFO: " << l2RemainInQ << std::endl;

	std::cout << std::setw(80) << std::left << "L1 in FIFO: " <<  m.l1Count - m.l1WithoutQ << std::endl;
	std::cout << std::setw(80) << std::left << "L2 in FIFO: " <<  m.l2Count - m.l2WithoutQ << std::endl;
	std::cout << std::setw(80) << std::left << "Total count in FIFO: " <<  (m.l1Count - m.l1WithoutQ) + (m.l2Count - m.l2WithoutQ) << std::endl << std::endl;
	
	std::cout << std::setw(80) << std::left << "L1 to server: " <<  m.l1WithoutQ + m.l1FromFIFO << std::endl;
	std::cout << std::setw(80) << std::left << "L2 to server: " <<  m.l2WithoutQ + m.l2FromFIFO << std::endl;
	std::cout << std::setw(80) << std::left << "Total count to server: " << (m.l1WithoutQ + m.l1FromFIFO) + (m.l2WithoutQ + m.l2FromFIFO) << std::endl << std::endl;
}

void print_header() {
	const char pp = ' ';

	std::cout
		<< std::left << std::setw(5) << std::setfill(pp) << "# "
		<< std::left << std::setw(15) << std::setfill(pp) << "Event"
		<< std::left << std::setw(10) << std::setfill(pp) << "Time"
		<< std::left << std::setw(15) << std::setfill(pp) << "L1 arrives"
		<< std::left << std::setw(15) << std::setfill(pp) << "L2 arrives"
		<< std::left << std::setw(10) << std::setfill(pp) << "Depart"
		<< std::left << std::setw(10) << std::setfill(pp) << "Server"
		<< std::left << std::setw(10) << std::setfill(pp) << "Queue" << std::endl;
}
