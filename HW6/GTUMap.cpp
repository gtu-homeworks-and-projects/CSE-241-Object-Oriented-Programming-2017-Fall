//
// Created by cecen on 12/13/2017.
//

#include "GTUMap.h"
#include "GTUSet.cpp"

namespace containers{
    template<class K, class V>
    GTUMap<K, V>::GTUMap(): GTUSet<std::pair<K,V> >::GTUSet () {}

    template<class K, class V>
    V &GTUMap<K, V>::operator[](const K& k) {
        shared_ptr<DataNode> currentData = this->testDataHead;
        for(;currentData->data->first != k && currentData->next != nullptr;
             currentData = currentData->next);
        return currentData->data->second;
        // Returns matching k's value, if its not present returns last element
    }
}