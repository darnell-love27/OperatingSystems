#include <stdio.h>
#include <stdlib.h>

/* IMPLEMENT ME: Declare your functions here */
int add (int a, int b);
int subtract (int a, int b);
int multiply (int a, int b);
int divide (int a, int b);


int main (void)
{
	/* IMPLEMENT ME: Insert your algorithm here */
  int a = 10;
  int b = 5;

  char operate_num;
  printf("Enter '0' for add, '1' for subtraction, '2' for multiplication, '3' for division, or '4' for exit: ");
  scanf(" %c", &operate_num);

  //Checks user input and does operation
  int choice = operate_num - '0';
  int result = operate_num[choice](a, b);
  printf("Result: %d\n", result);


	return 0;
}

/* IMPLEMENT ME: Define your functions here */
int add (int a, int b) { printf ("Adding 'a' and 'b'\n"); return a + b; }
int subtrat (int a, int b) { printf ("Subtracting 'b' from 'a'\n"); return a - b; }
int muliply (int a, int b) { printf ("Multiplying 'a' and 'b'\n"); return a * b; }
int divide (int a, int b) { printf ("Dividing 'a' by 'b'\n"); return a / b; }
