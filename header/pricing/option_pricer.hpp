#pragma once

class OptionPricer{
public:
    // I need a virtual destructor in a base class if i ever intend to delete a derived class object via a base class pointer.
    virtual ~OptionPricer()=default;
    
    virtual double price() const=0;
};
