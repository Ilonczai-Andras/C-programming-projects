#include "helper.h"

int main(int argc, char* argv[]) {
    int send = 1;
    int receive = 0;

    int file = 1;
    int socket = 0;
    
    if (strstr(argv[0], "chart") == NULL) 
    {
        printf("Hiba: A futtatható állomány neve nem megfelelő.\n");
        exit(1);
    }

    if (argc < 2)
    {
        
    }
    else if (argc >= 3)
    {
        
    }
    else if ( strcmp(argv[1], "--help") == 0) 
    {
        help();
        exit(0);
    }
    else if (strcmp(argv[1], "--version") == 0) 
    {
        vers();
        exit(0);
    }
    else if (strcmp(argv[1], "-send") == 0) 
    {
        
    }
    else if (strcmp(argv[1], "-receive") == 0) 
    {   
        receive = 1;
        send = 0;
    }
    else 
    {   
        printf("Hiba: Nem érvényes argumentumot adtál meg. Segítségért használd a --help kapcsolót.\n");
        exit(1);
    }

    //printf("%d %s %s", argc, argv[1], argv[2]);
    if (argc >= 3) 
    {   
        if (strcmp(argv[2], "-receive") == 0 || strcmp(argv[1], "-receive") == 0) 
        {
            receive = 1;
            send = 0;
        }
        if (strcmp(argv[2], "-socket") == 0 || strcmp(argv[1], "-socket") == 0) 
        {
            socket = 1;
            file = 0;
            
        }
        if (strcmp(argv[2], "-send") == 0 || strcmp(argv[1], "-send") == 0)
        {
            
        }
        if (strcmp(argv[2], "-file") == 0 || strcmp(argv[1], "-file") == 0)
        {
            
        }
        /*else 
        {
            printf("Hiba: Nem érvényes kommunikációs módot adtál meg.\n");
            exit(1);
        }*/
    }

    if (send && file)
    {   
        printf("A kommunikáció fájl alapú módon fog történni.\n");
        printf("A program küldő üzemmódban fog működni.\n");
        int *eredmeny;
        int res = Measurement(&eredmeny);

        SendViaFile(eredmeny, res);
    }

    if (send && socket)
    {   
        printf("A program küldő üzemmódban fog működni.\n");
        printf("A kommunikáció socket alapú módon fog történni.\n");
        int *eredmeny;
        int res = Measurement(&eredmeny);
        SendViaSocket(eredmeny, res);
    }

    if (receive && file)
    {   
        printf("A program fogadó üzemmódban fog működni.\n");
        printf("A kommunikáció fájl alapú módon fog történni.\n");

 
        signal(SIGUSR1, ReceiveViaFile);

        while (1) 
        {
            pause();
        }
    }

    if (receive && socket)
    {   
        printf("A program fogadó üzemmódban fog működni.\n");
        printf("A kommunikáció socket alapú módon fog történni.\n");
        ReceiveViaSocket();
    }
    return 0;
}