#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream in, out;
    in.open("bla.txt", ios::in);
    out.open("bad.txt", ios::out);
    
    string s;
    
    while (in >> s)
    {
        out << "/root/3D-Drawer/HAAR/bad_not_my/" << s << "\n";
    } 
}
