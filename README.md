# Evaluate infix expression with C

## Usage
For example, to evaluate "9+(3-1)*3+1.5/0.5":

```c
#include "eval-suffix-expr.h"

char expr[] = "9+(3-1)*3+1.5/0.5";
double rtv = eval_suffix_expr(expr);
```

## TODO
- [ ] handle exceptions like divison by zero
