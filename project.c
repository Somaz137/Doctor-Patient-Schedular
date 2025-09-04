#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <windows.h>
#include<conio.h>
#define MaxArrayLength 50
#define MaxAppointment 3
HANDLE hConsole;// for colorful display, by chatgpt


typedef struct{
    int doctorID;
    char name[MaxArrayLength];
    char timing[MaxArrayLength];
    int appointmentLimit;
} doctor;

typedef struct{
    char doctorName[MaxArrayLength];
    char availableAppointments[MaxAppointment][MaxArrayLength];// 3 cuz to limit doctor working hours. yet to implement 
} appointments;

typedef struct{
    char doctorName[MaxArrayLength];
    char bookedAppointments[MaxAppointment][MaxArrayLength];
    char patientName[MaxAppointment][MaxArrayLength];
} bookedAppointments;

void printHeader();
int countDoctorsInFile();
void loadDoctorList(doctor doctorList[], int );
void dispDoctorList(doctor doctorList[], int);
void loadDoctorAppointments(appointments doctorAppointments[], int);
void dispDoctorAppointments(appointments doctorAppointments[], bookedAppointments doctorBookedAppointments[], int, int);
void loadBookedAppointments(bookedAppointments doctorBookedAppointments[], int);
void dispChosenDoctor(doctor doctorList[], int, int);
int scheduleAppointment(appointments doctorAppointments[], bookedAppointments doctorBookedAppointments[], int, int *, int);
void saveBookedAppointments(bookedAppointments doctorBookedAppointments[], int);
int inputStringToInt(char userInput[]);
void removeNewLine(char userInput[]);
void drawBorder();
void setColor(int );
int main()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //initialize the global variable once for colorful display, by gpt
    int numberOfDoctors = countDoctorsInFile();
    int doctorIDChoice, userOptionChoice, appointmentID;
    char userInput[MaxArrayLength];
    doctor doctorList[numberOfDoctors];
    appointments doctorAppointments[numberOfDoctors];
    bookedAppointments doctorBookedAppointments[numberOfDoctors];
    // getch();
    printHeader();
    loadDoctorList(doctorList, numberOfDoctors);
    loadDoctorAppointments(doctorAppointments, numberOfDoctors);
    loadBookedAppointments(doctorBookedAppointments, numberOfDoctors);
    do{
        GoBackToDoctorList://return from after the dispChosenDoctor in interface
        dispDoctorList(doctorList, numberOfDoctors);
        drawBorder();
        setColor(10);
        printf("\t1. Select Doctor \n");
        setColor(12);
        printf("\tE. Exit Program");
        setColor(15);
        drawBorder();
        // setColor(6);
        printf("\tSelect Option: ");
        // setColor(15);
        fgets(userInput, sizeof(userInput), stdin);

        if((userInput[0] == 'E' || userInput[0] =='e') && userInput[1] == '\n')//because only 1 character so only checking first character
        {
            exit(1);
        }
        else if(inputStringToInt(userInput)!=1)
        {
            setColor(12);
            printf("\n\tInvalid choice!\n\tPress any key.\n");
            setColor(15);
            getch();
            continue;
        }

        dispDoctorList(doctorList, numberOfDoctors);
        drawBorder();
        // setColor(6);
        printf("\tEnter the Doctor ID: ");
        // setColor(15);
        fgets(userInput, sizeof(userInput), stdin);
        doctorIDChoice = inputStringToInt(userInput);

        if(doctorIDChoice>0 && doctorIDChoice<=numberOfDoctors)
        {
            break;
            //the loop keeps running until user selects a valid choice, then the control goes out of loop. disp statement right after loop.
        }
        else
        {
            setColor(12);
            printf("\tInvalid choice!\nPress any key.\n");
            setColor(15);
            getch();
        }
    }while(1);

    do{
        GoBackToDoctorAppointments://return from after the dispDoctorAppointments
        dispChosenDoctor( doctorList, numberOfDoctors, doctorIDChoice-1);
        drawBorder();
        setColor(10);
        printf("\t1. See Available Appointments");
        setColor(6);
        printf("\n\tB. Go Back");
        setColor(12);
        printf("\n\tE. Exit Program");
        setColor(15);
        drawBorder();
        printf("\tSelect option: ");
        fgets(userInput, sizeof(userInput), stdin);

        if((userInput[0] == 'e' || userInput[0] == 'E') && userInput[1] == '\n')
        {
            exit(1);
        }
        else if(inputStringToInt(userInput) == 1)//See available appointment
        {
            break;
        }
        else if((userInput[0] == 'b' || userInput[0] == 'B') && userInput[1] == '\n')//go back
        {
            goto GoBackToDoctorList;//to dispDoctorList AKA previous screen
        }
        else
        {
            setColor(12);
            printf("\tInvalid choice!\n\tPress any key.\n");
            setColor(15);
            getch();
        }
    }while(1);

    do{
        dispDoctorAppointments(doctorAppointments, doctorBookedAppointments, doctorIDChoice-1, numberOfDoctors);
        drawBorder();
        setColor(10);
        printf("\t1. Schedule an Appointment");
        setColor(6);
        printf("\n\tB. Go Back");
        setColor(12);
        printf("\n\tE. Exit Program");
        setColor(15);
        drawBorder();
        // setColor(6);
        printf("\tSelect Option: ");
        // setColor(15);
        fgets(userInput, sizeof(userInput), stdin);

        if((userInput[0] == 'e' || userInput[0] == 'E') && userInput[1] == '\n')
        {
            exit(1);
        }
        else if(inputStringToInt(userInput) == 1)//schedule an appointment
        {
            if(scheduleAppointment(doctorAppointments, doctorBookedAppointments, doctorIDChoice-1, &appointmentID, numberOfDoctors))
                break;
            else
                continue;
        }
        else if((userInput[0] == 'b' || userInput[0] == 'B') && userInput[1] == '\n')//go back
        {
            goto GoBackToDoctorAppointments;//to dispDoctorDoctorAppointments AKA previous screen
        }
        else
        {
            setColor(12);
            printf("\tInvalid choice!\n\tPress any key.\n");
            setColor(15);
            getch();
        }
    }while(1);

    saveBookedAppointments(doctorBookedAppointments, numberOfDoctors);
    goto GoBackToDoctorAppointments;
}
void printHeader()
{
    system("cls");
    // setColor(10); //bright green
    printf("\t\t\311\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\273\n");
    printf("\t\t\272  WELCOME TO THE DOCTOR PATIENT SCHEDULER  \272\n");
    printf("\t\t\310\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\274\n\n");
    // setColor(15);// reset to white
}
int countDoctorsInFile()
{
    int count=0;
    int doctorID;
    char name[MaxArrayLength];
    char timing[MaxArrayLength];
    int appointmentLimit;
    FILE *fp;
    fp = fopen("doctors.txt", "r");
    if(fp == NULL)
    {
        printf("Error reading doctor file.");
        exit(1);
    }
    int i=0;
    while(fscanf(fp, "%d,%[^,],%[^,],%d\n", &doctorID, name, timing, &appointmentLimit) == 4)
    {
    /* %[^,] means it will read till ',' >> cursor position |,
    then next ',' means scanf will encounter it and skip it, same with '\n' at the end >> curson position ,|
    if there is no ',' or some other character, fscanf will exit, or stop reading.
    == 4 means while loop will run as long as 4 successful readings.*/
        i++;
        count++;
    }
    fclose(fp);
    return count;
}
void loadDoctorList(doctor doctorList[], int arraySize)
{
    FILE *fp;
    fp = fopen("doctors.txt", "r");
    if(fp == NULL)
    {
        setColor(12);
        printf("Error reading doctor file.\nPress any key to exit.");
        setColor(15);
        getch();
        exit(1);
    }
    int i=0;
    int count=0;
    while(i<arraySize && fscanf(fp, "%d,%[^,],%[^,],%d\n", &doctorList[i].doctorID, doctorList[i].name, doctorList[i].timing, &doctorList[i].appointmentLimit) == 4)
    {
    /* %[^,] means it will read till ',' >> cursor position |,
    then next ',' means scanf will encounter it and skip it, same with '\n' at the end >> curson position ,|
    if there is no ',' or some other character, fscanf will exit, or stop reading.
    == 4 means while loop will run as long as 4 successful readings.*/
        i++;
        count++;
    }
    fclose(fp);
    }
