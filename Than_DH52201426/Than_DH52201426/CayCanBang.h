#pragma once
#include"Header.h"
namespace CayCanBang
{
	typedef int TYPEINFO;
	struct NODE
	{
		TYPEINFO info;
		int height;
		NODE* left;
		NODE* right;
	};
	typedef NODE* PTRNODE;
	typedef PTRNODE TREE;


#pragma region Khởi tạo Cây rỗng
	void init(TREE& root)
	{
		root = NULL;
	}
#pragma endregion


#pragma region Kiểm tra cây rỗng
	bool isEmpty(PTRNODE p)
	{
		return p == NULL;
	}
#pragma endregion


#pragma region Tạo cây root
	//Tạo cây p từ phần tử a[i]
	void create(TREE& p, TYPEINFO a[], TYPEINFO& i) 
	{
		if (a[i] == 0)
			p = NULL;					//Cây p rỗng;
		else
		{
			p = new NODE;				//Tạo nút mới p có khóa a[i](n)
			p->info = a[i];
			i++;						//Lấy phần tử kế tiếp
			create(p->left, a, i);		//Tạo cây con trái của p(L)
			i++;						//Lấy phần tử kế tiếp
			create(p->right, a, i);		//Tạo cây con phải của p(R)
		}
	}
#pragma endregion


#pragma region Tính chiều cao của cây
	//Lấy chiều cao của nút p
	int getHeight(PTRNODE p)
	{
		int height;
		if (p == NULL) height = 0;		//Cây p rỗng, chiều cao height = 0
		else height = p->height;		//Cây p khác rỗng, height = chiều cao nút p
		return height;
	}

	//Tính chiều cao của cây p
	void computeHeight(PTRNODE p)
	{
		int left_height = getHeight(p->left);	//Chiều cao của cây con trái
		int right_height = getHeight(p->right);	//Chiều cao của cây con phải
		if (left_height >= right_height)		//Khi chiều cao bên phải nhỏ hơn bên trái
			p->height = left_height + 1;		//Chiều cao của p = left_height + 1
		else
			p->height = right_height + 1;		//Trái lại, Chiều cao của p = right_height + 1
	}
#pragma endregion


#pragma region Thêm nút vào cây
	//Phép quay đơn LL
	void singleWithLeftChild(PTRNODE& p)
	{
		PTRNODE q = p->left;	//q là nút con trái của p
		p->left = q->right;		//Gắn cây con phải của q vào bên trái của p
		q->right = p;			//p là nút con phải của q
		computeHeight(p);		//Tính chiều cao của p
		computeHeight(q);		//Tính chiều cao của q
		p = q;					//p đi đến q
	}

	//Phép quay kép DLRR
	void doubleleWithLeftChild(PTRNODE& p)
	{
		PTRNODE q = p->left;	//q là nút con trái của p
		PTRNODE r = q->right;	//r là nút con phải của q
		q->right = r->left;		//Gắn cây con trái của r vào cây bên phải của q
		p->left = r->right;		//Gắn cây con phải của r vào cây bên trái của p
		r->left = q;			//q là nút con trái của r
		r->right = p;			//p là nút con phải của r
		computeHeight(q);		//Tính chiều cao của q
		computeHeight(p);		//Tính chiều cao của p
		computeHeight(r);		//Tính chiều cao của r
		p = r;					// p đi đến r
	}

	//Quay cây con trái
	void rotateWithLeftChild(PTRNODE& p)
	{
		//Cây p bị mất cân bằng: chiều cao cây con trái = chiều cao cây con phải + 2
		PTRNODE q = p->left;	//q là nút con trái của p
		//Khi cây q: chiều cao cây con trái >= chiều cao cây con phải
		if (getHeight(q->left) >= getHeight(q->right))
			singleWithLeftChild(p);		//Phép quay đơn LL
		else
			doubleleWithLeftChild(p);	//Phép quay kép DLRR
	}

	//Phép quay đơn RR
	void singleWithRightChild(PTRNODE& p)
	{
		PTRNODE q = p->right;	//q là nút con phải của p
		p->right = q->left;		//Gắn cây con trái của q vào bên phải của p
		q->left = p;			//p là nút con trái của	q
		computeHeight(p);		//Tính chiều cao của p
		computeHeight(q);		//Tính chiều cao của q
		p = q;					//p đi đên q
	}

