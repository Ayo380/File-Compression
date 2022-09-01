// Ayokunle Olugboyo
// mymap.h
//
// TODO: write this file header comment.
#pragma once

#include <iostream>
#include <sstream>

using namespace std;

template <typename keyType, typename valueType>
class mymap {
 private:
  struct NODE {
    keyType key;      // used to build BST
    valueType value;  // stored data for the map
    NODE* left;       // links to left child
    NODE* right;      // links to right child
    int nL;           // number of nodes in left subtree
    int nR;           // number of nodes in right subtree
    bool isThreaded;
  };
  NODE* root;  // pointer to root node of the BST
  int size;    // # of key/value pairs in the mymap

  //
  // iterator:
  // This iterator is used so that mymap will work with a foreach loop.
  //
  struct iterator {
   private:
    NODE* curr;  // points to current in-order node for begin/end

   public:
    iterator(NODE* node) { curr = node; }

    keyType operator*() { return curr->key; }

    bool operator==(const iterator& rhs) { return curr == rhs.curr; }

    bool operator!=(const iterator& rhs) { return curr != rhs.curr; }

    bool isDefault() { return !curr; }

    //
    // operator++:
    //
    // This function should advance curr to the next in-order node.
    // O(logN)
    //
    iterator operator++() {
      if (curr->isThreaded) {
        curr = curr->right;
      } else {
        curr = curr->right;
        while (curr != nullptr && curr->left != nullptr) {
          curr = curr->left;
        }
      }
      return *this;
    }
  };

  // private member function for toString to
  //
  // _add_string:
  //
  // helper function for toString
  // recursively goes travers through the entire map using in order
  //
  void _inorder_add_string(NODE* curr, ostream& out) {
    if (curr == nullptr) {
      return;
    }
    // travers through the left nodes first
    _inorder_add_string(curr->left, out);

    out << "key: " << curr->key << " value: " << curr->value << endl;
    // check if the node is threaded
    if (curr->isThreaded) {
      _inorder_add_string(nullptr, out);
    } else {
      _inorder_add_string(curr->right, out);
    }
  }
  // private member function for clear and destructor
  // _postorder_clear:
  //
  // takes the root as an argument
  // helper function for clear and desctructor function
  // Uses post order traverser to delete each node
  //
  void _postorder_clear(NODE* cur) {
    if (cur == nullptr) {
      return;
    }
    _postorder_clear(cur->left);
    if (!cur->isThreaded) {
      _postorder_clear(cur->right);
    }
    delete cur;
    size--;
  }
  // copy function to help the equal operator and copy constructor
  void _preorder_copy(NODE* curr) {
    if (curr == nullptr) {
      return;
    }
    put(curr->key, curr->value);
    _preorder_copy(curr->left);
    if (curr->isThreaded) {
      _preorder_copy(nullptr);
    } else {
      _preorder_copy(curr->right);
    }
  }

 public:
  //
  // default constructor:
  //
  // Creates an empty mymap.
  // Time complexity: O(1)
  //
  mymap() {
    this->root = nullptr;
    this->size = 0;
    // this->left = nullptr;
    // this->right = nullptr;
  }

  //
  // copy constructor:
  //
  // Constructs a new mymap which is a copy of the "other" mymap.
  // Sets all member variables appropriately.
  // Time complexity: O(n), where n is total number of nodes in threaded,
  // self-balancing BST.
  //
  mymap(const mymap& other) {
    root = nullptr;
    size = 0;
    _preorder_copy(other.root);
  }

  //
  // operator=:
  //
  // Clears "this" mymap and then makes a copy of the "other" mymap.
  // Sets all member variables appropriately.
  // Time complexity: O(n), where n is total number of nodes in threaded,
  // self-balancing BST.
  //
  mymap& operator=(const mymap& other) {
    if (this == &other) {
      return *this;
    }
    NODE* curr = root;
    // clearing "this map"
    _postorder_clear(curr);
    // call the helper function
    _preorder_copy(other.root);
    return *this;  // TODO: Update this return.
  }

  // clear:
  //
  // Frees the memory associated with the mymap; can be used for testing.
  // Time complexity: O(n), where n is total number of nodes in threaded,
  // self-balancing BST.
  //
  void clear() {
    NODE* cur = root;
    _postorder_clear(cur);
    size = 0;
    cur = nullptr;
  }
  //
  // destructor:
  //
  // Frees the memory associated with the mymap.
  // Time complexity: O(n), where n is total number of nodes in threaded,
  // self-balancing BST.
  //
  ~mymap() {
    NODE* cur = root;
    _postorder_clear(cur);
    size = 0;
    cur = nullptr;
  }

