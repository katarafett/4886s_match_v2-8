#define TORQUE 1
#define NORMAL 2
#define SPEED 6

int arrlen(int *arr);

int arrlen(int *arr) {
	// Count number of entries in array
	int i = 0;
	while (arr[i] != '\0') {
		i++;
	}
	return i;
}