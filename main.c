#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define _GNU_SOURCE
#define __USE_GNU 1
#include <fcntl.h>

/*Cited:
https://github.com/torvalds/linux/tree/master/Documentation/scheduler?/fbclid=IwAR2CY8FthfoUwz5gcZm0Ep_CLWaCcpLTlkpfl8V4H3C0R0992B26SyL357s
 https://github.com/funson/energy-aware-scheduling-simulator
https://www.youtube.com/channel/UCYgt4WpyNUNU06vBGegcBFA*/
int processesNumber;
int ProcessArrivalTime[1000];
int ProcessComputationTime[1000];
int ProcessDeadline[1000];
int ProcessPeriode[1000];
int PeriodsLCM;

char help[255];
int i,j,h;
float f;
/*struct task
{
    char name[20];
    double ArrivalTime;
    double ComputationTime;
    double Deadline;
    int unit;
};*/

void getProcesses()
{
    FILE *InputFile;
    InputFile = fopen("input.txt", "r");
    fgets(help, 255, InputFile);
    processesNumber = atoi(help);
    // getting processes from the file input.txt;
    for(i = 0; i<processesNumber; i++)
    {
        fgets(help, 255, InputFile);
        ProcessArrivalTime[i]= atoi(help);
        
        fgets(help, 255, InputFile);
        ProcessComputationTime[i]= atoi(help);
        
        fgets(help, 255, InputFile);
        ProcessDeadline[i]= atoi(help);
    }
    fclose(InputFile);
    // showing the processes;
    printf("Si| Ai, Ci, Di\n");
    printf("----------------\n");
    for(i=0; i<processesNumber ; i++)
    {
        printf("P%d | %d, %d, %d\n",i,ProcessArrivalTime[i], ProcessComputationTime[i], ProcessDeadline[i]);
    }
    
    
}
void getPeriodsLCM()
{
    int help[1000];
    for(i=0 ; i<processesNumber; i++)
    {
        help[i] = ProcessPeriode[i];
    }
    int h = 0;
    int theLowestValueInHelp;
    int theLowestValueIndex;
    while(h == 0)
    {
        h = 1;
        theLowestValueInHelp = help[0];
        theLowestValueIndex = 0;
        
        for(i=0 ; i<processesNumber; i++)
        {
            if(theLowestValueInHelp != help[i])
            {
                h = 0;
                if(theLowestValueInHelp>help[i])
                {
                    theLowestValueInHelp = help[i];
                    theLowestValueIndex = i;
                    
                }
                
            }
        }
        if(h == 0)
        {
            help[theLowestValueIndex] = theLowestValueInHelp + ProcessPeriode[theLowestValueIndex];
        }
    }
    PeriodsLCM = help[0];
    printf("We Can Schedule That Real System In %d Units(%d is the Lcm of our processes periods)",PeriodsLCM,PeriodsLCM);
    
}

//Float Schedulability
// Return Condition  (need to take power aware)
float schedulability()
{
    float Condition = 0;
    float x,y;
    printf("\n The Sum Of :");
    for(i=0 ; i<processesNumber ; i++)
    {
        x = ProcessComputationTime[i];
        y = ProcessPeriode[i];
        Condition = Condition + (x/y);
        printf(" (%d/%d) ",ProcessComputationTime[i],ProcessPeriode[i]);
    }
    printf("is equal : %f", Condition);
    return Condition;
}

void schedule()
{
    int earliestDeadline;
    int earliestDeadlineIndex;
    int schedulingTable[PeriodsLCM];
    int remainsCapacity[1000];
    int nextDeadline[1000];
    int processOnlineNewPeriod[1000];
    for(i=0 ; i<processesNumber ; i++)
    {
        nextDeadline[i] = ProcessDeadline[i];
        remainsCapacity[i] = ProcessComputationTime[i];
        processOnlineNewPeriod[i] = 0;
    }
    // scheduling time milestone...
    FILE *OutputtFile;
    OutputtFile = fopen("output.txt", "w");
    fprintf(OutputtFile, "Periods LCM = %d\n",PeriodsLCM);
    for(i=0; i<PeriodsLCM; i++)
    {
        printf("\n(%d,%d) : ",i,i+1);
        
        //getting the earliest deadline
        earliestDeadline = PeriodsLCM;
        earliestDeadlineIndex = -1;
        for(j=0 ; j<processesNumber ; j++)
        {
            if(remainsCapacity[j] > 0)
            {
                
                if(earliestDeadline > nextDeadline[j])
                {
                    
                    earliestDeadline = nextDeadline[j];
                    earliestDeadlineIndex = j;
                }
            }
        }
        printf("     [exc = %d] ",earliestDeadlineIndex);
        fprintf(OutputtFile, "(%d,%d) :   [exc = %d]\n",i,i+1,earliestDeadlineIndex);
        remainsCapacity[earliestDeadlineIndex]--;
        
        //get the next deadline distance
        for(j=0 ; j<processesNumber ; j++)
        {
            
            if(processOnlineNewPeriod[j] == (ProcessPeriode[j] - 1 ) )
            {
                nextDeadline[j] =  ProcessDeadline[j];
                remainsCapacity[j] = ProcessComputationTime[j];
                processOnlineNewPeriod[j] = 0;
            }
            else
            {
                if(nextDeadline[j] >0)
                {
                    nextDeadline[j]--;
                }
                else
                {
                    if(remainsCapacity[j] > 0)
                    {
                        printf("\nthe process %d has no chance to complete its Capacity",j);
                        fprintf(OutputtFile, "\nthe process %d has no chance to complete its Capacity",j);
                    }
                }
                processOnlineNewPeriod[j]++;
            }
            
            
        }
    }
    fclose(OutputtFile);
    
}


