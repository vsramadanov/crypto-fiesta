#include<array>
#include<algorithm>
#include<memory>
#include<vector>
#include<ranges>
#include<iostream>

namespace utils
{
namespace
{
    int index(char c) { return std::tolower(c) - 'a'; }
}

class prefix_tree
{
    constexpr static int R = 26;
    struct Node
    {
        bool end;
        std::array<std::unique_ptr<Node>, R> next;

        Node(bool is_end = false) : end{is_end}, next{} { }
    };

    Node root{};
    size_t N{};

    class iterator
    {
        Node *ptr;
    public:
        Node* operator->() { return ptr; }
        const Node* operator->() const { return ptr; }
        Node& operator*() { return *ptr; }
        const Node& operator*() const { return *ptr; }

        bool has_next(char c) { return static_cast<bool>(ptr->next[index(c)]); }
        iterator next(char c) { return ptr->next[index(c)].get(); }
        void step(char c) { ptr = ptr->next[index(c)].get(); }

        iterator(Node& node) noexcept { ptr = &node; }
        iterator(Node* node) noexcept { ptr = node; }

        operator bool() const noexcept { return ptr; }
    };

    void match(const std::string& prefix, const std::string& str, iterator it, std::vector<std::string>& storage)
    {
        if(!it)
            return;

        for (int k : std::ranges::views::iota(0UL, str.size()))
        {
            char c = str[k];
            if (c == '?')
            {
                for (int j : std::ranges::views::iota(0, R))
                {
                    char s = 'a' + j;
                    std::string new_prefix = prefix + str.substr(0, k) + s;
                    std::string new_str = str.substr(k + 1);
                    match(new_prefix, new_str, it.next(s), storage);
                }
            }
            else if(it && it.has_next(c))
            {
                it.step(c);
            }
            else
            {
                return;
            }
        }
        if (it->end)
            storage.emplace_back(prefix + str);
    }

public:
    prefix_tree() = default;
    ~prefix_tree() = default;
    
    bool insert(const std::string& key)
    {
        bool out = false;
        iterator it(root);
        for (char c : key)
        {
            if (std::tolower(c) < 'a' || 'z' < std::tolower(c))
                return false;

            if(!it.has_next(c))
                it->next[index(c)] = std::make_unique<Node>();
            it.step(c);
        }
        if (it->end == false)
        {
            it->end = true;
            N++;
            out = true;
        }
        return out;
    }
    bool contains(const std::string& key)
    {
        iterator ptr(root);
        for (char c : key)
        {
            if(!ptr.has_next(c))
                return false;
            ptr.step(c);
        }
        return ptr->end;
    }
    size_t size() { return N; }

    std::vector<std::string> keys()
    {

    }

    std::vector<std::string> match(const std::string& str)
    {
        std::vector<std::string> res{};
        iterator it(root);
        match("", str, it, res);
        return res;
    }
};

}