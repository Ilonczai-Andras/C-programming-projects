#include "helper.h"

int *res;
int index_var = 0;
int s;
#pragma pack(push,1)
typedef struct
{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct
{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;

typedef struct 
{
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)

void SignalHandler(int sig)
{
    if(sig==SIGUSR1)
    {
        printf("\nA fajlon keresztuli kuldés szolgáltatas nem elerheto!\n");
    }
    else if (sig == SIGINT)
    {
        printf("\nViszlát!\n");
        exit(0);
    }
    else if (sig == SIGALRM)
    {
        printf("\nA szerver nem válaszol időkereten belül\n");
        exit(0); 
    } 
}

void BMPcreator(int *Values, int numvalues)
{   
    int pixelbytesize = (numvalues*numvalues*24)/8;
    int filesize = pixelbytesize+sizeof(bitmap);
    
    int fd = open("chart.bmp", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        printf("Error: Unable to create file.\n");
        return;
    }
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    uint8_t *pixelbuffer = (uint8_t*)malloc(pixelbytesize);

    strcpy(pbitmap->fileheader.signature,"BM");
            pbitmap->fileheader.filesize = filesize;
            pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);

            pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
            pbitmap->bitmapinfoheader.width = numvalues;
            pbitmap->bitmapinfoheader.height = numvalues;
            pbitmap->bitmapinfoheader.planes = _planes;
            pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
            pbitmap->bitmapinfoheader.compression = _compression;
            pbitmap->bitmapinfoheader.imagesize = pixelbytesize;
            pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
            pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
            pbitmap->bitmapinfoheader.numcolorspallette = 0;


    memset(pixelbuffer, 0xFF, pixelbytesize);

    int y = numvalues / 2;
    int index = (y * numvalues + 0) * 3; // 3 bytes per pixel (24 bits)
    pixelbuffer[index] = 0xFF;             // Red component
    pixelbuffer[index + 1] = 0x00;         // Green component
    pixelbuffer[index + 2] = 0x00;         // Blue component

    for (int x = 1; x < numvalues; x++) 
    {
        int index = (y * numvalues + x) * 3;
        if (Values[x] == Values[x - 1]) 
        {

            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        } 
        else if (Values[x] > Values[x - 1]) 
        {

            y++;
            index = (y * numvalues + x) * 3;
            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        } 
        else 
        {
            y--;
            index = (y * numvalues + x) * 3;
            pixelbuffer[index] = 0xFF;     // Red component
            pixelbuffer[index + 1] = 0x00; // Green component
            pixelbuffer[index + 2] = 0x00; // Blue component
        }
    }

    write(fd, pbitmap, sizeof(bitmap));
    write(fd, pixelbuffer, pixelbytesize);

    close(fd);
}

int FindPID() {
    DIR *proc_dir = opendir(PROC_DIRECTORY);
    struct dirent *entry;
    int found_pid = -1;
    pid_t own_pid = getpid();

    if (proc_dir == NULL) {
        perror("Failed to open /proc directory");
        return -1;
    }

    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
            char path[300];
            snprintf(path, sizeof(path), "%s/%s/%s", PROC_DIRECTORY, entry->d_name, STATUS_FILE);

            FILE *status_file = fopen(path, "r");

            if (status_file == NULL) {
                perror("Failed to open status file");
                closedir(proc_dir);
                return -1;
            }

            char line[256];
            while (fgets(line, sizeof(line), status_file) != NULL) 
            {
                //strncmp(const char *str1, const char *str2, size_t n) compares at most the first n bytes of str1 and str2.
                if (strncmp(line, NAME_LINE_PREFIX, strlen(NAME_LINE_PREFIX)) == 0) {
                    char name[256];
                    //The C library function int sscanf(const char *str, const char *format, ...) reads formatted input from a string.
                    sscanf(line, "Name:\t%s", name);
                    if (strcmp(name, "chart") == 0) {
                        int pid;
                        while (fgets(line, sizeof(line), status_file) != NULL) {
                            if (strncmp(line, PID_LINE_PREFIX, strlen(PID_LINE_PREFIX)) == 0) {
                                sscanf(line, "Pid:\t%d", &pid);
                                if (pid != own_pid) {
                                    found_pid = pid;
                                    break;
                                }
                            }
                        }
                    }
                    break;
                }
            }

            fclose(status_file);
            if (found_pid != -1) {
                break;
            }
        }
    }

    closedir(proc_dir);
    return found_pid;
}
 
