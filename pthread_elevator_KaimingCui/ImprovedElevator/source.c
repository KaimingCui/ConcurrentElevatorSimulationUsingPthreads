#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include "header.h"
//the semaphore used to access num_of_arrives, num_of_finnish
struct semaphore{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}mutex;

typedef struct person{
	int id;
	int from_floor;
	int to_floor;
	int dir; //1 means going up, 0 means going down
	double arrival_time;
}person;

//double link
typedef struct ListNode{
	person* one_person;
	struct ListNode *pre;
	struct ListNode *next;
}ListNode;

struct elevator{
	int id;
	int current_floor;
	int next_floor;
	int dir;
	//pthread_mutex_t lock;
	//pthread_cond_v cv;
	//person *people;
	ListNode* ele_head;
	ListNode** pele_head;
	ListNode* pickup;
	ListNode** ppickup;
	ListNode* drops;
	ListNode** pdrops;
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

int DeleteGivenNode(ListNode** pHeadNode,ListNode* target_node){
	ListNode* temp_node = NULL;

	if(target_node->pre == NULL){
		DeleteHeadNode(pHeadNode);
		return 1;
	}
	else if(target_node->next == NULL){
		DeleteLastNode(pHeadNode);
		return 1;
	}
	else{
		temp_node = target_node->pre;
		temp_node->next = target_node->next;
		target_node->next->pre = temp_node;
		free(target_node);
		return 1;		
	}

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
		free((*pHeadNode)->one_person);
		free((*pHeadNode));
		(*pHeadNode) = temp_node;
	}

	if((*pHeadNode) != NULL){
		free((*pHeadNode)->one_person);
		free((*pHeadNode));
		(*pHeadNode) = NULL;
	}

	//printf("Clear people list done.\n");
}

