/*
Name  : DIneshkumar R
Date  : 13/08/2025
Title : Project 1 -  Address Book
*/

#ifndef CONTACT
#define CONTACT

#define MAX_CONTACTS 100
#define MAX_LENGTH 50


//Structure Declarations
struct Contact
{
    char name[MAX_LENGTH];
    char phone_number[15];
    char mail_id[MAX_LENGTH];
};
//Nested Structure
struct Address_book
{
    struct Contact contact_details[101];
    int contact_count;
};

//Function Prototypes
void init_intitalization(struct Address_book *addressbook);
int create_contact(struct Address_book *arr);
void list_contacts(struct Address_book *arr);
int search_contacts(struct Address_book *arr, int e);
int edit_contact(struct Address_book *addressbook);
int delete_contact(struct Address_book *arr);
int save_contacts(struct Address_book *addressbook);
int load_contacts(struct Address_book *addressbook);
int validate_name(struct Address_book *arr, int i1, int s);
int validate_phone_number(struct Address_book *arr, int i, int s);
int validate_mail_id(struct Address_book *arr, int i, int s);
void sorting_names(struct Address_book *arr);

#endif
