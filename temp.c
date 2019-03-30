#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define MAX_LENGTH 300

int data[MAX_LENGTH][45];
int viTriTrung[45][45];
int soKy = 0;
void readDatabase(FILE * file);
void sapXep(int kyBatDau, int kyKetThuc, int bangSapXep[45][2]);
int main() {
	int i, j, k;
	k = 0;
	printf("so ky: %d", soKy);
	FILE *file = fopen("ky-1-250.txt", "r");
	readDatabase(file);
	int bangXuatHien[45][2];
	for (i = 0; i < 45; i++) {
		bangXuatHien[i][0] = 0;
		bangXuatHien[i][1] = 0;
		viTriTrung[i][0] = i + 1;
		viTriTrung[i][1] = 0;
	}

	sapXep(5,100,bangXuatHien);
	for (i = 0; i < 45; i++) {
		printf("\n %d xuat hien: %d lan",bangXuatHien[i][0],bangXuatHien[i][1]);
	}
	/*for(i=0;i<soKy;i++){
	    printf("\nky %d co kq:",i);
	    for(j=0;j<45;j++){
	        printf("%d ",data[i][j]);
	    }
	}*/
	for(i =0; i<20; i++) {
		k=229+i;
		sapXep(i,k,bangXuatHien);
		printf("\nhoc lan %d: ",i+1);
//		for (j = 0; j < 45; j++)
//			printf("\n  so %d xuat hien: %d lan",bangXuatHien[j][0],bangXuatHien[j][1]);
		printf("\n  ***********\n  ky %d trung tai cac vi tri:",230+i);
		for(j=0; j<45; j++) {
			if(data[230+i][j]==1) {
				viTriTrung[j][1]+=1;
				printf("%d ",j+1);
			}
		}
	}
//	// sap xep bang vi tri trung
	for(i=44; i>=0; i--) {
		for(j=1; j<=i; j++) {
			if(viTriTrung[j][1]>viTriTrung[j-1][1]) {
				int temp = viTriTrung[j][0];
				viTriTrung[j][0]=viTriTrung[j-1][0];
				viTriTrung[j-1][0]=temp;
				temp = viTriTrung[j][1];
				viTriTrung[j][1]=viTriTrung[j-1][1];
				viTriTrung[j-1][1]=temp;
			}
		}
	}



	printf("\n\n=============================================================\nkhau du doan:");
	printf("\n6 vi tri trung nhieu nhat:");
	for(i=0; i<6; i++) {
		printf("\nvi tri %d trung %d lan",viTriTrung[i][0],viTriTrung[i][1]);
	}
	printf("\n\n--------------------------");
	// sap xep lan 249
	sapXep(20,249,bangXuatHien);
	for (i = 0; i < 45; i++)
		printf("\nSo %d xuat hien: %d lan",bangXuatHien[i][0],bangXuatHien[i][1]);

	// tu bang 249 dua ra ket qua goi y
	printf("\n\n=============================================================");
	printf("\n\nTu bang 249 dua ra ket qua goi y ky 250:");
	for(i=0; i<6; i++) {
		j= viTriTrung[i][0];
		printf(" %d",bangXuatHien[j][0]);
	}
	getch();
	return 0;
}

void swapf(float* a,float* b) {
	float temp = *a;
	*a = *b;
	*b = temp;
}


void readDatabase(FILE * file) {
	char ch;
	int i;
	while (!feof(file)) {
		for ( i = 0; i < 45; i++)
			data[soKy][i] = 0;
		for ( i = 1; i <= 4; i++)
			fscanf(file, "%c", &ch);
		for ( i = 0; i < 6; i++) {
			fscanf(file, "%c", &ch);
			int temp = ch - 48;
			fscanf(file, "%c", &ch);
			data[soKy][temp * 10 + ch - 48 - 1] = 1;
			fscanf(file, "%c", &ch);
		}
		soKy++;
	}
	fclose(file);
}
void sapXep(int kyBatDau, int kyKetThuc, int bangSapXep[45][2]) {
	int i, j;
	for (i = 0; i < 45; i++) {
		bangSapXep[i][0] = i + 1;
		bangSapXep[i][1] = 0;
	}
	for ( i = kyBatDau; i < kyKetThuc; i++) {
		for ( j = 0; j < 45; j++) {
			bangSapXep[j][1] += data[i][j];
		}
	}
	for ( i = 44; i >= 0; i--) {
		for (j = 1; j <= i; j++) {
			if (bangSapXep[j - 1][1] < bangSapXep[j][1]) {
				int temp = bangSapXep[j][0];
				bangSapXep[j][0] = bangSapXep[j - 1][0];
				bangSapXep[j - 1][0] = temp;
				temp = bangSapXep[j][1];
				bangSapXep[j][1] = bangSapXep[j - 1][1];
				bangSapXep[j - 1][1] = temp;
			}
		}
	}
}
