#include"City.h"
#include<math.h>
City::City(){
}
City::City(int a, double b, double c){
    this->id = a;
    this->x = b;
    this->y = c;
}
City::~City(){
}
double City::distanceTo(City* c){
    double dx = c->x - this->x;
    double dy = c->y - this->y;
    return sqrt(dx*dx +dy*dy);
}
