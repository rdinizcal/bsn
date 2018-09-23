#include <string>
#include <iostream>

using namespace std;

int main() {
    string s = "bpms-12.737219/bpmd-7.647789";
    string sys = s.substr(0,s.find('/'));
    string dia = s.substr(s.find('/')+1,s.length());
    cout << sys << ' ' << dia << endl;
    cout << sys.substr(sys.find('-')+1,sys.length()) << endl;
    

    return 0;
}
