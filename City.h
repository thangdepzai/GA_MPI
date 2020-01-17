#pragma once
#ifndef CITY_H
#define CITY_H
class City{
    public:
        int id;
        double x;
        double y;
        City();
        ~City();
        City(int a, double b, double c);
        double distanceTo(City* c);
};

#endif // CITY_H
