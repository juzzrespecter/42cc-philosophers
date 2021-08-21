# Philosophers

_Part of 42 common-core cursus: an introduction to concurrent programming and synchronization in C lang._

The project consists of a simulation of the Dining Philosophers problem: a number of philosophers
sit on a round table to think and eat. In between each philosopher, there is a fork, so there
is the same number of forks and philosophers. In the middle of the table, there is a bowl
filled with spaghetti, refilled everytime it is emptied. Philosophers are not known for their 
great motor coordination abilities, so they need two forks to take a meal and start eating.
There are 3 activities in this simulation: a philosopher can **think**, while they try to get
access to forks, they can **eat**, and at last they can **sleep** when they are done eating.

Basically this is a resource sharing problem, in which every philosopher is a thread running
in parallel, and each fork is a shared resource that must be shared between each pair of threads, 
with its access controlled by a mutex/semaphor.
The goal of this project is to code a simulation that can avoid deadlock when trying to access
to the resources, and also avoid resource starvation for every thread (no philosopher should
die if not inevitable!)

The program accepts four parameters, with a fifth optional one:
```
- [number_of_philosophers]		number of concurrent threads
- [time_to_die]				in miliseconds, time that must pass since starting a meal
					until a philo dies.
- [time_to_eat]				in miliseconds, time a philo takes eating.
- [time_to_sleep]			in miliseconds, time a philo takes sleeping
- [number_of_times_a_philo_must_eat]	the only way the simulation can end without tragedy,
					the optional argument indicates the number of meals
					every philo can eat at least before ending the program
```

### deadlock stategy

If we let every philosopher take first their left fork, we will run into a deadlock problem; no
philosopher can access its second fork as it is already in use by their right neighbour.

To avoid this problem, i have implemented the even/odd solution, odds first take their right fork,
evens take their left fork; 
Explained with an example, four philosophers sit at the table: Adorno takes the first seat, Horkheimer takes the second, Marcuse the third and lastly Habermas takes the fourth seat. Then, Adorno as the first odd philosopher will fight for its right fork with Horkheimer, which is even. The same case applies with the other pair: every pair of philosophers will fight for their first fork, leaving the loser blocked and the winner with access to the second fork, so with this solution there will be always at least one philosopher eating at the table.

## philo

In this version of the simulaton, every philosopher is executed as a thread, every fork is a mutex, a fork will be always shared between its left and right philosopher.

Execution:
```
make; 
./philo [n_of_philo] [time_to_die] [time_to_eat] [time_to_sleep] ([meal_cond]);
```
## philo_bonus

In this version, every philo is a process, forks are at the middle of the table as a named semaphore.

Execution:
```
make bonus;
./philo_bonus [n_of_philo] [time_to_die] [time_to_eat] [time_to_sleep] ([meal_cond]);
```
Project passed with 125/100 ✔️😎
