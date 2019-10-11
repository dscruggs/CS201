/**************************************************************************************************
Dan Scruggs
10/11/19
CS 201 Data Structures Library
**************************************************************************************************/


#include <iostream>
#include <cstdlib>
using namespace std;


template <class elmtype>
class CircularDynamicArray
{
    public:
        CircularDynamicArray();
        CircularDynamicArray(const CircularDynamicArray &a);
        CircularDynamicArray(int s);
        ~CircularDynamicArray();
        CircularDynamicArray<elmtype>& operator=(const CircularDynamicArray<elmtype> &a);
        elmtype& operator[](int i);
        void addEnd(elmtype v);
        void addFront(elmtype v);
        void delEnd();
        void delFront();
        int length();
        int capacity();
        void clear();
        elmtype QuickSelect(int k);
        elmtype WCSelect(int k);
        void stableSort();
        void radixSort(int i);
        int linearSearch(elmtype e);
        int binSearch(elmtype e);
    
    private:
        //variables for the private array
        elmtype *privateArr;
        elmtype error;
        int frontIndex;
        int endIndex;
        int sizeVar;
        int capacityVar;
        int multiplier = 2;

        //helper functions
        void grow();
        void shrink();
        int randomPartition(elmtype arr[], int l, int r);
        void swap(elmtype *a, elmtype *b);
        int QSpartition(elmtype arr[], int start, int end);
        int WCpartition(elmtype arr[], int start, int end, int pivot);
        elmtype findMedian(elmtype arr[], int start, int end);
        void insertionSort(elmtype arr[], int start, int end);
        elmtype quickSelectHelper(elmtype arr[], int start, int end, int k);
        elmtype WCSelectHelper(elmtype arr[], int start, int end, int k);
        void mergeSort(int start, int end, int realLength);
        void merge(int start, int mid, int end, int length);
        void countingSort(int bitOrder);
        int binSearchHelper(elmtype arr[], int start, int end, int size, int capacity, elmtype e);


};


//default constructor sizeVar 2
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray()
{
    privateArr = new elmtype[2];
    sizeVar = 2;
    capacityVar = 2;
    frontIndex = 0;
    endIndex = sizeVar - 1;
}


//copy constructor
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(const CircularDynamicArray &a)
{
    //construct new array and copy attributes
    privateArr = new elmtype[a.capacityVar];
    sizeVar = a.sizeVar;
    capacityVar = a.capacityVar;
    frontIndex = 0;
    endIndex = a.sizeVar - 1;
    int traversalIndex = a.frontIndex;
    
    //deep copy data to new privateArr
    for(int i = 0; i < a.sizeVar; i++)
    {
        if (traversalIndex > (a.capacityVar - 1))
        {
            traversalIndex = 0;
        }
        privateArr[i] = a.privateArr[traversalIndex];
        traversalIndex++;
    }
}



//constructor for privateArr with specific sizeVar
template <class elmtype>
CircularDynamicArray<elmtype>::CircularDynamicArray(int s)
{
    privateArr = new elmtype[s];
    sizeVar = s;
    capacityVar = s;
    frontIndex = 0;
    endIndex = sizeVar - 1;
}



//destructor
template <class elmtype>
CircularDynamicArray<elmtype>::~CircularDynamicArray()
{
    delete [] privateArr;
}

//copy assignment operator
template <class elmtype>
CircularDynamicArray<elmtype>& CircularDynamicArray<elmtype>::operator=(const CircularDynamicArray<elmtype> &a)
{
    //if user tries to copy the same object return the object
    if (this == &a)
    {
        return *this;
    }

    //clears spce privateArr and creates new array with attributes of a
    delete [] privateArr;

    privateArr = new elmtype[a.capacityVar];
    capacityVar = a.capacityVar;
    sizeVar = a.sizeVar;
    frontIndex = a.frontIndex;
    endIndex = a.endIndex;

    int traversalIndex = a.frontIndex;
    
    //deep copy data to new privateArr
    for(int i = 0; i < a.sizeVar; i++)
    {
        if (traversalIndex > (a.capacityVar - 1))
        {
            traversalIndex = 0;
        }
        privateArr[traversalIndex] = a.privateArr[traversalIndex];
        traversalIndex++;
    }

    return *this;
}

