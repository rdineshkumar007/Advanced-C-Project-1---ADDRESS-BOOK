/*
Name  : DIneshkumar R
Date  : 13/08/2025
Title : Project 1 - Address Book
*/

#include <stdio.h>
#include "contact.h"
#include "ctype.h"
#include <string.h>
#include <stdlib.h>

/* Function to trim whitespace from both ends of a string */
static char* strtrim(char* str)
{
    char *end; // hello end pointer
    // Trim leading space
    while(isspace((unsigned char)*str))
        str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end))
        end--;

    end[1] = '\0';
    return str;
}

/* Initialize the address book with zero contacts */
void init_intitalization(struct Address_book *addressbook)
{
    addressbook->contact_count = 0;
}

/* Load contacts from a CSV file into the address book */
int load_contacts(struct Address_book *addressbook)
{
    FILE *file = fopen("addressbook.csv", "r");
    if (file == NULL)
    {
        init_intitalization(addressbook);
        return 0;
    }

    char line[256];
    int contact_index = -1;
    int loaded_count = 0;

    // Skip the header lines (first 5 lines)
    for (int i = 0; i < 5; i++)
    {
        if (fgets(line, sizeof(line), file) == NULL)
            break;
    }

    // Read each contact line
    while (fgets(line, sizeof(line), file))
    {
        // Check if line starts with contact marker
        if (strstr(line, "|  ") == line)
        {
            contact_index++;
            if (contact_index >= MAX_CONTACTS)
                break;

            // Parse contact details
            char name[MAX_LENGTH], phone[15], email[MAX_LENGTH];
            if (sscanf(line, "|  %*d. | %49[^|]| %14[^|]| %49[^|]|", name, phone, email) == 3)
            {
                // Store trimmed values
                strcpy(addressbook->contact_details[contact_index].name, strtrim(name));
                strcpy(addressbook->contact_details[contact_index].phone_number, strtrim(phone));
                strcpy(addressbook->contact_details[contact_index].mail_id, strtrim(email));
                loaded_count++;
            }
        }
    }

    addressbook->contact_count = loaded_count;
    fclose(file);

    printf("\n-----------------------------------------------------------------|");
    printf("\nLoaded %d contacts from addressbook.csv", loaded_count);
    printf("\n-----------------------------------------------------------------|\n");
    return loaded_count;
}

/* Save contacts from address book to a CSV file */
int save_contacts(struct Address_book *addressbook)
{
    FILE *file = fopen("addressbook.csv", "w");
    if (file == NULL)
    {
        printf("\nError! Could not create file to save contacts.\n");
        return 0;
    }

    // Write header information
    fprintf(file, "List of Contacts in the Address Book\n");
    fprintf(file, "\n-----------------------------------------------------------------------------------\n");
    fprintf(file, "|                                  ADDRESS BOOK                                   |\n");
    fprintf(file, "-----------------------------------------------------------------------------------\n");
    fprintf(file, "|%-5s|        %-18s|    %-12s|           %-20s|\n", "S.NO", "NAME", "PHONE NO", "MAIL ID");
    fprintf(file, "-----------------------------------------------------------------------------------\n");

    // Write each contact's details
    for (int i = 0; i < addressbook->contact_count; i++)
    {
        fprintf(file, "|  %d%-2s| %-25s|   %-13s|          %-21s|\n",
                i+1, ".",
                addressbook->contact_details[i].name,
                addressbook->contact_details[i].phone_number,
                addressbook->contact_details[i].mail_id);
        fprintf(file, "-----------------------------------------------------------------------------------\n");
    }

    // Write footer information
    fprintf(file, "-----------------------------------------------------------------------------------\n");
    fprintf(file, "|Total Contacts present in Address Book   : %-38d|\n", addressbook->contact_count);
    fprintf(file, "|Total Space remaining in the Address Book: %-38d|\n", MAX_CONTACTS - addressbook->contact_count);
    fprintf(file, "-----------------------------------------------------------------------------------\n");

    fclose(file);

    printf("\n-----------------------------------------------------------------|");
    printf("\nContacts saved successfully to addressbook.csv");
    printf("\n-----------------------------------------------------------------|\n");
    return 1;
}

