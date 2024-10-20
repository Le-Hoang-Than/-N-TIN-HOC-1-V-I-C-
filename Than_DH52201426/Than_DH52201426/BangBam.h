#pragma once
namespace BangBam
{


	namespace PhuongPhapKetNoiTrucTiep
	{
#pragma region Cấu trúc dữ liệu

		typedef int TYPEINFO;		//Kiểu của key

		struct NODE					//Cấu trúc của nút
		{
			TYPEINFO key;			//Khóa của nút
			int count;				//Số lần xuất hiện của khóa
			NODE* left;				//Con trỏ nút con trái
			NODE* right;			//Con trỏ nút con phải
		};
		typedef NODE* PTRNODE;		//Kiểu nút con trỏ

		struct HASHTABLE			//Cấu trúc của bảng băm
		{
			PTRNODE* array;			//Mảng các con trỏ nút
			int m;					//Số phần tử của bảng băm
		};

		int hashFunction(int m, TYPEINFO the_key)		//Hàm băm
		{
			return the_key % m;						//Lấy số dư the_key chia m
		}

#pragma endregion



#pragma region Khởi tạo bảng băm

		//Tạo bảng băm hash_table rỗng: các phần tử là rỗng
		void init(HASHTABLE& hash_table, int m)
		{
			hash_table.array = new PTRNODE[m];		//Tạo bảng băm có m phần tử
			for (int i = 0; i < m; i++)
				hash_table.array[i] = NULL;			//Phần tử [i] rỗng
			hash_table.m = m;
		}

#pragma endregion



#pragma region Thêm khóa vào bảng băm

		//Thêm khóa the_key vào cây p
		void insertBST(PTRNODE& p, TYPEINFO the_key)
		{
			if (p == NULL)					//Khi cây p rỗng
			{
				p = new NODE;
				p->key = the_key;			//Khóa là the_key
				p->count = 1;				//Số lần xuất hiện bằng 1
				p->left = p->right = NULL;	//Không có nút con trái và phải
			}
			else							//Khi cây p không rỗng
			{
				if (the_key < p->key)		//the_key < khóa của p
					//Thêm khóa the_key vào cây con trái của p
					insertBST(p->left, the_key);
				else if (the_key == p->key)	//the_key = khóa của p
					p->count++;				//Tăng số lần xuất hiện
				else						//the_key > khóa của p
					//Thêm khóa the_key vào cây con phải của p
					insertBST(p->right, the_key);
			}
		}

		//Thêm khóa the_key vào bảng băm hash_table
		void insert(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int address = hashFunction(hash_table.m, the_key);
			//Thêm khóa the_key vào cây BST của phần tử address
			insertBST(hash_table.array[address], the_key);
		}

#pragma endregion



#pragma region Tìm kiếm khóa trong bảng băm

		//Tìm kiếm khóa the_key trong cây p
		PTRNODE findBST(PTRNODE p, TYPEINFO the_key)
		{
			while (p != NULL)					//Khi p không rỗng
			{
				if (the_key < p->key)			//the_key < khóa của p
					p = p->left;				//Tìm the_key trong cây con trái
				else if (the_key == p->key)		//the_key = khóa của p
					break;						//Tìm thấy, thoát lặp
				else							//the_key > khóa của p
					p = p->right;				//Tìm the_key trong cây con phải
			}
			return p;							//Trả về nút p
		}

		//Tìm khóa the_key trong bảng băm hash_table
		PTRNODE find(HASHTABLE hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int address = hashFunction(hash_table.m, the_key);
			//Tìm khóa the_key trong cây BST của phần tử address
			PTRNODE result = findBST(hash_table.array[address], the_key);
			return result;
		}

#pragma endregion



#pragma region Loại bỏ khóa trong bảng băm

		//Loại bỏ nút cực phải của cây r
		void removeRightMostNode(PTRNODE& r, PTRNODE& q)
		{
			if (r->right != NULL)	//r không là nút cực phải
				//Loại bỏ nút cực phải của cây con phải của r
				removeRightMostNode(r->right, q);
			else					//r là nút cực phải
			{
				//Chép nút cực phải r vào nút q
				q->key = r->key;
				q->count = r->count;
				q = r;
				//r đến nút con trái để gắn cây con trái vào nút cha
				r = r->left;
			}
		}

