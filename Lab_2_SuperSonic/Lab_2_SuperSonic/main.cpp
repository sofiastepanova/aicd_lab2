#include "linked_list.h"




int main() {

    unsigned seed = 20;
    LinkedList<int> lst2(5, 2, 10, seed);
    cout << lst2 << endl;
    cout << lst2[0] << endl;
    LinkedList<int> lst1;
    lst1.push_tail(1);
    lst1.push_tail(2);
    lst1.push_tail(3);
    lst1.push_tail(4);
    cout << lst1 << endl;
    reverse(lst1);

    cout << lst1 << endl;

}