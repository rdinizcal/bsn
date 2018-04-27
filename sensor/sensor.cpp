#include "../communications/include/data_sender.hpp"
#include "../VitalStatistics/include/VitalStatistics.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    ofstream log("../persistence/output/sensor_packages.txt");
    try{
        data_sender connection;
        for( ; ; ){          
            string data = generateData("baixo");
            log << "0-" + data;
            connection.send("0-" + data + '*');

            data = generateData("baixo");
            log << "1-" + data;
            connection.send("1-" + data + '*');

            data = generateData("baixo");
            log << "2-" + data;
            connection.send("2-" + data + '*');

            sleep(1);
        }        
    }
    catch (const Socket::ClosedConnection &x) {
        cout << "Connection closed" << endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}