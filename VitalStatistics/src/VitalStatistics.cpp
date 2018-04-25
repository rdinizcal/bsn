#include "VitalStatistics.h"

using namespace std;

string statusAnalysis(string categorized_data, string actual_status, deque<string> &data_queue) {

    string new_status;
    int l=0, m=0, h=0;

    if(data_queue.size()>=5){
        data_queue.pop_front();
    }

    data_queue.push_back(categorized_data);

    for(uint32_t i = 0; i < data_queue.size(); i++) {
        if(data_queue[i]=="baixo"){
            l++;
        } else if(data_queue[i]=="moderado") {
            m++;
        } else if (data_queue[i]=="alto"){
            h++;
        }
    }

    if(l>=3){
        new_status = "baixo";
    } else if (m>=3) {
        new_status = "moderado";
    } else if (h>=3) {
        new_status = "alto";
    } else {
        new_status = actual_status;
    }

    return new_status;
}

string generateData(string actual_status){

    string category;
    int p = (rand() % 100) + 1;

    if(actual_status == "baixo"){
        if(1 <= p && p <= 5) {
            category = "alto";
        } else if (5 < p && p <= 30) {
            category = "moderado";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "moderado") {
        if(1 <= p && p <= 15) {
            category = "alto";
        } else if (15 < p && p <= 30) {
            category = "baixo";
        } else {
            category = actual_status;
        }
    } else if(actual_status == "alto"){
        if(1 <= p && p <= 5) {
            category = "baixo";
        } else if (5 < p && p <= 30) {
            category = "moderado";
        } else {
            category = actual_status;
        }
    } else {
        category = actual_status;
    }

    return category;
}

bool controllerFSM(int t, string status){
    bool exe = false;
    
    if(status == "baixo"){
        exe = (t>=10)?true:false;
    } else if (status == "moderado") {
        exe = (t>=5)?true:false;
    } else if (status == "alto") {
        exe = (t>=1)?true:false;
    } 

    return exe;
}

string calculateHealthStatus(map<uint32_t,string> buffer ){
    double hr = 0;
    for(uint32_t i = 0; i < buffer.size(); i++){

        if (buffer[i] == "baixo") {
            hr += 0.1;
        } else if (buffer[i] == "moderado") {
            hr += 1;
        } else if (buffer[i] == "alto") {
            hr += 5;
        } 
    }
    
    return (hr<=0)?"unknown":(hr<1)?"bom":(hr<5)?"medio":(hr<20)?"ruim":"unknown";
}