#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

class Tile {
public:
  int x = 0;
  int y = 0;
  int width = 135;
  int height = 135;
  Color color;
  bool playable;
  Rectangle tile_rec = {x, y, width, height};

  void Draw() {

    // cout << " params: " << x << ", " << y << "\n";
    DrawRectangle(x, y, width, height, color);
    DrawRectangleLines(x, y, 135, 135, BLACK);
  }
};

class Piece {
public:
  bool is_king = false;
  bool is_selected = false;
  bool is_removed = false;
  int x = 0;
  int y = 0;
  Color color;
  Color lineColor;
  int radius = 30;

  void Draw() {
    // cout << " params: " << x << ", " << y << "\n";
    DrawCircle(x, y, radius, color);
    if (is_king) {
      DrawCircleLines(x, y, 5, GOLD);
      DrawCircleLines(x, y, 10, GOLD);
      DrawCircleLines(x, y, 15, GOLD);
      DrawCircleLines(x, y, 20, GOLD);
      DrawCircleLines(x, y, 30, GOLD);

    } else {

      DrawCircleLines(x, y, 5, lineColor);
      DrawCircleLines(x, y, 15, lineColor);
      DrawCircleLines(x, y, 30, lineColor);
    }
  }
};

void SetupPieces(Tile tiles[], Piece player_pieces[], int num_tiles,
                 int num_pieces, bool is_player_1) {

  if (is_player_1) {
    int tile_num = 0;
    for (int piece_num = 0; piece_num < num_pieces; tile_num++) {
      if (tiles[tile_num].playable) {
        player_pieces[piece_num].x = tiles[tile_num].x + 135 / 2;
        player_pieces[piece_num].y = tiles[tile_num].y + 135 / 2;
        player_pieces[piece_num].color = RED;
        player_pieces[piece_num].lineColor = BLACK;
        piece_num++;
      }
    }
  }

  if (!is_player_1) {
    // start from the back minus 1 for index
    int tile_num = num_tiles - 1;
    for (int piece_num = 0; piece_num < num_pieces; tile_num--) {
      if (tiles[tile_num].playable) {
        player_pieces[piece_num].x = tiles[tile_num].x + 135 / 2;
        player_pieces[piece_num].y = tiles[tile_num].y + 135 / 2;
        player_pieces[piece_num].color = BLUE;
        player_pieces[piece_num].lineColor = BLACK;
        piece_num++;
      }
    }
  }
}

void SetupBoard(Tile tiles[], int num_tiles) {
  int row;
  int column;
  int num_columns = 8;
  int num_rows = 8;
  int tile_num = 0;

  for (row = 0; row < num_rows; row++) {
    for (column = 0; column < num_columns; column++) {
      int tile_num = row * num_columns + column;
      tiles[tile_num].y = (135 * row);
      tiles[tile_num].x = (135 * column);

      // This sets the playable tiles and their colors for even rows
      if (row % 2 == 0) {
        if (column % 2 == 0) {
          tiles[tile_num].color = DARKGREEN;
          tiles[tile_num].playable = true;
        } else {
          tiles[tile_num].color = LIGHTGRAY;
          tiles[tile_num].playable = false;
        }
      }

      // This sets the playable tiles and their colors for odd rows
      if (row % 2 == 1) {
        if (column % 2 == 1) {
          tiles[tile_num].color = DARKGREEN;
          tiles[tile_num].playable = true;
        } else {
          tiles[tile_num].color = LIGHTGRAY;
          tiles[tile_num].playable = false;
        }
      }
    }
  }
}

void PrintCredits() {}

bool HasNoPieceAlready(Tile selected_tile, Piece player_1_pieces[],
                       Piece player_2_pieces[], int num_pieces) {
  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
    if (player_1_pieces[piece_num].x == selected_tile.x + 67 &&
        player_1_pieces[piece_num].y == selected_tile.y + 67) {
      cout << "false!\n";
      return false;
    }

    if (player_2_pieces[piece_num].x == selected_tile.x + 67 &&
        player_2_pieces[piece_num].y == selected_tile.y + 67) {
      cout << "false!\n";
      return false;
    }
  }
  cout << "true!\n";
  return true;
}

