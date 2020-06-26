#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include "header.h"

struct semaphore{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}mutex;

typedef struct person{
	int id;
	int from_floor;
	int to_floor;
	double arrival_time;
}person;

struct elevator{
	int id;
	int current_floor;
	//pthread_mutex_t lock;
	//pthread_cond_v cv;
	person *people;
};

int num_elevators;
int num_floors;
int beginning_time;
int elevator_speed;
int simulation_time;
int random_seed;
int num_people_started;
int num_people_finished;
int arrive_rate;
//pthread_mutex_t *lock;
//int global_count = 0;
pthread_mutex_t lock;
pthread_cond_t cv;

typedef struct ListNode{
	person* one_person;
	struct ListNode *pre;
	struct ListNode *next;
}ListNode;

ListNode* headNode = NULL;
ListNode** pHeadNode = &headNode;


ListNode* InitNode(ListNode* node){
	node = (ListNode*)malloc(sizeof(ListNode));
	
	if(node == NULL){
		fprintf(stderr,"malloc node defeat.\n");
		return NULL;
	}
	else{
		//node->one_person->id = 0;
		//node->one_person->from_floor = 0;
		//node->one_person->to_floor = 0;
		//node->one_person->arrival_time = 0;
		node->one_person = NULL;
		node->pre = NULL;
		node->next = NULL;
	}

	return node;
}

ListNode* InsertNode(ListNode* headNode,person* new_person){
	ListNode* temp_node = headNode;
	ListNode* temp_node1 = NULL;

//	if(headNode == NULL){
//		fprintf(stderr,"No list!\n");
//		return NULL;
//	}

	temp_node1 = InitNode(temp_node1);
	if(temp_node1 == NULL){
		fprintf(stderr,"malloc node defeat.\n");
		return NULL;
	}

	temp_node1->one_person = new_person;
	
	if(headNode == NULL){
		headNode = temp_node1;
		//pHeadNode = &headNode;
	}
	else{
		while(temp_node->next != NULL){
			temp_node = temp_node->next;
		}

		temp_node->next = temp_node1;
		temp_node1->pre = temp_node;
	}

	return headNode;

}

int DeleteHeadNode(ListNode** pHeadNode){
	ListNode* temp_node = NULL;

	if((*pHeadNode) == NULL){
		fprintf(stderr,"No List!\n");
		return 0;
	}

	if((*pHeadNode)->next != NULL){
		temp_node = (*pHeadNode)->next;
		temp_node->pre = NULL;
		free((*pHeadNode));
		(*pHeadNode) = temp_node;
	}
	else{
		free((*pHeadNode));
		(*pHeadNode) = NULL;
		//headNode = NULL;
		//pHeadNode = &headNode;
	}

	return 1;

}


int DeleteLastNode(ListNode** pHeadNode){
	ListNode* temp_node = *pHeadNode;
	ListNode* temp_node1 = NULL;

	if((*pHeadNode) == NULL){
		fprintf(stderr,"No List! cannot delete from an empty list!\n");
		return 0;
	}

	while(temp_node->next != NULL){
		temp_node = temp_node->next;
	}

	temp_node1 = temp_node->pre;

	free(temp_node);
	temp_node = NULL;
	temp_node1->next = NULL;

	return 1;

}



int DeleteNode(ListNode** pHeadNode,int* peopleID){
	ListNode* temp_node = (*pHeadNode);
	ListNode* temp_node1 = NULL;
	int Count = 0;

	if((*pHeadNode) == NULL){
		fprintf(stderr,"No List!\n");
		return 0;
	}

	while(temp_node != NULL){
		Count++;
		if(temp_node->one_person->id == *peopleID){
			break;
		}
		temp_node = temp_node->next;
	}

	temp_node1 = temp_node->pre;

	if(temp_node == NULL){
		fprintf(stderr,"cannot find that person.\n");
		return 0;
	}
	else if(Count == 1){
		DeleteHeadNode(pHeadNode);
		return 1;
	}
	else if(temp_node->next == NULL){
		DeleteLastNode(pHeadNode);
		return 1;
	}

	temp_node1->next = temp_node->next;
	temp_node->next->pre = temp_node1;
	free(temp_node);
	temp_node = NULL;

	return 1;

}

