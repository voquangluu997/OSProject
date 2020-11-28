#include <stdio.h>
#include <unistd.h>
#include <iostream>
// #include "fileHandle.h"
#include "stack.h"
#include "queue.h"

void menu();
double balanAlth(char *expression);
int getLength(char *);

int main()
{

   Stack st;
   Queue q;
   char expression[50] = "11+(20.5*5)/2";
   cout << "kq la : " << balanAlth(expression) << endl;

   // int pipefds1[2], pipefds2[2];
   // int returnstatus1, returnstatus2;
   // int pid;
   // char pipe1writeCalculation[50] = "Hiii";
   // char pipe2writeResult[50] = "Hellooo";
   // char readmessage[50];
   // returnstatus1 = pipe(pipefds1);

   // // readFromFile()

   // if (returnstatus1 == -1)
   // {
   //    printf("Unable to create pipe 1 \n");
   //    return 1;
   // }
   // returnstatus2 = pipe(pipefds2);

   // if (returnstatus2 == -1)
   // {
   //    printf("Unable to create pipe 2 \n");
   //    return 1;
   // }

   // menu();
   // int option;
   // scanf("%d", &option);
   // fflush(stdin);

   // pid = fork();
   // if (pid != 0) // Parent process
   // {
   //    close(pipefds1[0]);
   //    close(pipefds2[1]);
   //    // printf("In Parent: Writing to pipe 1 – Message is %s\n", pipe1writeCalculation);
   //    // write(pipefds1[1], pipe1writeCalculation, sizeof(pipe1writeCalculation));
   //    printf("In Parent: Writing to pipe 1 – Message is %d\n", option);
   //    char opt = option == 1 ? '1' : '2';
   //    write(pipefds1[1], &opt, 1);
   //    read(pipefds2[0], readmessage, sizeof(readmessage));
   //    printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage);
   // }
   // else
   // {                      //child process
   //    close(pipefds1[1]); // Close the unwanted pipe1 write side
   //    close(pipefds2[0]); // Close the unwanted pipe2 read side
   //    read(pipefds1[0], readmessage, sizeof(readmessage));

   //    printf("In Child: Reading from pipe 1 – Message is opt %s\n", readmessage);
   //    char hihi[40] = "oke ban oi ";
   //    // printf("In Child: Writing to pipe 2 – Message is %s\n", pipe2writeResult);
   //    // write(pipefds2[1], pipe2writeResult, sizeof(pipe2writeResult));
   //    printf("In Child: Writing to pipe 2 – Message is 1%s\n", hihi);
   //    write(pipefds2[1], hihi, sizeof(hihi));
   // }
   return 0;
}

void menu()
{
   printf("NHAP LUA CHON: \n");
   printf("1. Nhap bieu thuc\n");
   printf("2. Doc tu file input\n");
}
int getPriority(char c)
{
   if (c == '(')
      return 0;
   else if (c == '+' || c == '-' || c == '/' || c == '*')
      return 1;
   return 2;
}

bool isDigit(char c)
{
   if (c >= '0' && c <= '9')
      return true;
   return false;
}

int getNumber(char *str, int &i)
{
   int num = 0;
   char c = str[i];
   while (c >= '0' && c <= '9')
   {
      num = num * 10 + (c - '0');
      i++;
      c = str[i];
   };
   return num;
}

bool isOperation(char c)
{
   if (c == '-' || c == '+' || c == '/' || c == '*')
      return true;
   return false;
}

float calFromOperation(float nd1, float nd2, char op)
{
   switch (op)
   {
   case '+':
      return nd1 + nd2;
   case '-':
      return nd1 - nd2;
   case '*':
      return nd1 * nd2;
   case '/':
      if (nd2 == 0)
         return -9999;
      return nd1 / nd2;
   }
   return 0;
}

double calculator(Stack st, Queue q)
{
   float p1, p2;
   while (!isEmpty(q))
   {

      if (!isOperation(peek_char(q)))
      {
         push(st, 'x', remove_int(q));
      }
      else
      {
         p1 = (float)(pop_int(st));
         p2 = (float)(pop_int(st));
         float tempCal = calFromOperation(p2, p1, remove_char(q));
         push(st, 'x', (int)tempCal);
      }
   }
   return (double)(peek_int(st));
}

double balanAlth(char *expression)
{
   Stack st;
   Queue q;
   for (int i = 0; i < getLength(expression); i++)
   {
      char c = expression[i];
      if (isDigit(c) == true)
      {
         int num = getNumber(expression, i);
         i--;
         // i += num.length() - 1;
         offer(q, 'x', num);
      }

      else if (c == '(')
         push(st, c, 0);
      else if (c == ')')
      {
         char topSt;
         do
         {
            topSt = pop_char(st);
            if (topSt != '(')

               offer(q, topSt, 0);
         } while (topSt != '(');
      }
      else if (isOperation(c))
      {
         if (isEmpty(st))
         {
            push(st, c, 0);
         }
         else
         { // B
            char topPeek = peek_char(st);
            if (!isOperation(topPeek))
               push(st, c, 0);
            else
            {
               if (getPriority(topPeek) < getPriority(c))
                  push(st, c, 0);
               else
               {
                  while (!isEmpty(st) && isOperation(topPeek))
                  {
                     char topSt = pop_char(st);
                     offer(q, topSt, 0);
                  };
                  push(st, c, 0);
               }
            }
         }
      }
   }

   while (!isEmpty(st))
   {
      char topSt = pop_char(st);
      offer(q, topSt, 0);
   }
   return calculator(st, q);
}

int getLength(char *str)
{
   int i = 0;
   while (str[i])
   {
      i++;
   }
   return i;
}