//assignment operator
template <class elmtype>
elmtype& CircularDynamicArray<elmtype>::operator[](int i)
{
    
    //handles negative values
    if (i < 0)
    {
        if (-i > sizeVar)
        {
            cout << "Out of bounds reference : " << i << endl;
            return error;
        }

        else
        {
            i = (i + endIndex + 1 + capacityVar) % capacityVar;   
        }
    }

    //translates the user's input into location in the physical memory
    int physicalIndex = (i + frontIndex) % capacityVar;

    //checks if it's within the bounds of the privateArr
    if ((i >= sizeVar) || (-i > capacityVar))
    {
        cout << "Out of bounds reference : " << i << endl;
        return error;
    }
    else
    {
        return privateArr[physicalIndex];
    }
    
}

//adds to end of the array
template <class elmtype>
void CircularDynamicArray<elmtype>::addEnd(elmtype v)
{

    //checks if array is full
    if (sizeVar == capacityVar)
    {
        grow();
    }
    
    //if end index is the last elementof the private array, loops around to the front
    if (endIndex == capacityVar - 1)
    {
        endIndex = -1;
    }
    privateArr[endIndex + 1] = v;
    endIndex++;
    sizeVar++;

}



//adds to front of the array
template <class elmtype>
void CircularDynamicArray<elmtype>::addFront(elmtype v)
{

    //checks if array is full
    if (sizeVar == capacityVar)
    {
        grow();
    }

    //if front is at the front of the private array, loops around to the end
    if (frontIndex == 0)
    {
        frontIndex = capacityVar;
    }

    privateArr[frontIndex - 1] = v;
    frontIndex--;
    sizeVar++;

}



//deletes from end of the array
template <class elmtype>
void CircularDynamicArray<elmtype>::delEnd()
{
    //doesn't allow deleting if only 1 element
    if (frontIndex == endIndex)
    {
        cout << "The array only has one element left." << endl;
    }
    
    else
    {
        //if endIndex is at 0 then wraps around to end of store + 1 because it will be decremented
        if (endIndex == 0)
        {
            endIndex = capacityVar;
        }

        //moves end index to "delete" the last element
        endIndex--;
        sizeVar--;

        //if array 25% of capacity shrink store down half
        if (sizeVar * 4 <= capacityVar)
        {
            shrink();
        }
    }

}


//deletes from front of the array
template <class elmtype>
void CircularDynamicArray<elmtype>::delFront()
{
    //doesn't allow deleting if only 1 element
    if (frontIndex == endIndex)
    {
        cout << "The array only has one element left." << endl;
    }

    else
    {
        //increments index and decrements length to "delete" element
        frontIndex++;
        sizeVar--;

        //checks if it's last element and wraps around
        if (frontIndex == capacityVar)
        {
            frontIndex = 0;
        }
        
        //if it's less than 25% full shrinks store to half size
        if (sizeVar * 4 <= capacityVar)
        {
            shrink();
        }
    }
    
}


//returns length of the array
template <class elmtype>
int CircularDynamicArray<elmtype>::length()
{
    return sizeVar;
}


//returns capacity of the array
template <class elmtype>
int CircularDynamicArray<elmtype>::capacity()
{
    return capacityVar;
}


//clears array and makes a new one of length 2
template <class elmtype>
void CircularDynamicArray<elmtype>::clear()
{

    delete [] privateArr;

    privateArr = new elmtype[2];
    sizeVar = 2;
    capacityVar = 2;
    frontIndex = 0;
    endIndex = sizeVar - 1;

}


