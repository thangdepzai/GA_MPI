#include"Global.h"
Global::Global()
{
}
Global::~Global()
{
    delete instance;
}
Global* Global::instance = nullptr;
Global* Global::_getInstance()
{
    if(instance == nullptr)
    {
        instance= new Global();
    }
    return instance;
}
void Global::setSizePopulation(int sp)
{
    this->SIZE_POPULATION = sp;
}
void Global::setSizeGene(int sg)
{
    this->SIZE_GENE = sg;
}
void Global::setRMP(double rp)
{
    this->rmp = rp;
}
void Global::setRM(double _rm)
{
    this->rm = _rm;
}
