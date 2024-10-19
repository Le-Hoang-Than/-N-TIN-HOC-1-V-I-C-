#pragma once
#include"Header.h"
namespace CayTimKiemNhiPhan
{
	typedef int TYPEINFO;
	struct NODE {
		TYPEINFO info;
		NODE* left;
		NODE* right;
	};
	typedef NODE* PTRNODE;
	typedef PTRNODE TREE;



#pragma region Khởi tạo cây rỗng
	//Tạo cây root rỗng
	void init(TREE& root)
	{
		root = NULL;
	}
	//Tạo nút mới
	PTRNODE newNode(TYPEINFO data)
	{
		PTRNODE newNode = new NODE;
		newNode->info = data;
		newNode->left = newNode->right = NULL;
		return newNode;
	}
#pragma endregion



#pragma region Kiểm tra node rỗng
	bool isEmpty(PTRNODE p)
	{
		return p == NULL;
	}
#pragma endregion



#pragma region Tạo cây root
	//Tạo cây root từ phần tử a[i] theo phép duyệt NLR
	void create(PTRNODE& root, TYPEINFO a[], int& i)
	{
		if (a[i] == 0)						//Số 0 biểu diễn null
			root = NULL;					//Cây p rỗng
		else
		{
			root = new NODE;				//Tạo nút mới p
			root->info = a[i];				//Khóa của root là a[i]
			i++;							//Lấy phần tử kế tiếp
			create(root->left, a, i);		//Tạo cây con trái của p
			i++;							//Lấy phần tử kế tiếp
			create(root->right, a, i);		//Tạo cây con phải của root
		}
	}
#pragma endregion



#pragma region Duyệt cây
	//NLR
	void NLR(PTRNODE p)
	{
		if (p != NULL)
		{
			cout << p->info << " ";
			NLR(p->left);
			NLR(p->right);
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//NRL
	void NRL(PTRNODE p)
	{
		if (p != NULL)
		{
			cout << p->info << " ";
			NLR(p->right);
			NLR(p->left);
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//LNR
	void LNR(PTRNODE p)
	{
		if (p != NULL)
		{
			NLR(p->left);
			cout << p->info << " ";
			NLR(p->right);
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//RNL
	void RNL(PTRNODE p)
	{
		if (p != NULL)
		{
			NLR(p->right);
			cout << p->info << " ";
			NLR(p->left);
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//LRN
	void LRN(PTRNODE p)
	{
		if (p != NULL)
		{
			NLR(p->left);
			NLR(p->right);
			cout << p->info << " ";
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//RLN
	void RLN(PTRNODE p)
	{
		if (p != NULL)
		{
			NLR(p->right);
			NLR(p->left);
			cout << p->info << " ";
		}
		else cout << "Null"; //Nếu là cây rỗng
		cout << endl;
	}

	//Duyệt cây
	void traverseTree(TREE root)
	{
		NLR(root); //Sử dụng cách duyệt node-left-right
	}
#pragma endregion



#pragma region Thêm phần tử
	//Chèn nút vào cây
	void insertNode(TREE& root, TYPEINFO x)
	{
		if (root == NULL) // Nếu cây là rỗng
		{
			root = newNode(x); //newNode chính là node gốc (x chính là node gốc)
		}
		// cây có tồn tại phần tử
		// nếu phần tử thêm vào mà nhỏ hơn node gốc => thêm nó vào bên trái
		else if (root->info > x)
		{
			insertNode(root->left, x); // duyệt qua trái để thêm phần tử x
		}
		else if (root->info < x) // nếu phần tử thêm vào mà lớn hơn node gốc ==> thêm nó vào bên phải
		{
			insertNode(root->right, x); // duyệt qua phải để thêm phần tử x
		}
	}
#pragma endregion



#pragma region Tìm node
	PTRNODE find(PTRNODE p, TYPEINFO x)
	{
		if (p == NULL)
		{
			return NULL;
		}
		else if (x < p->info)
		{
			find(p->left, x);
		}
		else if (x > p->info)
		{
			find(p->right, x);
		}
		return p;
	}
#pragma endregion



#pragma region Xóa phần tử
	//Loại bỏ nút cực phải của cây root
	//Với "q" là nút bị loại bỏ
	//Với "p" là nút cực phải
	void removeRightMostNode(PTRNODE& p, PTRNODE& q)
	{
		if (p->right != NULL) //p không phải là cực phải
		{
			//Loại bỏ nút cực phải của cây con phải của p
			removeRightMostNode(p->right, q);
		}
		else
		{
			q->info = p->info;
			q = p;
			p = p->left;
		}
	}
	bool removeNode(PTRNODE& p, int data)
	{
		if (p != NULL) //Cây root khác rỗng
		{
			if (data < p->info) //Nếu như data nhỏ hơn node gốc
			{
				removeNode(p->left, data); //Duyệt sang nhánh trái của cây
			}
			else if (data > p->info)
			{
				removeNode(p->right, data); //Duyệt sang nhánh phải của cây
			}
			else //Đã tìm ra node cần xóa
			{
				PTRNODE q = p; //Đây là nút thế mạng
				if (p->left == NULL && p->right == NULL) // => p là nút lá
				{
					p = NULL;
				}
				else if (p->left != NULL && p->right == NULL)
				{
					//p có cây con trái và không có cây con phải
					p = p->left;
				}
				else if (p->left == NULL && p->right != NULL)
				{
					//p có không có cây con trái và có cây con phải
					p = p->right;
				}
				else //Loại bỏ nút cực phải của cây con trái của p
					removeRightMostNode(p->left, q);
				delete q; //Hủy bỏ nút q
				return true;
			}
		}
		return false;
	}
#pragma endregion



#pragma region Xóa cây
	//Xóa cây
	void clear(PTRNODE& p)
	{
		if (p != NULL) //Khi cây p khác rỗng
		{
			clear(p->left); //Hủy bỏ cây con trái
			clear(p->right); //Hủy bỏ cây con phải
			delete p; //Giải phóng nút p
			p = NULL; //Cây p rỗng
		}
	}
#pragma endregion



#pragma region Cập nhật lại giá trị của x
	//Cập nhật lại giá trị của x
	PTRNODE update(TREE& root, PTRNODE p, TYPEINFO x)
	{
		return NULL;
	}
#pragma endregion



#pragma region Kiểm tra tính chất của BST
	//Kiểm tra tính chất của cây tìm kiếm nhị phân
	bool isBSTree(PTRNODE p, int& key, bool& first_time)
	{
		bool is_bst = true;						//Là cây BST
		if (p != NULL)
		{
			//Kiểm tra cây con trái của p là cây bst (L)
			is_bst = isBSTree(p->left, key, first_time);
			if (is_bst)
			{
				//Kiểm tra khóa của p với khóa của key (N)
				if (first_time)					//p không có khóa trước key
					first_time = false;			//Không là lần đầu
				else if (key >= p->info)		//key >= khóa của p
					is_bst = false;				//Không là cây BST
				key = p->info;					//key là khóa của p
			}
			if (is_bst)
				//Kiểm tra cây con phải của p là cây BST (R)
				is_bst = isBSTree(p->right, key, first_time);
		}
		return is_bst;
	}
	//Kiểm tra cây root là cây BST
	bool isBST(PTRNODE p)
	{
		int key;
		bool first_time = true;					//Là lần đầu tiên
		bool result = isBSTree(p, key, first_time);
		return result;
	}
#pragma endregion



#pragma region Menu
	//Menu chức năng cho Cây tìm kiếm nhị phân
	void menu()
	{
		TREE root;
		init(root);
		int option = 0;
		do
		{
			system("cls");
			cout << "Cac tac vu cua cay:" << endl;
			cout << "1. Kiem tra cay rong." << endl;
			cout << "2. Them khoa vao cay." << endl;
			cout << "3. Tim kiem khoa trong cay." << endl;
			cout << "4. Loai bo khoa trong cay." << endl;
			cout << "5. Duyet cay theo thu tu NLR." << endl;
			cout << "6. Kiem tra tinh chat cua cay BST." << endl;
			cout << "7. Huy bo cay." << endl;
			cout << "0. Ket thuc." << endl;
			cout << "Chon tac vu: ";
			cin >> option;
			switch (option)
			{
			case 1:		//Kiểm tra cây rỗng
			{
				//Kiểm tra cây root rỗng
				if (isEmpty(root))
					cout << "Cay rong" << endl;
				else cout << "Cay khong rong" << endl;
				system("pause");
				break;
			}
			case 2:		//Thêm khóa vào cây
			{
				int data;
				cout << "Nhap du lieu them vao: ";
				cin >> data;
				//Thêm data vào cây root
				insertNode(root, data);
				break;
			}
			case 3:		//Tìm kiếm khóa trong cây
			{
				int data;
				cout << "Nhap khoa can tim kiem: ";
				cin >> data;
				//Tìm data trong cây root
				PTRNODE p = find(root, data);
				if (p != NULL)
					cout << "Tim thay khoa " << p->info << endl;
				else
					cout << "Khong tim thay khoa " << data << endl;
				system("pause");
				break;
			}
			case 4:		//Loại bỏ khóa trong 
			{
				int data;
				cout << "Nhap khoa muon loai bo: ";
				cin >> data;
				if (!removeNode(root, data))
				{
					cout << "Khong co khoa " << data << endl;
					system("pause");
				}
				break;
			}
			case 5:		//Duyệt cây theo thứ tự NLR
			{
				if (isEmpty(root))
					cout << "Cay rong" << endl;
				else
					traverseTree(root);
				system("pause");
				break;
			}
			case 6:		//Kiểm tra cây BST
			{
				int a[] = { 2,1,0,0,4,3,0,0,0 };
				int i = 0;
				//Tạo cây root thừ mảng a[] theo phép duyệt NLR
				create(root, a, i);
				//Kiểm tra cây root là cây BST
				bool is_bst = isBST(root);
				if (is_bst)
					cout << "La cay BST" << endl;
				else
					cout << "Khong la cay BST" << endl;
				system("pause");
				break;
			}
			case 7:		//Hủy bỏ cây
			{
				clear(root);
				break;
			}
			default:
				break;
			}
		} while (option != 0);
	}
#pragma endregion

}