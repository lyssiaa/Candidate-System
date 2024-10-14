/*Alyssia Salas, LA #2, 10/13/2023*/
/*files.c allows the user to load, display, sort, add, search, and remove candidates, with the 
added challenge of decrypting and encrypting candidate names using a secret code found in a separate file, codes.txt.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a candidate
typedef struct
{
  int identification;
  char first_name[50];
  char last_name[50];
  char date[5]; // Format: YYT
  int ranking1;
  int ranking2;
  char code;
} candidate_t;

// Function prototypes
int nrecruits(char filename1[]);
candidate_t *load_data(int *number);
void display_database(candidate_t *dbase, int count_1);
void sort_database(candidate_t *dbase, int count_1);
void search_candidate(candidate_t *dbase, int count_1);
void remove_candidate(candidate_t *dbase, int *count_1);
candidate_t *add_candidate(candidate_t *dbase, int *count_1);
void save_database(candidate_t *dbase, int count_1, int *error);
void sort_identification(candidate_t *dbase, int count_1);
void sort_firstname(candidate_t *dbase, int count_1);
void sort_lastname(candidate_t *dbase, int count_1);
void sort_date(candidate_t *dbase, int count_1);
void sort_ranking1(candidate_t *dbase, int count_1);
void sort_ranking2(candidate_t *dbase, int count_1);

int main(void)
{
  /*variable declaration*/
  int count;
  int error;
  int choice;

  candidate_t *database;

  /*print menu for user*/
  /*do-while loop to ensure that the user has the opportunity to perform multiple actions within
   the program and can repeatedly make menu choices until they choose to exit. */
  do
  {
    printf("[1] Load the Data\n");
    printf("[2] Display the database\n");
    printf("[3] Sort the database\n");
    printf("[4] Add a new candidate\n");
    printf("[5] Search for a candidate by name\n");
    printf("[6] Remove a candidate from database\n");
    printf("[7] Save the database into a new file\n");
    printf("[8] Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      // Load the data from a file into the database
      database = load_data(&count);
      if (database == NULL)
      {
        printf("File cannot be opened\n");
        return (1);
      }
      break;
    case 2:
      // Display the contents of the database
      display_database(database, count);
      break;
    case 3:
      // Sort the database based on user-defined criteria
      sort_database(database, count);
      break;
    case 4:
      // Add a new candidate to the database
      database = add_candidate(database, &count);
      break;
    case 5:
      // Search for a candidate in the database
      search_candidate(database, count);
      break;
    case 6:
      // Remove a candidate from the database
      remove_candidate(database, &count);
      break;
    case 7:
      // Save the database to a file
      save_database(database, count, &error);
        if (error == 1)
      {
        printf("Cannot write to this file\n");
        return (1);
      }
      break;
    case 8:
      // exit
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 8);
  free(database);
  return (0);
}

// Function to count the number of recruits in a file
int nrecruits(char filename1[])
{
  int i;
  FILE *afile;
  afile = fopen(filename1, "r");
  if (afile == NULL)
  {
    printf("Error opening the file for reading.\n");
    return (1);
  }
  // Declare a structure 'candidate_t' to hold data from the file.
  candidate_t person;
  i = 0; // Initialize a counter for the number of records.
  // Read data from the file while the end of the file is not reached.
  while (fscanf(afile, "%d %s %s %s %d %d", &person.identification, person.first_name, person.last_name, person.date, &person.ranking1, &person.ranking2) != EOF)
  {
    i++;
  }
  fclose(afile);
  // Return the number of records found in the file.
  return (i);
}

// Function to load data from files
candidate_t *load_data(int *number)
{
  char filename1[100];
  char filename2[100]; /*string for file name*/
  int *id;
  char *codes;
  int i, j, k;
  candidate_t *point; /*'candidate_t' is a user-defined structure */
  FILE *afile;        /* File pointer for the first file */
  FILE *efile;        /* File pointer for the decryption file */

  printf("Enter the first file name you want to read from: ");
  scanf("%s", filename1);

  printf("Enter the decryption file name: ");
  scanf("%s", filename2);

  afile = fopen(filename1, "r"); /* Open the first file for reading */
  efile = fopen(filename2, "r"); /* Open the decryption file for reading */

  if (afile == NULL || efile == NULL)
  {
    printf("Error opening the file for reading.\n");
    return (NULL);
  }

  *number = nrecruits(filename1);
  /*set new user defined structure and allocate SIZE bytes of memory*/
  point = (candidate_t *)malloc((*number) * sizeof(candidate_t));
  id = (int *)malloc((*number) * sizeof(int));
  codes = (char *)malloc((*number) * sizeof(char));
  i = 0;
  /*read data from afile and store it in arrays 'id' and 'codes'*/
  while (fscanf(afile, "%d %s %s %s %d %d", &point[i].identification, point[i].last_name, point[i].first_name,
                point[i].date, &point[i].ranking1, &point[i].ranking2) != EOF)
  {
    i++;
  }

  fclose(afile);
  j = 0;
  /* Read data from 'efile' and store it in arrays 'id' and 'codes' */
  while (fscanf(efile, " %d %c ", &id[j], &codes[j]) != EOF)
  {
    j++;
  }
  fclose(efile);

  /*decrypt file to find decrypted names*/
  for (i = 0; i < (*number); i++)
  { // Loop through each element in the 'point' array
    for (j = 0; j < (*number); j++)
    {
      if (point[i].identification == id[j])
      { // If 'identification' matches an element in 'id'
        point[i].code = codes[j];
        for (k = 0; k < strlen(point[i].first_name); k++)
        {
          if (point[i].first_name[k] >= 'a' && point[i].first_name[k] <= 'z')
          {
            point[i].first_name[k] = 'z' - (((point[i].code) - (point[i].first_name[k] - 'a')) % 26);
          }
        }
        for (k = 0; k < strlen(point[i].last_name); k++)
        { // Loop through each character in 'last_name'
          if (point[i].last_name[k] >= 'a' && point[i].last_name[k] <= 'z')
          { // If the character is a lowercase letter
            point[i].last_name[k] = 'z' - (((point[i].code) - (point[i].last_name[k] - 'a')) % 26);
          }
        }
      }
    }
  }
  printf("Database has successfully loaded!\n");
  free(id);
  free(codes);
  return (point);
}

