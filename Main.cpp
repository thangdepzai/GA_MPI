#include<stdio.h>
#include"Population.h"
#include"Global.h"
#include<algorithm>
#include <mpi.h>
#include <fstream>
#include"City.h"
#include<time.h>
using namespace std;
City** city;
double toado[60000];
int  *num_fitness_in_task;
void delay(int second){
    int mini_seconds = second*1000;
    clock_t start_time = clock();
    while(clock()< start_time + mini_seconds);
}

double CalFitness(int *route, int n)
{
    double sum =0.0;
    for(int i=0; i<n-1; i++)
    {
        sum +=city[route[i]]->distanceTo(city[route[i+1]]);
    }
    sum+= city[route[n-1]]->distanceTo(city[route[0]]);
    delay(1);
    return sum;
}
int main(int argc, char* argv[])
{
    int SIZE_POPULATION =  Global::_getInstance()->SIZE_POPULATION;
    int SIZE_GENE = Global::_getInstance()->SIZE_GENE;
    int numtasks, RANK, source;
    int tag_send = 0;
    int tag_return = 1;
    int max_num_fitness =0;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &RANK);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    double computional_time =0;
    double communicate_time =0;
    double sum_computional_time =0;
    double _s = MPI_Wtime();
    double _s1 = MPI_Wtime();
    source = 0;
    // chia trung binh so lan tinh fitness ve cac slave
    int num_fitness_avg = SIZE_POPULATION/numtasks;
    num_fitness_in_task = new int[numtasks];
    num_fitness_in_task[0] = num_fitness_avg;
    for(int id=1;id<numtasks;id++){
        int _s = num_fitness_avg*id;
        int _e = (id+1)*num_fitness_avg-1;
            if(SIZE_POPULATION - _e < num_fitness_avg)
            {
                _e = SIZE_POPULATION -1;
            }
            num_fitness_in_task[id] = _e -_s +1;
            max_num_fitness = max(num_fitness_in_task[id], max_num_fitness);
    }
    computional_time += MPI_Wtime() - _s1;


    if(RANK ==source)
    {
        _s = MPI_Wtime();
        ifstream inStream;
        inStream.open("data2.txt");
        if (inStream.fail())
        {
            printf("Can't open input file!\n");
            exit(1);
        }

        inStream>>SIZE_GENE;
        Global::_getInstance()->SIZE_GENE = SIZE_GENE;

        city = new City*[SIZE_GENE];
        int i=0;
        int k =0;
        while (i< SIZE_GENE )
        {
            int a;
            double b,c;
            inStream >> a >> b>>c;
            city[i] = new City(a,b,c);

            toado[k] = b;
            k++;
            toado[k] = c;
            k++;
            i++;
        }

        inStream.close();
        computional_time += MPI_Wtime() - _s;

        _s = MPI_Wtime();
        MPI_Bcast(&SIZE_GENE, 1, MPI_INT, source,MPI_COMM_WORLD );
        for(int i=1; i< numtasks; i++){
            MPI_Send(&toado, SIZE_GENE*2, MPI_DOUBLE, i,tag_send, MPI_COMM_WORLD);
        }
        communicate_time +=MPI_Wtime() - _s;

    }else{
        _s = MPI_Wtime();
        MPI_Bcast(&SIZE_GENE, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Recv(&toado, SIZE_GENE*2, MPI_DOUBLE, source,tag_send, MPI_COMM_WORLD, &status);
        int dem =0;
        Global::_getInstance()->SIZE_GENE = SIZE_GENE;
        city = new City*[SIZE_GENE];
        for(int i=0;i<SIZE_GENE*2 -1; i+=2){
            city[dem] = new City(dem+1, toado[i], toado[i+1]);
            dem++;

        }
        communicate_time +=MPI_Wtime() - _s;
    }

    if(RANK==source){
        double _s = MPI_Wtime();
        int rate =0;
        int T= 1;
        Chromosome * best= NULL;
        Population *p = new Population();
        p->Init();
        computional_time += MPI_Wtime() - _s;

        while(true){
        _s = MPI_Wtime();
            int *buff = NULL;
            for(int id=1; id<numtasks; id++){

                buff = new int[num_fitness_in_task[id]*SIZE_GENE];
                int _s = num_fitness_avg*id;
                int dem =0;
                for(int k = _s; k < _s+num_fitness_in_task[id];k++ ){
                    for(int j=0; j< SIZE_GENE; j++){
                        buff[dem] = p->new_pop[k]->gene[j];
                        dem++;
                    }
                   // printf("||||");

                }
                //printf("\n----------------------------------Dem = %d-----------------------------------\n", dem);
            //   printf("dem = %d %d!\n", dem, num_fitness_in_task[id]);

               // printf("Gui tinh toan tu %d  den %d \n ",_s,_s+num_fitness_in_task[id]);
                // Gui tinh toan
                MPI_Send(buff, num_fitness_in_task[id]*SIZE_GENE , MPI_INT, id, tag_send, MPI_COMM_WORLD);

                delete [] buff;
                buff = nullptr;

            }
            communicate_time +=MPI_Wtime() - _s;


            // thu thap tinh toan
            // Tinh tu 0 den AVG o rank 0
           // printf("DONE tu %d  den %d \n",0,num_fitness_avg -1);
            _s = MPI_Wtime();
            for(int i=0; i<num_fitness_avg; i++ )
            {
                p->new_pop[i]->fitness = CalFitness(p->new_pop[i]->gene, SIZE_GENE);
            }
            computional_time += MPI_Wtime() -_s;

            _s = MPI_Wtime();
            for(int id = 1; id< numtasks; id++)
            {
                double *data_rev = new double[num_fitness_in_task[id]];
                MPI_Recv( data_rev, num_fitness_in_task[id] , MPI_DOUBLE, id , tag_return, MPI_COMM_WORLD, &status);
                int sender = status.MPI_SOURCE;
                int _s = sender*num_fitness_avg;
                for(int h = _s; h<_s+num_fitness_in_task[sender]; h++) p->new_pop[h]->fitness = data_rev[h-_s];
               // printf("DONE tu %d  den %d\n",_s,_s+num_fitness_in_task[sender]);

            }
            communicate_time +=  MPI_Wtime() -_s;
            _s = MPI_Wtime();
            p->Selection();
            if(best!=nullptr && p->pop[0]->fitness == best->fitness) rate++;
            else rate =0;
            Global::_getInstance()->rm = Global::_getInstance()->rm0 + ((rate+1)*2.0/(Global::_getInstance()->CONDITION));
            best = p->pop[0];

            printf("T = %d %f rm = %f\n",T, best->fitness, Global::_getInstance()->rm);
            T++;
            if(T>=Global::_getInstance()->CONDITION) break;
            p->Reproduction();
            computional_time += MPI_Wtime() - _s;
        }
        best->printGene();
        printf("Cost Final : %f\n", best->fitness);
        printf("Computional Time %14f\n",computional_time);
        printf("Communicate Time %14f\n",communicate_time);
        if(p!=NULL)
        {
            delete p;
            p= NULL;
        }
       if(best!=nullptr) delete best;




    }else{
        int *bufR = new int[num_fitness_in_task[RANK]*SIZE_GENE];
        double * data_send = new double[max_num_fitness];
        int T =1;
        while(T< Global::_getInstance()->CONDITION){
            MPI_Recv( bufR, num_fitness_in_task[RANK]*SIZE_GENE, MPI_INT, source, tag_send, MPI_COMM_WORLD, &status);
            for(int i=0; i<num_fitness_in_task[RANK]; i++)
            {
                int * route = new int[SIZE_GENE];
                for(int k = 0; k< SIZE_GENE; k++){
                    route[k] = bufR[i*SIZE_GENE +k];

                }


                data_send[i] = CalFitness(route, SIZE_GENE);
                delete [] route;
            }
            MPI_Send( data_send, num_fitness_in_task[RANK], MPI_DOUBLE, source, tag_return, MPI_COMM_WORLD);
            T++;

        }
        delete [] data_send;
        data_send = NULL;
        delete [] bufR;
        bufR = NULL;

    }

        delete[] city;
        MPI_Finalize();
    return 0;
}


