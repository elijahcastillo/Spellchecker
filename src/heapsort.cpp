
#include "include/heapsort.h"
#include "include/score.h"

namespace Sort {

  void swapItem(std::vector<WordScore>& arr, int idx1, int idx2){
    WordScore temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
  }

  void heapify(std::vector<WordScore>& arr, int heapSize, int subTreeIndex){
    // Assume the root of the subtree is the largest
    int largest = subTreeIndex;
    int left = LEFT_CHILD(subTreeIndex); // Index of left child
    int right = RIGHT_CHILD(subTreeIndex); // Index of right child

    // Check if left child exists and is greater than the current largest
    if(left < heapSize && arr[left].score > arr[largest].score){
      largest = left;
    }

    // Check if right child exists and is greater than the current largest
    if(right < heapSize && arr[right].score > arr[largest].score){
      largest = right;
    }

    // If the largest element is not the root
    if(largest != subTreeIndex){
      // Swap the root with the largest element
      swapItem(arr, largest, subTreeIndex);

      // Recursively heapify the affected sub-tree to ensure it's a max heap
      heapify(arr, heapSize, largest);
    }


    // Example of tree before heapify:        2 
    //                                      /  \
    //                                     3    1
    //
    // After heapify on root:              3 
    //                                   /  \
    //                                  2    1
  }


  void heapSort(std::vector<WordScore>& arr){
    // Build max heap from the unsorted array
    int lastNonLeafNode = (arr.size() - 1) / 2;
    for(int i = lastNonLeafNode; i >= 0; i--){
      // Call heapify on each non-leaf node, starting from the last non-leaf node
      heapify(arr, arr.size(), i);
    }

    // One by one, extract elements from the heap
    // reducing its size as the sorted elements are pushed to the back
    for(int i = arr.size() - 1; i >= 0; i--){
      // Move the current root to the end (swap the first and last elements)
      swapItem(arr, 0, i);

      // Call heapify on the reduced heap to maintain the max heap property
      heapify(arr, i, 0);
    }
    // Example of array after initial build max heap: [9, 6, 5, 2, 3]
    // Example after first extraction and heapify:    [5, 3, 2, 6, 9] (6 and 9 swapped, then heapify)
  }

}