	//Phép quay kép DRLR
	void doubleWithRightChild(PTRNODE& p)
	{
		PTRNODE q = p->right;	//q là nút con phải của p
		PTRNODE r = q->left;	//r là nút con trái của q
		q->left = r->right;		//Gắn cây con phải của r vào bên trái q
		p->right = r->left;		//Gắn cây con trái của r vào bên phải p
		r->left = p;			//p là nút con trái của r
		r->right = q;			//q là nút con phải của r
		computeHeight(q);		//Tính chiều cao của q
		computeHeight(p);		//Tính chiều cao của p
		computeHeight(r);		//Tính chiều cao của r
	}

	//Quay cây con phải
	void rotateWithRightChild(PTRNODE& p)
	{
		//Cây p bị mất cân bằng: chiều cao cây con phải = chiều cao cây con trái + s
		PTRNODE q = p->right;							//q là nút con phải của p
		//Cây q: chiều cao câu con phải >= chiều cao cây con trái
		if (getHeight(q->right) >= getHeight(q->left))
			singleWithRightChild(p);					//Phép quay đơn RR
		else
			doubleWithRightChild(p);					//Phép quay kép DRLR
	}

	//Thêm nút vào cây
	void insert(PTRNODE& p, TYPEINFO data)
	{
		if (p == NULL)						//Cây p rỗng
		{
			p = new NODE;					//Tạo nút lá p
			p->info = data;					//khóa là data
			p->height = 1;					//Chiều cao của nút bằng 1
			p->left = NULL;					//Không có nút con trái
			p->right = NULL;				//Không có nút con phải
		}
		else if (data < p->info)			//data nhỏ hơn khóa của p
		{
			insert(p->left, data);			//Thêm data vào cây con trái
			//Cây p bị mất cân bằng bên trái
			if (getHeight(p->left) - getHeight(p->right) == 2)
				rotateWithLeftChild(p);		//Phép quay cây con bên trái
			computeHeight(p);				//Tính chiều cao của p
		}
		else if (data > p->info)			//data lớn hơn khóa của p
		{
			insert(p->right, data);			//Thêm data vào cây con phải
			//Cây p bị mất cân bằng bên phải
			if (getHeight(p->right) - getHeight(p->left) == 2)
				rotateWithRightChild(p);	//Phép quay cây con phải
			computeHeight(p);				//Tính chiều cao của p
		}
	}
#pragma endregion


#pragma region Tìm kiếm nút trong cây
	PTRNODE find(PTRNODE p, TYPEINFO data)
	{
		while (p != NULL)				//Cây p khác rỗng
		{
			if (data < p->info)			//data nhỏ hơn khóa của p
				p = p->left;			//tìm trong cây con trái
			else if (data == p->info)	//Tìm thấy khóa
				return p;
			else						//Trái lại, data lớn hơn khóa của p
				p = p->right;			//Tìm trong cây con phải
		}
		return NULL;					//Không tìm thấy trả về Null
	}
#pragma endregion


#pragma region Xóa nút trong cây
	//Xóa nút cực phải(nút thế mạng) của cây r
	void removeRightMostNode(PTRNODE& r, PTRNODE& q)
	{
		if (r->right != NULL)					//r không phải là nút cực phải
		{
			//Xóa nút cực phải của cây con phải của r
			removeRightMostNode(r->right, q);
			computeHeight(r);					//Tính chiều cao của r
		}
		else									//r là nút cực phải
		{
			q->info = r->info;					//Chép nút cực phải vào nút q
			q = r;								//q đi đên r
			//r đến nút con trái để gắn cây con trái vào nút cha
			r = r->left;
		}
	}

