#include"Population.h"
#include"Global.h"
#include<stdio.h>
#include <algorithm>
Population::Population()
{
    this->size_p = Global::_getInstance()->SIZE_POPULATION;
}

void Population::Init()
{

    for(int i=0; i<size_p; i++)
    {
        Chromosome *c = new Chromosome();
        c->Init();
        new_pop.push_back(c);
    }

}
Chromosome*  Population::Mutation(Chromosome *c)
{
    Chromosome *new_c = new Chromosome(c->gene);
    new_c->s =2;
    int N = Global::_getInstance()->SIZE_GENE;
    int i = rand()%(N) ;
    int j = rand()%(N);
    while(i==j)
    {
        j = rand()%(N);
    }
    int temp1 = new_c->gene[i];
    int temp2 = new_c->gene[j];
    new_c->gene[i] = temp2;
    new_c->gene[j] = temp1;
    return new_c;

}

std::vector<Chromosome*>  Population::Crossover(Chromosome *c1,Chromosome *c2 )
{
//    printf("--------------------------------\n");
//    c1->printGene();
//    c2->printGene();
    std::vector<Chromosome*> Child;
    Chromosome * o1 = new Chromosome(c1->gene);
    Chromosome * o2 = new Chromosome(c2->gene);
    Child.push_back(o1);
    Child.push_back(o2);
    Child[0]->s =1;
    Child[1]->s =1;
    int N = Global::_getInstance()->SIZE_GENE;
    int i = rand()%(N);
    int j = rand()%(N);
    while(i==j)
    {
        j = rand()%(N);
    }
    if(i>j)
    {
        int temp = i;
        i = j;
        j = temp;
    }
//    printf("SIZE %d Cross: %d %d\n",N, i, j);
    bool *bucket1 = new bool[N];
    bool *bucket2 = new bool[N];

    for(int k = 0; k<N; k++)
    {
        if(k>=i && k<= j){
            bucket1[c1->gene[k]] = true;
            bucket2[c2->gene[k]] = true;
        }else{
            bucket1[c1->gene[k]] = false;
            bucket2[c2->gene[k]] = false;
        }

    }
    int h1 = (j+1)%N;
    int h2 = (j+1)%N;
    for(int k = j+1; k<N; k++)
    {
        if(!bucket1[c2->gene[k]])
        {
            Child[0]->gene[h1] = c2->gene[k];
            h1= (h1+1)%N;
            bucket1[c2->gene[k]] = true;
        }
        if(!bucket2[c1->gene[k]])
        {
            Child[1]->gene[h2] = c1->gene[k];
            h2= (h2+1)%N;
            bucket2[c1->gene[k]] = true;
        }
    }
    for(int k = 0; k<=j; k++)
    {
        if(!bucket1[c2->gene[k]])
        {

            Child[0]->gene[h1] = c2->gene[k];
            h1= (h1+1)%N;
            bucket1[c2->gene[k]] = true;
        }
        if(!bucket2[c1->gene[k]])
        {
            Child[1]->gene[h2] = c1->gene[k];
            h2= (h2+1)%N;
            bucket2[c1->gene[k]] = true;
        }
    }
    //Child[0]->printGene();
   // Child[1]->printGene();
    delete [] bucket1;
    delete [] bucket2;
    bucket2 = NULL;
    bucket1 = NULL;
//    Child[0]->printGene();
//    Child[1]->printGene();
//    printf("---------------------------------------\n");
    return Child;

}
void Population::Reproduction()
{
    //printf("SIZE_P = %d and SIZE_NEW = %ld\n", size_p, new_pop.size());
    while(new_pop.size()< size_p)
    {
        int a = rand()%size_p;
        int b = rand()%size_p;
        while(a==b)
        {
            b = rand()%size_p;
        }
        double rd =  (rand()%10000)*1.0/ 10000;
        if( rd< Global::_getInstance()->rm)
        {
           // printf("Dot bien\n");
            new_pop.push_back(Mutation(pop[a]));
            new_pop.push_back(Mutation(pop[b]));
        }
        else if(rd <Global::_getInstance()->rmp)
        {
           // printf("Lai ghep\n");
            std::vector<Chromosome*> Child = Crossover(pop[a], pop[b]);
            new_pop.push_back(Child[0]);
            new_pop.push_back(Child[1]);
        }
        //printf("------------SIZE_NEW = %ld\n", new_pop.size());
    }
}

void Population::Selection()
{

    for(int i=0; i<new_pop.size(); i++)
    {
        pop.push_back(new_pop[i]);
    }
    new_pop.clear();
    sort(pop.begin(), pop.end(),[](Chromosome* const &a, Chromosome* const &b)
    {
        return a->fitness < b->fitness;
    });
    for(int i=Global::_getInstance()->SIZE_POPULATION; i<pop.size(); i++)
    {
        delete pop[i];
        pop[i] = NULL;
    }

    pop.resize(Global::_getInstance()->SIZE_POPULATION);
    // printf("%d %d\n", pop.size(), new_pop.size());
}
Population::~Population()
{
    for(int i=0; i<pop.size(); i++) delete pop[i];

}

