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

#pragma region Khởi tạo cây
	//Tạo cây root rỗng
	void init(TREE& root)
	{
		root = NULL;
	}
#pragma endregion


#pragma region Kiểm tra node rỗng
	bool isEmpty(PTRNODE p)
	{
		return p == NULL;
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
			PTRNODE p = new NODE; // khởi tạo 1 node để thêm vào cây
			p->info = x;// thêm dữ liệu x vào data
			p->left = NULL;
			p->right = NULL;
			root = p; //node p chính là node gốc (x chính là node gốc)
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
	void removeNode(PTRNODE& p, int data)
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
			}
		}
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


}