/* Create new contacts in the address book */
int create_contact(struct Address_book *arr)
{
    int n;
    // Check if address book is full
    if(arr -> contact_count >= 100)
    {
        printf("\n-----------------------------------------------------------------|");
        printf("\nAddress Book is full.\nNo more space to Add Contacts.\nDelete contacts to add more Contacts.");
        printf("\n-----------------------------------------------------------------|\n");
        return 0;
    }
    s45:
    printf("\nEnter the no.of Contacts,\nto be added in the Address Book: ");
    if(scanf(" %d", &n) != 1)
    {
        printf("-----------------------------------------------------------------|\n");
        printf("Error!..\nNo.of Contacts should be a number.\n");
        printf("-----------------------------------------------------------------|\n");
        printf("Retry!..\nEnter the correct values.\n");
        printf("-----------------------------------------------------------------|\n");
        while(getchar() != '\n');
        goto s45;
    }
    while(getchar() != '\n');

    // Check if there's enough space for new contacts
    if((n + arr -> contact_count) > 100)
    {
        printf("-----------------------------------------------------------------|\n");
        printf("Invalid Entry!\nAddress Book doesn't have enough space to add these many contacts.\n");
        printf("-----------------------------------------------------------------|\n");
        printf("Total space remaining in the Address Book : %d\n", 100 - (arr -> contact_count));
        printf("-----------------------------------------------------------------|\n");
        printf("Retry!..\nEnter the correct values.\n");
        printf("-----------------------------------------------------------------|\n");
        goto s45;
    }
    int count = 0;
    // Add each new contact
    for(int i = arr -> contact_count; i < 100; i++)
    {
        if(count < n)
        {
            printf("\nContact %d\n", i+1);
            // Validate and add name
            if(!validate_name(arr, i, 0))
            {
                printf("\n--------------------------------|");
                printf("\nExiting Create Contact.....\n");
                printf("\n--------------------------------|\n");
                return 0;
            }

            // Validate and add phone number
            if(!validate_phone_number(arr, i, 0))
            {
                printf("\n--------------------------------|");
                printf("\nExiting Create Contact.....\n");
                printf("\n--------------------------------|\n");
                return 0;
            }

            // Validate and add email
            if(!validate_mail_id(arr, i, 0))
            {
                printf("\n--------------------------------|");
                printf("\nExiting Create Contact.....\n");
                printf("\n--------------------------------|\n");
                return 0;
            }
            arr -> contact_count++;
            count++;
        }
    }
    // Sort contacts after adding new ones
    sorting_names(arr);
    printf("\nContacts created successfully in the Address Book.\n");
    printf("\n-----------------------------------------------------------------|");
    printf("\nAddress Book : \nTotal Contacts present in Address Book   : %d\nTotal Space remaining in the Address Book: %d", arr -> contact_count, 100 - arr -> contact_count);
    printf("\n-----------------------------------------------------------------|\n");
    return 0;
}

/* List all contacts in the address book */
void list_contacts(struct Address_book *arr)
{
    // Check if address book is empty
    if(arr -> contact_count == 0)
    {
        printf("\n---------------------------------------------|");
        printf("\nNo Contacts present in the Address book.");
        printf("\n---------------------------------------------|\n");
        return;
    }
    // Print header
    printf("\nList of Contacts in the Address Book\n");
    printf("\n-----------------------------------------------------------------------------------\n");
    printf("|                                  ADDRESS BOOK                                   |");
    printf("\n-----------------------------------------------------------------------------------");
    printf("\n|%-5s|        %-18s|    %-12s|           %-20s|","S.NO", "NAME", "PHONE NO", "MAIL ID");
    printf("\n-----------------------------------------------------------------------------------\n");
    // Print each contact
    for(int j = 0; j<arr -> contact_count; j++)
    {
        printf("|  %d%-2s| %-25s|   %-13s|          %-21s|\n", j+1,".", arr -> contact_details[j].name, arr -> contact_details[j].phone_number, arr -> contact_details[j].mail_id);
        printf("-----------------------------------------------------------------------------------\n");
    }
    // Print footer
    printf("-----------------------------------------------------------------------------------\n");
    printf("|Total Contacts present in Address Book   : %d                                     |\n",arr -> contact_count);
    printf("|Total Space remaining in the Address Book: %d                                    |\n",100 - arr -> contact_count);
    printf("-----------------------------------------------------------------------------------\n");
}

