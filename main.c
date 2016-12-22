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
                printf("%d got Tobacco\n", id);
                sem_post(&sAgent);
                break;

            case 1: // Paper
                sem_wait(&sPaper);
                printf("%d got Paper\n", id);
                sem_post(&sAgent);
                break;

            case 2: // Match
                sem_wait(&sMatch);
                printf("%d got Match\n", id);
                sem_post(&sAgent);
                break;

            default: // Error case
                perror("ERROR: Invalid argument ID: %d\n", id);
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
                puts("Agent set Tobacco");
                sem_post(&sTobacco);
                break;

            case 1: // Paper
                puts("Agent set Paper");
                sem_post(&sPaper);
                break;

            case 2: // Match
                puts("Agent set Match");
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

    /** Craete agent */
    pthread_create(&tAgent, NULL, agent, NULL);

    /** Wait for smokers */
    for (int i = 0; i < SMOKERS; i++)
        pthread_join(tSmokers[i], NULL);

    /** Wait for agent */
    pthread_join(tAgent, NULL);

    exit(0);
}
