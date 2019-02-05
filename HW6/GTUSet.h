//
// Created by cecen on 12/2/2017.
//

#ifndef HW6_GTUSET_H
#define HW6_GTUSET_H

#include <memory>
#include "GTUSetBase.h"


namespace containers{
    template <class T>
    class GTUSet: public GTUSetBase<T>{
    public:
        typedef typename GTUSetBase<T>::DataNode DataNode; // typedef required because of templated class
        typedef typename GTUSetBase<T>::GTUIterator GTUIterator;
        GTUSet(); // default constructor initializes shared pointers

        bool empty() const noexcept override{
            return *this->_size == 0;
        }
        bool isMember(const T& element) const noexcept override;
        unsigned int size() const noexcept override{
            return *this->_size;
        }
        unsigned int max_size() const noexcept override{
            return *this->_maxSize;
        }
        void insert(const T& element) throw(invalid_argument) override;
        void erase(const T& element) override;
        void clear() noexcept override;
        typename GTUSetBase<T>::GTUIterator find(const T& element) const override;
        unsigned int count(const T& val) const override;
        typename GTUSetBase<T>::GTUIterator begin() override;
        typename GTUSetBase<T>::GTUIterator end() override;

    private:
        void initClass(); // Initialization function
    };

    template <class T>
    shared_ptr<GTUSetBase<T> > setIntersection (const GTUSetBase<T>& first, const GTUSetBase<T>& second);
}

#endif //HW6_GTUSET_H