	//Xóa một nút trong cây
	void remove(PTRNODE& p, TYPEINFO data)
	{
		if (p != NULL)							//Cây p khác rỗng
		{
			if (data < p->info)					//data nhỏ hơn khóa của p
			{
				//Loại bỏ data trong cây con trái
				remove(p->left, data);
				//Cây p bị mất cân bằng: chiều cao cây con phải = chiều cao cây con trái + 2
				if (getHeight(p->right) - getHeight(p->left) == 2)
					rotateWithRightChild(p);	//Phép quay cây con phải
				computeHeight(p);				//Tính chiều cao của p
			}
			else if (data = p->info)			//Tìm thấy nút
			{
				PTRNODE q = p;					//q là p
				//p là nút lá
				if (p->left == NULL && p->right == NULL)
					p = NULL;					//Gắn null vào nút cha
				//p có cây con trái, không có cây con phải
				else if (p->left != NULL && p->right == NULL)
					p = p->left;				//Gắn cây con trái vào nút cha
				//p có cây con phải, không có cây con trái
				else if (p->left == NULL && p->right != NULL)
					p = p->right;				//Gắn cây con phải vào nút cha
				else							//p có 2 cây con
				{
					//Xóa nút cực phải của cây con trai của p
					removeRightMostNode(p->left, q);
					//Cây p bị mất cân bằng: chiều cao cây con phải = chiều cao cây con trái + 2
					//Vì gắn cây con trái của nút cực phải vào nút cha có thể làm giảm chiều cao
					//cây con trái của p
					if (getHeight(p->right) - getHeight(p->left) == 2)
						rotateWithRightChild(p);//Phép quay cây con phải
					computeHeight(p);			//Tính chiều cao của p
				}
				delete q;						//Giải phóng nút q
			}
			else								//data > khóa của p
			{
				//Loại bỏ data trong cây con phải
				remove(p->right, data);
				//Cây p bị mất cân bằng: chiều cao cây con trái = chiều cao cây con phải + 2
				if (getHeight(p->left) - getHeight(p->right) == 2)
					rotateWithLeftChild(p);		//Phép quay cây con trái
				computeHeight(p);				//Tính chiều cao của p
			}
		}
	}
#pragma endregion


#pragma region Duyệt cây
	//Duyệt cây p theo thứ tự NLR
	void NLR(PTRNODE p, TYPEINFO n)
	{
		for (int i = 1; i <= n; i++)						//Thụt lề đầu hàng
			cout << " ";									//Hiển thị các ký tự trống
		if (p != NULL)
		{
			cout << p->info << ": " << p->height << endl;	//Hiển thị nút p (N)
			NLR(p->left, n + 3);							//Duyệt cây con trái (L)
			NLR(p->right, n + 3);							//Duyệt cây con phải (R)
		}
		else cout << "null" << endl;						//Cây là rỗng
	}

	//Duyệt cây p theo thứ tự LNR(tăng dần)
	void LNR(PTRNODE p, TYPEINFO n)
	{
		for (int i = 1; i <= n; i++)						//Thụt lề đầu hàng
			cout << " ";									//Hiển thị các ký tự trống
		if (p != NULL)
		{
			LNR(p->left, n + 3);							//Duyệt cây con trái (L)
			cout << p->info << ": " << p->height << endl;	//Hiển thị nút p (N)
			LNR(p->right, n + 3);							//Duyệt cây con phải (R)
		}
		else cout << "null" << endl;						//Cây là rỗng
	}

	//Duyệt cây p theo thứ tự RNL(giảm dần)
	void RNL(PTRNODE p, TYPEINFO n)
	{
		for (int i = 1; i <= n; i++)						//Thụt lề đầu hàng
			cout << " ";									//Hiển thị các ký tự trống
		if (p != NULL)
		{
			RNL(p->right, n + 3);							//Duyệt cây con phải (R)
			cout << p->info << ": " << p->height << endl;	//Hiển thị nút p (N)
			RNL(p->left, n + 3);							//Duyệt cây con trái (L)
		}
		else cout << "null" << endl;						//Cây là rỗng
	}

	//Duyệt cây
	void traverse(PTRNODE root)
	{
		if (root != NULL)
		{
			cout << "Cay Can Bang AVL: " << endl;
			NLR(root, 0);							//Duyệt cây
		}
		else cout << "null" << endl;				//Cây rỗng
	}
#pragma endregion


#pragma region Kiểm tra tính cân bằng của cây
	bool isBalanced(PTRNODE p, int& height)
	{
		bool avl;
		if (p == NULL)								//Cây p rỗng
		{
			avl = true;								//Là cân bằng
			height = 0;
		}
		else										//Cây p không rỗng
		{
			int left_height, right_height;
			//Kiểm tra tính cân bằng của cây con trái của p (L)
			avl = isBalanced(p->left, left_height);
			if (avl)
				//Kiểm tra tính cân bằng của cây con phải của p (R)
				avl = isBalanced(p->right, right_height);
			if (avl)
			{
				//Kiểm tra cây p có tính cân bằng
				if (left_height == right_height + 2 || right_height == left_height + 2)
					avl = false;					//Mất cân bằng tại p
				else
				{
					//Tính chiều cao của p
					if (left_height > right_height)
						height = left_height + 1;
					else
						height = right_height + 1;
				}
			}
		}
		return avl;
	}
#pragma endregion


#pragma region Xóa cây
	//Xóa cây
	void clear(PTRNODE& p)
	{
		if (p != NULL)			//Khi cây p không rỗng
		{
			clear(p->left);		//Xóa cây con trái
			clear(p->right);	//Xóa cây con phải
			delete p;			//Giải phóng nút p
			p = NULL;			//Cây p rỗng
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


#pragma region Menu
	//Menu chức năng cho Cây Cân Bằng
	void menu()
	{
	}
#pragma endregion


}