//
// Created by cecen on 12/2/2017.
//

#include "GTUSet.h"
#include "GTUSetBase.cpp"

namespace containers {
    template<class T>
    GTUSet<T>::GTUSet () {
        initClass();
    }
    template<class T>
    void GTUSet<T>::insert (const T &element) throw(invalid_argument){
        if(isMember(element)) { // If member isnt in the set throws invalid argument exception
            throw invalid_argument("Element is already in set.");
        }
        else {
            shared_ptr<DataNode> tempNode(new DataNode(element)); // Temporary element node
            if (this->testDataHead == nullptr || *this->testDataHead->data >= *tempNode->data) { // If set is empty or element is smaller than first element
                tempNode->next = this->testDataHead; // Temporary element becomes head
                this->testDataHead = tempNode;
            } else {
                shared_ptr<DataNode> currentNode(this->testDataHead); // In order to keep head makes its copy
                for (; currentNode->next != nullptr && *currentNode->next->data < *tempNode->data;
                       currentNode = currentNode->next); // Node places just between other nodes
                tempNode->next = currentNode->next;
                currentNode->next = tempNode;
            }
            (*this->_size)++; // size increases no matter where data inserted
            if(size() > max_size()) *this->_maxSize = this->size();
        }
    }

    template<class T>
    void GTUSet<T>::initClass () {
        this->_maxSize = make_shared<unsigned int>(0);
        this->_size = make_shared<unsigned int>(0);
    }

    template<class T>
    void GTUSet<T>::erase (const T& element) {
        if(isMember(element)) { // Makes sure element is in the set
            GTUIterator tempVal = find(element);

            if (tempVal.getIndex() == size()) { //Special case last value. Marks previous nodes next as nullptr
                --tempVal;
                tempVal->next == nullptr;
            } else if (tempVal.getIndex() == 1) { //Special case first value. Swaps second nodes value and points to third.
                tempVal->data = tempVal->next->data;
                tempVal->next = tempVal->next->next;
            } else { //Just skips that node between previous and next nodes.
                --tempVal;
                tempVal->next = tempVal->next->next;
            }
            (*this->_size)--;
        }
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator GTUSet<T>::find (const T &element) const {
        GTUIterator tempVar(*this);
        int i;
        for (i = 1; i < this->size() && *tempVar != element; ++i,++tempVar);
        return tempVar;
    }

    template<class T>
    void GTUSet<T>::clear () noexcept {
        this->testDataHead = nullptr; // Smart pointers will delete itself
        *this->_size = 0; // size is 0
    }

    template<class T>
    unsigned int GTUSet<T>::count (const T &val) const {
        GTUIterator tempVal = find(val);
        return *tempVal == val ? 1 : 0; // since this is a set, A set can only contain maximum 1 of every element
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator GTUSet<T>::begin () {
        return GTUIterator(*this);
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator GTUSet<T>::end () {
        return GTUIterator(*this,this->size() + 1);
    }

    template<class T>
    bool GTUSet<T>::isMember (const T& element) const noexcept {
        if(empty()) return false;
        GTUIterator tempVar(*this);
        int i;
        for (i = 1; i < this->size() && *tempVar != element; ++i,++tempVar);
        return !(i == this->size() && *tempVar != element);
    }

    template<class T>
    shared_ptr<GTUSetBase<T> > setIntersection (const GTUSetBase<T> &first, const GTUSetBase<T> &second) {
        shared_ptr< containers::GTUSetBase<T> > InterSet(new GTUSet<T>);
        typename GTUSetBase<T>::GTUIterator iterFirst(first);
        for (int j = 0; j < first.size(); ++j, ++iterFirst)
            InterSet->insert(*iterFirst); //Fills with first set

        typename GTUSetBase<T>::GTUIterator iterSecond(second);
        for (int i = 0; i < second.size(); ++i, ++iterSecond)
            if(!first.isMember(*iterSecond))
                InterSet->insert(*iterSecond); //Fills with second set if member isnt in the first set

        return InterSet;
    }
}