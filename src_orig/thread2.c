/* pthread.c by detour@metalshell.com
 *
 * Create multiple POSIX thread processes and wait for
 * each one to complete.
 *
 * http://www.metalshell.com/
 *
 */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 10

/* This becomes the main() of the new thread.  It
   is requred to return void * and take void * as a
   parameter. */
void *
thread_function (void *arg)
{
  fprintf (stdout, "Thread: %d running.\n", (int) arg);

  // Sleep for arg+1 seconds to verify pthread_join.
  sleep ((int) arg + 1);

  fprintf (stdout, "Thread: %d done.\n", (int) arg);

  pthread_exit (0);
}

int
main ()
{
  int cnt;
  pthread_t p_thread[NUM_THREADS];

  for (cnt = 0; cnt < NUM_THREADS; cnt++)
    /* Returns 0 on successful creation of the thread.  The second
       parameter, left NULL for this example,  allows for options
       like CPU scheduling to be set. */
    if (pthread_create (&p_thread[cnt], NULL, thread_function, (void *) cnt)
	!= 0)
      fprintf (stderr, "Error creating the thread");

  // Cycle through each thread and wait until it is completed.
  for (cnt = 0; cnt < NUM_THREADS; cnt++)
    {
      // Waits for p_thread[cnt] to finish.
      pthread_join (p_thread[cnt], NULL);
    }

  fprintf (stdout, "All threads completed.\n");

  return 0;
}
