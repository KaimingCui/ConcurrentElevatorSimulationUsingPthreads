Project Name:CSCI 503 Operating System Homework 4
Author: Kaiming Cui
Email: cuik@iu.edu

Document:

The tar ball contains two directories "BasicElevator" and "ImprovedElevator", each of the directory contains 3 files, a source.c file, a header.h file, a Makefile.

Dependency:
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include "header.h"

Building and Testing:
1.Upload the zip file to tesla server on local (before login).
    scp Lab3_KaimingCui.tar [user_name]@tesla.cs.iupui.edu:/home/[user_name]
    * [user_name] is your username in IUPUI.
2. Login to tesla server with your private password.
    ssh [user_name]@tesla.cs.iupui.edu
3. untar the tar ball.
	tar -xvf Lab4_KaimingCui.tar
4. Go to BasicElevator directory or ImprovedElevator directory
5. Using Makefile to compile the source code
	make
5. The "BasicElevator(ImprovedElevator).out" executable file is produced, run this file
	./KaimingCuiMPMC.out six arguments
	./KaimingCuiPMM.out six arguments

Notice: The six arguments must in this order (same as the requirements on canvas):
NUM_OF_ELEVATORS, NUM_OF_FLOORS, how often people arrive, elevator_speed, Total time to simulate, a random seed

6.After run the program, you will see the movement of elevator and in the end you will see how many people arrive and how many people finished.


