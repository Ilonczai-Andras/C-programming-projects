#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100

char *chess_table[8][8];
int sorok = 8;
int oszlopok = 8;

char feher[MAX]; 
char fekete[MAX];

int start[2];

void print_chess_table(int sorok, int oszlopok,char *matrix[sorok][oszlopok])
{
    for (int i = 0; i < sorok; i++)
    {
        for (int j = 0; j < oszlopok; j++)
        {
            printf("%s\t", matrix[i][j]);
        }
        puts("");
    }
    
}

char *chess_table[8][8];

void fill_chess_table(int sorok, int oszlopok, char *matrix[sorok][oszlopok])
{
    for (int i = 0; i < sorok; i++)
    {
        for (int j = 0; j < oszlopok; j++)
        {
            matrix[i][j] = "----";
        }
    }

    matrix[0][0] = "b.ba";
    matrix[0][1] = "b.lo";
    matrix[0][2] = "b.fu";
    matrix[0][3] = "b.kn";
    matrix[0][4] = "b.ki";
    matrix[0][5] = "b.fu";
    matrix[0][6] = "b.lo";
    matrix[0][7] = "b.ba";

    matrix[1][0] = "b.pa";
    matrix[1][1] = "b.pa";
    matrix[1][2] = "b.pa";
    matrix[1][3] = "b.pa";
    matrix[1][4] = "b.pa";
    matrix[1][5] = "b.pa";
    matrix[1][6] = "b.pa";
    matrix[1][7] = "b.pa";

    /*felső sorok feltöltése*/

    matrix[7][0] = "f.ba";
    matrix[7][1] = "f.lo";
    matrix[7][2] = "f.fu";
    matrix[7][3] = "f.kn";
    matrix[6][4] = "f.ki";
    matrix[7][5] = "f.fu";
    matrix[7][6] = "f.lo";
    matrix[7][7] = "f.ba";

    matrix[6][0] = "f.pa";
    matrix[6][1] = "f.pa";
    matrix[6][2] = "f.pa";
    matrix[6][3] = "f.pa";
    //matrix[6][4] = "f.pa";
    matrix[6][5] = "f.pa";
    matrix[6][6] = "f.pa";
    matrix[6][7] = "f.pa";

    /*alsó sorok feltöltése*/
}

