//
// Created by cecen on 12/2/2017.
//

#include "GTUSetBase.h"

//This file only contains Data and Iterator class implementations

namespace containers{
    template<class T>
    GTUSetBase<T>::GTUSetBase () {}

    template<class T>
    GTUSetBase<T>::GTUIterator::GTUIterator (GTUSetBase &refSet, unsigned int i):tempSet(refSet),index(i){}

    template<class T>
    GTUSetBase<T>::GTUIterator::GTUIterator (GTUSetBase &refSet):GTUIterator(refSet,1) {}

    template<class T>
    GTUSetBase<T>::GTUIterator::GTUIterator (const GTUSetBase &refSet):tempSet(refSet),index(1) {
    }

    template<class T>
    T& GTUSetBase<T>::GTUIterator::operator* () {
        update();
        for(int i = 1; i < index && currentNode->next != nullptr; currentNode = currentNode->next, i++);
        return *currentNode->data;
    }

    template<class T>
    void GTUSetBase<T>::GTUIterator::update () {
        headNode = currentNode = tempSet.testDataHead;
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator &GTUSetBase<T>::GTUIterator::operator++ () {
        if(index <= tempSet.size()) {
            index++;
        }
        return *this;
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator &GTUSetBase<T>::GTUIterator::operator-- () {
        if(index > 1)index--;
        return *this;
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator &GTUSetBase<T>::GTUIterator::operator= (const GTUIterator &other) {
        *this = other;
        return *this;
    }

    template<class T>
    bool GTUSetBase<T>::GTUIterator::operator== (const T &other) {
        return **this == other;
    }

    template<class T>
    shared_ptr<typename GTUSetBase<T>::DataNode> GTUSetBase<T>::GTUIterator::operator-> () {
        GTUSetBase<T> &tempSetVar = const_cast<GTUSetBase<T>& >(tempSet);
        **this; // Advances iterator to its current index position
        return currentNode;
    }

    template<class T>
    unsigned int GTUSetBase<T>::GTUIterator::getIndex () {
        return index;
    }

    template<class T>
    bool GTUSetBase<T>::GTUIterator::operator!= (const GTUSetBase::GTUIterator &other) {
        return !(*this==other);
    }

    template<class T>
    bool GTUSetBase<T>::GTUIterator::operator== (const GTUSetBase::GTUIterator &other) {
        return &tempSet == &other.tempSet && index == other.index;
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator &GTUSetBase<T>::GTUIterator::operator++ (int) {
        return ++*this;
    }

    template<class T>
    typename GTUSetBase<T>::GTUIterator &GTUSetBase<T>::GTUIterator::operator-- (int) {
        return --*this;
    }

    template<class T>
    GTUSetBase<T>::DataNode::DataNode (const T &element): data(make_shared<T>(element)) {} // will be usefull for insertion of elements
}