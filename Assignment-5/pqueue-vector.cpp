#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

int VectorPriorityQueue::size() {
	return ourQueue.size();
}

bool VectorPriorityQueue::isEmpty() {
	return size() == 0;
}

void VectorPriorityQueue::enqueue(string value) {
	ourQueue.add(value);
}

// just pick the smallest
string VectorPriorityQueue::peek() {
	if (isEmpty()) {
		error("Priority queue is empty.");
	}
	int index = firstName();
	string firstStr = ourQueue[index];
	return firstStr;
}

// removing smallest
string VectorPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Priority queue is empty.");
	}
	int index = firstName();
	string firstStr = ourQueue[index];
	ourQueue.remove(index);
	return firstStr;
}

// looking for the smallest one
int VectorPriorityQueue::firstName() {
	int indx = 0;
	for (int i = 0; i < ourQueue.size(); i++) {
		if (ourQueue[indx] > ourQueue[i]) {
			indx = i;
		}
	}
	return indx;
}

