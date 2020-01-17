#pragma once
#ifndef CHROMOSOME_H
#define CHROMOSOME_H
class Chromosome{
    public:
        int*gene;
        int size_g;
        double fitness =0.0;
        int s;
        Chromosome();
        Chromosome(int* _gene);
        ~Chromosome();
        void Init();
        void SetFitness(double f);
        void printGene();
        void randomize (int *&arr, int n);

};
#endif
