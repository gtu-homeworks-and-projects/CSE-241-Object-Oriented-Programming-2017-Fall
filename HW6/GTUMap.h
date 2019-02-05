//
// Created by cecen on 12/13/2017.
//

#ifndef HW6_GTUMAP_H
#define HW6_GTUMAP_H

#include "GTUSet.h"

namespace containers{
    template <class K, class V>
    class GTUMap: public GTUSet<std::pair<K,V> > {
    public:
        typedef typename GTUSetBase<std::pair<K,V> >::DataNode DataNode;
        GTUMap();

        V& operator[](const K& k);
    };


}

#endif //HW6_GTUMAP_H