void to_upper(char* str)
{
    for (int i = 0; str[i]!= '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }
}

int eleje(const char *pre, const char *str)
{
    //printf("pre: %s  -- str: %s -- hossz: %d\n", pre, str, strncmp(pre, str, strlen(pre)));
    return strncmp(pre, str, strlen(pre)) == 0;
}

int VegeOK(char *string)
{   int flag = 0;

    int flag2 = 0;
    
    int hossz = strlen(string);

    char utolso = (string[hossz - 1]);
    char utolsoe = (string[hossz - 2]);
    char utolsoe2 = (string[hossz - 3]);
    char utolsoe3 = (string[hossz - 4]);
    char utolsoe4 = (string[hossz - 5]);

    if ((utolso == ')') && (isdigit(utolsoe)) && (utolsoe2 == ',') && (isdigit(utolsoe3)) && (utolsoe4 =='('))
    {
        flag = 1;
    }
    //printf("Flag1: %d\n", flag);

    if( ( ((int)utolsoe - 48) > 0 && ((int)utolsoe - 48) < 9) && 
        ( ((int)utolsoe3 - 48)> 0 && ((int)utolsoe3 - 48)< 9))
    {
        flag2 = 1;
    }

    //printf("Flag2: %d\n", flag2);
    
    return flag && flag2;
}

int finish[2];

void cel(char* str)
{
    finish[0] = str[strlen(str) - 4] - 48 - 1;
    finish[1] = str[strlen(str) - 2] - 48 - 1;

}

void hol_van(int sorok, int oszlopok, char *matrix[sorok][oszlopok], int start[2], char *babu)
{   for (int i = 0; i < sorok; i++)
    {
        for (int j = 0; j < oszlopok; j++)
        {
            if ( (strcmp(babu, matrix[i][j]) == 0) )
            {   
                start[0] = i;
                start[1] = j;

                i = 8;
                j = 8;
            }
            //printf("%s i:%d j:%d egyezes: %d\n",matrix[i][j],i ,j, (strcmp(babu, matrix[i][j]) == 0) );
        }
    } 
}

char *fekete_babuk[6] = {"b.ba","b.lo","b.fu","b.kn","b.ki","b.pa"};
char *feher_babuk[6]  = {"f.ba","f.lo","f.fu","f.kn","f.ki","f.pa"};

int benne_van_a_tomben(char *babuk[6], char *string)
{   
    int flag = 0;

    for (int i = 0; i < 6; i++)
    {
        if (strcmp(babuk[i], string) == 0)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

void basttya_lepes(int sorok, int oszlopok, char *matrix[sorok][oszlopok],int sor_cel, int oszlop_cel, char *babu)
{   
    hol_van(sorok, oszlopok, matrix, start, babu);

    int elso_eset = 0;

    int masodik_eset = 0;

    int harmadik_eset = 0;

    int negyedik_eset = 0;

  
        
    for (int i = start[0]+1, j = start[1]; i < sorok; i++)//lefelé
    {   
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0))

        {   //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel-1][oszlop_cel-1]) == 0) );
            elso_eset = 1;
        }
        //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel-1][oszlop_cel-1]) == 0) );
    }
        
    for (int i = start[0]-1, j = start[1]; i > -1; i--)//felfelé
    {   
                
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0) )
        {   
            masodik_eset = 1;     
        }
       
    }

    for (int i = start[0], j = start[1]+1; j < oszlopok; j++)//jobbra
    {   
                
         if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0) )

        {   
             harmadik_eset = 1;
        }
                
    }
        
    for (int i = start[0], j = start[1]-1; j > -1; j--)//balra
    {   
                
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0) )

        {   
            negyedik_eset = 1;
        }
                
    }

    if (elso_eset || masodik_eset || harmadik_eset || negyedik_eset)
    {
        matrix[sor_cel][oszlop_cel] = babu;
        matrix[start[0]][start[1]] ="----";
    } 
}

void lo_lepes(int sorok, int oszlopok, char *matrix[sorok][oszlopok],int sor_cel, int oszlop_cel, char *babu)
{   
    int flag = 0;

    int uj_sor = 0;
    int uj_oszlop = 0;

    hol_van(sorok, oszlopok, matrix, start, babu);

    //printf("kezdo:i: %d kezdo_j: %d\n", start[0], start[1]);
    //printf("finish0: %d, finish1: %d\n", finish[0], finish[1]);

    int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    // Check all 8 possible moves
    for (int i = 0; i < 8; i++) 
    {
        uj_sor = start[0] + dx[i];
        uj_oszlop = start[1] + dy[i];

        //printf("ujsor: %d ujoszlop:%d 1.felt %d 2.felt%d 3.felt: %d\n", uj_sor, uj_oszlop, (uj_sor >= 0 && uj_sor < sorok), (uj_oszlop >= 0 && uj_oszlop < oszlopok), ((sor_cel == uj_sor) && (oszlop_cel == uj_oszlop)));

        
        if ( (uj_sor >= 0 && uj_sor < sorok) && (uj_oszlop >= 0 && uj_oszlop < oszlopok) && ((sor_cel == uj_sor) && (oszlop_cel == uj_oszlop)) ) 
        {
            flag = 1;
            break;
        }
    }

    if (flag)
    {
        matrix[sor_cel][oszlop_cel] = babu;
        matrix[start[0]][start[1]] ="----";
    } 
}
void futo_lepes(int sorok, int oszlopok, char *matrix[sorok][oszlopok],int sor_cel, int oszlop_cel, char *babu)
{
    hol_van(sorok, oszlopok, matrix, start, babu);


    int elso_eset = 0;
    int masodik_eset = 0;
    int harmadik_eset = 0;
    int negyedik_eset = 0;


    for (int i = start[0]-1, j = start[1] - 1; i >= 0, j >= 0 ; i-- , j--)
    {
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0))

        {   //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel][oszlop_cel]) == 0) );
            elso_eset = 1;
        }
    }

    for (int i = start[0] - 1, j = start[1] + 1; i >= 0, j <= 7 ; i-- , j++)
    {
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0))

        {   
            //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel-1][oszlop_cel-1]) == 0) );
            
            masodik_eset = 1;
        }
    }

    for (int i = start[0] + 1, j = start[1] + 1; i <= 0, j <= 7 ; i++ , j++)
    {
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0))

        {   
            //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel-1][oszlop_cel-1]) == 0) );
            
            harmadik_eset = 1;
        }
    }

    for (int i = start[0] + 1, j = start[1] - 1; i <= 7, j >= 0 ; i++ , j--)
    {
        if ( (i == sor_cel) && (j == oszlop_cel) && (benne_van_a_tomben(feher_babuk, matrix[sor_cel][oszlop_cel]) == 0))

        {   
            //printf("i: %d, sor: %d j: %d, oszlop: %d, benne: %d\n",i, sor_cel, j, oszlop_cel, (benne_van_a_tomben(fekete_babuk, matrix[sor_cel-1][oszlop_cel-1]) == 0) );
            
            negyedik_eset = 1;
        }
    }

    if (elso_eset || masodik_eset || harmadik_eset || negyedik_eset)
    {
        matrix[sor_cel][oszlop_cel] = babu;
        matrix[start[0]][start[1]] ="----";
    } 
}

