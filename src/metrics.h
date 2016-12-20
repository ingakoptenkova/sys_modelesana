#pragma once

#include <iostream>

#define IDLE 0
#define BUSY 1

struct Metrics {
	double serverBusyTime;
	double lastEventTime;
	int l1WithoutQ;
	int l2WithoutQ;
	int l1Count;
	int l2Count;
	int l1FromFIFO;
	int l2FromFIFO;
	int serverStatus;

	int event_type;

	Metrics() : serverBusyTime(0), lastEventTime(0), l1WithoutQ(0), l2WithoutQ(0), l1Count(0), l2Count(0), l1FromFIFO(0), l2FromFIFO(0), serverStatus(IDLE), event_type(0) {

	}

	void update(double now) {
		double timeSinceLastEvent = now - lastEventTime;
		lastEventTime = now;
		serverBusyTime += (double)serverStatus * timeSinceLastEvent;
	}

	bool is_server_busy() {
		return serverStatus == BUSY;
	}

	void server_idle() {
		serverStatus = IDLE;
	}

	void server_busy(int value) {
		event_type = value;
		serverStatus = BUSY;
	}

	void increase_counter_without_queue_L1() {
		l1WithoutQ++;
	}

	void increase_counter_without_queue_L2() {
		l2WithoutQ++;
	}

	void increase_counter_L1() {
		l1Count++;
	}

	void increase_counter_L2() {
		l2Count++;
	}

	void increase_counter_from_queue_L1() {
		l1FromFIFO++;
	}

	void increase_counter_from_queue_L2() {
		l2FromFIFO++;
	}

	friend std::ostream& operator <<(std::ostream& os, const Metrics& p);
};
