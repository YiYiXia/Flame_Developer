/* Copyright(c) 2018 YimingXia(491691865@qq.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#pragma once
#include<iostream>
template<class T1>
struct LNode {
	T1 num;
	struct LNode<T1>* next;
};

template<class T>
class LinkList
{
public:
	LinkList();
	~LinkList();
	int ListLength;

	LNode<T>* L;
	LNode<T>* Head, Nil;
	void AddLNode(T num);
	void Delete(T num);
	T Search(int num);
	void Print();
	bool IsEmpty();
};
template<typename T>
LinkList<T>::LinkList()
{
	L = (LNode<T>*)malloc(sizeof(LNode<T>));
	L->next = NULL;
	L->num = NULL;
	ListLength = 0;
}
template<typename T>
LinkList<T>::~LinkList()
{

}
template<typename T>
void LinkList<T>::AddLNode(T num)
{
	LNode<T>* s;
	s = (LNode<T>*)malloc(sizeof(LNode<T>));
	s->num = num;
	s->next = NULL;
	if (L->next == NULL) L->next = s;
	else
	{
		s->next = L->next;
		L->next = s;
	}
	ListLength++;
}
template<typename T>
void LinkList<T>::Delete(T num)
{
	LNode<T>* p;
	LNode<T>* q;
	if (L->next == NULL) return;
	p = L;
	q = L->next;
	int n = 1;
	while (n != num)
	{
		p = p->next;
		q = q->next;
		n++;
	}
	p->next = q->next;
	free(q);
	ListLength--;
}
template<typename T>
T LinkList<T>::Search(int num)
{
	LNode<T>*  q;
	if (L->next == NULL) return 0;
	q = L->next;
	int n = 1;
	while (n != num)
	{
		q = q->next;
		n++;
	}
	return q->num;
}
template<typename T>
void LinkList<T>::Print()
{
	LNode<T>* p;
	if (L->next == NULL) return;
	p = L->next;
	while (p != NULL)
	{
		std::cout << p->num << std::endl;
		p = p->next;
	}
}

template<typename T>
bool LinkList<T>::IsEmpty()
{
	if (L->next == NULL) return false;
	else return true;
}