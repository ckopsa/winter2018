/***********************************************************************
* Program:
*    Lab 06, Vulnerabilities and Exploits
*    Brother Helfrich, CS470
* Author:
*    Colton Kopsa
* Summary:
*    This program is designed to demonstrate memory injection vulnerabilities.
*
************************************************************************/

#include <iostream>
#include <cassert>
using namespace std;

void stackExploit();
void heapExploit();
void arrayExploit();
void intExploit();
void arcExploit();
void subterfugeExploit();
void vtableExploit();
void ansiExploit();

/******************************************************
 * MAIN
 * This function take you to the various exploits
 *****************************************************/
int main()
{
   // prompt
   cout << "  1. Stack smashing\n"
        << "  2. Heap spraying\n"
        << "  3. Array index\n"
        << "  4. Integer overflow\n"
        << "  5. ARC injection\n"
        << "  6. Pointer subterfuge\n"
        << "  7. VTable spraying\n"
        << "  8. ANSI-Unicode conversion\n"
        << "> ";
   int select;
   //cin >> select;

   switch (5)
   {
      case 1:
         stackExploit();
         break;
      case 2:
         heapExploit();
         break;
      case 3:
         arrayExploit();
         break;
      case 4:
         intExploit();
         break;
      case 5:
         arcExploit();
         break;
      case 6:
         subterfugeExploit();
         break;
      case 7:
         vtableExploit();
         break;
      case 8:
         ansiExploit();
         break;
      default:
         cout << "Unknown option " << select << endl;
   }

   return 0;
}

/**************************************************************
 **************************************************************
 ********************    STACK       **************************
 **************************************************************
 **************************************************************/

/***********************************************
 * STACK VULNERABILITY
 * 1. There must be a buffer on the stack
 * 2. the buffer must be reachable from an external input
 * 3. The mechanism to fill the buffer must not check the correct buffersize
 **********************************************/
void stackVulnerability(char zipcode[])
{
  char tempZipcode[6];
  strcpy(tempZipcode, zipcode);
}

/*********************************************
 * STACK EXPLOIT
 * 1. The attacker must provide more data into the
 *    outwardly facing buffer than the buffer is designed to hold
 * 2. The attacker must know where the the stack pointer resides
 *    on the stack. This should be just beyond the end of the buffer
 * 3. The attacker must insert machine language instructions in the buffer.
 *    This may occur before, after, or even around the stack pointer. The
 *    machine language could be already compiled code in the program
 * 4. The attacker must overwrite the stack pointer. The old value,
 *    directing the flow of the program after the function is returned,
 *    must be changed from the calling function to the provided
 *    machine language in step 3.
 *********************************************/
void stackExploit()
{
  char notZipcode[60] = "This is definitely not a zipcode";
  stackVulnerability(notZipcode);
}

/**************************************************************
 **************************************************************
 ********************      HEAP      **************************
 **************************************************************
 **************************************************************/


/*************************************
 * HEAP VULNERABILITY
 * 1. There must be two adjacent heap buffers.
 * 2. The first buffer must be reachable through external input.
 * 3. The mechanism to fill the buffer from the external input must
 *    not correctly check for the buffer size.
 * 4. The second buffer must be released before the first.
 *************************************/
void heapVulnerability(char userPin[])
{
  char * pin = new char[4];
  char * authKey = new char[4];

  assert(pin < authKey);
  strcpy(pin, userPin);

  cout << "Authenticating with key:" << authKey << endl;

  delete [] authKey;
  delete [] pin;
}

/*************************************
 * HEAP EXPLOIT
 * 1. The attacker must provide more data into the outwardly facing
 *    heap buffer than the buffer is designed to hold
 * 2. The attacker must know the layout of the Memory Control Block (MCB)
 *    (essentially a linked list) residing just after the buffer
 * 3. The attacker must provide a new MCB containing both the location
 *    of the memory overwrite and the new data to be overwritten
 ***********************************/
void heapExploit()
{
  char maliciousPin[50] = "1W1LLH4CKY0U!MU@H@H@H@H@H@H@H@H!";
  heapVulnerability(maliciousPin);
}

/**************************************************************
 **************************************************************
 ********************     ARRAY      **************************
 **************************************************************
 **************************************************************/

/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ************************************/
void arrayVulnerability(int index)
{
  int userLoggedIn[6];
  bool isAdmin = false;

  userLoggedIn[index] = 1;

  if (index == 0)
    isAdmin = true;
  if (isAdmin)
    cout << "Welcome Admin! You have superuser privileges." << endl;
  else
    cout << "Welcome User! You have no superuser privileges." << endl;
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
  int badIndex = 6;
  arrayVulnerability(badIndex);
}


/**************************************************************
 **************************************************************
 *******************    INTEGER      **************************
 **************************************************************
 **************************************************************/

/********************************************
 * INTEGER VULNERABILITY
 * 1. There must be a security check represented by an expression.
 * 2. The expression must have the potential for overflow.
 * 3. At least one of the numbers used to compute the sentinel
 *    must be reachable through external input.
 *********************************************/