/* Search for contacts in the address book */
int search_contacts(struct Address_book *arr, int e)
{
    int c;
    int arr1[100];
    int c1 =0;
    do
    {
        s48 :
        printf("\nSearch Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.Exit\nEnter the option : ");
        if(scanf(" %d", &c) != 1)
        {
            printf("-----------------------------------------------------------------|\n");
            printf("Error!..\nOption should be a number.\n");
            printf("-----------------------------------------------------------------|\n");
            printf("Retry!..\nEnter the correct option.\n");
            printf("-----------------------------------------------------------------|\n");
            while(getchar() != '\n');
            goto s48;
        }
        while(getchar() != '\n');
        switch(c)
        {
            case 1 : // Search by name
                int t = arr -> contact_count;
                int f = 0, s = 0, j, flg =0;
                if(validate_name(arr, t, 1))
                {
                    for(j =0; j < arr -> contact_count; j++)
                    {
                        if(strcasecmp(arr->contact_details[t].name, arr->contact_details[j].name) == 0)
                        {
                            if(flg == 0)
                            {
                                printf("\n-----------------------------------------------------------------------------------\n");
                                printf("|                                  ADDRESS BOOK                                   |");
                                printf("\n-----------------------------------------------------------------------------------");
                                printf("\n|%-5s|        %-18s|    %-12s|           %-20s|","S.NO", "NAME", "PHONE NO", "MAIL ID");
                                printf("\n-----------------------------------------------------------------------------------\n");
                                flg =1;
                            }
                            printf("|  %d%-2s| %-25s|   %-13s|          %-21s|\n", s+1,".", arr -> contact_details[j].name, arr -> contact_details[j].phone_number, arr -> contact_details[j].mail_id);
                            printf("-----------------------------------------------------------------------------------\n");
                            f = 1;
                            if(c1 < 100) // Prevent array overflow
                            {
                                arr1[c1] = j;
                                c1++;
                            }
                            s++; // Increment found count
                        }
                    }
                    if(f==0)
                    {
                        printf("\n--------------------------------------------------------|");
                        printf("\nContact not found.\nTry Entering another Name.");
                        printf("\n--------------------------------------------------------|\n");
                    }
                }
                if(s > 1 && (e == 1))
                {
                    printf("\n--------------------------------------------------------------|\n");
                    printf("\n%d Contacts Found in the same Name.\n", s);
                    int s1;
                    s2 :
                    printf("Which Contact do you like to display from the above Contacts?");
                    printf("\n--------------------------------------------------------------|\n");
                    for(int i =1; i<=s; i++)
                    {
                        printf("\n-----------------\n");
                        printf("|Contact No | %d |", i);

                    }
                    printf("\n-----------------\n");
                    s46 :
                    printf("Enter your option : ");

                    if(scanf(" %d", &s1) != 1)
                    {
                        printf("-----------------------------------------------------------------|\n");
                        printf("Error!..\nOption should be a number.\n");
                        printf("-----------------------------------------------------------------|\n");
                        printf("Retry!..\nEnter the correct option.\n");
                        printf("-----------------------------------------------------------------|\n");
                        while(getchar() != '\n');
                        goto s46;
                    }
                    while(getchar() != '\n');
                    printf("\n--------------------------------------------------------------|\n");
                    if(s1 > s)
                    {
                        printf("\n------------------------------------------------------------|\n");
                        printf("\nError! Invalid option.\nTry entering a valid option.");
                        printf("\n------------------------------------------------------------|\n");
                        goto s2;
                    }
                    int op = arr1[s1 -1];
                     printf("\n-----------------------------------------------------------------------------------\n");
                     printf("|                                  ADDRESS BOOK                                   |");
                     printf("\n-----------------------------------------------------------------------------------");
                     printf("\n|%-5s|        %-18s|    %-12s|           %-20s|","S.NO", "NAME", "PHONE NO", "MAIL ID");
                     printf("\n-----------------------------------------------------------------------------------\n");
                     printf("|  %d%-2s| %-25s|   %-13s|          %-21s|\n", op+1,".", arr -> contact_details[op].name, arr -> contact_details[op].phone_number, arr -> contact_details[op].mail_id);
                     printf("-----------------------------------------------------------------------------------\n");
                }
                else if(e == 1)
                {
                    printf("\n-----------------------|\n");
                    printf("%d Contact Found.", s);
                    printf("\n-----------------------|\n");
                }
                if(!e)
                {
                    if(s > 1)
                    {
                        printf("\n----------------------------------------------------------------------|\n");
                        printf("\n%d Contacts Found in the same Name.\n", s);
                        int s1;
                        s3 :

                            printf("Which Contact do you like to make changes from the above Contacts?\n");
                            printf("\n----------------------------------------------------------------------|\n");
                            for(int i =1; i<=s; i++)
                            {
                                printf("\n-----------------\n");
                                printf("|Contact No | %d |", i);

                            }
                            printf("\n-----------------\n");
                            s47 :
                            printf("Enter your option : ");

                            if(scanf(" %d", &s1) != 1)
                            {
                                printf("-----------------------------------------------------------------|\n");
                                printf("Error!..\nOption should be a number.\n");
                                printf("-----------------------------------------------------------------|\n");
                                printf("Retry!..\nEnter the correct option.\n");
                                printf("-----------------------------------------------------------------|\n");
                                while(getchar() != '\n');
                                goto s47;
                            }
                            while(getchar() != '\n');
                            printf("\n----------------------------------------------------------------------|\n");
                            if(s1 > s)
                            {
                                printf("\n----------------------------------------------------------------------|\n");
                                printf("\nError! Invalid option.\nTry entering a valid option.");
                                printf("\n----------------------------------------------------------------------|\n");
                                goto s3;
                            }
                            return arr1[s1 -1];
                    }
                    else if(s ==1)
                    {
                        printf("\n-----------------------|\n");
                        printf("%d Contact Found.", s);
                        printf("\n-----------------------|\n");
                        return arr1[0];
                    }
                    else
                    {
                        goto d3;
                    }
                    if(f == 1)
                    {
                        return arr1[0];
                    }
                    else
                    {
                        return -1;
                    }

                }
                d3 :
                    break;

            case 2 : // Search by phone number
                int t1 = arr -> contact_count;
                int f1 = 0, flg1 = 0;
                s = 0;
                if(validate_phone_number(arr, t1, 1))
                {
                    for(int j =0; j < arr -> contact_count; j++)
                    {
                        if(strcmp(arr->contact_details[t1].phone_number, arr->contact_details[j].phone_number) == 0)
                        {
                            if(flg1 == 0)
                            {
                                printf("\n-----------------------------------------------------------------------------------\n");
                                printf("|                                  ADDRESS BOOK                                   |");
                                printf("\n-----------------------------------------------------------------------------------");
                                printf("\n|%-5s|        %-18s|    %-12s|           %-20s|","S.NO", "NAME", "PHONE NO", "MAIL ID");
                                printf("\n-----------------------------------------------------------------------------------\n");
                                flg1 =1;
                            }
                            printf("|  %d%-2s| %-25s|   %-13s|          %-21s|\n", s = s+1, ".", arr -> contact_details[j].name, arr -> contact_details[j].phone_number, arr -> contact_details[j].mail_id);
                            printf("-----------------------------------------------------------------------------------\n");
                            f1 = 1;
                            if(!e)
                            {
                                if(f1 == 1)
                                {
                                    return j;
                                }
                                else
                                {
                                    return -1;
                                }
                            }
                        }
                    }
                    if(f1==0)
                    {
                        printf("\n------------------------------------------------------|");
                        printf("\nContact not found.\nTry entering another Phone Number.\n");
                        printf("\n------------------------------------------------------|\n");
                    }
                }
                break;

            case 3  : // Search by email
                int t2 = arr -> contact_count;
                int f2 = 0, flg2 = 0;
                s = 0;
                if(validate_mail_id(arr, t2, 1))
                {
                    for(int j =0; j < arr -> contact_count; j++)
                    {
                        if(strcmp(arr->contact_details[t2].mail_id, arr->contact_details[j].mail_id) == 0)
                        {
                            if(flg2 == 0)
                            {
                                printf("\n-----------------------------------------------------------------------------------\n");
                                printf("|                                  ADDRESS BOOK                                   |");
                                printf("\n-----------------------------------------------------------------------------------");
                                printf("\n|%-5s|        %-18s|    %-12s|           %-20s|","S.NO", "NAME", "PHONE NO", "MAIL ID");
                                printf("\n-----------------------------------------------------------------------------------\n");
                                flg2 =1;
                            }
                            printf("|  %d%-2s| %-25s|   %-13s|          %-21s|\n", s = s+1, ".",  arr -> contact_details[j].name, arr -> contact_details[j].phone_number, arr -> contact_details[j].mail_id);
                            printf("-----------------------------------------------------------------------------------\n");
                            f2 = 1;
                            if(!e)
                            {
                                if(f2 == 1)
                                {
                                    return j;
                                }
                                else
                                {
                                    return -1;
                                }
                            }
                        }
                    }
                    if(f2==0)
                    {
                        printf("\n------------------------------------------------------|");
                        printf("\nContact not found.\nTry entering another Mail ID.");
                        printf("\n------------------------------------------------------|\n");
                    }
                }
                break;

            case 4 : // Exit search
                printf("\n-----------------------------------------------|\n");
                printf("\nExiting Search.....\n");
                printf("\n-----------------------------------------------|\n");
                break;

            default : // Invalid option
                printf("\n-----------------------------------------------|\n");
                printf("\nInvalid Choice.\n Enter your option again.\n");
                printf("\n-----------------------------------------------|\n");
        }
    }while(c != 4);

    return -1;
}