void kiraly_lepes(int sorok, int oszlopok, char *matrix[sorok][oszlopok],int sor_cel, int oszlop_cel, char *babu)
{   
    int flag = 0;

    int uj_sor = 0;
    int uj_oszlop = 0;

    hol_van(sorok, oszlopok, matrix, start, babu);

    //printf("kezdo:i: %d kezdo_j: %d\n", start[0], start[1]);
    //printf("finish0: %d, finish1: %d\n", finish[0], finish[1]);

    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Check all 8 possible moves
    for (int i = 0; i < 8; i++) 
    {
        uj_sor = start[0] + dx[i];
        uj_oszlop = start[1] + dy[i];

        //printf("ujsor: %d ujoszlop:%d 1.felt %d 2.felt%d 3.felt: %d\n", uj_sor, uj_oszlop, (uj_sor >= 0 && uj_sor < sorok), (uj_oszlop >= 0 && uj_oszlop < oszlopok), ((sor_cel == uj_sor) && (oszlop_cel == uj_oszlop)));

        
        if ( (uj_sor >= 0 && uj_sor < sorok) && (uj_oszlop >= 0 && uj_oszlop < oszlopok) && ((sor_cel == uj_sor) && (oszlop_cel == uj_oszlop)) ) 
        {
            flag = 1;
            //printf("jo sor: %d jo oszlop: %d\n",uj_sor, uj_oszlop);
            break;
        }
    }

    if (flag)
    {
        matrix[sor_cel][oszlop_cel] = babu;
        matrix[start[0]][start[1]] ="----";
    } 
}

