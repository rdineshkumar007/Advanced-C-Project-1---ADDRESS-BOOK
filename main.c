/*
Name  : DIneshkumar R
Date  : 13/08/2025
Title : Project 1 - Address Book
*/

#include <stdio.h>
#include "contact.h"  // Include the contact header file

/* Main function - Entry point of the Address Book program */
int main()
{
    int option;  // Variable to store user's menu choice
    struct Address_book addressbook;  // Create address book structure

    // Load existing contacts from file at program start
    load_contacts(&addressbook);

    // Display initial contact count and available space
    printf("\n-----------------------------------------------------------------|");
    printf("\nAddress Book : \nTotal Contacts present in Address Book   : %d\nTotal Space remaining in the Address Book: %d",
           addressbook.contact_count, 100 - addressbook.contact_count);
    printf("\n-----------------------------------------------------------------|\n");

    // Main program loop - shows menu until user exits
    do
    {
        s43:  // Label for input validation retry
        printf("\n-----------------------------------------------------------------|");
        printf("\nAddress Book Menu : \n");
        printf("1.Add Contacts\n2.Edit Contact\n3.Delete Contact\n4.Search Contact\n5.Display Contact\n6.Save Contact\n7.Save Contact and Exit.\n8.Exit Menu\n");
        printf("\nEnter the option : ");

        // Validate input is a number
        if(scanf(" %d", &option) != 1)
        {
            printf("-----------------------------------------------------------------|\n");
            printf("Error!..\nOption should be a digit.\n");
            printf("-----------------------------------------------------------------|\n");
            printf("Retry!..\nEnter the correct values.\n");
            printf("-----------------------------------------------------------------|\n");
            while(getchar() != '\n');  // Clear input buffer
            goto s43;  // Retry input
        }
        while(getchar() != '\n');  // Clear any extra characters
        printf("-----------------------------------------------------------------|\n");

        // Process user's menu selection
        switch (option)
        {
            case 1:  // Add new contacts
                create_contact(&addressbook);
                break;

            case 2:  // Edit existing contact
                edit_contact(&addressbook);
                break;

            case 3:  // Delete contact
                delete_contact(&addressbook);
                break;

            case 4:  // Search contacts
                search_contacts(&addressbook, 1);  // 1 indicates search mode
                break;

            case 5:  // Display all contacts
                list_contacts(&addressbook);
                break;

            case 6:  // Save contacts to file
                save_contacts(&addressbook);
                break;

            case 7:  // Save and exit
                save_contacts(&addressbook);
                printf("\n-----------------------------------------------------------------|\n");
                printf("INFO : Saving Contacts and Existing Address Book......");
                printf("\n-----------------------------------------------------------------|\n");
                option = 8;  // Set to exit value
                break;

            case 8:  // Exit without saving
                printf("\n-----------------------------------------------------------------|\n");
                printf("INFO : Existing Address Book Menu......");
                printf("\n-----------------------------------------------------------------|\n");
                break;

            default:  // Invalid option
                printf("\n-----------------------------------------------------------------|\n");
                printf("Invalid option.\nRetry! Enter a correct option.");
                printf("\n-----------------------------------------------------------------|\n");
                break;
        }
    } while(option != 8);  // Continue until exit option chosen

    return 0;  // End program
}
