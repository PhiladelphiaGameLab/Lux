#include "CGI.h"

//--------------------------------------------------------------
//  THIS FUNCTION NEEDS TO BE DONE.
//--------------------------------------------------------------
std::string CGI::decode_string(std::string str){
   const char *digits = "0123456789ABCDEF";
   int length = str.length();

   int outpos = 0;
   int pos;
   for (pos=0; pos<length; pos++){
      // Handle white space
      if (str[pos] == '+'){
         str[outpos++] = ' ';

      // Handle hex characters
      }else if (str[pos] == '%'){
         char ch1 = toupper(str[++pos]);
         char ch2 = toupper(str[++pos]);
         str[outpos++] = 16*(strchr(digits, ch1)-strchr(digits,'0'))
                       + strchr(digits, ch2)-strchr(digits,'0');
      }

       //Handle remaining characters
      else
         str[outpos++] = str[pos];
   }
   return str;
}

//--------------------------------------------------------------
//  THIS FUNCTION NEEDS TO BE DONE.
//--------------------------------------------------------------
CGI::CGI(){
   // Initialize private variables
   ArgCnt = 0;
   for (int pos = 0; pos < MAX_ARGS; pos++){
        Name[pos] = Value[pos] = "";
   }
   // Read environment variables
   std::string request_method = CGI::getEnvStr("REQUEST_METHOD");
   std::string query_string = CGI::getEnvStr("QUERY_STRING");
   std::string content_length = CGI::getEnvStr("CONTENT_LENGTH");

   int query_length = 0;

   // Check request_method variable
   if (request_method == "")
      return;

   // Handle GET requests
   if (request_method.compare("GET") == 0){
      if (query_string == "")
         return;
      if (content_length == "")
         query_length = query_string.length();
      else
         query_length = atoi(content_length.c_str());
   }



   // Handle POST requests
   if (request_method.compare("POST") == 0){
      if (content_length == "")
         return;
      else
         query_length = atoi(content_length.c_str());
      query_string = (char *)malloc(query_length);
      if (query_string == "")
         return;
      for (int pos = 0; pos < query_length; pos++){
        query_string[pos] = fgetc(stdin);
      }
   }

//      ?test=fun2&name=jake2&team=true2&EUID=123432


   // Separate query_string into arguments
   int start_name, end_name, start_value, end_value = 0;
   while (end_value < query_length){
      // Find argument name

      start_name = end_name = end_value + 1;
      while ((end_name<query_length) && query_string[end_name] != '='){
         end_name++;
      }

      // Copy and decode name string
      std::string xd = query_string.substr(start_name, end_name);
      Name[ArgCnt] = decode_string(xd);

      // Find argument value
      start_value = end_value = end_name + 1;
      while ((end_value<query_length) && query_string[end_value] != '&'){
         end_value++;
      }

      // Copy and decode value string
      std::string md = query_string.substr(start_value, end_value);
      Value[ArgCnt] = decode_string(md);
      ArgCnt++;
   }

}

// done
std::string CGI::get(std::string name){
   // Lookup argument by name
   int i;
   for (i=0; i<ArgCnt; i++){
      std::cout << "Is this " << name << "? " << Name[i] << " : " << Value[i] << std::endl;
      if (name.compare(Name[i]) == 0){
        return Value[i];
      }
    }
   return "";
}

// done
std::string CGI::GetName(int index){
   // Lookup argument by location
   if ((index >= 0) && (index < ArgCnt)){
      return Name[index];
   }
   return "";
}

// done
std::string CGI::GetValue(int index){
   // Lookup argument by location
   if ((index >= 0) && (index < ArgCnt)){
      return Value[index];
   }
   return "";
}

// done
int CGI::GetCnt(){
   return ArgCnt;
}

// done
void CGI::error(std::string message, int code){
    std::cout << "{ \t\"status\": \"error\", \n\t\"message\": \"" << message <<  " \n\t\"code\": " << code << "\n}" << std::endl;
}


// done
std::string CGI::getEnvStr(std::string key){
    char const* val = getenv(key.c_str());
    return val == NULL ? std::string() : std::string(val);
}

