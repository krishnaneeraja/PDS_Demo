#define _REENTRANT
#include <stdio.h>
#include <pthread.h>

/* Function prototypes for thread routines */
void *sub_a (void *);
void *sub_b (void *);
void *sub_c (void *);
void *sub_d (void *);
void *sub_e (void *);
void *sub_f (void *);

pthread_t thr_a, thr_b, thr_c;

void
main ()
{
  pthread_t main_thr;

  main_thr = thr_self ();
  printf ("Main thread = %d\n", main_thr);

  if (thr_create (NULL, 0, sub_b, NULL, THR_SUSPENDED | THR_NEW_LWP, &thr_b))
    fprintf (stderr, "Can't create thr_b\n"), exit (1);

  if (thr_create (NULL, 0, sub_a, (void *) thr_b, THR_NEW_LWP, &thr_a))
    fprintf (stderr, "Can't create thr_a\n"), exit (1);

  if (thr_create (NULL, 0, sub_c, (void *) main_thr, THR_NEW_LWP, &thr_c))
    fprintf (stderr, "Can't create thr_c\n"), exit (1);

  printf ("Main Created threads A:%d B:%d C:%d\n", thr_a, thr_b, thr_c);
  printf ("Main Thread exiting...\n");
  thr_exit ((void *) main_thr);
}

void *
sub_a (void *arg)
{
  pthread_t thr_b = (pthread_t) arg;
  pthread_t thr_d;
  int i;

  printf ("A: In thread A...\n");

  if (thr_create (NULL, 0, sub_d, (void *) thr_b, THR_NEW_LWP, &thr_d))
    fprintf (stderr, "Can't create thr_d\n"), exit (1);

  printf ("A: Created thread D:%d\n", thr_d);

/* process 
*/
  for (i = 0; i < 1000000 * (int) thr_self (); i++);
  printf ("A: Thread exiting...\n");
  thr_exit ((void *) 77);
}

void *
sub_b (void *arg)
{
  int i;

  printf ("B: In thread B...\n");

/* process 
*/

  for (i = 0; i < 1000000 * (int) thr_self (); i++);
  printf ("B: Thread exiting...\n");
  thr_exit ((void *) 66);
}

void *
sub_c (void *arg)
{
  void *status;
  int i;
  pthread_t main_thr, ret_thr;

  main_thr = (pthread_t) arg;

  printf ("C: In thread C...\n");

  if (thr_create (NULL, 0, sub_f, (void *) 0, THR_BOUND | THR_DAEMON, NULL))
    fprintf (stderr, "Can't create thr_f\n"), exit (1);

  printf ("C: Join main thread\n");

  if (thr_join (main_thr, (pthread_t *) & ret_thr, &status))
    fprintf (stderr, "thr_join Error\n"), exit (1);

  printf ("C: Main thread (%d) returned thread (%d) w/status %d\n", main_thr,
	  ret_thr, (int) status);

/* process 
*/

  for (i = 0; i < 1000000 * (int) thr_self (); i++);
  printf ("C: Thread exiting...\n");
  thr_exit ((void *) 88);
}


void *
sub_d (void *arg)
{
  pthread_t thr_b = (pthread_t) arg;
  int i;
  pthread_t thr_e, ret_thr;
  void *status;

  printf ("D: In thread D...\n");

  if (thr_create (NULL, 0, sub_e, NULL, THR_NEW_LWP, &thr_e))
    fprintf (stderr, "Can't create thr_e\n"), exit (1);

  printf ("D: Created thread E:%d\n", thr_e);
  printf ("D: Continue B thread = %d\n", thr_b);

  thr_continue (thr_b);
  printf ("D: Join E thread\n");

  if (thr_join (thr_e, (pthread_t *) & ret_thr, &status))
    fprintf (stderr, "thr_join Error\n"), exit (1);

  printf ("D: E thread (%d) returned thread (%d) w/status %d\n", thr_e,
	  ret_thr, (int) status);

/* process 
*/

  for (i = 0; i < 1000000 * (int) thr_self (); i++);
  printf ("D: Thread exiting...\n");
  thr_exit ((void *) 55);
}


void *
sub_e (void *arg)
{
  int i;
  pthread_t ret_thr;
  void *status;

  printf ("E: In thread E...\n");
  printf ("E: Join A thread\n");

  if (thr_join (thr_a, (pthread_t *) & ret_thr, &status))
    fprintf (stderr, "thr_join Error\n"), exit (1);

  printf ("E: A thread (%d) returned thread (%d) w/status %d\n", ret_thr,
	  ret_thr, (int) status);
  printf ("E: Join B thread\n");

  if (thr_join (thr_b, (pthread_t *) & ret_thr, &status))
    fprintf (stderr, "thr_join Error\n"), exit (1);

  printf ("E: B thread (%d) returned thread (%d) w/status %d\n", thr_b,
	  ret_thr, (int) status);
  printf ("E: Join C thread\n");

  if (thr_join (thr_c, (pthread_t *) & ret_thr, &status))
    fprintf (stderr, "thr_join Error\n"), exit (1);

  printf ("E: C thread (%d) returned thread (%d) w/status %d\n", thr_c,
	  ret_thr, (int) status);

  for (i = 0; i < 1000000 * (int) thr_self (); i++);

  printf ("E: Thread exiting...\n");
  thr_exit ((void *) 44);
}


void *
sub_f (void *arg)
{
  int i;

  printf ("F: In thread F...\n");

  while (1)
    {
      for (i = 0; i < 10000000; i++);
      printf ("F: Thread F is still running...\n");
    }
}
