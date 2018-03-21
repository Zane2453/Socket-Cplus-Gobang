#include<iostream>
#include<string>
#include<cstdlib>
#include<winsock2.h>
#include<assert.h>
using namespace std;

int board[22][22];

void show_board();
bool put_check(int,int,int,bool);
bool winner_check(int i,int j,int whosturn);

int main(){
    int iResult;
    WSAData wsaData;
    WORD version = MAKEWORD(2,1);
    iResult = WSAStartup(version, &wsaData);
    if(iResult != 0){
        cout<<"WinSock initialize error!";
    }

    SOCKET server_socket = INVALID_SOCKET;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        cout<<"Socket error!";
    }

    SOCKADDR_IN addr;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);

    int r = bind(server_socket, (SOCKADDR*)&addr, sizeof(addr));
    assert(r >= 0);

    int l = listen(server_socket, SOMAXCONN);

    SOCKET sConnect;
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    sConnect = accept(server_socket, (SOCKADDR*)&clientAddr, &clientAddrLen);
    if (sConnect != INVALID_SOCKET)
    {
        cout<<"server: got connection from "<<inet_ntoa(clientAddr.sin_addr)<<endl;
        system("pause");
    }

    //�ѽL����l�Ƴ]�w
    for(int i=0; i<=21; i++){
        for(int j=0; j<=21; j++){
            if(i==0 && j==0){
                 board[i][j] = 0;
            }
            else if(i==21 || j==21){
                 board[i][j] = 0;
            }
            //set the column index
            else if(i==0){
                 board[i][j] = j+30;
            }
            //set the row index
            else if(j==0){
                 board[i][j] = i+10;
            }
            //set the line of board
            else if(i==1 && j==1){
                 board[i][j] = -7;
            }
            else if(i==1 && j==20){
                 board[i][j] = -9;
            }
            else if(i==20 && j==1){
                 board[i][j] = -1;
            }
            else if(i==20 && j==20){
                 board[i][j] = -3;
            }
            else if(i==1){
                 board[i][j] = -8;
            }
            else if(j==1){
                 board[i][j] = -4;
            }
            else if(j==20){
                 board[i][j] = -6;
            }
            else if(i==20){
                 board[i][j] = -2;
            }
            else{
                 board[i][j] = -5;
            }
        }
     }

    //��Ӫ��a,Server�ݥ��U
    char *keyin = new char[100];
    //�ΨӰO���O�_���F��qclient�ݶǨ�
    char *check_connect = new char[1];
    int I,J,whosturn=1;
    bool end_check;
    system("cls");
    show_board();
    I=0;
    J=0;
    end_check = 1;
    while(1){
        //��֪ܽ��^�X
        if(whosturn == 2)
            cout << "Enemy's turn�G";
        else
            cout << "Your turn�G";
        //server�ݪ��^�X
        if(whosturn==1){
            cin >> keyin;
            //�B�z�r��keyin
            for(int n=0 ;n<(int)strlen(keyin) ;n++){
                //��J��a~t
                if(keyin[n]>=97 && keyin[n]<=116)
                    J = keyin[n]-96;
                //��J��A~T
                else if(keyin[n]>=65 && keyin[n]<=84)
                    J = keyin[n]-64;
                //��J���Ʀr0~9
                else if(keyin[n]>=48 && keyin[n]<=57)
                    I = I*10+keyin[n]-48;
                else if(keyin[n] == ' ')
                    continue;
                else{
                    cout <<"Illegal character!"<< endl;
                    system("pause");
                    goto no_winner;
                }
            }
            end_check=put_check(I,J,whosturn,end_check);
            if(end_check == 0)
                goto no_winner;
            end_check = winner_check(I,J,whosturn);
            //�P�_���U�ӬO�֪��^�X
            whosturn = (whosturn == 1? 2:1);
            if(end_check == 0){
                check_connect[0]='1';
                send(sConnect, check_connect, (int)strlen(check_connect), 0);
                check_connect[0]='0';
                //�N�r��keyin�ǵ�client��
                send(sConnect, keyin, (int)strlen(keyin), 0);
                goto no_winner;
            }
            //���HĹ�F
            else{
                check_connect[0]='1';
                send(sConnect, check_connect, (int)strlen(check_connect), 0);
                check_connect[0]='0';
                //�N�r��keyin�ǵ�client��
                send(sConnect, keyin, (int)strlen(keyin), 0);
                whosturn = (whosturn == 1? 2:1);
                system("cls");
                show_board();
                if(whosturn == 1)
                    cout <<"Server win!"<< endl;
                else
                    cout <<"Client win!"<< endl;
                break;
            }
        }
        //client�ݪ��^�X
        else{
            recv(sConnect, check_connect, (int)strlen(check_connect), 0);
            if(check_connect[0]=='1'){
                check_connect[0]='0';
                ZeroMemory(keyin, 100);
                //�N�qclient�ݦ��쪺�r��s��keyin
                recv(sConnect, keyin, sizeof(keyin), 0);
                for(int n=0; n<(int)strlen(keyin); n++){//�B�z�r��keyin
                   //��J��a~t
                    if(keyin[n]>=97 && keyin[n]<=116)
                        J = keyin[n]-96;
                    //��J��A~T
                    else if(keyin[n]>=65 && keyin[n]<=84)
                        J = keyin[n]-64;
                    //��J���Ʀr0~9
                    else if(keyin[n]>=48 && keyin[n]<=57)
                        I = I*10+keyin[n]-48;
                    else if(keyin[n] == ' ')
                        continue;
                    else{
                        cout << "Illegal character!" << endl;
                        system("pause");
                        goto no_winner;
                    }
                }
                end_check=put_check(I,J,whosturn,end_check);
                if(end_check == 0)
                    goto no_winner;
                end_check = winner_check(I,J,whosturn);
                //�P�_���U�ӬO�֪��^�X
                whosturn = (whosturn == 1? 2:1);
                if(end_check == 0)
                    goto no_winner;
                //���HĹ�F
                else{
                    whosturn = (whosturn == 1? 2:1);
                    system("cls");
                    show_board();
                    if(whosturn == 1)
                        cout <<"Server win!"<< endl;
                    else
                        cout <<"Client win!"<< endl;
                    break;
                }
            }
            else{
                system("cls");
                show_board();
                cout<<"Client's connection break!";
                break;
            }
        }
no_winner:
        I=0;
        J=0;
        end_check = 1;
        system("cls");
        show_board();
    }
    return 0;
}

