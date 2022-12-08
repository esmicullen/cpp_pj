#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>    
#include<fstream>    
using namespace std;

//�ܼ� ������ Ư�� ��ġ�� Ŀ���� �̵���Ű�� �Լ�
void gotoxy(int x, int y)
{
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//keyboard �Է�
int GetKeyDown()
{
    if (_kbhit() != 0)        //Ű���忡 ���� �Է��� ����
    {
        return _getch();    //�Էµ� Ű���� ��ȯ.
    }
    return 0;
}

//�ܼ� ����
void SetConsoleView()
{
    system("mode con:cols=80 lines=30");        //ȭ�� ũ�� ���� : con:cols -> ���� , lines -> ���� (0��° �� ~ 29��° ��)
    system("title WOOAHAE");    //���� �̸� ���� :  title (�����̸�)
}

//����ȭ�� draw
void DrawReadyGame()
{
    system("cls");
    gotoxy(9, 9);
    cout << "=============================================";
    gotoxy(9, 10);
    cout << "============== HANG MAN GAME ================";
    gotoxy(9, 11);
    cout << "=============================================";
    gotoxy(14, 17);
    cout << ">> Press S to Start <<";
    gotoxy(35, 29);
    cout << "E to Exit";
}

//����ȭ�� draw
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
    system("cls");
    gotoxy(5, 1);
    cout << "life = " << life;
    gotoxy(5, 2);
    cout << "score = " << score;
    gotoxy(5, 8);
    cout << "past = ";
    for (int i = 0; i < static_cast<int>(pastWord.size()); ++i)
    {
        cout << pastWord[i] << " ";
    }

    gotoxy(5, 12);
    cout << "input = ";
    gotoxy(13, 14);
    cout << "����ȭ�� 'main'" << endl;
}

//������ �����ϴ� �Լ� �Դϴ�. << ���� �κ� ���� �� �����ؾߵ�!
void SetDictionary(vector<string>& strArr)
{
    static const int INIT_NUM = 4;
    static const string str[INIT_NUM] = { "apple", "banana", "code", "program" };    //���� �ܾ��
    ifstream readFromFile("words.txt");        //words.txt ������ �б� �������� ����
    if (!readFromFile.is_open())            //is_open�� ���� �ʴ´ٴ°��� ������ �������� �ʴٴ� ��.
    {
        ofstream writeToFile("words.txt");    //���� �������� words.txt ������ ���� (������ ������ �ڵ� ������)
        for (int i = 0; i < INIT_NUM; ++i)    //���� �ܾ���� format�� �°� words.txt ���Ͽ� �Է�
        {
            string tmp = str[i];
            if (i != INIT_NUM - 1)
            {
                tmp += "\n";
            }
            writeToFile.write(tmp.c_str(), tmp.size());    //���Ͽ� ���� �Լ�
            strArr.push_back(str[i]); //�ܾ���(strArr)�� �ܾ ����ֽ��ϴ�.
        }
        writeToFile.close();    //������������ �ݱ�
        return;                    //�Լ���
    }

    //����� �Դٴ°��� �б��������� ���Ͽ��� �Ǿ��ٴ¶�.
    while (!readFromFile.eof())    //���� ������
    {
        string tmp;
        getline(readFromFile, tmp);    //���پ� �о
        strArr.push_back(tmp);        //�ܾ���(strArr)�� �ܾ� �ֱ�
    }
    readFromFile.close();        //�б��������� �ݱ�
    return;
}

//����ȭ�� ���
bool ReadyGame()
{
    DrawReadyGame();    //����ȭ�� �׸���
    while (true)
    {
        int key = GetKeyDown();                //Ű�� ������
        if (key == 's' || key == 'S')        //s�� ��ŸƮ
        {
            return true;
        }
        else if (key == 'e' || key == 'E')    //e�� ��
        {
            break;
        }
    }
    return false;
}

//���� ���� �Լ�.
void StartGame()
{
    int score = 0;
    vector<string> pastWord;    //�Է��� ���ܾ� ����
    vector<string> strArr;        //���� �ܾ���
    SetDictionary(strArr);        //read from file

    while (true)    //�ϳ��� ź�� ǥ���ϴ� ����
    {
        //1 play
        int num = 0;
        srand((unsigned int)time(NULL));    //�����Լ�
        num = rand() % static_cast<int>(strArr.size());    //�ܾ��� ���� ������ �ܾ� ����

        string strQuestion;                        // _ _ _ _ _ �� ǥ���� ����
        const string strOriginal = strArr[num];    //�ܾ �´��� ����Ȯ�ο����� ����
        const int originLen = static_cast<int>(strOriginal.length());

        //init
        for (int i = 0; i < originLen; ++i)
        {
            strQuestion += "_";    //���� ���̸�ŭ "_"
        }

        int life = originLen + 2;    //������� ���� �ܾ� ���� + 2

        //1 question
        while (true)    //�ϳ��� �ܾ ���ߴ� ����
        {
            DrawStartGame(life, score, pastWord);    //���ܾ�, �����, ����ǥ��

            //draw question
            gotoxy(5, 5);
            for (int i = 0; i < originLen; ++i)
            {
                cout << strQuestion[i] << " ";    // _ _ _ _ ǥ��
            }
            cout << endl;

            //input
            gotoxy(9, 12);
            string strInput;
            cin >> strInput;    //�Է� �ޱ�.
            if (strInput == "main")
            {
                return;
            }
            pastWord.push_back(strInput);    //�ѹ� �Է��� �ܾ�� pastword���� ǥ��

            if (strInput.length() == 1)        //�Է¹��� ��Ʈ���� ���̰� 1�ΰ��
            {
                //alphabet
                for (int i = 0; i < originLen; ++i)
                {
                    if (strOriginal[i] == strInput[0])    //�������� �ܾ �Է��� ���ĺ��� �ִ°��
                    {
                        strQuestion[i] = strInput[0];    // �ش� ��ġ�� "_" �� ���ĺ����� �ٲ��� 
                    }
                }
            }
            else if (strInput.length() > 1)    //�Է¹��� ��Ʈ���� ���̰� 1���� ū ���
            {
                //word
                if (strOriginal == strInput) //�������� �ܾ�� �Է� �ܾ ������ (����)
                {
                    //score up !!
                    score += 5;
                    pastWord.clear();
                    break;    //�ϳ��� �ܾ ���ߴ� ������ �����Եǰ� ���� ź���� �Ѿ�� �˴ϴ�.
                }
            }

            //Ʋ���ų�, �°ų� ��¶�� �Է��� �Ǹ� �������� ������ 1���� ���Դϴ�.
            life -= 1;
            if (life < 0)
            {
                score -= 5;
                if (score < 0)
                {
                    score = 0;
                }
                pastWord.clear();
                break;
            }
        }
    }
}

//�����Լ�
int main(void)
{
    SetConsoleView();
    bool isStart = false;
    while (true)
    {
        isStart = ReadyGame();    //readgame�Լ��� true��
        if (isStart)
        {
            StartGame();        //���ӽ���
        }
        else
        {
            break;
        }
    }
    return 0;
}
