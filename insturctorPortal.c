#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

struct studentsData
{
    char studentID[500];
    char studentName[500];
    char *lectureID;
    char *username;
};

struct lecturesData
{
    char ID[500], name[500], time[500], *username;
};

FILE *fp;
FILE *loginPointer;
FILE *checkPtr;
FILE *studentPtr;

void showMenu();
void login(char id[500], char password[500]);
void instructorMenu(char username[500]);
void addStudents(char username[500]);
void addLectures(char username[500]);
void absence(char username[500]);
int searchLecture(char id[500], char username[500]);
int searchStudent(char id[500], char lectureID[500], char username[500]);
void showStudents(char username[500]);
void adminMenu(char username[500]);
void addAbsence(char id[500], char lectureID[500], char username[500]);

void showSpecificUser(char command[500]);
void addUsers(char command[500]);
void showUsers(char username[500]);
void showFiles(char username[500], int choice);
void clientSocket(char username[500]);

void logout()
{
    int pid = fork();
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        system("echo 'LoggedOut' > check.txt");
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}
void addingPermission(char *command)
{
    int pid = fork();
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        system(command);
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}
void backup()
{
    int pid = fork();
    int retVal;

    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        retVal = system("./backup.sh");
        exit(0);
    }
    else
    {
        wait(NULL);
        if (retVal <= -1)
        {
            printf("\n========================================================");
            printf("\n\tError in viewing users\n");
            printf("========================================================\n");
            return;
        }

        return;
    }
}
void backupView()
{
    int pid = fork();

    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        system("ls -l | grep 'backup.zip'");

        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}

void addLoginLog(char username[500])
{
    int pid = fork();

    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        execl("numberOfLogins.sh", "numberOfLogins.sh", username, NULL);

        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}

void *threadProccess(void *buffer)
{
    char check[500];
    char username[500];
    sscanf(buffer, "%s %s", check, username);
    while (1)
    {

        if (strcmp(check, "InvalidLogin") == 0)
        {
            printf("\n========================================================\n");
            printf("\tInvalid Login\n");
            printf("========================================================\n");
            loginPointer = fopen("check.txt", "r");
            showMenu();
            fgets(buffer, 500, loginPointer);
            sscanf(buffer, "%s %s", check, username);
            fclose(loginPointer);
            if (strcmp(check, "LoggedIn") == 0 && strcmp(username, "rootUser") == 0)
            {
                addLoginLog(username);
                adminMenu(username);
            }
            else if (strcmp(check, "LoggedIn") == 0)
            {
                addLoginLog(username);
                instructorMenu(username);
            }
        }
        else if (strcmp(check, "LoggedIn") == 0 && strcmp(username, "rootUser") == 0)
        {
            addLoginLog(username);
            adminMenu(username);
        }
        else if (strcmp(check, "LoggedIn") == 0)
        {
            addLoginLog(username);
            instructorMenu(username);
        }

        else if (strcmp(check, "LoggedOut") == 0)
        {
            loginPointer = fopen("check.txt", "r");
            showMenu();
            fgets(buffer, 500, loginPointer);
            sscanf(buffer, "%s %s", check, username);
            fclose(loginPointer);

            if (strcmp(check, "LoggedIn") == 0 && strcmp(username, "rootUser") == 0)
            {
                addLoginLog(username);
                adminMenu(username);
            }
            else if (strcmp(check, "LoggedIn") == 0)
            {
                addLoginLog(username);
                instructorMenu(username);
            }
        }
        else
        {
            printf("Error while logging in");
            pthread_exit(0);
            break;
        }


    }
    return NULL;
}