int IfListEmpty(ListNode* headNode){
	
	if(headNode == NULL){
		return 1;
	}
	else{
		return 0;
	}

}

int GetLength(ListNode* headNode){
	ListNode* temp_node = headNode;
	int len = 0;

	if(headNode == NULL){
		fprintf(stderr,"No List! cannot get length.\n");
		return -1;
	}
	
	while(temp_node != NULL){
		len++;
		temp_node = temp_node->next;
	}

	return len;
}

void ClearList(ListNode** pHeadNode){
	ListNode* temp_node = NULL;
	if((*pHeadNode) == NULL){
		//fprintf(stderr,"No List! cannot clear an empty list.\n");
		return;
	}

	while((*pHeadNode)->next != NULL){
		temp_node = (*pHeadNode)->next;
		free((*pHeadNode));
		(*pHeadNode) = temp_node;
	}

	if((*pHeadNode) != NULL){
		free((*pHeadNode));
		(*pHeadNode) = NULL;
	}

	//printf("Clear people list done.\n");
}



void P(struct semaphore* S){
	pthread_mutex_lock(&S->mutex);
	while(S->value <= 0){
		pthread_cond_wait(&S->cond,&S->mutex);
	}
	S->value--;
	pthread_mutex_unlock(&S->mutex);
}

void V(struct semaphore* S){
	pthread_mutex_lock(&S->mutex);
	S->value++;
	if(S->value > 0){
		pthread_cond_signal(&S->cond);
	}
	pthread_mutex_unlock(&S->mutex);
}


int random_floor(){
	int floor =  (int)(rand()%num_floors);
	return floor;
}


void *do_elevator(void* param){
	int flag = 1;
	person* my_people;
	struct elevator* my_elevator = (struct elevator*)param;
	while(flag){
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			continue;
		}

		pthread_mutex_lock(&lock);
		while(IfListEmpty(headNode)){
			pthread_cond_wait(&cv,&lock);
			if((double)(time(NULL)-beginning_time)>simulation_time){
				flag = 0;
				pthread_mutex_unlock(&lock);
				return;
			}
		}
		//global_count--;
		my_people  = headNode->one_person;
		DeleteHeadNode(pHeadNode);			
		pthread_mutex_unlock(&lock);

		my_elevator->people = my_people;
		int from_floor = my_elevator->people->from_floor;
		int to_floor = my_elevator->people->to_floor;
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			free(my_elevator->people);
			continue;
		}

		printf("%f elevator %d starts moving from floor %d to %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->current_floor,from_floor);
		double spend = (abs(from_floor - my_elevator->current_floor))/(double)elevator_speed;
		sleep(spend);
		

		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			free(my_elevator->people);
			continue;
		}


		my_elevator->current_floor = from_floor;
		printf("%f elevator %d arrives floor %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,from_floor);
		my_elevator->people = my_people;
		printf("%f elevator %d picks up person %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->people->id);
		spend = (abs(my_elevator->current_floor - to_floor))/(double)elevator_speed;
		sleep(spend);
		

		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			free(my_elevator->people);
			continue;
		}

		my_elevator->current_floor = to_floor;
		printf("%f elevator %d drops person %d at floor %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->people->id,my_elevator->current_floor);
		free(my_elevator->people);
		
		P(&mutex);
		num_people_finished++;
		V(&mutex);
	}

	return;

}

