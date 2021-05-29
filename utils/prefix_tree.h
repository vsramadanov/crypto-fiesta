#include<array>
#include<algorithm>
#include<memory>
#include<vector>
#include<ranges>
#include<iostream>

namespace utils
{

class prefix_tree
{
    constexpr static int R = 'z' - 'a' + 1;
    struct Node
    {
        bool is_end;
        std::array<Node*, R> next;

        Node(bool val = false) : is_end{val}, next{} { }
    };

    Node root{};
    size_t N{};

    int index(char c) { return std::tolower(c) - 'a'; }

    Node* insert(Node* ptr, const std::string& key, size_t k)
    {
        if (ptr == nullptr)
            ptr = new Node;
        if (k == key.size())
        {
            if (ptr->is_end == false)
            {
                ptr->is_end = true;
                N++;
            }
            return ptr;
        }
        size_t idx = index(key[k]);
        ptr->next[idx] = insert(ptr->next[idx], key, k + 1);
        return ptr;
    }

    bool contains(Node *ptr, const std::string& key, size_t k)
    {
        if (ptr == nullptr)
            return false;
        if (k == key.size())
            return ptr->is_end;
        size_t idx = index(key[k]);
        return contains(ptr->next[idx], key, k + 1);
    }

    void match(const std::string& target, Node* root, size_t k, std::string& key, std::vector<std::string>& storage)
    {
        if (root == nullptr)
            return;
        if (k == target.size() && root->is_end)
            storage.push_back(key);

        if (target[k] == '?')
        {
            char c = 'a';
            for (auto* ptr : root->next)
            {
                key.push_back(c++);
                match(target, ptr, k + 1, key, storage);
                key.pop_back();
            }
            return;
        }
        char c = target[k];
        size_t idx = index(c);
        key.push_back(c);
        match(target, root->next[idx], k + 1, key, storage);
        key.pop_back();
        return;
    }

    template<typename Func>
    void visit(Node *root, Func&& f)
    {
        if (root == nullptr)
            return;
        for (auto* ptr : root->next)
            visit(ptr, f);
        f(root);
    }

public:
    prefix_tree() = default;
    ~prefix_tree()
    {
        for (auto* ptr : root.next)
            visit(ptr, [](auto* ptr){ delete ptr; });
    }
    
    void insert(const std::string& key)
    {
        insert(&root, key, 0);
    }
    bool contains(const std::string& key)
    {
        return contains(&root, key, 0);
    }

    std::vector<std::string> keys()
    {
        std::vector<std::string> res{};
        return res;
    }

    size_t size() { return N; }

    std::vector<std::string> match(const std::string& target)
    {
        std::vector<std::string> res{};
        std::string key;
        key.reserve(target.size());
        match(target, &root, 0, key, res);
        return res;
    }
};

}