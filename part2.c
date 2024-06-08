#include "slow_functions.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

// How nice of me to include a global that tells you how many commands there were :)
int total_commands = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_writer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reader = PTHREAD_COND_INITIALIZER;

// Shared index to keep track of the current command to write
int current_command = 0;

// Shared flag indicating if data has been written
int data_written = 0;

// ####################################################################################
// ## Please write some code in the following two functions

void * writer(void * in_ptr)
{
    char **commands = (char **)in_ptr;
    while (current_command < total_commands) {
        // Lock the mutex before writing
        pthread_mutex_lock(&mutex);

        // Wait until data has been read before writing new data
        while (data_written) {
            pthread_cond_wait(&cond_writer, &mutex);
        }

        // Call bad_write with the current command
        bad_write(commands[current_command]);
        current_command++;

        // Update the flag to indicate data has been written
        data_written = 1;

        // Signal the reader that data is available
        pthread_cond_signal(&cond_reader);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void * reader(void * empty)
{
	while (1) {
			// Lock the mutex before reading
			pthread_mutex_lock(&mutex);

			// Wait until there is data to read
			while (!data_written) {
				pthread_cond_wait(&cond_reader, &mutex);
			}

			// Call bad_read
			bad_read(NULL);

			// Update the flag to indicate data has been read
			data_written = 0;

			// Signal the writer that it can write new data
			pthread_cond_signal(&cond_writer);

			// Unlock the mutex
			pthread_mutex_unlock(&mutex);

			// Break the loop if all commands have been processed
			if (current_command >= total_commands) {
				break;
			}
		}
		return NULL;
}




int main()
{
	// ## SOME SPACE IN CASE YOU NEED TO INITIALISE VARIABLES



	// ################################################################################
	// ## DO NOT MODIFY BELOW HERE

	// ## Parse STDIN and read into commands
	char * commands[100];
	char line[256];
    while (fgets(line, 256, stdin))
    {
		commands[total_commands] = (char*)(malloc(strlen(line) * sizeof(char)));
 		strcpy(commands[total_commands], line);
		total_commands = total_commands + 1;
	}

	pthread_t write1;
	pthread_t read1;

	// Creates a thread which executes writer!
	if(pthread_create(&write1, NULL, writer, commands))
	{
		fprintf(stderr, "Error creating write thread\n");
		return 1;
	}

	// Creates a thread which executes reader!
	if(pthread_create(&read1, NULL, reader, NULL))
	{
		fprintf(stderr, "Error creating read thread\n");
		return 1;
	}

	// Ensure Threads Join Well
	if(pthread_join(write1, NULL)) 
	{
		fprintf(stderr, "Error joining write thread\n");
		return 2;
	}

	if(pthread_join(read1, NULL)) 
	{
		fprintf(stderr, "Error joining read thread\n");
		return 2;
	}

	// Did you really call the right functions?
	get_output();

	// I am a responsible memory user!
	int command_i;
	for (command_i = 0; command_i < total_commands; command_i++)
	{
		free(commands[command_i]);
	}
	return 0;
	// ## DO NOT MODIFY ABOVE HERE
	// ################################################################################
}

