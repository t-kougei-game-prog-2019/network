// ServerSample.cpp

#include "pch.h"
#include <iostream>
#include <winsock2.h>

int main()
{
	// 接続を許可するクライアント端末の情報
	const int PORT_NO = 7000;	//ポート番号はクライアントプログラムと共通

	SOCKET s, s1;         //ソケット

	struct sockaddr_in source;
	char ans[] = "送信成功";

	// 待機情報を登録
	memset(&source, 0, sizeof(source));
	source.sin_family = AF_INET;
	source.sin_port = htons(PORT_NO);
	source.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケット通信の準備
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 0), &data) < 0) {
		printf("%d\n", GetLastError());
		printf("ソケット通信準備エラー\n");
	}

	// ソケットの生成
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s < 0) {
		printf("%d\n", GetLastError());
		printf("ソケット生成エラー\n");
	}

	//ソケットのバインド
	if (bind(s, (struct sockaddr *)&source, sizeof(source)) < 0) {
		printf("%d\n", GetLastError());
		printf("バインドエラー\n");
	}

	//接続の許可
	if (listen(s, 1) < 0) {
		printf("接続許可エラー\n");

	}

	printf("接続開始\n");

	//クライアントから通信があるまで待機
	s1 = accept(s, NULL, NULL);
	if (s1 < 0) {
		printf("待機エラー\n");
	}

	while (1) {
		//クライアントから送信されたデータの受信
		char buffer[1024];
		if (recv(s1, buffer, sizeof(buffer), 0) < 0) {
			printf("受信エラー\n");
		}
		printf("%sを受信しました\n", buffer);

		// "bye"なら終了
		if (strcmp("bye", buffer) == 0) break;

		//クライアントへデータを送信する
		char send_data[1024+18];
		sprintf_s(send_data, "受信されました: %s\n", buffer);
		if (send(s1, send_data, strlen(send_data) + 1, 0) < 0) {
			printf("送信エラー\n");
		}
	}

	// ソケット通信の終了
	printf("接続終了\n");
	closesocket(s1);
	WSACleanup();

	return 0;
}
