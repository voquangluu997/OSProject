#include <fstream>

void readfromFile(char* str)
{
	int i=0; char t;
	FILE *f;
    f=fopen("input.txt","r");
    while((t = fgetc(f))!=EOF&& t=fgetc(f)!="\n")
    {
        str[i]=t;
        i++;
    }
    str[i]='\0';
    fclose(f);
}
int numberOfCharacter()
{
	int i=0; char t;
	FILE *f;
    f=fopen("input.txt","r");
    while((t = fgetc(f))!=EOF)
    {
        i++;
    }

    fclose(f);
    return i;
}