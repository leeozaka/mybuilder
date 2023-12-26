#include <conio3.h>
#include <corecrt.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONSOLE_LEN 64

typedef struct warnings warnings;
typedef struct optimizations optimizations;
typedef struct configs configs;

struct warnings {
  bool Wall;
  bool Wextra;
  bool Werror;
};

struct optimizations {
  bool O0;
  bool O1;
  bool O2;
  bool O3;
  bool Os;
};

struct configs {
  warnings warnings;
  optimizations optimizations;
  bool debug_infos;
  bool lc;
};

bool try_to_open(FILE **read_file) {
  DIR *dir = opendir("config");
  if (!dir)
    system("mkdir config");

  errno_t err;
  err = fopen_s(read_file, "config\\config.dat", "rb+");
  if (err == 0) {
    return true;
  }

  err = fopen_s(read_file, "config\\config.dat", "wb");
  if (err == 0) {
    fclose(*read_file);
    fopen_s(read_file, "config\\config.dat", "rb+");
    return true;
  }
  return false;
}

inline void reset_optimizations(configs *configuracao_atual) {
  configuracao_atual->optimizations.O0 = false;
  configuracao_atual->optimizations.O1 = false;
  configuracao_atual->optimizations.O2 = false;
  configuracao_atual->optimizations.O3 = false;
  configuracao_atual->optimizations.Os = false;
}

void make_config(configs *configuracao_atual) {
  char opc;
  system("cls");

  printf("gerenciador de cfg\n");
  printf("a - alertas\n");
  printf("b - otimizacoes\n");
  printf("c - debug infos\n");
  printf("d - lc\n");
  printf("x - finalizar configs\n");
  opc = _getch();

  while (opc != 'x') {
    switch (opc) {
    case 'a':
      printf("a - Wall\n");
      printf("b - Wextra\n");
      printf("c - Werror\n");
      opc = _getch();
      switch (opc) {
      case 'a':
        configuracao_atual->warnings.Wall = true;
        break;
      case 'b':
        configuracao_atual->warnings.Wall = true;
        configuracao_atual->warnings.Wextra = true;
        break;
      case 'c':
        configuracao_atual->warnings.Werror = true;
        break;
      default:
        printf("opc n cadastrada");
        system("pause");
      }
      break;

    case 'b':
      printf("grau de otimizacao: 0 a 4 (4 = size_optimization) \n");
      int n;
      scanf_s("%d", &n);
      switch (n) {
      case 0:
        reset_optimizations(configuracao_atual);
        configuracao_atual->optimizations.O0 = true;
        break;
      case 1:
        reset_optimizations(configuracao_atual);
        configuracao_atual->optimizations.O1 = true;
        break;
      case 2:
        reset_optimizations(configuracao_atual);
        configuracao_atual->optimizations.O2 = true;
        break;
      case 3:
        reset_optimizations(configuracao_atual);
        configuracao_atual->optimizations.Os = true;
        break;
      default:
        printf("opc n cadastrada\n");
        system("pause");
      }
      break;

    case 'c':
      printf("debug infos: s ou n");
      opc = _getch();
      switch (opc) {
      case 's':
        configuracao_atual->debug_infos = true;
        break;
      case 'n':
        configuracao_atual->debug_infos = false;
        break;
      default:
        printf("opc n cadastrada\n");
        system("pause");
      }
      break;

    case 'd':
      printf("lc s ou n");
      opc = _getch();
      switch (opc) {
      case 's':
        configuracao_atual->lc = true;
        break;
      case 'n':
        configuracao_atual->lc = false;
        break;
      default:
        printf("opc n cadastrada\n");
        system("pause");
      }
      break;
    }

    system("cls");
    printf("gerenciador de cfg\n");
    printf("a - alertas\n");
    printf("b - otimizacoes\n");
    printf("c - debug infos\n");
    printf("d - lc\n");
    printf("x - finalizar configs\n");
    opc = _getch();
  }
}

configs *init() {
  configs *new_config = (configs *)calloc(1, sizeof(configs));
  new_config->warnings.Werror = false;
  new_config->warnings.Wall = false;
  new_config->warnings.Wextra = false;
  new_config->optimizations.O0 = false;
  new_config->optimizations.O1 = false;
  new_config->optimizations.O2 = false;
  new_config->optimizations.O3 = false;
  new_config->optimizations.Os = false;
  new_config->debug_infos = false;
  new_config->lc = false;
  return new_config;
}

char *mkstring(configs *configuracao_atual, const char *filename) {

  char *cons_string = (char *)malloc(sizeof(char) * CONSOLE_LEN);
  cons_string[0] = '\0';
  strcat(cons_string, "gcc ");
  if (configuracao_atual->warnings.Wall == true)
    strcat(cons_string, (const char *)"-Werror ");
  if (configuracao_atual->warnings.Wextra == true)
    strcat(cons_string, (const char *)"-Wextra ");
  if (configuracao_atual->warnings.Werror == true)
    strcat(cons_string, (const char *)"-Werror ");

  if (configuracao_atual->optimizations.Os == true)
    strcat(cons_string, (const char *)"-Os ");
  if (configuracao_atual->optimizations.O0 == true)
    strcat(cons_string, (const char *)"-O0 ");
  if (configuracao_atual->optimizations.O1 == true)
    strcat(cons_string, (const char *)"-O1 ");
  if (configuracao_atual->optimizations.O2 == true)
    strcat(cons_string, (const char *)"-O2 ");
  if (configuracao_atual->optimizations.O3 == true)
    strcat(cons_string, (const char *)"-O3 ");

  if (configuracao_atual->debug_infos == true)
    strcat(cons_string, (const char *)"-g3 ");

  strcat(cons_string, filename);
  strcat(cons_string, " ");

  strcat(cons_string, (const char *)"-o ");
  strcat(cons_string, (const char *)"out.exe ");

  if (configuracao_atual->lc == true)
    strcat(cons_string, (const char *)"-lc ");

  return cons_string;
}

void write_cfg(configs *config, FILE *fp) {
  fwrite(&config, sizeof(configs), 1, fp);
}

void read_cfg(configs *config, FILE *fp) {
  fwrite(&config, sizeof(configs), 1, fp);
}

int main(int argc, char **argv) {
  FILE *cfg;
  configs *configuracao_atual = init();
  int file_size;
  char *console_write, opc;

  printf("leonardo's builder v0.0.1\n");
  printf("%s\n", try_to_open(&cfg) ? "config file opened"
                                   : "unexpected errror while opening file");

  if (configuracao_atual) {
    fseek(cfg, 0, SEEK_END);
    file_size = ftell(cfg);
    if (file_size > 0) {
      rewind(cfg);
      read_cfg(configuracao_atual, cfg);
      // fread(configuracao_atual, sizeof(configs), 1, cfg);
    } else {
      printf("sem cfg estabelecida\n");
      printf("aperte a para criar nova cfg ");
      if (_getch() == 'a') {
        make_config(configuracao_atual);
        write_cfg(configuracao_atual, cfg);
      }
    }
  }

  printf("%d  \n", argc);
  if (argc == 1) {
    printf("D - delete configs\n");
    if (_getch() == 'D') {
      fclose(cfg);
      system("rmdir /s config");
    }
    fclose(cfg);
    return 0;
  }
  console_write = mkstring(configuracao_atual, argv[1]);
  puts(console_write);

  file_size = ftell(cfg);
  printf("%d", file_size);
  fclose(cfg);
}
