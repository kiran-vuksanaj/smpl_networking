#include "err.h"

void printerr(char *msg){
  printf("error #%d during [%s]: %s\n",errno,msg,strerror(errno));
}
int  checkerr(char *msg,int r){
  if( r < 0 ){
    printerr(msg);
    return -1;
  }
  return 0;
}
void exit_err(char *msg,int r){
  if( r < 0 ){
    printerr(msg);
    exit(errno);
  }
}