/* Edit existing contacts in the address book */
int edit_contact(struct Address_book *addressbook)
{
    int c1;
    do
    {
        printf("\nSearch the contact to edit.\n");
        int edit = search_contacts(addressbook, 0);
        if(edit == -1)
        {
            printf("\n-----------------------------------------------|\n");
            printf("\nExiting Edit Contacts.....\n");
            printf("\n-----------------------------------------------|\n");
            break;
        }
        printf("\nEdit Contact menu : \n1.Name \n2.Mobile number\n3.Mail ID\n4.All edit\n5.Exit\nEnter the option : ");
        s49 :
        if(scanf(" %d", &c1) != 1)
        {
            printf("-----------------------------------------------------------------|\n");
            printf("Error!..\nOption should be a number.\n");
            printf("-----------------------------------------------------------------|\n");
            printf("Retry!..\nEnter the correct option.\n");
            printf("-----------------------------------------------------------------|\n");
            while(getchar() != '\n');
            goto s49;
        }
        while(getchar() != '\n');
        int f1 = 0;
        switch(c1)
        {
            case 1 : // Edit name
                if(validate_name(addressbook, edit, 0))
                {
                    printf("\n------------------|\n");
                    printf("\nName updated.\n");
                    printf("\n------------------|\n");
                    f1 =1;
                }
                break;

            case 2 : // Edit phone number
                if(validate_phone_number(addressbook, edit, 0))
                {
                    printf("\n--------------------------|\n");
                    printf("\nPhone Number updated.\n");
                    printf("\n--------------------------|\n");
                    f1 =1;
                }
                break;

            case 3 : // Edit email
                if(validate_mail_id(addressbook, edit, 0))
                {
                    printf("\n--------------------------|\n");
                    printf("\nMail ID updated.\n");
                    printf("\n--------------------------|\n");
                    f1 = 1;
                }
                break;

            case 4 : // Edit all fields
                if(validate_name(addressbook, edit, 0) && validate_phone_number(addressbook, edit, 0) && validate_mail_id(addressbook, edit, 0))
                   {
                        printf("\n--------------------------|\n");
                        printf("\nAll Contacts updated.\n");
                        printf("\n--------------------------|\n");
                        f1 = 1;
                   }
                break;

            case 5 : // Exit edit
                printf("\n-------------------------------|\n");
                printf("\nExiting Edit Contacts.....");
                printf("\n-------------------------------|\n");
                break;

            default : // Invalid option
                printf("\n----------------------------------------------|\n");
                printf("\nInvalid Choice.\n Enter your option again.");
                printf("\n----------------------------------------------|\n");
        }
        if(f1)
        {
            sorting_names(addressbook);
        }
    }while(c1 != 5);
    return 0;
}

