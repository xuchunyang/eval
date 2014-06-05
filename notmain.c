#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

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

void convert_items (char expression[])
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
int idx2;

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
  double tmp;

  idx2 = 0;
  for (i = 0; i < si; i++) {
    if (suffix[i].type == NUMBER)
      push2 (suffix[i].data);
    else
      switch (suffix[i].type) {
      case '+':
        push2 (pop2 () + pop2 ());
        break;
      case '*':
        push2 (pop2 () * pop2 ());
        break;
      case '-':
        tmp = pop2();
        push2 (pop2 () - tmp);
        break;
      case '/':
        tmp = pop2();
        assert (tmp != 0);
        push2 (pop2 () / tmp);
        break;
      default:
        assert(0);
        break;
      }
  }

  assert(idx2 == 1);
  return stack2[idx2-1];
}

int
main (int argc, char *argv[])
{
  char expression[] = "9+(3-1)*3+1.55/0.5"; /* types: +-/() double */

  convert_items (expression);
  convert_to_suffix ();
  printf ("%s = %.2f\n", expression, calculate_suffix ());

  return 0;
}
