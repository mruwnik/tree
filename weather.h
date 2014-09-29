#ifndef WEATHER_H
#define WEATHER_H
#include <QObject>
#include <QString>

class Weather: public QObject{
    Q_OBJECT
private:
    static double temperature;  // the current temp in C
    static double deltaTemp;    // the difference between the annual min and max temp
    static double avgTemp;      // the average temp
    static const double baseTempStep = 0.1;; // the min amout what the temp can change

    static int time;            // the current amount of hours since the start of the year
    static int year;            // the current year of growth
    static int timeStep;        // the amout of time simulated by 1 tick
public:
    Weather();

    static double getTemp(){return temperature;};
    static double getDeltaTemp(){return deltaTemp;};
    static double getAvgTemp(){return avgTemp;};
    static double getBaseTempStep(){return baseTempStep;};

    static int getTime(){return time;};
    static int getYear(){return year;};
    static int getTimeStep(){return timeStep;};

    static QString getFormatedTime();
    static QString getFormatedTimeStep();

public slots:
    static void setTemp(double temp){temperature = temp;};
    static void setDeltaTemp(double temp){deltaTemp = temp;};
    static void setAvgTemp(double temp){avgTemp = temp;};

    static void setTime(int newTime){time = newTime;};
    static void setYear(int newTime){year = newTime;};
    static void setTimeStep(int newTime){timeStep = newTime;};
};

#endif // WEATHER_H