int main(int argc, char *argv[])
{

    char buffer[500];
    int i = 1;
    int loginStatus;
    pthread_t thread;

    system("chmod 700 login.sh numberOfLogins.sh");

    loginPointer = fopen("check.txt", "r");
    if (loginPointer == NULL)
    {
        showMenu();
        loginPointer = fopen("check.txt", "r");
    }

    fgets(buffer, 500, loginPointer);
    fclose(loginPointer);

    if (pthread_create(&thread, 0, threadProccess, (void *)buffer))
    {
        printf("\n=================================================\n");
        printf("\tError in creating thread\n");
        printf("=================================================\n");
        return -1;
    }

    if (pthread_join(thread, NULL))
    {
        printf("\n=================================================\n");
        printf("\tError in joining thread\n");
        printf("=================================================\n");
        return -1;
    }
    return 0;
}

void showMenu()
{
    char id[500];
    char pass[500];

    printf("Login to instructors portal:\n");
    printf("========================================================\n");
    printf("ID: ");
    scanf("%s", id);
    printf("Password: ");
    scanf("%s", pass);
    login(id, pass);
    return;
}

void login(char id[500], char password[500])
{

    int pid = fork();
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        execl("login.sh", "login.sh", id, password, NULL);
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}

int searchLecture(char id[500], char username[500])
{
    int retval;
    char buffer[500];
    char check[500];
    int pid = fork();

    checkPtr = fopen("lectureSearch.txt", "r");

    if (checkPtr == NULL)
    {
        printf("\n========================================================");
        printf("\nError in Finding lecture\n");
        printf("========================================================\n");
    }
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        execl("searchLecture.sh", "searchLecture.sh", id, username, NULL);
        exit(0);
    }
    else
    {
        wait(NULL);

        fgets(buffer, 500, checkPtr);
        sscanf(buffer, "%s", check);

        if (strcmp(check, "Found") == 0)
        {
            fclose(checkPtr);

            return 1;
        }
        else if (strcmp(check, "NotFound") == 0)
        {
            fclose(checkPtr);

            return 0;
        }
        else
        {
            fclose(checkPtr);

            return 0;
        }
    }
}

int searchStudent(char id[500], char lectureID[500], char username[500])
{
    int retval;
    char buffer[500];
    char check[500];
    int pid = fork();

    checkPtr = fopen("studentSearch.txt", "r");

    if (checkPtr == NULL)
    {
        printf("\n========================================================");
        printf("\nError in Finding the Student\n");
        printf("========================================================\n");
    }
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        execl("searchStudent.sh", "searchStudent.sh", id, lectureID, username, NULL);
        exit(0);
    }
    else
    {
        wait(NULL);

        fgets(buffer, 500, checkPtr);
        sscanf(buffer, "%s", check);

        if (strcmp(check, "Found") == 0)
        {
            fclose(checkPtr);

            return 1;
        }
        else if (strcmp(check, "NotFound") == 0)
        {
            fclose(checkPtr);

            return 0;
        }
        else
        {
            fclose(checkPtr);

            return 0;
        }
    }
}

void addAbsence(char id[500], char lectureID[500], char username[500])
{
    int pid = fork();
    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        execl("absenceManager.sh", "absenceManager.sh", id, lectureID, username, NULL);
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}

