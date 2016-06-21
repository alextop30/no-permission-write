#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int opt; 															//var to hold the result of get opt
	bool cflagused=false; 												//evaluate if there is a c opt used
	int fd;																//declare file decriptor to hold open
	int perm;															//variable to check if permissions exist

	while((opt = getopt(argc, argv, "c")) != -1) 						//while there are arguments
	{ 																	//get opt to check for a -c command line opt

		switch(opt) 													//check the opt
		{
			case 'c':													//if option is -c 
			{
				cflagused=true; 										//set the flags variable to true
				break;													//break out of the case
			}
			default:													//default case -- invalid option typed
			{
				cout << "The only valid options is -c" << endl;			//error message dispalyed on default
				break;
			}
		}
	}
	
																									
	if (argc == 1)														// check for command line args
	{
		cout << "Usage: [-c] out_file message _string\n";
		cout << "where the message_string is appended to file ";		//if no command line args exist display message
		cout << "out_file.\nThe -c option clears the file before the "
			 <<"message is appended!\n";	 
		exit(EXIT_FAILURE);												//exit fail no command line args
	}
	
	
	int rs;																//holds output of stat
	
	struct stat buffer;													//struct to hold stat
	
	if(cflagused)														//if -c option is eneted
	{
		rs = stat(argv[2], &buffer);									//run stat with 3rd command line argument
		
		if(rs == -1)													//if rs is not available no file exists
		{
			fd = open(argv[2], O_WRONLY | O_CREAT,0000);				//create a file without permissions
	
		}
		rs = stat(argv[2], &buffer);									//actually run buffer
		
	}
	else
	{																	//else if no -c is entered test 2nd argument
		rs = stat(argv[1], &buffer);
		
		if(rs == -1)													//if rs2 is = to -1 create file 
		{
			fd = open(argv[1], O_WRONLY | O_CREAT,0000);				//file has no permissions
		
		}
		rs = stat(argv[1], &buffer);									//actually run buffer
		
	}
	
	close(fd);															//close the file descriptor

	if(rs == -1) 														//check if stat failed on any occations
	{																	//when it is actually opened after the
		perror("stat failed"); 											//file creation
		exit(1);
	}
	
	perm = buffer.st_mode & 0777;									
	if(perm > 0)														//check for any permissions on the file
	{
		cerr << "The File is not safe"<<endl;							//if the file has permissions 
		exit(EXIT_FAILURE);												//exit with fail
	}
	

	int change_perm; 													//change permission var
	
	
	if (cflagused)
	{
		
		change_perm = chmod(argv[2], 0200);								//change permissions so user write permission
		fd = open(argv[2], O_WRONLY | O_TRUNC);							//open the file with truncation
		
		if (fd ==-1)	
		{																//if the file was not opened display error
			cerr << "File could not be opened" << endl;
			exit(EXIT_FAILURE);
		}
	
		if (change_perm == -1)											//if change permission is = -1 it failed
		{
			perror("Can't change permissions");							//display error message
			exit(1);
		}
			
		for(int index = 3; index < argc; index++)						//step through argv - argc times
		{																//writing the contents to the file
			size_t sz = strlen (argv[index]);							//adding spaces between each word and
			write (fd, argv[index], sz + 1);							//new line characters at the end
			write (fd, " ", 1);
		} 	
		
		write (fd, "\n",1);												//write the new line character to the file
	}
	else
	{																	//else do the same but append to the file
		change_perm = chmod(argv[1], 0200);
		fd = open(argv[1], O_WRONLY | O_APPEND);						//open file -- name in argv 1
		if (fd ==-1)													//if the file was not opened display error
		{
			cerr << "File could not be opened" << endl;
			exit(EXIT_FAILURE);
		}
		
		if (change_perm == -1)											//if change permissions failed display error
		{
			perror("Can't change permissions");							//display message if permissions could not be changed
			exit(1);
		}
		for(int index = 2; index < argc; index++)						//step through argv - argc times and print each argument
		{																//starting at the second one inserting a space
			size_t sz = strlen (argv[index]);							//after each argument
			write (fd, argv[index], sz + 1);
			write (fd, " ", 1);
		} 	
		
		write (fd, "\n",1);												//write the new line character at the end
			
	}
	
	close(fd);															//close the file decriptor
	
	if (cflagused)														//if count is bigger than zero option -c is used
	{
		change_perm = chmod(argv[2], 0000);								//wipe permissions of correct element
	}
	else
	{
		change_perm = chmod(argv[1], 0000);								//if no option was used wipe permissions
	}
	
	return 0;															//return 0 out of the program
	
}

