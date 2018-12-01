#include "hdr/ircb.h"

int search(char * string, char buffer[BUFFERSIZE]){
    int stringlen = 0;
    int atend = 0;

    while(atend==0){
        if(string[stringlen] == '\0'){
            atend=1;
        }
        stringlen++;
    }
    stringlen = stringlen / sizeof(char);
    stringlen = stringlen-1;

    int stringn=0;
    int buffern=0;
    int valid=0;
    int check=0;
    int cn=0;
    int bufferlen = BUFFERSIZE;

    for(buffern=0;buffern<bufferlen; buffern++){
        stringn = 0;
        check = 0;
        cn = buffern;
        do {
            if(string[stringn]==buffer[cn]){
                check++;
                if(!((cn+2)>bufferlen)){
                cn++;
                }
            }
            else{

            }
            if(check==stringlen){
                valid=1;
            }
            stringn++;
        }while(stringn!=stringlen);
    }
    if (valid==1){
        return 1;
    }
    else {
        return 0;
    }
}
