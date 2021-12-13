#include <iostream>
#include <memory>
#include <stack>

template <typename T>
class BinarySTree {
public:
    BinarySTree() = default;
    BinarySTree(const BinarySTree&);
    BinarySTree(BinarySTree&&) = default;

    void insert(const T& value);
    void insert(T&& value);

    void remove(const T& value);
    void makeEmpty();

    const T& findMax() const;
    const T& findMin() const;
    bool contains(const T& value) const;
    bool isEmpty() const;

    void print(std::ostream& out = std::cout) const;

    BinarySTree& operator=(const BinarySTree&);
    BinarySTree& operator=(BinarySTree&&) = default;

private:
    struct Node {
        std::unique_ptr<T> value;
        std::shared_ptr<Node> left, right;

        Node(std::unique_ptr<T> vl, std::shared_ptr<Node> lt = nullptr, std::shared_ptr<Node> rt = nullptr)
            : value { std::move(vl) }
            , left { lt }
            , right { rt } {};

        Node(const Node& other)
            : value { std::make_unique<T>(*other.value) }
            , left { other.left }
            , right { other.right } {};
    };

    std::shared_ptr<Node> root;

    void insert(const T& value, std::shared_ptr<Node>& root);
    void insert(T&& value, std::shared_ptr<Node>& root);

    void remove(const T& value, std::shared_ptr<Node>& root);

    std::shared_ptr<Node> findMax(std::shared_ptr<Node> root) const;
    std::shared_ptr<Node> findMin(std::shared_ptr<Node> root) const;
    bool contains(const T& value, std::shared_ptr<Node> root) const;

    void print(std::ostream& out, std::shared_ptr<Node> root) const;

    std::shared_ptr<Node> copy(std::shared_ptr<Node> root) const;

public:
    struct inOrdIterator {
        using iterator_category = std::forward_iterator_tag;
        using diference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        inOrdIterator(std::shared_ptr<Node> root)
        {
            leftPush(root);
        }

        reference operator*()
        {
            return *(m_stack.top()->value);
        }

        pointer operator->()
        {
            return m_stack.top()->value;
        }

        inOrdIterator& operator++()
        {
            (*this)++;
            return *this;
        }

        inOrdIterator operator++(int)
        {
            std::shared_ptr<Node> curr = m_stack.top();
            m_stack.pop();

            if (curr->right != nullptr) {
                leftPush(curr->right);
            }

            return curr;
        }

        friend bool operator==(const inOrdIterator& lhs, const inOrdIterator& rhs)
        {
            if (lhs.m_stack.size() == 0 || rhs.m_stack.size() == 0) {
                return lhs.m_stack.size() != rhs.m_stack.size();
            }
            return lhs.m_stack.top()->value != rhs.m_stack.top()->value;
        }

        friend bool operator!=(const inOrdIterator& lhs, const inOrdIterator& rhs)
        {

            if (lhs.m_stack.size() == 0 || rhs.m_stack.size() == 0) {
                return lhs.m_stack.size() != rhs.m_stack.size();
            }
            return lhs.m_stack.top()->value != rhs.m_stack.top()->value;
        }

    private:
        std::stack<std::shared_ptr<Node>> m_stack;

        void leftPush(std::shared_ptr<Node> root)
        {
            while (root != nullptr) {
                m_stack.push(root);
                root = root->left;
            }
        }
    };

    inOrdIterator begin()
    {
        return inOrdIterator(root);
    }

    inOrdIterator end()
    {
        return inOrdIterator(nullptr);
    }
};

template <typename T>
BinarySTree<T>::BinarySTree(const BinarySTree<T>& other)
    : root { copy(other) } {};

template <typename T>
std::shared_ptr<struct BinarySTree<T>::Node> BinarySTree<T>::copy(std::shared_ptr<Node> root) const {
    if (root != nullptr) {
        return nullptr;
    }
    return std::make_shared<Node> = { root->value, copy(root->left), copy(root->right) };
}

template <typename T>
void BinarySTree<T>::insert(const T& value)
{
    insert(value, root);
}

template <typename T>
void BinarySTree<T>::insert(const T& value, std::shared_ptr<Node>& root)
{
    if (root == nullptr) {
        root = std::make_shared<Node>(std::make_unique<T>(value));
    } else if (value < *(root->value)) {
        insert(value, root->left);
    } else if (value > *(root->value)) {
        insert(value, root->right);
    }
}

template <typename T>
void BinarySTree<T>::insert(T&& value)
{
    insert(std::move(value), root);
}

template <typename T>
void BinarySTree<T>::insert(T&& value, std::shared_ptr<Node>& root)
{
    if (root == nullptr) {
        root = std::make_shared<Node>(std::make_unique<T>(std::move(value)));
    } else if (value < *root->value) {
        insert(std::move(value), root->left);
    } else if (value > *root->value) {
        insert(std::move(value), root->right);
    }
}

template <typename T>
void BinarySTree<T>::remove(const T& value)
{
    remove(value, root);
}

template <typename T>
void BinarySTree<T>::remove(const T& value, std::shared_ptr<Node>& root)
{
    if (value < *root->value) {
        remove(value, root->left);
    } else if (value > *root->value) {
        remove(value, root->right);
    } else if (root->left != nullptr && root->right != nullptr) {
        *root->value = *findMin(root->right)->value;
        remove(*root->value, root->right);
    } else {
        root = root->left == nullptr ? root->right : root->left;
    }
}

template <typename T>
void BinarySTree<T>::makeEmpty()
{
    root = nullptr;
}

template <typename T>
const T& BinarySTree<T>::findMax() const
{
    auto maxPtr = findMax(root);
    return *maxPtr->value;
}

template <typename T>
std::shared_ptr<struct BinarySTree<T>::Node> BinarySTree<T>::findMax(std::shared_ptr<Node> root) const {
    while (root != nullptr && root->right != nullptr)
        root = root->right;
    return root;
}

template <typename T>
const T& BinarySTree<T>::findMin() const
{
    auto minPtr = findMin(root);
    return *minPtr->value;
}

template <typename T>
std::shared_ptr<struct BinarySTree<T>::Node> BinarySTree<T>::findMin(std::shared_ptr<Node> root) const {
    while (root != nullptr && root->left != nullptr)
        root = root->left;
    return root;
}

template <typename T>
bool BinarySTree<T>::contains(const T& value) const
{
    return contains(value, root);
}

template <typename T>
bool BinarySTree<T>::contains(const T& value, std::shared_ptr<Node> root) const
{
    if (root == nullptr) {
        return false;
    } else if (value < *(root->value)) {
        contains(value, root->left);
    } else if (value > *(root->value)) {
        contains(value, root->right);
    }
    return true;
}

template <typename T>
bool BinarySTree<T>::isEmpty() const
{
    return root == nullptr;
}

template <typename T>
void BinarySTree<T>::print(std::ostream& out) const
{
    out << "digraph {" << std::endl;
    print(out, root);
    out << "}" << std::endl;
}

template <typename T>
void BinarySTree<T>::print(std::ostream& out, std::shared_ptr<Node> root) const
{
    if (root == nullptr)
        return;

    if (root->left != nullptr)
        out << *root->value << "->" << *root->left->value << std::endl;
    if (root->right != nullptr)
        out << *root->value << "->" << *root->right->value << std::endl;

    print(out, root->left);
    print(out, root->right);
}
