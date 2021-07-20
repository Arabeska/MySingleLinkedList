#pragma once

#include <cassert>
#include <cstddef>
#include <utility>
#include <initializer_list>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value = Type();
        Node* next_node = nullptr;
    };

public:

    SingleLinkedList<Type>() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        InitList(values, static_cast<int>(values.size()));
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);
        InitList(other, static_cast<int>(other.GetSize()));
    }

    ~SingleLinkedList() {
        Clear();
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            auto rhs_copy(rhs);
            swap(rhs_copy);
        }
        return *this;
    }

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) :node_(node) {
        }

    public:

        // Категория итератора - forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }
        
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            assert(node_ != nullptr);
            auto old_value(*this); 
            ++(*this); 
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ ptr_head_ };
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ ptr_head_ };
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto* node_ptr = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = node_ptr;
            node_ptr = nullptr;
        }
        size_ = 0;
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return GetSize() == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        (*(pos.node_)).next_node = new Node(value, ((*(pos.node_)).next_node));
        ++size_;
        return Iterator{ (*(pos.node_)).next_node };
    }

    void PopFront() noexcept {
        assert(IsEmpty() == false);
        auto* node_ptr = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = node_ptr;
        node_ptr = nullptr;
        --size_;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert((*(pos.node_)).next_node != nullptr);
        auto* x = (*(*(pos.node_)).next_node).next_node;
        delete (*(pos.node_)).next_node;
        (*(pos.node_)).next_node = x;
        x = nullptr;
        --size_;
        return Iterator{ (*(pos.node_)).next_node };
    }

private:
    Node head_;
    Node* ptr_head_ = &head_;
    size_t size_ = 0;

    template <typename Object>
    void InitList(const Object& obj, int size) {
        SingleLinkedList tmp;
        for (int i = size; i != 0; i--) {
            auto it_obj = obj.begin();
            std::advance(it_obj, (i - 1));
            tmp.PushFront(*it_obj);
        }
        swap(tmp);
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
