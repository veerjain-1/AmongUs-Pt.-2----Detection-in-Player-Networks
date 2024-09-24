#include "DetectPlayer.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *g_check = NULL;

bool has_duplicates() {
  for (int i = 0; i < g_player_count; ++i) {
    int count = 1;
    for (int j = 0; j < g_player_count; ++j) {
      if ((i != j) && (strcmp(g_player_array[i].username, g_player_array[j].username) == 0)) {
        count++;
        if (count >= 2) {
          return true;
        }
      }
    }
  }
  return false;
}

int is_visited(player_t* visited[], int visited_count, player_t* player) {
  for (int i = 0; i < visited_count; i++) {
    if (visited[i] == player) {
      return 1;
    }
  }
  return 0;
}

int is_imposter(player_t* player) {
  return strstr(player->secret, "amongus") != NULL;
}

int normal_characters(const char *str) {
  for (int i = 0; str[i]; i++) {
    if (!isalpha(str[i])) {
      return 0;
    }
  }
  return 1;
}

char* strtoke(char *str, const char *delim) {
  char *token = NULL;
  if (str) g_check = str;
  if (!g_check) return NULL;
  token = g_check;
  g_check = strpbrk(g_check, delim);
  if (g_check) {
    *g_check++ = '\0';
  }
  return token;
}

int unique_username(const char *str) {
  return strlen(str) <= MAX_NAME_LEN && normal_characters(str);
}

int get_lines(FILE *file) {
  char buffer[MAX_BUFF_LEN];
  int count = 0;
  while (fgets(buffer, MAX_BUFF_LEN, file)) {
    count++;
  }
  return count;
}

int imposters_in_min_trust_friend_path(player_t *player, char imposters[MAX_PLAYERS][MAX_NAME_LEN]) {
  int imposter_count = 0;
  player_t* current_player = player;
  player_t* next_player = NULL;
  player_t* visited[MAX_PLAYERS] = {NULL};
  int visited_count = 0;

  while (current_player != NULL) {
    visited[visited_count++] = current_player;
    if (is_imposter(current_player)) {
      strcpy(imposters[imposter_count++], current_player->username);
    }
    int min_trust = INT_MAX;
    int min_trust_index = -1;

    for (int i = 0; i < MAX_FRIENDS; i++) {
      if ((current_player->friend_ptrs[i] != NULL) && (current_player->trust[i] < min_trust)) {
        min_trust = current_player->trust[i];
        min_trust_index = i;
      }
    }

    if (min_trust_index == -1) {
      break;
    }

    next_player = current_player->friend_ptrs[min_trust_index];

    if (is_visited(visited, visited_count, next_player)) {
      return PATH_IS_CYCLE;
    }

    current_player = next_player;
  }

  return imposter_count;
}
