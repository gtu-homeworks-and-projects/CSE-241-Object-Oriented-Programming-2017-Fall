#include <iostream>
#include <memory>
#include <set>
#include "GTUMap.h"
#include "GTUMap.cpp"

using namespace std;
int main () {

    containers::GTUSet<int> a;
    containers::GTUSet<int> c;
    containers::GTUMap<char,int> b;

    cout << "First ones:" << endl;
    try { // try catch test
        a.insert(5);
        a.insert(5);
    }
    catch(invalid_argument& exception) {
        cerr << exception.what() << endl;
    }
    a.insert(4);
    a.insert(2);
    a.clear();
    a.insert(7);
    a.insert(5);
    a.erase(7);
    cout << "Count of element '5' in a: " << a.count(5) << endl; // count function test
    cout << "Max size of a: " << a.max_size() << endl; // max_size function test
    for(auto p :a) cout << p << endl; // Range based for loop test contains begin and end functions

    cout << "Second ones:" << endl;
    c.insert(10);
    c.insert(12);
    c.insert(13);
    c.insert(11);
    c.insert(14);
    for(auto p :c) cout << p << endl;

    cout << "Third ones:" << endl; // Third ones are Map tests
    b.insert(std::make_pair('a',2));
    b.insert(std::make_pair('b',1));
    b.insert(std::make_pair('c',3));
    b.insert(std::make_pair('d',4));
    b.insert(std::make_pair('e',5));
    cout << "c'th element in pair is: " << b['c'] << endl;
    for(auto p :b) cout << p.second << endl;

    cout << "Intersection of first and second sets:" << endl;
    shared_ptr<containers::GTUSetBase<int> > g = containers::setIntersection(a,c); // Intersection test
    for(auto p : *g) cout << p << endl;


    return 0;
}