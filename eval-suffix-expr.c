#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "eval-suffix-expr.h"

CAL_ERROR ERROR;

typedef enum
  {
    false = 0,
    true  = 1,
  } bool;

typedef enum
  {
    ADD = '+',
    MINUS = '-',
    MULT = '*',
    DIV = '/',
    L_PARE = '(',
    R_PARE = ')',
    NUMBER = 'a',
  } item_type;

typedef struct
{
  item_type type;
  double data;
} item;

item items[20];
int k;                          /* index for items */

void
convert_items (char expression[])
{
  int i;
  int j;
  char buffer[10];              /* save a number */
  int buffer_i;

  k = 0;
  for (i = 0; i < strlen(expression); i++) {
    if (isdigit (expression[i]))
      {
        j = i + 1;
        buffer_i = 0;
        buffer[buffer_i++] = expression[i];
        while (isdigit (expression[j]) || expression[j] == '.') {
          buffer[buffer_i++] = expression[j];
          i = j;
          j++;
        }
        buffer[buffer_i] = '\0';
        items[k].type = NUMBER;
        items[k].data = atof (buffer);
        k++;
      }
    else
      {
        items[k].type = expression[i];
        items[k].data = expression[i];
        k++;
      }
  }
}

char stack[20];
int idx;

void
push (char x)
{
  assert (idx < 20);
  stack[idx++] = x;
}

char
pop ()
{
  assert (idx > 0);
  return stack[--idx];
}


bool
higher_precedence (char a, char b)
{
  if ( (a == '*' || a == '/') && (b == '-' || b == '+') )
    return true;
  else
    return false;
}

item suffix[20];
int si;                          /* index for suffix items */


void
convert_to_suffix ()
{
  int i;

  idx = 0;
  for (i = 0; i < k; i++) {
    if (items[i].type == NUMBER)
      {
        suffix[si++] = items[i];

      }
    else
      switch (items[i].type) {
      case ')':
        while (stack[idx-1] != '(') {
          suffix[si].type = suffix[si].data = pop ();
          si++;
        }
        pop ();                 /* pop '(', don't print */
        break;
      case '(':
        push ('(');
        break;
      default:                  /* +, -, *, / */
        if (higher_precedence(stack[idx-1], items[i].type))
          {
            // print all stack
            while (idx != 0)
              {
                suffix[si].type = suffix[si].data = pop ();
                si++;
              }
          }
        push(items[i].type);
        break;
      }
  }
  while (idx != 0)
    {
      suffix[si].type = suffix[si].data = pop ();
      si++;
    }

}

double stack2[20];

double *sp;

#define pop3(sp) (*--(sp))
#define push3(sp, n) (*((sp)++) = (n))

void
push2 (double x)
{
  assert (idx2 < 20);
  stack2[idx2++] = x;
}

double
pop2 ()
{
  assert (idx2 > 0);
  return stack2[--idx2];
}

double
calculate_suffix ()
{
  int i;
  double a, b;

  sp = stack2;

  for (i = 0; i < si; i++) {
    if (suffix[i].type == NUMBER)
      push3 (sp, suffix[i].data);
    else
      switch (suffix[i].type) {
      case '+':
        a = pop3 (sp);
        b = pop3 (sp);
        push3 (sp, a + b);
        break;
      case '*':
        a = pop3 (sp);
        b = pop3 (sp);
        push3 (sp, a * b);
        break;
      case '-':
        a = pop3 (sp);
        b = pop3 (sp);
        push3 (sp, b - a);
        break;
      case '/':
        a = pop3 (sp);
        b = pop3 (sp);
        if (a == 0)
          {
            ERROR.code = 1;
            ERROR.message = strdup("Error: div-by-zero");
            return 0.0;
          }
        push3 (sp, b / a);
        break;
      default:
        assert(0);
        break;
      }
  }
  return stack2[0];
}

double
eval_suffix_expr (char expression[], CAL_ERROR *cal_error)
{
  double rtv;

  convert_items (expression);
  convert_to_suffix ();
  rtv = calculate_suffix ();

  idx = k = si = 0;
  *cal_error = ERROR;
  return rtv;
}