  // a copy of the contain function for the put function
  bool _is_there(keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      // if the key already exist then do not put
      if (key == cur->key) {
        return true;
      }
      // go left if it the key is less than the cur->key
      else if (key < cur->key) {
        cur = cur->left;
      }
      // go left if it the key is greater than the cur->key
      else {
        if (cur->isThreaded) {
          cur = nullptr;
        } else {
          cur = cur->right;
        }
      }
    }
    return false;  // TODO: Update this return.
  }

  //
  // put:
  //
  // Inserts the key/value into the threaded, self-balancing BST based on
  // the key.
  // Time complexity: O(logn + mlogm), where n is total number of nodes in the
  // threaded, self-balancing BST and m is the number of nodes in the
  // sub-tree that needs to be re-balanced.
  // Space complexity: O(1)
  //
  void put(keyType key, valueType value) {
    NODE* prev = nullptr;
    NODE* cur = root;
    bool there = _is_there(key);
    // searching for the key
    while (cur != nullptr) {
      // if the key already exist then update the value
      if (key == cur->key) {
        cur->value = value;
        return;
      } else if (key < cur->key) {
        prev = cur;
        if (there == false) {
          cur->nL++;  // need to check if it contains the key
        }
        cur = cur->left;
      } else {
        prev = cur;
        if (there == false) {
          cur->nR++;  // need to check if it contains the key
        }
        if (cur->isThreaded) {
          cur = nullptr;
        } else {
          cur = cur->right;
        }
      }
    }
    // if the key is not found
    NODE* n = new NODE();
    n->key = key;
    n->value = value;
    n->left = nullptr;
    n->right = nullptr;
    n->nL = 0;
    n->nR = 0;
    if (prev == nullptr) {
      root = n;
      //n->isThreaded = true;
    } else if (key < prev->key) {
      prev->left = n;
      n->right = prev;
      n->isThreaded = true;
    } else if(key > prev->key){
      // create another node for the threaded node
      prev->isThreaded = false;
      NODE* new_n = prev->right;
      prev->right = n;
      n->right = new_n;
      n->isThreaded = true;
    }
    size++;
  }

  //
  // contains:
  // Returns true if the key is in mymap, return false if not.
  // Time complexity: O(logn), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  bool contains(keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      // if the key already exist then do not put
      if (key == cur->key) {
        return true;
      }
      // go left if it the key is less than the cur->key
      else if (key < cur->key) {
        cur = cur->left;
      }
      // go left if it the key is greater than the cur->key
      else {
        if (cur->isThreaded) {
          cur = nullptr;
        } else {
          cur = cur->right;
        }
      }
    }
    return false;  // TODO: Update this return.
  }

  //
  // get:
  //
  // Returns the value for the given key; if the key is not found, the
  // default value, valueType(), is returned (but not added to mymap).
  // Time complexity: O(logn), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  valueType get(keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      // if the key is found return the value
      if (key == cur->key) {
        return cur->value;
      }
      // go left if it the key is less than the cur->key
      else if (key < cur->key) {
        cur = cur->left;
      }
      // go left if it the key is greater than the cur->key
      else {
      	if(cur->isThreaded)
      	{
      		cur = nullptr;
      	}
      	else
      	{
      		cur = cur->right;
      	}
      }
    }
    return valueType();  // TODO: Update this return.
  }

  //
  // operator[]:
  //
  // Returns the value for the given key; if the key is not found,
  // the default value, valueType(), is returned (and the resulting new
  // key/value pair is inserted into the map).
  // Time complexity: O(logn + mlogm), where n is total number of nodes in the
  // threaded, self-balancing BST and m is the number of nodes in the
  // sub-trees that need to be re-balanced.
  // Space complexity: O(1)
  //
  valueType operator[](keyType key) {
    NODE* cur = root;
    while (cur != nullptr) {
      // if the key already exist then do not put
      if (key == cur->key) {
        return cur->value;
      }
      else if (key < cur->key) {
        cur = cur->left;
      } else {
        if (cur->isThreaded) {
          cur = nullptr;
        } else {
          cur = cur->right;
        }
      }
    }
    // if the key does not exist insert the key into the map
    put(key, valueType());
    return valueType();
  }

  //
  // Size:
  //
  // Returns the # of key/value pairs in the mymap, 0 if empty.
  // O(1)
  //
  int Size() { return size; }

  //
  // begin:
  //
  // returns an iterator to the first in order NODE.
  // Time complexity: O(logn), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  iterator begin() {
    NODE* curr = root;
    // continuously go left until it cant anymore
    while (curr != nullptr) {
      //
      if (curr->left != nullptr) {
        curr = curr->left;
      }
      // if the left points to nullptr then move right.
      else {
        curr = curr->right;
      }
    }

    return iterator(curr);
  }

  //
  // end:
  //
  // returns an iterator to the last in order NODE.
  // this function is given to you.
  //
  // Time Complexity: O(1)
  //
  iterator end() {
    // no need if its at the end it returns nullptr anyways
    return iterator(nullptr);
  }

  //
  // toString:
  //
  // Returns a string of the entire mymap, in order.
  // Format for 8/80, 15/150, 20/200:
  // "key: 8 value: 80\nkey: 15 value: 150\nkey: 20 value: 200\n
  // Time complexity: O(n), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  string toString() {
    stringstream entire_map;
    _inorder_add_string(root, entire_map);
    return entire_map.str();  // TODO: Update this return.
  }

  //
  // toVector:
  //
  // Returns a vector of the entire map, in order.  For 8/80, 15/150, 20/200:
  // {{8, 80}, {15, 150}, {20, 200}}
  // Time complexity: O(n), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  // vector<pair<keyType, valueType> > toVector() {
  //   vector<int, int> vec;
  //   NODE* cur = root;
  //   while (cur != nullptr) {
  //     vec[cur->key] = cur->value;
  //     cur = cur->next;
  //   }
  //   return vec;  // TODO: Update this return.
  // }

  //
  // checkBalance:
  //
  // Returns a string of mymap that verifies that the tree is properly
  // balanced.  For example, if keys: 1, 2, 3 are inserted in that order,
  // function should return a string in this format (in pre-order):
  // "key: 2, nL: 1, nR: 1\nkey: 1, nL: 0, nR: 0\nkey: 3, nL: 0, nR: 0\n";
  // Time complexity: O(n), where n is total number of nodes in the
  // threaded, self-balancing BST
  //
  string checkBalance() {
    // TODO: write this function.

    return {};  // TODO: Update this return.
  }
};
