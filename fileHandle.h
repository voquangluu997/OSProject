#include <fstream>

void getExpressionsFromFile(char str[][100])
{
    int i = 0;
    int j = 0;
    char t;
    FILE *f;
    f = fopen("input.txt", "r");
    while ((t = fgetc(f)) != EOF)
    {
        str[i][j] = t;
        j++;
        if (t == '\n')
        {
            i++;
            j = 0;
        }
    }
    fclose(f);
}

int getLinesOfFile()
{
    int count = 0;
    bool isEmpty = true;
    char t;
    FILE *f;
    f = fopen("input.txt", "r");
    while ((t = fgetc(f)) != EOF)
    {
        isEmpty = false;
        if (t == '\n')
        {
            count++;
        }
    }
    fclose(f);
    return isEmpty ? 0 : count + 1;
}

void writeToFile(char data[][100])
{
    FILE *file;
   char expressions[100][100];
   getExpressionsFromFile(expressions);
   const char *filePath = "output.txt";
   int linesOfFile = getLinesOfFile();
   file = fopen(filePath, "w");
   if (!file)
      std::cout << "Can not open this file" << std::endl;
   for (int i = 0; i < linesOfFile; i++)
      fprintf(file, "%s = %s\n", expressions[i], data[i]);
   fclose(file);
}