void *do_person(void* param){
	int pcounter = 0;
	int flag = 1;
	srand(random_seed);
	
	while(flag){
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			pthread_cond_broadcast(&cv);
			continue;
		}
		sleep(arrive_rate);
		
		if((double)(time(NULL)-beginning_time > simulation_time)){
			flag = 0;
			pthread_cond_broadcast(&cv);
			continue;
		}

		person* a_person = (person*)malloc(sizeof(person));
		a_person->id = pcounter;
		pcounter++;
		a_person->from_floor = random_floor();
		a_person->to_floor = random_floor(); 
		a_person->arrival_time = (double)time(NULL) - beginning_time;

		pthread_mutex_lock(&lock);
		headNode = InsertNode(headNode,a_person);
		num_people_started++;
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			pthread_cond_broadcast(&cv);
			pthread_mutex_unlock(&lock);
			continue;
		}
		printf("%f person %d arrives on floor %d. waiting to go to floor %d.\n",a_person->arrival_time,a_person->id,a_person->from_floor,a_person->to_floor);	
		pthread_cond_signal(&cv);
		pthread_mutex_unlock(&lock);
	}
	//printf("pcounter:%d\n",pcounter);
	return;

}

void init(struct semaphore* S){
	S->value = 0;
	if(pthread_mutex_init(&S->mutex,NULL)){
		perror("mutex init error\n");
	}
	if(pthread_cond_init(&S->cond,NULL)){
		perror("cond init error\n");
	}
}



void main(int argc,char* argv[]){
	

	num_elevators = atoi(argv[1]);
	num_floors = atoi(argv[2]);
	arrive_rate = atoi(argv[3]);
	elevator_speed = atoi(argv[4]);
	simulation_time = atoi(argv[5]);
	random_seed = atoi(argv[6]);
	num_people_started = 0;
	num_people_finished = 0;
	
	init(&mutex);
	mutex.value = 1;

	
	if(pthread_mutex_init(&lock,NULL)){
		perror("mutex lock init error.\n");
	}
	if(pthread_cond_init(&cv,NULL)){
		perror("cond cv init error.\n");
	}

	pthread_t* people_thread;
	pthread_attr_t* p_attr;
	void* retval;
	

	people_thread = (pthread_t*)malloc(sizeof(pthread_t));
	p_attr = (pthread_attr_t*)malloc(sizeof(pthread_attr_t));
	
	beginning_time = time(NULL);

	if(pthread_attr_init(p_attr)){
		perror("p_attr init error.\n");
	}
	if(pthread_create(people_thread,p_attr,do_person,NULL) != 0){
		perror("pthread_create people thread error.\n");
		exit(1);
	}
	

	pthread_t* elevators_t;
	pthread_attr_t* e_attrs;

	elevators_t = (pthread_t*)malloc(sizeof(pthread_t)*num_elevators);
	e_attrs = (pthread_attr_t*)malloc(sizeof(pthread_attr_t)*num_elevators);
 	struct elevator *elevators = (struct elevator*)malloc(sizeof(struct elevator)*num_elevators);
	int i = 0;
	for(i = 0; i < num_elevators;i++){
			
		(elevators+i)->id = i;
		(elevators+i)->current_floor = 0;
		(elevators+i)->people = NULL;
		
		if(pthread_attr_init(e_attrs+i)){
			perror("e_attrs init error.\n");
		}
		if(pthread_create(elevators_t+i,e_attrs+i,do_elevator,elevators+i) != 0){
			perror("pthread create elevators_t error.\n");
			exit(1);
		}		

	}




	pthread_join(*people_thread,&retval);
	for(i = 0;i < num_elevators;i++){
		pthread_join(elevators_t[i],&retval);
	}
	free(people_thread);
	free(p_attr);
	free(elevators_t);
	free(e_attrs);
	free(elevators);
	ClearList(pHeadNode);
	
	printf("Simulation result:%d people have started, %d people have finished during %f seconds.\n",num_people_started,num_people_finished,(double)simulation_time);

	exit(0);	
}
