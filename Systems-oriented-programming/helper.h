#ifndef HELPER_H
#define HELPER_H

#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
//ido
#include <time.h>
//konyvtar
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <signal.h>
//Parhuzamos
#include<omp.h>
//socket
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
//Data
#define name_of "Ilonczai András"
#define version 1.01
#define date "2024.03.02"
//BMP
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _xpixelpermeter 3937
#define _ypixelpermeter 3937
#define pixel 0xFF
#pragma pack(push,1)
#pragma pack(pop)
//konyvtar
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <dirent.h>
#include <signal.h>

#define BUFSIZE 1024
#define PORT_NO 3333 

//PID
#define PROC_DIRECTORY "/proc"
#define STATUS_FILE "status"
#define NAME_LINE_PREFIX "Name:"
#define PID_LINE_PREFIX "Pid:"

// Function declarations
void SignalHandler(int sig);
int FindPID();
void ReceiveViaSocket();
void SendViaSocket(int *Values, int NumValues);
void ReceiveViaFile(int sig);
void SendViaFile(int *Values, int NumValues);
void help();
void vers();
int megfelelo(int perc, int ms);
int Measurement(int **Values);

#endif
