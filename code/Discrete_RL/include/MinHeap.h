/**
 * @file MinHeap.h
 * @author Dongping Qi (git: DongpingQ)
 * @brief This class is the standard binary minimum heap data structure.
 * @date 2022-07-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "specialConstants.h"

/*
  - Each heap node has 2 members: a unique index & a numerical value
  - "index" is the unique 1d representation of node (i,j) on a N x N grid,
    i.e. index = i * N + j, which should be processed before being passed in
*/
class Node {
 public:
    int index;
    double value;
    Node(int id, double val) {
        index = id;
        value = val;
    };
};

// A general min heap for FMM with decreasekey() function.
// Each heap node is an object of class "Node"
class MinHeap {
 public:
    int capacity;         // maximum possible size of min heap
    int hsize;            // current number of heap nodes in min heap
    int *pos;             // pointer to node positions in the tree
    Node **tree;          // binary tree to contain all heap nodes

    // constructor
    MinHeap(int cap);

    // destructor
    ~MinHeap();

    // to heapify a subtree with the root at node with index i
    void minHeapify(int i);

    // to get index of parent of node at index i
    inline int parent(int i) { return (i - 1)/2; }

    // to extract the root which is the minimum element
    int extractMin();

    // decreases a node's value with index i to new_val
    void decreaseNode(int i, double new_val);

    // inserts a new node with index = id & value = val
    void insertNode(int id, double val);

    // check whehter the heap is empty
    bool isEmpty();
};