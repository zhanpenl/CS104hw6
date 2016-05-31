#include <vector>
#include <stdexcept>

template <typename T>
class MinHeap {
public:
	MinHeap (int d);
	/* Constructor that builds a d-ary Min Heap
	This should work for any d >= 2,
	but doesn't have to do anything for smaller d.*/

	~MinHeap ();

	void add (T item, int priority);
	/* adds the item to the heap, with the given priority. */

	const T & peek () const;
	/* returns the element with smallest priority. */

	void remove ();
	/* removes the element with smallest priority. */

	bool isEmpty () const;
	/* returns true if there are no elements on the heap. */

	int getD() const ;

	int size() const;

	const T & operator[](int index) const;

	const int & prioAt(int index) const;

private:
	// whatever you need to naturally store things.
	// You may also add helper functions here.
	int d;
	std::vector<T> * itemVector;
	std::vector<int> * priorityVector;

	void swap(int lhs, int rhs);
	void pushBack(T item, int priority);
	void bubbleUp(int pos);
	void popBack();
	void trickleDown(int pos);
};

template<class T>
MinHeap<T>::MinHeap(int d) {
	if ( d < 2 ) {
		throw std::invalid_argument("invalid child number");
	}

	this->d = d;
	itemVector = new std::vector<T>(); // should be addresses of the items? 
	priorityVector = new std::vector<int>();
}

template<class T>
MinHeap<T>::~MinHeap() {
	delete itemVector;
	delete priorityVector;
}

template<class T>
void MinHeap<T>::swap(int lhs, int rhs) {
	T itemTemp = (*itemVector)[lhs];
	(*itemVector)[lhs] = (*itemVector)[rhs];
	(*itemVector)[rhs] = itemTemp;

	int priorityTemp = (*priorityVector)[lhs];
	(*priorityVector)[lhs] = (*priorityVector)[rhs];
	(*priorityVector)[rhs] = priorityTemp;
}

// error: invalid initialization of reference of type ‘const int&’ from expression of type
// ‘std::vector<int>'
template<class T>
const T & MinHeap<T>::peek() const { // const here: make sure in the body that all function calls
									 // are const specified

	if ( this->isEmpty() ) throw std::invalid_argument("Peek when the heap is empyty");
	return (*itemVector)[0];
}

template<class T>
void MinHeap<T>::add(T item, int priority) {
	pushBack(item, priority);
	bubbleUp(itemVector->size() - 1);
}

template<class T>
void MinHeap<T>::pushBack(T item, int priority) {
	itemVector->push_back(item);
	priorityVector->push_back(priority);
}

template<class T>
void MinHeap<T>::bubbleUp(int pos) {
	int parent = (pos - 1) / d;
	if ( pos > 0 && (*priorityVector)[pos] < (*priorityVector)[parent] ) {
		swap(pos, parent);
		bubbleUp(parent);
	}
}

template<class T>
void MinHeap<T>::remove() {
	swap(0, itemVector->size() - 1);
	popBack();
	trickleDown(0);
}

template<class T>
void MinHeap<T>::popBack() {
	priorityVector->pop_back();
	itemVector->pop_back();
}

template<class T>
void MinHeap<T>::trickleDown(int pos) {
	int child = pos * d + 1, child0 = child;
	if ( child < itemVector->size() ) {
		int temp = child + 1;
		while ( temp < itemVector->size() && temp < child0 + d ) { // originally child instead of child0!!!
			if ( (*priorityVector)[temp] < (*priorityVector)[child] ) child = temp;
			temp++;
		}
		if ( (*priorityVector)[pos] > (*priorityVector)[child] ) {
			swap(pos, child);
			trickleDown(child);
		}
	}
}

template<class T>
bool MinHeap<T>::isEmpty() const {
  return itemVector->size() == 0;
}

template<class T>
int MinHeap<T>::getD() const {
	return d;
}

template<class T>
int MinHeap<T>::size() const {
	return itemVector->size();
}

template<class T>
const T& MinHeap<T>::operator[](int index) const {
	if ( index < itemVector->size() ) {
		return (*itemVector)[index];
	}
	else {
		throw std::invalid_argument("invalid index");
	}
}

template<class T>
const int & MinHeap<T>::prioAt(int index) const {
	if ( index < priorityVector->size() ) {
		return (*priorityVector)[index];
	}
	else {
		throw std::invalid_argument("invalid index");
	}
}