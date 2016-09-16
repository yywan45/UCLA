//
//  main.cpp
//  hw q5
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

    
    switch (codeSection) {
        case 281:
            cout << "bigamy";
            break;
        case 321:
        case 322:
            cout << "selling illegal lottery tickets";
            break;
        case 383:
            cout << "selling rancid butter";
            break;
        case 598:
            cout << "wounding a bird in a public cemetery";
            break;
        default:
            cout << "some other crime";
            break;
    }
}
