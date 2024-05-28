#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define RESET "\x1b[0m" // ANSI escape codes for formatting
#define GREEN "\x1b[32m"
#define CYAN "\x1b[36m"
#define YELLOW "\x1b[33m"
#ifdef _WIN32
#define clrscr system("cls");
#else
#define clrscr system("clear");
#endif
void clrbfr();
int ticket_count();
void FREE();
void update_file();
void file_to_menu();
void initial_menu();
void display_menu();
void add_ticket();
void remove_ticket();
void price_update();
void take_order();
void view_order();
void total_bill();
void intro();
void admin_panel();
void change_pass();
void create_menu();
void load_admin_pass();
void admin_login();
void change_pass();
void general();
int main();

FILE *file;
FILE *pass;
char bfrpass[100];

struct ticket
{
    char destination[50];
    float price;
    struct ticket *next;
} *head = NULL, *tail = NULL;
typedef struct ticket ticket;

struct booking
{
    char name[50];
    int quantity;
    float price;
    struct booking *next;
} *bookhd = NULL, *ordrtl = NULL;
typedef struct booking booking;

void clrbfr() // clears the buffers
{
    while (getchar() != '\n')
        ;
}

int ticket_count() // needed by take_order & update_file
{
    int cnt = 0;
    ticket *temp = head;
    while (temp != NULL)
    {
        cnt++;
        temp = temp->next;
    }
    return cnt;
}

void FREE()
{
    ticket *tempit = head;
    while (tempit != NULL)
    {
        head = head->next;
        free(tempit);
        tempit = head;
    }
    booking *tempor = bookhd;
    while (tempor != NULL)
    {
        bookhd = bookhd->next;
        free(tempor);
        tempor = bookhd;
    }
}

void update_file()
{

    int loop = ticket_count();
    ticket *temp = head;
    while (loop--)
    {
        fputs(temp->destination, file);
        fputc(',', file);
        fprintf(file, "%.2f", temp->price);
        fputc('\n', file);
        temp = temp->next;
    }
    fclose(file);
}

void file_to_menu() // get called from second compilation
{
    file = fopen("try_hard.txt", "r");
    char buffer[100];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
        count++;
    }
    rewind(file);
    while (count--)
    {
        ticket *temp = malloc(sizeof(ticket));

        fscanf(file, " %[^,]", temp->destination);
        fseek(file, 1, SEEK_CUR);
        fscanf(file, " %f", &temp->price);
        fseek(file, 1, SEEK_CUR);
        temp->next = NULL;
        if (head == NULL)
        {

            head = temp;
            tail = head;
        }
        else
        {
            tail->next = temp;
            tail = temp;
        }
    }
    fclose(file);
}

void initial_menu() // prepopulating the inventory
{
    ticket *temp = NULL;
    for (int i = 0; i < 10; i++)
    {
        if (head == NULL)
        {
            head = malloc(sizeof(ticket));
            head->next = NULL;
            tail = head;
        }
        else
        {
            temp = malloc(sizeof(ticket));
            temp->next = NULL;
            tail->next = temp;
            tail = temp;
        }
    }
    temp = head;
    strcpy(temp->destination, "Dhaka to Chittagong");
    temp->price =  	656.0;
    temp = temp->next;

    strcpy(temp->destination, "Dhaka to Sylhet");
    temp->price = 425.0;
    temp = temp->next;

    strcpy(temp->destination, "Chittgong to Sylhet");
    temp->price = 315.25;
    temp = temp->next;

    strcpy(temp->destination, "Dhaka to Rajshahi");
    temp->price =  340;
    temp = temp->next;

    strcpy(temp->destination, "Dhaka to Khulna");
    temp->price = 966.0;
    temp = temp->next;

    strcpy(temp->destination, "Rajshahi to Khulna");
    temp->price =  	310.99;
    temp = temp->next;

    strcpy(temp->destination, "Dhaka to Dinajpur");
    temp->price =  	620.99;
    temp = temp->next;

    strcpy(temp->destination, "Dhaka to Sirajgonj ");
    temp->price =  	240.99;
    temp = temp->next;

    strcpy(temp->destination, "Sirajgonj to Khulna");
    temp->price = 399.99;
    temp->next = NULL;
    tail = temp;
}

void display_menu()
{
    clrscr
    intro();
    printf(CYAN "---Available Destinations---\n");
    ticket *temp = head;
    int i = 0;
    while (temp != NULL)
    {
        i++;
        printf("%d. %s - BDT %.2f\n", i, temp->destination, temp->price);
        temp = temp->next;
    }
    printf(CYAN"\nPress enter to  continue - ");
    clrbfr();
    printf("\n" RESET);
}