/*display database for user*/
void display_database(candidate_t *dbase, int count_1)
{
  int i;
  // Loop through the 'candidate_t' array and display the information for each candidate.
  printf("Candidate Information\n");
  for (i = 0; i < count_1; i++)
  {
    printf("ID:     %09d\n", dbase[i].identification);
    printf("Name:   %s %s\n", dbase[i].first_name, dbase[i].last_name);
    printf("Date:   %s\n", dbase[i].date);
    printf("Rank 1: %d\n", dbase[i].ranking1);
    printf("Rank 2: %d\n", dbase[i].ranking2);
    printf("\n"); // Add a newline to separate information for different candidates.
  }
}

// Function for sorting
void sort_database(candidate_t *dbase, int count_1)
{
  int choose;
  // Display a menu for sorting options
  printf("[1] Identification number\n");
  printf("[2] First Name\n");
  printf("[3] Last Name\n");
  printf("[4] Date\n");
  printf("[5] Rank 1\n");
  printf("[6] Rank 2\n");
  printf("[7] Exit\n");
  printf("Enter your choice: ");
  scanf("%d", &choose);

  switch (choose)
  {
  case 1:
    sort_identification(dbase, count_1);
    break;
  case 2:
    sort_firstname(dbase, count_1);
    break;
  case 3:
    sort_lastname(dbase, count_1);
    break;
  case 4:
    sort_date(dbase, count_1);
    break;
  case 5:
    sort_ranking1(dbase, count_1);
    break;
  case 6:
    sort_ranking2(dbase, count_1);
    break;
  default:
    printf("Invalid choice. Please try again.\n");
  } // Return without calling 'display_database' if the choice is invalid.
  // Regardless of the sorting choice, display the sorted database
  display_database(dbase, count_1);
}

// Searching for a candidate function
void search_candidate(candidate_t *dbase, int count_1)
{
  char candidfirstname[50];
  char candidlastname[50];
  int found = 0; // Initialize a flag to indicate whether the candidate is found.
  int i;
  printf("Enter candidates first and last name in all lowercase: ");
  scanf("%s %s", candidfirstname, candidlastname);

  // Loop through the candidate database to search for the candidate by name.
  for (i = 0; i < count_1; i++)
  {
    if (strcmp(candidfirstname, dbase[i].first_name) == 0 && strcmp(candidlastname, dbase[i].last_name) == 0)
    {
      // Check if the candidate's name matches the first name and last name.

      // Display candidate information if found.
      printf("Candidate Information\n");
      printf("ID:     %09d\n", dbase[i].identification);
      printf("Name:   %s %s\n", dbase[i].first_name, dbase[i].last_name);
      printf("Date:   %s\n", dbase[i].date);
      printf("Rank 1: %d\n", dbase[i].ranking1);
      printf("Rank 2: %d\n", dbase[i].ranking2);

      found = 1; // Set the flag to indicate that the candidate was found.
      break;     // Exit the loop since the candidate is found.
    }

    if (!found)
    {
      printf("Candidate not found.\n");
    }
  }
}

void remove_candidate(candidate_t *dbase, int *count_1)
{
  char firstname[30];
  char lastname[30];
  int i, j; // New index to keep track of the candidates in the new database

  printf("Enter the first and last name of the candidate you want to remove: ");
  scanf("%s %s", firstname, lastname);

  // Initialize the new database index
  j = 0;

  // Search for the candidate with the specified first name and last name
  for (i = 0; i < *count_1; i++)
  {
    if (strcmp(firstname, dbase[i].first_name) == 0 && strcmp(lastname, dbase[i].last_name) == 0)
    {
      for (j = i; j < (*count_1) - 1; j++)
      {
        dbase[j] = dbase[j + 1];
      }
      (*count_1)--;
      printf("Candidate %s %s has been removed successfully! \n", firstname, lastname);
      return;
    }
  }
}

