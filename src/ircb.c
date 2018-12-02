#include "hdr/ircb.h"

int main(int argc, char ** argv)
{
  char * unick;
  unick = "unamehere";
  int verbose = 0;
  if (argc > 0) {
    for (int i = 1; i < argc; ++i){
      if (!strcmp(argv[i], "-v")){
          verbose = 1;
      }
      else if (strcmp(argv[i], "-v")){
          unick = argv[i];
      }
    }
  }

  char nick[32];
  int lasti=0;
  for(int i=0;i<32&&i<strlen(unick);i++){
    nick[i] = unick[i];
    ++lasti;
  }
  nick[lasti] = '\0';
  for(int i=lasti;i<32;i++){
    nick[i]='\0';
  }

  int connect = openConnection(IRCSERVER);
	if (connect == -1){
			return 0;
	}

	char buffer[BUFFERSIZE] = {0};
  int joined = 0;
  int inchannel = 0;
  int sendgreeting = 0;
  int chanop = 1;
  int authing = 0;
  int authcnt = 0;
  int jncnt = 0;
  int printone = 0;
  char smessage[1024];
  char * message;
  printf("Opening socket to %s\n",IRCSERVER);
  if (!joined){
    printf("Connecting...\n");
  }
  while(1){
    for(int i = 0;i<BUFFERSIZE;i++){
      buffer[i] = '\0';
    }
    //send(sock , hello , strlen(hello) , 0 );
    valread = read( sock , buffer, BUFFERSIZE);
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
          snprintf( smessage, sizeof( smessage ), "%s%s%s%s%s%s%s", "NICK ", nick, "\r\n USER ", nick, " 0 * :", nick, "\r\n");
          send(sock , smessage , strlen(smessage) , 0 );
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
      snprintf( svalue, sizeof( svalue ), "%s%s%s", "001 ", nick, " :");
      if(search(svalue,buffer)){
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
    snprintf( svalue, sizeof( svalue ), "%s%s%s", "PRIVMSG ", nick, " :auth");
    if(authing || search(svalue,buffer)){
      authing = 1;
      if(authing){
          snprintf( svalue, sizeof( svalue ), "%s%s%s", "PRIVMSG ", nick, " :otheruser password");
          if(search(svalue,buffer)){
            message="MODE #channel +o :otheruser\r\n";
            send(sock , message , strlen(message) , 0 );
            printf("Authed otheruser!\n");
            authing = 0;
          }
          snprintf( svalue, sizeof( svalue ), "%s%s%s", "PRIVMSG ", nick, " :otheruser2 password");
          if(search(svalue,buffer)){
            message="MODE #channel +o :otheruser2\r\n";
            send(sock , message , strlen(message) , 0 );
            printf("Authed otheruser2!\n");
            authing = 0;
          }
          snprintf( svalue, sizeof( svalue ), "%s%s%s", "PRIVMSG ", nick, " :otheruser3 password");
          if(search(svalue,buffer)){
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
