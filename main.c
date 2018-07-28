#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <asm/stat.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>
#define PORT 6665


int search(char * string, char buffer[1024]){
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
    int bufferlen = 1024;

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

int main(int argc, char const ** argv)
{
    char * nick;
    nick = "rootcake";
    int verbose = 0;
    int none = 0;
    if (argc > 0) {
        for (int i = 0; ++i < argc; ++i){
            if (!strcmp(argv[i], "-v")){
                verbose = 1;
                none = 0;
            }
            else if (strcmp(argv[1], "-v")){
                nick = argv[1];
            }
            else{
                if (!verbose){
                    none = 1;
                }
            }
        }
    }
    if (argc > 0) {
        for (int i = 0; ++i < argc; ++i){
            if (!strcmp(argv[i], "-v")){
                verbose = 1;
                none = 0;
            }
            else{
                if (!verbose){
                    none = 1;
                }
            }
        }
    }

    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    //char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    char ip[100];

    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( "irc.freenode.net" ) ) == NULL)
    {
       // get the host info
       herror("gethostbyname");
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
       //Return the first one;
       strcpy(ip , inet_ntoa(*addr_list[i]) );
    }



    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip , &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    int joined = 0;
    int inchannel = 0;
    int sendgreeting = 0;
    int chanop = 1;
    int authing = 0;
    int authcnt = 0;
    int jncnt = 0;
    int printone = 0;
    char * message;
    printf("Opening socket to %s\n",ip);
    if (!joined){
	    printf("Connecting...\n");
    }
   while(1){
       char buffer[] = {0};
        //send(sock , hello , strlen(hello) , 0 );
        valread = read( sock , buffer, 1024);
	if (verbose || printone == 0){
		printf("%s",buffer );
		printone = 1;
	}
	if(search("ERROR :Closing Link: subnet mask here",buffer)){
		printf("Server disconnected.\n");
		printf("Quitting client.\n");
		return 0;
	}
        if(joined == 0){
		if(jncnt == 0){
        		if(search("Checking Ident", buffer)){
                		 message="NICK username here\r\n USER username here 0 * :username here\r\n";
        		  	 send(sock , message , strlen(message) , 0 );
				 printf("Providing credentials.\n");
				 ++jncnt;
			}
		}
		else{
			++jncnt;
			if(jncnt>=5){
				jncnt = 0;
			}
		}

        }
        if(inchannel == 0){
            if(search("Thank you for using freenode!",buffer)){
                message="JOIN :#channel\r\n";
                send(sock , message , strlen(message) , 0 );
                joined = 1;
		inchannel = 1;
                printf("Joined #channel!\n");
            }
        }
        if((inchannel == 1) && (sendgreeting == 0)){
            sendgreeting = 1;
            message="PRIVMSG #channel :At ease men.\r\n";
            send(sock , message , strlen(message) , 0 );
            printf("Sent greeting!\n");
        }
        if(search("PING :",buffer)){
             message="PONG :Just replying!\r\n";
             send(sock , message , strlen(message) , 0 );
        }
        if(authing || search("PRIVMSG username here :auth",buffer)){
            authing  = 1;
            if(authing){
                if(search("PRIVMSG username here :otheruser password",buffer)){
                    message="MODE #channel +o :otheruser\r\n";
                    send(sock , message , strlen(message) , 0 );
                    printf("Authed otheruser!\n");
                    authing = 0;
                }
                else if(search("username here :otheruser2 password",buffer)){
                    message="MODE #channel +o :otheruser2\r\n";
                    send(sock , message , strlen(message) , 0 );
                    printf("Authed otheruser2!\n");
                    authing = 0;

                }
                else if(search("username here :otheruser3 password",buffer)){
                    message="MODE #channel +o :otheruser3\r\n";
                    send(sock , message , strlen(message) , 0 );
                    printf("Authed otheruser3!\n");
                    authing = 0;
                }
                ++authcnt;
                if(authcnt == 10){
                    authcnt = 0;
                    authing = 0;
                }
            }
        }
        fflush(stdout);
   }
    return 0;
}
