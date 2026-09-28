#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"

//==================== VALIDATION FUNCTIONS ======================
 // Validation: Only alphabets and spaces allowed
int validate_name(const char *name)
{
  if (strlen(name) == 0) return 0;
  for (int i = 0; name[i] != '\0'; i++) {
    //Allowing alphabets and spaces (for names like "Alekya ")
    if (!isalpha(name[i]) && name[i] != ' ') {
      return 0; //Invalid
    }
  }
  return 1; //Valid
}
//Validation Only digits, exactly 10 digits, and completely unique
int validate_phonenum(AddressBook *addressBook, const char *phone) {
  //check length
  if (strlen(phone) != 10) return 0;
  // Check if all characters are digits
  for (int i = 0; i < 10; i++) {
    if (!isdigit(phone[i])) return 0;
  }
  // Check uniqueness against existing contacts
  for (int i = 0; i < addressBook->contactCount; i++) {
    if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
      printf("Error: This phone number already exists.\n");
      return 0;
    }
  }
  return 1; // Valid
}

// Validation: Must contain '@', '.com', text before '@', and text between them
int validate_email(const char *email) {
  char *at_ptr = strchr(email, '@');
  if (at_ptr == NULL) {
    return 0;
  } //No '@' found

  //Must have at least one character before '@'
  if (at_ptr == email) 
  { 
    return 0;
  }

  char *dot_com_ptr = strstr(at_ptr, ".com");
  if (dot_com_ptr == NULL) {
    return 0; // No '.com' found after '@'
  }
  // Must have at least one character between '@' and '.com'
  if (dot_com_ptr == (at_ptr + 1)) {
    return 0;
  }

  if (strcmp(dot_com_ptr, ".com") != 0) {
    return 0;
  }
  return 1; // Valid
}


void listContacts(AddressBook *addressBook)
{
	//list all the contacts..
  printf("\n-------------------------------------------------------------\n");
  printf("%-25s %-15s %-30s\n","NAME","PHONE NUM","EMAIL");
  printf("-------------------------------------------------------------\n");

  for(int i=0 ; i< addressBook -> contactCount ; i++) 
  {
    printf("%-25s %-15s %-30s\n", 
    addressBook -> contacts[i].name, 
    addressBook -> contacts[i].phone, 
    addressBook -> contacts[i].email);
  }

}

// ==================== CREATE CONTACT =========================

void createContact(AddressBook *addressBook)
{
  /* Define the logic to create a Contacts */
  // Check if the Address Book is maxed out
  if (addressBook->contactCount >= MAX_CONTACTS) 
  {
    printf("Error: Address Book is full (%d/%d contacts).\n", addressBook->contactCount, MAX_CONTACTS);
    return;
  }

  // Temporary buffers to hold input before confirming saving
  char temp_name[50];
  char temp_phone[20];
  char temp_email[50];

  printf("\n--- Create New Contact ---\n");

  // STEP 1: Read and Validate Name
  while (1) 
  {
    printf("Enter Name: ");
    scanf(" %[^\n]", temp_name); // Reads spaces as well until Enter is pressed

    if (validate_name(temp_name)) 
    {
      break; // Valid! Move to next step
    } 
    else 
    {
      printf("Error: Name must contain only alphabets and spaces. Please try again.\n");
    }
  }

  // STEP 2: Read and Validate Phone Number
  while (1) 
  {
    printf("Enter Phone Number (10 digits): ");
    scanf("%s", temp_phone);

    if (validate_phonenum(addressBook, temp_phone)) 
    {
      break; // Valid! Move to next step
    } else 
    {
      printf("Error: Phone number must be exactly 10 digits. Please try again.\n");
    }
  }

  // STEP 3: Read and Validate Email
  while (1) 
  {
    printf("Enter Email Address: ");
    scanf("%s", temp_email);

    if (validate_email(temp_email)) 
    {
      break; // Valid! Move to next step
    } else 
    {
      printf("Error: Invalid email format (Example: user@domain.com). Please try again.\n");
    }
  }

  // SAVING THE CONTACT: Copy temporary data into the permanent array
  int index = addressBook->contactCount;
  strcpy(addressBook->contacts[index].name, temp_name);
  strcpy(addressBook->contacts[index].phone, temp_phone);
  strcpy(addressBook->contacts[index].email, temp_email);

  // Increment total contact tracking count
  addressBook->contactCount++;

  printf("\nSuccess: Contact saved successfully!\n");
}

