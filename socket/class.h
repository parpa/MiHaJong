#ifndef CLASS_H
#define CLASS_H

#include <string>
#include <cstdint>
#include <WinSock2.h>
#include "except.h"

namespace mihajong_socket {

class Sock {
private:
	class client_thread; // クライアントのスレッド
	class server_thread; // サーバーのスレッド
	union Thread { // スレッドオブジェクトのポインタ(共用体)
	public:
		client_thread* client;
		server_thread* server;
	};
	static uint32_t addr2var(const std::string& address); // アドレスを取得
	bool isServer;
	sockaddr_in addr;
	SOCKET sock;
	Thread threadPtr;
public:
	Sock () {}; // ソケット初期化
	Sock (const std::string& destination, uint16_t port); // クライアント接続
	~Sock (); // 接続を切る
	void connect (const std::string& destination, uint16_t port); // クライアント接続
	void connect (); // クライアント再接続
	bool connected (); // 接続されているかを確認
	void disconnect (); // 接続を切る
};

class Sock::client_thread { // クライアントのスレッド
public:
	enum errorType {errNone, errConnection,};
	client_thread();
	static DWORD WINAPI thread(LPVOID lp); // スレッドを起動するための処理
	bool isConnected (); // 接続済かを返す関数
	void setaddr (const sockaddr_in destination); // 接続先を設定する
	void setsock (SOCKET* const socket); // ソケットを設定する
	void terminate (); // 切断する
	void chkError (); // エラーをチェックし、もしエラーだったら例外を投げる
private:
	SOCKET* mySock; // ソケット(ポインタ)
	errorType errtype; // エラーの種類
	int errcode; // エラーコード
	bool connected; // 接続済みかのフラグ[ワーカースレッドから書き込み]
	bool terminated; // 接続済みかのフラグ[親スレッドから書き込み]
	bool finished; // 終了済みかのフラグ[ワーカースレッドから書き込み]
	sockaddr_in myAddr; // アドレス情報[親スレッドから書き込み]
	DWORD WINAPI myThreadFunc(); // スレッドの処理
};

class Sock::server_thread { // サーバーのスレッド
	// TODO: これを実装する
};

}
#endif
