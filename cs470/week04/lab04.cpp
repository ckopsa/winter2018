/***********************************************************************
 * Program:
 *    Lab 04
 *    Brother Helfrich, CS470
 * Author:
 *    Colton Kopsa
 * Summary:
 *    This program sanitizes input to ensure that all whitelisted tags
 *    are displayed properly, and all non-whitelisted tags are made
 *    HTML-friendly
 *
 *    Estimated:  4.0 hrs
 *    Actual:     4.0 hrs
 ************************************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
using namespace std;

class Sanitizer
{
public:
   Sanitizer(string * whitelist, int whitelistLength)
      : whitelist(whitelist), whitelistLength(whitelistLength)
   {}

   /***
    * Sanitizes HTML by exchanging non-whitelisted tags for their HTML-friendly
    * counterparts.
    */
   string sanitize(const string & input)
   {
      tagStack = stack<string>();
      // Iterate through string
      string returnString = "";
      for (int i = 0; i < input.length(); i++)
      {
         // If chevron found extract tag
         if (input[i] == '<')
         {
            // Tag extraction
            string temp = "";
            for (; input[i] != '>' && i < input.length(); i++)
            {
               temp += input[i];
            }
            temp += ">";

            bool isBadNest = false;
            if (isOpeningTag(temp))
               tagStack.push(temp);
            else if (extractTagName(tagStack.top()) == extractTagName(temp))
               tagStack.pop();
            else
               isBadNest = true;

            if (!isTagWhiteListed(temp) || isBadNest)
               returnString += encode(temp);
            else
               returnString += temp;
         }
         else
         {
            returnString += input[i];
         }
      }
      return returnString;
   }

private:

   // PROPERTIES

   string * whitelist;
   int whitelistLength;
   stack<string> tagStack;

   // METHODS

   /***
    * Extracts the tag text from a tag and returns it.
    */

   bool isOpeningTag(const string & input)
   {
      return input[1] != '/';
   }

   string extractTagName(const string & input)
   {
      string returnString = "";
      for (int i = 0; i < input.length(); i++)
      {
         switch (input[i])
         {
            // Ignore tag related characters
         case '<':
         case '>':
         case '/':
            break;
            // Ignore everything after the tag text
         case ' ':
            return returnString;
            break;
         default:
            returnString += input[i];
         }
      }
      return returnString;
   }

   /***
    * Determines if tag is whitelisted
    */
   bool isTagWhiteListed(const string & input)
   {
      // Is our tag white listed?
      for (int i = 0; i < whitelistLength; i++)
      {
         if (extractTagName(input) == whitelist[i])
            return true;
      }

      // No whitelisted tag
      return false;
   }

   /***
    * Encodes HTML into safe ready-to-display text
    */
   string encode(const string & input)
   {
      string temp = "";
      // Change out '<' or '>' for their HTML-friendly counterparts
      for(int i = 0; i < input.length(); i++)
      {
         switch(input[i])
         {
         case '<':
            temp += "&lt;";
            break;
         case '>':
            temp += "&gt;";
            break;
         default:
            temp += input[i];
            break;
         }
      }
      return temp;
   }
};

/***
 * Gets input from user, sanitizes it, and displays it.
 */
int main()
{
   string whitelist[] =
      {
         "a",
         "abbr",
         "acronym",
         "b",
         "blockquote",
         "cite",
         "code",
         "del",
         "em",
         "i",
         "q",
         "strike",
         "strong"
      };
   Sanitizer sanitizer(whitelist, 13);
   string input;
   do
   {
      cout << "> ";
      getline(cin, input);
      cout << "\t" << sanitizer.sanitize(input) << endl;
   }
   while(input != "quit");
}
