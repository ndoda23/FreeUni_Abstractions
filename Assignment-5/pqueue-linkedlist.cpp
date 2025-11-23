
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = NULL;
	logicalLength = 0;
}

// deleting data
LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	Cell* temporary;
	while (head != NULL) {
		temporary = head;
		head = head->next;
		delete temporary;
	}
}

int LinkedListPriorityQueue::size() {
	return logicalLength;
}

bool LinkedListPriorityQueue::isEmpty() {	
	return logicalLength==0;
}

void LinkedListPriorityQueue::enqueue(string value) {
	Cell* ourCell = new Cell;
	// name is value
	ourCell->name = value;
	// there is no next
	ourCell->next = NULL;

	// if head is emopty or its value is bigger
	if (head == NULL || value < head->name) {
		ourCell->next = head;
		head = ourCell;
		logicalLength++;
		return;
	}

	Cell* curr = head;
	while (curr->next != NULL && curr->next->name < value) {
		curr = curr->next;
	}
	ourCell->next = curr->next;
	curr->next = ourCell;
	logicalLength++;
}

// first is smallest
string LinkedListPriorityQueue::peek() {
	if (isEmpty()) {
		error("Priority queue is empty!");
	}
	return head->name;
}


string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Priority queue is empty!");
	}

	//deleting smallest , which is first element.
	Cell* min = head;
	string res = min->name;
	head = head->next;
	logicalLength--;
	delete min;
	return res;
}

