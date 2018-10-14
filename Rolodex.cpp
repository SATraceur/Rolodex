
#include <iostream>
#include <unistd.h>

using namespace std;



template<class T>
class Rolodex {
public:

    typedef T* iterator;
    
    struct RolodexItem
    {
        T value_;
        RolodexItem* next_;
        RolodexItem* prev_;
    };
    
    /**
     * Creates a new empty Rolodex
     */
    Rolodex() 
    {
        sentinel = new RolodexItem;
        sentinel->next_ = sentinel;
        sentinel->prev_ = sentinel;
        current = sentinel;        
    }

    /**
     * Returns true if the Rolodex is positioned before the first card.
     */
    bool isBeforeFirst() 
    {
        return(current == sentinel);  // change as needed
    }

    /**
     * Returns true if the Rolodex is positioned after the last card.
     */
    bool isAfterLast() 
    {
        return(current == sentinel);  // change as needed
    }

    /**
     * Rotates the Rolodex one step forwards.
     */
    void rotateForward() 
    {
        current = current->next_;
        return;
    }

    /**
     * Rotates the Rolodex one step backwards.
     */
    void rotateBackward() 
    {        
        current = current->prev_;  
        return;
    }

    /**
     * Returns the value of the current card.
     */
    const T& currentValue() 
    {
        return(current->value_);      // replace both lines with real code!
    }

    /**
     * Inserts a new card after the current position and
     * positions the Rolodex at the newly inserted card.
     */
    void insertAfterCurrent(const T& value) 
    {
        current->next_->prev_ = new RolodexItem;
        current->next_->prev_->prev_ = current;
        current->next_->prev_->next_ = current->next_;
        current->next_ = current->next_->prev_;
        current = current->next_;
        current->value_ = value;
        return;
    }

    /**
     * Inserts a new card before the current position and
     * positions the Rolodex at the newly inserted card.
     */
    void insertBeforeCurrent(const T& value) 
    {
        current->prev_->next_ = new RolodexItem;
        current->prev_->next_->next_ = current;
        current->prev_->next_->prev_ = current->prev_;
        current->prev_ = current->prev_->next_;
        current = current->prev_;
        current->value_ = value;
        return;
    }

    void roloDelete()
    {
        RolodexItem* position;
        current->prev_->next_ = current->next_;
        current->next_->prev_ = current->prev_;
        if(current->next_ != sentinel)
        {
            position = current->next_;
        }
        else
        {
            position = current->prev_;
        }
        delete current;
        current = position;
        return;
    }
    
private:
    RolodexItem* sentinel;
    RolodexItem* current;
};