void ReceiveViaSocket()
{
   /************************ Declarations **********************/
   int bytes;                        // received/sent bytes
   int err;                          // error code
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   unsigned int client_size;         // length of the sockaddr_in client
   struct sockaddr_in server;        // address of server
   struct sockaddr_in client;        // address of client

   /************************ Initialization ********************/
   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;
   client_size = sizeof client;
   signal(SIGINT, SignalHandler);
   signal(SIGTERM, SignalHandler);

   /************************ Creating socket *******************/
   s = socket(AF_INET, SOCK_DGRAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, "Socket creation error.\n");
      exit(1);
      }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Binding socket ********************/
   err = bind(s, (struct sockaddr *) &server, server_size);
   if ( err < 0 ) {
      fprintf(stderr,"Binding error.\n");
      exit(2);
   }
   
   int szam = 0;

   /************************ Receive data **********************/
    bytes = recvfrom(s, buffer, BUFSIZE, flag, (struct sockaddr *) &client, &client_size );
    if ( bytes < 0 ) 
    {
        fprintf(stderr, " Receiving error.\n");
        exit(3);
    }
    szam = atoi(buffer);
    

    /************************ Sending data **********************/
    bytes = sendto(s, buffer, strlen(buffer)+1, flag, (struct sockaddr *) &client, client_size);
    if ( bytes <= 0 ) 
    {
        fprintf(stderr, "Sending error.\n");
        exit(4);
    }
    printf("Meret: %s\n", buffer);

    while (1)
    {   
        index_var = 0;
        res = (int*)realloc(res, (index_var + 1) * sizeof(int));
        while(1)
        { 
            printf("\nWaiting for a message...\n");
            /************************ Receive data **********************/
            bytes = recvfrom(s, buffer, BUFSIZE, flag, (struct sockaddr *) &client, &client_size );
            if ( bytes < 0 ) 
            {
                fprintf(stderr, " Receiving error.\n");
                exit(5);
            }
            printf ("%d Client's message: %s\n", index_var + 1, buffer);

            res = (int*)realloc(res, (index_var + 1) * sizeof(int));
            if (res == NULL) 
            {
                fprintf(stderr, "Memory reallocation failed.\n");
                exit(6);
            }
            res[index_var] = atoi(buffer);
            index_var++;

            /************************ Sending data **********************/
            bytes = sendto(s, buffer, strlen(buffer)+1, flag, (struct sockaddr *) &client, client_size);
            if ( bytes <= 0 ) 
            {
                fprintf(stderr, "Sending error.\n");
                exit(7);
            }
            printf ("%d Server's message: %s\n", index_var, buffer);

            if (index_var == szam)
            {
                break;
            }
    
        }
        BMPcreator(res, index_var);
    }
   
   free(res);
}

void SendViaSocket(int *Values, int NumValues)
{
    /************************ Declarations **********************/
   int s;                            // socket ID
   int bytes;                        // received/sent bytes
   int flag;                         // transmission flag
   char on;                          // sockopt option
   char buffer[BUFSIZE];             // datagram buffer area
   unsigned int server_size;         // length of the sockaddr_in server
   struct sockaddr_in server;        // address of server

   /************************ Initialization ********************/
   on   = 1;
   flag = 0;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = inet_addr("127.0.0.1");
   server.sin_port        = htons(PORT_NO);
   server_size = sizeof server;

   /************************ Creating socket *******************/
   s = socket(AF_INET, SOCK_DGRAM, 0 );
   if ( s < 0 ) {
      fprintf(stderr, " Socket creation error.\n");
      exit(10);
   }
   setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
   setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

   /************************ Sending data **********************/
    sprintf(buffer, "%d", NumValues); 
    bytes = sendto(s, buffer, strlen(buffer)+1, flag, (struct sockaddr *) &server, server_size);
    if ( bytes <= 0 )
    {
        fprintf(stderr, " Sending error.\n");
        exit(11);
    }
    printf ("Meresi adat db: %s\n", buffer);
    
    signal(SIGALRM, SignalHandler);
    alarm(1);
    
    /************************ Receive data **********************/
    bytes = recvfrom(s, buffer, BUFSIZE, flag, (struct sockaddr *) &server, &server_size);
    if ( bytes < 0 ) 
    {
      fprintf(stderr, "Receiving error.\n");
      exit(12);
    }
    printf("Server's valasz: %s\n\n", buffer);

    char NumValues_to_str[20]; 

    sprintf(NumValues_to_str, "%d", NumValues);

   if (strcmp(NumValues_to_str, buffer) != 0)
   {    
        fprintf(stderr, "Küldött érték nem egyenlő a viszakapottal\n");
        exit(13);
        //Küldött érték nem egyenlő a viszakapottal;
   }
   
   int i = 0;
   char mentett[100];
   while(i < NumValues) 
   {
        sprintf(buffer, "%d", Values[i]);
        /************************ Sending data **********************/
        bytes = sendto(s, buffer, strlen(buffer)+1, flag, (struct sockaddr *) &server, server_size);
        if ( bytes <= 0 ) {
            fprintf(stderr, " Sending error.\n");
            exit(14);
        }
        printf ("%d word: \"%s\" have been sent to server.\n",i+1,buffer);
        strcpy(mentett, buffer);
        /************************ Receive data **********************/
        bytes = recvfrom(s, buffer, BUFSIZE, flag, (struct sockaddr *) &server, &server_size);
        if ( bytes < 0 ) 
        {
            fprintf(stderr, "Receiving error.\n");
            exit(15);
        }
        printf("%d word: \"%s\" From server\n", i+1, buffer);
        if (strlen(mentett) != strlen(buffer))
        {   
            fprintf(stderr, "Az adatok bajtban megadott merete es a most kapott ertékek elteroek\n");
            exit(16);
        }
        i++;
        
   }

   /************************ Closing ***************************/
   close(s);
}

