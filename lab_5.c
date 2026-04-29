#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createInputFile(char *text)
{
    FILE *f;

    f = fopen("input.txt", "w");
    fprintf(f, "%s", text);
    fclose(f);
}

void readFromInputFile(char *text)
{
    FILE *f;

    f = fopen("input.txt", "r");
    fgets(text, 1000, f);
    fclose(f);
}

int splitWords(char *text, char **words)
{
    int count = 0;
    char *word;

    word = strtok(text, " ");

    while (word != NULL)
    {
        words[count] = word;
        count++;

        word = strtok(NULL, " ");
    }

    return count;
}

void changeWords(char **words, int count)
{
    char *temp;
    int i;

    for (i = 0; i < count; i = i + 3)
    {
        if (i + 2 < count)
        {
            temp = words[i];
            words[i] = words[i + 2];
            words[i + 2] = temp;
        }
        else if (i + 1 < count)
        {
            temp = words[i];
            words[i] = words[i + 1];
            words[i + 1] = temp;
        }
    }
}

void printWords(char **words, int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        printf("%s ", words[i]);
    }

    printf("\n");
}

void writeOutputFile(char *initialText, char **words, int count)
{
    FILE *f;
    int i;

    f = fopen("output.txt", "w");

    fprintf(f, "Initial string:\n");
    fprintf(f, "%s\n\n", initialText);

    fprintf(f, "Changed string:\n");

    for (i = 0; i < count; i++)
    {
        fprintf(f, "%s ", words[i]);
    }

    fclose(f);
}

int main()
{
    char *text;
    char *initialText;
    char **words;
    int count;

    text = (char*)malloc(1000 * sizeof(char));
    initialText = (char*)malloc(1000 * sizeof(char));
    words = (char**)malloc(100 * sizeof(char*));

    printf("Enter the string:\n");
    fgets(text, 1000, stdin);

    createInputFile(text);

    text[strlen(text) - 1] = '\0';

    strcpy(initialText, text);

    readFromInputFile(text);

    count = splitWords(text, words);

    printf("\nInitial string:\n");
    printf("%s\n", initialText);

    changeWords(words, count);

    printf("Changed string:\n");
    printWords(words, count);

    writeOutputFile(initialText, words, count);

    free(text);
    free(initialText);
    free(words);

    return 0;
}