//grows capacity of array to accomodate new elements
template <class elmtype>
void CircularDynamicArray<elmtype>::grow()
{
    //creates new capacity and store for the copy
    int newCapacity = capacityVar * multiplier;
    
    elmtype *newStore = new elmtype[newCapacity];

    int traversalIndex = frontIndex;

    //deep copy data to new store which will become new array
    for(int i = 0; i < sizeVar; i++)
    {
        if (traversalIndex > (capacityVar - 1))
        {
            traversalIndex = 0;
        }
        newStore[i] = privateArr[traversalIndex];
        traversalIndex++;
    }

    //clear old store and set store equal to the copy
    delete [] privateArr;

    privateArr = newStore;
    capacityVar = newCapacity;
    frontIndex = 0;
    endIndex = sizeVar - 1;

}


//shrinks capacity of array if length is too small to save memory
template <class elmtype>
void CircularDynamicArray<elmtype>::shrink()
{
    //creates new capacity and store for the copy
    int newCapacity = capacityVar / multiplier;
    
    elmtype *newStore = new elmtype[newCapacity];

    int traversalIndex = frontIndex;

    //deep copy data to new store which will become new array
    for(int i = 0; i < sizeVar; i++)
    {
        if (traversalIndex > (capacityVar - 1))
        {
            traversalIndex = 0;
        }
        newStore[i] = privateArr[traversalIndex];
        traversalIndex++;
    }

    //clear old store and set store equal to the copy
    delete [] privateArr;

    privateArr = newStore;
    capacityVar = newCapacity;
    frontIndex = 0;
    endIndex = sizeVar - 1;

}


//Finds the kth smallest element in the array, this is called and 
//creates new array to be sorted then calls the recursive helper function
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::QuickSelect(int k)
{

    //if k is within number of elements of array
    //copy the contents of the array to new one so leaving the original unchanged during selection
    if ((k > 0) && (k <= sizeVar))
    {
        //creates new array
        int tempStart = 0;
        int tempEnd = sizeVar - 1;
        elmtype *newStore = new elmtype[sizeVar];
        int traversalIndex = frontIndex;

        //deep copy data to new temporary store for sorting and selecting
        for(int i = 0; i < sizeVar; i++)
        {
            if (traversalIndex > (capacityVar - 1))
            {
                traversalIndex = 0;
            }
            newStore[i] = privateArr[traversalIndex];
            traversalIndex++;
        }
        
        //calls recursive helper function to perform the selection
        return quickSelectHelper(newStore, tempStart, tempEnd, k);

    }

    //if k is too small or too large
    else
    {
        cout << k << " is out of bounds, there are " << sizeVar << " elements in the array." << endl;
        return error;
    }
    
}


//recursive function to find kth smallest element, called by QuickSelect
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::quickSelectHelper(elmtype arr[], int start, int end, int k)
{
        //partitions array around random element and returns pivot position
        int pivotPos = randomPartition(arr, start, end);

        //found the value, return the value of the kth smallest element
        if (pivotPos - start == k - 1)
        {
            return arr[pivotPos];
        }

        //position is greater so value we want is less, recurse on left half of array
        if (pivotPos - start > k - 1)
        {
            return quickSelectHelper(arr, start, pivotPos - 1, k);
        }

        //positon is less so value we want is greater, 
        //recurse on right and adjust k to match new array indexes
        if (pivotPos - start < k - 1)
        {
            return quickSelectHelper(arr, pivotPos + 1, end, k-pivotPos+start-1);
        }
}


//selects random partition value, called by quickSelectHelper
template <class elmtype>
int CircularDynamicArray<elmtype>::randomPartition(elmtype arr[], int start, int end)
{

    int range = end - start + 1;
    int pivot = rand() % range;
    swap(&arr[start + pivot], &arr[end]);
    return QSpartition(arr, start, end);

}

//partitions an array around a random pivot value, called in quick select
template <class elmtype>
int CircularDynamicArray<elmtype>::QSpartition(elmtype arr[], int start, int end)
{
    //sets pivot to the end for swapping
    int pivot = arr[end];
    int index = start;

    //moves all values less than pivot value to the left of it
    for (int i = start; i < end; i++)
    {
        if (arr[i] <= pivot) 
        {
            swap(&arr[index], &arr[i]);
            index++;
        }
    }

    swap(&arr[index], &arr[end]);
    return index;

}

