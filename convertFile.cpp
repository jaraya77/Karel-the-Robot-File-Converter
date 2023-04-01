 /***************************************************************************************
 	Johnny Araya																		
 	5/18/2022																			
 	Final Project- Karel and Jarel File Converter 										
 																						
 	This program coverts a Karel world file into a Jarel world file 					
 	and also from Jarel world file into a Karel world file and makes					
 	a new file with '_Converted' at the end of the file. Not only my 					
 	program can covert an individual file but it can convert a whole 					
 	directory of Jarel and Karel files if you prompt it too.							
 																						
 	Mars Hill University Honor Code														
 		We, the students of Mars Hill University, pledge ourselves 						
 		to uphold integrity, honesty, and academic responsibility 						
 		in and out of the classroom.												
 																						
 	Mars Hill University Honor Pledge													
 		On my honor, I have neither given nor received any academic aid 				
 		or information that would violate the Honor Code of Mars Hill University		
 																						
 ****************************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

using namespace std;

void convertFile(unsigned char *file, unsigned char *bytes, int size);
void writingFile(unsigned char *file, char *fileName, int size);
void karelFile(void);
void jarelFile(void);
bool isKarel(unsigned char *file, unsigned char *karel, int size); 
bool isJarel(unsigned char *file, unsigned char *jarel, int size);
void convertProperFile(char *fileName, int size);

unsigned char karel[]={192,117,0,197,79,236,46,82}; //seting the eight binary bits in the Karel array
unsigned char jarel[]={14,50,238,70,114,147,124,248}; //setting the eight binary bits in the Jarel array
unsigned char file[10000]; 

DIR *dp;
struct stat statStruct;
struct  dirent *de;

char fileName[4000];
char errorString[1024];
char pathname[4096];

int main(int argc, char **argv){

    int size = 0;
    int j=0;
    char name[4096];


    int exists;

    exists=lstat(argv[1], &statStruct);//grabs argument's information and stores it in statstruct while saving the status into exists
	strcpy(name, argv[1]); //copies argv[1] into 'name' to dp=opendir(name)

	//if there are not two argumaents, printout error below
    if (argc != 2){ 
        cout << "Error, no files or directories inputed" << endl;
        return 1;
    }

    if(exists<0){ //if exist(argument) is empty, print error below
        perror(errorString);
		cout << ("ERROR HERE\n") << endl;
        exit(1);
    } else {
        if(S_ISDIR(statStruct.st_mode)){    //checks if it is a directory
            dp=opendir(name);  //attempt to open the directory there may be an issue here
            if(!dp){ //if not, then print errors
                perror(errorString);
            }else{ //directory can be opened
                while((de=readdir(dp))){//traverses through the directory file by file until it's own name is returned
                    if(de->d_name[0]!='.'){
						strcpy(fileName, name);//copy the directory name to fullpath as a starting point
 
                         //only add the slash between directories/files if it isn't there already
                         if(fileName[strlen(fileName)-1]!='/')
                             strcat(fileName, "/");//if user calls program with "/" or "/home/" as argument, this will apply
                         strcat(fileName, de->d_name);//finally, add the name of the file/subdirectory to the end of the path
 
                         exists=lstat(pathname, &statStruct);

                        convertProperFile(fileName, size);//Converts file into jarel, karel, or neither

                    }
                }

            }
        }else{
            exists=lstat(argv[1], &statStruct);

            strcpy (fileName, argv[1]);
            
            convertProperFile(fileName, size); //converts file into jarel, karel, or neither
            
        }
    }
}

/************************************************
 Converts a file into jarel, karel, or neither by identifying the file type first and reading file hex
 ***********************************************/
void convertProperFile(char *fileName, int size){
    fstream in(fileName,ios::binary|ios::in);
    size = 0;

    char c;
    while(true) {
        c=in.get();
        if(in.eof()) 
            break;
        file[size++]=c;
    }
    in.close();


    //Attempt to identify file type
    if(isKarel(file, karel, size)){
        karelFile();
        convertFile(file, jarel, size);
        writingFile(file, fileName, size);
    } else if(isJarel(file, jarel, size)){
        jarelFile();
        convertFile(file, karel, size);
        writingFile(file, fileName, size);
    } else{
        cout << "This is neither Jarel nor Karel" << endl;
    }

}


/************************************************
  Checks if file is of karel type
 ***********************************************/
bool isKarel(unsigned char *file, unsigned char *karel, int size){
    int i;
    int j=0;

    //Traverses through bits to identify if it matches to karel bits
    for(int i=(size - 32); i < (size-32+8); i++){
        if(file[i] != karel[j++]){
            return false;
        }
    }
    return true;

}

/************************************************
  Checks if file is of jarel type
 ***********************************************/
bool isJarel(unsigned char *file, unsigned char *jarel, int size){
    int i;
    int j=0;

    //Traverses through bits to identify if it matches to jarel bits
    for(int i=(size - 32); i < (size-32+8); i++){
        if(file[i] != jarel[j++]){
            return false;
        }
    }
    return true;
}


/************************************************
  Identifies if file is of karel type
 ***********************************************/
void karelFile(void){
    cout << "This is a Karel world file" << endl;
}

/************************************************
  Identifies if file is of jarel type
 ***********************************************/
void jarelFile(void){
    cout << "This is a Jarel world file" << endl;
}


/************************************************
  Does the byte value change to karel or jarel
 ***********************************************/
void convertFile(unsigned char *file, unsigned char *bytes, int size){
    int i;
    int j=0;

    for(int i=(size - 32); i < (size-32+8); i++){//Traverses through a starting point of bits and changes their value
        file[i] = bytes[j++];


    }
}


/************************************************
 Writes to a new file with the original file name followed by "_Converted" 
 ***********************************************/
void writingFile(unsigned char *file, char *fileName, int size){
    strcat (fileName, "_Converted"); //Concatenates "_Converted" to the end of the copied file name

    fstream out(fileName,ios::binary|ios::out); //writes

    for(int i = 0; i < size; i++) {

        out.put(file[i]);
    }
    out.close();

}
