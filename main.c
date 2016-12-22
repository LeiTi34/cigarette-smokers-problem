/************************************/
/* Author: Alex Leidwein            */
/* Name: Cigarette Smokers Problem  */
/* Revision: 1.1                    */
/* Date: 22.12.2016                 */
/************************************/

/** Includes */
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

/** Defines */
#define SMOKERS 3

/** Semaphores */
sem_t sTobacco, sPaper, sMatch;
sem_t sAgent;

/** Bools for items */
int Tobacco = 0;
int Paper = 0;
int Match = 0;

/*
 * Smoker waits for the missing Item
 * Argument wait for:
 * - 0 ... Tobacco
 * - 1 ... Paper
 * - 2 ... Match
 */
void *smoker(void * arg)
{
  int id = *(int*)arg; // Convert argument to int

  for(;;)
  {
    switch(id)
    {
      case 0: // Tobacco
        sem_wait(&sTobacco);
        if(Paper > 0 && Match > 0)  //Check if Items are avalible
        {
          Paper = 0;
          Match = 0;
          printf("%d: Got paper and match\n", id);
          sem_post(&sAgent);
        }
        else printf("%d: Error: Paper or match is not avalible!\n", id);
        break;

      case 1: // Paper
        sem_wait(&sPaper);
        if(Tobacco > 0 && Match > 0)  //Check if Items are avalible
        {
          Tobacco = 0;
          Match = 0;
          printf("%d: Got tobacco and match\n", id);
          sem_post(&sAgent);
        }
        else printf("%d: Error: Tobacco or match is not avalible!\n", id);
        break;

      case 2: // Match
        sem_wait(&sMatch);
        if(Tobacco > 0 && Paper > 0)  //Check if Items are avalible
        {
          Tobacco = 0;
          Paper = 0;
          printf("%d: Got tobacco and paper\n", id);
          sem_post(&sAgent);
        }
        else printf("%d: Error: Tobacco or paper is not avalible!\n", id);
        break;

      default: // Error case
        printf("ERROR: Invalid argument ID: %d\n", id);
        exit(1);
        break;
    }
  }
}

/*
 * Agent sets a random Item
 */
void *agent(void * arg)
{
  for(;;)
  {
    sem_wait(&sAgent);  // Wait for smoker

    usleep(1000000);  // Delay for better visability

    int id = rand() % 3;  // Generate a random ID for a random item

    switch(id)
    {
      case 0: // Tobacco
        puts("Agent set paper and match");
        Paper++;
        Match++;
        sem_post(&sTobacco);
        break;

      case 1: // Paper
        puts("Agent set tobacco and match");
        Tobacco++;
        Match++;
        sem_post(&sPaper);
        break;

      case 2: // Match
        puts("Agent set tobacco and paper");
        Tobacco++;
        Paper++;
        sem_post(&sMatch);
        break;

      default: // Error case
        printf("ERROR wrong Agent ID: %d\n", id);
        break;
    }
  }
}

/*
 * Main
 */
int main(int argc, char* argv[])
{
  /** Init semaphores */
  sem_init(&sTobacco, 0, 0);
  sem_init(&sPaper, 0, 0);
  sem_init(&sMatch, 0, 0);

  sem_init(&sAgent, 0, 1);

  /** Define threads */
  pthread_t tSmokers[SMOKERS], tAgent;

  int smokersID[SMOKERS];

  /** Create smokers */
  for(int i = 0; i < SMOKERS; i++)
  {
    smokersID[i] = i;
    pthread_create(&tSmokers[i], NULL, smoker, &smokersID[i]);
  }
  puts("Smokers created");

  /** Craete agent */
  pthread_create(&tAgent, NULL, agent, NULL);
  puts("Agent created");

  /** Wait for smokers */
  for (int i = 0; i < SMOKERS; i++)
    pthread_join(tSmokers[i], NULL);

  /** Wait for agent */
  pthread_join(tAgent, NULL);

  exit(0);
}