//median of medians selection algorithm to find the kth smallest value in the array
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::WCSelect(int k)
{

    //if k is within number of elements of array
    if ((k > 0) && (k <= sizeVar))
    {
        //creates new array
        int tempStart = 0;
        int tempEnd = sizeVar - 1;
        elmtype *newStore = new elmtype[sizeVar];
        int traversalIndex = frontIndex;

        //deep copy data to new temporary store for sorting and selecting
        for(int i = 0; i < sizeVar; i++)
        {
            if (traversalIndex > (capacityVar - 1))
            {
                traversalIndex = 0;
            }
            newStore[i] = privateArr[traversalIndex];
            traversalIndex++;
        }
        
        //calls recursive helper function to perform the selection
        return WCSelectHelper(newStore, tempStart, tempEnd, k);

    }

    //if k is too small or too large
    else
    {
        cout << k << " is out of bounds, there are " << sizeVar << " elements in the array." << endl;
        return error;
    }

}

//recursive function that runs WCselect after the first loop
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::WCSelectHelper(elmtype arr[], int start, int end, int k)
{
    //number of elements
    int n = end - start + 1;

    //array for medians, need to add 4 since division will floor it for the final group if < 5
    elmtype median[(n+4)/5];
    int i = 0;
    for (i=0; i < n/5; i++)
    {
        median[i] = findMedian(arr, start+i*5, start+4+i*5);
    }
    //for the last group if < 5 elements
    if (i*5<n)
    {
        median[i] = findMedian(arr, start+i*5, (start+i*5) + end%5);
        i++;
    }

    int medianofMedians;
    //recurse to find the median of these medians
    if (i == 1)
    {
        medianofMedians = median[i-1];
    }
    else
    {
        medianofMedians = WCSelectHelper(median, 0, i-1, i/2);
    }

    //find the position of this median of medians in overall array and partition around it
    int pivotPos = WCpartition(arr, start, end, medianofMedians);

    //position is equal to kth smallest element, done and can return it
    if (pivotPos-start == k-1)
    {
        return arr[pivotPos];
    }

    //position is greater so value we want is less, recurse on left half of array
    if (pivotPos-start > k-1)
    {
        return WCSelectHelper(arr, start, pivotPos - 1, k);
    }

    //positon is less so value we want is greater, recurse on right and adjust k to match new array indexes
    if (pivotPos-start < k-1)
    {
        return WCSelectHelper(arr, pivotPos+1, end, k-pivotPos+start-1);
    }
}

//finds median for WCselect
template <class elmtype>
elmtype CircularDynamicArray<elmtype>::findMedian(elmtype arr[], int start, int end)
{

    //sorts the portion of the array and returns its median
    insertionSort(arr, start, end);
    return arr[start + (end-start+1)/2];

}

//sorts the arrays for WCselect
template <class elmtype>
void CircularDynamicArray<elmtype>::insertionSort(elmtype arr[], int start, int end)
{

    int temp, j;

    //loops through elements to compare
    for (int i = start + 1; i < end - start + 1; i++)
    {
        temp = arr[i];
        j = i - 1;

        //move all elements greater than the temp element being compared to the right of it
        while ((j >= start) && (arr[j] > temp))
        {
            arr[j + 1] = arr[j];
            j--;
        }

        //put the element being compared in its proper place
        arr[j + 1] = temp;
    }

}

//partitions an array around a pivot value, called in worst case select
template <class elmtype>
int CircularDynamicArray<elmtype>::WCpartition(elmtype arr[], int start, int end, int pivot)
{

    //finds and moves pivot value to last element
    int i; 
    for (i=start; i<end; i++) 
    {
        if (arr[i] == pivot)
        { 
           break;
        }   
    }   

    swap(&arr[i], &arr[end]); 
    int index = start;

    //moves all values less than pivot value to the left of it
    for (int i = start; i < end; i++)
    {
        if (arr[i] <= pivot) 
        {
            swap(&arr[index], &arr[i]);
            index++;
        }
    }

    swap(&arr[index], &arr[end]);
    return index;

}

