#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "stack.h"
#include "queue.h"

using namespace std;

void menu();
double balanAlth(char *expression);
int getLength(char *);
void enterExpression(int *pipe1, int *pipe2);
void getExpressionsFromFile();
char *doubleToString(double x);
void parentProcess(int *pipe1fds1, int *pipefds2, int option);
void childProcess(int *pipefds1, int *pipefds2);
int main()
{
   Stack st;
   Queue q;
   int pipefds1[2], pipefds2[2];
   int returnstatus1, returnstatus2;
   int pid;

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
      parentProcess(pipefds1, pipefds2, option);
   }
   else
   {
      childProcess(pipefds1, pipefds2);
   }
   return 0;
}
void menu()
{
   printf("NHAP LUA CHON: \n");
   printf("1. Nhap bieu thuc\n");
   printf("2. Doc tu file input\n");
}

//lay thu tu uu tien cac phan tu
int getPriority(char c)
{
   if (c == '(')
      return 0;
   else if (c == '+' || c == '-')
      return 1;
   return 2;
}

//kiem tra kt c co phai toan hang hay khong
bool isDigit(char c)
{
   if (c >= '0' && c <= '9')
      return true;
   return false;
}
//lay gia tri cua 1 so bao gom truong hop so do co do dai >= 2
double getNumber(char *str, int &i)
{
   int in = 0;
   double decimal = 0;
   bool isDecimal = false;
   int lenOfDecimal = 0;
   char c = str[i];
   while ((c >= '0' && c <= '9') || c == '.')
   {
      if (c != '.' && !isDecimal)
         in = in * 10 + (c - '0');
      if (c == '.')
         isDecimal = true;
      if (c != '.' && isDecimal)
      {
         decimal = decimal * 10 + (c - '0');
         lenOfDecimal++;
      }
      i++;
      c = str[i];
   };
   if (!isDecimal)
      return in;
   for (int i = 0; i < lenOfDecimal; i++)
      decimal /= 10;
   return (double)(in + decimal);
}
//kiem tra kt co phai toan tu hay khong
bool isOperation(char c)
{
   if (c == '-' || c == '+' || c == '/' || c == '*')
      return true;
   return false;
}
//thuc hien tinh toan nd1 *toan tu* nd2
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
      return (double)((int)((nd1 / nd2) * 100)) / 100;
   }
   return 0;
}
//tinh ket qua sau khi da chuyen xong bieu thuc qua dang hau to
double calculator(Stack st, Queue q)
{
   double p1, p2;
   while (!isEmpty(q))
   {
      if (!isOperation(peek_char(q)))
         push(st, 'x', remove_num(q));
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
//thuat toan ba lan
double balanAlth(char *expression)
{
   Stack st;
   Queue q;
   int len = getLength(expression);
   for (int i = 0; i < len; i++)
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
//lay do dai cua 1 xau ki tu
int getLength(char *str)
{
   int i = 0;
   while (str[i] && str[i] != '\0')
   {
      i++;
   }
   return i;
}
// nguoi dung chon nhap bieu thuc tu ban phim
void enterExpression(int *pipe1, int *pipe2)
{
   char *pipe1writeExpression = new char[100]; // lay bieu thuc nguoi dung nhap vao de gui cho tien trinh con
   char *readMessage = new char[100];          //nhan lai ket qua da tinh toan tu tien trinh con
   int returnStatus;
   cout << "In parent : Enter expression: ";
   cin >> pipe1writeExpression;
   cin.ignore();
   cout << "In Parent: Writing to pipe 1 - ms is " << pipe1writeExpression << endl;
   write(pipe1[1], pipe1writeExpression, sizeof(pipe1writeExpression));
   read(pipe2[0], readMessage, sizeof(readMessage));
   cout << "In Parent: Reading from pipe 2 – Result is " << readMessage << endl;
}
// thuc hien tien trinh cha
void parentProcess(int *pipefds1, int *pipefds2, int option)
{
   close(pipefds1[0]);
   close(pipefds2[1]);
   switch (option)
   {
   case 1:
      enterExpression(pipefds1, pipefds2);
      break;
   case 2:
      getExpressionsFromFile();
      break;
   }
}
// thuc hien tien trinh con
void childProcess(int *pipefds1, int *pipefds2)
{
   close(pipefds1[1]);                     // Close pipe1 write side
   close(pipefds2[0]);                     // Close pipe2 read side
   char *readMessage = new char[100];      //doc bieu thuc tu tien trinh cha
   char *pipe2writeResult = new char[100]; //ghi lai ket qua sau khi tinh toan de gui lai tien trinh cha
   read(pipefds1[0], readMessage, sizeof(readMessage));
   cout << "In Child: Reading from pipe 1 : expression readed from Parent process  is : " << readMessage << endl;
   double rs = balanAlth(readMessage);
   pipe2writeResult = doubleToString(rs);
   cout << "In Child: Writing to pipe 2 – Message is " << pipe2writeResult << endl;
   write(pipefds2[1], pipe2writeResult, sizeof(pipe2writeResult));
}
// truong hop lay bieu thuc doc tu file
void getExpressionsFromFile()
{
}
//chuyen ket qua tinh toan duoc tu kieu double sang xau ki tu
char *doubleToString(double x)
{
   char *y = new char[100];
   bool isNegative = x > 0 ? false : true;
   x = isNegative ? -x : x;
   int in = x;              //phan nguyen
   double decimal = x - in; //phan thap phan
   int size = 0;
   char yTemp[100];
   if (decimal != 0)
   {
      size++;                //dem dau cham
      int positionOfDot = 0; //vi tri cua dau cham
      int tempIn = in;
      do //xac dinh vi tri dau cham
      {
         tempIn /= 10;
         positionOfDot++;
      } while ((tempIn) > 0);
      yTemp[positionOfDot] = '.';
      int tempDot = positionOfDot;
      while ((tempDot) > 0)
      {
         yTemp[--tempDot] = (in % 10) + '0';
         in /= 10;
         size++;
      }
      int numOfDecimal = 0;                                     //so so thap phan
      while ((decimal - (int)decimal != 0) && numOfDecimal < 2) //dem so thap phan lam tron ve 2 so
      {
         decimal *= 10;
         yTemp[++positionOfDot] = ((int)decimal % 10) + '0';
         numOfDecimal++;
         size++;
      }
      for (int i = 0; i < size; i++)
      {
         y[i] = yTemp[i];
      }
   }
   else
   {
      int j = 0;
      char temp;
      while (in > 0)
      {
         yTemp[j++] = (in % 10) + '0';
         in /= 10;
         size++;
      }
      int sizeTemp = size;
      for (int i = 0; sizeTemp > 0; i++)
      {
         y[i] = yTemp[sizeTemp - 1];
         sizeTemp--;
      }
   }
   if (isNegative)
   {
      while (size > 0)
      {
         y[size] = y[size - 1];
         size--;
      }
      y[0] = '-';
   }
   return y;
}