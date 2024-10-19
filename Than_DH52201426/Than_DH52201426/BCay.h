#pragma once
#include"Header.h"
namespace BCay
{
	const int M = 5;
	const int M2 = M / 2;	//B-Tree cấp 2
	typedef int TYPEINFO;	//Kiểu dữ liệu của khóa
	struct PAGE;			//Trang
	typedef PAGE* PTRPAGE;	//Con trỏ trang
	struct ITEM				//Phần tử khóa
	{
		TYPEINFO key;		//Khóa
		PTRPAGE next;		//Con trỏ trang con phải
	};
	//Cấu trúc của trang
	struct PAGE
	{
		ITEM key_item[M];	//Mảng các phần tử khóa
		PTRPAGE p_first;	//Con trỏ trang con đầu tiên
		TYPEINFO key_num;	//Số khóa của trang
	};
	PTRPAGE root;			//Con trỏ root của B-Tree


#pragma region Khởi tạo cây rỗng
	//Tạo cây root rỗng
	void init(PTRPAGE& root)
	{
		root = NULL;
	}
#pragma endregion


#pragma region Kiểm tra cây rỗng
	//Kiểm tra cây rỗng
	bool isEmpty(PTRPAGE p)
	{
		return p == NULL;
	}
#pragma endregion


#pragma region Tạo trang root
	//Xóa trang p
	void clear(PTRPAGE& p);
	//Tạo trang root
	void createPage(PTRPAGE& root)
	{
		if (root != NULL)
			clear(root);
		root = new PAGE;
	}
#pragma endregion


#pragma region Tìm kiếm khóa trong cây
	//Tìm khóa trong trang p
	void findPage(PTRPAGE p, TYPEINFO data, bool& found, int& pos)
	{
		found = false;									//Chưa tìm thấy
		int k;
		int i = 0;										//i ở đầu dãy
		int j = p->key_num - 1;							//i ở cuối dãy
		//Tồn tại dãy [i..j] và chưa tìm thấy
		while (i <= j && !found)
		{
			k = (i + j) / 2;							//k ở chính giữa
			if (data < p->key_item[k].key)				//data < khóa của phần tử k
				j = k - 1;
			else if (data == p->key_item[k].key)		//data = khóa của phần tử k
				found = true;							//Tìm thấy 
			else										//data > khóa của phần tử k
				i = k + 1;								//Dãy con [k+1..j]
		}
		if (found)										//Tìm thấy data
			pos = k;
		else
			pos = j;
	}

	//Tìm khóa trong cây p
	void findBTree(PTRPAGE p, TYPEINFO data, bool& found, int& pos)
	{
		if (p == NULL)						//Cây p rỗng
		{
			found = false;					//Không tìm thấy
			pos = -1;						//trả về vị trí -1
		}
		else								//cây p khồn rỗng
		{
			findPage(p, data, found, pos);	//Tìm tìm data trong trang p
			if (!found)						//Không tìm thấy
			{
				if (pos == -1)
					//Tìm data trong cây con đầu tiên
					findBTree(p->p_first, data, found, pos);
				else
					//Tìm data trong cây con phải p->key_item[pos].next
					findBTree(p->key_item[pos].next, data, found, pos);
			}
		}
	}

	//Tìm khóa trong cây root
	bool find(PTRPAGE root, TYPEINFO data)
	{
		bool found;
		int pos;
		findBTree(root, data, found, pos);
		return found;
	}
#pragma endregion


#pragma region Thêm khóa vào cây
	//Thêm khóa data vào trang p sau chỉ số pos
	//Trang con phải của khóa data là trang r
	void insertPage(PTRPAGE& p, int pos, TYPEINFO data, PTRPAGE r,
		bool& overflow, int& overflow_key, PTRPAGE& overflow_page)
	{
		overflow = false;								//Chưa bị tràn
		overflow_key = data;							//data gửi cho trang cha
		overflow_page = NULL;							//p không có trang anh em bên phải

		//Thêm khóa data vào trang p tại chỉ số pos + 1
		//Di chuyển các khóa từ chỉ số post + 1 về bên phải
		for (int i = p->key_num; i > pos; i--)
		{
			//Chép phần tử [i] vào phần tử [i+1]
			p->key_item[i + 1].key = p->key_item[i].key;
			p->key_item[i + 1].next = p->key_item[i].next;
		}
		p->key_item[pos + 1].key = data;				//Phần tử [i+1] chứa data
		//r là trang con bên phải của data
		p->key_item[pos + 1].next = r;
		p->key_num++;									//Tăng chỉ số khóa của trang p
		if (p->key_num == M)							//Trang bị tràn (overflow)
		{
			overflow = true;							//Có bị tràn

			//overflow_key là khóa chính giữa được gửi lên trang cha của trang p
			overflow_key = p->key_item[M2].key;

			//Tách trang p và tạo trang mới overflow_page
			//Tạo trang mới overflow_page, là trang anh em bên phải của p
			overflow_page = new PAGE;
			overflow_page->key_num = M2;
			//Chép các khóa của nửa cuối trang p vào trang overflow_page
			int k = 0;
			for (int i = M2 + 1; i < p->key_num; i++)
			{
				overflow_page->key_item[k].key = p->key_item[i].key;
				overflow_page->key_item[k].next = p->key_item[i].next;
				k++;
			}

			//Trang đầu tiên của trang overflow_page là trang bên phải của phần tử khóa có chỉ số M2
			overflow_page->p_first = p->key_item[M2].next;
			p->key_num = M2;							//Số khóa của trang p là M2
		}
	}

