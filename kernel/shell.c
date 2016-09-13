#include <kernel.h>

static WINDOW shell_wnd = {0, 9, 61, 16, 0, 0, 0xDC};
static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};

//Buffer to store commands
char cmdBuffer[61];

//keeping track of current command
int i=0;


 
 //  Function to print help menu
  
void Help(){
 	wprintf(&shell_wnd," The following are legal commands in this shell: \n");
 	wprintf(&shell_wnd," help                  - for displaying help information\n");
 	wprintf(&shell_wnd," wait 'time'           - Make process sleep for given time\n");
 	wprintf(&shell_wnd," ps                    - list all processes\n");
 	wprintf(&shell_wnd," clr                   - refresh window screen\n");
 	wprintf(&shell_wnd," echo                  - similar to posix echo command\n");
 	wprintf(&shell_wnd," train help            - View the train commands\n");
}


// Function to make process sleep for entered duuration
 
void Sleep(){
	char timePtr[10];
	ignoreWhiteSpaces(timePtr,5);
	char* duration = timePtr;
	int error=0;
	//To convert it into int.
	int num=0;
	while(*duration != '\0'){
		if (*duration >= '0' && *duration <= '9'){
			num = num * 10 + *duration - '0';
		}else{
			wprintf(&shell_wnd,"Please enter a valid Number! \n");
			error=1;
			num=1; 
			break;
		}
		duration++;
	}
	if(!error){
		wprintf(&shell_wnd,"Sleeping! \n");
			
	} 
	sleep(num);
}


 // Function to print processes and their info
 
void AllProcs(){
	print_all_processes(&shell_wnd);
}

 // Function to clears window screen

void clrWindow(){
	clear_window(&shell_wnd);
 	wprintf(&shell_wnd," Welcome to Ammar's Shell ! \n");
        wprintf(&shell_wnd,"\n type 'help' for a list of usable commands \n");
}


 // Function to echo the argument(s) passed
 
void Echo(){
	char* strng = cmdBuffer+5;
	wprintf(&shell_wnd,strng);
	wprintf(&shell_wnd,"\n");
}

/**
 * This function stores the whitespaced removed argument from the command buffer starting at index passed
 * @param s   [pointer to the array inwhich the argument has to be stored]
 * @param num [Index of the argument inside the command buffer]
 */

void ignoreWhiteSpaces(char* s,int num){
	int i=0;
	i+=num;

	while(*(cmdBuffer+i)==' '||*(cmdBuffer+i)=='\t')	i++;
	while(*(cmdBuffer+i)!='\0'){
	 *s=*(cmdBuffer+i);
	 s++;
	 i++;
	}
	*s='\0';
}

/**
 * This function stores the whitespaced removed argument from the command buffer starting at index passed
 * @param s   [pointer to the array inwhich the argument has to be stored]
 * @param num [Index of the argument inside the command buffer]
 * @return [Returns the current counter in the command array]
 */

int ignoreWhiteSpacesReturn(char* s,int num){
	int i=0;
	i+=num;

	while(*(cmdBuffer+i)==' '||*(cmdBuffer+i)=='\t')	i++;
	while(*(cmdBuffer+i)!='\0'){
	 *s=*(cmdBuffer+i);
	 s++;
	 i++;
	}
	*s='\0';
	return i;
}


 // Function to clear the buffer contents
 

void clrBuffer(){
	int i;
	for ( i= 0; i < 61; ++i)
	{
		cmdBuffer[i] = '\0';
	}

}

//Starts train at speed 4
void StartTrain(){

	change_speed('4');

}

//Stops the train 
void StopTrain(){

	change_speed('0');

}

//Func to Initialize Train Function
void InitTrain(){
	init_train(&train_window);
}

// Function to make process sleep for entered duuration
 
void ChgSpeed(){
	char spdPtr[10];
	ignoreWhiteSpaces(spdPtr,8);
	char* spd = spdPtr;
	while(*spd != '\0'){
		if (*spd >= '0' && *spd <= '5'){
			if(*spd == '0'){
				change_speed('0');
				break;
			}
			else if(*spd == '1'){
				change_speed('1');
				break;
			}
			else if(*spd == '2'){
				change_speed('2');
				break;
			}
			else if(*spd == '3'){
				change_speed('3');
				break;
			}
			else if(*spd == '4'){
				change_speed('4');
				break;
			}
			else if(*spd == '5'){
				change_speed('5');
				break;
			}
		}
		else{
			wprintf(&shell_wnd,"Please enter a valid Number between 0-5! \n");
			break;
		}
	}
}


/*
 * Basic String compare Function
 * @param1	[User entered command]
 * @param2	[Predefined TOS command]
 * @return	[Boolean]
 */

