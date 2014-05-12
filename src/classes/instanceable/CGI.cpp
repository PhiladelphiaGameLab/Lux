#include "CGI.h"

CGI::CGI(){
    cout << "Content-Type: text/html\n\n" << endl;
    get = getEnvStr("QUERY_STRING");
    // handle POST and PUT as well

    if(getEnvStr("REQUEST_METHOD").compare("GET") == 0){

    }else if(getEnvStr("REQUEST_METHOD").compare("POST") == 0){

    }else if(getEnvStr("REQUEST_METHOD").compare("PUT") == 0){

    }
}

static std::string CGI::getEnvStr(std::string const& key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

std::string CGI::get(std::string parameter, size_t size){

    // Sanatize input

    parameter += "=";
    std::size_t start = get.find(parameter);
    std::size_t end = get.find("&", start);
    std::string found;
    if (start != std::string::npos){
        if(end != std::string::npos){
            found = get.substr(start,end-start);
        }else{
            found = get.substr(start,strlen(get)-start);
        }
    }
    return found;
}

/*

//--------------------------------------------------------------
// Purpose: Utilities for reading CGI arguments
// Author:  John Gauch
// Date:    Spring 2002
//--------------------------------------------------------------

#include "cgi.h"
using namespace std;

//--------------------------------------------------------------
// Function for copying a substring.
//--------------------------------------------------------------
char *copy_string(char *str, int start, int end)
{
   // Create string
   char *copy = (char *)malloc(end-start+1);
   if (copy == NULL)
      return NULL;

   // Copy substring
   int pos;
   for (pos = start; pos < end; pos++)
      copy[pos-start] = str[pos];
   copy[pos-start] = '\0';
   return copy;
}

//--------------------------------------------------------------
// Function for decoding CGI strings.
//--------------------------------------------------------------
void decode_string(char *str)
{
   const char *digits = "0123456789ABCDEF";
   int length = strlen(str);

   int outpos = 0;
   for (int pos=0; pos<length; pos++)
   {
      // Handle white space
      if (str[pos] == '+')
         str[outpos++] = ' ';

      // Handle hex characters
      else if (str[pos] == '%')
      {
         char ch1 = toupper(str[++pos]);
         char ch2 = toupper(str[++pos]);
         str[outpos++] = 16*(strchr(digits, ch1)-strchr(digits,'0'))
                       + strchr(digits, ch2)-strchr(digits,'0');
      }

      // Handle remaining characters
      else
         str[outpos++] = str[pos];
   }

   // Mark end of string
   str[outpos] = '\0';
}

//--------------------------------------------------------------
// Constructor function reads and parses CGI arguments.
//--------------------------------------------------------------
Cgi::Cgi()
{
   // Initialize private variables
   ArgCnt = 0;
   for (int pos = 0; pos < MAX_ARGS; pos++)
      Name[pos] = Value[pos] = NULL;

   // Read environment variables
   char *request_method = getenv("REQUEST_METHOD");
   char *query_string = getenv("QUERY_STRING");
   char *content_length = getenv("CONTENT_LENGTH");
   // printf("request_method=%s\n", request_method);
   // printf("query_string=%s\n", query_string);
   // printf("content_length=%s\n", content_length);
   int query_length = 0;

   // Check request_method variable
   if (request_method == NULL)
      return;

   // Handle GET requests
   if (strcmp(request_method, "GET") == 0)
   {
      if (query_string == NULL)
         return;
      if (content_length == NULL)
         query_length = strlen(query_string);
      else
         query_length = atoi(content_length);
   }

   // Handle POST requests
   if (strcmp(request_method, "POST") == 0)
   {
      if (content_length == NULL)
         return;
      else
         query_length = atoi(content_length);
      query_string = (char *)malloc(query_length);
      if (query_string == NULL)
         return;
      for (int pos = 0; pos < query_length; pos++)
	 query_string[pos] = fgetc(stdin);
   }

   // Separate query_string into arguments
   int start_name, end_name, start_value, end_value = -1;
   while (end_value < query_length)
   {
      // Find argument name
      start_name = end_name = end_value + 1;
      while ((end_name<query_length) && (query_string[end_name] != '='))
         end_name++;

      // Copy and decode name string
      Name[ArgCnt] = copy_string(query_string, start_name, end_name);
      decode_string(Name[ArgCnt]);

      // Find argument value
      start_value = end_value = end_name + 1;
      while ((end_value<query_length) && (query_string[end_value] != '&'))
         end_value++;

      // Copy and decode value string
      Value[ArgCnt] = copy_string(query_string, start_value, end_value);
      decode_string(Value[ArgCnt]);
      ArgCnt++;
   }
}


//--------------------------------------------------------------
// Destructor function releases memory.
//--------------------------------------------------------------
Cgi::~Cgi()
{
}

//--------------------------------------------------------------
// Argument lookup by name.
//--------------------------------------------------------------
char *Cgi::GetArg(const char name[])
{
   // Lookup argument by name
   for (int arg=0; arg<ArgCnt; arg++)
      if (strcmp(name, Name[arg]) == 0)
         return Value[arg];

   // Return NULL if not found
   return NULL;
}

//--------------------------------------------------------------
// Argument name lookup by number.
//--------------------------------------------------------------
char *Cgi::GetName(int index)
{
   // Lookup argument by location
   if ((index >= 0) && (index < ArgCnt))
      return Name[index];
   else
      return NULL;
}

//--------------------------------------------------------------
// Argument value lookup by number.
//--------------------------------------------------------------
char *Cgi::GetValue(int index)
{
   // Lookup argument by location
   if ((index >= 0) && (index < ArgCnt))
      return Value[index];
   else
      return NULL;
}

//--------------------------------------------------------------
// Return number of CGI arguments.
//--------------------------------------------------------------
int Cgi::GetCnt()
{
   return ArgCnt;
}

*/


