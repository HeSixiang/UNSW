/* This file will contain your solution. Modify it as you wish. */
#include <types.h>
#include <lib.h>
#include <synch.h>
#include "producerconsumer_driver.h"

/* Declare any variables you need here to keep track of and
   synchronise your bounded. A sample declaration of a buffer is shown
   below. It is an array of pointers to items.

   You can change this if you choose another implementation.
   However, you should not have a buffer bigger than BUFFER_SIZE
*/

data_item_t * item_buffer[BUFFER_SIZE] = {NULL};

/* Semaphores which the simulator uses to determine the item buffer
 * full or emepty 
 */
struct semaphore *is_full;
struct semaphore *is_empty;

// Semaphore which the simulator uses to do mutual exclusion
struct semaphore *mutex;

//two integers to store the index of array in order to make queue(FIFO)
volatile unsigned long int get;
volatile unsigned long int put;

/* consumer_receive() is called by a consumer to request more data. It
   should block on a sync primitive if no data is available in your
   buffer. It should not busy wait! */

data_item_t * consumer_receive(void)
{
   data_item_t * item = NULL;
   P(is_full);
   P(mutex);
   item = item_buffer[get];
   item_buffer[get] = NULL;
   if (++get == BUFFER_SIZE) {
      get = 0;
   }
   V(mutex);
   V(is_empty);

   KASSERT(item != NULL);

   return item;
}

/* procucer_send() is called by a producer to store data in your
   bounded buffer.  It should block on a sync primitive if no space is
   available in your buffer. It should not busy wait!*/

void producer_send(data_item_t *item)
{
   P(is_empty);
   P(mutex);

   KASSERT(item_buffer[put] == NULL);

   item_buffer[put] = item;
   if (++put == BUFFER_SIZE) {
      put = 0;
   }
   V(mutex);
   V(is_full);
}

/* Perform any initialisation (e.g. of global data) you need
   here. Note: You can panic if any allocation fails during setup */

void producerconsumer_startup(void)
{
   is_full = sem_create("the_number_of_items_in_buffer", 0);
   is_empty = sem_create("the_number_of_spaces_in_buffer", BUFFER_SIZE);
   mutex = sem_create("change_the_buffer", 1);
   if(!is_full || !is_empty || !mutex) {
      panic("producerconsumer: couldn't create semaphore\n");
   }
   get = 0;
   put = 0;
}

/* Perform any clean-up you need here */
void producerconsumer_shutdown(void)
{
   sem_destroy(is_full);
   sem_destroy(is_empty);
   sem_destroy(mutex);
}