void instructorMenu(char username[500])
{

    int choice;
    int i = 0;
    while (true)
    {
        printf("\n========================================================");
        printf("\nWelcome to the Insturctor portal %s\n", username);
        printf("========================================================\n");
        printf("1.\tAdd Lectures\n");
        printf("2.\tAdd Students to a lecture\n");
        printf("3.\tShow Students\n");
        printf("4.\tAbsence\n");
        printf("5.\tExit\n");
        printf("6.\tLogout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("========================================================\n");

        if (choice == 1)
        {
            addLectures(username);
        }
        else if (choice == 2)
        {
            addStudents(username);
        }
        else if (choice == 3)
        {
            showStudents(username);
        }
        else if (choice == 4)
        {
            absence(username);
        }
        else if (choice == 5)
        {
            pthread_exit(0);
            exit(1);
        }
        else if (choice == 6)
        {
            logout();
            pthread_exit(0);
            exit(1);
        }
        else
        {
            printf("========================================================\n");
            printf("Invalid Input");
        }
    }
}

void adminMenu(char username[500])
{
    system("chmod 700 searchBackupFiles.sh searchLecture.sh backup.sh searchStudent.sh absenceManager.sh");
    int choice;
    int i = 0;
    char command[100];
    char command2[100];
    char *usernameInst = (char *)malloc(500);
    char *password = (char *)malloc(500);
    char input1[500];
    while (true)
    {
        printf("\n========================================================");
        printf("\nWelcome to the Admin portal %s\n", username);
        printf("========================================================\n");
        printf("1.\tAdd Users\n");
        printf("2.\tShow Users details\n");
        printf("3.\tInitiate Backup request from server\n");
        printf("4.\tShow files and directories\n");
        printf("5.\tExit\n");
        printf("6.\tLogout\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("========================================================\n");

        if (choice == 1)
        {
            printf("\n========================================================");
            printf("\nAdd Instructors to the System\n");
            printf("========================================================\n");

            printf("Instructor Username: ");
            scanf("%s", usernameInst);
            printf("Enter Password: ");
            scanf("%s", password);
            printf("========================================================\n");

            strcpy(command, "echo '");
            strcat(command, usernameInst);
            strcat(command, ",");
            strcat(command, password);
            strcat(command, "'");
            strcat(command, " ");
            strcat(command, ">>");
            strcat(command, "LoginData.txt");

            addUsers(command);
            free(usernameInst);
            free(password);
        }
        else if (choice == 2)
        {
            printf("\nSelect Search Type\n");
            printf("========================================================\n");
            printf("1.\tAll Users\n");
            printf("2.\tSpecific User\n");
            printf("3.\tExit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            printf("\n========================================================\n");

            if (choice == 1)
            {
                showUsers(username);
            }
            else if (choice == 2)
            {
                printf("The data will appear in this format (ID,Password)\n");
                printf("Enter Username: ");
                scanf("%s", input1);
                printf("\n========================================================\n");
                strcpy(command, "grep -E '");
                strcat(command, input1);
                strcat(command, ",");
                strcat(command, "' LoginData.txt");
                showSpecificUser(command);
            }
            else
            {
                continue;
            }
        }
        else if (choice == 3)
        {
            clientSocket(username);
        }
        else if (choice == 4)
        {
            printf("\n========================================================");
            printf("\n\tSelect the type of files you want to search:\n");
            printf("========================================================\n");
            printf("1.\tText files (txt)\n");
            printf("2.\tShell files (sh)\n");
            printf("3.\tExecuatable Files\n");
            printf("4.\tAll files\n");
            printf("5.\tAll directories\n");
            printf("6.\tExit\n");

            printf("Enter your choice: ");
            scanf("%d", &choice);
            printf("========================================================\n");
            showFiles(username, choice);
        }
        else if (choice == 5)
        {
            pthread_exit(0);
            exit(1);
        }
        else if (choice == 6)
        {
            logout();
            pthread_exit(0);
            exit(1);
        }
        else
        {
            printf("========================================================\n");
            printf("Invalid Input");
        }
    }
}

void showSpecificUser(char command[500])
{
    int pid = fork();

    int retVal;

    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        retVal = system(command);
        exit(0);
    }
    else
    {
        wait(NULL);
        if (retVal <= -1)
        {
            printf("\n========================================================");
            printf("\n\tError in adding users\n");
            printf("========================================================\n");
            return;
        }
        return;
    }
}

void addUsers(char command[500])
{
    int pid = fork();

    int retVal;

    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        retVal = system(command);
        exit(0);
    }
    else
    {
        wait(NULL);
        if (retVal <= -1)
        {
            printf("\n========================================================");
            printf("\n\tError in adding users\n");
            printf("========================================================\n");
            return;
        }
        else
        {
            printf("\n========================================================");
            printf("\n\tUser Added Successfully\n");
            printf("========================================================\n");
            return;
        }
    }
}

void showUsers(char username[500])
{
    int pid = fork();
    int retVal;

    if (pid < 0)
    {
        //This is an error indication
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    {
        //Make the child process
        retVal = system("cat LoginData.txt");
        exit(0);
    }
    else
    {
        wait(NULL);
        if (retVal <= -1)
        {
            printf("\n========================================================");
            printf("\n\tError in viewing users\n");
            printf("========================================================\n");
            return;
        }

        return;
    }
}

void showFiles(char username[500], int choice)
{
    int pid = fork();
    int retVal;

    if (choice == 1)
    {
        if (pid < 0)
        {
            //This is an error indication
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            //Make the child process
            retVal = system("ls -l | grep .txt");
            exit(0);
        }
        else
        {
            wait(NULL);
            if (retVal <= -1)
            {
                printf("\n========================================================");
                printf("\n\tError in viewing users\n");
                printf("========================================================\n");
                return;
            }

            return;
        }
    }
    else if (choice == 2)
    {
        if (pid < 0)
        {
            //This is an error indication
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            //Make the child process
            retVal = system("ls -l | grep .sh");
            exit(0);
        }
        else
        {
            wait(NULL);
            if (retVal <= -1)
            {
                printf("\n========================================================");
                printf("\n\tError in viewing users\n");
                printf("========================================================\n");
                return;
            }

            return;
        }
    }
    else if (choice == 3)
    {
        if (pid < 0)
        {
            //This is an error indication
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            //Make the child process
            retVal = system("find . -type f -executable -print");
            exit(0);
        }
        else
        {
            wait(NULL);
            if (retVal <= -1)
            {
                printf("\n========================================================");
                printf("\n\tError in viewing users\n");
                printf("========================================================\n");
                return;
            }

            return;
        }
    }
    else if (choice == 4)
    {
        if (pid < 0)
        {
            //This is an error indication
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            //Make the child process
            retVal = system("ls -p | grep -v /");
            exit(0);
        }
        else
        {
            wait(NULL);
            if (retVal <= -1)
            {
                printf("\n========================================================");
                printf("\n\tError in viewing users\n");
                printf("========================================================\n");
                return;
            }

            return;
        }
    }
    else if (choice == 5)
    {
        if (pid < 0)
        {
            //This is an error indication
            fprintf(stderr, "Fork Failed");
            exit(-1);
        }
        else if (pid == 0)
        {
            //Make the child process
            retVal = system("ls -d */");
            exit(0);
        }
        else
        {
            wait(NULL);
            if (retVal <= -1)
            {
                printf("\n========================================================");
                printf("\n\tError in viewing users\n");
                printf("========================================================\n");
                return;
            }

            return;
        }
    }
    else if (choice == 6)
    {
        return;
    }
}

void clientSocket(char username[500])
{
    int port = 5678;
    int sock = -1, valRead, choice;
    struct sockaddr_in address;
    struct hostent *host;
    int len, lenRead;
    char *command = "Initate Backup";
    char buffer[1024] = {0};

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock <= 0)
    {
        fprintf(stderr, "error: cannot create socket\n");
        return;
    }

    /* connect to server */
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    host = gethostbyname("localhost");
    if (!host)
    {
        fprintf(stderr, " error: unknown host\n");
        return;
    }
    memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
    {
        fprintf(stderr, " error: cannot connect to host\n");
        return;
    }

    printf("\n========================================================\n");
    printf("Here are the files that will be backedup in the system\nthe backup file name is [BACKUP NUMBER] backup.zip note that you create a maximum of 10 backup files\n");
    printf("------------------------\n");
    printf("\tText Files\n");
    printf("------------------------\n");
    printf("1.\tlectures.txt\n");
    printf("2.\tLoginData.txt\n");
    printf("3.\tAddedStudents.txt\n");
    printf("------------------------\n");
    printf("\tC Files\n");
    printf("------------------------\n");
    printf("1.\tinsturctorPortal.c\n");
    printf("2.\tserverC.c\n");
    printf("------------------------\n");
    printf("\tShell Files\n");
    printf("------------------------\n");
    printf("1.\tlogin.sh\n");
    printf("2.\tabsenceManager.sh\n");
    printf("3.\tsearchLecture.sh\n");
    printf("4.\tsearchStudent.sh\n");
    printf("5.\tbackup.sh\n");
    printf("6.\tsearchBackupFiles.sh\n");
    printf("7.\tnumberOfLogins.sh\n");
    printf("========================================================\n");
    backupView();
    printf("\n========================================================\n");
    printf("Are you sure you want to create a new backup file\n");
    printf("1.\tBackup\n");
    printf("2.\tExit\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\n========================================================\n");
        len = strlen(command);
        write(sock, &len, sizeof(int));
        write(sock, command, len);
        lenRead = read(sock, &lenRead, sizeof(int));
        valRead = read(sock, buffer, lenRead + 1);
        if (strcmp("Found", buffer) == 0)
        {
            printf("Initiating Backup...\n");
            backup();
            printf("\nBackup Finished\n");
            len = 0;
            write(sock, &len, sizeof(int));
            close(sock);
        }
        else
        {
            printf("Failed to Backup.\nPlease check the files listed are within the systems directory or if they even exist\n");
            len = 0;
            write(sock, &len, sizeof(int));
            close(sock);
        }
    }
    else if (choice == 2)
    {
        printf("\n========================================================\n");
        len = 0;
        write(sock, &len, sizeof(int));
        close(sock);
    }
    else
    {
        printf("\n========================================================\n");
        printf("Invalid Input\n");
    }

    return;
}