void add_ticket()
{
    clrscr
    intro();
    ticket *temp = malloc(sizeof(ticket));
    tail->next = temp;
    tail = temp;
    tail->next = NULL;
    printf(CYAN "Name of Destinations - ");
    scanf(" %[^\n]", tail->destination);
    printf("Price of tickets of the Destinations - ");
    while (1)
    {
        if (scanf(" %f", &temp->price) != 1)
        {
            printf("Invalid inputs. Enter with digits\n");
            clrbfr();
            continue;
        }
        else
        {
            if (temp->price <= 0)
                printf("Enter a proper value\n");
            else
                break;
        }
    }
    clrbfr();
    update_file();
    printf("---Ticket added---\n");
    printf(CYAN "\nPress enter to continue - ");
    clrbfr();
    printf("\n" RESET);
}

void remove_ticket()
{
    clrscr
    intro();
    int flag = 0;
    printf(CYAN "Removed Destination name (Mind the Caps) - ");
    char name[50];
    scanf(" %[^\n]", name);
    clrbfr();
    ticket *temp = head, *prev = NULL;
    while (temp != NULL)
    {
        int check;
        check = strcmp(name, temp->destination); // returns 0 if equal
        if (check == 0)
        {
            if (temp == head)
            {
                head = head->next;
                free(temp);
                flag = 1;
                break;
            }
            else if (temp->next == NULL)
            {
                prev->next = NULL;
                tail = prev;
                free(temp);
                flag = 1;
                break;
            }
            else
            {
                prev->next = temp->next;
                free(temp);
                flag = 1;
                break;
            }
        }
        prev = temp;
        temp = temp->next;
    }
    if (flag)
        printf("---Destination removed---\n");
    else
        printf(CYAN "Destination not found!!!Operation failed!\n");

    update_file();
    printf(CYAN "\nPress enter to continue - ");
    clrbfr();
    printf("\n" RESET);
}

