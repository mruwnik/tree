#include "weather.h"

double Weather::temperature = -10;  // the current temp in C
double Weather::deltaTemp = 500;    // the difference between the annual min and max temp
double Weather::avgTemp = 150;      // the average temp

int Weather::time = 1;            // the current amount of hours since the start of the year
int Weather::year = 1;            // the current year of growth
int Weather::timeStep = 14;


Weather::Weather(){
}

QString formatTime(int time){
    QString timeStr;
    int days = time/24;
    if(days > 0){
        timeStr = (days > 9 ? "" : " ") + QString::number(days) + "d";
    }
    int hours = time % 24;
    timeStr += (hours > 9 ? " " : "  ") + QString::number(hours) + "h";
    return timeStr;
}

QString Weather::getFormatedTime(){
    return formatTime(time);
}

QString Weather::getFormatedTimeStep(){
    return formatTime(timeStep);
}


