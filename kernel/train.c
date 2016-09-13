#include <kernel.h>

#define TRAIN_NUMBER "20\0"

static WINDOW train_window=  {0, 0, 80, 10, 0, 0, ' '};

//Variable to pause/sleep between commands
int sleep_ticks = 10;

//current config
int config =0;

//Variable to determine whether train auto-pilot is active
int running =0;


void ClrBuffer(void);
void ConcatenateStr(char*,char*);
void send_ToCom(char*);
void InitSwitch(void);
void setSwitch(char*,char*);
int getSwitchStat(char*);
void change_direction(void);
void change_speed(char*);
void config1(void);
int isRunning(void);



//Clear the train buffer
void ClrBuffer(){
	char cmd[5];

	cmd[0] = 'R';
	cmd[1] = '\0';
	send_ToCom(cmd);
}

//Concatenate Strings
void ConcatenateStr(char* s1, char* s2){
	while (*s1!='\0') s1++;
	while (*s2!='\0') {
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1='\0';

}

//Sends Commands to Train
void send_ToCom(char* string){


	char command[15];
	command[0] = '\0';
	
	char temp[10];
	COM_Message msg;
	ConcatenateStr(command,string);
	ConcatenateStr(command,"\015");
	
	int len;
	msg.output_buffer =command;
   	msg.len_input_buffer = len;
   	msg.input_buffer = temp;

   	
   	sleep(sleep_ticks);
   	send(com_port,&msg);

}

//Initializing switches to handle Zamboni
void InitSwitch(){
	setSwitch('5','G');
	setSwitch('8','G');
	setSwitch('9','R');
	setSwitch('1','G');
	setSwitch('4','G');
}

//Set Switch to R or G
void setSwitch(char* switchNum, char* RorG){
	
	char cmd[5];
	cmd[0] = 'M';
	cmd[1] = switchNum;
	cmd[2] = RorG;
	cmd[3] = '\0';
	send_ToCom(cmd);
}


//Checking for vehicle on contact, return 1 if vehicle present otherwise returns 0
int getSwitchStat(char* c){

	char cmd[10];
	ClrBuffer();
	cmd[0] = 'C';
	cmd[1] = '\0';
	ConcatenateStr(cmd,c);
	ConcatenateStr(cmd,"\015");
	COM_Message msg;
	
	char* input;
	int len=3;

	msg.output_buffer = cmd;
   	msg.len_input_buffer = len;
   	msg.input_buffer = input;
      	sleep(sleep_ticks);
   	send(com_port,&msg);
   	if(msg.input_buffer[1]=='1') return 1;
   		else return 0;
}


//change direction of train
void change_direction(){
	
	char cmd[5];
	cmd[0] = 'L';
	cmd[1] = '\0';
	ConcatenateStr(cmd,TRAIN_NUMBER);
	cmd[3] = 'D';
	cmd[4] = '\0';
	send_ToCom(cmd);
}

//Change Train's Speed
void change_speed(char* s){
	
	char cmd[6];
	cmd[0] = 'L';
	cmd[1] = '\0';
	ConcatenateStr(cmd,TRAIN_NUMBER);
	cmd[3] = 'S';
	cmd[4] = s;
	cmd[5] = '\0';
	send_ToCom(cmd);
}


//check if train is running
int isRunning(){
	return running;
}


//configuration 1 & 2
void config1(){
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','G');

	while(!getSwitchStat("1"));
	change_speed('0');
	change_direction();
	change_speed('4');
	setSwitch('5','R');
	setSwitch('6','R');

	while(!getSwitchStat("8"));
	sleep(150);
	change_speed('0');
	wprintf(&train_window,"Great Success!\n");
}

//configuration 3
void config3(){
	change_speed('4');
	setSwitch('5','R');
	setSwitch('6','G');
	setSwitch('7','R');

	while(!getSwitchStat("12"));
	change_speed('0');
	change_direction();
	change_speed('4');

	while(!getSwitchStat("13"));
	change_speed('0');
	setSwitch('8','G');
	change_direction();
	change_speed('4');
	setSwitch('5','G');
	setSwitch('4','R');
	setSwitch('3','R');

	while(!getSwitchStat("5"));
	change_speed('0');
	wprintf(&train_window,"Great Success!\n");
	}

//configuration 4
void config4(){
	change_speed('4');
	setSwitch('5','G');
	setSwitch('8','G');
	setSwitch('9','G');

	while(!getSwitchStat("14"));
	change_speed('0');
	change_speed('4');

	while(!getSwitchStat("16"));
	sleep(650);//Dead-end of track, sleep time may vary on different systems!
	change_speed('0');
	change_direction();
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','R');

	while(!getSwitchStat("5"));
	change_speed('0');
	wprintf(&train_window,"Great Success!\n");
}

//configuration 1 with Zamboni
void config1Zamboni(){
	setSwitch('7','R');
	setSwitch('2','R');

	while(!getSwitchStat("10"));
	setSwitch('1','R');
	config1();
}

//configuration 2 with Zamboni
void config2Zamboni(){
	setSwitch('7','R');
	setSwitch('2','R');

	while(!getSwitchStat("3"));
	setSwitch('8','R');
	setSwitch('1','R');
	config1();
}


//configuration 3 with Zamboni
void config3Zamboni(){
	setSwitch('4','R');
	setSwitch('3','R');

	while(!getSwitchStat("10"));
	change_speed('5');
	setSwitch('5','R');
	setSwitch('6','G');

	while(!getSwitchStat("9"));
	setSwitch('5','G');

	while(!getSwitchStat("12"));
	change_speed('0');
	change_direction();
	setSwitch('7','R');
	
	while(!getSwitchStat("13"));	
	setSwitch('2','R');
	change_speed('5');

	while(!getSwitchStat("3"));
	setSwitch('1','R');
	
	while(!getSwitchStat("12"));
	change_speed('0');
	setSwitch('1','G');
	setSwitch('7','G');
	change_speed('4');

	while(!getSwitchStat("9"));
	change_speed('0');
	setSwitch('7','R');
	setSwitch('2','R');

	while(!getSwitchStat("10"));
	setSwitch('1','R');
	change_speed('4');
	setSwitch('4','R');
	setSwitch('3','R');

	while(!getSwitchStat("5"));
	change_speed('0');
	wprintf(&train_window,"Great Success!\n");
}


//configuration 4 with Zamboni
void config4Zamboni(){

	while(!getSwitchStat("14"));
	setSwitch('8','R');
	change_speed('4');

	while(!getSwitchStat("6"));
	change_speed('0');
	change_direction();
	setSwitch('4','R');
	setSwitch('3','G');
	change_speed('4');

	while(!getSwitchStat("1"));
	change_speed('0');

	while(!getSwitchStat("12"));
	setSwitch('8','G');

	while(!getSwitchStat("13"));
	change_speed('5');
	setSwitch('4','G');
	setSwitch('9','G');

	while(!getSwitchStat("14"));
	change_speed('0');
	change_direction();	
	change_speed('4');

	while(!getSwitchStat("16"));
	sleep(450);//Dead-end of track, sleep time may vary on different systems!
	change_speed('0');
	setSwitch('3','R');
	setSwitch('9','R');

	while(!getSwitchStat("13"));
	change_direction();
	change_speed('4');

	while(!getSwitchStat("14"));
	change_speed('0');
	change_direction();
	change_speed('5');
	setSwitch('2','G');
	setSwitch('1','R');

	while(!getSwitchStat("1"));
	change_speed('0');
	
	while(!getSwitchStat("14"));
	setSwitch('8','R');
	change_speed('4');

	while(!getSwitchStat("6"));
	change_speed('0');
	setSwitch('4','R');
	setSwitch('3','R');
	change_direction();
	change_speed('4');

	while(!getSwitchStat("5"));
	change_speed('0');

	wprintf(&train_window,"Great Success!\n");
}


//Check for Zamboni
int check_zamboni(){
	int i;
	int found=0;
	for(i=0;i<60;i++){
		sleep(sleep_ticks);
			if(getSwitchStat("4")){
				found =1;
				break;
			}
		}

	for(i=0;i<60 && found;i++){
		
			sleep(sleep_ticks);
			if(getSwitchStat("7")){
				return 7;
			}
			sleep(sleep_ticks);
			if(getSwitchStat("13")){
				return 13;
			}
		
	}
	return 0;
}

//determine the type of configuration
int check_config(){
	if((getSwitchStat("8"))&&(getSwitchStat("2"))){
		//config1 & config2
		config= 1;
	}else if((getSwitchStat("5"))&&(getSwitchStat("11"))){
		//config3
		config= 3;
	}else if((getSwitchStat("5"))&&(getSwitchStat("16"))){
		//config4
		config= 4;
	}
	else {
		config=0;
	}

	return config;
}

//Train Process
void train_process(PROCESS self, PARAM param)
{
	clear_window(&train_window);
	wprintf(&train_window,"Initializing Train\n");
	wprintf(&train_window,"Recognizing Setup\n");

	
	running =1;
	int zam=0;
	int config=0;

	InitSwitch();
	ClrBuffer();
	zam = check_zamboni();


	if(zam){
		wprintf(&train_window,"Zamboni Detected!\n");
		
	}else wprintf(&train_window,"Zamboni not found\n");
	config=check_config();

	//WITHOUT ZAMBONI
	switch(config){
		case 0:
			wprintf(&train_window,"Configuration not found\n");
			break;
		case 1:
			wprintf(&train_window,"Configuration 1/2 detected\n");
			if(!zam) config1();
			break;
		case 3:
			wprintf(&train_window,"Configuration 3 detected\n");
			if(!zam) config3();
			break;
		case 4:
			wprintf(&train_window,"Configuration 4 detected\n");
			if(!zam) config4();
			break;
	}

	//WITH ZAMBONI
	if(zam==7&&config==1){
		config1Zamboni();
	}else if(zam==13&&config==1){
		config2Zamboni();
	}else if(zam&&config==3){
		config3Zamboni();
	}
	else if(zam&&config==4){
		config4Zamboni();
	}

	running=0;
	remove_ready_queue(active_proc);
	resign();
	while(1);
	
}

//Initialize train functions
void init_train(WINDOW* wnd)
{
	create_process(train_process, 4, 0, "Train Process");
}

