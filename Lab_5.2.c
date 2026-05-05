#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    char name[30];
    char surname[30];
    char education[30];
    char specialty[40];
    float salary;
    Date startDate;
} Employee;

typedef struct
{
    int years;
    int months;
    int days;
} WorkTime;

int daysInMonth(int month, int year)
{
    if (month == 2)
    {
        if (year % 4 == 0)
            return 29;
        else
            return 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;

    return 31;
}

Date getToday()
{
    Date today;
    time_t t;
    struct tm *now;

    t = time(NULL);
    now = localtime(&t);

    today.day = now->tm_mday;
    today.month = now->tm_mon + 1;
    today.year = now->tm_year + 1900;

    return today;
}

WorkTime calculateWorkTime(Date start, Date today)
{
    WorkTime result;

    result.years = today.year - start.year;
    result.months = today.month - start.month;
    result.days = today.day - start.day;

    if (result.days < 0)
    {
        result.months--;
        result.days = result.days + daysInMonth(start.month, start.year);
    }

    if (result.months < 0)
    {
        result.years--;
        result.months = result.months + 12;
    }

    return result;
}

int educationCategory(char education[])
{
    if (strcmp(education, "bachelor") == 0)
        return 1;

    if (strcmp(education, "master") == 0)
        return 2;

    if (strcmp(education, "doctorate") == 0)
        return 3;

    return 4;
}

void readEmployees(Employee *e, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("\nEmployee %d\n", i + 1);

        printf("Name: ");
        scanf("%s", e[i].name);

        printf("Surname: ");
        scanf("%s", e[i].surname);

        printf("Education bachelor/master/doctorate: ");
        scanf("%s", e[i].education);

        printf("Specialty: ");
        scanf("%s", e[i].specialty);

        printf("Salary: ");
        scanf("%f", &e[i].salary);

        printf("Start date day month year: ");
        scanf("%d %d %d",
              &e[i].startDate.day,
              &e[i].startDate.month,
              &e[i].startDate.year);
    }
}

void writeExperiment(Employee *e, int n)
{
    FILE *f;
    int i;

    f = fopen("experiment.txt", "w");

    fprintf(f, "%d\n", n);

    for (i = 0; i < n; i++)
    {
        fprintf(f, "%s %s %s %s %.2f %d %d %d\n",
                e[i].name,
                e[i].surname,
                e[i].education,
                e[i].specialty,
                e[i].salary,
                e[i].startDate.day,
                e[i].startDate.month,
                e[i].startDate.year);
    }

    fclose(f);
}

void readExperiment(Employee *e, int n)
{
    FILE *f;
    int i;
    int tempN;

    f = fopen("experiment.txt", "r");

    fscanf(f, "%d", &tempN);

    for (i = 0; i < n; i++)
    {
        fscanf(f, "%s %s %s %s %f %d %d %d",
               e[i].name,
               e[i].surname,
               e[i].education,
               e[i].specialty,
               &e[i].salary,
               &e[i].startDate.day,
               &e[i].startDate.month,
               &e[i].startDate.year);
    }

    fclose(f);
}

void showEmployees(Employee *e, int n)
{
    int i;

    printf("\nData from experiment.txt:\n");

    for (i = 0; i < n; i++)
    {
        printf("\n%s %s\n", e[i].name, e[i].surname);
        printf("Education: %s\n", e[i].education);
        printf("Specialty: %s\n", e[i].specialty);
        printf("Salary: %.2f\n", e[i].salary);
        printf("Start date: %d.%d.%d\n",
               e[i].startDate.day,
               e[i].startDate.month,
               e[i].startDate.year);
    }
}

void sortByEducation(Employee *e, int n)
{
    int i, j;
    Employee temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - 1; j++)
        {
            if (educationCategory(e[j].education) > educationCategory(e[j + 1].education))
            {
                temp = e[j];
                e[j] = e[j + 1];
                e[j + 1] = temp;
            }
        }
    }
}

void writeOutput(Employee *e, int n)
{
    FILE *f;
    int i;
    Date today;
    WorkTime wt;

    today = getToday();

    sortByEducation(e, n);

    f = fopen("output.txt", "w");

    for (i = 0; i < n; i++)
    {
        wt = calculateWorkTime(e[i].startDate, today);

        fprintf(f, "%s %s | %s | %s | %.2f | %d years %d months %d days\n",
                e[i].name,
                e[i].surname,
                e[i].education,
                e[i].specialty,
                e[i].salary,
                wt.years,
                wt.months,
                wt.days);
    }

    fclose(f);
}

void showFile(char fileName[])
{
    FILE *f;
    char ch;

    f = fopen(fileName, "r");

    printf("\nContent of %s:\n", fileName);

    while ((ch = fgetc(f)) != EOF)
    {
        printf("%c", ch);
    }

    fclose(f);
}

void copyBlock()
{
    FILE *out;
    FILE *exp;
    char block[300];
    char oldText[3000];
    fpos_t pos;
    int start;
    int length;
    int place;
    int i;
    int oldLength;

    printf("\nStart position from output.txt: ");
    scanf("%d", &start);

    printf("Block length: ");
    scanf("%d", &length);

    printf("Insert position in experiment.txt:\n");
    printf("1 - beginning\n");
    printf("2 - end\n");
    printf("Choose: ");
    scanf("%d", &place);

    out = fopen("output.txt", "r");

    fseek(out, start, SEEK_SET);
    fgetpos(out, &pos);
    fsetpos(out, &pos);

    for (i = 0; i < length; i++)
    {
        block[i] = fgetc(out);
    }

    block[length] = '\0';

    fclose(out);

    exp = fopen("experiment.txt", "r");

    i = 0;
    while ((oldText[i] = fgetc(exp)) != EOF)
    {
        i++;
    }

    oldText[i] = '\0';
    oldLength = i;

    fclose(exp);

    exp = fopen("experiment.txt", "w");

    if (place == 1)
    {
        fprintf(exp, "%s", block);
        fprintf(exp, "%s", oldText);
    }
    else
    {
        fprintf(exp, "%s", oldText);
        fprintf(exp, "%s", block);
    }

    fclose(exp);
}

int main()
{
    Employee *employees;
    int n;

    printf("Number of employees: ");
    scanf("%d", &n);

    employees = (Employee *)malloc(n * sizeof(Employee));

    readEmployees(employees, n);

    writeExperiment(employees, n);

    readExperiment(employees, n);

    showEmployees(employees, n);

    writeOutput(employees, n);

    showFile("output.txt");

    copyBlock();

    showFile("experiment.txt");

    free(employees);

    return 0;
}
