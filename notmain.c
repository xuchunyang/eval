#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

typedef enum
{
    false = 0,
    true  = 1,
} bool;

char stack[10];
int idx;
int top;

void
push (char x)
{
  assert (idx < 10);
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

  return 0;
}
