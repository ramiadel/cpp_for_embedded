//
//  Generic_Register.h
//  assignement_3_Register
//
//  Created by Rami Adel on 08.03.22.
//  Copyright © 2022 Rami Adel. All rights reserved.
//


/*
 *FILE GUARD
 */
#pragma once

/*ENUM FOR THE STATUS OF THE REGISTERS' PERMISSION
 * Read only
 * Write only
 * Read and Write
 * Reserved
 
 *NOTE: YOU CAN CONFIGURE THE PERMISSION OF ANY BIT JUST **ONE TIME**
 *AND IT IS THE FIRST TIME
 */
typedef enum{
    readOnly,
    writeOnly,
    readAndWrite,
    reserved
}REGISTER_PERMISSION;



template <typename Type>
class genericRegister {
    /*
     *PRIVATE SECTION :
     */
    private :
        /*
            * myRegister HOLDS THE ADDRESS OF THE REGISTER
            * myRegister CAN BE ASSIGNED THROUGH THE CONSTRUCTOR
            */
        Type * myRegister;
        /*
            * permission HOLDS THE PERMISSION OF myRegister
            */
        Type permissionMask;
        REGISTER_PERMISSION permission;
        
        /*
            * THESE STATUS ARRAYS FOR HOLDING THE STATUS OF EACH BIT OF myRegister
            * IF IT IS 1 ----> IT INDICATES THE ARRAY'S NAME
            * IF IT IS 0 ----> IT INDICATES THE OPPOSITE OF THE ARRAY'S NAME
            */
        bool* readOnlyeArr   =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
        bool* readOnlyArr    =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
        bool* writeOnlyArr   =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
        bool* readAndWriteArr =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
        bool* reservedArr    =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
        bool* configuredArr   =  (bool*)calloc(sizeof(Type)*8,sizeof(bool));
    /*
     *PUBLIC SECTION :
     */
    public :
        /*
            *DEFAULT CONSTRUCTOR
            */
        genericRegister();
    
        /*
            *PARAMATARIZED CONSTRUCTORS
            */
        genericRegister(Type * registerAddress);
        genericRegister(Type * registerAddress , REGISTER_PERMISSION permission);
    
        /*
            *DEFAULT DESTRUCTOR
            */
        ~genericRegister();
    
        /*
            *SET myRegister TO A VALUE
            *NOTE : THE BITS THAT CAN BE WRITTEN **ONLY**
            */
        bool setRegisterValue(Type   registerValue );
    
        /*
            *
            */
        bool getRegisterValue(Type & registerValue );
        bool setBit(std::uint8_t   pinNumber);
        bool clearBit(std::uint8_t   pinNumber);
        bool getBit(std::uint8_t pinNumber , bool & bitValue);
    
        void setRegisterPermission  (REGISTER_PERMISSION permission);
    
        bool setReadOnlyArr(std::uint8_t pinNumber , bool bit);
        bool setWriteOnlyArr(std::uint8_t pinNumber , bool bit);
        bool setReadAndWriteArr(std::uint8_t pinNumber, bool bit);
        bool setReservedArr(std::uint8_t pinNumber , bool bit);
        //bool setConfigureddArr(std::uint8_t pinNumber , bool bit);
    
        bool getReadOnlyArr(std::uint8_t pinNumber);
        bool getWriteOnlyArr(std::uint8_t pinNumber);
        bool getReadAndWriteArr(std::uint8_t pinNumber);
        bool getReservedArr(std::uint8_t pinNumber);
        //bool getConfigureddArr(std::uint8_t pinNumber , bool bit);
    
        Type getMask();
};

/*
 *A HELPER METHOD TO EXTRACT THE MASK FROM THE PERMISSION ARRAYS SO THAT
 *WE CAN READ / WRITE THE ALLOWED BITS FROM **myRegiter**
 *mask SET BITS ARE THE BITS CORROSPONDING TO THE BITS THAT WE CAN **WRITE ON**
 */
template <typename Type>
Type getMask(){
    Type mask = 0;
    for(Type i = 0 ; i < sizeof(Type)*8 ; i++){
        mask |= ( getReadAndWriteArr(i)  << i ) | ( getWriteOnlyArr(i) << i ) ;
    }
    return mask ;
}

/*
 *THE DEFAULT CONSTRUCTOR
 *SET THE REGISTER TO NULL OF NONE IS PASSED
 *DEFAULT PERMISSION IS ReadAndWrie
 */
template <typename Type>
genericRegister <Type>::genericRegister(){
    myRegister = NULL;
    permission = readAndWrite ;
    memset(readAndWriteArr, 1 , sizeof(Type));
    memset(configuredArr, 1 , sizeof(Type));
}