void addStudents(char username[500])
{
    char lectureID[500];
    int retVal, choice;
    char IDRead[500];
    char timeRead[500];
    char nameRead[500];
    char buffer[500];
    char usernameRead[500];
    char command[500];

    fp = fopen("lectures.txt", "r");

    if (fp == NULL)
    {
        printf("\n========================================================");
        printf("\nError in reading lectures\n");
        printf("========================================================\n");
    }

    fgets(buffer, 500, fp);

    while (feof(fp) == 0)
    {
        sscanf(buffer, "%s %s %s %s", IDRead, timeRead, nameRead, usernameRead);
        if (strcmp(username, usernameRead) == 0)
        {
            printf("\n========================================================\n");
            printf("Here are the data for lecture: %s\n", nameRead);
            printf("========================================================\n");
            printf("1.\tLecture ID: %s\n", IDRead);
            printf("2.\tLecture Name: %s\n", nameRead);
            printf("3.\tLecture Instructor: %s\n", username);
            printf("4.\tLecture Time: %s\n", timeRead);
            printf("========================================================\n");
        }
        fgets(buffer, 500, fp);
    }
    fclose(fp);

    printf("Select Lecture ID: ");
    scanf("%s", lectureID);
    printf("========================================================\n");
    retVal = searchLecture(lectureID, username);
    if (retVal == 1)
    {

        int lectures;
        printf("\n========================================================");
        printf("\nHow many Students do you want to the lecture\n");
        printf("========================================================\n");
        printf("Number of Students: ");
        scanf("%d", &lectures);
        printf("\n========================================================\n");
        struct studentsData *link = (struct studentsData *)malloc(lectures * sizeof(struct studentsData));

        for (int i = 0; i < lectures; ++i)
        {
            printf("\nEnter Student ID: ");
            scanf("%s", (link + i)->studentID);
            printf("Enter Student Name: ");
            scanf("%s", (link + i)->studentName);
            printf("\n========================================================\n");
            (link + i)->lectureID = lectureID;
            (link + i)->username = username;
        }

        printf("Here are the data for students you want to add\n");
        printf("========================================================\n");

        for (int i = 0; i < lectures; ++i)
        {
            printf("1.\tStudent ID: %s\n", (link + i)->studentID);
            printf("2.\tStudent Name: %s\n", (link + i)->studentName);
            printf("3.\tLecture ID: %s\n", (link + i)->lectureID);
            printf("4.\tLecture Instructor: %s\n", (link + i)->username);
            printf("========================================================\n");
        }
        printf("Are you sure you want to add these students to the system\n");
        printf("1.\tAdd to the system\n");
        printf("2.\tExit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            studentPtr = fopen("AddedStudents.txt", "a");

            if (studentPtr == NULL)
            {
                printf("\n========================================================");
                printf("\nError in adding students\n");
                printf("========================================================\n");
                free(link);
                fclose(studentPtr);
            }

            for (int i = 0; i < lectures; ++i)
            {
                fprintf(studentPtr, "%s %s %s %s Absence%s%s%s%s.0\n", (link + i)->studentID, (link + i)->studentName, (link + i)->lectureID, (link + i)->username, (link + i)->studentID, (link + i)->studentName, (link + i)->lectureID, (link + i)->username);
                printf("%s Information Added:\n", (link + i)->studentName);
            }
            printf("\n========================================================");
            printf("\nStudents Added Successfuly\n");
            printf("========================================================\n");
            free(link);
            fclose(studentPtr);
            strcpy(command, "chmod 700 ");
            strcat(command, "AddedStudents.txt");
            addingPermission(command);
        }
        else
        {
            free(link);
        }
    }
    else
    {
        printf("\n========================================================");
        printf("\nInvalid ID\n");
        printf("========================================================\n");
    }
    return;
}