// (The remaining search, edit, and delete functions stay below here...)

void searchContact(AddressBook *addressBook)
{
  /* Define the logic to search a Contacts */
  {
    int choice;
    char query[50];
    int found = 0;

    // Check if the Address Book has any data to search
    if (addressBook->contactCount == 0) {
      printf("The Address Book is empty.\n");
      return;
    }

    printf("\n--- Search Contact Menu ---\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone Number\n");
    printf("3. Search by Email\n");
    printf("4. Back to Main Menu\n");
    printf("Enter your choice: ");
    fflush(stdout);
    scanf("%d", &choice);
    getchar(); // Clean up the newline character from the buffer

    switch (choice) 
    {
      case 1:
      printf("Enter Name to search: ");
      scanf(" %[^\n]", query); // Reads the full name including spaces
            
      // Loop through and look for an exact name match
      for (int i = 0; i < addressBook->contactCount; i++) 
      {
        if (strcmp(addressBook->contacts[i].name, query) == 0) 
        {
          if (!found) 
          { // Print headers only once when the first match is found
            printf("\n%-25s %-15s %-30s\n", "NAME", "PHONE NUM", "EMAIL");
            printf("-------------------------------------------------------------------------\n");
          }
          printf("%-25s %-15s %-30s\n", 
                  addressBook->contacts[i].name, 
                  addressBook->contacts[i].phone, 
                  addressBook->contacts[i].email);
          found = 1;
        }
      }
      if (!found) 
      {
        printf("Error: Contact with name '%s' is not present.\n", query);
      }
      break;

      case 2:
            printf("Enter Phone Number to search: ");
            scanf("%s", query);
            
            // Loop through and look for an exact phone match
            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].phone, query) == 0) {
                    if (!found) {
                        printf("\n%-25s %-15s %-30s\n", "NAME", "PHONE NUM", "EMAIL");
                        printf("-------------------------------------------------------------------------\n");
                    }
                    printf("%-25s %-15s %-30s\n", 
                           addressBook->contacts[i].name, 
                           addressBook->contacts[i].phone, 
                           addressBook->contacts[i].email);
                    found = 1;
                }
            }
            if (!found) {
                printf("Error: Contact with phone number '%s' is not present.\n", query);
            }
            break;

        case 3:
            printf("Enter Email Address to search: ");
            scanf("%s", query);
            
            // Loop through and look for an exact email match
            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].email, query) == 0) {
                    if (!found) {
                        printf("\n%-25s %-15s %-30s\n", "NAME", "PHONE NUM", "EMAIL");
                        printf("-------------------------------------------------------------------------\n");
                    }
                    printf("%-25s %-15s %-30s\n", 
                           addressBook->contacts[i].name, 
                           addressBook->contacts[i].phone, 
                           addressBook->contacts[i].email);
                    found = 1;
                }
            }
            if (!found) {
                printf("Error: Contact with email '%s' is not present.\n", query);
            }
            break;

        case 4:
            return; // Go back to the main menu immediately

        default:
            printf("Invalid choice. Returning to Main Menu.\n");
            return;
    }
    
    if (found) {
        printf("-------------------------------------------------------------------------\n");
    }
  }

}

void editContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
    int search_choice, edit_choice;
    char query[50];
    int found_index = -1; // Stores the index of the contact we want to edit

    if (addressBook->contactCount == 0) {
        printf("The Address Book is empty. Nothing to edit.\n");
        return;
    }

    printf("\n--- Edit Contact Search Menu ---\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone Number\n");
    printf("3. Search by Email\n");
    printf("Enter your choice: ");
    fflush(stdout);
    scanf("%d", &search_choice);
    getchar(); // Clean newline

    switch (search_choice) {
        case 1: // Search by Name (Handles duplicates)
            printf("Enter Name to edit: ");
            scanf(" %[^\n]", query);

            int index_map[MAX_CONTACTS];
            int match_count = 0;

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].name, query) == 0) {
                    index_map[match_count] = i;
                    match_count++;
                }
            }

            if (match_count == 0) {
                printf("Error: Contact with name '%s' is not present.\n", query);
                return;
            } 
            else if (match_count == 1) {
                found_index = index_map[0];
            } 
            else {
                printf("\nMultiple contacts found with that name:\n");
                printf("%-4s %-25s %-15s %-30s\n", "S.No", "NAME", "PHONE NUM", "EMAIL");
                printf("-------------------------------------------------------------------------\n");
                for (int i = 0; i < match_count; i++) {
                    int actual_idx = index_map[i];
                    printf("%-4d %-25s %-15s %-30s\n", 
                           i + 1, 
                           addressBook->contacts[actual_idx].name, 
                           addressBook->contacts[actual_idx].phone, 
                           addressBook->contacts[actual_idx].email);
                }
                printf("-------------------------------------------------------------------------\n");
                
                int serial_no;
                printf("Enter the Serial Number (S.No) of the contact to edit: ");
                fflush(stdout);
                scanf("%d", &serial_no);
                getchar();

                if (serial_no < 1 || serial_no > match_count) {
                    printf("Error: Invalid serial number selection.\n");
                    return;
                }
                found_index = index_map[serial_no - 1];
            }
            break;

        case 2: // Search by Phone Number
            printf("Enter Phone Number to edit: ");
            scanf("%s", query);

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].phone, query) == 0) {
                    found_index = i;
                    break;
                }
            }
            if (found_index == -1) {
                printf("Error: Contact with phone number '%s' is not present.\n", query);
                return;
            }
            break;

        case 3: // Search by Email
            printf("Enter Email Address to edit: ");
            scanf("%s", query);

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].email, query) == 0) {
                    found_index = i;
                    break;
                }
            }
            if (found_index == -1) {
                printf("Error: Contact with email '%s' is not present.\n", query);
                return;
            }
            break;

        default:
            printf("Invalid choice. Returning to Main Menu.\n");
            return;
    }

    // ==================== EDIT SUB-MENU ====================
    printf("\nContact found! What field would you like to edit?\n");
    printf("1. Edit Name\n");
    printf("2. Edit Phone Number\n");
    printf("3. Edit Email\n");
    printf("Enter your choice: ");
    fflush(stdout);
    scanf("%d", &edit_choice);
    getchar(); // Clean newline

    char temp_input[50];

    switch (edit_choice) {
        case 1:
            while (1) {
                printf("Enter New Name: ");
                scanf(" %[^\n]", temp_input);
                if (validate_name(temp_input)) {
                    strcpy(addressBook->contacts[found_index].name, temp_input);
                    break;
                } else {
                    printf("Error: Name must contain only alphabets and spaces.\n");
                }
            }
            break;

        case 2:
            while (1) {
                printf("Enter New Phone Number (10 digits): ");
                scanf("%s", temp_input);
                
                // Temporary twist: If they type their OWN existing number, let it pass validation
                if (strcmp(addressBook->contacts[found_index].phone, temp_input) == 0) {
                    break; 
                }
                
                if (validate_phonenum(addressBook, temp_input)) {
                    strcpy(addressBook->contacts[found_index].phone, temp_input);
                    break;
                } else {
                    printf("Error: Phone number must be 10 unique digits.\n");
                }
            }
            break;

        case 3:
            while (1) {
                printf("Enter New Email Address: ");
                scanf("%s", temp_input);
                if (validate_email(temp_input)) {
                    strcpy(addressBook->contacts[found_index].email, temp_input);
                    break;
                } else {
                    printf("Error: Invalid email format.\n");
                }
            }
            break;

        default:
            printf("Invalid choice. Modification cancelled.\n");
            return;
    }

    printf("\nSuccess: Contact updated successfully!\n");
}

