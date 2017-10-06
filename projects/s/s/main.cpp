#include<windows.h>
#include<cstdio>
#include<iostream>
#include <string> 
#include <fstream>
#include<vector>


int main() {
  std::ifstream readstream;
  readstream.open("out.txt", std::ios::in);

  std::vector<std::string> input;
  std::cout << "------" << std::endl;
  while (!readstream.eof()) {
	std::string tmp;
	std::getline(readstream, tmp);
	input.push_back(tmp);
	std::cout << tmp << std::endl;
	std::cout << "------" << std::endl;
  }

  WSADATA wsadata;
  SOCKET sock, sock0;
  struct sockaddr_in addr, client;
  int len;

  //初期化
  int err;
  if (err = WSAStartup(MAKEWORD(2, 0), &wsadata) != 0) {
	switch (err) {
	case WSASYSNOTREADY:
	  //ネットワークサブシステムがネットワークへの接続を準備できていない
	  printf("WSASYSNOTREADY\n");
	  break;
	case WSAVERNOTSUPPORTED:
	  //要求されたwinsockのバージョンがサポートされていない
	  printf("WSAVERNOTSUPPORTED\n");
	  break;
	case WSAEINPROGRESS:
	  //ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している
	  printf("WSAEINPROGRESS\n");
	  break;
	case WSAEPROCLIM:
	  //winsockが処理できる最大プロセス数に達した
	  printf("WSAEPROCLIM\n");
	  break;
	case WSAEFAULT:
	  //第二引数であるlpWSAData は有効なポインタではない
	  printf("WSAEFAULT\n");
	  break;
	}
	return 1;
  }


  //ソケットの作成
  sock0 = socket(AF_INET, SOCK_STREAM, 0);
  if (sock0 == INVALID_SOCKET) {
	printf("error(socket) : %d\n", WSAGetLastError());
	//return 1;
  }


  //ソケットの設定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(60000);
  addr.sin_addr.S_un.S_addr = INADDR_ANY;

  //******************************************************
  char name[32];
  struct hostent *hostent1;
  struct in_addr inaddr1;

  gethostname(name, sizeof(name));

  hostent1 = gethostbyname(name);
  for (int i = 0; (hostent1->h_addr_list[i]) != NULL; ++i) {
	inaddr1.S_un.S_addr = **(unsigned int **)(hostent1->h_addr_list + i);

	printf("IPアドレス %d = %s\n", i, inet_ntoa(inaddr1));
  }
  printf("ポート番号 = %d\n", ntohs(addr.sin_port));
  //******************************************************



  //TIME_WAIT状態のポートが存在していてもbindができるようにする
  BOOL T = 1;
  setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&T, sizeof(T));

  if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
	printf("error(bind) : %d\n", WSAGetLastError());
	//return 1;
  }


  //TCPクライアントからの接続要求を待てる状態にする
  if (listen(sock0, 5) != 0) {
	printf("error(listen) : %d\n", WSAGetLastError());
	//return 1;
  }


  while (1) {
	//TCPクライアントからの接続要求を受け付ける
	printf("----------------------------\nWaiting...\n");
	len = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &len);
	printf("Connecting...\n");
	if (sock == INVALID_SOCKET) {
	  printf("error(accept) : %d\n", WSAGetLastError());
	  //return 1;
	}
	else {
	  //acceptした相手の確認
	  printf("accepted connection from %s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	  //ピースの数をここで先に送る
	  for (int i = 0; i < 4; ++i) {
		send(sock, input[i].c_str(), 1024,0);
	  }
	  
	}
	printf("End\n");
	closesocket(sock);
  }

  closesocket(sock0);

  // winsock2の終了処理
  WSACleanup();


  return 0;
}