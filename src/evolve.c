/***************************************************************************//**

  @file         evolve.c

  @author       Stephen Brennan

  @date         Created Wednesday, 22 July 2015

  @brief        Evolutionary algorithm to search for a string.

  @copyright    Copyright (c) 2015, Stephen Brennan.  Released under the Revised
                BSD License.  See LICENSE.txt for details.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#define BEAM 100
#define MUTATIONS 50
#define CROSS (BEAM*(BEAM-1)/2)
#define NWORDS (BEAM + MUTATIONS*BEAM + CROSS)

/*
   Get a random character.  By changing this, you can make it do only
   alphabetical characters or something.  That would make the search more
   efficient.
 */
char random_character()
{
  return (char) (rand() % (CHAR_MAX - CHAR_MIN) + CHAR_MIN);
}

/*
  Evaluate the fitness of a string by comparing it to the target.  The fitness
  is defined as the number of characters it has in common with the target.
 */
int fitness(char *candidate, char *target)
{
  int fit = 0;

  while (*candidate != '\0' && *target != '\0') {
    if (*candidate++ == *target++) {
      fit++;
    }
  }

  return fit;
}

/*
  "Mutate" a string, by randomly changing one of its characters.
 */
void mutate(char *result, char *string, size_t length)
{
  char newchar = random_character();
  size_t index = rand() % length;
  strncpy(result, string, length + 1);
  result[index] = newchar;
}

/*
  "Cross" two strings, so that the new one inherits qualities of both.
 */
void cross(char *result, char *c1, char *c2)
{
  while (*c1 != '\0' && *c2 != '\0') {
    if (rand() % 2 == 0) {
      *result = *c1;
    } else {
      *result = *c2;
    }
    result++; c1++; c2++;
  }
  *result = '\0';
}

/*
  Search for a target string by initializing a population of random strings, and
  iterating through "generations" of new ones.  New generations are created by
  mutating and crossing over the parents of the generation.  The parents of the
  next generation are determined by choosing the most "fit" individuals from the
  current population.
 */
int search(char *target)
{
  size_t i, j, k, length = strlen(target);
  char words[NWORDS][length+1];
  char buffer[length+1];
  int fitnesses[NWORDS];
  int fit, generation = 0;;

  // initialize the initial beam of words
  for (i = 0; i < BEAM; i++) {
    for (j = 0; j < length; j++) {
      words[i][j] = random_character();
    }
    words[i][j] = '\0';
  }

  // iterate until the string converges
  while (strcmp(words[0], target) != 0) {
    k = BEAM;

    // first, mutate the strings in the beam a few times
    for (i = 0; i < BEAM; i++) {
      for (j = 0; j < MUTATIONS; j++) {
        mutate(words[k++], words[i], length);
      }
    }

    // then, cross the strings from the beam
    for (i = 0; i < BEAM; i++) {
      for (j = i+1; j < BEAM; j++) {
        cross(words[k++], words[i], words[j]);
      }
    }

    // rank all the fitnesses
    for (i = 0; i < NWORDS; i++) {
      fitnesses[i] = fitness(words[i], target);
    }

    // finally, do selection sort to get the best (we only need the top BEAM, so
    // it's a reasonable choice).
    for (i = 0; i < BEAM; i++) {
      k = i;
      for (j = i; j < NWORDS; j++) {
        if (fitnesses[j] > fitnesses[i]) {
          k = j;
        }
      }
      if (i == k) {
        continue;
      }
      // swap
      strncpy(buffer, words[i], length+1);
      fit = fitnesses[i];
      strncpy(words[i], words[k], length+1);
      fitnesses[i] = fitnesses[k];
      strncpy(words[k], buffer, length+1);
      fitnesses[k] = fit;
    }
    generation++;
    printf("generation %d: fitness %d\n", generation, fitnesses[0]);
  }
  return generation;
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "usage: %s string_to_search_for\n", argv[0]);
    return EXIT_FAILURE;
  }
  srand(time(NULL));
  printf("%d\n", search(argv[1]));
  return EXIT_SUCCESS;
}
