/*
Student Name: Connor Sparkman
Student NetID: cps260
Compiler Used: Linux GCC
Program description: Opens and reads answers and questions file. Compares them to each other and takes in answers, telling user whether or not they're correct.
*/
/* All of the includes for the project go here … 
 * those includes must support signals, interval timers,
 * read, open, and close. You will also need support for 
 * error numbers
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
/* Problem 1  */
#define BUF_SIZE 1024
static int timed;

/* Problem 2 */ 
int myPrint(const char *str)                    // Take a constant string and output it to stdout using write. Return 0 on failure and 1 on success.
{
if (write(1,str,strlen(str)) == 1 ){
      return 1;                                // Success
    }
else{
return 0;                                      // Failure
}
}

/* Problem 3 */ 
int myPrintInt(const int val)                  // Take a constant int and output it to stdout using write. Return 0 on failure and 1 on success.
{
char val1[BUF_SIZE];                          // create a char array to append the ints to, to be able to write
sprintf(val1, "%d", val);
if (write(1,val1,strlen(val1) == 1 )){
      return 1;                               // Success
    }
else{
return 0;                                     // Failure
}
}

/* Problem 4 */
void signalHandler(int sig)                  // Implement the signal handler to handle SIGINT and SIGALARM
{
int val1, val2;
char space[BUF_SIZE];

    if (sig == SIGINT){
        val1 = myPrint("\nExit: Are you sure? (Y/n) ");
    if (val2 == -1){
        perror("read");                     // If read error, perror
        exit(EXIT_FAILURE);
    }
    val2 = read(1, &space, BUF_SIZE);      // Once user has done ctrl c, read in next input to confirm or deny exit
    if (strcmp(space, "Y\n") == 0 || strcmp(space, "y\n") == 0){        // If user confirms, exit
        exit(EXIT_SUCCESS);
    }
    else {
        return;                            // Otherwise, go back to program
    }
}
    
    
    if (sig == SIGALRM){
    	timed = 1;
    }


}
/* This function reads a line of text from a file
 * we are simulating another functions behavior here ..
 * Return a 0 if no characters were read, return a 1 otherwise
 */
int readLine(int fd, char *line)
{
  strcpy(line, "\0");                    // Make line null
  char space[BUF_SIZE];
  int rdVal;                     

  while (1){                            // While 1. read into space character array
    rdVal = read(fd, &space, 1); 
    if (rdVal == 0){
      return 0;
    }
    if ((strcmp(space, "\n") == 0)){  // If the character is a newline, return 1, else add space character array to line character array
      return 1;
    }
    else{
      strcat(line, space);
    }}}
/* Problem 5 */



/* This function reads a question answer pairing
 * from the provided pair of file descriptors
 * It returns 0 if the files are empty
 * and 1 if if successfully reads the pairing
 */

int readQA(int questFd, int ansFd, char *quest, char *ans)
{
  if (readLine(questFd,quest) == 0) return 0; 
  if (readLine(ansFd, ans) == 0) return 0;
  return 1;
}

int main(int argc, char *argv[])
{

  int numRead = 0;
  int numWrite = 0;
  int question = 1;
  int correct = 0;
  char buf[BUF_SIZE];
  char quest[BUF_SIZE];
  char ans[BUF_SIZE];
  int questFd, ansFd;

 /* Problem 6a */
  struct sigaction hndlr;                             // signals and timers structures
  struct itimerval terminate;
  struct itimerval begin;

 /* Problem 6b */ 
  hndlr.sa_handler = signalHandler;                   // signal handlers
  sigemptyset(&hndlr.sa_mask);
  hndlr.sa_flags = 0;

 /* Problem 6c */
  begin.it_interval.tv_sec = 0;                      // 30 second timer
  begin.it_interval.tv_usec = 0;
  begin.it_value.tv_sec = 30;
  begin.it_value.tv_usec = 0;

 /* Problem 6d */
  terminate.it_interval.tv_sec = 0;                 // terminate timer 
  terminate.it_interval.tv_usec = 0; 
  terminate.it_value.tv_sec = 0;
  terminate.it_value.tv_usec = 0;

 /* Problem 7 */
  sigaction(SIGINT, &hndlr, NULL);                  // Sigaction for SIGINT and SIGALARM
  sigaction(SIGALRM, &hndlr, NULL);
  
 /* Problem 8 */
    ansFd = open("ans.txt", O_RDONLY);             // Open answers file. If error then perror
    if (ansFd == -1){
        perror("read"); 
        exit(EXIT_FAILURE);
    }
    
    questFd = open("quest.txt", O_RDONLY);        // Open questions file If error then perror
    if (questFd == -1){
        perror("read"); 
        exit(EXIT_FAILURE);
    }
    
    
    readQA(questFd, ansFd, quest, ans);          // read the first question, answer pairing prior to entering the loop
  while (1)
    {
      /* output the current question */
      myPrint("#");
      myPrintInt(question);
      myPrint(" ");
      myPrint(quest);
      myPrint("? ");
      /* we will set the timedout flag to 0, since it hasn't yet */
      timed = 0;

      /* now set the interval timer prior to reading in the user's response */
      int itimerVal = setitimer(ITIMER_REAL, &begin, NULL);
      
      /*  Problem 9 */

      numRead = read(1, buf, BUF_SIZE);         // read in user's response using read instead of scanf

  /* Problem 10 */


      /* test to see if the user responded and if an error has occurred 
       * an error can actually occur as part of the functionality of this program 
       * both SIGINT and SIGALRM will cause the read call to return a -1 
       * this can be recognized by testing for a specific error number ...  
       * The error is known as interrupted systems call 
       * Should this be the case, do the following .... 
       * If the read call was interrupted and the timedout flag isn't set 
       * just continue 
       * if the timedout flag was set, inform the user that time is up 
       * then go to the next question
       */
      if (numRead == 0) break;
      if (numRead == -1)
      {
	      if (errno == EINTR)
	      {
	        if (timed)
		 {
		    myPrint("\nTime's up, next question\n");
		    if (readQA(questFd, ansFd, quest, ans) == 0) break;
		    question++;
		 }
	        continue;
	      } 
	    perror("read");
	    exit(EXIT_FAILURE);
	 }
      /* disable the timer here */
/* Problem 11 */
      itimerVal = setitimer(ITIMER_REAL, &terminate, NULL);
      
      /* we will convert the buf being read in to a c-string by tacking on a 0 */
      buf[numRead-1] = 0;
      /* check the answer */
      if (strcmp(buf,ans) == 0)
	{
	   correct++;
	   myPrint("\ncorrect\n");
       } else
       {
	  myPrint(ans);
	  myPrint("\nwrong\n");
       }
      /* read the next question .. break if there are no more questions */
      if (readQA(questFd, ansFd, quest, ans) == 0) break;

      question++;
    }

  myPrint("final score is ");
  myPrintInt(correct);
  myPrint(" out of ");
  myPrintInt(question);

 /* Problem 12 */
close(questFd);
close(ansFd);

  return 0;
}
