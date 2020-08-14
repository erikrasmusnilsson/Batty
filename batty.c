#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define CONFIG_FILE "/usr/share/batty/batty.conf"
#define BANNER_FILE "/usr/share/batty/hdr.txt"
#define CAPACITY 100

char *BATTERY_FILE = "/sys/class/power_supply/BAT0/capacity";
int BATTERY_THICKNESS = 5;
int POLL_TIME = 30;
int DEBUG = 0;

void update_battery_status(int *battery) 
{
	FILE *file;
	if ((file = fopen(BATTERY_FILE, "r")) == NULL)
	{
		printf("Error opening file: %s\n", BATTERY_FILE);
		return;
	}
	fscanf(file, "%d", battery);
	fclose(file);
}

void *thread_job(void *arg) 
{
	int *battery = (int *) arg;
	update_battery_status(battery);
	sleep(POLL_TIME);
}

void print_battery(int battery) 
{
	int battery_norm = 2;
	int section_size = CAPACITY / battery_norm  / 4;
	printf("\r\033[%dA", BATTERY_THICKNESS);
	for (int j = 0; j < BATTERY_THICKNESS; j++)
	{
		for (int i = 0; i < battery / battery_norm; i++) 
		{
			if (i < section_size) printf("\033[0;31m");
			else if (i < section_size * 2) printf("\033[0;33m");
			else printf("\033[0;32m");
			printf("\u2588");
		}
		printf("\033[0;34m");
		for (int i = 0; i < CAPACITY / battery_norm - battery / battery_norm; i++) 
		{
			printf("-");
		}
		printf("\033[0;0m");
		printf("\n");
	}
	printf("Current charge: %d%%", battery);
	fflush(stdout);
}

void print_header() 
{
	FILE *file;
	char buffer[128];

	if ((file = fopen(BANNER_FILE, "r")) == NULL) 
	{
		printf("Ouch mah doo...\n");
		return;
	}
	printf("\033[0;35m");
	while (fgets(buffer, 128, file) != NULL) printf("%s", buffer);
	fclose(file);
	printf("\033[0;0m");
}


int main(int argc, char *argv[])
{
	int battery;
	FILE *config_file;
	if ((config_file = fopen(CONFIG_FILE, "r")) == NULL) 
	{
		printf("Could not open config file at %s, proceeding with default values.\n", CONFIG_FILE);
	} else 
	{
		fscanf(config_file, "%*s %d", &BATTERY_THICKNESS);
		fscanf(config_file, "%*s %d", &POLL_TIME);
	}
	if (argc > 1 && strcmp(argv[1], "-d") == 0)
	{
	       	DEBUG = 1;
	}

	printf("\033[9999;1H");
	print_header();
	// TODO: write this cleaner
	for (int i = 0; i < BATTERY_THICKNESS + 1; i++) printf("\n");
	update_battery_status(&battery);
	print_battery(battery);
	while(1)
	{
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, thread_job, &battery);
		pthread_join(thread_id, NULL);
		print_battery(battery);
	}
	exit(0);
}