/* Delete contacts from the address book */
int delete_contact(struct Address_book *arr)
{
    printf("\nSearch the contact to delete.\n");
    char c1;
    int dlt = search_contacts(arr, 0);
    if(dlt != -1)
    {
        a36 :
            printf("\nDo you really want to delete this Contact?\nEnter (Y/N) : ");
            scanf(" %c", &c1);
            while(getchar() != '\n');
            if(c1 == 'Y' || c1 == 'y')
            {
                printf("\n-----------------------------------------------|\n");
                // Shift all contacts after the deleted one
                for(int i = dlt; i < arr -> contact_count - 1 ; i++)
                {
                    strcpy(arr -> contact_details[i].name, arr -> contact_details[i+1].name);
                    strcpy(arr -> contact_details[i].phone_number, arr -> contact_details[i+1].phone_number);
                    strcpy(arr -> contact_details[i].mail_id, arr -> contact_details[i+1].mail_id);
                }
                arr -> contact_count--;
                printf("\n-----------------------------------------------------------------|");
                printf("\nContact deleted successfully.");
                printf("\n-----------------------------------------------------------------|\n");
                printf("\n-----------------------------------------------------------------|");
                printf("\nAddress Book : \nTotal Contacts present in Address Book   : %d\nTotal Space remaining in the Address Book: %d", arr -> contact_count, 100 - arr -> contact_count);
                printf("\n-----------------------------------------------------------------|\n");
            }
            else if(c1 == 'N' || c1 == 'n')
            {
                printf("\n-------------------------------|\n");
                printf("\nExiting Delete Contacts.....\n");
                printf("\n-------------------------------|\n");
                return 0;
            }
            else
            {
                printf("\nError! Invalid option.\n");
                goto a36;
            }
    }
    return 0;
}

