#include <stdio.h>
#include <assert.h>

#include "eval-suffix-expr.h"

int
main(int argc, char *argv[])
{
  char expr[] = "9+(3-1)*3+1.55/0.5";
  char expr2[] = "1*3-2/10.3+32.23";

  assert(eval_suffix_expr(expr) == 9+(3-1)*3+1.55/0.5);
  assert(eval_suffix_expr(expr2) == 1*3-2/10.3+32.23);
  assert(eval_suffix_expr("1+2-3*4.5/5") == 1+2-3*4.5/5);

  /* assert(eval_suffix_expr("1/0") == 1/0.0); */

  return 0;
}
