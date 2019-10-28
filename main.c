#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STRING 50

void read_file(char *file_name);
void separate(char *linea,FILE *file_estimate);
void estimate(int ts,float acc_x,float acc_y,float acc_z,FILE *file_estimate);
void write_line(FILE *f,double pitch,double roll);


int main(int argc, char *argv[]) {
	char file_name[]="attitude_exam.log";
	read_file(file_name);
	return 0;
}

void read_file(char *file_name){
	FILE *file;
	FILE *file_estimate;
	char linea[1024];
	file = fopen(file_name, "r");
	file_estimate = fopen("estimate_roll_pitch.log","w");
    while(fgets(linea, 1024, (FILE*) file)) {
        separate(linea,file_estimate);
    }
    fclose(file);
    fclose(file_estimate);
}

void separate(char *linea,FILE *file_estimate){
	char* token = strtok(linea, ";");
	int ts;
	float acc_x,acc_y,acc_z;
	float total;
    ts = atoi(token);
	token = strtok(NULL, ";"); 
	acc_x = strtof(token,NULL)/1000;
	token = strtok(NULL, ";"); 
	acc_y = strtof(token,NULL)/1000;
	token = strtok(NULL, ";"); 
	acc_z = strtof(token,NULL)/1000;
	total = acc_x*acc_x + acc_y*acc_y + acc_z*acc_z;
	//printf("%i X: %.3f Y: %.3f Z: %.3f  %.2f\n",ts,acc_x,acc_y,acc_z,total);
	estimate(ts,acc_x,acc_y,acc_z,file_estimate);
}

void estimate(int ts,float acc_x,float acc_y,float acc_z,FILE *file_estimate){
	double Gx = acc_x;
  	double Gy = acc_y;
  	double Gz = acc_z;
  	double u = 0.01;
  	double sgn = 0;
  	double roll = 0.00,pitch=0.00;
  	
  	pitch = atan2(-Gx,sqrt(Gy*Gy + Gz*Gz)) * 57.3;
  	if(Gz<0)
  		sgn = -1;
  	else if (Gz>0)
  		sgn = 1;
  	roll = atan2(Gy, sgn*(sqrt(Gz*Gz + Gx*Gx*u))) * 57.3;
  	printf("Pitch: %.2f  Roll: %.2f\n",pitch,roll);
  	write_line(file_estimate,pitch,roll);
}

void write_line(FILE *f,double pitch,double roll){
	char line[MAX_STRING]="";
	char bufP[5],bufR[8];
	gcvt(pitch, 6, bufP); 
	gcvt(roll, 8, bufR); 
	strcat(line,bufP);
  	strcat(line,",");
  	strcat(line,bufR);
  	strcat(line,"\n");
  	fputs(line,f);
}
