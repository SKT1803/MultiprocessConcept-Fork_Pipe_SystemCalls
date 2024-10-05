#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


int isPrime(int num);
int checkPrime(int i, int num);
int nrDigits(int n);



int main(int argc, char* argv[]) {

    int fd[2];  // parent <--> P3, first pipe = to read from the parent
    int fd2[2]; // parent <--> P3, second pipe = to write to the parent
        
    if(pipe(fd) == -1 || pipe(fd2) == -1) { // checking if pipes are working
        printf("Error with pipe");
        return 1;
    }

    int id = fork(); // creating first child process P3

    if (id == -1) { // checking if fork is succesfully created
        perror("fork");
        return 1;
    }

    if (id == 0) { // Child process  P3, receives integers from the parent one by one, computes the number of primes received, send the count to the parent process. 2 pipes are used to communicate with the parent process.
    
        close(fd[1]); // Close write end of first pipe
        close(fd2[0]); // Close read end of second pipe     

        int x; // variable to write numbers on, numbers from the parent process
        int numberOfPrime = 0;
        while (read(fd[0], &x, sizeof(int)) > 0) { // reads the numbers from the parent process via pipe         
           numberOfPrime += isPrime(x); // check the number via isPrime method. If it is prime, number of primes increases.  
        }
 
        write(fd2[1], &numberOfPrime, sizeof(int)); // send the number of prime to parent process.
           
        close(fd[0]); // Close read end of first pipe
        close(fd2[1]); // Close write end of second pipe
 
    } else {  // I created the second child in here. Because I want it to be total 2 children. If I created the second child with the first one, there would be 3 children and one parent. 
    
        
          int fd3[2]; // parent <--> P2, first pipe = to read from the parent
    	  int fd4[2]; // parent <--> P2, second pipe = to write to the parent
    
     	  if(pipe(fd3) == -1 || pipe(fd4) == -1) { // checking if pipes are working
       	        printf("Error with pipe");
        	return 1;
            }
        
        
          int id2 = fork(); // creating the second child process = P2
          
          if (id2 == -1) { // checking if fork is succesfully created
               perror("fork");
               return 1;
             }
          
          if(id2 == 0 ){ // P2 = child process 2. This child process takes  integers from the parent one by one, categorizes each integers according to number of digits. After send the count information to parent process via pipe. 2 pipes are used to communicate with the parent process.
          	
          	close(fd3[1]); // Close write end of first pipe, parent <--> P2
          	close(fd4[0]); // Close read end of the second pipe, parent <--> P2
          	
          	close(fd[0]); // Close read end of first pipe, parent <--> P3
       		close(fd[1]); // Close write end of first pipe, parent <--> P3
        	close(fd2[1]); // Close write end of second pipe, parent <--> P3
        	close(fd2[0]); // Close read end of second pipe, parent <--> P3
          	
          	
          	int counts[5] = {0,0,0,0,0}; // array that store the digits count between 1 to 5. First I initialized the all digits counts to 0.
          	int z; // it stores numbers from the parent process     	         
        	while (read(fd3[0], &z, sizeof(int)) > 0) { // reads the numbers from the parent process via pipe
        	
          		int digits = nrDigits(z); // nrDigits method take the integer(z) and returns its digits number.
          		if(digits>=1 && digits <=5){ // I am checking the number of digits of the integer. In the question, the number of digits for the given integers can be between [1, 5]. So I am checking if returned number of digits in this range.  
          		 	counts[digits - 1]++; // This array represents the number of digits. But array starts from 0 index. So, [digits-1] gives the 0 index, that is for 1 digits. After determining the index, I am incrementing the corresponding value.(ex: number is 100, returned 3 digits --> counts[3-1]--> count[2] = number of 3 digits in the array.)
          		}          		     
       		    }
       		     		 
       		 write(fd4[1], counts, sizeof(counts)); // sending the digits count information to the parent process(P1) via using pipe. I am sending the counts array.
       		 close(fd4[1]); // close the writng pipe
       		  	 
          }else{ // P1 = Parent process. It reads integers from the txt file. Then send the integers one by one to the child processes (P1 and P2). After that, it takes the information from the child processes. Finally, prints the result out to the consol in desired format.
              
        close(fd[0]); // Close read end of first pipe, parent <--> P3
        close(fd2[1]); // Close write end of second pipe, parent <--> P3
        
        close(fd3[0]); // Close read end of first pipe, parent <--> P2
        close(fd4[1]); // Close write end of second pipe, parent <--> P2


        FILE *file;
        int num;
        
       // char fileName[55];
        //printf("Input file: ");
        //scanf("%s", fileName); // it takes file name as an input
        printf("Input file: %s\n", argv[1]);
        printf("\n");
        file = fopen(argv[1], "r");
        if(file == NULL) {
            perror("Error opening file");
            return 1;
        }
         
        int numberOfIntegers = 0; 
        while (fscanf(file, "%d,", &num) == 1) { // Read the integer values from the txt file
            write(fd[1], &num, sizeof(int)); // Write numbers to the P3 process using the first pipe between parent <--> P3
            numberOfIntegers++; // It counts the total number of integers in the txt file
            write(fd3[1], &num, sizeof(int)); // Write number to the P2 process using the first pipe between parent <--> P2
        }
        
        fclose(file);
        close(fd[1]); // Close write end of first pipe, parent <--> P3
        close(fd3[1]); // Close write end of first pipe, parent <--> P2
        
         int d_count[5]; // I am creating an array to store the number of digits counts after the read the values from the P2
         read(fd4[0], d_count, sizeof(d_count)); // Read the number of digits from the P2 using the second pipe, parent <--> P2
         close(fd4[0]); // Close read end of second pipe, parent <--> P2
         
         wait(NULL); // waitpid(id2, NULL,0); Waiting for child process to finish its execution.
        
        
        
        int nPrime; // After getting the value from P3 process via second pipe (parent <--> P3), I store the number of primes.
        read(fd2[0], &nPrime, sizeof(int)); // Read the number of primes from the P3 using the second pipe, parent <--> P3
        close(fd2[0]); // Close read end of second pipe, parent <--> P3
      

        wait(NULL); // waitpid(id, NULL,0); Waiting for child process to finis its execution.
        
        for(int i = 0; i<5; i++){ // Printing the output in desired format.
       	     printf("%d digits - %d\n", i+1, d_count[i]); // Prints the number of digits.
       	  }
        
        printf("Primes - %d\n", nPrime ); // Prints the number of primes.
        printf("Nonprimes - %d\n", numberOfIntegers- nPrime ); // Prints the number of non Primes. I counted the number of integers when I read the integers from txt file. I substract number of primes from this value to find number of nonprimes.
              
        } 
        
    }

    return 0;
}

