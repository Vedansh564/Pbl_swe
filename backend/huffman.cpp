#include "huffman.h"
#include<bits/stdc++.h>
using namespace std;
Huffman::Huffman() {}

void Huffman::buildFrequencyTable(string& inputFilePath) {
    ifstream inFile(inputFilePath, ios::binary);
    if (!inFile) {
        cout << "Error opening file: " << inputFilePath << endl;
        return;
    }
    char ch;
    while (inFile.get(ch)) {
        frequency[ch]++;
    }
    inFile.close();
}

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

void Huffman::buildHuffmanTree() {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    for (auto& pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }

    root = minHeap.top();
    generateHuffmanCodes(root, "");
}

void Huffman::generateHuffmanCodes(Node* node, std::string code) {
    if (!node) return;
    if (!node->left && !node->right) {
        huffmanCode[node->ch] = code;
    }
    generateHuffmanCodes(node->left, code + "0");
    generateHuffmanCodes(node->right, code + "1");
}
void Huffman::writeEncodedFile(string& inputFilePath, string& outputFilePath) {
    ifstream inFile(inputFilePath, ios::binary);
    ofstream outFile(outputFilePath, ios::binary);
    if (!inFile || !outFile) {
         cout<<"Error opening files for writing.\n";
        return;
    }

    string encodedText = "";
    char ch;
    while (inFile.get(ch)) {
        encodedText += huffmanCode[ch];
    }

    int extraBits = 8 - (encodedText.size() % 8);  
    for (int i = 0; i < extraBits; i++) encodedText += "0";
    outFile.put(extraBits);
    for (size_t i = 0; i < encodedText.size(); i += 8) {
        bitset<8> byte(encodedText.substr(i, 8));
        outFile.put(static_cast<unsigned char>(byte.to_ulong()));
    }
    inFile.close();
    outFile.close();
}
void Huffman::compress(string& inputFilePath,string& outputFilePath) {
    buildFrequencyTable(inputFilePath);
    buildHuffmanTree();
    writeEncodedFile(inputFilePath, outputFilePath);
}

