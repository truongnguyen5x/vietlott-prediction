#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 300

//cấu trúc chứa luật kết hợp
typedef struct {
	int x[6], y[6];   	//X->Y
	int n1, n2;			//số lượng của X và Y
} luatKetHop;

float minSup, minConf;
int data[MAX_LENGTH][45], soKy = 0, input, a[10];

//đọc cở sở dữ liệu
void readDatabase(FILE * file);
//tìm L1
int **Large1ItemSets(int *n, int ***tapBiLoai);
//tìm Ck
int **timUngCuVien(int **L, int soTapPhoBien, int *soUCV, int **tapBiLoai, int soBiLoai, int k);
//tìm tập Lk
int **timTapPhoBien(int **C, int *soTapPhoBien, int soUCV, int ***tapBiLoai, int *soBiLoai, int k);
//hiển thị 1 mảng 2 chiều
void show(int **array, int n, int k);
//dùng backtracking để tìm các tập con của 1 tập hợp
void timTapCon(int *array, int n, int *soLuong, int*** tapCon, int u);
//tạo và lựa chọn các luật kết hợp
luatKetHop* timLuatKetHop(int* array, int n, int* soLuong);
luatKetHop* chonLuatKetHop(luatKetHop* array, int *soLuatKetHop);
//ghép các luật kết hợp đã tìm đc thành 1 mảng
luatKetHop* ghepCacLuatKetHop(int ***array, int k, int*soTapPhoBien, int *n);
//tìm bao đóng
int* timKetQua(luatKetHop *array, int soLuong);


int main() {
	int **C, **tapBiLoai, ***L, *ketQua;
	L = malloc(7 * sizeof(int**));
	int k = 2, soTapPhoBien[7], soUCV, soBiLoai, soLgLuatKetHop;
	FILE *file = fopen("ky-1-250.txt", "r");
	readDatabase(file);
	printf("Chuong trinh du doan ket qua Vietlott 6/45\n");
	printf("Ban hay nhap minSupport va minConfident:( >=0 va <=100)\n");
	printf("Khuyen khich: minSup tu 1.4 tro len, minConf tu 40 tro len\n");
	scanf("%f%f", &minSup, &minConf);
	minConf/=100;
	printf("Cac tap pho bien la: \n");

	//apriori
	L[1] = Large1ItemSets(&soTapPhoBien[1], &tapBiLoai);
	soBiLoai = 45 - soTapPhoBien[1];
	while (soTapPhoBien[k - 1] != 0 && k != 7) {
		C = timUngCuVien(L[k - 1], soTapPhoBien[k - 1], &soUCV, tapBiLoai, soBiLoai, k - 1);
		L[k] = timTapPhoBien(C, &soTapPhoBien[k], soUCV, &tapBiLoai, &soBiLoai, k);
		k++;
	}

	//tìm các luật kết hợp từ các tập phổ biến
	luatKetHop* luatKH = ghepCacLuatKetHop(L, k - 2, soTapPhoBien, &soLgLuatKetHop);
	printf("Moi ban nhap 1 so tu 1->45: \n"); scanf("%d", &input);
	ketQua = timKetQua(luatKH, soLgLuatKetHop);

	//in kết quả
	if (ketQua[0]*ketQua[1]*ketQua[2]*ketQua[3]*ketQua[4]*ketQua[5] != 0) {
		printf("Chuong trinh du doan ket qua Vietlott la:\n");
		for (int i = 0; i < 6; i++)printf("%d ", ketQua[i] );
	} else printf("Khong the du doan ket qua Vietlott, hay giam minSup va minConf hoac chon 1 so khac tu 1=>45\n");

}
/**
  *
  *@param 
  */