//stable sort in O(n) time, I chose merge sort
template <class elmtype>
void CircularDynamicArray<elmtype>::stableSort()
{

    mergeSort(frontIndex, endIndex, sizeVar);

}

//recursive algorithm that splits the array into one unit chunks then merges them back together in sorted order
template <class elmtype>
void CircularDynamicArray<elmtype>::mergeSort(int start, int end, int length)
{
    //recursion will terminate when array is broken into its individual elements (length == 1)
    if (length > 1)
    {
        int mid = ((start + (length - 1)/2) % capacityVar);

        //checks if even or odd to adjust lengths of each
        //even
        if (length/2 * 2 == length)
        {
        mergeSort(start, mid, length/2);
        mergeSort(mid + 1, end, length/2);
        }

        //odd
        else
        {
        mergeSort(start, mid, (length + 1)/2);
        mergeSort(mid + 1, end, length/2);
        }

        merge(start, mid, end, length);
    }

}

//merges the seperate arrays into one sorted array
template <class elmtype>
void CircularDynamicArray<elmtype>::merge(int start, int mid, int end, int length) 
{
    int i, j, k;
    int length1, length2;
    int traversalIndex = start;
    
    //checks if ever or add to properly set length1
    //even
    if (length/2 * 2 == length)
    {
        length1 = length/2;
    }
    //odd
    else
    {
        length1 = (length + 1)/2;
    }

    length2 = length/2;

    elmtype arr1[length1], arr2[length2];

    //deep copy data to new arrays
    for (i=0; i < length1; i++)
    {
        if (traversalIndex == capacityVar)
        {
            traversalIndex = 0;
        }
        arr1[i] = privateArr[traversalIndex];
        traversalIndex++;
    }

    traversalIndex = mid + 1;
    for (j=0; j < length2; j++)
    {
        if (traversalIndex == capacityVar)
        {
            traversalIndex = 0;
        }
        arr2[j] = privateArr[traversalIndex];
        traversalIndex++;
    }
    
    i = 0;
    j = 0;
    k = start;
    
    //sorts both arrays into new array
    while (i < length1 && j < length2)
    {
        //resets index to beginning if it reaches end of array
        if (k == capacityVar)
        {
            k = 0;
        }

        //sorts two arrays
        if (arr1[i] <= arr2[j])
        {
            //cout << endl << "1 ran";
            privateArr[k] = arr1[i];
            i++;
        }

        else
        {
            //cout << endl << "2 ran";
            privateArr[k] = arr2[j];
            j++;
        }

        k++;
    }

    //puts the remaining elements of array1 into new array
    while (i < length1)
    {
        //cout << endl << "1 ran";
        if (k == capacityVar)
        {
            k = 0;
        }
        privateArr[k] = arr1[i];
        i++;
        k++;
    }

    //puts the remaining elements of array2 into new array
    while (j < length2)
    {
        //cout << endl << "2 ran";
        if (k == capacityVar)
        {
            k = 0;
        }
        privateArr[k] = arr2[j];
        j++;
        k++;
    }

}


//sorts the array from least to most significant of first i bits using radix sort
template <class elmtype>
void CircularDynamicArray<elmtype>::radixSort(int i)
{
    //j will be the number of bits from the least significant bit
    for(int j=0; j < i; j++)
    {
        countingSort(j);
    }
}


//sorts the array on a specific bit bitOrder units away from the 0th bit
template <class elmtype>
void CircularDynamicArray<elmtype>::countingSort(int bitOrder)
{
    int comp = 1;
    int temp[sizeVar];
    int i;
    int counter[2] = {0};
    int traversalIndex = frontIndex;

    //checks if 1 or 0 is present in the bitOrder place for the int
    //then increments that index in the counter array
    for (i=0; i<sizeVar; i++)
    {
        traversalIndex = (frontIndex + i) % capacityVar;
        counter[((privateArr[traversalIndex]>>(bitOrder))&comp)]++;
        
    }

    //sums up counter array to know index placement
    for (i=1; i<2; i++)
    {
        counter[i] += counter[i-1];
    }

    //sorts into the temp array
    traversalIndex = frontIndex;
    for (i = sizeVar - 1; i >= 0; i--)
    {
        traversalIndex = (frontIndex + i) % capacityVar;
        temp[counter[((privateArr[traversalIndex]>>bitOrder)&comp)] - 1] = privateArr[traversalIndex];
        counter[((privateArr[traversalIndex]>>(bitOrder))&comp)]-- ;
    }

    //copies data to original array
    traversalIndex = frontIndex;
    for (i=0; i < sizeVar; i++)
    {
        traversalIndex = (frontIndex + i) % capacityVar;
        privateArr[traversalIndex] = temp[i];
    }
}