int utkozes(int sor, int oszlop, char *matrix[sor][oszlop], char *szin)
{   
    if (strcmp(szin, "feher")== 0)
    {     
        printf("Feher\n");

        int flag = 0;

        to_upper(feher);
        cel(feher);


        //printf("feher: %s\n", feher);

        if (eleje("BASTTYA",feher))
        {   
            printf("Bastya\n");

            hol_van(sorok, oszlopok, chess_table, start, feher_babuk[0]); //hol van a bábu
            //printf("ko0_i. %d, ko1_j:%d\n", start[0], start[1]);
            //printf("finish0: %d, finish1: %d\n", finish[0], finish[1]);
            puts("");

            if ((finish[0] > start[0]) && (finish[1] == start[1]))
            {
                for (int i = start[0]+1, j = start[1] ; i <= finish[0]; i++)//lefelé
                {   //printf("i: %d j:%d feherb: %d fekteb: %d\n",i,j,benne_van_a_tomben(feher_babuk, chess_table[i][j]),benne_van_a_tomben(fekete_babuk, chess_table[i][j]));

                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    
                        flag = 1;
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    }
                }
                return flag;
            }

            else if ((finish[0] < start[0]) && (finish[1] == start[1]))
            {   
                for (int i = start[0]-1, j = start[1] ; i >=0; i--)//felfelé
                {   
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                        return flag;
                        }
                    
                        flag = 1;
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    }
                    
                }
                return flag;
            }

            else if ((finish[0] == start[0]) && (finish[1] > start[1]))
            {
                for (int i = start[0], j = start[1]+1 ; j <=oszlopok; j++)//jobbra
                {   
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    
                        flag = 1;
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    }
                }
                return flag;
            }

            else if ((finish[0] == start[0]) && (finish[1] < start[1]))
            {
                for (int i = start[0], j = start[1]-1 ; j >= 0; j--)//balra
                {   
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    
                        flag = 1;
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {
                        if ((finish[0] == i) && (finish[1] == j))
                        {
                            return flag;
                        }
                    }
                }
                return flag;
            }
            
        }
        else if(eleje("FUTO",feher))
        {   printf("Futo\n");
            hol_van(sorok, oszlopok, chess_table, start, feher_babuk[2]);          

            if ((start[0] > finish[0]) && (start[1] > finish[1]))//balra fel 1
            {   //printf("benne1\n");
                for (int i = start[0] - 1, j = start[1] - 1 ; i >= finish[0], j >= finish[1]; i--, j--)
                {   
                    //printf("i: %d j: %d babu:%s ",i, j, chess_table[i][j]);
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("benne3\n");
                            return flag;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {   
                        //printf("benne4\n");

                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("benne5\n");
                            return flag;
                        }
                    }
                }
                return flag;

            }
            else if ((start[0] > finish[0]) && (start[1] < finish[1]))//jobbra fel 2
            {   //printf("benne2\n");

                for (int i = start[0] - 1, j = start[1] + 1 ; i >= finish[0], j <= finish[1]; i--, j++)
                {   
                    //printf("i: %d j: %d babu:%s\n ",i, j, chess_table[i][j]);
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("1.if felt1: %d felt2:%d\n",(finish[0] == i), (finish[1] == j) );
                            return flag;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {   
                        //printf("else\n");

                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("else_igaz\n");
                            return flag;
                        }
                    }
                }
                return flag;

            }
            else if ((start[0] < finish[0]) && (start[1] < finish[1]))//jobbra le 3
            {   //printf("benne3\n");
                for (int i = start[0] + 1, j = start[1] + 1 ; i <= finish[0], j <= finish[1]; i++, j++)
                {   
                    //printf("i: %d j: %d babu:%s ",i, j, chess_table[i][j]);
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("benne3\n");
                            return flag;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {   
                        //printf("benne4\n");

                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("benne5\n");
                            return flag;
                        }
                    }
                }
                return flag;

            }
            else if ((start[0] < finish[0]) && (start[1] > finish[1]))//balra le 4
            {   //printf("benne4\n");
                for (int i = start[0] + 1, j = start[1] - 1; i <= finish[0], j >= finish[1]; i++, j--)
                {   
                    //printf("i: %d j: %d babu:%s ",i, j, chess_table[i][j]);
                    if (benne_van_a_tomben(feher_babuk, chess_table[i][j]) || benne_van_a_tomben(fekete_babuk, chess_table[i][j]))
                    {   
                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("benne3\n");
                            return flag;
                        }
                        else
                        {
                            flag = 1;
                        }
                    }
                    else if (strcmp("----", chess_table[i][j]) == 0)
                    {   
                        //printf("else\n felt1: %d, felt2: %d\n",(finish[0] == i), (finish[1] == j));

                        if ((finish[0] == i) && (finish[1] == j))
                        {   
                            //printf("felt igaz\n");
                            return flag;
                        }
                    }
                }
                return flag;
            }
            
        }
        else if(eleje("KIRALY", feher))
        {   printf("Kiraly\n");
            int flag = 0;

            int uj_sor = 0;
            int uj_oszlop = 0;

            hol_van(sorok, oszlopok, matrix, start, feher_babuk[4]);

            //printf("kezdo:i: %d kezdo_j: %d\n", start[0], start[1]);
            //printf("finish0: %d, finish1: %d\n", finish[0], finish[1]);
            //puts("");

            int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
            int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

            // Check all 8 possible moves
            for (int i = 0; i < 8; i++) 
            {
                uj_sor = start[0] + dx[i];
                uj_oszlop = start[1] + dy[i];

                //printf("ujsor: %d ujoszlop:%d 1.felt %d 2.felt %d 3.felt: %d felt 4: %d\n", uj_sor, uj_oszlop, (uj_sor >= 0 && uj_sor < sorok), (uj_oszlop >= 0 && uj_oszlop < oszlopok), ((finish[0] == uj_sor) && (finish[1] == uj_oszlop)), (benne_van_a_tomben(feher_babuk, chess_table[finish[0]][finish[1]]) == 0));
                
                if ( (uj_sor >= 0 && (uj_sor < sorok)) && (uj_oszlop >= 0 && (uj_oszlop < oszlopok)) && ((finish[0] == uj_sor) && (finish[1] == uj_oszlop)) && (benne_van_a_tomben(feher_babuk, chess_table[finish[0]][finish[1]]) == 0) ) 
                {
                    //printf("jo sor: %d jo oszlop: %d\n",uj_sor, uj_oszlop);
                    return flag;
                }
            }
            //printf("Flag: %d\n", flag);
        }
        return 1;
        
    }
}