BOOL stringcmp(char* cmd1, char* cmd2){
	while (*cmd1 == *cmd2 && *cmd2 != '\0') {
        cmd1++;
        cmd2++;
    }
    return *cmd2 == '\0';
}

//function to handle trailing whitespaces before command
int trim(char* cmd){
   int i=0;
   while(*cmd==' ' || *cmd=='\t'){
   i++;
   cmd++;
   }
   return i;
}

// command Execution!
 
void execCmd(){
	int i=0; // the first character in the command buffer
        i=trim(&cmdBuffer[0]);
        char* cmd = &cmdBuffer[i];
	char arg[10];
	if(stringcmp(cmd,"help")){
		Help();
	}else if(stringcmp(cmd,"clr")){
		clrWindow();
	}else if(stringcmp(cmd,"echo ")){
		Echo();
	}else if(stringcmp(cmd,"echo")){
		wprintf(&shell_wnd,"Invalid command! please type echo 'your string' \n");
	}else if(stringcmp(cmd,"wait ")){
		Sleep();
	}else if(stringcmp(cmd,"wait")){
		wprintf(&shell_wnd,"Invalid command! please type wait 'duration to sleep' \n");
	}else if(stringcmp(cmd,"ps")){
		AllProcs();
	}else if(stringcmp(cmd,"train ")){
		int count;
		count = ignoreWhiteSpacesReturn(arg,6);
		if(stringcmp(arg,"auto")){
		InitTrain();
		}else if(stringcmp(arg,"stop")){
			if(!isRunning()){
				wprintf(&shell_wnd,"Train stopping!\n");
				StopTrain();
			}else{
				wprintf(&shell_wnd,"Train is in auto-pilot!\n");
			}
		}else if(stringcmp(arg,"d")){
			if(!isRunning()){
				wprintf(&shell_wnd,"Changing Train direction!\n");
				change_direction();
			}else{
				wprintf(&shell_wnd,"Train is in auto-pilot!\n");
			}
		}else if(stringcmp(arg,"s")){
			if(!isRunning()){
				wprintf(&shell_wnd,"Changing Train speed!\n");
				ChgSpeed();
			}else{
				wprintf(&shell_wnd,"Train is in auto-pilot!\n");
			}
		}else if(stringcmp(arg,"go")){
			if(!isRunning()){
				wprintf(&shell_wnd,"Train is on the GO!\n");
				StartTrain();
			}else{
				wprintf(&shell_wnd,"Train is in auto-pilot!\n");
			}
		} else if(stringcmp(arg,"help")){
			wprintf(&shell_wnd,"Train HELP:::\n");
			wprintf(&shell_wnd,"train auto               -Starts automatic train Application\n");
			wprintf(&shell_wnd,"train stop               -Stops the train\n");
			wprintf(&shell_wnd,"train go                 -Makes the train go\n");
			wprintf(&shell_wnd,"train d                  -Changes the train direction\n");
			wprintf(&shell_wnd,"train s 'num'            -Changes speed\n");
			wprintf(&shell_wnd,"train help               -displays train help\n");
		}else {
			wprintf(&shell_wnd,"Entered wrong command, enter \"train help\" for details\n");
		}
	}else if(stringcmp(cmd,"train")){
		wprintf(&shell_wnd,"Please enter \"train help\" for details\n");
	}else{
    		wprintf(&shell_wnd,"Please enter a valid Command! \n");
        }    
}




 // Entry point for the main Shell Process

void tosShellProcess(PROCESS self, PARAM param){


 	char ch;
 	Keyb_Message msg;

 	clear_window(kernel_window);

 	wprintf(&shell_wnd," Welcome to Ammar's Shell ! \n");
        wprintf(&shell_wnd,"\n type 'help' for a list of usable commands \n");


	
	// Start the main loop
	
	while(1){
		
		output_char(&shell_wnd,'=');
		output_char(&shell_wnd,'>');
		output_char(&shell_wnd,' ');
		while(1){

			msg.key_buffer = &ch;
			send(keyb_port, &msg);

			switch(ch){
				
                
				//check for return
				
				case 13:
					cmdBuffer[i]='\0';
					wprintf(&shell_wnd, "\n");
					execCmd();
					i=0;
					clrBuffer();
					output_char(&shell_wnd,'=');
					output_char(&shell_wnd,'>');
					output_char(&shell_wnd,' ');
					break;
				
				//check for backspace
		
				case '\b':
 		                       if (i == 0)
                    		       continue;
                    i--;
                    wprintf(&shell_wnd, "%c", ch);
                    break;

				//other input
				
				default:
					cmdBuffer[i] = ch;
					
					//Check for overflow...
					 
					if(i<59){
						i++;
						wprintf(&shell_wnd, "%c", ch);
					}

					
					break;
					

			}
		}
	}
}

 void init_shell()
 {

 	create_process(tosShellProcess,3,0,"Shell");
 	resign();
 }
