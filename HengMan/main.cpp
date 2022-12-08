#include<iostream>
#include<string>
#include<ctime>
#include<cstdlib>
#include<conio.h>
#include<Windows.h>
#include<vector>    
#include<fstream>    
using namespace std;

//콘솔 내부의 특정 위치로 커서를 이동시키는 함수
void gotoxy(int x, int y)
{
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//keyboard 입력
int GetKeyDown()
{
    if (_kbhit() != 0)        //키보드에 뭔가 입력이 오면
    {
        return _getch();    //입력된 키값을 반환.
    }
    return 0;
}

//콘솔 세팅
void SetConsoleView()
{
    system("mode con:cols=80 lines=30");        //화면 크기 조정 : con:cols -> 가로 , lines -> 세로 (0번째 줄 ~ 29번째 줄)
    system("title WOOAHAE");    //게임 이름 설정 :  title (게임이름)
}

//시작화면 draw
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

//게임화면 draw
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
    cout << "메인화면 'main'" << endl;
}

//사전을 세팅하는 함수 입니다. << 여기 부분 수정 및 설정해야됨!
void SetDictionary(vector<string>& strArr)
{
    static const int INIT_NUM = 4;
    static const string str[INIT_NUM] = { "apple", "banana", "code", "program" };    //샘플 단어들
    ifstream readFromFile("words.txt");        //words.txt 파일을 읽기 전용으로 오픈
    if (!readFromFile.is_open())            //is_open이 되지 않는다는것은 파일이 존재하지 않다는 뜻.
    {
        ofstream writeToFile("words.txt");    //쓰기 전용으로 words.txt 파일을 오픈 (파일이 없으면 자동 생성됨)
        for (int i = 0; i < INIT_NUM; ++i)    //샘플 단어들을 format에 맞게 words.txt 파일에 입력
        {
            string tmp = str[i];
            if (i != INIT_NUM - 1)
            {
                tmp += "\n";
            }
            writeToFile.write(tmp.c_str(), tmp.size());    //파일에 쓰는 함수
            strArr.push_back(str[i]); //단어장(strArr)에 단어를 집어넣습니다.
        }
        writeToFile.close();    //쓰기전용파일 닫기
        return;                    //함수끝
    }

    //여기로 왔다는것은 읽기전용으로 파일오픈 되었다는뜻.
    while (!readFromFile.eof())    //파일 끝까지
    {
        string tmp;
        getline(readFromFile, tmp);    //한줄씩 읽어서
        strArr.push_back(tmp);        //단어장(strArr)에 단어 넣기
    }
    readFromFile.close();        //읽기전용파일 닫기
    return;
}

//시작화면 기능
bool ReadyGame()
{
    DrawReadyGame();    //시작화면 그리기
    while (true)
    {
        int key = GetKeyDown();                //키가 들어오면
        if (key == 's' || key == 'S')        //s는 스타트
        {
            return true;
        }
        else if (key == 'e' || key == 'E')    //e는 끝
        {
            break;
        }
    }
    return false;
}

//게임 시작 함수.
void StartGame()
{
    int score = 0;
    vector<string> pastWord;    //입력한 영단어 저장
    vector<string> strArr;        //맞출 단어장
    SetDictionary(strArr);        //read from file

    while (true)    //하나의 탄을 표현하는 루프
    {
        //1 play
        int num = 0;
        srand((unsigned int)time(NULL));    //랜덤함수
        num = rand() % static_cast<int>(strArr.size());    //단어장 내에 랜덤한 단어 선택

        string strQuestion;                        // _ _ _ _ _ 로 표현할 변수
        const string strOriginal = strArr[num];    //단어가 맞는지 정답확인용으로 저장
        const int originLen = static_cast<int>(strOriginal.length());

        //init
        for (int i = 0; i < originLen; ++i)
        {
            strQuestion += "_";    //정답 길이만큼 "_"
        }

        int life = originLen + 2;    //생명력은 정답 단어 길이 + 2

        //1 question
        while (true)    //하나의 단어를 맞추는 루프
        {
            DrawStartGame(life, score, pastWord);    //사용단어, 생명력, 점수표기

            //draw question
            gotoxy(5, 5);
            for (int i = 0; i < originLen; ++i)
            {
                cout << strQuestion[i] << " ";    // _ _ _ _ 표기
            }
            cout << endl;

            //input
            gotoxy(9, 12);
            string strInput;
            cin >> strInput;    //입력 받기.
            if (strInput == "main")
            {
                return;
            }
            pastWord.push_back(strInput);    //한번 입력한 단어는 pastword에서 표기

            if (strInput.length() == 1)        //입력받은 스트링의 길이가 1인경우
            {
                //alphabet
                for (int i = 0; i < originLen; ++i)
                {
                    if (strOriginal[i] == strInput[0])    //오리지널 단어에 입력한 알파벳이 있는경우
                    {
                        strQuestion[i] = strInput[0];    // 해당 위치의 "_" 를 알파벳으로 바꿔줌 
                    }
                }
            }
            else if (strInput.length() > 1)    //입력받은 스트링의 길이가 1보다 큰 경우
            {
                //word
                if (strOriginal == strInput) //오리지널 단어랑 입력 단어가 같을때 (정답)
                {
                    //score up !!
                    score += 5;
                    pastWord.clear();
                    break;    //하나의 단어를 맞추는 루프를 나가게되고 다음 탄으로 넘어가게 됩니다.
                }
            }

            //틀리거나, 맞거나 어쨋든 입력이 되면 라이프가 무조건 1개씩 깎입니다.
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

//메인함수
int main(void)
{
    SetConsoleView();
    bool isStart = false;
    while (true)
    {
        isStart = ReadyGame();    //readgame함수가 true면
        if (isStart)
        {
            StartGame();        //게임시작
        }
        else
        {
            break;
        }
    }
    return 0;
}