/*
 *THE 1st PARAMATERIZED CONSTRUCTOR
 *SET THE REGISTER TO AN ADDRESS
 *DEFAULT PERMISSION IS ReadAndWrie
 */
template <typename Type>
genericRegister <Type>::genericRegister(Type * registerAddress ){
    
    myRegister = registerAddress ;
    this -> permission = readAndWrite ;
    memset(readAndWriteArr, 1 , sizeof(Type));
    memset(configuredArr, 1 , sizeof(Type));
    
}

/*
 *THE 2nd PARAMATERIZED CONSTRUCTOR
 *SET THE REGISTER TO AN ADDRESS
 *SET THE PERMISSION TO A SPECIFIED PERMISSION
 *INITIALIZE THE PERMISSION ARRAYS TO ZERO
 */
template <typename Type>
genericRegister <Type>::genericRegister(Type * registerAddress , REGISTER_PERMISSION permission ){
    
    myRegister = registerAddress ;
    this -> permission = permission ;
    
    
}

/*
 *THE DESTRUCTOR
 *FREE THE DYNAMICALLY ALLOCATED BYTES IN THE HEAP SECTION
 */
template <typename Type>
genericRegister <Type>::~genericRegister(){
    free (readOnlyeArr);
    free (writeOnlyArr);
    free (readAndWriteArr);
    free (reservedArr);
    free (configuredArr);
}

/*
 *myRegister IS LOCKED [REGISTER IS EITHER WRITEONLY OR READANDWRITE]?
 *IF LOCKED : RETURN ERROR
 *ELSE
 *CHECK FOR THE BITS IN myRegister THAT YOU'RE ALLOWED
 *TO **WRITE ON** THEN MASK THE OTHER BITS
 *WRITE ON THESE BITS **ONLY**
 */
template <typename Type>
bool genericRegister <Type>::setRegisterValue(Type   registerValue ){
    
    Type mask ;
    if(permission == writeOnly || permission == readAndWrite ){
        mask = getMask();
        *myRegister |= (registerValue & mask);
        return 1 ;
    }
    else{
        return 0 ;
    }
    
}

/*
 *myRegister IS LOCKED [REGISTER IS EITHER READONLY OR READANDWRITE]?
 *IF LOCKED : RETURN ERROR
 *ELSE
 *CHECK FOR THE BITS IN myRegister THAT YOU'RE ALLOWED
 *TO **READ FROM**
 *IF ALL BITS ARE PERMITTED TO READ THEN RETURN THE VALUE TO THE registerValue
 *ELSE
 *RETURN ERROR
 */
