#ifndef EVAL_H
#define EVAL_H

typedef struct {
  int         code;
  char       *message;
} CAL_ERROR;

double eval_suffix_expr(char expression[], CAL_ERROR *cal_error);

#endif
