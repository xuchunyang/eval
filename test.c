#include <stdio.h>
#include <assert.h>

#include "eval-suffix-expr.h"

int
main(int argc, char *argv[])
{
  char expr[] = "9+(3-1)*3+1.55/0.5";
  double rtv = 0.0;
  CAL_ERROR cal_error = {.code = 0, .message = ""};

  rtv = eval_suffix_expr(expr, &cal_error);
  if (cal_error.code != 0)      /* Error occur! */
    {
      fprintf (stderr, "%s", cal_error.message);
      return 1;
    }
  printf ("%s = %.2f\n", expr, rtv);

  return 0;
}

void
other_test_cases ()
{
  char expr[] = "1*3-2/10.3+32.23";
  CAL_ERROR cal_error = {.code = 0, .message = ""};

  assert(eval_suffix_expr(expr, &cal_error) == 1*3-2/10.3+32.23);
  assert(eval_suffix_expr("1+2-3*4.5/5", &cal_error) == 1+2-3*4.5/5);
  assert(eval_suffix_expr("1/0.1", &cal_error) == 1/0.1);
}