//Algorithm for power aware scheduleing
//void p_sched(){
double getj_thUatTimet(int* queue, int t, int j,int total)
{
    //do step 2
    double diff = queue[j] - t;
    int remainedTask = getRemainedTask(queue,total);
    return (double)remainedTask*1.0 / ((double)total*1.0) / diff;
}

int getRemainedTask(int* queue,int total)
{
    //get the number of remained tasks
    int i;
    int count = 0;
    //int length = sizeof(queue) / sizeof(int);
    for (i = 0; i < total; i++)
    {
        if (queue[i] != -1)
            count++;
    }
    return count;
}

int isQueueEmpty(int* queue,int total)
{
    //check is queue empty
    if (getRemainedTask(queue,total) == 0)
    {
        return 1;
    }
    return 0;
}
double* getUtilizationFactor(int* queue, int time,int total)
{
    //allocate memory to utiliazaitonFactor
    double* utiliazationFactor = (double*)malloc(200 * sizeof(double));
    //total tasks in the queue
    int totalTask = total;
    //a temp array to store u_j(t)
    double* tempUF = (double*)malloc(totalTask * sizeof(double));
    for (int i = 0; i<totalTask; i++)
    {
        //initialize temp array
        tempUF[i] = 0;
    }
    while (!isQueueEmpty(queue,totalTask))
    {
        //if queue is not empty
        //get the number of remained tasks
        int remainedTask = getRemainedTask(queue,totalTask);
        for (int i = 0; i < totalTask; i++)
        {
            if (queue[i] != -1)
            {
                //queue[i] == -1 means this task has been done
                //step 2, calculate u_j(t)
                tempUF[i] = getj_thUatTimet(queue, time, i, totalTask);
            }
        }
        double sum = 0;
        for (int i = 0; i < totalTask; i++)
        {
            sum += tempUF[i];
        }
        //get average
        utiliazationFactor[time] = sum / getRemainedTask(queue,totalTask);
        int selectedTask;
        int flag = 0;
        for (int i = 0; i < totalTask; i++)
        {
            if (tempUF[i] == utiliazationFactor[time])
            {
                //get k and set the kth task to be done.
                selectedTask = i;
                tempUF[i] = 0;
                time = queue[i];
                queue[i] = -1;
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            //if there is no utilizationFactor(t) = u_k(t), then execute the first task in the queue
            for (int i = 0; i < totalTask; i++)
            {
                if (queue[i] != -1)
                {
                    selectedTask = i;
                    tempUF[i] = 0;
                    time = queue[i];
                    queue[i] = -1;
                    flag = 1;
                    break;
                }
            }
        }
    }
    return utiliazationFactor;
}

    //Have a tiny test with int* q = (int*)malloc(sizeof(int) * 3);
    /*q[0] = 3;
    q[1] = 5;
    q[2] = 9;
    int t = 2;
    int total = 3;
    double* rFactor = getUtilizationFactor(q, t,total);*/



int main()
{
    
    
    printf("EDF Programme \n");
    for(i=0; i<1024000; i++);
    printf("done \n");
    
    printf("\n\nOur System :\n");
    getProcesses();
    
    printf("\n\nPeriods LCM :\n");
    getPeriodsLCM();
    
    printf("\n\nSchedulability Test :\n");
    f = schedulability();
    if(f <= 1)
    {
        printf("\n This Real Time System Is Schedulable because %f <= 1", f);
       
        schedule();
    }
    else
    {
        printf("\nThis Real Time System Is Not Schedulable because %f > 1",f);
        FILE *OutputtFile;
        OutputtFile = fopen("output.txt", "w");
        fprintf(OutputtFile, "This Real Time System Is Not Schedulable because %f > 1",f);
        fclose(OutputtFile);
        exit(0);
    }
}
