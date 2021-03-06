#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);
void freeArrayOfCharArrays(char **array, size_t array_length);

int main (int argc, char **argv)
{
    // Get list of paths to binary executables
    std::vector<std::string> os_path_list;
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);

    
    /************************************************************************************
     *   Example code - remove in actual program                                        *
     ************************************************************************************/
    // Shows how to loop over the directories in the PATH environment variable
    /*
    int i;
    for (i = 0; i < os_path_list.size(); i++)
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i].c_str());
    }
    */
    /************************************************************************************
     *   End example code                                                               *
     ************************************************************************************/


    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");

    std::vector<std::string> command_list; // to store command user types in, split into its variour parameters
    char **command_list_exec; // command_list converted to an array of character arrays
    std::vector<std::string> history;
    char **history_exec;
    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)


    /************************************************************************************
     *   Example code - remove in actual program                                        *
     ************************************************************************************/
    // Shows how to split a command and prepare for the execv() function
    /*
    std::string example_command = "ls -lh";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n");

    // Second example command - reuse the `command_list` and `command_list_exec` variables
    example_command = "echo \"Hello world\" I am alive!";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n"); */
    /************************************************************************************
     *   End example code                                                               *
     ************************************************************************************/

    FILE *file;
    if (file = fopen("history.txt", "r")) 
    {
        //printf("file exists");
        char hold[128];
        while(fgets(hold, 128, file) != NULL) {
            //std::cout << hold;
            strtok(hold, "\n");
            history.push_back(hold);
            
        }

        fclose(file);
    }
    
    bool keepGoing = true;
    int j = history.size();
    while(keepGoing)
    {
        char *character[128];
        char **char_input;

        std::cout << "osshell> ";
        std::cin.getline(*character, 128);
        std::string input = *character;
        history.push_back(*character);
        vectorOfStringsToArrayOfCharArrays(history, &history_exec);
        //std::cout << history.back();
        splitString(input, ' ', command_list);
        vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);

        if(command_list.size() == 0)
        {
            history.pop_back();
            vectorOfStringsToArrayOfCharArrays(history, &history_exec);
            j--;
        }

        else if(strcmp(command_list_exec[0], "exit") == 0)
        {
            
            vectorOfStringsToArrayOfCharArrays(history, &history_exec);
            //std::cout << command_list_exec[0];
            FILE *file;
            file = fopen("history.txt", "w");
            
            for(int i = 0; i < history.size(); i++) {
                fputs(history_exec[i], file);
                fputs("\n", file);
            }
            
            fclose(file);
            break;
        } else if(strcmp(command_list_exec[0], "history") == 0)
        {
            if(command_list_exec[1] == NULL) {
                if(history.size() != 0) {
                    for(int i = 0; i < history.size() - 1; i++) 
                    {
                        if(i+1 > 9) {
                            printf(" %d: %s\n", i + 1, history_exec[i]);
                        } else {
                            printf("  %d: %s\n", i + 1, history_exec[i]);
                        }
                    }
                }
                
            } else if(strcmp(command_list_exec[1], "clear") == 0)
            {
                history.clear();
                vectorOfStringsToArrayOfCharArrays(history, &history_exec);
                j = -1;
            } else 
            {
                int isDigit = 1;
                for(int i = 0; i < command_list.at(1).size(); i++)
                {
                    if(!isdigit(command_list.at(1).at(i)))
                    {
                        isDigit = 0;
                    }
                }
                if(atoi(command_list_exec[1]) < 0)
                {
                    isDigit = 0;
                }
                if(isDigit == 1)
                {
                    if(history.size() != 0) {
                        if(atoi(command_list_exec[1]) > history.size()) 
                        {
                            for(int i = 0; i < history.size() - 1; i++) 
                            {
                                if(i+1 > 9) {
                                    printf(" %d: %s\n", i + 1, history_exec[i]);
                                } else {
                                    printf("  %d: %s\n", i + 1, history_exec[i]);
                                }
                            }
                        } else 
                        {
                            for(int i = history.size() - 1 - atoi(command_list_exec[1]); i < history.size() - 1; i++) 
                            {
                                if(i+1 > 9) {
                                    printf(" %d: %s\n", i + 1, history_exec[i]);
                                } else {
                                    printf("  %d: %s\n", i + 1, history_exec[i]);
                                }
                                
                            }
                        }
                    } 

                } else {
                    
                    printf("Error: history expects an integer > 0 (or 'clear')\n");
                }
            }
        } else if(command_list_exec[0][0] == '.' || command_list_exec[0][0] == '/') {
            //beginning of fork
                pid_t pid;
                int status;
                
                pid = fork();
                
                //child
                if(pid == 0) {
                    //std::cout << "executing\n" << history_exec[j] << j << "\n";
                    system(command_list_exec[0]);
                    
                } else {//parent
                    
                    waitpid(pid, &status, 0);
                    exit(0);
                }
        }
        else{
            

            FILE *file;
            bool fileFound = false;
            for (int i = 0; i < os_path_list.size(); i++)
            {
                /* try to open file to read */
                std::vector<std::string> directory;
                std::string directory_s = os_path_list.at(i);
                char **directory_exec;
                
                directory_s.push_back('/');
                directory_s += command_list.at(0);

                directory.push_back(directory_s);
                vectorOfStringsToArrayOfCharArrays(directory, &directory_exec);
                
                
                if (file = fopen(directory_exec[0], "r")) 
                {
                //printf("file exists");
                fileFound = true;

                fclose(file);
                }
            }
            if(fileFound == false) {
                std::cout << history.back() << ": Error command not found\n";
            } else {
                //beginning of fork
                pid_t pid;
                int status;
                
                pid = fork();
                
                //child
                if(pid == 0) {
                    //std::cout << "executing\n" << history_exec[j] << j << "\n";
                    system(history_exec[j]);
                    
                } else {//parent
                    
                    waitpid(pid, &status, 0);
                    exit(0);
                }
                
                
                //end of forked
            }
            
            
            
            
            
            
        }
        j++;
    }
    return 0;
}

/*
   text: string to split
   d: character delimiter to split `text` on
   result: vector of strings - result will be stored here
*/
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}

/*
   list: vector of strings to convert to an array of character arrays
   result: pointer to an array of character arrays when the vector of strings is copied to
*/
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result)
{
    int i;
    int result_length = list.size() + 1;
    *result = new char*[result_length];
    for (i = 0; i < list.size(); i++)
    {
        (*result)[i] = new char[list[i].length() + 1];
        strcpy((*result)[i], list[i].c_str());
    }
    (*result)[list.size()] = NULL;
}

/*
   array: list of strings (array of character arrays) to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        if (array[i] != NULL)
        {
            delete[] array[i];
        }
    }
    delete[] array;
}