	//Thêm khóa data vào cây p
	void insertBTree(PTRPAGE& p, TYPEINFO data, bool& overflow, int& overflow_key,
		PTRPAGE& overflow_page)
	{
		overflow_key = data;							//data gửi cho trang cha
		//Trang p không có trang anh em bên phải
		overflow_page = NULL;
		if (p == NULL)
			//Có bị tràn, data được gửi cho trang lá
			overflow = true;
		else											//Cây p khác rỗng
		{
			overflow = false;
			bool found;
			int pos;
			//Tìm khóa data trong trang p
			//Nếu tìm thấy, trả về found là true và chỉ số là pos
			//Nếu không tìm thấy, trả về chỉ số pos của phần tử có khóa vừa nhỏ hơn khóa data
			findPage(p, data, found, pos);

			if (!found)
			{
				//Không tìm thấy.
				//Thêm khóa data vào trang con của trang p
				PTRPAGE q;								//Thêm khóa data vào trang q
				if (pos == -1)
					q = p->p_first;
				else
					//q là trang con bên phải của phần tử có chỉ số pos
					q = p->key_item->next;

				//Thêm khóa data vào trang q là trang con của trang p
				//Trả về overflow là true nếu trang q bị tràn,
				//khóa gửi cho trang p là overflow_key
				//Trang con bên phải của trang p là trang overflow_page
				insertBTree(q, data, overflow, overflow_key, overflow_page);

				if (overflow)							//Có bị tràn
				{
					//Data là khóa overflowkey được gửi cho trang p
					int data2 = overflow_key;
					PTRPAGE r = overflow_page;			//r là trang overflow_page

					//Thêm khóa tràn data2 của trang con q vào trang p
					insertPage(p, pos, data2, r, overflow, overflow_key, overflow_page);

					if (overflow)						//Trang p bị tràn
					{
						//Quá trình overflow đi lên đến trang gốc root
						//thì B-Tree tăng chiều cao
						if (p == root)						//Trang root bị tràn
						{
							//Tạo trang r mới có phần tử khóa overflow_key
							createPage(q);
							q->p_first = p;				//p là trang con đầu tiên của q
							//Gửi khóa overflow_key cho trang cha
							q->key_item[0].key = overflow_key;
							//overflow_page là trang con thứ 2 của trang q
							q->key_item[0].next = overflow_page;
							q->key_num = 1;				//Trang q chỉ có 1 phần tử
							root = q;						//root trỏ trang q
							overflow = false;			//Trang r chưa bị tràn
						}
					}
				}
			}
		}
	}

	//Thêm khóa data vào cây root
	void insert(PTRPAGE& root, TYPEINFO data)
	{
		if (root == NULL)					//Cây root rỗng
		{
			//Tạo trang gốc root chứa data
			createPage(root);
			root->key_item[0].key = data;
			root->key_num = 1;
		}
		else								//Cây root khác rỗng
		{
			//Thêm khóa data vào cây root
			bool overflow;
			int overflow_key;
			PTRPAGE overflow_page;
			insertBTree(root, data, overflow, overflow_key, overflow_page);
		}
	}
#pragma endregion


#pragma region Loại bỏ khóa trong cây
	//Loại bỏ khóa có chỉ số pos của trang p
	void removeKey(PTRPAGE& p, int pos)
	{
		//Di chuyển các khóa từ chỉ số pos + 1 về bên trái
		for (int i = pos; i < p->key_num; i++)
		{
			p->key_item[i].key = p->key_item[i + 1].key;
			p->key_item[i].next = p->key_item[i + 1].next;
		}
		p->key_num--;	//Giảm số khóa của trang p
	}

