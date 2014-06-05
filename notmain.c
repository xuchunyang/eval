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

void convert_items ()
{
  char expression[] = "9+(3-1)*3+10.3/2.1"; /* types: +-/() double */
  int i;
  int j;
  int k;
  char buffer[10];              /* save a number */
  int buffer_i;


  k = 0;
  for (i = 0; i < strlen(expression); i++) {
    /* printf ("%c\t", expression[i]); */
    if (isdigit (expression[i]))
      {
        j = i + 1;
        buffer_i = 0;
        /* printf("%c", expression[i]); */
        buffer[buffer_i++] = expression[i];
        while (isdigit (expression[j]) || expression[j] == '.') {
          /* printf ("%c", expression[j]); */
          buffer[buffer_i++] = expression[j];
          i = j;
          j++;
        }
        buffer[buffer_i] = '\0';
        items[k].type = NUMBER;
        items[k].data = atof (buffer);
        k++;
        printf ("%.2f", atof (buffer));
        printf (" ");
      }
    else
      {
        items[k].type = expression[i];
        items[k].data = expression[i];
        k++;
        printf ("%c ", expression[i]);
      }
  }
  printf ("\n\n=====check result====\n");
  for (i = 0; i < k; i++) {
    if (items[i].type == NUMBER)
      printf ("%d: %.2f\n", i, items[i].data);
    else
      printf ("%d: %c\n", i, (char)items[i].data);
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

int
main (int argc, char *argv[])
{
  convert_items ();
  char expression[] = "9+(3-1)*3+1/2"; /* types: +-/() double */
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
  double calculate_suffix ();
  char t_suffix[] = "931-3*+12/+";
  printf ("%s\n", t_suffix);
  printf ("%f\n", calculate_suffix ());

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
calculate_suffix ()
{
  int i;
  double tmp;

  idx2 = 0;
  for (i = 0; items[i].type != 0; i++) {
    /* printf ("%c", suffix[i]); */
    if (items[i].type == NUMBER)
      push2 (items[i].data);
    else
      switch (items[i].type) {
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
