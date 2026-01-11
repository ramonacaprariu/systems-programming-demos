#include <stdio.h>
#include <math.h>

//Function Definitions
void printAsBinary(unsigned short);
int getHeartRate(int, unsigned short*);
void prepareHeartRate(int, unsigned short*);
void presentHeartRate(unsigned short*);

/*********************************************************************/
/*Convert an unsigned short to an integer that resembles binary value*/
/**while abiding by the groupings of the 16-bit integer short model***/
/*********************************************************************/
void printAsBinary(unsigned short n) {
	for (int i=15; i>=0; i--) {
		if ((int)(n/pow(2,i)) > 0) {
			printf("1");
			n = n - pow(2,i);
		}
		else
			printf("0");
		//creating segmentations in the representative statement to mimic 
		//the groupings in the bit model
		if (i == 3 || i == 11 || i == 13 || i == 15) {
			printf(" ");
		}
	}
	printf("\n");
}

/*****************************************************************************/
/*Obtain heart rate data and check that the values are within the legal range*/
/*****************************************************************************/
int getHeartRate(int heartBPM, unsigned short *heartRate) {
	//illegal input check:
	if (heartBPM < 0 || heartBPM > 255) {
		*heartRate = (*heartRate | (1 << 12));
		if (heartBPM < 0) {
			*heartRate = (*heartRate | (1 << 11));
		}
		return 1;
	}
	//bit set
	*heartRate = *heartRate | heartBPM;
	//bit shift
	*heartRate = *heartRate << 3;
	//if legal then:
	return 0;
}

/**************************************************************/
/*Set status bits, parity bits, and abornormal heart rate bits*/
/*based on return value of getHeartRate function and heartBPM**/
/**************************************************************/
void prepareHeartRate(int sensorFunction, unsigned short *heartRate) {
	//check whether sensorFunction is illegal
	if (sensorFunction == 1) {
		//bit set: abnormality
		*heartRate = (*heartRate | (1 << 2));
		//bit set: status
		*heartRate = (*heartRate | (1 << 1));
		*heartRate = (*heartRate | (1 << 0));
		//bit set: parity
		*heartRate = (*heartRate | (1 << 15));
		return;
	}
	//heart rate sensor legal
	//abnormal heart rate check
	if ((*heartRate >> 3) < 50 || (*heartRate >> 3) > 165) {
		//low heart rate
		if ((*heartRate >> 3) < 50) {
			*heartRate = (*heartRate | (1 << 11));
			*heartRate = (*heartRate | (1 << 12));
		}
		*heartRate = (*heartRate | (1 << 2));
		*heartRate = (*heartRate | (1 << 12));
	//normal heart rate
	} else {
		// *heartRate = (*heartRate | (1 << 0));
		*heartRate = (*heartRate | (0 << 0));
	}
}


/*********************************************************/
/*Interpret the value of the bits to present data to user*/
/*********************************************************/
void presentHeartRate(unsigned short *heartRate) {
    printf("\n");

	printf("The output of the heart rate sensor is 0x%x = ", *heartRate);
	printAsBinary(*heartRate);

	//bit reads for status
	if (!(*heartRate & 1 << 0) >> 0) {
		printf("The value of bit 0 is 0, which indicates the device is correct reading \n");
		if (!(*heartRate & 1 << 1) >> 1) {
			printf("The value of bit 1 is 1, which indicates the device is required to reset \n");
		} else {
			printf("The value of bit 1 is 0, which indicates the device is not required to reset \n");
		}
		if ((*heartRate & 1 << 2) >> 2) {
			printf("The value of bit 2 is 1, which indicates the abnormal heart rate detached \n");
		} else {
			printf("The value of bit 2 is 0, which indicates the normal heart rate detached \n");
		}
	} else {
		if (((*heartRate & (1 << 1)) >> 1)) {
			printf("The value of bit 0 is 1, which indicates the device is incorrect reading \n");
			printf("The value of bit 1 is 1,  which indicates the sensor is required to reset \n");
		}
		if (((*heartRate & (1 << 2)) >> 2)) {
			printf("The value of bit 2 is 1, which indicates the abnormal heart rate detached \n");
		}
	}
	
	//bit read for heart heartBPM
	printf("The value of bits 3 to 10 is 0x%x, which indicates the heart rate is %d bpm \n",
	(*heartRate & ~0xf807) >> 3, (*heartRate & ~0xf807) >> 3);
	
	//bit read for heart rate abnormalities
	if (((*heartRate & 1 << 11) >> 11) && ((*heartRate & 1 << 12) >> 12)) {
		printf("The value of bits 11 to 12 is 3, which indicates a low heart rate \n");
	} else if (!((*heartRate & 1 << 11) >> 11) && ((*heartRate & 1 << 12) >> 12)) {
		printf("The value of bits 11 to 12 is 1, which indicates a high heart rate \n");
	} else {
		printf("The value of bits 11 to 12 is 0, which indicates a normal heart rate \n");
	}

	//bit read for parity
	if ((*heartRate & 1 << 15) >> 15) {
		printf("The value of bit 15 is 1, which indicates the illegal input \n");
	} else {
		printf("The value of bit 15 is 0, which indicates the normal input \n");
	}
	
}

void main() {
	unsigned short val = 0;
	int heartBPM, sensorFunction;
	//heartRate is defined as a pointer
	unsigned short *heartRate = &val;
	//unsigned short heartRate = 0;
	
	printf("Please provide a heartBPM reading: \n");
	scanf("%d", &heartBPM);
	
	//printAsBinary(heartRate);
	sensorFunction = getHeartRate(heartBPM, heartRate);
	prepareHeartRate(sensorFunction, heartRate);
	presentHeartRate(heartRate);
}