		//Loại bỏ khóa the_key trong cây p
		bool removeBST(PTRNODE& p, TYPEINFO the_key)
		{
			if (p != NULL)					//Cây p không rỗng
			{
				if (the_key < p->key)		//the_key < khóa của p
					//Tìm và loại bỏ the_key trong cây con trái
					removeBST(p->left, the_key);
				else if (the_key > p->key)	//the_key > khóa của p
					//Tìm và lạo bỏ the_key trong cây con phải
					removeBST(p->right, the_key);
				else						//the_key = khóa của p
				{
					PTRNODE q = p;			//q là p
					if (p->left == NULL && p->right == NULL)
						//p là nút lá
						p = NULL;
					else if (p->left != NULL && p->right == NULL)
						//p có cây con trái, không có cây con phải
						p = p->left;		//Gắn cây con trái vào nút cha
					else if (p->left == NULL && p->right != NULL)
						//p có cây con phải, không có cây con trái
						p = p->right;		//Gắn cây con phải vào nút cha
					else
						//p có 2 cây con
						//Loại bỏ nút cực phải của cây con trái của p
						removeRightMostNode(p->left, q);
					delete q;				//Hủy bỏ nút q
					return true;			//Loại bỏ thành công
				}

			}
			else return false;				//Loại bỏ bất thành
		}

		//Loại bỏ khóa the_key trong bảng băm hash_table
		bool remove(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int address = hashFunction(hash_table.m, the_key);
			//Loại bỏ khóa the_key trong cây BST của phần tử [address]
			return removeBST(hash_table.array[address], the_key);
		}

#pragma endregion



#pragma region Hiển thị bảng băm

		//Hiển thị cây p theo thứ tự LNR (tăng dần)
		void traverseLNR(PTRNODE p)
		{
			if (p != NULL)
			{
				traverseLNR(p->left);
				cout << p->key << " ";
				traverseLNR(p->right);
			}
		}

		//Hiển thị bảng băm hashTable
		void traverse(HASHTABLE hash_table)
		{
			cout << "Bang Bam: " << endl;
			for (int i = 0; i < hash_table.m; i++)
			{
				cout << "Phan tu: " << i << ": ";
				//Hiển thị cây BST của phần tử [i]
				traverseLNR(hash_table.array[i]);
				cout << endl;
			}
		}

#pragma endregion



#pragma region Hủy bỏ bảng băm

		//Hủy bỏ cây p
		void clearLNR(PTRNODE& p)
		{
			if (p != NULL)
			{
				clearLNR(p->left);
				clearLNR(p->right);
				delete p;
				p = NULL;
			}
		}

		//Hùy bỏ bảng băm hash_table
		void clear(HASHTABLE& hash_table)
		{
			for (int i = 0; i < hash_table.m; i++)
				clearLNR(hash_table.array[i]);
		}

#pragma endregion



#pragma region Menu
		//Menu chức năng cho Phương pháp Kết nối trực tiếp
		void menu()
		{
			int m = 11;
			HASHTABLE hash_table;
			init(hash_table, m);	//Tạo hash_table với m phần tử

			char option = {};
			do
			{
				system("cls");
				cout << "Cac tac vu cua bang bam:" << endl;
				cout << "1. Them khoa vao bang bam." << endl;
				cout << "2. Tim kiem khoa trong bang bam." << endl;
				cout << "3. Loai bo khoa trong bang bam." << endl;
				cout << "4. Hien thi bang bam." << endl;
				cout << "5. Huy bo bang bam." << endl;
				cout << "0. Ket thuc." << endl;
				cout << "Chon tac vu: ";
				cin >> option;
				switch (option)
				{
				case '1':	//Thêm khóa vào bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can them vao: ";
					cin >> the_key;
					insert(hash_table, the_key);
					break;
				}
				case '2':	//Tìm khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can tim kiem: ";
					cin >> the_key;
					if (find(hash_table, the_key) == NULL)
						cout << "Khong tim thay khoa " << the_key << endl;
					else
						cout << "Tim thay khoa" << the_key << endl;
					system("pause");
					break;
				}
				case '3':	//Loại bỏ khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can loai bo: ";
					cin >> the_key;
					if (!remove(hash_table, the_key))
					{
						cout << "Khong co khoa " << the_key << endl;
						system("pause");
					}
					break;
				}
				case '4':	//Hiển thị bảng băm
				{
					traverse(hash_table);
					system("pause");
					break;
				}
				case '5':	//Hủy bỏ bảng băm
				{
					clear(hash_table);
					break;
				}

