#include "../drivers/display.h"
#include "../cpu/timer.h"

void print_banner() {
  const char* banner[] =  {
    "  --  Welcome to Aurora  --  \n\n",
    "  /$$$$$$                                                    /$$$$$$   /$$$$$$ \n",
    " /$$__  $$                                                  /$$__  $$ /$$__  $$\n",
    "| $$  \\ $$ /$$   /$$  /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$ | $$  \\ $$| $$  \\__/\n",
    "| $$$$$$$$| $$  | $$ /$$__  $$ /$$__  $$ /$$__  $$|____  $$| $$  | $$|  $$$$$$ \n",
    "| $$__  $$| $$  | $$| $$  \\__/| $$  \\ $$| $$  \\__/ /$$$$$$$| $$  | $$ \\____  $$\n",
    "| $$  | $$| $$  | $$| $$      | $$  | $$| $$      /$$__  $$| $$  | $$ /$$  \\ $$\n",
    "| $$  | $$|  $$$$$$/| $$      |  $$$$$$/| $$     |  $$$$$$$|  $$$$$$/|  $$$$$$/\n",
    "|__/  |__/ \\______/ |__/       \\______/ |__/      \\_______/ \\______/  \\______/ \n",
    "\n - Version:   0.0.0",
    "\n - Copyright: Copyright (C) 2025 Pedro Frohlich, AuroraOS",
    "\n\n"
  };

  char* banner_ptr = banner;
  char* banner_end = banner + 12;

  clear_screen();
  
  do {
    sleep(20);
    print_string(*banner_ptr++);
  } while (banner_ptr < banner_end);
}
