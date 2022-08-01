#include <filesystem>
#include "Header.h"
using namespace std;

void encode() {
  
        string name1;
        cout << "Введите имя файла" << endl;
        cin >> name1;
        ifstream f;
        f.open(name1);
        if (!f) cout << "Файл не открылся  " << name1 << endl;
        vector <char> read1;
        double chastota = 0;// считаем частоты символов 
        map<char, int> m;//ассоциации: символ(ключ) - число вхождений
        while (!f.eof())//читаем текст из файла посимвольно
        {
            char buf;
            f.read(&buf, 1);
            read1.push_back(buf);
            m[buf]++;
            chastota++;
        }
        
        for (auto i : read1)
        {
            cout << i;
        }

        cout << endl;
        map <char, int> ::iterator itr;
        for (itr = m.begin(); itr != m.end(); itr++)
        {
            cout << itr->first << "-встречается раз: " << itr->second << endl;
        }

        ////// записываем начальные узлы в список list

        list<Node*> t;//лист указателей на Node
        for (itr = m.begin(); itr != m.end(); ++itr)
        {
            Node* p = new Node;//Создаем указатель на Node,Создаем в динамической памяти указатель на узел
            p->symbol = itr->first;//first указывает на char
            p->in = itr->second;//second указывает на int
            t.push_back(p);//указатель записываем в list в конец строки
        }
        // создаем дерево      

        while (t.size() != 1)//пока в списке не останется 1 элемент(на каждом шаге удаляем(объединяем) 2, доб. 1)
        {
            t.sort(MyCompare());//Сортируем List по числу чхождений

            Node* SonL = t.front();//первый элемент в списке
            t.pop_front();//первый элемент удаляем
            Node* SonR = t.front();//ставим 2 элемент на первое место
            t.pop_front();//удаляем второй элемент

            Node* parent = new Node(SonL, SonR);//создаем новый узел, 
            //у которого число вхождений равно сумме удаленных узлов, его левый указатель-1й удаленный эл-т
            t.push_back(parent);//заносим переменную parent в список

        }
        Node* root = t.front();   // указатель на вершину дерева
        //таблица значений кодов с помощью рекурсии:
        BuildTable(root);
       

        f.clear();
        f.seekg(0); // перемещаем указатель снова в начало файла

        ofstream g("bin.txt", ios::out | ios::binary);
        ofstream h("2.txt", ios::out | ios::binary);
        /*Строка с закодированным файлом*/
        //string decoded_;
        /*Заглавление файла*/
        //decoded_ += (int)m.size();//кол-во символов в таблице кодов
        //decoded_ += (int)chastota;//кол-во символов в исх тексте
        /*for (auto& i : table)
        {
            decoded_ += i.second.size();//кол-во нулей и едениц в коде символа
            decoded_ += i.first;//исходный символ
            string s;
            for (int l = 0; l < i.second.size(); l++)
                s += (i.second[l] + '0');
            decoded_ += strtol(s.c_str(), NULL, 2);// "101"->5 {0,256}
        }*/

        double pl;
        pl = 0;//длина
        int count = 0;//счетчик для указателя 
        char buf = 0;//временная переменая,в которой храним биты
        string messageCode;
        while (!f.eof())
        {
            char symbol = f.get();//считываем символ из файла
            vector<bool> x = table[symbol];//смотрим ее код
            //выводим коды
            for (int n = 0; n < x.size(); n++)
            {
                pl++;
                //проверяем биты, buf-8 нулей изначально
                buf = buf | x[n] << (7 - count);//побитовое сложение,копирует слева-направо
                count++;
                if (count == 8)
                {
                    count = 0;
                    messageCode.push_back(buf);
                    buf = 0;
                }
                g << x[n];
            }

        }
        if (count!=0)
        {
            buf >> (8 - count);
            messageCode.push_back(buf);
            
        }
        
        int ost = 8 - count;
        h << messageCode;
        pl = pl / 8;
        double compressCoef = chastota / pl;
        cout << compressCoef << endl;
        cout << messageCode;
        h.close();
        f.close();
        g.close();

        //Выводим таблицу кодоов в файл 
        ofstream k("table_symbol.txt", ios::out | ios::binary);
        map<char, vector<bool> >::iterator it;
        vector<bool>::iterator ii;
        int l = 0;
        k << ost << ' ';
        for (it = table.begin(); it != table.end(); it++)
        {

            k << it->first;
            if (it->second.size() < 10)k << '0';
            k << it->second.size();
            for (ii = table[it->first].begin(); ii != table[it->first].end(); ii++)
                k << (*ii);

        }
        k.close();
       
 

}
void decode() {
   
    ifstream k("table_symbol.txt");
    setlocale(LC_ALL, "Russian"); //локализация символов
    //читаем таблицу кодов
    int empty;
    map<char, vector <bool> >codes;
    k >> empty;//остатки
    char letter;
    k.read(&letter, sizeof(letter));//пробел
    while (!k.eof())
    {
       k.read(&letter, sizeof(letter));//символ
       if (k.eof())break;
       char sizep[2];
       k.read(sizep, sizeof(sizep));//кол-во символов в двоичном коде символа
       int size = atoi(sizep);//преобразование строки в число int
       for (int i = 0; i < size; i++)//чтение двоичного кода
       {
             char boolbuf;
             k.read(&boolbuf, sizeof(boolbuf));
             if (boolbuf == '1') codes[letter].push_back(true);
             else codes[letter].push_back(false);
       }
    }
    k.close();
    map<char, vector<bool> >::iterator it;
    vector<bool>::iterator ii;
    //таблица кодов в консоль
    for (it = codes.begin(); it != codes.end(); it++)
    {
        cout << endl;
        cout << it->first << " : ";
        for (ii = codes[it->first].begin(); ii != codes[it->first].end(); ii++)
            cout << (*ii);
       
    }
    cout << endl;

    map<vector <bool>, char> table_code;

    for (auto iter = codes.begin(); iter != codes.end(); iter++)
    {
         table_code[iter->second] = iter->first;
    }
  
    bool flag1 = false;
    vector <bool> bufstr;
    ifstream F("2.txt", ios::in | ios::binary);
    while (!F.eof())
    {
        char letter;
        F.read(&letter, sizeof(letter));//читаем символ
        if (F.eof())break;
        for (int i = 7; i > -1; i--)
        {
           int b = 1 << i; //сдвиг
           //при первой итерации 1 станет (10000000)-128
           //дальше 64,32,16,8,4,2,1
           if ((letter & b) == b)bufstr.push_back(true);//поразрядное умножение-(проверяем 8-i бит символа начиная слева) 
           //если на этом месте у символа стоит 1ка(проверям  путем сравнения с b) то добавляем в вектор 1
           else bufstr.push_back(false);//, иначе-0
        }
        int iterat = 0;
        auto pos = F.tellg();//текущая позиция в файле
        F.read(&letter, sizeof(letter));//читаем след символ
        if (F.eof())//если пришли в конец файла
        {
           for (int i = 1; i < empty; i++)// до числа остатков(кол-во бит, которых не хватило до 8 при архивировании)
           {
             bufstr.pop_back(); flag1 = true;
           }
        }
        else F.seekg(pos);//на 1 символ назад

       //выведем двоичный код для проверки
        for (vector<bool>::iterator itr = bufstr.begin(); itr != bufstr.end(); ++itr)
            cout << *itr;
        cout << endl;
        
        ofstream v("razarhiv.txt", ios::out | ios::binary);//вывод символа
        do
        {
            vector <bool> outstr;
            do
            {
                outstr.push_back(bufstr[iterat]);
                iterat++;
                if (iterat == bufstr.size())break;
            } while (!table_code.count(outstr));
            if (iterat == bufstr.size())break;
            v << table_code[outstr];
            vector <bool>::iterator it1 = bufstr.begin(), it2 = bufstr.begin();
            advance(it2, iterat);//перемещает it2 на iterat вперед
            bufstr.erase(it1, it2);//удалит в диапозоне
            outstr.clear();
            iterat = 0;
        } while (iterat != bufstr.size());
    }
    //выведем двоичный код для проверки
    for (vector<bool>::iterator itr = bufstr.begin(); itr != bufstr.end(); ++itr)
        cout << *itr;
    F.close();
    


    
}

int main()
{
    setlocale(LC_ALL, "Russian");
    cout << "Введите: Архивировать-1, Разархивировать-0" << endl;
    bool flag;
    cin >> flag;
    if (flag == true) encode(); else decode();
   
}
