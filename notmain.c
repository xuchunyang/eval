#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef enum
{
    false = 0,
    true  = 1,
} bool;

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

int
main (int argc, char *argv[])
{
  char expression[] = "9+(3-1)*3+1/2";
  int i;

  idx = 0;
  printf ("%s\n", expression);
  for (i = 0; i < strlen (expression); i++) {
    if (isdigit(expression[i]))
      printf ("%c ", expression[i]);
    else
      switch (expression[i]) {
      case ')':
        while (stack[idx-1] != '(')
          printf ("%c ", pop());
        pop ();                 /* pop '(', don't print */
        break;
      case '(':
        push ('(');
        break;
      default:                  /* +, -, *, / */
        if (higher_precedence(stack[idx-1], expression[i]))
          {
            // print all stack
            while (idx != 0)
              {
                printf ("%c ", pop ());
              }
          }
        push(expression[i]);
        break;
      }
  }
  while (idx != 0)
    {
      printf ("%c ", pop ());
    }

  printf ("\n\n=============\n");
  double calculate_suffix (char []);
  char t_suffix[] = "931-3*+12/+";
  printf ("%s\n", t_suffix);
  printf ("%f\n", calculate_suffix (t_suffix));

  return 0;
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
calculate_suffix (char suffix[])
{
  int i;
  double tmp;

  idx2 = 0;
  for (i = 0; i < strlen (suffix); i++) {
    /* printf ("%c", suffix[i]); */
    if (isdigit(suffix[i]))
      push2 (suffix[i] - '0');
    else
      switch (suffix[i]) {
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
