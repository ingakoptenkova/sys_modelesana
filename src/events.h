#pragma once

#include <iostream>

#define L1 0
#define L2 1

class Event {

	double arrival;
public:
	virtual bool equals(int type) = 0;

	Event(double time) {
		arrival = time;
	};

	friend std::ostream& operator <<(std::ostream& os, const Event& p);

protected:
	virtual void print(std::ostream& os) const { }

};

class Event_L1 : public Event {

public:
	Event_L1(double time) : Event(time) {

	}
	bool equals(int type) {
		return L1 == type;
	}
protected:
	virtual void print(std::ostream& os) const { 
		os << "L1";
	}
};


class Event_L2 : public Event {
public:
	Event_L2(double time) : Event(time) {

	}

	bool equals(int type) {
		return L2 == type;
	}
protected:
	virtual void print(std::ostream& os) const { 
		os << "L2";
	}
};

inline std::ostream& operator <<(std::ostream& os, const Event& p) {
	p.print(os);
	return os;
}



std::ostream & operator<<(std::ostream & os, std::queue<Event*> queue) {
	os << queue.size();
	while (!queue.empty()) {
		os << *(queue.front()) << " ";
		queue.pop();
	}
	return os; // end of function
}