				default:
					break;
				}
			} while (option != '0');
		}
#pragma endregion


	}



	namespace PhuongPhapKetNoiHopNhat
	{
#pragma region Cấu trúc dữ liệu

		const int FREE = -1;		//Phần tử trống

		typedef int TYPEINFO;		//Kiểu của khóa

		struct ITEM					//Cấu trúc của phần tử
		{
			TYPEINFO key;			//Khóa của phần tử
			int next;				//Con trỏ phần tử kế tiếp
		};
		typedef ITEM* PTRITEM;		//Phần tử con trỏ

		struct HASHTABLE			//Cấu trúc bảng băm
		{
			PTRITEM array;			//Bảng băm
			int count;				//Số khóa hiện tại
			int m;					//Số phần tử của bảng băm
		};

		//Hàm băm
		int hashFunction(int m, int the_key)
		{
			return the_key % m;
		}

#pragma endregion



#pragma region Khởi tạo bảng băm

		//Tạo bảng băm trống
		void init(HASHTABLE& hash_table, int m)
		{
			hash_table.array = new ITEM[m];			//Tạo bảng băm có m phần tử
			for (int i = 0; i < m; i++)
			{
				hash_table.array[i].key = FREE;		//Phần tử [i] trống
				hash_table.array[i].next = FREE;	//Không có phần tử kế tiếp
			}
			hash_table.count = 0;					//Số khóa hiện tại = 0
			hash_table.m = m;						//Số khóa phần tử = m
		}

#pragma endregion



#pragma region Thêm khóa vào bảng băm

		//Thêm khóa the_key vào bảng băm hash_table
		bool insert(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			if (hash_table.count == hash_table.m)		//Bảng băm bị đầy
				return false;							//Không thêm vào được
			else
			{
				//Địa chỉ băm của the_key
				int address = hashFunction(hash_table.m, the_key);
				//Phần tử [address] trống
				if (hash_table.array[address].key == FREE)
					//Thêm khóa the_key tại phần tử [address]
					hash_table.array[address].key = the_key;
				else									//Đụng độ
				{
					//Tìm phần tử [i] trống từ cuối bảng băm về phía trên
					int i = hash_table.m - 1;			//i bắt đầu từ cuối bảng băm
					//Phần tử [i] khác trống
					while (hash_table.array[i].key != FREE)
						i--;							//i đi lên phía trên bảng

					//Thêm khóa the_key tại phần tử [i]
					hash_table.array[i].key = the_key;

					//Gắn phần tử [i] vào cuối danh sách liên kết
					//Tìm phần tử cuối cùng của danh sách liên kết
					int k = address;
					//Phần tử k chưa là phần tử cuối cùng
					while (hash_table.array[i].next != FREE)
						k = hash_table.array[k].next;	//k đến phần tử kế tiếp
					//Phần tử cuối cùng [k] trỏ phần tử [i]
					hash_table.array[k].next = i;
				}
				hash_table.count++;						//Tăng số khóa của bảng băm
				return true;
			}
		}

#pragma endregion



#pragma region Tìm kiếm khóa trong bảng băm

		//Tìm khóa the_key trong bảng băm hash_table
		int find(HASHTABLE hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int address = hashFunction(hash_table.m, the_key);
			//Phần tử [address] trống
			if (hash_table.array[address].key == FREE)
				return -1;							//Trả về -1
			else
			{
				int i = address;
				//i là phần tử của bảng băm và chưa tìm thấy
				while (i != FREE)
				{
					//Khóa của phần tử [i] = the_key
					if (hash_table.array[i].key == the_key)
						break;							//Tìm thấy
					else
						i = hash_table.array[i].next;	//i đến phần tử kế tiếp
				}
				return i;
			}
		}

#pragma endregion



#pragma region Loại bỏ khóa trong bảng băm

		//Loại bỏ khóa the_key của bảng băm hash_table
		bool remove(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int i = hashFunction(hash_table.m, the_key);

			//Tìm khóa the_key trong bảng băm
			bool found = false;							//Chưa tìm thấy
			//i là phần tử của bảng băm và vẫn chưa tìm thấy
			while (i != FREE && !found)
			{
				if (hash_table.array[i].key == the_key)
					//Khóa của phần tử [i] = the_key
					found = true;
				else
					i = hash_table.array[i].next;		//i đến phần tử kế tiếp
			}

			if (found)									//Tìm thấy khóa
			{
				//Loại bỏ phần tử [i]
				int before = FREE;				//before là phần tử trước phần tử [i]
				int k;
				//Di chuyển các phần tử sau phần tử [i] về phía trước
				//Phần tử [i] chưa là phần tử cuối cùng
				while (hash_table.array[i].next != FREE)
				{
					//k là phần tử sau phần tử [i]
					k = hash_table.array[i].next;
					//Chép phần tử [k] vào phần tử [i]
					hash_table.array[i].key = hash_table.array[k].key;
					before = i;					//before đến i
					i = k;						//i đến k
				}
				//Phần tử cuối cùng [i] là trống
				hash_table.array[i].key = FREE;
				//Phần tử [i] có phần tử trước [before]
				if (before != FREE)
					//Phần tử [before] là phần tử cuối cùng
					hash_table.array[before].next = FREE;
				hash_table.count--;
				return true;	//Loại bỏ thành công
			}
			return false;		//Loại bỏ bất thành
		}

#pragma endregion



#pragma region Hiển thị bảng băm

		//Hiển thị bảng băm hash_table
		void display(HASHTABLE hash_table)
		{
			cout << "Bang bam: " << endl;
			for (int i = 0; i < hash_table.m; i++)
			{
				cout << "Phan tu " << i << ": " << hash_table.array[i].key;
				cout << " -> " << hash_table.array[i].next << endl;
			}
			cout << "So khoa hien tai: " << hash_table.count << endl;
		}

#pragma endregion



#pragma region Hủy bỏ bảng băm

		//Hủy bỏ bảng băm hash_table
		void clear(HASHTABLE& hash_table)
		{
			for (int i = 0; i < hash_table.m; i++)
			{
				hash_table.array[i].key = FREE;		//Phần tử trống
				hash_table.array[i].next = FREE;		//Không có phần tử kế tiếp
			}
			hash_table.count = 0;					//Số khóa hiện tại = 0
		}

#pragma endregion



#pragma region Menu
		//Menu chức năng cho Phương pháp kết nối hợp nhất
		void menu()
		{
			int m = 11;
			HASHTABLE hash_table;
			init(hash_table, m);	//Tạo hash_table với m phần tử

			char option = {};
			do
			{
				system("cls");
				cout << "Cac tac vu cua bang bam:" << endl;
				cout << "1. Them khoa vao bang bam." << endl;
				cout << "2. Tim kiem khoa trong bang bam." << endl;
				cout << "3. Loai bo khoa trong bang bam." << endl;
				cout << "4. Hien thi bang bam." << endl;
				cout << "5. Huy bo bang bam." << endl;
				cout << "0. Ket thuc." << endl;
				cout << "Chon tac vu: ";
				cin >> option;
				switch (option)
				{
				case '1':	//Thêm khóa vào bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can them vao: ";
					cin >> the_key;
					if (insert(hash_table, the_key) == false)
					{
						cout << "Bang bam bi day." << endl;
						system("pause");
					}
					break;
				}
				case '2':	//Tìm khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can tim kiem: ";
					cin >> the_key;
					int pos = find(hash_table, the_key);
					if (pos == FREE)
						cout << "Khong tim thay khoa " << the_key << endl;
					else
						cout << "Tim thay khoa" << hash_table.array[pos].key << endl;
					system("pause");
					break;
				}
				case '3':	//Loại bỏ khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can loai bo: ";
					cin >> the_key;
					if (!remove(hash_table, the_key))
					{
						cout << "Khong co khoa " << the_key << endl;
						system("pause");
					}
					break;
				}
				case '4':	//Hiển thị bảng băm
				{
					display(hash_table);
					system("pause");
					break;
				}
				case '5':	//Hủy bỏ bảng băm
				{
					clear(hash_table);
					break;
				}

				default:
					break;
				}
			} while (option != '0');
		}