int main()
{

    fill_chess_table(sorok, oszlopok, chess_table);
    print_chess_table(sorok, oszlopok, chess_table);
    puts("");


    while(1)
    {   
        fgets(feher, MAX, stdin);
        feher[strlen(feher)-1] = '\0';

        to_upper(feher);
        cel(feher);

        puts("");

        //játékos 1 / fehér játékos 
        while( ! ( VegeOK(feher) && 
        (benne_van_a_tomben(feher_babuk, chess_table[finish[0]][finish[1]]) == 0 )&&
        (utkozes(sorok, oszlopok, chess_table,"feher") == 0)&&
        (
            ( eleje("BASTTYA",feher)  && strlen(feher)  == 12 ) || 
            ( eleje("LO",feher)       && strlen(feher) == 7 ) || 
            ( eleje("FUTO",feher)     && strlen(feher) == 9 ) || 
            ( eleje("KIRALY",feher)   && strlen(feher) == 11) ||
            ( eleje("KIRALYNO",feher) && strlen(feher)  == 13) 
        )
            ) 
            )
        {   
            printf("Rossz Lepes! Adj meg egy ujat\n");
            printf("finish0: %d, finish1: %d\n", finish[0], finish[1]);
            printf("start0. %d, start1:%d\n", start[0], start[1]);
            puts("");
            //printf("utkozes:%d\n",(utkozes(sorok, oszlopok, chess_table,"feher") == 0));
           
            fgets(feher, MAX, stdin);
            feher[strlen(feher)-1] = '\0';

            to_upper(feher);
            cel(feher);
        }
        
        if (eleje("BASTTYA",feher))
        {   
            //printf("BENT\n");
            //int result = utkozes(sorok, oszlopok, chess_table, finish[0], finish[1]);
            //printf("result = %d\n",result);
            basttya_lepes(sorok, oszlopok, chess_table, finish[0], finish[1], feher_babuk[0]);

        }
        else if (eleje("LO",feher))
        {
            lo_lepes(sorok, oszlopok, chess_table, finish[0], finish[1], feher_babuk[1]);
        }
        else if(eleje("FUTO",feher))
        {
            futo_lepes(sorok, oszlopok, chess_table, finish[0], finish[1], feher_babuk[2]);
        }
        else if(eleje("KIRALY", feher))
        {
            kiraly_lepes(sorok,oszlopok,chess_table,finish[0], finish[1],feher_babuk[4]);
        }

        print_chess_table(sorok, oszlopok, chess_table);
    }
}