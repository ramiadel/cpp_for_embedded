//
//  main.cpp
//  assignement_3_Register
//
//  Created by Rami Adel on 08.03.22.
//  Copyright © 2022 Rami Adel. All rights reserved.
//

#include <iostream>
#include "Generic_Register.h"

int main() {
    std::uint32_t regist = 0xff;
    //genericRegister <std::uint32_t> reg ();
    genericRegister <std::uint32_t> reg1 (&regist, readAndWrite);
    /*
     ...
     ...
     ...
     PUT YOUR TEST CASES HERE ..
     */
   
    return 0;
}
