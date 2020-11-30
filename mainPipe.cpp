#include <stdio.h>
#include <unistd.h>
#include <iostream>
// #include "fileHandle.h"
#include "stack.h"
#include "queue.h"

using namespace std;

void menu();
double balanAlth(char *expression);
int getLength(char *);
void enterExpression(int *pipe1, int *pipe2);
void getExpressionsFromFile();
char *doubleToString(double x, char *y);
bool isOperation(char c);

int main()
{
   char xx[100];
   doubleToString(-23, xx);
   cout << "amen " << xx;
   Stack st;
   Queue q;
   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   int pid;
   char pipe1writeExpression[100];
   char pipe2readExpression[100];
   char pipe1readResult[100];
   char pipe2writeResult[100];
   char readMessage[100];

   returnstatus1 = pipe(pipefds1);
   if (returnstatus1 == -1)
   {
      printf("Unable to create pipe 1 \n");
      return 1;
   }

   returnstatus2 = pipe(pipefds2);

   if (returnstatus2 == -1)
   {
      printf("Unable to create pipe 2 \n");
      return 1;
   }

   menu();
   int option;
   cin >> option;
   // fflush(stdin);
   cin.ignore();

   pid = fork();
   if (pid != 0) // Parent process
   {
      close(pipefds1[0]);
      close(pipefds2[1]);

      switch (option)
      {
      case 1:
         // char c[100];
         // enterExpression(pipefds1, pipefds2);
         // char c[100];
         int returnStatus;
         cout << "In parent : nhap bieu thuc : ";
         cin >> pipe1writeExpression;
         cout << "In Parent: Writing to pipe 1 - ms is " << pipe1writeExpression << endl;
         write(pipefds1[1], pipe1writeExpression, sizeof(pipe1writeExpression));
         read(pipefds2[0], readMessage, sizeof(readMessage));
         cout << "In Parent: Reading from pipe 2 – Message is " << readMessage << endl;
         break;
      case 2:
         getExpressionsFromFile();
         break;
      }
   }
   else
   {
      close(pipefds1[1]); // Close pipe1 write side
      close(pipefds2[0]); // Close pipe2 read side
      char c[100];
      // int returnStatus;
      // int i = 0;
      read(pipefds1[0], readMessage, sizeof(readMessage));
      cout << "In Child: Reading from pipe 1 : bieu thuc doc duoc tu Parent process  la : " << readMessage << endl;
      double rs = balanAlth(readMessage);
      cout << " kq theo balan la : " << rs << endl;
      doubleToString(rs, c);
      cout << "kq convert thanh chuoi la : " << c << endl;
      cout << "In Child: Writing to pipe 2 – Message is " << c << endl;
      write(pipefds2[1], c, sizeof(c));
   }
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
   else if (c == '+' || c == '-')
      return 1;
   return 2;
}

bool isDigit(char c)
{
   if (c >= '0' && c <= '9')
      return true;
   return false;
}

double getNumber(char *str, int &i)
{
   int in = 0;
   int decimal = 0;
   bool isDecimal = false;
   int lenOfDecimal = 0;
   double result;
   char c = str[i];
   while ((c >= '0' && c <= '9') || c == '.')
   {
      if (c != '.' && !isDecimal)
      {
         in = in * 10 + (c - '0');
      }
      if (c == '.')
      {

         isDecimal = true;
      }
      if (c != '.' && isDecimal)
      {
         decimal = decimal * 10 + (c - '0');
         lenOfDecimal++;
      }
      i++;
      c = str[i];
   };
   int temp = lenOfDecimal;
   while (temp > 0)
   {
      in = in * 10 + decimal;
      temp--;
      result = in;
   }
   while (lenOfDecimal > 0)
   {
      result = result / 10;
      lenOfDecimal--;
   }
   result = isDecimal ? result : in;
   return result;
}

bool isOperation(char c)
{
   if (c == '-' || c == '+' || c == '/' || c == '*')
      return true;
   return false;
}

double calFromOperation(double nd1, double nd2, char op)
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
   double p1, p2;
   while (!isEmpty(q))
   {

      if (!isOperation(peek_char(q)))
      {
         push(st, 'x', remove_num(q));
      }
      else
      {
         p1 = (double)(pop_num(st));
         p2 = (double)(pop_num(st));
         double tempCal = calFromOperation(p2, p1, remove_char(q));
         push(st, 'x', tempCal);
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
         double num = getNumber(expression, i);
         i--;
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
   while (str[i] && str[i] != '\0')
   {
      i++;
   }
   return i;
}

void enterExpression(int *pipe1, int *pipe2)
{
   char c[100], readRs[100];
   int returnStatus;
   cout << "In parent : nhap bieu thuc : ";
   cin >> c;
   write(pipe1[1], c, sizeof(c));
   read(pipe2[0], readRs, sizeof(readRs));
   cout << " doc tu cha ne : " << c << endl;
}
void getExpressionsFromFile()
{
}
char *doubleToString(double x, char *y)
{
   bool isNegative = x > 0 ? false : true;
   x = isNegative ? -x : x;
   int in = x;
   double decimal = x - in;

   if (decimal != 0)
   {
      int positionOfDot = 0;
      int tempIn = in;
      do
      {
         tempIn /= 10;
         positionOfDot++;
      } while ((tempIn) > 0);
      y[positionOfDot] = '.';
      int tempDot = positionOfDot;
      while ((tempDot) >= 0)
      {
         y[--tempDot] = (in % 10) + '0';
         in /= 10;
      }
      int numOfDecimal = 0;
      while ((decimal - (int)decimal != 0) && numOfDecimal < 2)
      {
         decimal *= 10;
         y[++positionOfDot] = ((int)decimal % 10) + '0';
         numOfDecimal++;
      }
   }
   else
   {
      int count = 0;
      int j = 0;
      char temp;
      while (in > 0)
      {
         y[j++] = (in % 10) + '0';
         in /= 10;
         // count++;
      }
      while (j >= count)
      {
         temp = y[j - 1];
         y[j - 1] = y[count];
         j--;
         y[count++] = temp;
      }
   }

   // 2.5
   if (isNegative)
   {
      cout << "y : " << y << "len : " << getLength(y) << endl;
      int size = 0;
      int i = 0;
      while (y[size] && y[size] != '\0' && y[size] != EOF)
      {
         cout << y[i] << endl;
         size++;
      }
      // getLength(y);
      int t = 0;
      while (size - t > 0)
      {
         y[size - t] = y[size - t - 1];
         t++;
      }
      y[0] = '-';
   }
   return y;
}