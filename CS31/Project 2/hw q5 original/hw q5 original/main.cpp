//
//  main.cpp
//  hw q5 original
//
//  Created by Yuen Yee Wan on 10/15/15.
//  Copyright © 2015 yywan. All rights reserved.
//

#include <iostream>
using namespace std;

int main() {
    
    int codeSection;
    cout << "What is the Code Section? ";
    cin >> codeSection;
    
   	if (codeSection == 281)
        cout << "bigamy";
    else if (codeSection == 321  ||  codeSection == 322)
        cout << "selling illegal lottery tickets";
    else if (codeSection == 383)
        cout << "selling rancid butter";
    else if (codeSection == 598)
        cout << "wounding a bird in a public cemetery";
    else
        cout << "some other crime";
}