candidate_t *add_candidate(candidate_t *dbase, int *count_1)
{
  char junk;
  int i;
  candidate_t candid;
  candidate_t *newDatabase;

  // Prompt the user to enter new candidate's information
  printf("Enter the new candidate's information:\n");
  printf("Enter 9 digit ID: ");
  scanf("%d", &candid.identification);
  printf("First Name: ");
  scanf("%s", candid.first_name);
  printf("Last Name: ");
  scanf("%s", candid.last_name);
  printf("Date: ");
  scanf("%s", candid.date);
  printf("Rank 1: ");
  scanf("%d", &candid.ranking1);
  printf("Rank 2: ");
  scanf("%d", &candid.ranking2);
  printf("Encryption Code: ");
  scanf("%c%c", &junk, &candid.code); /*junk variable for buffer*/
  // Increment the count of candidates
  (*count_1)++;

  // allocate memory for the new database
  newDatabase = (candidate_t *)malloc(sizeof(candidate_t) * (*count_1));

  if (newDatabase != NULL)
  {
    // Copy data from the old database to the new one
    for (i = 0; i < (*count_1 - 1); i++)
    {
      newDatabase[i] = dbase[i];
    }
    // Add the new candidate to the end of the database
    newDatabase[*count_1 - 1] = candid;
  }
  else
  {
    printf("Memory allocation failed. Candidate not added.\n");
    (*count_1)--; // Decrement the count to maintain consistency
  }

  // Free the old database
  free(dbase);

  printf("Candidate has been successfully added!\n");

  return (newDatabase); // Return
}

// Implement saving the database function
void save_database(candidate_t *dbase, int count_1, int *error)
{
  int i, k;
  char filename1[50];
  char filename2[50];
  FILE *afile;
  FILE *efile;

  // Prompt the user for file names
  printf("Enter a new file name to save data into: ");
  scanf("%s", filename1);
  printf("Enter a new file name to save encryption into: ");
  scanf("%s", filename2);
  /*open files for writing*/
  efile = fopen(filename1, "w");
  afile = fopen(filename2, "w");
  /*check files and terminate the program if file==NULL which is returned to main*/
  if (afile == NULL || efile == NULL)
  {
    *error = 1;
  }
  // Write data to afile
  for (i = 0; i < count_1; i++)
  {
    fprintf(afile, "%d %c\n", dbase[i].identification, dbase[i].code);
  }
  fclose(afile);

  /*encrypt the files before saving*/
  for (i = 0; i < count_1; i++)
  {
    for (k = 0; k < strlen(dbase[i].first_name); k++)
    {
      if (dbase[i].first_name[k] >= 'a' && dbase[i].first_name[k] <= 'z')
      {
        dbase[i].first_name[k] = 'a' + (((dbase[i].code) - ('z' - (dbase[i].first_name[k]))) % 26);
      }
    }
    for (k = 0; k < strlen(dbase[i].last_name); k++)
    { // Loop through each character in 'last_name'
      if (dbase[i].last_name[k] >= 'a' && dbase[i].last_name[k] <= 'z')
      { // If the character is a lowercase letter
        dbase[i].last_name[k] = 'a' + (((dbase[i].code) - ('z' - (dbase[i].last_name[k]))) % 26);
      }
    }
  }
  for (i = 0; i < count_1; i++)
  { /*print into new file*/
    fprintf(efile, "%09d %s %s %s %d %d\n",
            dbase[i].identification,
            dbase[i].last_name,
            dbase[i].first_name,
            dbase[i].date,
            dbase[i].ranking1,
            dbase[i].ranking2);
  }

  printf("Database has been successfully saved!\n");
  /*close efile*/
  fclose(efile);
}

/*function to sort by ID*/
void sort_identification(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    {
      if (dbase[j].identification > dbase[j + 1].identification)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}
/*function to sort by first name*/
void sort_firstname(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    { /*compare s1 and s2*/
      if (strcmp(dbase[j].first_name, dbase[j + 1].first_name) > 0)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}
/*function to sort by last name*/
void sort_lastname(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    { /*compare s1 and s2*/
      if (strcmp(dbase[j].last_name, dbase[j + 1].last_name) > 0)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}
/*function to sort by date*/
void sort_date(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    { /*compare s1 and s2*/
      if (strcmp(dbase[j].date, dbase[j + 1].date) > 0)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}
/*function to sort by ranking 1*/
void sort_ranking1(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    {
      if (dbase[j].ranking1 > dbase[j + 1].ranking1)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}
/*function to sort by ranking 2*/
void sort_ranking2(candidate_t *dbase, int count_1)
{
  int i, j;
  candidate_t temp;
  for (i = 0; i < count_1 - 1; i++)
  {
    for (j = 0; j < count_1 - 1; j++)
    {
      if (dbase[j].ranking2 > dbase[j + 1].ranking2)
      {
        // swap
        /*use temp variable to store database[j+1]*/
        temp = dbase[j + 1];
        dbase[j + 1] = dbase[j];
        dbase[j] = temp;
      }
    }
  }
}