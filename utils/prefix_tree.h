#include<array>
#include<algorithm>
#include<memory>
#include<vector>
#include<ranges>
#include<iostream>
#include<iterator>
#include<stack>
#include<string>

namespace utils
{

class prefix_tree
{
    constexpr static int R = 'z' - 'a' + 1;
    struct Node {
        bool is_end;
        std::array<Node*, R> next;

        Node(bool val = false) : is_end{val}, next{} { }
    };

    Node   root{};
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

    /*
     * visits all nades.
     * implemented with depth first search
     */
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

    class const_iterator : public std::iterator<
                            std::forward_iterator_tag,   // iterator_category
                            std::string,                 // value_type
                            long,                        // difference_type
                            const std::string*,          // pointer
                            std::string> {               // reference
        const Node *root;
        struct NodeDescriptor {
            const Node *node;
            char symbol;
            int level;
        };
        std::stack<NodeDescriptor> st{};
        std::string key{};

        void dfs() {
            while(!st.empty()) {
                auto descr = st.top();
                st.pop();

                for(auto idx : std::views::iota(0, R) | std::views::reverse) {
                    auto w = descr.node->next[idx];
                    if (w)
                        st.push({w, static_cast<char>('a' + idx), descr.level + 1});
                }

                key.resize(descr.level);
                key.push_back(descr.symbol);
                if (descr.node->is_end)
                    return;
            }
            key = "";
        }
    public:
        explicit const_iterator(const Node *node, bool end = false) : root(node) {
            if (end)
                return;

            for(auto idx : std::views::iota(0, R) | std::views::reverse) {
                auto w = root->next[idx];
                if (w)
                    st.push({w, static_cast<char>('a' + idx), 0});
            }
            dfs();
        }

        const_iterator& operator++() { dfs(); return *this; }
        bool            operator==(const_iterator other) const { return root == other.root && key == other.key; }
        bool            operator!=(const_iterator other) const { return !(*this == other); }
        reference       operator*() const { return key; }
        pointer         operator->() const { return &key; }
    };

    const_iterator cbegin() const { return const_iterator{&root}; }
    const_iterator cend() const  { return const_iterator{&root, true}; }

    auto begin() const { return cbegin(); }  // work around for
    auto end() const  { return cend(); }     // range-based loops


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