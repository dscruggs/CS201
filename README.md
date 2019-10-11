# CS201
Projects from CS201 - Data Structures and Algorithms. Implementation of a circular dynamic array template class including various sorting and selection algorithms in C++.

# Public Methods
te**CircularDynamicArray()** - default constructor, creates array of size 2

**CircularDynamicArray(const CircularDynamicArray &a)** - copy constructor, copies attributes of a into new CircularDynamicArray

**CircularDynamicArray(int s)** - custom constructor, creates array of size s

**~CircularDynamicArray()** - default destructor

**CircularDynamicArray<elmtype>& operator=(const CircularDynamicArray<elmtype> &a)** - copy assignment operator
  
**elmtype& operator[](int i)** - overloader to make [] operator work like expected with c++ arrays

**void addEnd(elmtype v)** - used to add to end of the array

**void addFront(elmtype v)** - used to add to front of the array

**void delEnd()** - used to delete last element in the array

**void delFront()** - used to delete first element in the array

**int length()** - returns length of the array

**int capacity()** - returns capacity of the array

**void clear()** - clears array and creates new array of size 2

**elmtype QuickSelect(int k)** - returns the kth smallest value in the array using quick selection algorithm

**elmtype WCSelect(int k)** - returns kth smallest value in the array using worst case selection algorithm

**void stableSort()** - stable sorts the array using mergesort

**void radixSort(int i)** - performs radix sort on the array on the ith least significant bits

**int linearSearch(elmtype e)** - returns the index of element e using linear search

**int binSearch(elmtype e)** - returns the index of element e using binary search, assumes a sorted array