	//Trộn trang p với khóa có chỉ số pos của trang cha parent và trang q vào
	//trang p. Giải phóng trang q
	void mergePage(PTRPAGE p, PTRPAGE parent, int pos, PTRPAGE q)
	{
		//Thêm khóa có chỉ số pos của trang cha parent vào cuối trang p
		int n = p->key_num;								//Số khóa của trang p
		p->key_item[n].key = parent->key_item[pos].key;
		p->key_item->next = q->p_first;
		n++;											//Tăng số khóa

		//Thêm tất cả khóa của trang q vào cuối trang p
		for (int i = 0; i < q->key_num; i++)
		{
			p->key_item[n + i].key = q->key_item[i].key;
			p->key_item[n + i].next = q->key_item[i].next;
		}
		p->key_num = M - 1;								//Số khóa của trang p

		//Loại bỏ khóa có chỉ số pos của trang cha parent
		//Di chuyển các khóa sau khóa có chỉ số pos về bên trái của trang cha parent
		for (int i = pos; i < parent->key_num; i++)
		{
			parent->key_item[i].key = parent->key_item[i + 1].key;
			parent->key_item[i].next = parent->key_item[i + 1].next;
		}
		parent->key_num--;								//Giảm số khóa của trang cha parent

		//Quá trình underflow đi lên đến trang gốc root thì B-Tree bị giảm chiều cao
		//Trang cha parent là trang root và không có khóa
		if (parent == root && parent->key_num == 0)
			root = p;									//root trỏ trang p
		delete q;										//Hủy bỏ trang q
	}

	//Trang p lấy một khóa của trang anh em bên phải q
	//Trang parent là trang cha của trang p và trang q
	//Khóa có chỉ số pos của trang cha parent thêm vào cuối trang p
	//Khóa đầu tiên của trang q thay thế khóa có chỉ số pos của trang cha parent
	void retrieveRightPage(PTRPAGE p, PTRPAGE parent, int pos, PTRPAGE q)
	{
		//Thêm khóa có chỉ số pos của trang cha parent vào cuối trang p
		p->key_item[p->key_num].key = parent->key_item[pos].key;
		p->key_item[p->key_num].next = q->p_first;
		p->key_num++;									//Tăng số khóa của trang p

		//Thay thế khóa có chỉ số pos của trang cha parent bởi khóa đầu tiên của trang q
		parent->key_item[pos].key = q->key_item[0].key;

		//Di chuyển tất cả khóa của trang q về đầu trang
		q->p_first = q->key_item[0].next;
		for (int i = 1; i < q->key_num; i++)
		{
			q->key_item[i - 1].key = q->key_item[i].key;
			q->key_item[i - 1].next = q->key_item[i].next;
		}
		q->key_num--;									//Giảm số khóa của trang q
	}

	//Trang p lấy một khóa của trang anh em bên trái q
	//Thêm khóa có chỉ số pos của trang cha parent vào đầu trang p
	void retrieveLeftPage(PTRPAGE p, PTRPAGE parent, int pos, PTRPAGE q)
	{
		//Di chuyển tất cả khóa của trang q về bên phải
		for (int i = p->key_num - 1; i >= 0; i--)
		{
			p->key_item[i + 1].key = p->key_item[i].key;
			p->key_item[i + 1].next = p->key_item[i].next;
		}
		//Thêm khóa có chỉ số pos của trang cha parent vào đầu trang p
	}
#pragma endregion


#pragma region Duyệt cây
	//Duyệt trang p
	void traversePage(PTRPAGE p)
	{
		for (int i = 0; i < p->key_num; i++)
			//Hiển thị khóa của phần tử i
			cout << p->key_item[i].key << " ";
		cout << endl;
	}

	//Duyệt cây p
	void traverseTree(PTRPAGE p, int k)
	{
		for (int i = 1; i <= k; i++)			//Thụt đầu hàng
			cout << " ";
		cout << "Trang: ";
		traversePage(p);						//Hiển thị trang p
		if (p->p_first != NULL)					//p có các cây con
			traverseTree(p->p_first, k + 2);	//Hiển thị cây con đầu tiên
		for (int i = 0; i < p->key_num; i++)	//Hiển thị các cây con còn lại
			if (p->key_item[i].next != NULL)
				//Hiển thị cây con của khóa có chỉ số i
				traverseTree(p->key_item[i].next, k + 2);
	}
	//Duyệt B-Tree
	void traverseBTree(PTRPAGE root)
	{
		if (root != NULL)				//Cây khác rỗng
			traverseTree(root, 0);		//Hiển thị BTree
		else cout << "null" << endl;	//Cây root rỗng
	}
#pragma endregion


#pragma region Hủy bỏ cây
	//Hủy bỏ cây bằng phép duyệt LRN
	void clear(PTRPAGE& p)
	{
		if (p != NULL)								//Trang p không rỗng
		{
			if (p->p_first != NULL)					//p có cây con đầu tiên
				clear(p->p_first);					//Hủy bỏ cây con đầu tiên
			for (int i = 0; i < p->key_num; i++)	//Hủy bỏ các cây con còn lại
				if (p->key_item[i].next != NULL)
					clear(p->key_item[i].next);		//Hủy bỏ cây con của khóa có chỉ số i
			delete p;								//Hủy bỏ trang p
			p = NULL;								//Trang p rỗng
		}
	}
#pragma endregion


#pragma region Cập nhật lại giá trị của x

#pragma endregion


#pragma region Menu
	//Menu chức năng cho B Cây
	void menu()
	{
	}
#pragma endregion

}