void addLectures(char username[500])
{
    int lectures, choice;
    char command[500];

    fp = fopen("lectures.txt", "a");
    if (fp == NULL)
    {
        printf("\n========================================================");
        printf("\nError in adding lectures\n");
        printf("========================================================\n");
    }

    printf("\n========================================================");
    printf("\nHow many lectures do you want to add\n");
    printf("========================================================\n");
    printf("Number of Lectures: ");
    scanf("%d", &lectures);
    printf("\n========================================================\n");
    struct lecturesData *link = (struct lecturesData *)malloc(lectures * sizeof(struct lecturesData));

    for (int i = 0; i < lectures; i++)
    {
        printf("Enter Lecture ID %d: ", i + 1);
        scanf("%s", (link + i)->ID);
        printf("Enter Lecture Time %d: ", i + 1);
        scanf("%s", (link + i)->time);
        printf("Enter Lecture Name %d: ", i + 1);
        scanf("%s", (link + i)->name);
        (link + i)->username = username;
        printf("========================================================\n");
    }

    printf("Here are the data for the lectures you want to add\n");
    printf("========================================================\n");

    for (int i = 0; i < lectures; i++)
    {
        printf("1.\tLecture ID: %s\n", (link + i)->ID);
        printf("2.\tLecture Name: %s\n ", (link + i)->time);
        printf("3.\tLecture Time: %s\n", (link + i)->name);
        printf("4.\tLecture Instructor: %s\n", (link + i)->username);
        printf("========================================================\n");
    }

    printf("Are you sure you want to add these lectures to the system\n");
    printf("1.\tAdd to the system\n");
    printf("2.\tExit\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        if (fp == NULL)
        {
            printf("\n========================================================");
            printf("\nError in adding lectures\n");
            printf("========================================================\n");
        }

        for (int i = 0; i < lectures; i++)
        {
            fprintf(fp, "%s %s %s %s\n", (link + i)->ID, (link + i)->name, (link + i)->time, (link + i)->username);
            printf("========================================================\n");
        }

        printf("\n========================================================");
        printf("\nLectures Added Successfuly\n");
        printf("========================================================\n");
        fclose(fp);
        strcpy(command, "chmod 700 ");
        strcat(command, "lectures.txt");
        addingPermission(command);
    }

    free(link);

    return;
}