void price_update()
{
    clrscr
    intro();
    int flag = 0;
    printf(CYAN "Updated Destination name(Mind the Caps) - ");
    char name[50];
    scanf(" %[^\n]", name);
    clrbfr();
    ticket *temp = head;
    while (temp != NULL)
    {
        int check;
        check = strcmp(name, temp->destination); // returns 0 if equal
        if (check == 0)
        {
            printf("Updated price - ");
            while (1)
            {
                if (scanf("%f", &temp->price) != 1)
                {
                    printf("Invalid inputs. Enter valid digits.\n");
                    clrbfr();
                }
                else if (temp->price <= 0)
                {
                    printf("Enter a proper value\n");
                    clrbfr();
                }
                else
                {
                    clrbfr();
                    break;
                }
            }
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if (flag)
        printf("---Price updated---\n");
    else
        printf(CYAN "\nDestination not found!!!Operation Failed!\n");

    update_file();
    printf(CYAN "\nPress enter to continue - ");
    clrbfr();
    printf("\n" RESET);
}

void take_order()
{
    clrscr
    intro();
    booking *tempor = NULL;
    ticket *tempit = head;

    // clearing previous order
    if (bookhd != NULL)
    {
        tempor = bookhd;
        while (bookhd != NULL)
        {
            bookhd = bookhd->next;
            free(tempor);
            tempor = bookhd;
        }
    }

    int i, j = ticket_count();
    while (1)
    {
        printf(CYAN"Enter menu item ID (0 to exit/ 111 for menu): ");
        scanf(" %d", &i);
        clrbfr();
        if (i >= 0 && i <= j || i == 111)
        {
            if (i == 0)
                break;
            else if (i == 111)
                display_menu();
            else
            {
                if (bookhd == NULL)
                {
                    bookhd = malloc(sizeof(booking));
                    ordrtl = bookhd;
                    tempor = bookhd;
                    i--;
                    while (i--)
                    {
                        tempit = tempit->next;
                    }
                    strcpy(tempor->name, tempit->destination);
                    tempor->price = tempit->price;
                    tempor->next = NULL;
                    printf("Enter Number of tickets - ");
                    while (1)
                    {
                        scanf(" %d", &tempor->quantity);
                        clrbfr();
                        if (tempor->quantity > 0 && tempor->quantity <= 10)
                        {
                            break;
                        }
                        else
                        {
                            printf("Max 10 order at a time. Try again - ");
                        }
                    }
                    tempit = head;
                }
                else
                {
                    i--;
                    while (i--)
                    {
                        tempit = tempit->next;
                    }
                    tempor = malloc(sizeof(booking));
                    strcpy(tempor->name, tempit->destination);
                    tempor->price = tempit->price;
                    tempor->next = NULL;
                    ordrtl->next = tempor;
                    ordrtl = tempor;

                    printf("Enter Number of tickets - ");
                    while (1)
                    {
                        scanf(" %d", &tempor->quantity);
                        if (tempor->quantity > 0 && tempor->quantity <= 10)
                        {
                            clrbfr();
                            break;
                        }
                        else
                        {
                            clrbfr();
                            printf("Max 10 order at a time. Try again - ");
                        }
                    }
                    tempit = head;
                }
            }
        }
        else
        {
            int count = ticket_count();
            printf("Invalid input. Press (1 - %d)\n", count);
        }
    }
    printf("---Order Taken---\n");
    printf(CYAN "\nPress enter to continue - ");
    clrbfr();
    printf("\n" RESET);
}

void view_order()
{
    clrscr
    intro();
    printf(CYAN "---Current Order---\n");
    int i = 0;
    booking *temp = bookhd;
    while (temp != NULL)
    {
        i++;
        printf("%d. %s x%d\n", i, temp->name, temp->quantity);
        temp = temp->next;
    }
    printf("\n" RESET);
}

void total_bill()
{
    float bill = 0;
    booking *temp = bookhd;
    while (temp != NULL)
    {
        bill += temp->price * temp->quantity;
        temp = temp->next;
    }
    printf(CYAN "---Total Bill - %.2fBDT\n", bill);

    printf(CYAN "\nPress enter to continue - ");
    clrbfr();

    printf("\n");
}

void intro()
{

    printf("\n");
    printf(YELLOW"\n\t\t\t*****************Assalamu Alaikum***************");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\t**                                           **");
    printf("\n\t\t\t**      1.Sumaiya Islam Lamia(232-15-785     **");
    printf("\n\t\t\t**      2.Sobeha Prova(232-15-893)           **");
    printf("\n\t\t\t**      3.Sourav Hossain Turjo (232-15-144)  **");
    printf("\n\t\t\t**      4.JannatulFerdous Jannati(232-15-113)**");
    printf("\n\t\t\t**      5.Aliful Islam Alif  (232-15-183)    **");
    printf("\n\t\t\t**                                           **");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\t-----------------------------------------------");

    printf("\n");
    printf("\n");

    printf("\n\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t        = Prepared By Team HEXCLAN ");
    printf("\n\t\t        =Daffodil International University ");
    printf("\n\t\t        =Lab Project ");
    printf("\n\t\t        =Spring 2024----A ");
    printf("\n\t\t        =Topic :  Ticket Management System .");
    printf("\n\t\t        =*******************************=");
    printf("\n\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");
}

void create_menu()
{
    if (access("try_hard.txt ", F_OK)) // returns 0 if exists, -1 if dont
    {
        initial_menu(); // first compilation
    }
    else
    {
        file_to_menu();
    }
}

void load_admin_pass()
{
    if (access("admin_passwd.txt", F_OK) == 0)
        pass = fopen("admin_passwd.txt", "r");
    else
    {
        pass = fopen("admin_passwd.txt", "w+");
        fprintf(pass, "lamia");
        rewind(pass);
    }

    char c;
    int i = 0;
    while ((c = fgetc(pass)) != EOF)
    {
        bfrpass[i] = c;
        i++;
    }
    bfrpass[i] = '\0';
    fclose(pass);
}

void admin_login()
{
    clrscr
    intro();
    load_admin_pass(); // loaded in bfrpass
    char password[100];
    printf(CYAN "Enter password - ");
    int flag = 0;
    while (1)
    {
        scanf(" %[^\n]", password);
        getchar();
        if (flag)
        {
            int chek1 = 0, chek2 = 0;
            chek1 = strcmp(password, "x");
            chek2 = strcmp(password, "X");
            if (chek1 == 0 || chek2 == 0)
            {
                printf("Cancelled!\n" RESET);
                printf(CYAN "\nPress enter to continue - " RESET);
                clrbfr();
                break;
            }
        }
        flag++;
        int comp;
        comp = strcmp(password, bfrpass);
        if (comp == 0)
        {
            printf("---Login Successfull!---\n" RESET);
            printf(CYAN "\nPress enter to continue - " RESET);
            clrbfr();
            admin_panel();
            break;
        }
        else
        {
            printf("Incorrect Password!\n");
            printf("Try again(or enter 'x' to cancel) - ");
        }
    }

    printf(RESET "\n");
}

void admin_panel()
{
    while (1)
    {
        clrscr
        intro();
        printf(CYAN "-------ADMIN-------\n");
        printf(CYAN "1. Update menu\n");
        printf("2. Update password\n");
        printf("3. sign out\n");
        printf("4. Exit the program\n");
        printf("Pick your choice - ");
        int ch;
        while (1)
        {
            if (scanf(" %d", &ch) != 1)
            {
                printf("Invalid choice. Try again - ");
                clrbfr();
                continue;
            }
            else
            {
                if (ch < 0 || ch > 4)
                {
                    printf("Invalid choice. Try again - ");
                    continue;
                }
                else
                    break;
            }
        }
        switch (ch)
        {
        case 1:
            clrscr
            intro();
            printf(CYAN "1. Update Price chart\n");
            printf("2. Add an Destination\n");
            printf("3. Remove an Destination\n");
            printf("4. View menu\n");
            printf("5. Cancel\n");
            printf("Enter your choice - ");
            int choice;
            while (1)
            {
                if (scanf(" %d", &choice) != 1)
                {
                    printf("Please Enter a Valid Digit ");
                    clrbfr();
                    continue;
                }
                else
                {
                    if (choice < 0 || choice > 5)
                    {
                        printf("Invalid Choice. Try again - ");
                        clrbfr();
                        continue;
                    }
                    else
                    {
                        clrbfr();
                        if (choice == 5)
                        {
                            printf("---Cancelled---\n");
                            printf(CYAN "\nPress enter to continue - ");
                            clrbfr();
                            printf("\n" RESET);
                            break;
                        }
                        switch (choice)
                        {
                        case 1:
                            price_update();
                            break;
                        case 2:
                            add_ticket();
                            break;
                        case 3:
                            remove_ticket();
                            break;
                        case 4:
                            display_menu();
                            break;
                        }
                        break;
                    }
                }
            }
            break;
        case 2:
            change_pass();
            break;
        case 3:
            clrscr
            intro();
            printf(CYAN "---You have signed out successfully!---\n");
            printf(CYAN "\nPress Enter to continue - " RESET);
            clrbfr();
            clrbfr();
            return; // return to main menu
        case 4:
            update_file();
            FREE();
            clrscr
            intro();
            printf(CYAN "---Exiting the Program---\n\n");
            exit(0);
        }
    }
}

void change_pass()
{
    clrscr
    intro();
    FILE *ptr;
    ptr = fopen("admin_passwd.txt", "w");
    printf(CYAN "Enter new password - ");
    char buffer[100];
    scanf(" %[^\n]", buffer);
    getchar();
    fputs(buffer, ptr);
    fclose(ptr);
    printf("---Your Password changed successfully!---\n" RESET);
    printf(CYAN "\nPress enter to continue - " RESET);
    clrbfr();
}

void general()
{
    while (1)
    {
        clrscr
        intro();
        printf(CYAN "----Customer Panel----\n");
        printf("1. Display Destinations\n");
        printf("2. New Order\n");
        printf("3. Display Order\n");
        printf("4. sign out\n");
        printf("5. Exit\n");
        printf("Pick your choice : " RESET);

        int i;
        while (1)
        {
            if (scanf(" %d", &i) != 1)
            {
                printf(CYAN "Invalid choice. Please Try again.\n" RESET);
                clrbfr();
                continue;
            }
            else
            {
                if (i < 1 || i > 5)
                {
                    printf(CYAN "Invalid choice. Please Try again.\n" RESET);
                    clrbfr();
                    continue;
                }
                else
                {
                    clrbfr();
                    break;
                }
            }
        }
        printf("\n");
        if (i == 5)
        {
            update_file();
            FREE();
            clrscr
            intro();
            printf(CYAN "---Exiting the Program---\n\n");
            exit(0);
        }
        switch (i)
        {
        case 1:
            display_menu();
            break;
        case 2:
            take_order();
            break;
        case 3:
            view_order();
            total_bill();
            break;
        case 4:
            clrscr
            intro();
            printf(CYAN "Enter admin password - ");
            char password[100];
            scanf(" %[^\n]", password);
            clrbfr();
            load_admin_pass(); // loaded in bfrpass
            int check = 0;
            check = strcmp(password, bfrpass);
            if (check == 0)
            {
                printf("---Logged out successfully!---\n");
                printf("\nPress enter to continue - ");
                clrbfr();
                return;
            }
            else
            {
                printf("---Authorization failed!---\n");
                printf("\nPress enter to continue - ");
                clrbfr();
            }
            break;
        }
    }
}

int main()
{
    create_menu();
    while (1)
    {
        clrscr
        intro();
        printf(CYAN "---Ticket Management system---\n");
        printf("1. Admin Panel Access\n");
        printf("2. Customer Panel\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        char ch;
        while (scanf(" %c", &ch) == 1)
        {
            clrbfr();
            if (ch == '1' || ch == '2' || ch == '3')
                break;
            else
                printf(CYAN"Invalid choice. Please try again - ");
        }
        printf("\n");
        switch (ch)
        {
        case '1':
            admin_login();
            break;
        case '2':
            clrscr
            intro();
            printf(CYAN "---General Access Granted---\n");
            printf("Press enter to continue - " RESET);
            clrbfr();
            general();
            break;
        default:
            clrscr
            FREE();
            intro();
            printf(CYAN "---Exiting the Program---\n\n");
            return 0;
        }
    }
}
