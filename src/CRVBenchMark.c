/*
 ============================================================================
 Name        : CRVBenchMark.c
 Author      : Yogi Joshi
 Version     : 1.0
 Note 		 : * Concrete and Abstract events are same in case of below program
 	 	 	   * Event makes corresponding predicate true
 	 	 	     For example:
 	 	 	     Event pthread_running(100) makes pthread_running(100) = true
 	 	 	   * Main thread is not counted while evaluating the Quantifiers
 	 	 	     of FO formula
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
const int THREAD_COUNT=100000;
pthread_mutex_t ex_mutex;
void *threadFunc(void *threadId)
{
	int tid = (int)(threadId);
	fprintf(stdout,"Performing task inside thread %d\n",tid);
	/** Above line generates event pthread_running(tid) where tid is threadId**/
	if(tid % 2 == 0)
	{
		pthread_mutex_lock(&ex_mutex);
		/** above line generates event pthread_mutex_lock(tid, 'ex_mutex')
		 * where tid is threadId and 'ex_mutex' is constant in FOL**/
		if(tid % 50 == 0)
		{
			pthread_mutex_destroy(&ex_mutex);
			/** above line generates event pthread_mutex_destroy(tid, 'ex_mutex')
			 *  * where tid is threadId and 'ex_mutex' is constant in FOL**/
			pthread_exit(NULL);
			/** above line generates event pthread_exit(tid)
			* where tid is threadId **/
		}
	}
	pthread_mutex_unlock(&ex_mutex);
	/** above line generates event pthread_mutex_unlock(tid, 'ex_mutex')
	* where tid is threadId and 'ex_mutex' is constant in FOL**/

	pthread_exit(NULL);
	/** above line generates event pthread_exit(tid)
	* where tid is threadId **/
}
int main(void) {
	int idStore[THREAD_COUNT];
	int i; void *status;
	pthread_t threads[THREAD_COUNT];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_mutex_init(&ex_mutex, NULL);
	/** above line generates event pthread_mutex_init(10000, 'ex_mutex') **/

	for( i = 0; i < THREAD_COUNT;i++)
	{
		idStore[i] = i;
		pthread_create(&threads[i], &attr,threadFunc,idStore[i]);
		/** above line generates event pthread_create(tid) where tid is threadId **/
	}
	for( i = 1; i < THREAD_COUNT;i++)
	{
		pthread_join(threads[i], &status);
		/** above line generates event pthread_join(tid) where tid is threadId **/
	}

	//pthread_mutex_destroy(&ex_mutex);

	return EXIT_SUCCESS;
}
