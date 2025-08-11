#include "linked_list/find_loop.hpp"

// Implement your find_loop logic here.

LinkedListAE::LinkedListAE(int value):m_value(value),m_next(nullptr){}

LinkedListAE * findLoop (LinkedListAE *head){
    LinkedListAE *slow=head;
    LinkedListAE *fast=head;
    while (fast!=nullptr && fast->m_next!=nullptr){
        slow=slow->m_next;
        fast=fast->m_next->m_next;
        if (slow==fast){
            slow=head;
            
            while (slow!=fast) {
                slow=slow->m_next;
                fast=fast->m_next;
                
            }
            return slow;
        }
    }
    return nullptr;
}

