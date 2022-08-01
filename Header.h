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
	int in;// количество вхождений
	char symbol;// символ
	Node* left, * right;//указатели на левый и правый символ

	Node()
		//// конструктор по умолчанию
	{
		left = right = NULL;
	}

	Node(Node* L, Node* R)// конструктор принимает ссылки на правый и левый элемент
		//конструктор для parent
	{
		left = L;
		right = R;
		in = L->in + R->in;
	}
};
struct MyCompare//Структура для сравнения и сортировки
{
	bool operator()(const Node* l, const Node* r)//перегрузка оператора
		const
	{
		return l->in < r->in;// возвращает true, если правый параметр больше
	}
};
vector<bool> code;
// вектор для кода             
map<char, vector<bool> > table;
//ассоциативная связь символ-код 
void BuildTable(Node* root)//функция принимает корневой элемент
{
	if (root->left != NULL)//если слева не 0
	{
		code.push_back(0);//записываем в вспомогательный вектор 0
		BuildTable(root->left);//запускаем рекурсивную функцию для левого элемента
	}

	//тоже самое для правого элемента, только ставим в соот-е 1
	if (root->right != NULL)
	{
		code.push_back(1);
		BuildTable(root->right);
	}
	//если это "начальный"(с буквой) узел, 
	//то ассоциируем эту букву с кодом
	if (root->left == NULL && root->right == NULL)
		table[root->symbol] = code;

	//удаляем с конца 1 символ
	if (!code.empty())//проверка на пустоту
		code.pop_back();//возвращаемся обратно на 1эл-т
}