/* Validate and set a contact name */
int validate_name(struct Address_book *arr, int i1, int s)
{
    char ch;
    a1 :
        printf("Enter Name : ");
        scanf(" %49[^\n]", arr->contact_details[i1].name);
        while(getchar() != '\n');
        int f = 0;
        int len = strlen(arr->contact_details[i1].name);
        // Check first character is alphabetic
        if(!isalpha(arr -> contact_details[i1].name[0]))
        {
            printf("\n---------------------------------------------------------------------------------------|");
            printf("\nError! Name Should not contain any numbers or special characters at the beginning.");
            printf("\n---------------------------------------------------------------------------------------|\n");
            goto a13;
        }
        // Check remaining characters are alphanumeric
        for(int m =1; m<len; m++)
        {
            if(!isalnum(arr -> contact_details[i1].name[m]))
            {
                f =1;
                printf("\n-----------------------------------------------------------------------|");
                printf("\nError! Name Should not contain any numbers or special characters.");
                printf("\n-----------------------------------------------------------------------|\n");
                goto name_valid;
            }
        }
    name_valid :
        if(f == 1)
        {
            printf("\nError! Entered Name is Invalid.\n");
            a13:
                printf("Would you like to re-enter Name?\nEnter (Y/N) : ");
                scanf(" %c", &ch);
                while(getchar() != '\n');
                if(ch == 'Y' || ch == 'y')
                {
                    printf("\n-----------------------------------------------|\n");
                    goto a1;
                }
                else if(ch == 'N' || ch == 'n')
                {
                    printf("\n-----------------------------------------------|\n");
                    return 0;
                }
                else
                {
                    printf("\nError! Invalid option.\n");
                    goto a13;
                }
        }
    return 1;
}

