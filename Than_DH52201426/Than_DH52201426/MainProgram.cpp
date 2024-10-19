#include"Header.h"
#include"CayTimKiemNhiPhan.h"
#include"CayCanBang.h"
#include"CayTangLoe.h"
#include"BCay.h"
#include"HangUuTien.h"
#include"CayThuTuBoPhan.h"
#include"CayNghien.h"
#include"BangBam.h"
int main()
{
	char option = {};
	do
	{
		system("cls");
		cout << "Danh sach cac cau truc du lieu:" << endl;
		cout << "1. Cay Tim Kiem Nhi Phan." << endl;
		cout << "2. Cay Can Bang." << endl;
		cout << "3. Cay Tang Loe." << endl;
		cout << "4. B Cay." << endl;
		cout << "5. Hang Uu Tien." << endl;
		cout << "6. Cay Thu Tu Bo Phan." << endl;
		cout << "7. Cay Nghien." << endl;
		cout << "8. Bang Bam." << endl;
		cout << "0. Ket thuc chuong trinh." << endl;
		cout << "Chon tac vu: ";
		cin >> option;
		switch (option)
		{
		case '1': 
			CayTimKiemNhiPhan::menu();
			break;
		case '2': 
			CayCanBang::menu();
			break;
		case '3': 
			CayTangLoe::menu();
			break;
		case '4': 
			BCay::menu();
			break;
		case '5': 
			HangUuTien::menu();
			break;
		case '6': 
			CayThuTuBoPhan::menu();
			break;
		case '7': 
			CayNghien::menu();
			break;
		case '8': 
			BangBam::menu();
			break;
		default: 
			break;
		}
	} while (option != '0');
	return 0;
}