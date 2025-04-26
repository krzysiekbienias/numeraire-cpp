
#include "linked_list/single_linked_list.hpp"
// Implement your single_linked_list logic here.

//Constructor
LinkedList::LinkedList():head(nullptr){}

//Desctructor

LinkedList::~LinkedList(){
    Node* current =head;
    while (current!=nullptr){
        Node * nextNode=current->next;
        delete current;
        current=nextNode;
    }
}


int LinkedList::size()const {
    int count =0;
    Node * current=head;
    while (current!=nullptr) {
        ++count;
        current=current->next;
    }
    return count;
}


bool LinkedList::isEmpty()const{
    return head==nullptr;
}

void LinkedList::insertAtEnd(int value){
    Node* newNode = new Node(value);
    if(head==nullptr){
        head=newNode;
        return;
    }
    // Traverse till end of ll
    Node* current =head;
    while (current->next!=nullptr) {
        current=current->next;
    }
    current->next=newNode;
    
}

int LinkedList::getMiddle() const{
    if (!head){
        throw std::runtime_error("List is empty");
    }
    
    Node * slow=head;
    Node * fast=head;
    while (fast && fast->next){
        slow=slow->next;
        fast=fast->next->next;
    }
    return slow->value;
}


bool LinkedList::search(int value)const{
    Node * current = head;
    while(current!=nullptr){
        if (current->value==value){
            return true;
        }
        current=current->next;
        
    }
    return false;
    }


void LinkedList::remove_tail(){
    if (head ==nullptr or head->next==nullptr){
        head=nullptr;
        return ;
    }
    Node * current =head;
    while(current->next!=nullptr && current->next->next !=nullptr){
        current=current->next;
        
    }
    current->next=nullptr;
}