/* Validate and set a phone number */
int validate_phone_number(struct Address_book *arr, int i, int s)
{
    char ch;
     a2:
        printf("Enter Phone Number : ");
        scanf(" %49[^\n]", arr->contact_details[i].phone_number);
        while(getchar() != '\n');
        int m =0, f = 0;
        // Check first digit is between 6-9
        if(arr->contact_details[i].phone_number[0] < '6' || arr->contact_details[i].phone_number[0] > '9')
        {
            f = 1;
            printf("\n---------------------------------------------------------------------------------|");
            printf("\nError! Entered Phone Number is Invalid.\nIt should start in from 6 to 9 digit.");
            printf("\n---------------------------------------------------------------------------------|\n");
            goto a23;

        }
        // Check all characters are digits
        while(arr->contact_details[i].phone_number[m])
        {
            if(!isdigit(arr->contact_details[i].phone_number[m]))
            {
                f = 1;
                printf("\n----------------------------------------------------------------------------|");
                printf("\nError! Entered Phone Number is Invalid.\nIt should only contain digits.");
                printf("\n----------------------------------------------------------------------------|\n");
                goto a23;
            }
            m++;
        }
        // Check length is exactly 10 digits
        if(strlen(arr->contact_details[i].phone_number) != 10)
        {
            printf("\n--------------------------------------------------|");
            printf("\nError! Phone Number should contain 10 digits.");
            printf("\n--------------------------------------------------|\n");
            goto a23;
        }
        if(f == 1)
        {
            a23:
                printf("Would you like to re-enter Phone Number?\nEnter (Y/N) : ");
                scanf(" %c", &ch);
                while(getchar() != '\n');
                if(ch == 'Y' || ch == 'y')
                {
                     printf("\n-----------------------------------------------|\n");
                    goto a2;
                }
                else if(ch == 'N' || ch == 'n')
                {
                     printf("\n-----------------------------------------------|\n");
                    return 0;
                }
                else
                {
                    printf("\nError! Invalid option.\n");
                    goto a23;
                }
        }
        // Check for duplicate phone numbers (if not in search mode)
        if(!s)
        {
            for(int j = 0; j<i; j++)
            {
                if(strcasecmp(arr->contact_details[i].phone_number, arr->contact_details[j].phone_number) == 0)
                {
                    printf("\n-----------------------------------------------------------------------|");
                    printf("\nError! Entered Phone Number is already present in the Address Book.");
                    printf("\n-----------------------------------------------------------------------|\n");
                    goto a23;
                }
            }
        }
        return 1;
}