//swaps two elements in an array, called in selection and sorting
template <class elmtype>
void CircularDynamicArray<elmtype>::swap(elmtype *a, elmtype *b)
{

    elmtype temp = *a;
    *a = *b;
    *b = temp;

}

//checks each element one by one and returns index of a match if it exists
template <class elmtype>
int CircularDynamicArray<elmtype>::linearSearch(elmtype e)
{

    //index is the value that will be returned relative to the user
    //traversalIndex is the index for the physical array in memory
    int index = 0;
    int traversalIndex = frontIndex;

    //standard linear search through the array comparing each value
    for (int i=0; i<sizeVar ;i++)
    {
        if (traversalIndex >= capacityVar)
        {
            traversalIndex = 0;
        }
        if (privateArr[traversalIndex] == e)
        {
            return index;
        }
        traversalIndex++;
        index++;
    }

    //error message and return -1 if value is not in array
    cout << "Could not find " << e << " in the array." << endl;
    return -1;

}

//standard binary search, returns index of element e. assumes sorted array
template <class elmtype>
int CircularDynamicArray<elmtype>::binSearch(elmtype e)
{

    if (sizeVar >= 1)

    { 
        int mid = (frontIndex + (sizeVar - 1) / 2) % capacityVar;
  
        // If the element is present at the middle 
        // itself 
        if (privateArr[mid] == e) 
        {
            if (frontIndex <= mid)
            {
                return mid - frontIndex;
            }
            if (frontIndex > mid)
            {
            return mid + capacityVar - frontIndex; 
            }
        }
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (privateArr[mid] > e)
            return binSearchHelper(privateArr, frontIndex, mid - 1, sizeVar/2, capacityVar, e); 
  
        // Else the element can only be present 
        // in right subarray
        return binSearchHelper(privateArr, mid + 1, endIndex, sizeVar/2, capacityVar, e); 
    } 
  
    // We reach here when element is not 
    // present in array 
    cout << "Could not find " << e << " in the array." << endl;
    return -1; 

} 

//helper function for binary search that runs after first loop
template <class elmtype>
int CircularDynamicArray<elmtype>::binSearchHelper(elmtype arr[], int start, int end, int size, int capacity, elmtype e)
{ 

    if (size >= 1)
    {
        if (end < 0)
        {
            end = end + capacity;
        }
        if (start > capacity)
        {
            start = start - capacity;
        }
        //maybe need to look at this function, check mergesort for finding midpoint
        int mid = (start + (size - 1) / 2) % capacity;
        
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == e) 
        {
            if (frontIndex <= mid)
            {
                return mid - frontIndex;
            }
            if (frontIndex > mid)
            {
                return mid + capacityVar - frontIndex;
            }
        }
        // If element is smaller than mid, then 
        // it can only be present in left subarray
        //maybe need to look at this size/2 in these calls, could be wrong. trace it out for even and odd. same with midpoint 
        if (arr[mid] > e) 
        {
            return binSearchHelper(arr, start, mid - 1, (size - 1)/2, capacity, e); 
        }
        // Else the element can only be present 
        // in right subarray
        //checks if even or odd to give proper size
        if ((size/2)*2 == size)
        {
            return binSearchHelper(arr, mid + 1, end, (size)/2, capacity, e);
        }
        else
        {
            return binSearchHelper(arr, mid + 1, end, (size - 1)/2, capacity, e);
        }
    }   

    // We reach here when element is not 
    // present in array 
    cout << "Could not find " << e << " in the array." << endl;
    return -1; 

}