void dispDoctorList(doctor doctorList[], int arraySize)
{
    system("cls");
    printHeader();
    setColor(11);
    printf("\t\tLIST OF DOCTORS   ");
    setColor(15);
    drawBorder();
    setColor(9);
    printf("\t%-15s %-20s", "DOCTOR ID", "DOCTOR NAME");
    setColor(15);
    drawBorder();
    // drawBorder();
    for(int i=0; i<arraySize; i++)
    {
        printf("\t     %-9d %s\n", doctorList[i].doctorID, doctorList[i].name);
    }
}
void dispChosenDoctor(doctor doctorList[], int arraySize, int DoctorIDChoice)
{
    system("cls");
    printHeader();
    drawBorder();
    setColor(11);
    printf("\t  %-20s %-20s %-20s ", "DOCTOR NAME", "DOCTOR TIMING", "MAX APPOINTMENTS");
    setColor(15);
    drawBorder();
    if (DoctorIDChoice >= 0 && DoctorIDChoice < arraySize)
    {
        printf("\t%-20s %-20s %9d\n\n", doctorList[DoctorIDChoice].name, doctorList[DoctorIDChoice].timing, doctorList[DoctorIDChoice].appointmentLimit);
    }
}
void loadDoctorAppointments(appointments doctorAppointments[], int arraySize)
{
    FILE *fp;
    fp = fopen("appointments.txt", "r");
    if(fp == NULL)
    {
        setColor(12);
        printf("Error reading doctor file.");
        setColor(15);
        exit(1);
    }
    int i=0;
    while(i<arraySize && fscanf(fp, "%[^,],%[^,],%[^,],%[^\n]\n", doctorAppointments[i].doctorName, doctorAppointments[i].availableAppointments[0], doctorAppointments[i].availableAppointments[1], doctorAppointments[i].availableAppointments[2]) == 4)
    {
        i++;
    }
    fclose(fp);
}
void dispDoctorAppointments(appointments doctorAppointments[], bookedAppointments doctorBookedAppointments[], int DoctorIDChoice, int numberOfDoctors)
{
    system("cls");
    printHeader();
    drawBorder();
    setColor(9);
    printf("\t  %-17s %-25s %-20s %-20s", "DOCTOR NAME", "APPOINTMENTS AVAILABLE", "APPOINTMENT STATUS", "PATIENT NAME");
    setColor(15);
    drawBorder();
    if (DoctorIDChoice >= 0 && DoctorIDChoice < numberOfDoctors)
    {
        printf("\t%-20s", doctorAppointments[DoctorIDChoice].doctorName);
        if(!(strcmp(doctorAppointments[DoctorIDChoice].availableAppointments[0], doctorBookedAppointments[DoctorIDChoice].bookedAppointments[0])))
        {
            printf("1.%20s", doctorAppointments[DoctorIDChoice].availableAppointments[0]);
            setColor(12);
            printf("%15s", "     Booked");
            setColor(3);
            printf("%10s%s\n", " " ,doctorBookedAppointments[DoctorIDChoice].patientName[0]);
            setColor(15);
        }
        else
        {
            printf("1.%20s", doctorAppointments[DoctorIDChoice].availableAppointments[0]);
            setColor(10);
            printf("%15s\n", doctorBookedAppointments[DoctorIDChoice].bookedAppointments[0]);
            setColor(15);
        }

        if(!(strcmp(doctorAppointments[DoctorIDChoice].availableAppointments[1], doctorBookedAppointments[DoctorIDChoice].bookedAppointments[1])))
        {
            printf("\t%20s2.%20s", " ", doctorAppointments[DoctorIDChoice].availableAppointments[1]);
            setColor(12);
            printf("%15s", "     Booked");
            setColor(3);
            printf("%10s%s\n", " " ,doctorBookedAppointments[DoctorIDChoice].patientName[1]);
            setColor(15);
        }
        else 
        {
            printf("\t%20s2.%20s", " ", doctorAppointments[DoctorIDChoice].availableAppointments[1]);
            setColor(10);
            printf("%15s\n", doctorBookedAppointments[DoctorIDChoice].bookedAppointments[1]);
            setColor(15);
        }

        if(!(strcmp(doctorAppointments[DoctorIDChoice].availableAppointments[2], doctorBookedAppointments[DoctorIDChoice].bookedAppointments[2])))
        {
            printf("\t%20s2.%20s", " ", doctorAppointments[DoctorIDChoice].availableAppointments[2]);
            setColor(12);
            printf("%15s", "     Booked");
            setColor(3);
            printf("%10s%s\n", " " ,doctorBookedAppointments[DoctorIDChoice].patientName[2]);
            setColor(15);
        }
        else
        {
            printf("\t%20s3.%20s", " ", doctorAppointments[DoctorIDChoice].availableAppointments[2]);
            setColor(10);
            printf("%15s\n", doctorBookedAppointments[DoctorIDChoice].bookedAppointments[2]);
            setColor(15);
        }
    }

}
void loadBookedAppointments(bookedAppointments doctorBookedAppointments[], int arraySize)
{
    FILE *fp;
    fp = fopen("bookedAppointments.txt", "r");
    if(fp == NULL)
    {
        printf("Error reading bookedAppointments file.");
        exit(1);
    }
    int i=0;
    while(i<arraySize)
    {
        fscanf(fp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", 
            doctorBookedAppointments[i].doctorName, 
            doctorBookedAppointments[i].bookedAppointments[0], 
            doctorBookedAppointments[i].bookedAppointments[1], 
            doctorBookedAppointments[i].bookedAppointments[2], 
            doctorBookedAppointments[i].patientName[0], 
            doctorBookedAppointments[i].patientName[1], 
            doctorBookedAppointments[i].patientName[2]);
            removeNewLine(doctorBookedAppointments[i].patientName[2]);
        // doctorBookedAppointments[i].patientName[2][strcspn(doctorBookedAppointments[i].patientName[2], "\n")]='\0';
        i++;
    }
    fclose(fp);
}
int scheduleAppointment(appointments doctorAppointments[], bookedAppointments doctorBookedAppointments[], int doctorIDChoice, int *appointmentID, int numberOfDoctors)
{
    char confirmChoice;
    char userInput[MaxArrayLength];
    while(1)
    {
        ChooseAgain:
        dispDoctorAppointments(doctorAppointments, doctorBookedAppointments, doctorIDChoice, numberOfDoctors);//doctorIDChoice is already -1 when sent to this function
        drawBorder();
        setColor(4);
        printf("\tB. Go Back");
        setColor(15);
        drawBorder();
        printf("\tEnter the appointment ID: ");
        fgets(userInput, sizeof(userInput), stdin);
        *appointmentID=inputStringToInt(userInput);
        // getchar();//to remove the \n from enter in scanf, so fgets doesnt take \n as input ahead

        if((userInput[0] == 'b' || userInput[0] == 'B') && userInput[1] == '\n')
            return 0;

        if(*appointmentID>0 && *appointmentID<=3)
        {
            *appointmentID-=1;// to start from 0, because of index
            if(!(strcmp(doctorAppointments[doctorIDChoice].availableAppointments[*appointmentID], doctorBookedAppointments[doctorIDChoice].bookedAppointments[*appointmentID])))
            {    
                setColor(12);
                printf("\n\tThe slot is already Booked.\n\tPress any key.");
                setColor(15);
                getch();
                continue;
            }
            else
            {
                while(1)
                {
                    printf("\n\tEnter Your Name: ");
                    fgets(doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID], sizeof(doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID]), stdin);
                    removeNewLine(doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID]);
                    // doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID][strcspn(doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID], "\n")]='\0';
                    setColor(6);
                    printf("\n\tConfirm?(Y/N): ");
                    setColor(15);
                    fgets(userInput, sizeof(userInput), stdin);
                    // getchar();
                    if((userInput[0] == 'Y' || userInput[0] == 'y') && userInput[1] == '\n')
                        break;
                    if((userInput[0] == 'N' || userInput[0] == 'n') && userInput[1] == '\n')
                    {
                        strcpy(doctorBookedAppointments[doctorIDChoice].patientName[*appointmentID], "");
                        goto ChooseAgain;
                    }
                    else
                    {
                        setColor(12);
                        printf("\n\tInvalid Choice.\n");
                        setColor(15);
                        continue;
                    }   
                }
                strcpy(doctorBookedAppointments[doctorIDChoice].bookedAppointments[*appointmentID], doctorAppointments[doctorIDChoice].availableAppointments[*appointmentID]);
                setColor(10);
                printf("\tAppointment Scheduled Successfully!\n\tPress any key");
                setColor(15);
                getch();
                return 1;
            }
        }
        else
        {
            setColor(12);
            printf("\tInvalid Appointment.\n\tPress any key.");
            setColor(15);
            getch();
            continue;
        }
    // break;
    }
}
void saveBookedAppointments(bookedAppointments doctorBookedAppointments[], int arraySize)
{
    FILE *fp;
    fp = fopen("bookedAppointments.txt", "w+");
    if(fp == NULL)
    {
        printf("Error writing bookedAppointments file.");
        exit(1);
    }
    int i=0;
    while(i<arraySize)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n", 
            doctorBookedAppointments[i].doctorName, 
            doctorBookedAppointments[i].bookedAppointments[0], 
            doctorBookedAppointments[i].bookedAppointments[1], 
            doctorBookedAppointments[i].bookedAppointments[2], 
            doctorBookedAppointments[i].patientName[0], 
            doctorBookedAppointments[i].patientName[1], 
            doctorBookedAppointments[i].patientName[2]);
        i++;
    }
    fclose(fp);
}
int inputStringToInt(char userInput[])
{
    return atoi(userInput);
}
void removeNewLine(char userInput[])
{
    userInput[strcspn(userInput, "\n")] = '\0';
}
void drawBorder()
{
    printf("\n\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\315\n");
}
void setColor(int color)
{
    SetConsoleTextAttribute(hConsole, color);
}
//replace the size with an actual variable when calling load doctor in main.
//show the typing on the screen like below:
/*for (int i = 0; doctorList[0].name[i] != '\0'; i++) 
    {
        putchar(doctorList[0].name[i]);
        Sleep(50);
    }
*/
//add exits at every choice