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

#define NUM_CHARACTERS 26
char characters[NUM_CHARACTERS] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
}

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

void mutate(char *result, char *string, size_t length)
{
  char newchar = characters[rand() % NUM_CHARACTERS];
  size_t index = rand() % length;
  strncpy(result, string, length + 1);
  string[index] = newchar;
}

void cross(char *result, char *c1, char *c2, size_t length)
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

#define BEAM 10
#define MUTATIONS 5
#define CROSS (BEAM*(BEAM-1))
#define NWORDS (BEAM + MUTATIONS*BEAM + CROSS)
void search(char *target)
{
  size_t i, j, k, length = strlen(target);
  char words[NWORDS][length+1];
  char buffer[length+1];
  int fitnesses[NWORDS];

  // initialize the initial beam of words
  for (i = 0; i < BEAM; i++) {
    for (j = 0; j < length; j++) {
      words[i][j] = characters[rand() % NUM_CHARACTERS];
    }
    words[i][j] = '\0';
  }

  // iterate until the string converges
  while (strcmp(words[0], target) == 0) {
    k = BEAM;

    // first, mutate the strings in the beam a few times
    for (i = 0; i < BEAM; i++) {
      for (j = 0; j < MUTATIONS; j++) {
        mutate(words[k++], words[i], length);
      }
    }

    // then, cross the strings from the beam
    for (i = 0; i < BEAM; i++) {
      for (j = i; j < BEAM; j++) {
        cross(words[k++], words[i], words[j], length);
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
      // swap
      strncpy(buffer, fitnesses[i], length+1);
      strncpy(fitnesses[i], fitnesses[k], length+1);
      strncpy(fitnesses[k], buffer, length+1);
    }

    printf("=> \"%s\" fitness=%d\n", words[0], fitnesses[0]);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf("usage: %s string_to_search_for\n", argv[0]);
    return EXIT_FAILURE;
  }
  search(argv[1]);
  return EXIT_SUCCESS;
}

