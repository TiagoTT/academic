#include<stdio.h>
#include <pthread.h>



int main(){

	int i;
	int nprocess =3;
	void *funcao1(void* arg){
       		/* processo1*/	
		int x;
		x=1;
		printf( "processo1\n");
		return NULL;
	}
	void *funcao2(void* arg){
		/* processo2*/
		int y;
		y=2;
		printf( "processo2\n");
		return NULL;
	}
	void *funcaon(void* arg){
		/* processo n*/
		int w;
		w=3;
		printf( "processo3\n");
		return NULL;
	}
	
	
	
	{
	/*
 	* int  pthread_create(pthread_t  *  thread, pthread_attr_t * attr,
 	*  void * (*start_routine)(void *), void * arg);
	*/
	
		pthread_t threads[ 3 ];

		if( pthread_create( &threads[0], NULL, funcao1, NULL) != 0){
			printf(" Erro a criar thread = 1\n");
		
		}
		if( pthread_create( &threads[1], NULL, funcao2, NULL) != 0){
                        printf(" Erro a criar thread = 2\n");
		}
		if( pthread_create( &threads[2], NULL, funcaon, NULL) != 0){
			printf(" Erro a criar thread = 3\n");
		}
	
	/*
 	* int pthread_join(pthread_t th, void **thread_return);
 	*
 	*/
	
		for(i=0; i< nprocess; i++){
			pthread_join(threads[i], NULL);
		
		}

	}
	

	return 0;
}
