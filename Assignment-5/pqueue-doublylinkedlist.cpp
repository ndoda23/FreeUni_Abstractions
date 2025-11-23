#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	head = NULL;
	logLength = 0;
}

// deleting data
DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	Cell* tmp = head;
	while (tmp != NULL) {
		Cell* newCell = tmp;
		tmp = tmp->next;
		delete newCell;
	}
}

int DoublyLinkedListPriorityQueue::size() {
	return logLength;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	
	return logLength == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	Cell* newCell = new Cell;
	// it must be first element
	newCell->back = NULL;

	newCell->name = value;
	// if head is null , there cannot be next element
	if (head == NULL) {
		newCell->next = NULL;
	}
	else {
		newCell->next = head;
	}
	if (head != NULL) {
		head->back = newCell;
	}
	head = newCell;
	logLength++;
}

string DoublyLinkedListPriorityQueue::peek() {
	if (isEmpty()) {
		error("Your priority queue is empty!");
	}
	
	Cell* smallest = head;
	// looking for the smallest one
	for (Cell* current = head; current != NULL; current = current->next) {
		if (current->name < smallest->name) {
			smallest = current;
		}
	}
	return smallest->name;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Your priority queue is empty!");
	}
	// looking for the smallest one.
	Cell* smallest = head;
	for (Cell* current = head; current != NULL; current = current->next) {
		if (current->name < smallest->name) {
			smallest = current;
		}
	}

	string min = smallest->name;

	// process of deleting smallest.
	if (smallest->back == NULL && smallest->next == NULL) {
		head = NULL;
	}
	if (smallest->back != NULL) {
		smallest->back->next = smallest->next;
	}
	else if (smallest->back == NULL) {
		head = smallest->next;
	}
	if (smallest->next != NULL) {
		smallest->next->back = smallest->back;
	}

	delete smallest;
	logLength--;
	return min;
}

