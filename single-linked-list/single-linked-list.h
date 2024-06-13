#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

#include <iostream>

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList() : size_(0) {
        //
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    SingleLinkedList(std::initializer_list<Type> values) {
        FillFromContainer(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        FillFromContainer(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
	if (&rhs != this) {
            SingleLinkedList tmp(rhs);
            this->swap(tmp);
	}
        return *this;
    }

   
    void swap(SingleLinkedList& other) noexcept {
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
        std::swap(other.head_ptr_, head_ptr_);
    }

    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void PopFront() noexcept {
        Node* to_remove = head_.next_node;
        head_.next_node = to_remove->next_node;
        to_remove->next_node = nullptr;
        delete to_remove;
        --size_;
    }
    

    void Clear() noexcept {
        Node* tmp = nullptr;
        while (head_.next_node) {
            tmp = head_.next_node;
            head_.next_node = head_.next_node->next_node;
            delete tmp;
        }
	size_ = 0;
    }

    template <typename ValueType>
    class BasicIterator {
 
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
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
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
	    assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
	    assert(node_ != nullptr);
            BasicIterator copy(node_);
            node_ = node_->next_node;
            return copy;
        }

        [[nodiscard]] reference operator*() const noexcept {
	    assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
	    assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;

    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
 
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }
    
    Node* GetHeadPtr() const noexcept {
        return &head_;
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(head_ptr_);
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(head_ptr_);
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator(pos.node_->next_node);
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        Node* next = pos.node_->next_node;
        pos.node_->next_node = next->next_node;
        next->next_node = nullptr;
        delete next;
        --size_;
        return Iterator(pos.node_->next_node);
    }

private:
    Node head_;
    size_t size_ = 0;
    Node* head_ptr_ = &head_;

private:    
    template<typename T>
    void FillFromContainer(T& container) {
        Node* curr = &head_;
        for (const auto& value : container) {
            curr->next_node = new Node(value, nullptr);
            curr = curr->next_node;
            ++size_;
        }
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
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

