#include"Chromosome.h"
#include"Global.h"
#include<stdio.h>
#include <algorithm>
Chromosome::Chromosome(){
    size_g = Global::_getInstance()->SIZE_GENE;
    s =0;

}
Chromosome::~Chromosome(){
    delete[] gene;
    gene =  NULL;
}


Chromosome::Chromosome(int* _gene){
    size_g = Global::_getInstance()->SIZE_GENE;
    this->gene = new int[size_g];

    for(int i=0;i<size_g; i++) this->gene[i] = _gene[i];
}

void Chromosome::Init(){
    this->s =0;
    gene = new int[size_g];
    for(int i=0;i<size_g; i++) gene[i] = i;
    randomize(gene, size_g);
}

void Chromosome::SetFitness(double f){
    fitness = f;
}
void Chromosome::printGene(){
    printf("%d  , ",this->s);
    for(int i=0;i<size_g; i++){
        printf("%d ", gene[i]);
    }
    printf("\n");
}
void Chromosome::randomize(int*& arr, int n){

   // srand (0);
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
       int tem = arr[i];
       arr[i] = arr[j];
       arr[j] = tem;
    }
}

