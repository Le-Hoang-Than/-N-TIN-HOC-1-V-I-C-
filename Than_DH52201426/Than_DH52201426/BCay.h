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
	bool findPage(PTRPAGE p, TYPEINFO the_key, int& pos)
	{
		bool found = false;								//Chưa tìm thấy
		int k;
		int i = 0;										//i ở đầu dãy
		int j = p->key_num - 1;							//i ở cuối dãy
		//Tồn tại dãy [i..j] và chưa tìm thấy
		while (i <= j && !found)
		{
			k = (i + j) / 2;							//k ở chính giữa
			if (the_key < p->key_item[k].key)				//the_key < khóa của phần tử k
				j = k - 1;
			else if (the_key == p->key_item[k].key)		//the_key = khóa của phần tử k
				found = true;							//Tìm thấy 
			else										//the_key > khóa của phần tử k
				i = k + 1;								//Dãy con [k+1..j]
		}
		if (found)										//Tìm thấy the_key
			pos = k;
		else
			pos = j;
		return found;
	}

	//Tìm khóa trong cây p
	bool findBTree(PTRPAGE p, TYPEINFO the_key, int& pos)
	{
		if (p == NULL)						//Cây p rỗng
		{
			pos = -1;						//trả về vị trí -1
			return false;					//Không tìm thấy
		}
		else								//cây p khồn rỗng
		{
			bool found = findPage(p, the_key, pos);	//Tìm tìm the_key trong trang p
			if (!found)						//Không tìm thấy
			{
				if (pos == -1)
					//Tìm the_key trong cây con đầu tiên
					findBTree(p->p_first, the_key, pos);
				else
					//Tìm the_key trong cây con phải p->key_item[pos].next
					findBTree(p->key_item[pos].next, the_key, pos);
			}
		}
	}

	//Tìm khóa trong cây root
	bool find(PTRPAGE root, TYPEINFO the_key)
	{
		int pos;
		return findBTree(root, the_key, pos);
	}
#pragma endregion