void intVulnerability(int userID, char username[])
{
  const int MAX_USERS = 500;
  const int MAX_USERNAME_LENGTH = 100;
  char usernames[MAX_USERS][MAX_USERNAME_LENGTH];
  void * sentinel = &usernames + MAX_USERS * MAX_USERNAME_LENGTH;

  if (userID * MAX_USERNAME_LENGTH + usernames < sentinel)
    strcpy(usernames[userID], username);
}

/**********************************************
 * INTEGER EXPLOIT
 * 1. Provide input, either a buffer size or a single value,
 *    that is directly or indirectly used in the vulnerable expression
 * 2. The input must exceed the valid bounds of the data-type,
 *    resulting in an overflow or underflow condition
 **********************************************/
void intExploit()
{
  char maliciousUsername[5] = "rmrf";
  int maliciousId = 3000000000; // -1294967296
  intVulnerability(maliciousId, maliciousUsername);
}

/**************************************************************
 **************************************************************
 ********************      ARC       **************************
 **************************************************************
 **************************************************************/

/****
 * Simple Function to point to
 */
void authenticate()
{
  cout << "You are authenticated." << endl;
}

/*********************************
 * ARC VULNERABILITY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input
 *    to overwrite the function pointer. This typically happens through
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must
 *    be dereferenced
 ********************************/
void arcVulnerability(char userInput[])
{
  char username[10];
  void (* pAuthenticate)() = authenticate;

  strcpy(username, userInput);

  pAuthenticate();

}

/*********************************
 * ARC EXPLOIT
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the function pointer
 * 2. The attacker must have the address to another function
 *    which is to be used to replace the existing function pointer
  ********************************/
void arcExploit()
{
  char maliciousUsername[20] = "HA GIVE ME ROOT!";
}

/**************************************************************
 **************************************************************
 *******************    SUBTERFUGE    *************************
 **************************************************************
 **************************************************************/

/****************************************
 * Pointer Subterfuge Vulnerability
 * 1. There must be a pointer used in the code.
 * 2. There must be a way to overwrite the pointer
 * 3. After the pointer is dereferenced, the pointer is dereferenced
 ****************************************/
void subterfugeVulnerability(long * array, int size)
{
   long int buffer[2];
   const char * message = "Safe";  // 1. Pointer

   for (int i = 0; i < size; i++)  // 2. Overwrite
      buffer[i] = array[i];

   cout << "Message is: \""
        << message                 // 3. Dereference
        << "\".\n";
}

/****************************************
 * Pointer Subterfuge Exploitation
 * 1. The attacker must exploit a vulnerability allowing
 *    unintended access to the pointer
 * 2. The attacker must be able to provide a new pointer
 *    referring to data altering the normal flow of the program
 ****************************************/
void subterfugeExploit()
{
   // an attacker's array
   long int array[3] =             // 1. Exploit
   {
      1,
      1,
      (long int)"Dangerous"        // 2. Provide pointer
   };

   // exploit it
   subterfugeVulnerability(array, 3);
}

/**************************************************************
 **************************************************************
 ********************     VTABLE     **************************
 **************************************************************
 **************************************************************/

/***********************************
 * VULNERABILITY
 * 1. The vulnerable class must be polymorphic.
 * 2. The class must have a buffer as a member variable.
 * 3. Through some vulnerability, there must be a way for user input
 *    to overwrite parts of the VTable.
 * 4. After a virtual function pointer is overwritten,
 *    the virtual function must be called.
 **********************************/
class Vulnerability
{
};

/************************************
 * VTABLE EXPLOIT
 * 1. Through some vulnerability, the VTable pointer
 *     or a function pointer within the VTable must be overwritten
 * 2. The attacker must have the address to another VTable pointer
 *     or a function pointer
 ***********************************/
void vtableExploit()
{
}


/**************************************************************
 **************************************************************
 ******************    ANSI-Unicode    ************************
 **************************************************************
 **************************************************************/

/*********************************************************
 * ANSI - UNICODE VULNERABILITY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer
 *    rather than the number of elements in the buffer.
 ********************************************************/
void ansiVulnerability(short sourceData[])
{
  short backupData[6];
  int numElements = sizeof(backupData);
  cout << numElements << " total elements." << endl;
  for (int i = 0; i < numElements; i++)
    backupData[i] = sourceData[i];
  for (int i = 0; i < numElements; i++)
    cout << sourceData[i] << " == " << backupData[i] << endl;
}

/***********************************************
 * ASCI - UNIODE EXPLOIT
 * 1. The attacker must provide more than half as much data
 *    into the outwardly facing buffer as it is designed to hold
 **********************************************/
void ansiExploit()
{
  //                                        G  O  O  G  L  E
  short sourceData[12] = {0, 7, 3, 3, 4, 5, 6, 0, 0, 6, 1, 3 }; // Unicode Text :)
  ansiVulnerability(sourceData);
}
