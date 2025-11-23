/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

HeapPriorityQueue::HeapPriorityQueue() {
	cap = 4;
	logicalLength = 0;
	heap = new string[cap+1];
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] heap;
}

int HeapPriorityQueue::size() {
	return logicalLength;
}

bool HeapPriorityQueue::isEmpty() {
	return logicalLength==0;
}

void HeapPriorityQueue::enqueue(string value) {
	// if it needs resizing
	if (logicalLength + 1 > cap) {
		makeLarger();
	}
	//adding new element
	logicalLength++;
	heap[logicalLength] = value;
	goUp(logicalLength);

}

string HeapPriorityQueue::peek() {
	if (isEmpty()) {
		error("Priority queue is emtpy!");
	}
	return heap[1];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Priority queue is emtpy!");
	}
	string minVal = heap[1];
	heap[1] = heap[logicalLength];
	logicalLength--;
	goDown(1);
	return minVal;
}

 // resizing array if it is necessary.
void HeapPriorityQueue::makeLarger() {
	cap = cap * 2;
	string* newHeap = new string[cap+1];
	for (int i = 1; i <= logicalLength; i++) {
		// copying data to new array
		newHeap[i] = heap[i];
	}
	// delete old array
	delete[] heap;	
	heap = newHeap;
}

// bubbleUp
void HeapPriorityQueue::goUp(int index) {
	for (index; index > 1; index /= 2) {
		if (heap[index] < heap[index / 2]) {
			swap(heap[index], heap[index / 2]);
		}
	}
}

 // bubble down method , it finds the smallest string.
void HeapPriorityQueue::goDown(int index) {
	int smallest = index;
	int left = 2 * index;
	int right = 2 * index + 1;

	if (left <= logicalLength) {
		if (heap[left] < heap[smallest]) {
			smallest = left;
		}
	}
	if (right <= logicalLength) {
		if (heap[right] < heap[smallest]) {
			smallest = right;
		}
	}


	if (smallest != index) {
		swap(heap[index], heap[smallest]);
		goDown(smallest);
	}

	if (smallest == index) {
		return;
	}
}