Notice: for improved elevator, the logical is that the elevator will serve the first person who is the first one push the button (I mean at that time no person before this person push the button). The elevator will go to the floor to pick up that person, during this movement, the elevator will not stop to pickup the other people. After the elevator pick up that first person, than it will pick up the prople that are in its way. For example, a person push the button at floor 8 he/she wants to go to floor 0, the idle elevator at floor 0 goes to floor 8 without stop, after the elevator pick up that person at floor 8, it will going down to floor 0. during the elevator going down, it will pick up the persons who push button below floor 8 and want to go to the floor below floor 8 (let's say person B at floor 5 push the button when the elevator is above him/her, person B want to go to floor 3, then person B will be picked up by the elevator).

Improtant:
For Improved Elevator
If you set up less elevator, high people arrive rate and long total simulation time, you will see the output looks like this, it means the logical works, the elevator can pickup and drops not more than one person. This output depends on how the random function generates the peosons, if you are lucky, you will see it, but anyway, you can check the output to see if the elevator can pick up more than one people.

7.000000 elevator 0 drops Person 2, Person 3 at floor 1

Example output:
For Basic Elevator:
(base) [cuik@tesla BasicElevator]$ ./BasicElevator.out 3 10 1 2 10 20
1.000000 person 0 arrives on floor 1. waiting to go to floor 8.
1.000000 elevator 0 starts moving from floor 0 to 1.
1.000000 elevator 0 arrives floor 1.
1.000000 elevator 0 picks up person 0.
2.000000 person 1 arrives on floor 7. waiting to go to floor 9.
2.000000 elevator 2 starts moving from floor 0 to 7.
3.000000 person 2 arrives on floor 6. waiting to go to floor 1.
3.000000 elevator 1 starts moving from floor 0 to 6.
4.000000 elevator 0 drops person 0 at floor 8.
4.000000 person 3 arrives on floor 2. waiting to go to floor 1.
4.000000 elevator 0 starts moving from floor 8 to 2.
5.000000 elevator 2 arrives floor 7.
5.000000 elevator 2 picks up person 1.
5.000000 person 4 arrives on floor 0. waiting to go to floor 1.
6.000000 elevator 2 drops person 1 at floor 9.
6.000000 elevator 2 starts moving from floor 9 to 0.
6.000000 elevator 1 arrives floor 6.
6.000000 elevator 1 picks up person 2.
6.000000 person 5 arrives on floor 0. waiting to go to floor 4.
7.000000 elevator 0 arrives floor 2.
7.000000 elevator 0 picks up person 3.
7.000000 elevator 0 drops person 3 at floor 1.
7.000000 elevator 0 starts moving from floor 1 to 0.
7.000000 elevator 0 arrives floor 0.
7.000000 elevator 0 picks up person 5.
7.000000 person 6 arrives on floor 7. waiting to go to floor 9.
8.000000 elevator 1 drops person 2 at floor 1.
8.000000 elevator 1 starts moving from floor 1 to 7.
8.000000 person 7 arrives on floor 4. waiting to go to floor 9.
9.000000 elevator 0 drops person 5 at floor 4.
9.000000 elevator 0 starts moving from floor 4 to 4.
9.000000 elevator 0 arrives floor 4.
9.000000 elevator 0 picks up person 7.
9.000000 person 8 arrives on floor 5. waiting to go to floor 7.
10.000000 elevator 2 arrives floor 0.
10.000000 elevator 2 picks up person 4.
10.000000 elevator 2 drops person 4 at floor 1.
10.000000 elevator 2 starts moving from floor 1 to 5.
10.000000 person 9 arrives on floor 9. waiting to go to floor 9.
Simulation result:10 people have started, 6 people have finished during 10.000000 seconds.

For Improved Elevator:
(base) [cuik@tesla ImprovedElevator]$ ./ImprovedElevator.out 2 10 1 2 30 20
1.000000 person 0 arrives on floor 1. waiting to go to floor 8.
1.000000 elevator 0 starts moving from floor 0 to 1.
1.000000 elevator 0 arrives floor 1.
1.000000 elevator 0 picks up Person 0 at floor 1
1.000000 elevator 0 starts moving from floor 1 to 8.
2.000000 person 1 arrives on floor 7. waiting to go to floor 9.
2.000000 elevator 1 starts moving from floor 0 to 7.
3.000000 person 2 arrives on floor 6. waiting to go to floor 1.
4.000000 elevator 0 arrives floor 8.
4.000000 elevator 0 drops Person 0 at floor 8
4.000000 elevator 0 starts moving from floor 8 to 6.
4.000000 person 3 arrives on floor 2. waiting to go to floor 1.
5.000000 elevator 0 arrives floor 6.
5.000000 elevator 0 picks up Person 2 at floor 6
5.000000 elevator 0 starts moving from floor 6 to 2.
5.000000 elevator 1 arrives floor 7.
5.000000 elevator 1 picks up Person 1 at floor 7
5.000000 elevator 1 starts moving from floor 7 to 9.
5.000000 person 4 arrives on floor 0. waiting to go to floor 1.
6.000000 elevator 1 arrives floor 9.
6.000000 elevator 1 drops Person 1 at floor 9
6.000000 elevator 1 starts moving from floor 9 to 0.
6.000000 person 5 arrives on floor 0. waiting to go to floor 4.
7.000000 elevator 0 arrives floor 2.
7.000000 elevator 0 picks up Person 3 at floor 2
7.000000 elevator 0 starts moving from floor 2 to 1.
7.000000 elevator 0 arrives floor 1.
7.000000 elevator 0 drops Person 2, Person 3 at floor 1
7.000000 elevator 0 starts moving from floor 1 to 0.
7.000000 elevator 0 arrives floor 0.
7.000000 elevator 0 picks up Person 5 at floor 0
7.000000 elevator 0 starts moving from floor 0 to 4.
7.000000 person 6 arrives on floor 7. waiting to go to floor 9.
8.000000 person 7 arrives on floor 4. waiting to go to floor 9.
9.000000 elevator 0 arrives floor 4.
9.000000 elevator 0 drops Person 5 at floor 4
9.000000 elevator 0 starts moving from floor 4 to 7.
9.000000 person 8 arrives on floor 5. waiting to go to floor 7.
10.000000 elevator 0 arrives floor 7.
10.000000 elevator 0 picks up Person 6 at floor 7
10.000000 elevator 0 starts moving from floor 7 to 9.
10.000000 elevator 1 arrives floor 0.
10.000000 elevator 1 picks up Person 4 at floor 0
10.000000 elevator 1 starts moving from floor 0 to 1.
10.000000 elevator 1 arrives floor 1.
10.000000 elevator 1 drops Person 4 at floor 1
10.000000 elevator 1 starts moving from floor 1 to 4.
10.000000 person 9 arrives on floor 9. waiting to go to floor 9.
11.000000 elevator 0 arrives floor 9.
11.000000 elevator 0 drops Person 6 at floor 9
11.000000 elevator 0 starts moving from floor 9 to 9.
11.000000 elevator 0 arrives floor 9.
11.000000 elevator 0 picks up Person 9 at floor 9
11.000000 elevator 0 drops Person 9 at floor 9
11.000000 elevator 1 arrives floor 4.
11.000000 elevator 1 picks up Person 7 at floor 4
11.000000 elevator 1 starts moving from floor 4 to 5.
11.000000 elevator 1 arrives floor 5.
11.000000 elevator 1 picks up Person 8 at floor 5
11.000000 elevator 1 starts moving from floor 5 to 7.
11.000000 person 10 arrives on floor 5. waiting to go to floor 6.
11.000000 elevator 0 starts moving from floor 9 to 5.
12.000000 elevator 1 arrives floor 7.
12.000000 elevator 1 drops Person 8 at floor 7
12.000000 elevator 1 starts moving from floor 7 to 9.
12.000000 person 11 arrives on floor 1. waiting to go to floor 1.
13.000000 elevator 1 arrives floor 9.
13.000000 elevator 1 drops Person 7 at floor 9
13.000000 elevator 1 starts moving from floor 9 to 1.
13.000000 elevator 0 arrives floor 5.
13.000000 elevator 0 picks up Person 10 at floor 5
13.000000 elevator 0 starts moving from floor 5 to 6.
13.000000 elevator 0 arrives floor 6.
13.000000 elevator 0 drops Person 10 at floor 6
13.000000 person 12 arrives on floor 9. waiting to go to floor 3.
13.000000 elevator 0 starts moving from floor 6 to 9.
14.000000 person 13 arrives on floor 0. waiting to go to floor 1.
14.000000 elevator 0 arrives floor 9.
14.000000 elevator 0 picks up Person 12 at floor 9
14.000000 elevator 0 starts moving from floor 9 to 3.
15.000000 person 14 arrives on floor 5. waiting to go to floor 4.
16.000000 person 15 arrives on floor 7. waiting to go to floor 6.
17.000000 elevator 1 arrives floor 1.
17.000000 elevator 1 picks up Person 11 at floor 1
17.000000 elevator 1 drops Person 11 at floor 1
17.000000 elevator 1 starts moving from floor 1 to 0.
17.000000 elevator 1 arrives floor 0.
17.000000 elevator 1 picks up Person 13 at floor 0
17.000000 elevator 1 starts moving from floor 0 to 1.
17.000000 elevator 1 arrives floor 1.
17.000000 elevator 1 drops Person 13 at floor 1
17.000000 elevator 1 starts moving from floor 1 to 5.
17.000000 elevator 0 arrives floor 3.
17.000000 elevator 0 drops Person 12 at floor 3
17.000000 elevator 0 starts moving from floor 3 to 7.
17.000000 person 16 arrives on floor 5. waiting to go to floor 4.
18.000000 person 17 arrives on floor 5. waiting to go to floor 3.
19.000000 elevator 1 arrives floor 5.
19.000000 elevator 1 picks up Person 14, Person 16, Person 17 at floor 5
19.000000 elevator 1 starts moving from floor 5 to 4.
19.000000 elevator 1 arrives floor 4.
19.000000 elevator 1 drops Person 14, Person 16 at floor 4
19.000000 elevator 1 starts moving from floor 4 to 3.
19.000000 elevator 1 arrives floor 3.
19.000000 elevator 1 drops Person 17 at floor 3
19.000000 elevator 0 arrives floor 7.
19.000000 elevator 0 picks up Person 15 at floor 7
19.000000 elevator 0 starts moving from floor 7 to 6.
19.000000 elevator 0 arrives floor 6.
19.000000 elevator 0 drops Person 15 at floor 6
19.000000 person 18 arrives on floor 7. waiting to go to floor 0.
19.000000 elevator 1 starts moving from floor 3 to 7.
20.000000 person 19 arrives on floor 4. waiting to go to floor 8.
20.000000 elevator 0 starts moving from floor 6 to 4.
21.000000 elevator 0 arrives floor 4.
21.000000 elevator 0 picks up Person 19 at floor 4
21.000000 elevator 0 starts moving from floor 4 to 8.
21.000000 elevator 1 arrives floor 7.
21.000000 elevator 1 picks up Person 18 at floor 7
21.000000 elevator 1 starts moving from floor 7 to 0.
21.000000 person 20 arrives on floor 3. waiting to go to floor 4.
22.000000 person 21 arrives on floor 2. waiting to go to floor 0.
23.000000 elevator 0 arrives floor 8.
23.000000 elevator 0 drops Person 19 at floor 8
23.000000 elevator 0 starts moving from floor 8 to 3.
23.000000 person 22 arrives on floor 5. waiting to go to floor 8.
24.000000 elevator 1 arrives floor 0.
24.000000 elevator 1 drops Person 18 at floor 0
24.000000 elevator 1 starts moving from floor 0 to 2.
24.000000 person 23 arrives on floor 9. waiting to go to floor 1.
25.000000 elevator 1 arrives floor 2.
25.000000 elevator 1 picks up Person 21 at floor 2
25.000000 elevator 1 starts moving from floor 2 to 0.
25.000000 person 24 arrives on floor 5. waiting to go to floor 1.
25.000000 elevator 0 arrives floor 3.
25.000000 elevator 0 picks up Person 20 at floor 3
25.000000 elevator 0 starts moving from floor 3 to 4.
25.000000 elevator 0 arrives floor 4.
25.000000 elevator 0 drops Person 20 at floor 4
25.000000 elevator 0 starts moving from floor 4 to 5.
25.000000 elevator 0 arrives floor 5.
25.000000 elevator 0 picks up Person 22 at floor 5
25.000000 elevator 0 starts moving from floor 5 to 8.
26.000000 elevator 1 arrives floor 0.
26.000000 elevator 1 drops Person 21 at floor 0
26.000000 elevator 1 starts moving from floor 0 to 9.
26.000000 elevator 0 arrives floor 8.
26.000000 elevator 0 drops Person 22 at floor 8
26.000000 elevator 0 starts moving from floor 8 to 5.
26.000000 person 25 arrives on floor 0. waiting to go to floor 2.
27.000000 elevator 0 arrives floor 5.
27.000000 elevator 0 picks up Person 24 at floor 5
27.000000 elevator 0 starts moving from floor 5 to 1.
27.000000 person 26 arrives on floor 7. waiting to go to floor 3.
28.000000 person 27 arrives on floor 5. waiting to go to floor 6.
29.000000 elevator 0 arrives floor 1.
29.000000 elevator 0 drops Person 24 at floor 1
29.000000 elevator 0 starts moving from floor 1 to 0.
29.000000 elevator 0 arrives floor 0.
29.000000 elevator 0 picks up Person 25 at floor 0
29.000000 elevator 0 starts moving from floor 0 to 2.
29.000000 person 28 arrives on floor 8. waiting to go to floor 5.
30.000000 elevator 1 arrives floor 9.
30.000000 elevator 1 picks up Person 23 at floor 9
30.000000 elevator 1 starts moving from floor 9 to 8.
30.000000 elevator 1 arrives floor 8.
30.000000 elevator 1 picks up Person 28 at floor 8
30.000000 elevator 1 starts moving from floor 8 to 7.
30.000000 elevator 1 arrives floor 7.
30.000000 elevator 1 picks up Person 26 at floor 7
30.000000 elevator 1 starts moving from floor 7 to 5.
30.000000 elevator 0 arrives floor 2.
30.000000 elevator 0 drops Person 25 at floor 2
30.000000 elevator 0 starts moving from floor 2 to 5.
30.000000 person 29 arrives on floor 8. waiting to go to floor 3.
Simulation result:30 people have started, 25 people have finished during 30.000000 seconds.