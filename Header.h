#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <string>

using namespace std; 
class Node
{
public:
	int in;// ���������� ���������
	char symbol;// ������
	Node* left, * right;//��������� �� ����� � ������ ������

	Node()
		//// ����������� �� ���������
	{
		left = right = NULL;
	}

	Node(Node* L, Node* R)// ����������� ��������� ������ �� ������ � ����� �������
		//����������� ��� parent
	{
		left = L;
		right = R;
		in = L->in + R->in;
	}
};
struct MyCompare//��������� ��� ��������� � ����������
{
	bool operator()(const Node* l, const Node* r)//���������� ���������
		const
	{
		return l->in < r->in;// ���������� true, ���� ������ �������� ������
	}
};
vector<bool> code;
// ������ ��� ����             
map<char, vector<bool> > table;
//������������� ����� ������-��� 
void BuildTable(Node* root)//������� ��������� �������� �������
{
	if (root->left != NULL)//���� ����� �� 0
	{
		code.push_back(0);//���������� � ��������������� ������ 0
		BuildTable(root->left);//��������� ����������� ������� ��� ������ ��������
	}

	//���� ����� ��� ������� ��������, ������ ������ � ����-� 1
	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
	}
	//���� ��� "���������"(� ������) ����, 
	//�� ����������� ��� ����� � �����
	if (root->left == NULL && root->right == NULL)
		table[root->symbol] = code;

	//������� � ����� 1 ������
	if (!code.empty())//�������� �� �������
		code.pop_back();//������������ ������� �� 1��-�
}


