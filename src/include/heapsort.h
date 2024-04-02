#pragma once
#include <iostream> 
#include <vector>
#include "score.h"

namespace Sort {

#define LEFT_CHILD(index) ((2*(index)) + 1)
#define RIGHT_CHILD(index) ((2*(index)) + 2)

  void swapItem(std::vector<WordScore>& arr, int idx1, int idx2);

  void heapify(std::vector<WordScore>& arr, int heapSize, int subTreeIndex);

  void heapSort(std::vector<WordScore>& arr);

}
