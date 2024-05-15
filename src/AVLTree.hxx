#pragma once
#include <vector>
#include <utility>

template <typename E>
class AVLTree {
private:
    struct Node {
        int key;
        E data;
        int left, right;
        int height;

        Node(int k, const E& d) : key(k), data(d), left(-1), right(-1), height(1) {}
    };

    std::vector<Node> nodes;

    int height(int index) {
        return (index == -1) ? 0 : nodes[index].height;
    }

    int getBalanceFactor(int index) {
        return height(nodes[index].right) - height(nodes[index].left);
    }

    void fixHeight(int index) {
        int leftHeight = height(nodes[index].left);
        int rightHeight = height(nodes[index].right);
        nodes[index].height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
    }

    int rotateRight(int y) {
        int x = nodes[y].left;
        nodes[y].left = nodes[x].right;
        nodes[x].right = y;
        fixHeight(y);
        fixHeight(x);
        return x;
    }

    int rotateLeft(int x) {
        int y = nodes[x].right;
        nodes[x].right = nodes[y].left;
        nodes[y].left = x;
        fixHeight(x);
        fixHeight(y);
        return y;
    }

    int balance(int index) {
        fixHeight(index);

        if (getBalanceFactor(index) == 2) {
            if (getBalanceFactor(nodes[index].right) < 0)
                nodes[index].right = rotateRight(nodes[index].right);
            return rotateLeft(index);
        }

        if (getBalanceFactor(index) == -2) {
            if (getBalanceFactor(nodes[index].left) > 0)
                nodes[index].left = rotateLeft(nodes[index].left);
            return rotateRight(index);
        }

        return index;
    }

    int insert(int index, int key, const E& data) {
        if (index == -1) {
            nodes.push_back(Node(key, data));
            return nodes.size() - 1;
        }

        if (key < nodes[index].key)
            nodes[index].left = insert(nodes[index].left, key, data);
        else
            nodes[index].right = insert(nodes[index].right, key, data);

        return balance(index);
    }

    int remove(int index, int key) {
        if (index == -1) return -1;

        if (key < nodes[index].key)
            nodes[index].left = remove(nodes[index].left, key);
        else if (key > nodes[index].key)
            nodes[index].right = remove(nodes[index].right, key);
        else {
            int l = nodes[index].left;
            int r = nodes[index].right;

            nodes.erase(nodes.begin() + index);

            if (r == -1) return l;

            int min = findMin(r);
            nodes[min].right = removeMin(r);
            nodes[min].left = l;

            return balance(min);
        }

        return balance(index);
    }

    int findMin(int index) {
        return (nodes[index].left == -1) ? index : findMin(nodes[index].left);
    }

    int removeMin(int index) {
        if (nodes[index].left == -1) return nodes[index].right;

        nodes[index].left = removeMin(nodes[index].left);
        return balance(index);
    }

public:
    AVLTree() = default;

    void clear() {
        nodes.clear();
    }

    bool has(int key) const {
        if (nodes.size() == 0){
            return false;
        }
        int curr = 0;
        while (curr != -1) {
            if (key < nodes[curr].key)
                curr = nodes[curr].left;
            else if (key > nodes[curr].key)
                curr = nodes[curr].right;
            else
                return true;
        }
        return false;
    }

    E getValue(int key) const {
        int curr = 0;
        while (curr != -1) {
            if (key < nodes[curr].key)
                curr = nodes[curr].left;
            else if (key > nodes[curr].key)
                curr = nodes[curr].right;
            else
                return nodes[curr].data;
        }
        return E();
    }

    void setValue(int key, E value) {
        int curr = 0;
        while (curr != -1) {
            if (key < nodes[curr].key)
                curr = nodes[curr].left;
            else if (key > nodes[curr].key)
                curr = nodes[curr].right;
            else
            {
                nodes[curr].data = value;
                break;
            }
        }
    }

    void insertKey(int key, const E& data) {
        if (nodes.empty())
            nodes.push_back(Node(key, data));
        else
            insert(0, key, data);
    }

    void removeKey(int key) {
        if (!nodes.empty())
            remove(0, key);
    }

    std::vector<int> getKeys() const {
        std::vector<int> result;
        if(nodes.size() == 0){
            return result;
        }
        std::cout << "Nodes size: " << nodes.size() << std::endl;
        for (int i=0; i<nodes.size(); i++)
        {
            std::cout << i << ' ' << nodes[i].key <<' ' << nodes[i].left << ' ' << nodes[i].right << std::endl;
        }
        keysHelper(0, result);
        return result;
    }

    void keysHelper(int index, std::vector<int>& result) const {
        std::cout << "push " << index << std::endl;
        if (index >= 0 && index < nodes.size()) {
            keysHelper(nodes[index].left, result);
            result.push_back(nodes[index].key);
            keysHelper(nodes[index].right, result);
        }
    }

    std::vector<std::pair<int, E>> entries() const {
        std::vector<std::pair<int, E>> result;
        entriesHelper(0, result);
        return result;
    }

    void entriesHelper(int index, std::vector<std::pair<int, E>>& result) const {
        if (index == -1) return;
        entriesHelper(nodes[index].left, result);
        result.emplace_back(nodes[index].key, nodes[index].data);
        entriesHelper(nodes[index].right, result);
    }

    std::vector<E> values() const {
        std::vector<E> result;
        valuesHelper(0, result);
        return result;
    }

    void valuesHelper(int index, std::vector<E>& result) const {
        if (index == -1) return;
        valuesHelper(nodes[index].left, result);
        result.push_back(nodes[index].data);
        valuesHelper(nodes[index].right, result);
    }

    int size() const {
        return nodes.size();
    }
};