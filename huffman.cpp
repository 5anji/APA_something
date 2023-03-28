#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>

class Node {
public:
    char ch;
    int freq;
    Node *left, *right;
};

Node* get_node(char ch, int freq, Node* left, Node* right) {
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comparator {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void encode_char(Node* root, std::unordered_map<char, std::string>& huffman_code, std::string const& str = "") {
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        huffman_code[root->ch] = str;
    }

    encode_char(root->left, huffman_code, str + "0");
    encode_char(root->right, huffman_code, str + "1");
}

void decode_char(Node* root, int& index, std::string const& str) {
    if (root == nullptr) {
        return;
    }

    if (!root->left && !root->right) {
        std::cout << root->ch;
        return;
    }

    index++;

    (str[index] == '0') ? decode_char(root->left, index, str)
                        : decode_char(root->right, index, str);
}

void build_huffman_tree(std::string const& text) {
    std::unordered_map<char, int> freq;
    for (auto&& ch : text) {
        freq[ch]++;
    }

    std::priority_queue<Node*, std::vector<Node*>, comparator> pq;

    for (auto&& pair : freq) {
        pq.push(get_node(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(get_node('\0', sum, left, right));
    }

    Node* root = pq.top();

    std::unordered_map<char, std::string> huffman_code;
    encode_char(root, huffman_code);

    std::cout << "Huffman Codes are: ";
    for (auto&& pair : huffman_code) {
        std::cout << "[\'" << pair.first << "\' -> " << pair.second << "]  ";
    }

    std::cout << std::endl
              << std::endl
              << "Original string was: " << text << std::endl;

    std::stringstream str;

    for (auto&& ch : text) {
        str << huffman_code[ch];
    }

    std::cout << std::endl
              << "Encoded string is: " << str.str() << std::endl;

    int index = -1;
    std::cout << std::endl
              << "Decoded string is: ";

    while (index < static_cast<int>(str.str().size()) - 2) {
        decode_char(root, index, str.str());
    }

    std::cout << std::endl;

    delete root;
}

int main() {
    std::string text;

    std::cout << "Input text: ";
    std::cin >> text;

    build_huffman_tree(text);

    return 0;
}