#pragma region Thêm khóa vào cây
	//Thêm khóa the_key vào trang p sau chỉ số pos
	//Trang con phải của khóa the_key là trang r
	void insertPage(PTRPAGE& p, int pos, TYPEINFO the_key, PTRPAGE r,
		bool& overflow, int& overflow_key, PTRPAGE& overflow_page)
	{
		overflow = false;								//Chưa bị tràn
		overflow_key = the_key;							//the_key gửi cho trang cha
		overflow_page = NULL;							//p không có trang anh em bên phải

		//Thêm khóa the_key vào trang p tại chỉ số pos + 1
		//Di chuyển các khóa từ chỉ số post + 1 về bên phải
		for (int i = p->key_num; i > pos; i--)
		{
			//Chép phần tử [i] vào phần tử [i+1]
			p->key_item[i + 1].key = p->key_item[i].key;
			p->key_item[i + 1].next = p->key_item[i].next;
		}
		p->key_item[pos + 1].key = the_key;				//Phần tử [i+1] chứa the_key
		//r là trang con bên phải của the_key
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

	//Thêm khóa the_key vào cây p
	void insertBTree(PTRPAGE& p, TYPEINFO the_key, bool& overflow, int& overflow_key,
		PTRPAGE& overflow_page)
	{
		overflow_key = the_key;							//the_key gửi cho trang cha
		//Trang p không có trang anh em bên phải
		overflow_page = NULL;
		if (p == NULL)
			//Có bị tràn, the_key được gửi cho trang lá
			overflow = true;
		else											//Cây p khác rỗng
		{
			overflow = false;
			int pos;
			//Tìm khóa the_key trong trang p
			//Nếu tìm thấy, trả về found là true và chỉ số là pos
			//Nếu không tìm thấy, trả về chỉ số pos của phần tử có khóa vừa nhỏ hơn khóa the_key
			;

			if (!findPage(p, the_key, pos))
			{
				//Không tìm thấy.
				//Thêm khóa the_key vào trang con của trang p
				PTRPAGE q;								//Thêm khóa the_key vào trang q
				if (pos == -1)
					q = p->p_first;
				else
					//q là trang con bên phải của phần tử có chỉ số pos
					q = p->key_item->next;

				//Thêm khóa the_key vào trang q là trang con của trang p
				//Trả về overflow là true nếu trang q bị tràn,
				//khóa gửi cho trang p là overflow_key
				//Trang con bên phải của trang p là trang overflow_page
				insertBTree(q, the_key, overflow, overflow_key, overflow_page);

				if (overflow)							//Có bị tràn
				{
					//the_key là khóa overflowkey được gửi cho trang p
					int the_key2 = overflow_key;
					PTRPAGE r = overflow_page;			//r là trang overflow_page

					//Thêm khóa tràn the_key2 của trang con q vào trang p
					insertPage(p, pos, the_key2, r, overflow, overflow_key, overflow_page);

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

	//Thêm khóa the_key vào cây root
	void insert(PTRPAGE& root, TYPEINFO the_key)
	{
		if (root == NULL)					//Cây root rỗng
		{
			//Tạo trang gốc root chứa the_key
			createPage(root);
			root->key_item[0].key = the_key;
			root->key_num = 1;
		}
		else								//Cây root khác rỗng
		{
			//Thêm khóa the_key vào cây root
			bool overflow;
			int overflow_key;
			PTRPAGE overflow_page;
			insertBTree(root, the_key, overflow, overflow_key, overflow_page);
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
		p->key_item[0].key = parent->key_item[pos].key;
		p->key_item[0].next = p->p_first;
		p->p_first = q->key_item[q->key_num - 1].next;
		p->key_num++;									//Tăng số khóa của trang b

		//Chép khóa cuối cùng của trang q vào khóa có chỉ số pos của trang cha parent
		parent->key_item[pos].key = q->key_item[q->key_num - 1].key;
		q->key_num--;									//Giảm số khóa của trang q
	}

	//Xử lý underflow của trang p với trang anh em bên phải
	//Trang p không có trang anh em bên trái
	//Trang parent là trang cha của trang p
	//Trang anh em bên phải của trang p: parent->key_item[pos].next
	void underflowRightPage(PTRPAGE& p, PTRPAGE parent, int pos)
	{
		//right_page là trang anh em bên phải của trang p
		PTRPAGE right_page = parent->key_item[pos].next;
		if (right_page->key_num > M2)
			//Lấy một khóa của trang anh em bên phải
			retrieveRightPage(p, parent, pos, right_page);
		else
			//Trộn trang p với trang anh em bên phải
			mergePage(p, parent, pos, right_page);
	}

	//Xử lý underflow của trang p với trang anh em bên trái
	//Trang p không có trang anh em bên phải
	//Trang parent là trang cha của trang p
	//Trang anh em bên trái của trang p là parent->key_item[pos - 1].next
	void underflowLeftPage(PTRPAGE& p, PTRPAGE parent, int pos)
	{
		//left_page là trang anh em bên trái của trang p
		PTRPAGE left_page = parent->key_item[pos - 1].next;
		if (left_page->key_num > M2)	//left_page có nhiều hơn n khóa
			//Lấy một khóa của trang left_page
			retrieveLeftPage(p, parent, pos, left_page);
		else							//left_page có n khóa
		{
			//Trộn trang p với trang left_page
			mergePage(left_page, parent, pos, p);
			p = left_page;
		}
	}

	//Xử lý trang p bị underflow
	//Trang parent là trang cha của trang p
	//p là trang con parent->key_item[pos].next;
	void underflow(PTRPAGE& p, PTRPAGE parent, int pos)
	{
		PTRPAGE left_page;
		PTRPAGE right_page;
		if (pos == -1)
			//p là trang con đầu tiên, chỉ có anh em bên phải
			left_page = NULL;			//p không có trang anh em bên trái
		else if (pos == 0)				//p có trang anh em bên trái
			//Trang anh em bên trái của trang p là trang con đầu tiên của trang cha parent
			left_page = parent->p_first;
		else
			//Trang anh em bên trái của trang p là trang parent->key_item[pos - 1].next
			left_page = parent->key_item[pos - 1].next;

		if (pos == parent->key_num - 1)
			//p là trang con cuối cùng, chỉ có trang anh em bên trái
			right_page = NULL;			//p không có trang anh em bên phải
		else							//p có trang anh em bên phải
			//Trang anh em bên phải của trang p là trang parent->key_item[pos + 1].next
			right_page = parent->key_item[pos + 1].next;

		if (left_page == NULL)
			//p không có trang anh em bên trái
			//Xử lý underflow với trang anh em bên phải
			underflowRightPage(p, parent, pos + 1);
		else if (right_page == NULL)
			//p không có trang anh em bên phải
			//Xử lý underflow với trang anh em bên trái
			underflowLeftPage(p, parent, pos);
		else
		{
			//p có trang anh em bên phải
			// Xét trang anh em bên phải của trang p
			if (right_page->key_num > M2)
				//Trang anh em bên phải right_page có nhiều hơn M2 khóa
				//Lấy một khóa của trang anh em bên phải
				retrieveRightPage(p, parent, pos + 1, right_page);
			else
			{
				//Trang anh em bên phải right_page có M2 khóa
				//Xét trang anh em bên trái
				if (left_page->key_num > M2)
					//Trang anh em bên trái left_page có nhiều hơn M2 khóa
					//Lấy một khóa của trang left_page
					retrieveLeftPage(p, parent, pos, left_page);
				else
					//Trộn trang p với trang right_page
					mergePage(p, parent, pos + 1, right_page);
			}
		}
	}

	//Loại bỏ khóa cực phải của cây con bên trái của element
	//element là phần tử khóa bị loại bỏ
	//Chép khóa cực phải của trang q vào khóa của phần tử element
	//Trang parent là trang của trang q
	void removeRightMostNode(PTRPAGE& q, PTRPAGE parent, int pos, ITEM& element)
	{
		int n = q->key_num - 1;
		if (q->key_item[n].next != NULL)		//q chưa là trang cực phải
		{
			//Loại bỏ khóa cực phải của trang cực phải q
			removeRightMostNode(q->key_item[n].next, q, n, element);

			if (q->key_num < M2)				//Trang q bị underflow
				//Xử lý trang q bị underflow với trang cha parent
				underflow(q, parent, pos);
		}
		else									//q là trang cực phải
		{
			//Chép khóa cực phải của trang q vào phần tử khóa element
			element.key = q->key_item[n].key;
			q->key_num--;						//Giảm số khóa của trang q

			if (q->key_num < M2)				//Trang q bị underflow
				//Xử lý trang q bị underflow với trang cha parent
				underflow(q, parent, pos);
		}
	}

	//Loại bỏ khóa the_key trong cây p
	//p là trang con parent->key_item[pos].next
	bool removeBTree(PTRPAGE& p, TYPEINFO the_key, PTRPAGE parent, int pos)
	{
		PTRPAGE q;
		int pos_key;
		if (p != NULL)
		{
			if (findPage(p, the_key, pos_key))	//Tìm thấy
			{
				if (pos_key != -1)				//Tìm thấy tại chỉ số pos_key
				{
					if (p->p_first == NULL)		//p là trang lá
					{
						//Loại bỏ khóa the_key trong trang p
						removeKey(p, pos_key);
						if (p->key_num < M2)	//Trang p bị underflow
						{
							if (p == root)		// p là trang root
							{
								if (p->key_num == 0)	//Trang root rỗng
								{
									delete p;
									root = NULL;
								}
							}
							else
								//Xử lý underflow của trang p
								underflow(p, parent, pos);
						}
					}
					else
					{
						//Tìm khóa cực phải của cây con bên trái q của trang p
						//q là trang con bên trái của p
						int position;
						if (pos_key == 0)
						{
							q = p->p_first;		//q là trang con đầu tiên của p
							position = -1;
						}
						else
						{
							//q là trang con p->key_item[pos_key - 1].next
							q = p->key_item[pos_key - 1].next;
							position = pos_key - 1;
						}

						//Trang q là trang con của khóa p->key_item[pos_key]
						//p->key_item[pos_key] là khóa bị loại bỏ
						//Loại bỏ khóa cực phải của cây q
						removeRightMostNode(q, p, position, p->key_item[pos_key]);

						if (p != root)					//p không là trang root
							if (p->key_num < M2)		//Trang p bị underflow
								//Xử lý underflow của trang p với trang cha parent
								underflow(p, parent, pos);
					}
				}
				return true;		//Loại bỏ được
			}
			else					//Không tìm thấy
			{
				if (p->p_first != NULL)	//p không là trang lá
				{
					if (pos_key == -1)
						q = p->p_first;	//q là trang con đầu tiên của p
					else
						//q là trang con p->key_item[pos_key].next
						q = p->key_item[pos_key].next;

					//Loại bỏ khóa the_key trong cây q
					//q là trang con p->key_item[pos_key].next
					removeBTree(q, the_key, p, pos_key);

					if (p != root)				//p không là trang root
						if (p->key_num < M2)	//Trang p bị underflow
							//Xử lý underflow của trang p với trang cha parent
							underflow(p, parent, pos);
				}
				return false;
			}
		}
		return false;
	}

	//Loại bỏ khóa the_key trong cây root
	//root là trang con của khóa parent->key_item[pos]
	//Trang ảo: pos = -2
	bool remove(PTRPAGE& root, TYPEINFO the_key)
	{
		//parent là trang cha của trang root (trang ảo)
		PTRPAGE parent = NULL;
		int pos = -2;				//pos = -1 (trang đầu tiên p_first)
		return removeBTree(root, the_key, parent, pos);
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
		init(root);
		char option = {};
		do
		{
			system("cls");
			cout << "Cac tac vu cua cay:" << endl;
			cout << "1. Kiem tra cay rong." << endl;
			cout << "2. Them khoa vao cay." << endl;
			cout << "3. Tim kiem khoa trong cay." << endl;
			cout << "4. Loai bo khoa trong cay." << endl;
			cout << "5. Duyet cay theo thu tu NLR." << endl;
			cout << "6. Huy bo cay." << endl;
			cout << "0. Ket thuc." << endl;
			cout << "Chon tac vu: ";
			cin >> option;
			switch (option)
			{
			case '1':		//Kiểm tra cây rỗng
			{
				//Kiểm tra cây root rỗng
				if (isEmpty(root))
					cout << "Cay rong" << endl;
				else cout << "Cay khong rong" << endl;
				system("pause");
				break;
			}
			case '2':		//Thêm khóa vào cây
			{
				int the_key;
				cout << "Nhap du lieu them vao: ";
				cin >> the_key;
				//Thêm the_key vào cây root
				insert(root, the_key);
				break;
			}
			case '3':		//Tìm kiếm khóa trong cây
			{
				int the_key;
				cout << "Nhap khoa can tim kiem: ";
				cin >> the_key;
				//Tìm the_key trong cây root
				if (find(root, the_key))
					cout << "Tim thay khoa " << the_key << endl;
				else
					cout << "Khong tim thay khoa " << the_key << endl;
				system("pause");
				break;
			}
			case '4':		//Loại bỏ khóa trong cây
			{
				int the_key;
				cout << "Nhap khoa muon loai bo: ";
				cin >> the_key;
				if (!remove(root, the_key))
				{
					cout << "Khong co khoa " << the_key << endl;
					system("pause");
				}
				break;
			}
			case '5':		//Duyệt cây theo thứ tự NLR
			{
				if (isEmpty(root))
					cout << "Cay rong" << endl;
				else
					traverseBTree(root);
				system("pause");
				break;
			}
			case '6':		//Hủy bỏ cây
			{
				clear(root);
				break;
			}
			default:
				break;
			}
		} while (option != '0');
	}
#pragma endregion

}