#pragma endregion

	}



	namespace PhuongPhapThuTuyenTinh
	{
#pragma region Cấu trúc dữ liệu

		const int FREE = -1;

		typedef int TYPEINFO;
		struct HASHTABLE		//Cấu trúc của bảng băm
		{
			TYPEINFO* array;	//Bảng băm
			int count;			//Số khóa của hiện tại
			int m;				//Số phần tử của bảng băm
		};

		//Hàm băm
		int hashFunction(int m, TYPEINFO the_key)
		{
			return the_key % m;
		}

#pragma endregion



#pragma region Khởi tạo bảng băm

		//Tạo bảng băm hash_table trống có m phần tử
		void init(HASHTABLE& hash_table, int m)
		{
			hash_table.array = new TYPEINFO[m];		//Tạo bảng băm có m phần tử
			for (int i = 0; i < m; i++)
				hash_table.array[i] = FREE;			//Phần tử [i] trống
			hash_table.count = 0;					//Số khóa hiện tại = 0
			hash_table.m = m;						//Số phần tử = m
		}

#pragma endregion



#pragma region Thêm khóa vào bảng băm

		//Thêm khóa the_key vào bảng băm hash_table
		bool insert(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			//Bảng băm bị đầy (còn 1 phần tử trống)
			if (hash_table.count == hash_table.m - 1)
				return false;					//Không thêm khóa vào được
			else
			{
				//Địa chỉ băm của the_key
				int i = hashFunction(hash_table.m, the_key);
				//Tìm phần tử trống để thêm khóa the_key
				//Phần tử [i] khác trống
				while (hash_table.array[i] != FREE)
				{
					i++;						//i đến phần tử kế tiếp
					//i quay vòng về đầu bảng băm (nếu cần)
					i = i % hash_table.m;
				}
				hash_table.array[i] = the_key;	//Thêm th_key tại phần tử [i]
				hash_table.count++;				//Tăng số khóa hiện tại
			}
		}

#pragma endregion



#pragma region Tìm kiếm khóa trong bảng băm

		//Tìm khóa the_key trong bảng băm hash_table
		int find(HASHTABLE hash_table, TYPEINFO the_key)
		{
			//Địa chỉ băm của the_key
			int i = hashFunction(hash_table.m, the_key);
			while (hash_table.array[i] != FREE)
			{
				//Khóa của phần tử [i] = the_key
				if (hash_table.array[i] == the_key)
					return i;	//Tìm thấy
				else
				{
					i++;		//i đến phần tử kế tiếp
					//i quay vòng về đầu hàng băm nếu cần
					i = i % hash_table.m;
				}
			}
			return -1;			//Không tìm thấy
		}

#pragma endregion



#pragma region Loại bỏ khóa trong bảng băm

		//Loại bỏ phần tử [i] trong bảng băm hash_table
		void removePosition(HASHTABLE& hash_table, int i)
		{
			bool conti = true;				//Còn tiếp tục
			bool inside = true;				//Còn tồn tại trong
			do
			{
				hash_table.array[i] = FREE;	//Phần tử [i] trống
				//Chép phần tử [i] khác trống kế tiếp vào phần tử [j] trống
				int j = i;					//j là i
				do
				{
					i++;					//i đến phần tử kế tiếp
					//i quay vòng về đầu bảng băm (nếu cần)
					i = i % hash_table.m;
					if (hash_table.array[i] == FREE)	//Phần tử [i] trống
						conti = false;					//Không tiếp tục
					else								//Phần tử [i] khác trống
					{
						//Địa chỉ băm của khóa hash_table.array[i]
						int r = hashFunction(hash_table.m, hash_table.array[i]);
						//r ở bên trong vùng vòng từ chỉ số j đến chỉ số i
						inside = ((j < r && r <= i) ||	//j <  r <= i
							(r <= i && i < j) ||		//r <= i <  j
							(i < j && j < r));			//i <  j <  r
					}
				} while (conti && inside);		//Còn tiếp tục, r còn bên trong
				if (conti)						//Còn tiếp rục
					//Chép phần tử [i] vào phần tử [j]
					hash_table.array[j] = hash_table.array[i];
			} while (conti);					//Còn tiếp tục
		}

		//Loại bỏ khóa the_key trong bảng băm hash_table
		bool remove(HASHTABLE& hash_table, TYPEINFO the_key)
		{
			//Tìm khóa the_key trong bảng băm hash_table
			int i = find(hash_table, the_key);
			if (i == -1)			//Không tìm thấy
				return false;
			else
			{
				hash_table.count--;	//Giảm số khóa của bảng băm
				//Loại bỏ phần tử [i] trong bảng băm hassh_table
				removePosition(hash_table, i);
				return true;
			}
		}

#pragma endregion



#pragma region Hiển thị bảng băm

		//Hiển thị bảng băm hash_table
		void display(HASHTABLE hash_table)
		{
			cout << "Bang bam: " << endl;
			for (int i = 0; i < hash_table.m; i++)
				//Hiển thị phần tử [i] của bảng băm 
				cout << "Phan tu " << i << ": " << hash_table.array[i] << endl;
			//Hiển thị số khóa của bảng băm
			cout << "So phan tu: " << hash_table.count << endl;
		}

#pragma endregion



#pragma region Hủy bỏ bảng băm

		//Hủy bỏ bảng băm hash_table
		void clear(HASHTABLE& hash_table)
		{
			for (int i = 0; i < hash_table.m; i++)
				hash_table.array[i] = FREE;			//Phần tử i trống
			hash_table.count = 0;					//Số khóa hiện tại = 0
		}

#pragma endregion



#pragma region Menu
		//Menu chức năng cho Phương pháp kết nối hợp nhất
		void menu()
		{
			int m = 11;
			HASHTABLE hash_table;
			init(hash_table, m);	//Tạo hash_table với m phần tử

			char option = {};
			do
			{
				system("cls");
				cout << "Cac tac vu cua bang bam:" << endl;
				cout << "1. Them khoa vao bang bam." << endl;
				cout << "2. Tim kiem khoa trong bang bam." << endl;
				cout << "3. Loai bo khoa trong bang bam." << endl;
				cout << "4. Hien thi bang bam." << endl;
				cout << "5. Huy bo bang bam." << endl;
				cout << "0. Ket thuc." << endl;
				cout << "Chon tac vu: ";
				cin >> option;
				switch (option)
				{
				case '1':	//Thêm khóa vào bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can them vao: ";
					cin >> the_key;
					if (insert(hash_table, the_key) == false)
					{
						cout << "Bang bam bi day." << endl;
						system("pause");
					}
					break;
				}
				case '2':	//Tìm khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can tim kiem: ";
					cin >> the_key;
					int pos = find(hash_table, the_key);
					if (pos == -1)
						cout << "Khong tim thay khoa " << the_key << endl;
					else
						cout << "Tim thay khoa" << the_key << endl;
					system("pause");
					break;
				}
				case '3':	//Loại bỏ khóa trong bảng băm
				{
					TYPEINFO the_key;
					cout << "Nhap khoa can loai bo: ";
					cin >> the_key;
					if (!remove(hash_table, the_key))
					{
						cout << "Khong co khoa " << the_key << endl;
						system("pause");
					}
					break;
				}
				case '4':	//Hiển thị bảng băm
				{
					display(hash_table);
					system("pause");
					break;
				}
				case '5':	//Hủy bỏ bảng băm
				{
					clear(hash_table);
					break;
				}

				default:
					break;
				}
			} while (option != '0');
		}
#pragma endregion

	}




#pragma region Menu
	//Menu chức năng cho Bảng băm
	void menu()
	{
		char option = {};
		do
		{
			system("cls");
			cout << "Cac phuong phap cua bang bam:" << endl;
			cout << "\nA. Phuong phap Ket noi." << endl;
			cout << "  1. Phuong phap Ket noi Truc tiep." << endl;
			cout << "  2. Phuong phap Ket noi Hop nhat." << endl;
			cout << "\nB. Phuong phap Dia chi mo." << endl;
			cout << "  3. Phuong phap thu tuyen tinh." << endl;
			cout << "\n0. Ket thuc." << endl;
			cout << "Chon tac vu: ";
			cin >> option;
			switch (option)
			{
			case '1':
				PhuongPhapKetNoiTrucTiep::menu();
				break;
			case '2':
				PhuongPhapKetNoiHopNhat::menu();
				break;
			case '3':PhuongPhapThuTuyenTinh::menu();
				break;
			default:
				break;
			}
		} while (option != '0');
	}
#pragma endregion

}