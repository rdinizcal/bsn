#include "../communications/include/data_sender.hpp"
#include "../VitalStatistics/include/VitalStatistics.h"
#include <bits/stdc++.h>

using namespace std;

int main(){
    ofstream slog;
    slog.open("../../persistence/output/sensor_packages.txt");
    try{
        data_sender connection;
        for( ; ; ){          
            string data = generateData("baixo");
            slog << "0-" + data + ' ';
            connection.send("0-" + data + '*');

            data = generateData("baixo");
            slog << "1-" + data + ' ';
            connection.send("1-" + data + '*');

            data = generateData("baixo");
            slog << "2-" + data + ' ';
            connection.send("2-" + data + '*');
            
            slog << endl;

            sleep(1);
        }        
    }
    catch (const Socket::ClosedConnection &x) {
        cout << "Connection closed" << endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    slog.close();
}