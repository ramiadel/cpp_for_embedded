#ifndef GPIO_hpp
#define GPIO_hpp
#include <stdio.h>

class GPIO {
    
    private :
    
    public :
    GPIO(){}
    virtual void configurePin (char portName , int pinNumber );
    virtual void writePin (char portName ,int pinNumber , int value );
    virtual int readPin (char portName ,int pinNumber);
    virtual void togglePint (char portName ,int pinNumber);
    
};

#endif /* GPIO_hpp */
