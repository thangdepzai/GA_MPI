#pragma once
#ifndef POPULATION_H
#define POPULATION_H
#include<vector>
#include"Chromosome.h"
class Population{
    public:
        int size_p;
        std::vector<Chromosome*> pop;
        std::vector<Chromosome*> new_pop;
        Population();
        ~Population();
        void Init();
        std::vector<Chromosome*> Crossover(Chromosome * c1, Chromosome * c2);
        Chromosome* Mutation(Chromosome *c);
        void Reproduction();
        void Selection();
};
#endif
