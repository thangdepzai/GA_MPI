#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H
class Global{
    public:
        ~Global();
        static Global* _getInstance();
        void setSizePopulation(int sp);
        void setSizeGene(int sg);
        void setRMP(double rmp);
        void setRM(double rm);
        int SIZE_POPULATION = 2000;
        int CONDITION = 500;
        int SIZE_GENE=1 ;
        double rmp=0.9;
        double rm=0.1;
        double rm0=0.1;
        int seed;
    private:
        static Global* instance;
        Global();

};
#endif // GLOBAL_H
