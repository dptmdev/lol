 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#define MAXSIZE     128 
 
void die(char *s) 
{ 
  perror(s); 
  exit(1); 
} 
 
typedef struct msgbuf 
{ 
    long    mtype; /* message type, must be > 0 */ 
    char    mtext[MAXSIZE]; 
}; 
 
int main(void) 
{ 
    int msqid; 
    int msgflg = IPC_CREAT | 0666; 
    key_t key; 
    struct msgbuf sbuf; 
    size_t buflen; 
 
    key = 1234; 
 
    if ((msqid = msgget(key, msgflg )) < 0)   //Get the message queue ID for 
//the given key 
      die("msgget"); 
 int msz_count = 0;
 int i = 0;
 printf("Enter the no of message: ");
    //Message Type 
    sbuf.mtype = 1;
    scanf("%d", &msz_count);
    // meta data 
    scanf("%[^\n]", &sbuf.mtext);
    getchar();
    //sbuf.mtext = msz_count;
 
    buflen = strlen(sbuf.mtext) + 1 ; 
    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) 
    { 
        printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buflen); 
        die("msgsnd"); 
    } 
 
    else 
        printf("meta deta Message Sent\n");
     
    
 while(i < msz_count){
    printf("Enter a message to add to message queue : "); 
    scanf("%[^\n]",sbuf.mtext); 
    getchar(); 
 
    buflen = strlen(sbuf.mtext) + 1 ; 
 
    if (msgsnd(msqid, &sbuf, buflen, IPC_NOWAIT) < 0) 
    { 
        printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buflen); 
        die("msgsnd"); 
    } 
 
    else {
        printf("Message Sent\n");
	i++;
	}
    } 

    exit(0); 
} 