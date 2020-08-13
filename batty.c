#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PATH "/sys/class/power_supply/BAT0/capacity"
#define CAPACITY 100

int DEBUG = 0;

void *update_battery_status(void *arg) 
{
	int *battery = (int *)arg;
	FILE *file;
	if ((file = fopen(PATH, "r")) == NULL)
	{
		printf("Error opening file: %s\n", PATH);
	}
	fscanf(file, "%d", battery);
	if (DEBUG == 1) printf("\rCurrent battery status: %d\n", *battery);
	fclose(file);
	sleep(2);
}

void print_battery(int battery) 
{
	int battery_norm = 1;
	int section_size = CAPACITY / 3;
	printf("\r");
	for (int i = 0; i < battery / battery_norm; i++) 
	{
		if (i < section_size) printf("\033[0;31m");
		else if (i < section_size * 2) printf("\033[0;33m");
		else printf("\033[0;32m");
		printf("[");
	}
	printf("\033[0;34m");
	for (int i = 0; i < CAPACITY / battery_norm - battery / battery_norm; i++) 
	{
		printf("-");
	}
	printf("\033[0;0m");
	printf("%d%%", battery);
	fflush(stdout);
}

int main(int argc, char *argv[])
{
	int battery;
	if (argc > 1 && strcmp(argv[1], "-d") == 0)
	{
	       	DEBUG = 1;
	}
	while(1)
	{
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, update_battery_status, &battery);
		pthread_join(thread_id, NULL);
		print_battery(battery);
	}
	exit(0);
}
