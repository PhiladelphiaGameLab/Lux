#include <iostream>
#include "MD5.h"
 
using std::cout; using std::endl;
 
int main(int argc, char *argv[])
{
    cout << "md5 of 'Lux': " << md5("Lux") << endl;   //just use the md5(const string str) function is enough for Lux project
    return 0;
}