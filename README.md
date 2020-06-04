# Power-Aware-Scheduling

With the complete knowledge of the arrival of sporadic tasks, the optimal strategy is to set the supply voltage to the single minimum voltage W the time such that W the tasks just meet their deadline. h this example, for the time interval
[0,200].

I assume the deadlines are equal to the periods. Each sporadic task Si (Ai, Ci, Di) is characterized by its arrival time Ai, worst-case computation time Ci at the reference voltage, and hard destine Di. All the parameters of sporadic task become known when the task arrives. Online scheduling algorithm focus on dynamically performing feasibility checks.

I am using EDF in the acceptance test section. And it starts working when a sporadic task arrives to check if it is schedulable or not.


According to the referenced paper, we let S = Si(Ai;Ci;Di) : 1 <= I <=m  mdenote We stream of the tasks that have been accepted but uncompleted at the time t when an acceptance test is to be carried out. They assume RC(Si; t) denote
tie unfinished portion of the worst-case execution time at the highest speed of the task Si at the time t. They keep track of the maximum utilization factor of the processor. U(t) > 1, the set S can not be feasibly scheduled even at the highest speed using the EDF schedulng algorithm. When Snew arrives, update RC(S1; t) for the currently running task S1 and compute a new U(t) including the new task so that if U(t) > 1; Snew is not accepted.

After that, According to the STS algorithm from the paper [1], my understanding is follows and I implemented this to schedule the tasks in optimal speeds in order to minimize the energy consumption and get the output of the complete task schedule and the optimal speed schedule.

2 References
[1] I. Hong, M. Potkonjak, and M. B. Srivastava, \On-line scheduling of hard
real-time tasks on variable voltage processor," Proc. Computer-Aided Design
(ICCAD), pages 653{656, November 1998.
