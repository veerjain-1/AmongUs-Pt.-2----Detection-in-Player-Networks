#include "PlayerDetect.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int g_player_count = 0;
player_t g_player_array[MAX_PLAYERS] = {0};

static void print_player(player_t player) {
  printf("\tusername: %s\n", player.username);
  printf("\tsecret: %s\n", player.secret);
  printf("\tfriend\ttrust\tconnected:\n");
  for (int i = 0; i < MAX_FRIENDS; i++) {
    if (*player.friends[i] != 0) {
      printf(
        "\t%s\t%d\t%s\n", player.friends[i], player.trust[i],
        player.friend_ptrs[i]->username
      );
    }
  }
}

int main() {
  int choice = 0;
  bool good_file = false;
  while (true) {
    printf(
      "Select a function to test:\n"
      "    1) read_players()\n"
      "    2) connect_players()\n"
      "    3) length_of_max_trust_friend_path()\n"
      "    4) imposters_in_min_trust_friend_path()\n"
      "    5) print players\n"
      "    6) exit\n"
      "Enter your choice: "
    );

    scanf("%d", &choice);
    getchar();

    char in_file[MAX_FILE_LEN] = {0};
    char username[MAX_NAME_LEN] = {0};
    player_t player = {0};

    switch (choice) {
      case 1:
        good_file = false;

        printf("\nEnter the input file name: ");
        scanf("%s", in_file);

        int num_players = read_players(in_file);
        printf("\nread_players(%s) returned %d.\n\n", in_file, num_players);

        if (num_players > 1) {
          good_file = true;
        }
        break;

      case 2:
        if (!good_file) {
          printf("\ng_player_array is not loaded. Use read_players() first!\n\n");
          break;
        }

        connect_players();
        printf("\nPlayers successfully connected.\n\n");
        break;

      case 
