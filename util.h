// Ayokunle Olugboyo
// STARTER CODE: util.h
//
// TODO:  Write your own header and fill in all functions below.
//
#include <fstream>
#include <functional>  // std::greater
#include <iostream>
#include <map>
#include <queue>  // std::priority_queue
#include <string>
#include <vector>  // std::vector

#include "bitstream.h"
#include "hashmap.h"
#include "mymap.h"
#pragma once

struct HuffmanNode {
  int character;
  int count;
  HuffmanNode* zero;
  HuffmanNode* one;
};
// for the priority queue comparism function
class prioitize {
 public:
  bool operator()(const HuffmanNode* p1, const HuffmanNode* p2) const {
    return p1->count > p2->count;
  }
};
//
// *This method frees the memory allocated for the Huffman tree.
// Recursively
void freeTree(HuffmanNode* node) {
  if (node == nullptr) {
    return;
  }
  // free HuffmanNode zero recursively
  freeTree(node->zero);
  // free HuffmanMode one recursively
  freeTree(node->one);
  delete node;
  node = nullptr;
}

//
// *This function build the frequency map.  If isFile is true, then it reads
// from filename.  If isFile is false, then it reads from a string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap& map) {
  if (isFile) { // dudes in computer science
    ifstream inFS(filename);
    char c; 
    while (inFS.get(c)) {
      // using the containsKey function in Hashmaps
      if (map.containsKey(c)) {
        // get character c from the map and put it back in the
        // map as key and value
        int b = map.get(c) + 1;
        map.put(c, b);
        // if it is not in the key keep the value as one
      } else {
        map.put(c, 1);
      }
    }
  }
  // treating filename as a string variable
  else {
    for (char c : filename) {
      // copy same the one before check if it contains
      // then put in the map
      if (!map.containsKey(c)) {
        map.put(c, 1);
      } else {
        int b = map.get(c) + 1;
        map.put(c, b);
      }
    }
  }
  map.put(PSEUDO_EOF, 1);
}

//
// *This function builds an encoding tree from the frequency map.
// In the priority queue the small one's have priority, at the top
HuffmanNode* buildEncodingTree(hashmap& map) {
  priority_queue<HuffmanNode*, vector<HuffmanNode*>, prioitize> queue;
  vector<int> keys = map.keys();
  for (int i = 0; i < keys.size(); i++) {
    // creating a huffmanNode to put in the queue
    // with the size of the key in the hashmap
    HuffmanNode* current = new HuffmanNode;
    current->count = map.get(keys[i]);
    current->one = nullptr;
    current->zero = nullptr;
    current->character = keys[i];
    queue.push(current);
  }
  // going through the queue up until the last node
  // pop 2 top nodes at a time for node one and zero
  // then put the addition of each node count back into the queue
  while (queue.size() > 1) {
    // creating a new node
    HuffmanNode* current = new HuffmanNode;
    current->zero = queue.top();
    // get the count for encoding of the first node
    int code1 = queue.top()->count;
    queue.pop();
    current->one = queue.top();
    // get the count for encoding of the second node
    int code2 = queue.top()->count;
    queue.pop();
    current->character = NOT_A_CHAR;
    current->count = code1 + code2;
    queue.push(current);
  }

  return queue.top();  // TO DO: update this return
}
// helper function to traverse through the tree to build the map
void traverse_preorder(HuffmanNode* tree, mymap<int, string>& encodingMap,
                       string s) {
  if (tree == nullptr) {
    return;
  }
  // NOT_A_CHAR is found
  if (tree->one == nullptr && tree->zero == nullptr) {
    encodingMap.put(tree->character, s);
  }
  traverse_preorder(tree->zero, encodingMap, s + "0");
  traverse_preorder(tree->one, encodingMap, s + "1");
}
//
// *This function builds the encoding map from an encoding tree.
//
mymap<int, string> buildEncodingMap(HuffmanNode* tree) {
  mymap<int, string> encodingMap;
  string s;
  // if the tree is empty nothing happens
  if (tree == nullptr) {
    return encodingMap;
  }
  // needs a helper function to use pre_order treverse
  traverse_preorder(tree, encodingMap, s);
  return encodingMap;  // TO DO: update this return
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, mymap<int, string>& encodingMap,
              ofbitstream& output, int& size, bool makeFile) {
  string encoding;
  char str;
  if (makeFile == true) {
    // getting character from the file and their encodings
    while (input.get(str)) {
      encoding += encodingMap.get(str);
    }
    // done getting characters
    encoding += encodingMap.get(PSEUDO_EOF);
    size = encoding.size();
    // for each char encoding write the bit
    for (int i = 0; i < size; i++) {
      if (encoding[i] == '1') {
        output.writeBit(1);
      } else if (encoding[i] == '0') {
        output.writeBit(0);
      }
    }
  } else {
    while (input.get(str)) {
      encoding += encodingMap.get(str);
    }
    // done getting characters
    encoding += encodingMap.get(PSEUDO_EOF);
    size = encoding.size();
  }
  return encoding;  // TO DO: update this return
}

//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream& input, HuffmanNode* encodingTree, ofstream& output) {
  string decoding = "";
  string bit_node = "";
  // to get the bit of the file
  while (!input.eof()) {
    int bit = input.readBit();
    if (bit == 0) {
      bit_node += "0";
    } else if (bit == 1) {
      bit_node += "1";
    }
  }
  HuffmanNode* current = encodingTree;
  // go through each character of the bit
  for (char a : bit_node) {
    if (encodingTree->one == nullptr && encodingTree->zero == nullptr) {
      // if it is end of character break
      if (encodingTree->character == PSEUDO_EOF) {
        break;
      }
      // if it is a character output it and add it to the decoding string
      if (encodingTree->character != NOT_A_CHAR) {
        output.put(encodingTree->character);
        decoding += encodingTree->character;
      }
      encodingTree = current;
    }
    if (a == '1') {
      encodingTree = encodingTree->one;
    } else if (a == '0') {
      encodingTree = encodingTree->zero;
    }
  }

  return decoding;  // TO DO: update this return
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
  string tag = ".huf";
  hashmap maps;
  mymap<int, string> map_compress;
  // build the frequency
  buildFrequencyMap(filename, true, maps);
  HuffmanNode* Node = buildEncodingTree(maps);
  // build the map from the node
  map_compress = buildEncodingMap(Node);
  ifstream input(filename);
  ofbitstream output(filename + tag);
  stringstream ss;
  ss << maps;
  output << maps;
  int size;
  string str = encode(input, map_compress, output, size, true);
  freeTree(Node);
  return str;  // TO DO: update this return
}

//
// *This function completes the entire decompression process.  Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// using the encoding tree to decode the file.  This function should create a
// compressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt".  The function should return a string version of the
// uncompressed file.  Note: this function should reverse what the compress
// function did.
//
string decompress(string filename) {
  size_t pos = filename.find(".huf");
  if ((int)pos >= 0) {
    filename = filename.substr(0, pos);
  }
  pos = filename.find(".");
  string ext = filename.substr(pos, filename.length() - pos);
  filename = filename.substr(0, pos);
  ifbitstream input(filename + ext + ".huf");
  ofstream output(filename + "_unc" + ext);
  hashmap dump;
  input >> dump;  // get rid of frequency map at top of file
  
  HuffmanNode* encodingTree = buildEncodingTree(dump);
  string decodeStr = decode(input, encodingTree, output);
  freeTree(encodingTree);
  return decodeStr;  // TO DO: update this return
}