void absence(char username[500])
{

    char choice1[500];
    char choice2[500];

    int retVal;
    char lectureID[500];
    char studentID[500];
    char studentName[500];
    char absence[500];
    char buffer[500];
    char usernameRead[500];

    fp = fopen("AddedStudents.txt", "r");

    if (fp == NULL)
    {
        printf("\n========================================================");
        printf("\nError in reading students\n");
        printf("========================================================\n");
    }

    fgets(buffer, 500, fp);

    while (feof(fp) == 0)
    {
        sscanf(buffer, "%s %s %s %s %s", studentID, studentName, lectureID, usernameRead, absence);
        if (strcmp(username, usernameRead) == 0)
        {
            char *token = strtok(absence, ".");
            token = strtok(NULL, ".");
            printf("\n========================================================\n");
            printf("Here are the data for: %s\n", studentName);
            printf("========================================================\n");
            printf("1.\tStudent ID: %s\n", studentID);
            printf("2.\tStudent Name: %s\n", studentName);
            printf("3.\tLecture ID: %s\n", lectureID);
            printf("4.\tLecture Instructor: %s\n", usernameRead);
            printf("5.\tAbsence: %s\n", token);

            printf("========================================================\n");
        }
        fgets(buffer, 500, fp);
    }
    fclose(fp);

    printf("Enter the required data to add the absence\n");
    printf("Enter Student ID: ");
    scanf("%s", choice1);
    printf("Enter Lecture ID: ");
    scanf("%s", choice2);
    retVal = searchStudent(choice1, choice2, username);
    if (retVal == 1)
    {
        addAbsence(choice1, choice2, username);
        printf("\n========================================================");
        printf("\nAbsence Added Successfully\n");
        printf("========================================================\n");
    }
    else
    {
        printf("\n========================================================");
        printf("\nInvalid Information Entered\n");
        printf("========================================================\n");
    }
}
void showStudents(char username[500])
{
    char choice[500];
    char lectureID[500];
    char studentID[500];
    char studentName[500];
    char absence[500];
    char buffer[500];
    char usernameRead[500];

    char usernameFromLecture[500];
    char lectureIDFromLecture[500];
    char lectureName[500];
    char lectureTime[500];

    fp = fopen("lectures.txt", "r");
    studentPtr = fopen("AddedStudents.txt", "r");

    if (studentPtr == NULL)
    {
        printf("\n========================================================");
        printf("\nError in reading students\n");
        printf("========================================================\n");
    }

    if (fp == NULL)
    {
        printf("\n========================================================");
        printf("\nError in reading lectures\n");
        printf("========================================================\n");
    }
    printf("Here are all the lectures available in the system\n");
    printf("========================================================\n");

    fgets(buffer, 500, fp);

    while (feof(fp) == 0)
    {
        sscanf(buffer, "%s %s %s %s", lectureIDFromLecture, lectureName, lectureTime, usernameFromLecture);
        if (strcmp(username, usernameFromLecture) == 0)
        {
            printf("1.\tLecture ID: %s\n", lectureIDFromLecture);
            printf("2.\tLecture Name: %s\n", lectureName);
            printf("3.\tLecture Instructor: %s\n", usernameFromLecture);
            printf("4.\tLecture Time: %s\n", lectureTime);
            printf("========================================================\n");
        }
        fgets(buffer, 500, fp);
    }
    fclose(fp);

    printf("Enter Lecture ID: ");
    scanf("%s", choice);

    fgets(buffer, 500, studentPtr);

    printf("\n========================================================\n");
    printf("Here are the students for this lecture\n");
    printf("========================================================\n");

    while (feof(studentPtr) == 0)
    {
        sscanf(buffer, "%s %s %s %s %s", studentID, studentName, lectureID, usernameRead, absence);
        if (strcmp(username, usernameRead) == 0 && strcmp(lectureID, choice) == 0)
        {
            char *token = strtok(absence, ".");
            token = strtok(NULL, ".");

            printf("1.\tStudent ID: %s\n", studentID);
            printf("2.\tStudent Name: %s\n", studentName);
            printf("3.\tLecture ID: %s\n", lectureID);
            printf("4.\tLecture Instructor: %s\n", usernameRead);
            printf("5.\tAbsence: %s\n", token);

            printf("========================================================\n");
        }
        fgets(buffer, 500, studentPtr);
    }
    fclose(studentPtr);
}
