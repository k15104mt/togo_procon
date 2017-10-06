#include<winsock.h>
#include<cstdio>
#include<string>
#include <fstream>

int main() {
  WSADATA wsadata;
  struct sockaddr_in server;
  SOCKET sock;


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
  char input[32], ips[32];
  int ip[4], flag = 1;
  do {
	memset(input, '0', 32);
	printf("IPアドレスを入力してください-->");
	fgets(input, 32, stdin);
	sscanf_s(input, "%d.%d.%d.%d\n", &ip[0], &ip[1], &ip[2], &ip[3]);
	sprintf_s(ips, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	printf("IP=%s ok?(yes = 1/no = 0)-->", ips);
	fgets(input, 32, stdin);
	sscanf_s(input, "%d\n", &flag);

  } while (flag == 0);



  // 接続先指定用構造体の準備
  server.sin_family = AF_INET;
  server.sin_port = htons(60000);
  server.sin_addr.S_un.S_addr = inet_addr(ips);

  // ソケットの作成
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET) {
	printf("error(socket) : %d\n", WSAGetLastError());
	return 1;
  }
  else {
	// サーバに接続
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
	  printf("connect : %d\n", WSAGetLastError());
	}
	else {
	  std::ofstream writer;
	  writer.open("out.txt", std::ios::out);

	  char input[1024];
	  recv(sock, input, 1024, 0);
	  int num;
	  sscanf_s(input, "%d", &num);
	  writer << num << "\n";
	  for (int i = 0; i < num; ++i) {
		memset(input, 0, sizeof(char) * 1024);
		recv(sock, input, 1024, 0);
		writer << input << "\n";
		printf("\n--------\n%s\n--------\n", input);
	  }

	}
  }

  closesocket(sock);

  // winsock2の終了処理
  WSACleanup();

  return 0;
}