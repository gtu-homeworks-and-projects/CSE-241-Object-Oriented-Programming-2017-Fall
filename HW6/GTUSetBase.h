//
// Created by cecen on 12/2/2017.
//

#ifndef HW6_GTUSETBASE_H
#define HW6_GTUSETBASE_H

#include <iostream>
#include <memory>
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::cout;
using std::cin;
using std::endl;
using std::invalid_argument;

namespace containers{
    template <class T>
    class GTUSetBase {
    public:
        GTUSetBase();

        class DataNode{ // Linked List type data for set
        public:
            DataNode(const T& element);

            shared_ptr<T> data = nullptr;
            shared_ptr<DataNode> next = nullptr;
        };

        class GTUIterator{ // Iterator declaration
        private:
            const GTUSetBase<T> &tempSet;
            shared_ptr<DataNode> headNode;
            shared_ptr<DataNode> currentNode;
            unsigned int index = 1; // Means which data to point
        public:
            GTUIterator(const GTUSetBase &refSet);
            GTUIterator(GTUSetBase &refSet);
            GTUIterator (GTUSetBase &refSet, unsigned int i);

            void update(); // Updates iterator variables to set variables
            T& operator*();
            shared_ptr<GTUSetBase<T>::DataNode> operator->();
            GTUIterator& operator++();
            GTUIterator& operator++(int);
            GTUIterator& operator--();
            GTUIterator& operator--(int);
            GTUIterator& operator=(const GTUIterator& other);
            bool operator!= (const GTUIterator &other);
            bool operator== (const GTUIterator &other);
            bool operator==(const T& other); // Checks elements equality

            unsigned int getIndex(); // gets where iterator is
        };

        virtual bool empty() const noexcept = 0;
        virtual bool isMember (const T& element) const noexcept = 0;
        virtual unsigned int size() const noexcept = 0;
        virtual unsigned int max_size() const noexcept = 0; // returns sets maximum size state
        virtual void insert(const T& element) throw(invalid_argument) = 0;
        virtual void erase(const T& element) = 0;
        virtual void clear() noexcept = 0;
        virtual GTUIterator find(const T& val) const = 0;
        virtual unsigned int count(const T& val) const = 0;
        virtual GTUIterator begin() = 0;
        virtual GTUIterator end() = 0;
    protected:
        shared_ptr<unsigned int> _size;
        shared_ptr<unsigned int> _maxSize;
        shared_ptr<DataNode> testDataHead;
    };


}

#endif //HW6_GTUSETBASE_H