void ReceiveViaFile(int sig) 
{
    signal(SIGINT, SignalHandler);

    FILE *file = fopen("Measurement.txt", "r");
    if (file == NULL) {
        printf("Hiba: A fajl nem talalhato vagy nem sikerult megnyitni.\n");
        return;
    }

    int capacity = 100;
    int *measurements = (int*)malloc(capacity * sizeof(int));
    if (measurements == NULL) {
        printf("Hiba: Nem sikerult memoriat foglalni.\n");
        fclose(file);
        return;
    }

    int count = 0; // Számláló az elemek számához

    int measurement;
    while (fscanf(file, "%d", &measurement) == 1) {

        if (count == capacity) {
            capacity *= 2;
            int *temp = realloc(measurements, capacity * sizeof(int));
            if (temp == NULL) {
                printf("Hiba: Nem sikerult memoriat boviteni.\n");
                free(measurements);
                fclose(file);
                return;
            }
            measurements = temp;
        }
        measurements[count++] = measurement;
    }

    fclose(file);

    BMPcreator(measurements, count);

    free(measurements);
}

void SendViaFile(int *Values, int NumValues) 
{
    FILE *file = fopen("Measurement.txt", "w");
    if (file == NULL) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NumValues; ++i) 
    {
        fprintf(file, "%d\n", Values[i]);
    }

    fclose(file);

    int pid = FindPID();
    if (pid == -1) 
    {
        printf("Error: No process in receiver mode found.\n");
        exit(EXIT_FAILURE);
    } 
    else 
    {   
        kill(pid, SIGUSR1);
    }
}

void help()
{
    printf("Lehetseges parancsok:\n");
    printf("--help\n");
    printf("--version   verzio lekerdezes\n");

    printf("\nProgram uzemmodjai:\n");
    printf("-send       kuldo uzemmod\n");
    printf("-receive    fogado uzemmod\n");

    printf("\nKommunikacio uzemmodjai:\n");
    printf("-file       file uzemmod\n");
    printf("-socket     socket uzemmod\n");
}

void vers()
{   
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("\nDeveloper: %s\n", name_of);
        }

        #pragma omp section
        {
            printf("\nVersion: %.1f\n", version);
        }

        #pragma omp section
        {
            printf("\nDate: %s\n", date);
        }
    }
}

int megfelelo(int perc, int ms)
{
    return ((perc * 60 + ms) < 100) ? 1 : 0;
}

int Measurement(int **Values)
{
    time_t T1; // Variable to store current time

    // Get current time and store it in T1
    time(&T1);

    struct tm *T4;
    T4 = localtime(&T1); // Convert current time to local time and store in T4

    //1. 35,4838% == 11/31 kovetkezo elemet csokkentjuk 1-el
    //2. 42,8571% kovetkezo elemet noveljuk 1-el
    //3. 21,6591% a kovetkezo elem marad ugyanaz
    
    int perc = T4->tm_min;
    int ms = T4->tm_sec;

    // int perc = 15;
    // int ms = 24;

    // int perc = 41;
    // int ms = 27;

    int eredmeny = 0;

    if (perc >= 0 && perc < 15)
    {   
        if (megfelelo(perc, ms))
        {
            eredmeny = 100;
        }
        else
        {
            eredmeny = perc * 60 + ms;
        }
    }

    if(perc >= 15 && perc < 30)
    {   
        if (megfelelo(perc-15, ms))
        {
            eredmeny = 100;
        }
        else
        {   
            perc -= 15;
            eredmeny = perc * 60 + ms;
        }
    }

    if (perc >= 30 && perc < 45)
    {   
        if (megfelelo(perc - 30, ms))
        {
            eredmeny = 100;
        }
        else
        {
            perc -= 30;
            eredmeny = perc * 60 + ms;
        }
    }
    
    if(perc >= 45 && perc < 60)
    {
        if (megfelelo(perc - 45, ms))
        {
            eredmeny = 100;
        }
        else
        {
            perc -= 45;
            eredmeny = perc * 60 + ms;
        }
    }
    //printf("Perc: %d ms: %d Ered: %d\n",T4->tm_min, T4->tm_sec, eredmeny);
    //Sleep(1000);

    if (eredmeny % 32 == 0)
    {
        
    }
    else
    {
        int tmp = 0;
        tmp = eredmeny % 32;
        eredmeny = eredmeny + (32 - tmp);
    }

    srand(time(NULL));

    int *values = (int *)malloc(eredmeny * sizeof(int));
    values[0] = 0;

    for (int i = 1; i < eredmeny; i++)
    {   
        double d = (double)rand() / RAND_MAX;

        if (d < 0.428571)
        {
            values[i] = values[i - 1] + 1;
        }
        else if (d >= 0.428571 && d < (0.354838 + 0.428571))
        {
            values[i] = values[i - 1] - 1;
        }
        else
        {
            values[i] = values[i - 1];
        }
    }
    /*for (int i = 0; i < eredmeny; i++)
    {
        printf("%d\n", i);
    }*/
    

    *Values = values;

    return eredmeny;
}