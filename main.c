#ifndef NOINC
#include "3rd/cosmopolitan/cosmopolitan.h"
#endif 

#define RET_Z( a, b) do{ if((a) == 0) return b; }while(0);
#define RET_NZ( a, b) do{ if((a) != 0) return b; }while(0);

#define STR_LEN 2048

struct _app {
  char* version;
  char* headFiles;     //header
  char* rollFiles;     //roll file
  char* outFile;       //out file
  char* confFile;      //config file
} app;

static void app_init() {
  app.version = "0.0.1";
  app.headFiles = "";
  app.rollFiles = "";
  app.confFile = "rollc.txt";
  app.outFile = "out.c";
}

static void app_exit() {
  
}


int args_from_file(char* argv0, const char* fname, int* argc, char*** argv) {
  FILE* file = NULL;
  RET_Z( file = fopen(fname, "r") , 0);

  char tmp_str[STR_LEN];
  size_t str_idx = 0;

  char* tmp_argv[20];
  tmp_argv[(*argc)++] = argv0; 

  char c;
  while( (c = fgetc(file)) ) {
    if( c == '\r' || c == '\n' || c == ' ' || c == EOF) {
      if( str_idx > 0 ) {
        tmp_str[str_idx] = 0;
        char* av = malloc(str_idx + 1);
        memcpy(av, tmp_str, str_idx + 1);
        tmp_argv[(*argc)++] = av;
        str_idx = 0;
      }
    }else{
      tmp_str[str_idx++] = c;
    }
    if( c == EOF ) break;
  }

  (*argv) = malloc( (*argc) * sizeof(char*));

  for(int i = 0; i < (*argc); ++i){
    (*argv)[i] = tmp_argv[i];
  }

  return 1;
}


static int use_opts(int argc, char *argv[]) {
  int opt;
  while( (opt = getopt(argc, argv, "r:o:c:h:")) != -1 ) {
    switch (opt)
    {
    case 'r':
      app.rollFiles = optarg;
      break;
    case 'o':
      app.outFile = optarg;
      break;
    case 'c':
      app.confFile = optarg;
      break;
    case 'h':
      app.headFiles = optarg;
      break;
    default:
      break;
    }
  }
}

static int rollc_file(FILE* wf, FILE* rf, int skipInc) {
  char c;
  char line[STR_LEN * 8];
  size_t idx = 0;
  
  int record = 0;
  while( (c = fgetc(rf))  ) {
    if( c == '\n' || c == EOF) {
      line[idx] = 0;

      if( !(idx >= 11 && strncmp(line, "#include", 8) == 0 && skipInc)  ) 
        fprintf(wf, "%s\n", line);

      idx = 0;
    }else{
      line[idx++] = c;
    }

    if( c == EOF ) break;
  }
  fputs("\n\n", wf);
}

static int rollc(FILE* wf, const char* files, int skipInc){
  size_t len = strlen(files);

  char fname[STR_LEN];
  char* p = files;
  char* q = p;
  while(1){
    if( *q == ',' || *q == 0) {
      memcpy(fname, p, q-p);

      size_t ed = q - p;
      fname[ ed ] = '\0';
      printf("rollc file ===== %s\n", fname);

      FILE* rf = NULL;
      if( (rf = fopen(fname, "r")) == 0) {
        printf("rollc file err : %s\n", fname);
        break;
      }
      rollc_file(wf, rf, skipInc);
      fclose(rf);
      p = q + 1;
    }
    if(*q == 0) break;
    q++;
  }
  return 1;
}


int main(int argc, char *argv[]){
  printf("+------------------ rollc start ------------------+\n");
  app_init();

  use_opts(argc, argv);

  // read config file 
  int fargc = 0;
  char** fargv;
  if( args_from_file(argv[0], app.confFile, &fargc, &fargv) ){
    use_opts(fargc, fargv);
  }else{
    printf("read config file %s error !\n", app.confFile);
  }

  //printf("headFiles -- %s \n", app.headFiles);
  //printf("rollFiles -- %s \n", app.rollFiles);
  //printf("outFile -- %s \n", app.outFile);
  //printf("confFile -- %s \n", app.confFile);

  char dirName[STR_LEN];
  strcpy(dirName, app.outFile);
  makedirs(dirname(dirName), 0777);
  FILE* wf = NULL;
  if( (wf = fopen(app.outFile, "w"))  == 0) {
    printf("open out file %s err! \n", app.outFile );
    return 0;
  }

  rollc(wf, app.headFiles, 0);
  rollc(wf, app.rollFiles, 1);

  printf("rollc end...\n");

  fclose( wf );

  printf("+------------------ rollc end --------------------+\n");
  return 0;
}