/* Validate and set an email address */
int validate_mail_id(struct Address_book *arr, int i, int s)
{
    char ch;
    // List of valid top-level domains
    const char *valid_tlds[] = {".com", ".in", ".org", ".net", ".edu"};
    int valid_tld_count = 5;

    a3:
        printf("Enter Mail ID : ");
        scanf(" %49[^\n]", arr->contact_details[i].mail_id);
        while(getchar() != '\n');
        char *email = arr->contact_details[i].mail_id;
        int len = strlen(email);
        int at_p = -1;
        int f = 1;

        // Check minimum length
        if(len <= 6)
        {
            f = 0;
            printf("\n--------------------------------------------------|");
            printf("\nError: Mail ID must be at least 7 characters.\n");
            printf("\n--------------------------------------------------|");
            goto a33;
        }

        // Check for valid TLD
        int tld_valid = 0;
        const char *found_tld = NULL;
        for(int t = 0; t < valid_tld_count; t++)
        {
            int tld_len = strlen(valid_tlds[t]);
            if(len >= tld_len && strcmp(email + len - tld_len, valid_tlds[t]) == 0)
            {
                tld_valid = 1;
                found_tld = valid_tlds[t];
                break;
            }
        }

        if(!tld_valid)
        {
            f = 0;
            printf("\n-------------------------------------------------------------------------------|");
            printf("\nError: Mail ID must end with valid TLD's like .com, .in, .org, .net or .edu\n");
            printf("\n-------------------------------------------------------------------------------|\n");
            goto a33;
        }

        // Check for duplicate TLD
        if(found_tld)
        {
            char *first_tld = strstr(email, found_tld);
            if(first_tld && strstr(first_tld + strlen(found_tld), found_tld))
            {
                f = 0;
                printf("\n-------------------------------------------------------------|");
                printf("\nError: '%s' appears multiple times in Mail ID\n", found_tld);
                printf("\n-------------------------------------------------------------|\n");
                goto a33;
            }
        }
        // Check for exactly one @ symbol
        int at = 0;
        for(int k = 0; k<len; k++)
        {
            if(arr -> contact_details[i].mail_id[k] == '@')
            {
                at++;
                at_p = k;
            }
        }

        if(at != 1)
        {
            f = 0;
            printf("\n-----------------------------------------------------|");
            printf("\nError: Mail ID must contain exactly one '@'.");
            printf("\n-----------------------------------------------------|\n");
            goto valid_mail;
        }
        // Check starts with lowercase letter
        if(!islower(arr -> contact_details[i].mail_id[0]))
        {
            f = 0;
            printf("\n-------------------------------------------------------|");
            printf("\nError: Mail ID must start with a lowercase alphabet.");
            printf("\n-------------------------------------------------------|\n");
            goto valid_mail;
        }
        // Validate local part (before @)
        for(int m = 1; m < at_p; m++)
        {
            char c = arr -> contact_details[i].mail_id[m];
            if(!isalnum(c) || isupper(c))
            {
                f = 0;
                printf("\n-------------------------------------------------------------------------------------------|");
                printf("\nError! Mail ID should only contain lowercase alphabets/digits in local part(before '@').");
                printf("\n-------------------------------------------------------------------------------------------|\n");
                goto valid_mail;
            }
        }
        // Check domain part exists
        if(at_p >= len -5)
        {
            f = 0;
            printf("\n-----------------------------------------------------------------------------------|");
            printf("\nError! In your mail id there is no domain part(after '@').");
            printf("\n-----------------------------------------------------------------------------------|\n");
            goto valid_mail;
        }
        // Validate domain part (after @)
        for(int m1 = at_p +1; m1 < len -4; m1++)
        {
            if(!isalpha(arr -> contact_details[i].mail_id[m1]))
            {
                f = 0;
                printf("\n----------------------------------------------------|");
                 printf("\nError: Only alphabets allowed for domain part.");
                printf("\n----------------------------------------------------|\n");
                goto valid_mail;
            }
        }

        valid_mail :
            if(f == 0)
            {
                printf("\nError! Entered Mail ID is Invalid.\n");
                a33:
                    printf("Would you like to re-enter Mail ID?\nEnter (Y/N) : ");
                    scanf(" %c", &ch);
                    while(getchar() != '\n');
                    if(ch == 'Y' || ch == 'y')
                    {
                         printf("\n-----------------------------------------------|\n");
                        goto a3;
                    }
                    else if(ch == 'N' || ch == 'n')
                    {
                         printf("\n-----------------------------------------------|\n");
                        return 0;
                    }
                    else
                    {
                        printf("\nError! Invalid option.\n");
                        goto a33;
                    }
            }
        // Check for duplicate emails (if not in search mode)
        if(!s)
        {
            for(int j3 = 0; j3 < i; j3++)
            {
                if(strcasecmp(arr->contact_details[i].mail_id, arr->contact_details[j3].mail_id) == 0)
                {
                    printf("\n--------------------------------------------------------------------|");
                    printf("\nError! Entered Mail ID is already present in the Address Book.\n");
                    printf("\n--------------------------------------------------------------------|\n");
                    goto a33;
                }
            }
        }
        return 1;
}

/* Sort contacts alphabetically by name */
void sorting_names(struct Address_book *arr)
{
    // Bubble sort implementation
    for(int i = 0; i < arr -> contact_count-1; i++)
    {
        for(int j = 0; j < arr -> contact_count-i-1; j++)
        {
            if(strcasecmp(arr->contact_details[j].name, arr->contact_details[j+1].name) > 0)
            {
                // Swap contacts
                arr -> contact_details[arr -> contact_count] = arr->contact_details[j];
                arr->contact_details[j] = arr->contact_details[j+1];
                arr->contact_details[j+1] = arr -> contact_details[arr -> contact_count];
            }
        }
    }
}