void deleteContact(AddressBook *addressBook)
{
    /* Define the logic for Editcontact */
    int choice;
    char query[50];
    int found_index = -1; // Stores the index of the contact we want to delete

    if (addressBook->contactCount == 0) {
        printf("The Address Book is empty. Nothing to delete.\n");
        return;
    }

    printf("\n--- Delete Contact Menu ---\n");
    printf("1. Search by Name\n");
    printf("2. Search by Phone Number\n");
    printf("3. Search by Email\n");
    printf("Enter your choice: ");
    fflush(stdout);
    scanf("%d", &choice);
    getchar(); // Clean up newline

    switch (choice) {
        case 1: // Delete by Name (Handles duplicates)
            printf("Enter Name to delete: ");
            scanf(" %[^\n]", query);

            int index_map[MAX_CONTACTS]; // Array to store the matching indices
            int match_count = 0;

            // Find all matching contacts and store their positions
            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].name, query) == 0) {
                    index_map[match_count] = i;
                    match_count++;
                }
            }

            if (match_count == 0) {
                printf("Error: Contact with name '%s' is not present.\n", query);
                return;
            } 
            else if (match_count == 1) {
                // Only one contact matches, select it directly
                found_index = index_map[0];
            } 
            else {
                // Multiple contacts found, display them with serial numbers
                printf("\nMultiple contacts found with that name:\n");
                printf("%-4s %-25s %-15s %-30s\n", "S.No", "NAME", "PHONE NUM", "EMAIL");
                printf("-------------------------------------------------------------------------\n");
                for (int i = 0; i < match_count; i++) {
                    int actual_idx = index_map[i];
                    printf("%-4d %-25s %-15s %-30s\n", 
                           i + 1, 
                           addressBook->contacts[actual_idx].name, 
                           addressBook->contacts[actual_idx].phone, 
                           addressBook->contacts[actual_idx].email);
                }
                printf("-------------------------------------------------------------------------\n");
                
                int serial_no;
                printf("Enter the Serial Number (S.No) to delete: ");
                fflush(stdout);
                scanf("%d", &serial_no);
                getchar();

                if (serial_no < 1 || serial_no > match_count) {
                    printf("Error: Invalid serial number selection.\n");
                    return;
                }
                // Map the user's serial selection back to the actual index position
                found_index = index_map[serial_no - 1];
            }
            break;

        case 2: // Delete by Phone Number
            printf("Enter Phone Number to delete: ");
            scanf("%s", query);

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].phone, query) == 0) {
                    found_index = i;
                    break; // Found unique match, stop searching
                }
            }
            if (found_index == -1) {
                printf("Error: Contact with phone number '%s' is not present.\n", query);
                return;
            }
            break;

        case 3: // Delete by Email
            printf("Enter Email Address to delete: ");
            scanf("%s", query);

            for (int i = 0; i < addressBook->contactCount; i++) {
                if (strcmp(addressBook->contacts[i].email, query) == 0) {
                    found_index = i;
                    break; // Found unique match, stop searching
                }
            }
            if (found_index == -1) {
                printf("Error: Contact with email '%s' is not present.\n", query);
                return;
            }
            break;

        default:
            printf("Invalid choice. Returning to Main Menu.\n");
            return;
    }

    // ==================== SHIFTING LOGIC ====================
    // Overwrite the contact at found_index by shifting all items after it up by one slot
    for (int i = found_index; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    // Decrement total count
    addressBook->contactCount--;
    printf("\nSuccess: Contact deleted successfully!\n");

}
void saveContactsToFile(AddressBook *addressBook)
{
    // Open the file in write mode ("w")
    FILE *fptr = fopen("contacts.csv", "w");
    if (fptr == NULL) {
        printf("Error: Could not open file to save contacts.\n");
        return;
    }

    // 1. Write the total contact count as the very first line
    fprintf(fptr, "%d\n", addressBook->contactCount);

    // 2. Loop through and save each contact in CSV format
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fptr, "%s,%s,%s\n", 
                addressBook->contacts[i].name, 
                addressBook->contacts[i].phone, 
                addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf("Success: Contacts saved to 'contacts.csv' successfully!\n");
}

void loadContactsFromFile(AddressBook *addressBook)
{
    // Open the file in read mode ("r")
    FILE *fptr = fopen("contacts.csv", "r");
    if (fptr == NULL) {
        // If the file doesn't exist yet, it's fine! Just start fresh with dummy records.
        printf("No saved database found. Loading initial setup.\n");
        initialize(addressBook);
        return;
    }

    // 1. Read the saved contact count from the first line
    if (fscanf(fptr, "%d\n", &addressBook->contactCount) != 1) {
        addressBook->contactCount = 0;
    }

    // 2. Loop and read each line according to your exact formatting rules
    for (int i = 0; i < addressBook->contactCount; i++) {
        // %[^,] means: read everything up until a comma
        fscanf(fptr, "%[^,],%[^,],%s\n", 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }

    fclose(fptr);
    printf("Success: Loaded %d contacts from storage.\n", addressBook->contactCount);
}