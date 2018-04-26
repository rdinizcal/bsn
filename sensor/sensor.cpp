#include "../communications/include/data_sender.hpp"
#include <bits/stdc++.h>

using namespace std;

int main(){
    try{
        data_sender connection;
        connection.send("Alooou\n");
    }
    catch (const Socket::ClosedConnection &x) {
        cout << "Connection closed" << endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}