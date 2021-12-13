#include <iostream>
#include <memory>
#include <stack>

template <typename T>
class AVLTree {
public:
    AVLTree() = default;
    AVLTree(const AVLTree&);
    AVLTree(AVLTree&&) = default;

    void insert(const T& value);
    void insert(T&& value);

    void remove(const T& value);
    void makeEmpty();

    const T& findMax() const;
    const T& findMin() const;
    bool contains(const T& value) const;
    bool isEmpty() const;

    void print(std::ostream& out = std::cout) const;

    AVLTree& operator=(const AVLTree&);
    AVLTree& operator=(AVLTree&&) = default;

private:
    struct Node {
        std::unique_ptr<T> value;
        std::shared_ptr<Node> left, right;
        int height;

        Node(std::unique_ptr<T> vl, std::shared_ptr<Node> lt = nullptr, std::shared_ptr<Node> rt = nullptr, const int& h = 0)
            : value { std::move(vl) }
            , left { lt }
            , right { rt }
            , height { h } {};

        Node(const Node& other)
            : value { std::make_unique<T>(*other.value) }
            , left { other.left }
            , right { other.right }
            , height { other.height } {};
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

    static constexpr int ALLOWED_INBALANCE = 1;
    int height(std::shared_ptr<Node> root) const;
    void rotateLeftChild(std::shared_ptr<Node>& root);
    void rotateRightChild(std::shared_ptr<Node>& root);
    void doubleLeftChild(std::shared_ptr<Node>& root);
    void doubleRightChild(std::shared_ptr<Node>& root);
    void balance(std::shared_ptr<Node>& root);

public:
    struct InOrdIterator {
        using iterator_category = std::forward_iterator_tag;
        using diference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        InOrdIterator(std::shared_ptr<Node> root)
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

        InOrdIterator& operator++()
        {
            (*this)++;
            return *this;
        }

        InOrdIterator operator++(int)
        {
            std::shared_ptr<Node> curr = m_stack.top();
            m_stack.pop();

            if (curr->right != nullptr) {
                leftPush(curr->right);
            }

            return curr;
        }

        friend bool operator==(const InOrdIterator& lhs, const InOrdIterator& rhs)
        {
            if (lhs.m_stack.size() == 0 || rhs.m_stack.size() == 0) {
                return lhs.m_stack.size() != rhs.m_stack.size();
            }
            return lhs.m_stack.top()->value != rhs.m_stack.top()->value;
        }

        friend bool operator!=(const InOrdIterator& lhs, const InOrdIterator& rhs)
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

    InOrdIterator begin()
    {
        return InOrdIterator(root);
    }

    InOrdIterator end()
    {
        return InOrdIterator(nullptr);
    }
};

template <typename T>
AVLTree<T>::AVLTree(const AVLTree<T>& other)
    : root { copy(other) } {};

template <typename T>
std::shared_ptr<struct AVLTree<T>::Node> AVLTree<T>::copy(std::shared_ptr<Node> root) const {
    if (root != nullptr) {
        return nullptr;
    }
    return std::make_shared<Node> = { root->value, copy(root->left), copy(root->right) };
}

template <typename T>
void AVLTree<T>::insert(const T& value)
{
    insert(value, root);
}

template <typename T>
int AVLTree<T>::height(std::shared_ptr<Node> root) const
{
    return root == nullptr ? -1 : root->height;
}

template <typename T>
void AVLTree<T>::rotateLeftChild(std::shared_ptr<Node>& root)
{
    auto tmp = root->left;
    root->left = tmp->right;
    tmp->right = root;
    root->height = std::max(height(root->left), height(root->right)) + 1;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
    root = tmp;
}

template <typename T>
void AVLTree<T>::rotateRightChild(std::shared_ptr<Node>& root)
{
    auto tmp = root->right;
    root->right = tmp->left;
    tmp->left = root;
    root->height = std::max(height(root->left), height(root->right)) + 1;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
    root = tmp;
}

template <typename T>
void AVLTree<T>::doubleLeftChild(std::shared_ptr<Node>& root)
{
    rotateRightChild(root->left);
    rotateLeftChild(root);
}

template <typename T>
void AVLTree<T>::doubleRightChild(std::shared_ptr<Node>& root)
{
    rotateLeftChild(root->right);
    rotateRightChild(root);
}

template <typename T>
void AVLTree<T>::balance(std::shared_ptr<Node>& root)
{
    if (root == nullptr)
        return;
    if (height(root->left) - height(root->right) > ALLOWED_INBALANCE)
        if (height(root->left->left) >= height(root->left->right))
            rotateLeftChild(root);
        else
            doubleLeftChild(root);
    else if (height(root->right) - height(root->left) > ALLOWED_INBALANCE)
        if (height(root->right->right) >= height(root->right->left))
            rotateRightChild(root);
        else
            doubleRightChild(root);

    root->height = std::max(height(root->left), height(root->right)) + 1;
}

template <typename T>
void AVLTree<T>::insert(const T& value, std::shared_ptr<Node>& root)
{
    if (root == nullptr) {
        root = std::make_shared<Node>(std::make_unique<T>(value));
    } else if (value < *(root->value)) {
        insert(value, root->left);
    } else if (value > *(root->value)) {
        insert(value, root->right);
    }
    balance(root);
}

template <typename T>
void AVLTree<T>::insert(T&& value)
{
    insert(std::move(value), root);
}

template <typename T>
void AVLTree<T>::insert(T&& value, std::shared_ptr<Node>& root)
{
    if (root == nullptr) {
        root = std::make_shared<Node>(std::make_unique<T>(std::move(value)));
    } else if (value < *root->value) {
        insert(std::move(value), root->left);
    } else if (value > *root->value) {
        insert(std::move(value), root->right);
    }

    balance(root);
}

template <typename T>
void AVLTree<T>::remove(const T& value)
{
    remove(value, root);
}

template <typename T>
void AVLTree<T>::remove(const T& value, std::shared_ptr<Node>& root)
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
    balance(root);
}

template <typename T>
void AVLTree<T>::makeEmpty()
{
    root = nullptr;
}

template <typename T>
const T& AVLTree<T>::findMax() const
{
    auto maxPtr = findMax(root);
    return *maxPtr->value;
}

template <typename T>
std::shared_ptr<struct AVLTree<T>::Node> AVLTree<T>::findMax(std::shared_ptr<Node> root) const {
    while (root != nullptr && root->right != nullptr)
        root = root->right;
    return root;
}

template <typename T>
const T& AVLTree<T>::findMin() const
{
    auto minPtr = findMin(root);
    return *minPtr->value;
}

template <typename T>
std::shared_ptr<struct AVLTree<T>::Node> AVLTree<T>::findMin(std::shared_ptr<Node> root) const {
    while (root != nullptr && root->left != nullptr)
        root = root->left;
    return root;
}

template <typename T>
bool AVLTree<T>::contains(const T& value) const
{
    return contains(value, root);
}

template <typename T>
bool AVLTree<T>::contains(const T& value, std::shared_ptr<Node> root) const
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
bool AVLTree<T>::isEmpty() const
{
    return root == nullptr;
}

template <typename T>
void AVLTree<T>::print(std::ostream& out) const
{
    out << "digraph {" << std::endl;
    print(out, root);
    out << "}" << std::endl;
}

template <typename T>
void AVLTree<T>::print(std::ostream& out, std::shared_ptr<Node> root) const
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
