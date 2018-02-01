//============================================================================
// Name        : bbg_filehandling.cpp
// Author      : Abhinav Saxena
// Version     :
// Copyright   : Your copyright notice
// Description : Writing data into files in proper data format
//============================================================================

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

//using namespace std;

char* deblank(char* input) {
	int i, j;
	char *output = input;
	for (i = 0, j = 0; i < strlen(input); i++, j++) {
		if (input[i] != ' ')
			output[j] = input[i];
		else
			j--;
	}
	output[j] = 0;
	return output;
}

int file_write(char *data, char *filename) {
	FILE *fp1;
	FILE *fp;
	char* datetime1;
	char datetime[50] = "";
	char path[100] = "";

	static const char ble[] = "ble";
	static const char bledevdata[] = "bledevdata";
	static const char bledevlist[] = "bledevlist";
	static const char zigbee[] = "zigbee";
	static const char zigdevdata[] = "zigdevdata";
	static const char zigdevlist[] = "zigdevlist";
	static const char zwave[] = "zwave";
	static const char zdevdata[] = "zdevdata";
	static const char zdevlist[] = "zdevlist";
	static const char wifiauth[] = "wifiauth";
	static const char wifierror[] = "wifierror";
	static const char devlist[] = "devlist";
	static const char internet[] = "internet";
	static const char loclist[] = "loclist";
	static const char main[] = "main";
	static const char sys[] = "sys";
	static const char user[] = "user";
	static const char usrid[] = "usrid";

	//static const char prepath[]="/media/";
	static const char rootpath[] = "/media/root_bbg/";
	static const char blepath[] = "/media/root_bbg/bbg_ble/";
	static const char wifipath[] = "/media/root_bbg/bbg_wifi/";
	static const char zigbeepath[] = "/media/root_bbg/bbg_zigbee/";
	static const char zwavepath[] = "/media/root_bbg/bbg_zwave/";

	static const char postpath[] = ".log";

	/* Open the command for reading. */
	fp1 = popen("/bin/date +%d%m%Y%H%M%S", "r");
	if (fp1 == NULL) {
		printf("Error : Failed to run command\n");
		return 1;
	}

	/* Read the output a line at a time - output it. */
	while (fgets(datetime, sizeof(datetime) - 1, fp1) != NULL) {
		//printf("%s", datetime);
	}
	/* close */
	pclose(fp1);

	if ((strcmp(filename, devlist) == 0) || (strcmp(filename, internet) == 0)
			|| (strcmp(filename, loclist) == 0) || (strcmp(filename, main) == 0)
			|| (strcmp(filename, sys) == 0) || (strcmp(filename, user) == 0)
			|| (strcmp(filename, usrid) == 0)) {
		printf("in root_bbg folder\n");

		strcat(path, rootpath);
		strcat(path, filename);
		strcat(path, postpath);
	}

	else if ((strcmp(filename, ble) == 0) || (strcmp(filename, bledevlist) == 0)
			|| (strcmp(filename, bledevdata) == 0)) {
		printf("in bbg_ble folder\n");

		strcat(path, blepath);
		strcat(path, filename);
		strcat(path, postpath);
	}

	else if ((strcmp(filename, zwave) == 0) || (strcmp(filename, zdevlist) == 0)
			|| (strcmp(filename, zdevdata) == 0)) {
		printf("in bbg_zwave folder\n");

		strcat(path, zwavepath);
		strcat(path, filename);
		strcat(path, postpath);

	} else if ((strcmp(filename, zigbee) == 0)
			|| (strcmp(filename, zigdevlist) == 0)
			|| (strcmp(filename, zigdevdata) == 0)) {
		printf("in bbg_zigbee folder\n");

		strcat(path, zigbeepath);
		strcat(path, filename);
		strcat(path, postpath);

	} else if ((strcmp(filename, wifiauth) == 0)
			|| (strcmp(filename, wifierror) == 0)) {
		printf("in bbg_wifi folder\n");

		strcat(path, wifipath);
		strcat(path, filename);
		strcat(path, postpath);

	}

	printf("path = %s\n", path);
	printf("Opening file\n");

	fp = fopen(path, "a+");

	if (fp == NULL) {
		printf("Error : Could not open file\n");
		return 1;
	}

	printf("Success : Writing data to file\n");

	// Filtering data, removing extra space and extra \n newline character
	datetime1 = deblank(datetime);
	datetime1[strcspn(datetime1, "\r\n")] = 0;

	// SOL
	fputs("$", fp);
	fputs(",", fp);

	// PAYLOAD
	fputs(datetime1, fp);
	fputs(",", fp);
	fputs(data, fp);

	// EOL
	fputs(",", fp);
	fputs("#", fp);
	fputs("\n", fp);

	printf("Success : Closing the file \n");

	fclose(fp);
	return 0;
}