template <typename Type>
bool genericRegister <Type>::getRegisterValue(Type & registerValue ){

    if(permission == readAndWrite || permission == readOnly){
        if(this -> permissionMask == ~((Type)0)){
            registerValue = *myRegister;
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
    
}

/*
 *THE BIT IS LOCKED [REGISTER IS EITHER WRITEONLY OR READANDWRITE]?
 *RETURN ERROR
 *ELSE
 *SET THE pinNumber in myRegister
 */
template <typename Type>
bool genericRegister <Type>::setBit(std::uint8_t   pinNumber){
    
    if((getReadAndWriteArr(pinNumber) || getWriteOnlyArr(pinNumber))&& (getReadAndWriteArr(pinNumber) || getWriteOnlyArr(pinNumber))){
        *myRegister |= (1<<pinNumber);
        return 1;
    }
    else{
        return 0;
    }
    
}

/*
*THE BIT IS LOCKED [REGISTER IS EITHER WRITEONLY OR READANDWRITE]?
*RETURN ERROR
*ELSE
*CLEAR THE pinNumber in myRegister
*/
template <typename Type>
bool genericRegister <Type>::clearBit(std::uint8_t  pinNumber){
    
    if( (getReadAndWriteArr(pinNumber) || getWriteOnlyArr(pinNumber)) && ((getReadAndWriteArr(pinNumber) || getWriteOnlyArr(pinNumber)) )){
        myRegister &= ~(1<<pinNumber);
        return 1;
    }
    else{
        return 0;
    }
    
}

/*
*THE BIT IS LOCKED [REGISTER IS EITHER READONLY OR READANDWRITE]?
*RETURN ERROR
*ELSE
*GET THE pinNumber in myRegister
*/
template <typename Type>
bool genericRegister <Type>::getBit(std::uint8_t pinNumber , bool & bitValue){
    
    if( (getReadOnlyArr(pinNumber) || getReadAndWriteArr(pinNumber)) && ((getReadAndWriteArr(pinNumber) || getReadOnlyArr(pinNumber)) ) ){
        bitValue = (myRegister>>pinNumber) & 1 ;
        return 1;
    }
    else{
        return 0;
    }
}

/*
*SETTER FUNCTION
*TO SET THE PERMISSION OFF myRegister BUT BASED ON THE MASK
*WITH THE PERMISSION ARRAYS
*/
template <typename Type>
void genericRegister <Type>::setRegisterPermission(REGISTER_PERMISSION permission){
    Type permissionMask = 0 ;
    if(permission == writeOnly || permission == readAndWrite){
        permissionMask = ~(Type(0));
    }
    Type mask;
    mask = getMask();
    this->permissionMask |= (permissionMask & mask) ;
}

/*
*SETTER FUNCTION
*TO SET A BIT IN myRegister TO BE ReadOnly IF IT IS NOT SET TO
*ANYTHING ELSE
*/
template <typename Type>
bool genericRegister <Type>::setReadOnlyArr(std::uint8_t pinNumber ,bool bit){
    if( (this->permission == readAndWrite||this->permission == writeOnly|| (reservedArr[pinNumber]^1 && readOnlyArr[pinNumber]^1 && writeOnlyArr[pinNumber]^1 &&readAndWrite^1))&&(!configuredArr[pinNumber])){
        readOnlyArr[pinNumber] = bit;
        configuredArr[pinNumber] = bit;
        return 1;
    }
    else{
        return 0;
    }
}

/*
*SETTER FUNCTION
*TO SET A BIT IN myRegister TO BE WriteOnly IF IT IS NOT SET TO
*ANYTHING ELSE
*/
template <typename Type>
bool genericRegister <Type>:: setWriteOnlyArr(std::uint8_t pinNumber ,bool bit){
    if((this->permission == readAndWrite||this->permission == writeOnly|| (reservedArr[pinNumber]^1 && readOnlyArr[pinNumber]^1 && writeOnlyArr[pinNumber]^1 &&readAndWrite^1 ))&& (!configuredArr[pinNumber])){
        writeOnlyArr [pinNumber] = bit;
        configuredArr[pinNumber] = bit;
        return 1;
    }
    else{
        return 0;
    }
}

/*
*SETTER FUNCTION
*TO SET A BIT IN myRegister TO BE ReadAndWrite IF IT IS NOT SET TO
*ANYTHING ELSE
*/
template <typename Type>
bool genericRegister <Type>::setReadAndWriteArr(std::uint8_t pinNumber ,bool bit){
    if(((this->permission == readAndWrite) || (reservedArr[pinNumber]^1 && readOnlyArr[pinNumber]^1 && writeOnlyArr[pinNumber]^1 &&readAndWrite^1 )) && (!configuredArr[pinNumber])){
        readAndWriteArr [pinNumber] = bit;
        configuredArr[pinNumber] = bit;
        return 1;
    }
    else{
        return 0;
    }
}

/*
*SETTER FUNCTION
*TO SET A BIT IN myRegister TO BE RESERVED IF IT IS NOT SET TO
*ANYTHING ELSE
*/
template <typename Type>
bool genericRegister <Type>::setReservedArr(std::uint8_t pinNumber ,bool bit){
    if(reservedArr[pinNumber]^1 && readOnlyArr[pinNumber]^1 && writeOnlyArr[pinNumber]^1 &&readAndWrite^1 &&!configuredArr[pinNumber]){
        reservedArr[pinNumber] = bit;
        configuredArr[pinNumber] = bit;
        return 1;
    }
    else{
        return 0;
    }
}

/*
*GETTER FUNCTION
*TO CHECK WHETHER A BIT IN myRegister IS ReadOnly OR NOT
*/
template <typename Type>
bool genericRegister <Type>::getReadOnlyArr(std::uint8_t pinNumber ){
    return readOnlyArr[pinNumber];
}

/*
*GETTER FUNCTION
*TO CHECK WHETHER A BIT IN myRegister IS WrtieOnly OR NOT
*/
template <typename Type>
bool genericRegister <Type>::getWriteOnlyArr(std::uint8_t pinNumber){
    return writeOnlyArr[pinNumber];
}

/*
*GETTER FUNCTION
*TO CHECK WHETHER A BIT IN myRegister IS ReadAndWrite OR NOT
*/
template <typename Type>
bool genericRegister <Type>::getReadAndWriteArr(std::uint8_t pinNumber ){
    return readAndWriteArr[pinNumber];
}

/*
 *GETTER FUNCTION
 *TO CHECK WHETHER A BIT IN myRegister IS RESERVED OR NOT
 */
template <typename Type>
bool genericRegister <Type>::getReservedArr(std::uint8_t pinNumber){
    return reservedArr[pinNumber];
}
