## Evaluate infix expression with C

### Usage
For example, to evaluate "9+(3-1)*3+1.5/0.5":

```c
#include "eval-suffix-expr.h"

char expr[] = "9+(3-1)*3+1.5/0.5";
CAL_ERROR cal_error = {.code = 0, .message = ""};
double rtv = 0.0;

rtv = eval_suffix_expr(expr, &cal_error);
if (cal_error.code != 0)      /* Error occurs! */
  {
    fprintf (stderr, "%s", cal_error.message);
  }
else
  {
    printf ("The result of %s is %f\n", expr, rtv);
  }
```

### TODO
- [x] handle exceptions like divison by zero
- [-] clean unnecessary functions
