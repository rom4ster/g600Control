// DLL1Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../dll1/dllmain.h"

int main()
{
    G600Api *  g600 = new G600Api();
    
    g600->load();
    auto out = g600->initMemRef();
    g600->read(*out);
    std::cout << *out << "\n";
    G600Api::inj_struct s;
    char str[7] = "ff0000";
    const char * eff = "01";
    const char* dur = "02";
    const char* dpi = "6e013d6f8a97";
    s.code = G600Api::Codes::COLOR;
    s.len = 6;
    s.value = str;
    g600->inject(*out, &s);
    s.code = G600Api::Codes::DPI;
    s.len = 12;
    s.value = (char *) dpi;
    g600->inject(*out, &s);
    std::cout << *out << "\n";
   auto succ = g600->write(*out);
    delete g600;
    
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