void readDatabase(FILE * file) {
	char ch;
	int i;
	while (!feof(file)) {
		for (i = 0; i < 45; i++)data[soKy][i] = 0;
		for (i = 1; i <= 4; i++)fscanf(file, "%c", &ch);
		for (i = 0; i < 6; i++) {
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

int **Large1ItemSets(int *n, int ***tapBiLoai) {
	int **C, soBiLoai, i, j;
	C = malloc(45 * sizeof(int*));
	for (i = 0; i < 45; ++i) {
		C[i] = malloc(6 * sizeof(int));
		C[i][0] = i + 1;
		for (j = 1; j < 6; j++)C[i][j] = 0;
	}
	return	timTapPhoBien(C, n, 45, tapBiLoai, &soBiLoai, 1);
}

int **timTapPhoBien(int **C, int *soTapPhoBien, int soUCV, int ***tapBiLoai, int *soBiLoai, int k) {
	int **ketQua, *count, i, j, m, flag, MinSup ;
	count = malloc(sizeof(int) * soUCV);
	MinSup = (int)(minSup * soKy / 100.0);
	*soTapPhoBien = 0;
	for (i = 0; i < soUCV; ++i) count[i] = 0;
	for (i = 0; i < soUCV; ++i) {
		for (j = 0; j < soKy; j++) {
			flag = 1;
			for (m = 0; m < k; m++)	if (data[j][C[i][m] - 1] == 0)flag = 0;
			if (flag) count[i]++;
		}
	}
	for (i = 0; i < soUCV; i++) {
		if (count[i] >= MinSup) (*soTapPhoBien)++;
	}
	ketQua = malloc(*soTapPhoBien * sizeof(int*));
	*soBiLoai = soUCV - *soTapPhoBien;
	*tapBiLoai = malloc((*soBiLoai) * sizeof(int*));
	j = 0, m = 0;
	for (i = 0; i < soUCV; i++) {
		if (count[i] >= MinSup) ketQua[j++] = C[i];
		else (*tapBiLoai)[m++] = C[i];
	}
	if (k >= 2) {
		printf("\nKet qua tai k= %d", k);
		show(ketQua, *soTapPhoBien, k);
	}
	return ketQua;
}

int **timUngCuVien(int **L, int soTapPhoBien, int *soUCV, int **tapBiLoai, int soBiLoai, int k) {
	int temp1[6], temp2[6], temp3[46], blackList[46], **ketQua, arraySize = 100, i, j, n, m, p, count, flag;
	*soUCV = 0;
	ketQua = malloc(arraySize * sizeof(int*));
	for (i = 0; i < soTapPhoBien; i++) {
		for (j = 0; j < 46; j++)blackList[j] = 0;
		for (j = 0; j < 46; j++)temp3[j] = 0;
		for (j = 0; j < soTapPhoBien; j++) {
			count = 0;
			for (n = 0; n < 6; n++)temp1[n] = 0;
			for (n = 0; n < 6; n++)temp2[n] = 0;
			for (n = 0; n < k; n++)	for (m = 0; m < k; m++)	if (L[i][n] == L[j][m]) {
						temp1[n] = 1;
						temp2[m] = 1;
						count++;
					}
			if (count == k - 1) {
				n = 0, m = 0;
				while (temp1[n] == 1)n++;
				while (temp2[m] == 1)m++;
				if (L[i][n] > L[j][m]) blackList[L[j][m]] = 1;
			}
			for (n = 0; n < k ; n++)temp3[L[j][n]] = 1;
		}
		for (j = 1; j <= 45; j++)if (blackList[j] == 1)temp3[j] = 0;
		for (j = 0; j < k; j++) temp3[L[i][j]] = 0;
		for (j = 1; j < 46; j++) {
			if (temp3[j] == 1) {
				flag = 0;
				ketQua[*soUCV] = malloc(6 * sizeof(int));
				for (n = k + 1; n < 6; n++)ketQua[*soUCV][n] = 0;
				for (n = 0; n < k; n++)ketQua[*soUCV][n] = L[i][n];
				ketQua[*soUCV][k] = j;
				for (n = 0; n < soBiLoai; n++) {
					count = 0;
					for (m = 0; m <= k; m++) {
						for (p = 0; p < k; p++) {
							if (ketQua[*soUCV][m] == tapBiLoai[n][p])count++;
						}
					}
					if (count == k) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					for (n = k; n > 0; n--) {
						for (m = 0; m < n; m++) {
							if (ketQua[*soUCV][m + 1] < ketQua[*soUCV][m]) {
								p = ketQua[*soUCV][m + 1];
								ketQua[*soUCV][m + 1] = ketQua[*soUCV][m];
								ketQua[*soUCV][m] = p;
							}
						}
					}
					(*soUCV)++;
					if ((*soUCV ) > arraySize - 1) {
						arraySize += 100;
						ketQua = realloc(ketQua, arraySize * sizeof(int*));
					}
				} else free(ketQua[*soUCV]);
			}
		}
	}
	return ketQua;
}

void show(int **array, int n, int k) {
	printf(" co %d ket qua\n", n );
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++)printf("%2d ", array[i][j] );
		printf("\n");
	}
}

void timTapCon(int *array, int n, int *soLuong, int*** tapCon, int u) {
	int i, count;
	if (u == n + 1) {
		(*soLuong)++;
		*tapCon = realloc(*tapCon, (*soLuong) * sizeof(int*));
		(*tapCon)[(*soLuong) - 1] = malloc(6 * sizeof(int));
		count = 0;
		for (i = 1; i <= n; i++)if (a[i]) (*tapCon)[(*soLuong) - 1][count++] = array[i - 1];
		for (; count <= 5 ; count++)(*tapCon)[(*soLuong) - 1][count] = 0;
		return;
	}
	a[u] = 0;
	timTapCon(array, n, soLuong, tapCon, u + 1);
	a[u] = 1;
	timTapCon(array, n, soLuong, tapCon, u + 1);
}

luatKetHop* timLuatKetHop(int* array, int n, int* soLuong) {
	int **tapCon = NULL, i, j, k, m, count, flag;
	*soLuong = 0;
	timTapCon(array, n, soLuong, &tapCon, 1);
	luatKetHop *ketQua = malloc((*soLuong - 2) * sizeof(luatKetHop));
	for (i = 0; i < *soLuong - 2; i++) {
		j = 0;
		while (tapCon[i + 1][j] != 0)j++;
		for (m = 0; m < j; m++)ketQua[i].x[m] = tapCon[i + 1][m];
		ketQua[i].n1 = j;
		count = 0;
		for (m = 0; m < n; m++) {
			flag = 0;
			for (k = 0; k < j; k++)if (array[m] == ketQua[i].x[k]) flag = 1;
			if (flag == 0)ketQua[i].y[count++] = array[m];
		}
		ketQua[i].n2 = n - j;
	}
	(*soLuong) -= 2;
	return ketQua;
}

luatKetHop* chonLuatKetHop(luatKetHop * array, int *soLuatKetHop) {
	int i, j, n, nXY = 0, flag, count;
	float *nX;
	luatKetHop * ketQua;
	nX = malloc((*soLuatKetHop) * sizeof(float));
	for (i = 0; i < *soLuatKetHop; i++) {
		nX[i] = 0.0;
		for (j = 0; j < soKy; j++) {
			flag = 0;
			for (n = 0; n < array[i].n1; n++)if (data[j][array[i].x[n] - 1] == 0)flag = 1;
			if (flag == 0)nX[i] += 1.0;
		}
	}
	for (i = 0; i < soKy; i++) {
		flag = 0;
		for (n = 0; n < array[0].n1; n++)if (data[i][array[0].x[n] - 1] == 0)flag = 1;
		for (n = 0; n < array[0].n2; n++)if (data[i][array[0].y[n] - 1] == 0)flag = 1;
		if (flag == 0)nXY++;
	}
	for (i = 0; i < *soLuatKetHop; i++)nX[i] = nXY / nX[i];
	count = 0;
	for (i = 0; i < *soLuatKetHop; i++)if (nX[i] > minConf)count++;
	ketQua = malloc(count * sizeof(luatKetHop));
	count = 0;
	for (i = 0; i < *soLuatKetHop; i++)if (nX[i] > minConf)ketQua[count++] = array[i];  //cần giải phóng bộ nhớ
	*soLuatKetHop = count;
	return ketQua;
}

luatKetHop* ghepCacLuatKetHop(int ***array, int k, int*soTapPhoBien, int *n) {
	int count = 0, i, j, m, soLuong;
	luatKetHop *ketQua = NULL, *temp;
	for (i = 2; i <= k; i++) {
		for (j = 0; j < soTapPhoBien[i]; j++) {
			soLuong = 0;
			temp = timLuatKetHop(array[i][j], i, &soLuong);
			temp = chonLuatKetHop(temp, &soLuong);
			ketQua = realloc(ketQua, (count + soLuong) * sizeof(luatKetHop));
			for (m = 0; m < soLuong; m++)ketQua[count++] = temp[m];
		}
	}
	*n = count;
	printf("\n");
	printf("so luong cac luat ket hop %d\n", count );
	for (i = 0; i < count; i++) {
		printf("%d:  ", i );

		for (int f = 0; f < ketQua[i].n1; f++)printf("%2d ", ketQua[i].x[f] );
		printf(" => ");
		for (int f = 0; f < ketQua[i].n2; f++)printf("%2d ", ketQua[i].y[f] );
		printf("\n");
	}
	return ketQua;
}

int* timKetQua(luatKetHop *array, int soLuong) {
	int i, *ketQua, count = 1, **tapCon, solgTapCon, j, k, flag, m, koTimThay = 0, n;
	ketQua = malloc(6 * sizeof(int));
	for (i = 0; i < 6; i++)ketQua[i] = 0;
	ketQua[0] = input;
	while (count != 6 && koTimThay == 0) {
		koTimThay = 1;
		solgTapCon = 0;
		tapCon = NULL;
		timTapCon(ketQua, count, &solgTapCon, &tapCon, 1);
		for (i = 1; i < solgTapCon && koTimThay == 1; i++) {
			//trong moi tap con
			n = 0;
			while (tapCon[i][n] != 0)n++;
			for (j = 0; j < soLuong && koTimThay == 1; j++) {
				flag = 0;
				for (k = 0; k < array[j].n1; k++)for (m = 0; m < n; m++)if (array[j].x[k] == tapCon[i][m])flag++;
				if (flag == n) {
					for (k = 0; k < array[j].n2 && koTimThay == 1; k++) {
						flag = 0;
						for (m = 0; m < count; m++) {
							if (array[j].y[k] == ketQua[m])flag = 1;
						}
						if (flag == 0) {
							ketQua[count++] = array[j].y[k]; koTimThay = 0;
							printf( "Dua vao luat thu %d \n", j );
							for (m = 0; m < 6; m++)printf("%d ", ketQua[m] ); printf("\n\n");
							break;
						}
					}
				}
			}
		}
	}
	return ketQua;
}
