#include <iostream>
#include <string>

using namespace std;

string addBinary(string a, string b)
{
    if(a.length() == 0 || (a.length() == 1 && a[0] == '0'))
        return b;
    if(b.length() == 0 || (b.length() == 1 && b[0] == '0'))
        return a;

    string::size_type a_length = a.length();
    string::size_type b_length = b.length();
    string result = "";

    bool carry = false;
    size_t i = 1;
    while(true)
    {
        int temp = 0;
        if(carry)
            ++temp;
        for(; i <= a_length;)
        {
            if(a.at(a_length - i) == '1')
                ++temp;
            break;
        }
        for(; i <= b_length;)
        {
            if(b.at(b_length - i) == '1')
                ++temp;
            break;
        }
        if(temp == 3)
        {
            carry = true;
            result += '1';
        }
        else if(temp == 2)
        {
            carry = true;
            result += '0';
        }
        else if(temp == 1)
        {
            carry = false;
            result += '1';
        }
        else
        {
            result += '0';
        }
        if(i >= a_length && i >= b_length && (!carry))
            break;
        ++i;
    }
    
    return string(result.rbegin(), result.rend());
}

int main()
{
    string s1("1010");
    string s2("1011");

    cout << addBinary(s1, s2) << endl;
    s1 = "111111111111";
    s2 = "1";
    cout << addBinary(s1, s2) << endl;
}