bool CheckWin(Piece player_1_pieces[], Piece player_2_pieces[],
              int num_pieces) {
  bool player_1_out = true;
  bool player_2_out = true;
  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
    if (!player_1_pieces[piece_num].is_removed) {
      player_1_out = false;
    }
    if (!player_2_pieces[piece_num].is_removed) {
      player_2_out = false;
    }
  }
  if (!player_1_out && !player_2_out) {
    return false;
  } else {
    return true;
  }
}

void RemovePiece(Piece *piece_to_remove) {
  cout << "trying to remove!\n";
  piece_to_remove->x = 3000;
  piece_to_remove->y = 3000;
  piece_to_remove->is_removed = true;
}

void UpdateMouse() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    DrawCircleV(GetMousePosition(), 30, RED);
  }
}

int main() {

  int num_tiles = 64;
  int num_pieces = 12;
  Tile tiles[num_tiles];
  Piece player_1_pieces[num_pieces];
  Piece player_2_pieces[num_pieces];
  const int screen_width = 1440;
  const int screen_height = 1080;
  bool is_player_1_turn = true;
  bool player_1_won = false;
  bool player_2_won = false;
  bool is_draw = false;
  Piece *selectedPiece = nullptr;
  Tile *selectedTile = nullptr;
  Piece *piece_to_remove = nullptr;
  Rectangle draw = {1100, 300, 320, 50};
  Rectangle cant_move = {1100, 400, 320, 50};

  InitWindow(screen_width, screen_height, "Checkers");
  SetTargetFPS(60);

  SetupBoard(tiles, num_tiles);
  SetupPieces(tiles, player_1_pieces, num_tiles, num_pieces, true);
  SetupPieces(tiles, player_2_pieces, num_tiles, num_pieces, false);
  while (WindowShouldClose() == false) {

    // Logic

    if (is_player_1_turn) {

      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
          Vector2 piece_pos = {player_1_pieces[piece_num].x,
                               player_1_pieces[piece_num].y};
          // radius of a piece is always 30
          if (CheckCollisionPointCircle(GetMousePosition(), piece_pos, 30)) {
            if (selectedPiece == nullptr) {
              selectedPiece = &player_1_pieces[piece_num];
              selectedPiece->color = MAROON;
              selectedPiece->lineColor = WHITE;
            } else {
              selectedPiece->color = RED;
              selectedPiece->lineColor = BLACK;
              selectedPiece = nullptr;
            }
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (selectedPiece) {
          Vector2 move_pos = GetMousePosition();

          for (int tile_num = 0; tile_num < num_tiles; tile_num++) {

            Rectangle tile_rec = {tiles[tile_num].x, tiles[tile_num].y, 135,
                                  135};

            if (CheckCollisionPointRec(move_pos, tile_rec)) {
              selectedTile = &tiles[tile_num];
              cout << selectedTile->x << ", " << selectedTile->y << "\n";
            }
          }
        }
      }

      if (selectedPiece && selectedPiece->is_king && selectedTile) {

        if (selectedTile->playable)
          if (selectedTile->x != selectedPiece->x - 67)
            if (HasNoPieceAlready(*selectedTile, player_1_pieces,
                                  player_2_pieces, num_pieces))
              if (selectedTile->x == selectedPiece->x + 68 &&
                      selectedTile->y == selectedPiece->y + 68 ||
                  selectedTile->x == selectedPiece->x - 202 &&
                      selectedTile->y == selectedPiece->y + 68 ||
                  selectedTile->x == selectedPiece->x - 202 &&
                      selectedTile->y == selectedPiece->y - 202 ||
                  selectedTile->x == selectedPiece->x + 68 &&
                      selectedTile->y == selectedPiece->y - 202) {

                selectedPiece->color = RED;
                selectedPiece->x = selectedTile->x + 67;
                selectedPiece->y = selectedTile->y + 67;
                is_player_1_turn = false;

              }

              else if (selectedTile->x == selectedPiece->x - 337 &&
                       selectedTile->y == selectedPiece->y + 203) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_2_pieces[piece_num].x == selectedPiece->x - 135 &&
                      player_2_pieces[piece_num].y == selectedPiece->y + 135) {
                    piece_to_remove = &player_2_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x + 203 &&
                         selectedTile->y == selectedPiece->y + 203) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_2_pieces[piece_num].x == selectedPiece->x + 135 &&
                      player_2_pieces[piece_num].y == selectedPiece->y + 135) {
                    piece_to_remove = &player_2_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x - 337 &&
                         selectedTile->y == selectedPiece->y - 337) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_2_pieces[piece_num].x == selectedPiece->x - 135 &&
                      player_2_pieces[piece_num].y == selectedPiece->y - 135) {
                    piece_to_remove = &player_2_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x + 203 &&
                         selectedTile->y == selectedPiece->y - 337) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_2_pieces[piece_num].x == selectedPiece->x + 135 &&
                      player_2_pieces[piece_num].y == selectedPiece->y - 135) {
                    piece_to_remove = &player_2_pieces[piece_num];
                  }
                }
              }
        if (piece_to_remove) {
          selectedPiece->color = RED;
          selectedPiece->x = selectedTile->x + 67;
          selectedPiece->y = selectedTile->y + 67;
          RemovePiece(piece_to_remove);
          piece_to_remove = nullptr;
          is_player_1_turn = false;
        }
        selectedPiece->color = RED;
        selectedPiece->lineColor = BLACK;
        selectedPiece = nullptr;
        selectedTile = nullptr;

      } else if (selectedTile && selectedPiece) {

        if (selectedTile->playable)
          if (selectedTile->y > selectedPiece->y)
            if (selectedTile->x != selectedPiece->x - 67)
              if (HasNoPieceAlready(*selectedTile, player_1_pieces,
                                    player_2_pieces, num_pieces))
                if (selectedTile->x == selectedPiece->x + 68 &&
                        selectedTile->y == selectedPiece->y + 68 ||
                    selectedTile->x == selectedPiece->x - 202 &&
                        selectedTile->y == selectedPiece->y + 68) {

                  selectedPiece->color = RED;
                  selectedPiece->x = selectedTile->x + 67;
                  selectedPiece->y = selectedTile->y + 67;
                  is_player_1_turn = false;
                } else if (selectedTile->x == selectedPiece->x - 337 &&
                           selectedTile->y == selectedPiece->y + 203) {
                  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                    if (player_2_pieces[piece_num].x ==
                            selectedPiece->x - 135 &&
                        player_2_pieces[piece_num].y ==
                            selectedPiece->y + 135) {
                      piece_to_remove = &player_2_pieces[piece_num];
                    }
                  }
                } else if (selectedTile->x == selectedPiece->x + 203 &&
                           selectedTile->y == selectedPiece->y + 203) {
                  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                    if (player_2_pieces[piece_num].x ==
                            selectedPiece->x + 135 &&
                        player_2_pieces[piece_num].y ==
                            selectedPiece->y + 135) {
                      piece_to_remove = &player_2_pieces[piece_num];
                    }
                  }
                }
        if (piece_to_remove) {
          selectedPiece->color = RED;
          selectedPiece->x = selectedTile->x + 67;
          selectedPiece->y = selectedTile->y + 67;
          RemovePiece(piece_to_remove);
          piece_to_remove = nullptr;
          is_player_1_turn = false;
        }
        if (selectedPiece->y == 1012) {
          selectedPiece->is_king = true;
        }

        selectedPiece->color = RED;
        selectedPiece->lineColor = BLACK;
        selectedPiece = nullptr;
        selectedTile = nullptr;
      }
    } else {

      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
          Vector2 piece_pos = {player_2_pieces[piece_num].x,
                               player_2_pieces[piece_num].y};
          // radius of a piece is always 30
          if (CheckCollisionPointCircle(GetMousePosition(), piece_pos, 30)) {
            if (selectedPiece == nullptr) {
              selectedPiece = &player_2_pieces[piece_num];
              selectedPiece->color = DARKBLUE;
              selectedPiece->lineColor = WHITE;
            } else {
              selectedPiece->color = BLUE;
              selectedPiece->lineColor = BLACK;
              selectedPiece = nullptr;
            }
          }
        }
      }

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (selectedPiece) {
          Vector2 move_pos = GetMousePosition();

          for (int tile_num = 0; tile_num < num_tiles; tile_num++) {

            Rectangle tile_rec = {tiles[tile_num].x, tiles[tile_num].y, 135,
                                  135};

            if (CheckCollisionPointRec(move_pos, tile_rec)) {
              selectedTile = &tiles[tile_num];
              cout << selectedTile->x << ", " << selectedTile->y << "\n";
            }
          }
        }
      }

      if (selectedPiece && selectedPiece->is_king && selectedTile) {

        if (selectedTile->playable)
          if (selectedTile->x != selectedPiece->x - 67)
            if (HasNoPieceAlready(*selectedTile, player_1_pieces,
                                  player_2_pieces, num_pieces))
              if (selectedTile->x == selectedPiece->x + 68 &&
                      selectedTile->y == selectedPiece->y + 68 ||
                  selectedTile->x == selectedPiece->x - 202 &&
                      selectedTile->y == selectedPiece->y + 68 ||
                  selectedTile->x == selectedPiece->x - 202 &&
                      selectedTile->y == selectedPiece->y - 202 ||
                  selectedTile->x == selectedPiece->x + 68 &&
                      selectedTile->y == selectedPiece->y - 202) {

                selectedPiece->color = BLUE;
                selectedPiece->x = selectedTile->x + 67;
                selectedPiece->y = selectedTile->y + 67;
                is_player_1_turn = true;

              }

              else if (selectedTile->x == selectedPiece->x - 337 &&
                       selectedTile->y == selectedPiece->y + 203) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_1_pieces[piece_num].x == selectedPiece->x - 135 &&
                      player_1_pieces[piece_num].y == selectedPiece->y + 135) {
                    piece_to_remove = &player_1_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x + 203 &&
                         selectedTile->y == selectedPiece->y + 203) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_1_pieces[piece_num].x == selectedPiece->x + 135 &&
                      player_1_pieces[piece_num].y == selectedPiece->y + 135) {
                    piece_to_remove = &player_1_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x - 337 &&
                         selectedTile->y == selectedPiece->y - 337) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_1_pieces[piece_num].x == selectedPiece->x - 135 &&
                      player_1_pieces[piece_num].y == selectedPiece->y - 135) {
                    piece_to_remove = &player_1_pieces[piece_num];
                  }
                }
              } else if (selectedTile->x == selectedPiece->x + 203 &&
                         selectedTile->y == selectedPiece->y - 337) {
                for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                  if (player_1_pieces[piece_num].x == selectedPiece->x + 135 &&
                      player_1_pieces[piece_num].y == selectedPiece->y - 135) {
                    piece_to_remove = &player_1_pieces[piece_num];
                  }
                }
              }
        if (piece_to_remove) {
          selectedPiece->color = BLUE;
          selectedPiece->x = selectedTile->x + 67;
          selectedPiece->y = selectedTile->y + 67;
          RemovePiece(piece_to_remove);
          piece_to_remove = nullptr;
          is_player_1_turn = true;
        }
        selectedPiece->color = BLUE;
        selectedPiece->lineColor = BLACK;
        selectedPiece = nullptr;
        selectedTile = nullptr;
      } else if (selectedTile && selectedPiece) {

        if (selectedTile->playable)
          if (selectedTile->y < selectedPiece->y)
            if (selectedTile->x != selectedPiece->x - 67)
              if (HasNoPieceAlready(*selectedTile, player_1_pieces,
                                    player_2_pieces, num_pieces))
                if (selectedTile->x == selectedPiece->x - 202 &&
                        selectedTile->y == selectedPiece->y - 202 ||
                    selectedTile->x == selectedPiece->x + 68 &&
                        selectedTile->y == selectedPiece->y - 202) {

                  selectedPiece->color = BLUE;
                  selectedPiece->x = selectedTile->x + 67;
                  selectedPiece->y = selectedTile->y + 67;
                  is_player_1_turn = true;
                } else if (selectedTile->x == selectedPiece->x - 337 &&
                           selectedTile->y == selectedPiece->y - 337) {
                  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                    if (player_1_pieces[piece_num].x ==
                            selectedPiece->x - 135 &&
                        player_1_pieces[piece_num].y ==
                            selectedPiece->y - 135) {
                      piece_to_remove = &player_1_pieces[piece_num];
                    }
                  }
                } else if (selectedTile->x == selectedPiece->x + 203 &&
                           selectedTile->y == selectedPiece->y - 337) {
                  for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
                    if (player_1_pieces[piece_num].x ==
                            selectedPiece->x + 135 &&
                        player_1_pieces[piece_num].y ==
                            selectedPiece->y - 135) {
                      piece_to_remove = &player_1_pieces[piece_num];
                    }
                  }
                }
        if (piece_to_remove) {
          selectedPiece->color = BLUE;
          selectedPiece->x = selectedTile->x + 67;
          selectedPiece->y = selectedTile->y + 67;
          RemovePiece(piece_to_remove);
          piece_to_remove = nullptr;
          is_player_1_turn = true;
        }

        if (selectedPiece->y == 67) {
          selectedPiece->is_king = true;
        }

        selectedPiece->color = BLUE;
        selectedPiece->lineColor = BLACK;
        selectedPiece = nullptr;
        selectedTile = nullptr;
      }

      /// Check for king piece
    }

    // Drawing
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // Draw Board
    for (int tile_num = 0; tile_num < num_tiles; tile_num++) {
      tiles[tile_num].Draw();
    }
    DrawRectangleLines(0, 0, 1080, 1080, BLACK);
    // Draw Starting Player pieces
    for (int piece_num = 0; piece_num < num_pieces; piece_num++) {
      // cout << "Piece Number " << piece_num << ": ";
      player_1_pieces[piece_num].Draw();
      player_2_pieces[piece_num].Draw();
    }

    if (CheckWin(player_1_pieces, player_2_pieces, num_pieces)) {
      if (is_player_1_turn) {
        bool player_2_won = true;
      } else {
        bool player_1_won = true;
      }
    } else {

      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();

        if (CheckCollisionPointRec(mouse_pos, cant_move)) {
          if (is_player_1_turn) {
            player_2_won = true;
          } else {
            player_1_won = true;
          }

        } else if (CheckCollisionPointRec(mouse_pos, draw)) {
          is_draw = true;
        }
      }
    }

    if (is_draw && !player_1_won && !player_2_won) {
      DrawText("DRAW!", 1100, 500, 40, ORANGE);
    }

    if (player_1_won && !is_draw && !player_2_won) {

      DrawText("Player 1 Won!", 1100, 500, 40, ORANGE);
    }

    if (player_2_won && !is_draw && !player_1_won) {

      DrawText("Player 2 Won!", 1100, 500, 40, ORANGE);
    }

    DrawRectangleRec(cant_move, LIGHTGRAY);
    DrawText("GIVE UP", 1180, 405, 40, WHITE);

    DrawRectangleRec(draw, LIGHTGRAY);
    DrawText("DRAW", 1200, 305, 40, WHITE);

    DrawText("C++ Checkers\nNicholas C. Barinaga", 1110, 1000, 20, LIGHTGRAY);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}