void ClearListnofree(ListNode** pHeadNode){
	ListNode* temp_node = NULL;
	if((*pHeadNode)==NULL){
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

}


int cmp_lg(const void *a , const void *b)
{
        return *(int *)a - *(int *)b;
}

int cmp_gl( const void *a , const void *b)
{
        return *(int *)b - *(int *)a;
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

//elevator thread function
void *do_elevator(void* param){
	int flag = 1;
	person* my_people;
	struct elevator* my_elevator = (struct elevator*)param;
	while(flag){
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			continue;
		}
		//go to pick up the first person who is the first one push the button
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
		//my_people  = headNode->one_person;
		my_elevator->ele_head = InsertNode(my_elevator->ele_head,headNode->one_person);		
		DeleteHeadNode(pHeadNode);			
		pthread_mutex_unlock(&lock);

		//my_elevator->people = my_people;
		//int from_floor = my_elevator->people->from_floor;
		//int to_floor = my_elevator->people->to_floor;
		my_elevator->next_floor = my_elevator->ele_head->one_person->from_floor;
		if(my_elevator->current_floor >= my_elevator->next_floor){
			my_elevator->dir = 0;
		}
		else{
			my_elevator->dir = 1;
		}
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;
			ClearList(my_elevator->pele_head);
			continue;
		}
		
		printf("%f elevator %d starts moving from floor %d to %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->current_floor,my_elevator->next_floor);
		double spend = (abs(my_elevator->next_floor - my_elevator->current_floor))/(double)elevator_speed;
		sleep(spend);
		
		if((double)(time(NULL)-beginning_time) > simulation_time){
			flag = 0;	
			ClearList(my_elevator->pele_head);
			continue;
		}
		
		my_elevator->current_floor = my_elevator->next_floor;
		printf("%f elevator %d arrives floor %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->current_floor);
		my_elevator->dir = my_elevator->ele_head->one_person->dir;	
		pthread_mutex_lock(&lock);
		//check the global list
		ListNode* temp_node = headNode;
		
		//check the double link to see any other people I can pick up
		while(temp_node != NULL){
			if(temp_node->one_person->dir == my_elevator->dir){
				//when elevator is going down
				if(my_elevator->dir == 0){
					if(temp_node->one_person->from_floor <= my_elevator->current_floor){
						//add the person into elevator's list
						my_elevator->ele_head = InsertNode(my_elevator->ele_head,temp_node->one_person);				
						//delete that node from global list
						//ListNode* target_node = temp_node;
						int* targetid = &(temp_node->one_person->id);
						temp_node = temp_node->next;
						DeleteNode(pHeadNode,targetid);
						continue;
					}
					else{
						temp_node = temp_node->next;
					}
				}
				//when elevator is going up
				else if(my_elevator->dir == 1){
					if(temp_node->one_person->from_floor >= my_elevator->current_floor){
						my_elevator->ele_head = InsertNode(my_elevator->ele_head,temp_node->one_person);
						//ListNode* target_node = temp_node;
						int* targetid = &(temp_node->one_person->id);	
						temp_node = temp_node->next;
						DeleteNode(pHeadNode,targetid);
						continue;
					}
					else{	
						temp_node = temp_node->next;
					}		
				}

			}
			else{
				temp_node = temp_node->next;
			}
		}

		pthread_mutex_unlock(&lock);
		int num_people = GetLength(my_elevator->ele_head);
		int stations = num_people*2;
		int* floor_order = (int*)malloc(sizeof(int)*stations);
		
		//printf("-------------\n");
		//printf("%d\n",stations);
		temp_node = my_elevator->ele_head;
		int i = 0;
		while(temp_node != NULL){
			*(floor_order+i) = temp_node->one_person->from_floor;
			*(floor_order+(i+1)) = temp_node->one_person->to_floor;
			i = i + 2;
			temp_node = temp_node->next;
		}
		
		if(my_elevator->dir == 1){
			qsort(floor_order,stations,sizeof(int),cmp_lg);
		}
		else{
			qsort(floor_order,stations,sizeof(int),cmp_gl);
		}
		/*int cc = 0;
		for(cc = 0; cc < stations;cc++){
			printf("-------------\n");
			printf("%d",*(floor_order + cc));
		}*/
		i = 0;
		while(IfListEmpty(my_elevator->ele_head) != 1){
			temp_node = my_elevator->ele_head;
			//i = 0;
			while(temp_node != NULL){
				//ListNode* targetNode = NULL;
				if(temp_node->one_person->from_floor == my_elevator->current_floor && temp_node->one_person->to_floor == my_elevator->current_floor){	
					my_elevator->pickup = InsertNode(my_elevator->pickup,temp_node->one_person);	
					my_elevator->drops = InsertNode(my_elevator->drops,temp_node->one_person);
					i = i + 2;
					//targetNode = temp_node;
					int* targetid = &(temp_node->one_person->id);
					temp_node = temp_node->next;
					DeleteNode(my_elevator->pele_head,targetid);
				//	P(&mutex);
				//	num_people_finished++;
				//	V(&mutex);
				}
				else if(temp_node->one_person->from_floor == my_elevator->current_floor){
					my_elevator->pickup = InsertNode(my_elevator->pickup,temp_node->one_person);	
					i++;
					temp_node = temp_node->next;
				}
				else if(temp_node->one_person->to_floor == my_elevator->current_floor){
					my_elevator->drops = InsertNode(my_elevator->drops,temp_node->one_person);
					i++;
					//targetNode = temp_node;
					int* targetid = &(temp_node->one_person->id);
					temp_node = temp_node->next;
					DeleteNode(my_elevator->pele_head,targetid);
				//	P(&mutex);
				//	num_people_finished++;
				//	V(&mutex);
				}
				else{
					temp_node = temp_node->next;
				}
			}
			//int num_pickup = GetLength(my_elevator->pickup); 
			//int num_drops = GetLength(my_elevator->drops);
			char strout[218] = "";
			char tempstr[20] = "";
			temp_node = my_elevator->pickup;
			int j = 0;
			while(temp_node != NULL){
				if(j==0){
					double d = (double)(time(NULL) - beginning_time);
					strcpy(tempstr,"");
					sprintf(tempstr,"%.6lf",d);
					strcat(strout,tempstr);
					strcat(strout," elevator ");
					strcpy(tempstr,"");
					sprintf(tempstr,"%d",my_elevator->id);
					strcat(strout,tempstr);
					strcat(strout," picks up ");
					//printf("%f elevator %d picks up ",(double)(time(NULL)-beginning_time),my_elevator->id);
				}
				strcpy(tempstr,"");
				sprintf(tempstr,"%d",temp_node->one_person->id);
				strcat(strout,"Person ");
				strcat(strout,tempstr);
				//printf("Person %d",temp_node->one_person->id);
				if((temp_node = temp_node->next) != NULL){
					strcat(strout,", ");
					//printf(", ");
				}else{
					strcat(strout," at floor ");
					strcpy(tempstr,"");
					sprintf(tempstr,"%d",my_elevator->current_floor);
					strcat(strout,tempstr);
				}
				j++;
			}
			if((double)(time(NULL)-beginning_time) > simulation_time){
				flag = 0;
				ClearListnofree(my_elevator->ppickup);
				ClearList(my_elevator->pdrops);
				break;	
			}
			if(strcmp(strout,"") != 0){
				printf("%s\n",strout);
			}
			temp_node = my_elevator->drops;
			j = 0;
			strcpy(strout,"");
			strcpy(tempstr,"");
			while(temp_node != NULL){
				if(j == 0){	
					double d = (double)(time(NULL) - beginning_time);
					strcpy(tempstr,"");
					sprintf(tempstr,"%.6lf",d);
					strcat(strout,tempstr);
					strcat(strout," elevator ");
					strcpy(tempstr,"");
					sprintf(tempstr,"%d",my_elevator->id);
					strcat(strout,tempstr);
					strcat(strout," drops ");
					//printf("%f elevator %d drops ",(double)(time(NULL)-beginning_time),my_elevator->id);
				}
				strcpy(tempstr,"");
				sprintf(tempstr,"%d",temp_node->one_person->id);
				strcat(strout,"Person ");
				strcat(strout,tempstr);
				//printf("Person %d",temp_node->one_person->id);
				if((temp_node = temp_node->next) != NULL){
					strcat(strout,", ");
					//printf(", ");
				}
				else{
					strcat(strout," at floor ");
					strcpy(tempstr,"");
					sprintf(tempstr,"%d",my_elevator->current_floor);
					strcat(strout,tempstr);	
				}
				j++;
			}
			P(&mutex);
			num_people_finished = num_people_finished + j;
			V(&mutex);
			if((double)(time(NULL)-beginning_time) > simulation_time){
				flag = 0;
				ClearListnofree(my_elevator->ppickup);
				ClearList(my_elevator->pdrops);
				break;
			}
			if(strcmp(strout,"")!=0){
				printf("%s\n",strout);
			}
			if(i >= (stations)){
				ClearListnofree(my_elevator->ppickup);
				ClearList(my_elevator->pdrops);
				break;
			}
			my_elevator->next_floor = *(floor_order + i);
			printf("%f elevator %d starts moving from floor %d to %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->current_floor,my_elevator->next_floor);
			spend = (abs(my_elevator->next_floor - my_elevator->current_floor))/(double)elevator_speed;
			sleep(spend);
			if((double)(time(NULL)-beginning_time) > simulation_time){
				flag = 0;	
				ClearListnofree(my_elevator->ppickup);
				ClearList(my_elevator->pdrops);
				break;
			}
			my_elevator->current_floor = my_elevator->next_floor;
			printf("%f elevator %d arrives floor %d.\n",(double)((double)time(NULL)-beginning_time),my_elevator->id,my_elevator->current_floor);
			ClearListnofree(my_elevator->ppickup);
			ClearList(my_elevator->pdrops);
		}
		free(floor_order);
		ClearList(my_elevator->pele_head);
		
	}

	return;

}

//people thread function
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
		if(a_person->from_floor >= a_person->to_floor){
			a_person->dir = 0;
		}
		else{
			a_person->dir = 1;
		}
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

//init semaphore
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
		//(elevators+i)->people = NULL;
		(elevators+i)->next_floor = 0;
		(elevators+i)->ele_head = NULL;
		(elevators+i)->pele_head = &((elevators+i)->ele_head);
		(elevators+i)->dir = 1;
		(elevators+i)->pickup = NULL;
		(elevators+i)->ppickup = &((elevators+i)->pickup);
		(elevators+i)->drops = NULL;
		(elevators+i)->pdrops = &((elevators+i)->drops);
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
