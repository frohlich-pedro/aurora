#include "../drivers/display.h"

void print_banner() {
  print_string("  --  Welcome to Aurora  --  \n\n");
  print_string("  /$$$$$$                                                    /$$$$$$   /$$$$$$ \n");
  print_string(" /$$__  $$                                                  /$$__  $$ /$$__  $$\n");
  print_string("| $$  \\ $$ /$$   /$$  /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$ | $$  \\ $$| $$  \\__/\n");
  print_string("| $$$$$$$$| $$  | $$ /$$__  $$ /$$__  $$ /$$__  $$|____  $$| $$  | $$|  $$$$$$ \n");
  print_string("| $$__  $$| $$  | $$| $$  \\__/| $$  \\ $$| $$  \\__/ /$$$$$$$| $$  | $$ \\____  $$\n");
  print_string("| $$  | $$| $$  | $$| $$      | $$  | $$| $$      /$$__  $$| $$  | $$ /$$  \\ $$\n");
  print_string("| $$  | $$|  $$$$$$/| $$      |  $$$$$$/| $$     |  $$$$$$$|  $$$$$$/|  $$$$$$/\n");
  print_string("|__/  |__/ \\______/ |__/       \\______/ |__/      \\_______/ \\______/  \\______/ \n");
  print_string("\n - Version:   0.0.0");
  print_string("\n - Copyright: Copyright (C) 2025 Pedro Frohlich, AuroraOS");
  print_string("\n\n");
}