int nrDigits(int num){ // It takes an integer as parameter and returns the number of digits of this integer. I check the 0 alone. For other numbers, to find the number of digits, I divide the number by 10 iteratively. Each iteration,I track the count. When the calculation is done, count is returned.
	
	if(num==0){
		return 1;
	}
	
	int count = 0;
	while(num != 0){
		num/=10;
		count++;
	 }
     return count;
}

int isPrime(int num){ // Computes the number of prime integers received. I used helper method, checkPrime(). Because 2 is a prime number and I can only take one parameter for this method (as instructed in HW1). So, I am sending 2 and the received number to the checkPrime() method. If its return 0, I increase the noPrime by 1. After get all the number, number of primes is
	
	int noPrime = 0; // Count for number of primes
	if(checkPrime(2, num) == 0){ // If its true, number is prime.
	  noPrime++; // Increasing the number of prime.
	}else{
	  noPrime = noPrime;
	}
	
	return noPrime;

}
int checkPrime(int i, int num){ // Helping method for isPrime(). I recursively check if numbers are prime or not. If the number is prime, return = 0, if not, return =1. 
	if(num<=1){
	   return 1; // 1 is not a prime n.
	}
	else if(num == i){ // Prime
	   return 0;
	}
	else if(num%i == 0){ // Not a prime
	   return 1;
	}
        else{
           return checkPrime(i+1, num); // Checking recursively
        }
}