void show_board(){//��ܴѽL
    for(int i=0 ;i<=21 ;i++){
        for(int j=0 ;j<=21 ;j++){
            switch(board[i][j]){
                case -1:
                    cout << "�|";
                    break;
                case -2:
                    cout << "�r";
                    break;
                case -3:
                    cout << "�}";
                    break;
                case -4:
                    cout << "�u";
                    break;
                case -5:
                    cout << "�q";
                    break;
                case -6:
                    cout << "�t";
                    break;
                case -7:
                    cout << "�z";
                    break;
                case -8:
                    cout << "�s";
                    break;
                case -9:
                    cout << "�{";
                    break;
                case 0:
                    cout << "�@";
                    break;
                case 1:
                    cout << "��";
                    break;
                case 2:
                    cout << "��";
                    break;
                case 11:
                    cout << "��";
                    break;
                case 12:
                    cout << "��";
                    break;
                case 13:
                    cout << "��";
                    break;
                case 14:
                    cout << "��";
                    break;
                case 15:
                    cout << "��";
                    break;
                case 16:
                    cout << "��";
                    break;
                case 17:
                    cout << "��";
                    break;
                case 18:
                    cout << "��";
                    break;
                case 19:
                    cout << "��";
                    break;
                case 20:
                    cout << "10";
                    break;
                case 21:
                    cout << "11";
                    break;
                case 22:
                    cout << "12";
                    break;
                case 23:
                    cout << "13";
                    break;
                case 24:
                    cout << "14";
                    break;
                case 25:
                    cout << "15";
                    break;
                case 26:
                    cout << "16";
                    break;
                case 27:
                    cout << "17";
                    break;
                case 28:
                    cout << "18";
                    break;
                case 29:
                    cout << "19";
                    break;
                case 30:
                    cout << "20";
                    break;
                case 31:
                    cout << "��";
                    break;
                case 32:
                    cout << "��";
                    break;
                case 33:
                    cout << "��";
                    break;
                case 34:
                    cout << "��";
                    break;
                case 35:
                    cout << "��";
                    break;
                case 36:
                    cout << "��";
                    break;
                case 37:
                    cout << "��";
                    break;
                case 38:
                    cout << "��";
                    break;
                case 39:
                    cout << "��";
                    break;
                case 40:
                    cout << "��";
                    break;
                case 41:
                    cout << "��";
                    break;
                case 42:
                    cout << "��";
                    break;
                case 43:
                    cout << "��";
                    break;
                case 44:
                    cout << "��";
                    break;
                case 45:
                    cout << "��";
                    break;
                case 46:
                    cout << "��";
                    break;
                case 47:
                    cout << "��";
                    break;
                case 48:
                    cout << "��";
                    break;
                case 49:
                    cout << "��";
                    break;
                case 50:
                    cout << "��";
                    break;
                }
        }
        cout << endl;
    }
}

bool put_check(int i,int j,int whosturn,bool end_check){//�P�_�i�_�U�o��A�p�G�i�H���W�аO
     if(i == 0 || i >= 21 || j == 0 || j >= 21){
        cout <<"Out of the range!"<< endl;
        end_check=0;
        system("pause");
     }
     else if(board[i][j] == 1 || board[i][j] == 2){
        cout <<"Here has a piece!"<< endl;
        end_check=0;
        system("pause");
     }
     //���W�аO
     else
        board[i][j]=whosturn;
     return end_check;
}

bool winner_check(int i,int j,int whosturn){//�p�Gwin = 1�N���H�ӧQ�F
    int count,temp1,temp2;
    bool win=0;
   //�P�_���k���_�����s�u
    count=1;
    temp1=i;
    while(board[--temp1][j] == whosturn)
        count++;
    temp1=i;
    while(board[++temp1][j] == whosturn)
        count++;
    if(count >= 5){
        win=1;
        goto winner_check_end;
    }
    //�P�_�W�U���_�����s�u
    count=1;
    temp2=j;
    while(board[i][--temp2] == whosturn)
        count++;
    temp2 = j;
    while(board[i][++temp2] == whosturn)
        count++;
    if(count >= 5){
        win=1;
        goto winner_check_end;
    }
    //�P�_���W��k�U���_�����s�u
    count=1;
    temp1=i;
    temp2=j;
    while(board[--temp1][--temp2] == whosturn)
        count++;
    temp1=i;
    temp2=j;
    while(board[++temp1][++temp2] == whosturn)
        count++;
    if(count >= 5){
        win=1;
        goto winner_check_end;
    }
    //�P�_���U��k�W���_�����s�u
    count=1;
    temp1=i;
    temp2=j;
    while(board[++temp1][--temp2] == whosturn)
        count++;
    temp1=i;
    temp2=j;
    while(board[--temp1][++temp2] == whosturn)
        count++;
    if(count >= 5){
        win=1;
        goto winner_check_end;
    }
winner_check_end:
    return win;
}
