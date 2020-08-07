// 18142574_倉田涼平_プログラミング演習最終課題
/*
    各自でテーマを決めて自由にプログラムを作成せよ
    ただし、以下の条件を満たすこと
        C++を使用すること
        独自のクラス、関数を定義し用いること
        イテレーターを使用すること
        公開されている様々なライブラリを使用しても構わない
*/

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <stdio.h>//キー入力（GetKeyState）　など
#include <conio.h>//キー入力
#include <windows.h >//着色　　など
#include <stdio.h>//着色　
#include <regex>
#include <cassert>

using namespace std;

//-------------------------------------------------------------------------------------------------■関数
//キー情報　今入力されているキー番号を返す　＋　引数がfalseなら連続同じキーは返さない
int keyOnOff(bool kurikaesiOK) {

    static int pre_key = 100;//最初は何も押されてない
    int key;
    //キーボード読み取り
    // W キー押されてるか
    if (GetKeyState('W') & 0x8000) {
        key = 1;
    }
    // D キー押されてるか
    else if (GetKeyState('D') & 0x8000) {
        key = 2;
    }
    // S キー押されてるか
    else if (GetKeyState('S') & 0x8000) {
        key = 3;
    }
    // A キー押されてるか
    else if (GetKeyState('A') & 0x8000) {
        key = 4;
    }
    // Space キー押されてるか
    else if (GetKeyState(VK_SPACE) & 0x8000) {
        key = 0;
    }
    else {
        key = -1;
    }

    if (kurikaesiOK == true) {
        pre_key = key;//returnの前に代入
        return key;
    }
    else {//kurikaesiOK == false
        if (pre_key == key) {//前の入力と同じなら押されてないこととする
            pre_key = key;
            return -1;
        }
        else {//前の入力と異なるので押されたこととする
            pre_key = key;
            return key;
        }
    }
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■関数
//約n秒間待機
void stopTime(int n) {
    int i = 0;
    if (i <= 0) i = 1;//下限
    while (1) {
        i++;
        Sleep(1000);
        if (i > n) break;
    }
}
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------■クラス
//画面に映すものを登録し、それを表示する関数をもつクラス
struct screen {
protected:
    string mapstr;
    vector<string> screenvec;
    vector<string> screenwords = { "","","","","","","","","","","","","","","","","","","","" };//要素数は最大で20個
    vector<string> screenwords2 = { "","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","" };//要素数は最大で40個
public:
    void pushScreVec(string str) { screenvec.push_back(str); }//値をセットしたらscreenvecに要素を付け足す
    void viewScreVec() {
        if (screenvec.size() == 13 * 17) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, 0 });//画面の更新位置を{0,0}にする
            for (int i = 0; i < 13 * 17; i++) {
                mapstr = screenvec[i];
                if (mapstr == "空") printf("\x1b[47m　\x1b[49m\x1b");
                else if (mapstr == "■") printf("\x1b[47m\x1b[32m■\x1b[49m\x1b[39m\x1b");
                else if (mapstr == "↑") printf("\x1b[47m\x1b[33m↑\x1b[39m\x1b[49m\x1b");
                else if (mapstr == "→") printf("\x1b[47m\x1b[33m→\x1b[39m\x1b[49m\x1b");
                else if (mapstr == "↓") printf("\x1b[47m\x1b[33m↓\x1b[39m\x1b[49m\x1b");
                else if (mapstr == "←") printf("\x1b[47m\x1b[33m←\x1b[39m\x1b[49m\x1b");
                else if (mapstr == "敵") printf("\x1b[47m\x1b[33m敵\x1b[39m\x1b[49m\x1b");
                else cout << mapstr;
                if ((i + 1) % 17 == 0) {
                    cout << endl;
                }
            }
            screenvec = {};
        }
    }
    void pushScreWord(int i, string str) { screenwords[i] = str; }//描画したい文字を付け足していく
    void viewScreWord() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, 0 });//画面の更新位置を{0,0}にする
        for (int i = 0; i < 20; i++) {
            cout << screenwords[i];
        }
    }
    void pushScreWord2(int i, string str) { screenwords2[i] = str; }//描画したい文字を付け足していく
    void viewScreWord2() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, 0 });//画面の更新位置を{0,0}にする
        for (int i = 0; i < 39; i++) {
            cout << screenwords2[i];
        }
    }
};
//------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■クラス→inFileMapのためのクラス
//ベクトルの型　→　名前とベクトル、縦横サイズ
struct mapInfo {
protected:
    string myname;
    vector<int> mapvec;
    int tate = 11;
    int yoko = 11;
public:
    void setName(string newMyname) { myname = newMyname; }
    void pushMapvec(string newMapvec) { mapvec.push_back(stoi(newMapvec)); }
    void setTate(string newTate) { tate = stoi(newTate); }
    void setYoko(string newYoko) { yoko = stoi(newYoko); }
    void setMapvec(vector<int> newMapvec) { mapvec = newMapvec; }
    bool useOK() {//使用できるかチェック
        if (mapvec.end() - mapvec.begin() >= 121) {//使える条件：最小11×11
            if ((12 + tate) * (16 + yoko) == mapvec.end() - mapvec.begin()) return true;//サイズが合っているか
            else return false;
        }
        else return false;
    }
    string getName() { return myname; }
    int getTate() { return tate; }
    int getYoko() { return yoko; }
    vector<int> getMapvec() { return mapvec; }
};
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■クラス
//迷路のゲームを実行するクラスで、外から変更できるのは myname, mapvec, tate, yoko の４つのみ
struct stageClass : public screen, public mapInfo {
private:
    //---------------------プレイヤー情報
    int mymapplace = 1;
    int myhoukou = 1;
    //---------------------敵情報
    vector<int> tekiInfo = {};
    //---------------------
    bool saveOK = true;//すでにこのマップを保存したか　または　ファイルを操作できるか

    //-------------------------------------------------------------------------------------------------メンバ関数
    //迷路マップを用意する
    vector<int> mapVec(int& mystartplace) {
        //             　マップ縦長さ横長さ　ゴールの位置　　自分のスタート位置
        /*
        vector<int> mapvec{
            //0                 1                   2                   3
              //1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//1       0 ~ 35
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//2      36 ~ 71
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//3      72 ~ 107
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//4      108 ~ 143
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//5      144 ~ 179
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//6      180 ~ 215
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//7      216 ~ 251
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//8      252 ~ 287
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//9      288 ~ 323
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//10     324 ~ 359
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//11     360 ~ 395
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//12     396 ~ 431
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//13     432 ~ 467
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//14     468 ~ 503
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//15     504 ~ 539
                1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//16     540 ~ 575
                1,1,1,1,1,1,1,1,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,//17     576 ~ 611
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//18     612 ~ 647
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//19     648 ~ 683
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//20     684 ~ 719
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//21     720 ~ 755
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//22     756 ~ 791
                1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,//23     792 ~ 827
        };//まずは全て壁のマップを作製
        */

        //改マップベクトル作製
        vector<int> mapvec{};
        for (int i = 0; i < 6; i++) {//上段の壁
            for (int j = 0; j < 16 + yoko; j++) {
                mapvec.push_back(1);
            }
        }
        for (int m = 0; m < tate; m++) {//中段の壁
            for (int n = 0; n < 8; n++) {//左横壁
                mapvec.push_back(1);
            }
            for (int p = 0; p < yoko; p++) {//中央
                mapvec.push_back(2);
            }
            for (int o = 0; o < 8; o++) {//右横壁
                mapvec.push_back(1);
            }
        }
        for (int k = 0; k < 6; k++) {//下段の壁
            for (int l = 0; l < 16 + yoko; l++) {
                mapvec.push_back(1);
            }
        }
        int randomseisuu;
        int myplace = (mapvec.end() - mapvec.begin()) - 6 * (16 + yoko) - (8 + yoko);//スタート地点=(マップベクトルのサイズ)-6×(16+yoko)-(8+yoko)
        mystartplace = myplace;
        vector<int> rireki = { mystartplace };//スタート地点を入れておく
        vector<int> houkou = {};//これだけでは４方向全て進めないことになる
        random_device engine;

        mapvec[mystartplace] = 0;//スタート地点を0にする

        while (1) {//-----------------------------まずはゴールまで一直線
            //上下左右確認
            if (mapvec[myplace - 2 * (16 + yoko)] == 2) {//２つ上が壁である（1：絶対壁ではなくて2）
                houkou.push_back(1);
            }
            if (mapvec[myplace + 2] == 2) {//２つ右が壁である（1：絶対壁ではなくて2）
                houkou.push_back(2);
            }
            if (mapvec[myplace + 2 * (16 + yoko)] == 2) {//２つ下が壁である（1：絶対壁ではなくて2）
                houkou.push_back(3);
            }
            if (mapvec[myplace - 2] == 2) {//２つ左が壁である（1：絶対壁ではなくて2）
                houkou.push_back(4);
            }
            if (houkou.end() - houkou.begin() == 0) {//-----------●周りがすべて道または壁---------------
                houkou = {};//リセット
                rireki.pop_back();
                rireki.pop_back();
                //２マス戻る
                myplace = rireki[rireki.end() - rireki.begin() - 1];//２マス戻ったところをもう一度、つまりベクトルの最後
                if (myplace == mystartplace) {
                    break;//--------------------------------------------------------------★２マス戻るを続けてスタート位置に戻ったら終了
                }
                continue;
            }
            else {//-------------------------------●まだ進めるので続ける--------------------------
                uniform_int_distribution<int> random(0, houkou.end() - houkou.begin() - 1);//上下右左にランダム
                randomseisuu = random(engine);
                switch (houkou[randomseisuu]) {
                case 1://上に進む
                    mapvec[myplace - (16 + yoko)] = 0;
                    mapvec[myplace - 2 * (16 + yoko)] = 0;
                    rireki.push_back(myplace - (16 + yoko));//このときに通った道を覚えておく
                    rireki.push_back(myplace - 2 * (16 + yoko));//このときに通った道を覚えておく
                    myplace -= 2 * (16 + yoko);
                    break;
                case 2://右に進む
                    mapvec[myplace + 1] = 0;
                    mapvec[myplace + 2] = 0;
                    rireki.push_back(myplace + 1);//このときに通った道を覚えておく
                    rireki.push_back(myplace + 2);//このときに通った道を覚えておく
                    myplace += 2;
                    break;
                case 3://下に進む
                    mapvec[myplace + (16 + yoko)] = 0;
                    mapvec[myplace + 2 * (16 + yoko)] = 0;
                    rireki.push_back(myplace + (16 + yoko));//このときに通った道を覚えておく
                    rireki.push_back(myplace + 2 * (16 + yoko));//このときに通った道を覚えておく
                    myplace += 2 * (16 + yoko);
                    break;
                case 4://左に進む
                    mapvec[myplace - 1] = 0;
                    mapvec[myplace - 2] = 0;
                    rireki.push_back(myplace - 1);//このときに通った道を覚えておく
                    rireki.push_back(myplace - 2);//このときに通った道を覚えておく
                    myplace -= 2;
                    break;
                }
                houkou = {};//リセット
            }
        }
        return mapvec;
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //敵の数を決定する
    void tekiInfoMake() {
        int averageSize = tate / 10 + yoko / 10;//切り捨て
        for (int i = 0; i < averageSize; i++) tekiInfo.push_back(1);
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //敵の初期位置決定
    void tekiHaiti() {
        //プレーヤーの初期位置とは別で、かつ、地面であるところにランダムに置いていく
        int randomiti;
        random_device engine;
        uniform_int_distribution<int> random((6 * (16 + yoko) + 8), ((6 + tate) * (16 + yoko) - 8));//迷路部分にランダムに置く
        for (auto i = tekiInfo.begin(); i != tekiInfo.end(); i++) {//敵の数だけ配置処理をする
            while (1) {
                randomiti = random(engine);
                if (randomiti == mymapplace || mapvec[randomiti] != 0) {//プレイヤーの初期位置または壁には置けない
                    continue;
                }
                else if (mapvec[randomiti] == 0) {//置ける
                    *i = randomiti;
                    break;
                }
                else {//謎
                    *i = -1;//いないことにしておく
                    break;
                }
            }
        }
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //敵の移動先決定
    void tekiIdou() {
        int tekiidouhoukou = -1;
        int tekiidousaki = -1;//移動先のマップの場所
        for (int i = 0; i < tekiInfo.end() - tekiInfo.begin(); i++) {//敵の数だけ処理
            if (tekiInfo[i] < 0) continue;//マップ上に存在しない
            tekiidouhoukou = -1;
            //自分の行と列を確認
            int a = tekiInfo[i] - (tekiInfo[i] % (16 + yoko)) + 1;//自分のいる行の左端
            for (int p = a; p < tekiInfo[i]; p++) {//自分の左確認
                if (mymapplace == p) tekiidouhoukou = 4;
            }
            for (int q = tekiInfo[i]; q < a + (16 + yoko); q++) {//自分の右確認
                if (mymapplace == q) tekiidouhoukou = 2;
            }
            int b = tekiInfo[i] % (16 + yoko);
            for (int r = b; r < tekiInfo[i]; r += (16 + yoko)) {//自分の上確認
                if (mymapplace == r) tekiidouhoukou = 1;
            }
            for (int s = tekiInfo[i]; s < (16 + yoko) * (11 + tate) + b; s += (16 + yoko)) {//自分の下確認
                if (mymapplace == s) tekiidouhoukou = 3;
            }
            tekiidousaki = -1;
            //行動ルール１：ランダムに動き回る
            if (tekiidouhoukou == -1) {
                random_device engine;
                uniform_int_distribution<int> random(1, 10);
                int randomseisuu = random(engine);
                if (randomseisuu == 1) {
                    tekiidousaki = tekiInfo[i] - (16 + yoko);
                }
                else if (randomseisuu == 2) {
                    tekiidousaki = tekiInfo[i] + 1;
                }
                else if (randomseisuu == 3) {
                    tekiidousaki = tekiInfo[i] + (16 + yoko);
                }
                else if (randomseisuu == 4) {
                    tekiidousaki = tekiInfo[i] - 1;
                }
            }
            //行動ルール２：直線状（壁越しもあり）にプレーヤーを見つけた場合はその方向に向かって直進
            else {
                if (tekiidouhoukou == 1) {
                    tekiidousaki = tekiInfo[i] - (16 + yoko);
                }
                else if (tekiidouhoukou == 2) {
                    tekiidousaki = tekiInfo[i] + 1;
                }
                else if (tekiidouhoukou == 3) {
                    tekiidousaki = tekiInfo[i] + (16 + yoko);
                }
                else if (tekiidouhoukou == 4) {
                    tekiidousaki = tekiInfo[i] - 1;
                }
            }
            //　＊　移動先が地面である　かつ　移動先に他の「敵」がいない
            int flag = 0;//フラグ
            if (tekiidousaki >= 0 && tekiidousaki < mapvec.end() - mapvec.begin()) {
                if (mapvec[tekiidousaki] == 0) {
                    for (int j = 0; j < tekiInfo.end() - tekiInfo.begin(); j++) {
                        if (tekiInfo[j] == tekiidousaki) {
                            flag++;//移動先に他の敵がいたらフラグたてる
                        }
                    }
                    if (flag == 0) tekiInfo[i] = tekiidousaki;//移動先に敵がいない
                }
            }
        }
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //自分の移動先決定
    void myIdou(int idouhoukou) {
        //まず移動するかしないかを決めて、移動するなら隣の地面へ移動する
        //また、隣に移動する際に、ほかの　CHARA と同じ位置になるならば　移動はキャンセル
        int tonari = 0;
        tonari = mymapplace;
        if (idouhoukou == 1) {//上
            tonari -= (16 + yoko);
        }
        else if (idouhoukou == 2) {//右
            tonari += 1;
        }
        else if (idouhoukou == 3) {//下
            tonari += (16 + yoko);
        }
        else if (idouhoukou == 4) {//左
            tonari -= 1;
        }
        else {//
            //こうなることはない
        }
        //移動先が地面かを調べる
        if (mapvec[tonari] == 0) {
            //移動できる
            mymapplace = tonari;//自分の位置を変更
            myhoukou = idouhoukou;//自分の向きをキーの方向に変える
        }
        else {
            //移動先が地面ではないので移動できないが方向は変える
            myhoukou = idouhoukou;//自分の向きをキーの方向に変える
        }
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //方向キーが押された際、または、敵が移動する際にマップ情報を更新し描画する
    void mapViewer() {
        //          マップベクトル　　　　自分の現在位置と向いている方向　　敵情報　　　　マップの縦長さ横長さ
        while (1) {
            /*　画面に映すもの　縦13×横17
                □□□□□□□□□□□□□□□□□n-(6*yoko+104)   n-(6*yoko+88)
                □□□□□□□□□□□□□□□□□n-(5*yoko+88)   n-(5*yoko+72)
                □□□□□□□□□□□□□□□□□n-(4*yoko+72)   n-(4*yoko+56)
                □□□□□□□□□□□□□□□□□n-(3*yoko+56)   n-(3*yoko+40)
                □□□□□□□□□□□□□□□□□n-(2*yoko+40)   n-(2*yoko+24)
                □□□□□□□□□□□□□□□□□n-(yoko+24)     n-(yoko+8)
                □□□□□□□□■□□□□□□□□n-8         n  n+8
                □□□□□□□□□□□□□□□□□n+(yoko+8)     n+(yoko+8)
                □□□□□□□□□□□□□□□□□n+(2*yoko+24)   n+(2*yoko+40)
                □□□□□□□□□□□□□□□□□n+(3*yoko+40)   n+(3*yoko+56)
                □□□□□□□□□□□□□□□□□n+(4*yoko+56)   n+(4*yoko+72)
                □□□□□□□□□□□□□□□□□n+(5*yoko+72)   n+(5*yoko+88)
                □□□□□□□□□□□□□□□□□n+(6*yoko+88)   n+(6*yoko+104)
            */
            //マップ更新
            //    mymapplace == n
            int vecnum = mymapplace - (6 * yoko + 104);//左上から描き始める
            int tmp = 0;
            int tekiiru = false;//今描くマスに敵がいるか→敵はマップベクトルmapvecに書き込まないからこのようにする必要がある
            screen S;
            for (int i = mymapplace - (6 * yoko + 104); i < mymapplace + (6 * yoko + 104 + 1); i++) {
                //---------------------------------------------------------------
                tekiiru = false;//そのマスには敵がいないことにしておく
                for (int j = 0; j < tekiInfo.end() - tekiInfo.begin(); j++) {//敵
                    if (vecnum == tekiInfo[j]) {
                        S.pushScreVec("敵");
                        tekiiru = true;//そのマスに敵がいたらtrue
                    }
                }
                //------------------------------------------------------------------
                if (tekiiru == false) {//すでに敵の描画が終わっていなければマップベクトルにのっとって描画する
                    if (vecnum == mymapplace) {
                        if (myhoukou == 1) S.pushScreVec("↑");
                        else if (myhoukou == 2) S.pushScreVec("→");
                        else if (myhoukou == 3) S.pushScreVec("↓");
                        else if (myhoukou == 4) S.pushScreVec("←");
                        else S.pushScreVec("↑");
                    }
                    else if (mapvec[vecnum] == 0) S.pushScreVec("空");
                    else if (mapvec[vecnum] == 1 || mapvec[vecnum] == 2) S.pushScreVec("■");
                    else S.pushScreVec("？");
                }
                vecnum += 1;
                tmp += 1;
                if (tmp % 17 == 0) {
                    vecnum += yoko - 1;//右端にきても最後に　vecnum += 1 をしているので、　yoko-1=19 にする必要がある
                    if (vecnum < mymapplace + (6 * yoko + 104)) {//ベクトルの反復子を用いているのでベクトルの最大数以上の要素を示すことはできない
                        i += yoko - 1;
                    }
                }
            }
            S.viewScreVec();//描画
            break;//描画したので終了
        }
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //マップをファイルに保存する
    void outfile() {
        system("cls");//画面をまっさらにする
        screen S;
        S.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　マップの名前を変更しますか？★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　現在：");
        S.pushScreWord(1, myname);
        S.pushScreWord(2, "\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord(3, "→");
        S.pushScreWord(4, "　●　いいえ　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord(5, "　");
        S.pushScreWord(6, "　●　はい　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★★★★★★★★★★★★★★★★★\n");
        S.viewScreWord();
        int keybanngou = 0;
        int menu_num_S = 0;
        while (1) {
            keybanngou = keyOnOff(false);
            if (keybanngou == 1 || keybanngou == 3) {//上または下
                if (menu_num_S == 0) menu_num_S = 1;
                else menu_num_S = 0;
            }
            else if (keybanngou == 0) {
                if (menu_num_S == 1) {//はいなら変更
                    system("cls");
                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//cinバッファ削除
                    while (cin >> myname) {
                        if (myname == "") cout << "名前を入力してください!!";
                        else break;
                    }
                }
                else {//いいえならそのまま保存
                    break;
                }
            }
            if (menu_num_S == 0) {
                S.pushScreWord(3, "→"); S.pushScreWord(5, "　");
            }
            else {
                S.pushScreWord(3, "　"); S.pushScreWord(5, "→");
            }
            S.viewScreWord();
        }
        ofstream outfile("mapData.txt", ios_base::app);
        outfile << myname << "," << tate << "," << yoko << endl;
        for (int j = 0; j < mapvec.end() - mapvec.begin(); j++) {
            outfile << mapvec[j] << ",";
        }
        outfile << endl;
        outfile.close();
        saveOK = false;
    }
    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------メンバ関数
    //メニュー操作
    int menu() {//マップベクトルは、保存できるように引数に入れている
        int nokorininnzuu = 0;
        for (int i = 0; i < tekiInfo.end() - tekiInfo.begin(); i++) {
            if (tekiInfo[i] != -1) {//まだ生き残っている
                nokorininnzuu += 1;
            }
        }
        system("cls");//画面をまっさらにする
        screen S;
        S.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
            "★ーーーーー＜メニュー＞ーーーー★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord(1, "→");
        S.pushScreWord(2, "　●　続ける　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord(3, "　");
        S.pushScreWord(4, "　●　");
        if (saveOK == true) S.pushScreWord(5, "マップをセーブ　");
        else S.pushScreWord(5, "セーブできません");
        S.pushScreWord(6, "　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord(7, "　");
        S.pushScreWord(8, "　●　ゲームをやめる　　　★\n"
            "★ーーーーーーーーーーーーーーー★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　残りの敵：　");
        S.pushScreWord(9, to_string(nokorininnzuu));
        S.pushScreWord(10, "\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★★★★★★★★★★★★★★★★★\n");
        S.viewScreWord();
        int time = 0;
        int keybanngou;
        int menu_num_S = 0;
        while (1) {
            time++;
            if (time == 100000) {//キーを受けつける
                time = 0;
                keybanngou = keyOnOff(false);
                if (keybanngou == 1) {//上
                    if (menu_num_S == 0) menu_num_S = 2;
                    else if (menu_num_S == 1) menu_num_S = 0;
                    else menu_num_S = 1;
                }
                else if (keybanngou == 3) {//下
                    if (menu_num_S == 0) menu_num_S = 1;
                    else if (menu_num_S == 1) menu_num_S = 2;
                    else menu_num_S = 0;
                }
                else if (keybanngou == 0) {//スペース→決定
                    if (menu_num_S == 0) {//●マップにもどる
                        system("cls");//画面をまっさらにする
                        return 0;
                    }
                    else if (menu_num_S == 1 && saveOK == true) {//●マップを保存
                        outfile();//ファイル保存 saveOK　はここで false になっている
                        S.pushScreWord(5, "セーブしました　");//「セーブしました」に変更
                        S.viewScreWord();
                    }
                    else if (menu_num_S == 1 && saveOK == false) {//セーブできません　セーブしました
                        //反応なし
                    }
                    else {//●ゲームを終了
                        return 1;
                    }
                }

                if (menu_num_S == 0) {
                    S.pushScreWord(1, "→"); S.pushScreWord(3, "　"); S.pushScreWord(7, "　");
                }
                else if (menu_num_S == 1) {
                    S.pushScreWord(1, "　"); S.pushScreWord(3, "→"); S.pushScreWord(7, "　");
                }
                else {
                    S.pushScreWord(1, "　"); S.pushScreWord(3, "　"); S.pushScreWord(7, "→");
                }
                S.viewScreWord();
            }
        }
    }
    //-------------------------------------------------------------------------------------------------


public:
    //-------------------------------------------------------------------------------------------------メンバ関数・public
    //ステージ：クリアで bool →　true を返す
    bool gameStart()
    {
        //マップを生成して登録
        if (mapvec.end() - mapvec.begin() == 0) {//マップ読み込みをしていないなら、これからランダム生成
            mapvec = mapVec(mymapplace);
        }
        else {//ファイルからマップを読み込んでいる
            //ので、そのまま引き継ぐが、ME.mymapplaceが登録されていないのでここで計算
            mymapplace = (mapvec.end() - mapvec.begin()) - 6 * (16 + yoko) - (8 + yoko);//スタート地点=(マップベクトルのサイズ)-6×(16+yoko)-(8+yoko)
        }

        //クラスに敵を登録
        tekiInfoMake();//敵を生成
        tekiHaiti();//敵の初期位置を設定

        //変数宣言
        int time = 0;//今の時間（キーの受付）
        int time2 = 0;//今の時間（敵の動き　＋　クリア判定）
        int time3 = 0;//今の時間（画面更新）
        int nexttime = 1000000;//次にキーを受け付けるまでの時間
        int nexttime2 = 50000000;//次に敵が行動するまでの時間
        int nexttime3 = 1000000;//次にクリア判定するまでの時間
        bool cooltimeBOOL = false;
        int cooltime = 0;//キーのクールタイム
        int keybanngou;//押されているキー番号一時保管用

        system("cls");//画面をまっさらにする
        //ループ開始
        while (1) {
            time++;
            //方向キー受付開始
            if (time > nexttime + cooltime) {
                time = 0;
                keybanngou = keyOnOff(true);//今押されているキー番号取得　何も押されていない場合は -1 を返す
                if (keybanngou > 0) {//いずれかのキーが押されている
                    cooltimeBOOL = true;
                    myIdou(keybanngou);
                    mapViewer();//画面更新
                }
                else if (keybanngou == 0) {
                    switch (menu()) {
                    case 0://コンティニュー
                        stopTime(1);//連続でメニューを開かないように
                        break;
                    case 1://やめる
                        return false;//ゲーム終了
                    }
                    //メニュー操作：menu関数に移動
                }
                else {
                    cooltimeBOOL = false;//何のキーも押していない
                }
                //キーのクールタイムを再設定
                if (cooltimeBOOL == true) {
                    cooltime = 5000000 - nexttime;//数字の部分が、クールタイムになったときの待たされる時間
                }
                else {
                    cooltime = 0;
                }
            }

            time2++;
            //敵の動きの実装　＋　クリア判定
            if (time2 > nexttime2) {//      敵移動可能時間になり、
                time2 = 0;
                tekiIdou();//敵の位置を変更
                mapViewer();//画面更新
            }

            time3++;
            //クリア判定
            if (time3 > nexttime3) {
                time3 = 0;
                //まずは敵生存判定
                //自分と重なったら敵はやられる
                for (int i = 0; i < tekiInfo.end() - tekiInfo.begin(); i++) {
                    if (mymapplace == tekiInfo[i]) {//自分と重なったら敵はやられる
                        tekiInfo[i] = -1;
                    }
                }
                //クリア判定
                int flag = 0;
                for (int i = 0; i < tekiInfo.end() - tekiInfo.begin(); i++) {
                    if (tekiInfo[i] >= 0) {//その敵が生きてたらflagをインクリメント
                        flag++;//フラグをたてる
                    }
                }
                if (flag == 0) {//敵が一人もいないのでクリア
                    system("cls");//画面をまっさらにする
                    int menu_num_S = 0;
                    screen S;
                    S.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　クリアしました　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　");
                    if (saveOK == true) {//保存可能
                        S.pushScreWord(1, "→");
                        S.pushScreWord(2, "　●　マップを保存　　　　★\n"
                            "★　　　　　　　　　　　　　　　★\n"
                            "★　");
                        S.pushScreWord(3, "　");
                    }
                    else {//保存不可
                        menu_num_S = 1;
                        S.pushScreWord(1, "　");
                        S.pushScreWord(2, "　　　　　　　　　　　　　★\n"
                            "★　　　　　　　　　　　　　　　★\n"
                            "★　");
                        S.pushScreWord(3, "→");
                    }
                    S.pushScreWord(4, "　●　終了　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★★★★★★★★★★★★★★★★★\n");
                    S.viewScreWord();
                    while (1) {
                        keybanngou = keyOnOff(false);//今押されているキー番号取得　何も押されていない場合は -1 を返す
                        if (keybanngou == 1 && saveOK == true) {//上 かつ　セーブができる
                            if (menu_num_S == 0) menu_num_S = 1;
                            else menu_num_S = 0;
                        }
                        else if (keybanngou == 3 && saveOK == true) {//下　かつ　セーブができる
                            if (menu_num_S == 0) menu_num_S = 1;
                            else menu_num_S = 0;
                        }
                        if (keybanngou == 0) {
                            if (menu_num_S == 0) {//●マップを保存
                                outfile();//ファイル保存
                                break;
                            }
                            else {//●終了
                                break;
                            }
                        }

                        if (menu_num_S == 0) {
                            S.pushScreWord(1, "→"); S.pushScreWord(3, "　");
                        }
                        else {
                            S.pushScreWord(1, "　"); S.pushScreWord(3, "→");
                        }
                        S.viewScreWord();
                    }
                    return true;//クリアである
                    break;//ゲーム終了
                }
            }
        }
    }
    //-------------------------------------------------------------------------------------------------

    void setSaveOK(bool newSaveOK) { saveOK = newSaveOK; }
    void resetClass() {
        myname = "";
        mapvec = {};
        tate = 11;
        yoko = 11;
        mymapplace = 1;
        myhoukou = 1;
        tekiInfo = {};
        saveOK = true;
    }
};
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------■関数
//ファイルを読み込む　ファイルが見つからない場合はfalseを返す
bool inFileMap(vector<mapInfo>& Vec) {
    Vec = {};//初期化
    int tate = 0;
    int yoko = 0;
    ifstream infile("mapData.txt");
    if (!infile) {//ファイルが見つからない
        return false;
    }
    int InfoNum = 0;
    int a = 0;
    regex rx(R"(,)");
    string line;//"mapData.txt"にある情報をそのまま入れる
    while (getline(infile, line)) {//一行ずつ読み込む
        sregex_token_iterator it(line.begin(), line.end(), rx, -1);
        sregex_token_iterator end;
        if (*it == "") break;
        if (a % 2 == 0) {
            Vec.emplace_back();
            Vec[InfoNum].setName(*it++);
            Vec[InfoNum].setTate(*it++);
            Vec[InfoNum].setYoko(*it++);
        }
        else {
            while (it != end) {
                Vec[InfoNum].pushMapvec(*it++);
            }
            if (Vec[InfoNum].useOK() == true) InfoNum++;//使えるならば数に入れる
        }
        a++;
    }
    infile.close();
    return true;
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■関数
//引数に指定した順番の要素を消し、ベクトルのサイズを小さくする
void eraseVec_mapInfo(vector<mapInfo>& Vec, int kesuyouso) {
    vector<mapInfo> newVec;
    int vecnum = 0;
    for (int i = 0; i < Vec.end() - Vec.begin(); i++) {
        if (i != kesuyouso) {
            newVec.emplace_back();
            newVec[vecnum].setName(Vec[i].getName());
            newVec[vecnum].setTate(to_string(Vec[i].getTate()));
            newVec[vecnum].setYoko(to_string(Vec[i].getYoko()));
            newVec[vecnum].setMapvec(Vec[i].getMapvec());
            vecnum++;
        }
    }
    Vec = newVec;//Vecの更新
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■関数
//読み込んだマップから選択し、変更後のファイルを保存する　変更後のVecの何番目を選んだのかを返す(キャンセルなら-1)
int chooseMap(vector<mapInfo>& Vec) {
    while (1) {//ファイルの変更をした場合ここに戻る
        system("cls");
        /////////////////////////////////////
        screen S;
        S.pushScreWord2(0, "★★★★★★★★★★★★★★★★★\n"
            "★ーーー＜マップ読み込み＞ーーー★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S.pushScreWord2(2, "→");//矢印0
        S.pushScreWord2(3, "　");
        S.pushScreWord2(5, "\n"
            "★　");
        S.pushScreWord2(6, "　");//矢印1
        S.pushScreWord2(7, "　");
        S.pushScreWord2(9, "\n"
            "★　");
        S.pushScreWord2(10, "　");//矢印2
        S.pushScreWord2(11, "　");
        S.pushScreWord2(13, "\n"
            "★　");
        S.pushScreWord2(14, "　");//矢印3
        S.pushScreWord2(15, "　");
        S.pushScreWord2(17, "\n"
            "★　");
        S.pushScreWord2(18, "　");//矢印4
        S.pushScreWord2(19, "　");
        S.pushScreWord2(21, "\n"
            "★　");
        S.pushScreWord2(22, "　");//矢印5
        S.pushScreWord2(23, "　");
        S.pushScreWord2(25, "\n"
            "★　");
        S.pushScreWord2(26, "　");//矢印6
        S.pushScreWord2(27, "　");
        S.pushScreWord2(29, "\n"
            "★　");
        S.pushScreWord2(30, "　");//矢印7
        S.pushScreWord2(31, "　");
        S.pushScreWord2(33, "\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★★★★★★★★★★★★★★★★★\n");
        ///////////////////////////////////////////
        screen S2;
        S2.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S2.pushScreWord(1, "●●");//あとで選択したマップの情報が入る
        S2.pushScreWord(2, "\n"
            "★　　をどうしますか？　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S2.pushScreWord(3, "→");
        S2.pushScreWord(4, "　●　決定　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S2.pushScreWord(5, "　");
        S2.pushScreWord(6, "　●　削除　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        S2.pushScreWord(7, "　");
        S2.pushScreWord(8, "　●　キャンセル　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★★★★★★★★★★★★★★★★★\n");
        ///////////////////////////////////////////

        //データを読み取り表示できるようにする
        int VecSize = Vec.end() - Vec.begin();
        int SpushNum = 4;
        vector<string> strvec;
        for (int i = 0; i < VecSize + 1; i++) {//"キャンセル"を入れるところも用意
            strvec.emplace_back();
        }
        for (int i = 0; i < VecSize; i++) {
            strvec[i] = Vec[i].getName();
            strvec[i] += "：";
            strvec[i] += to_string(Vec[i].getTate());
            strvec[i] += "×";
            strvec[i] += to_string(Vec[i].getYoko());
            S.pushScreWord2(SpushNum, strvec[i]);
            SpushNum += 4;
        }
        strvec[VecSize] = "キャンセル";
        S.pushScreWord2(SpushNum, strvec[VecSize]);//"キャンセル"を入れる

        //変数宣言
        bool breakOK = false;//この関数をはじめからやり直す
        int keybanngou = 0;
        int menu_num_S = 0;
        int menu_num_S2 = 0;

        system("cls");
        S.viewScreWord2();
        while (1) {//----------------------------------------------------------------------------
            keybanngou = keyOnOff(false);
            if (keybanngou == 1) {//上
                if (menu_num_S == 0) menu_num_S = VecSize;
                else if (menu_num_S == 1) menu_num_S = 0;
                else if (menu_num_S == 2) menu_num_S = 1;
                else if (menu_num_S == 3) menu_num_S = 2;
                else if (menu_num_S == 4) menu_num_S = 3;
                else if (menu_num_S == 5) menu_num_S = 4;
                else if (menu_num_S == 6) menu_num_S = 5;
                else if (menu_num_S == 7) menu_num_S = 6;
            }
            else if (keybanngou == 3) {//下
                if (menu_num_S == 0) {
                    if (VecSize < 1) menu_num_S = 0;//キャンセルのみ
                    else menu_num_S = 1;
                }
                else if (menu_num_S == 1) {
                    if (VecSize < 2) menu_num_S = 0;
                    else menu_num_S = 2;
                }
                else if (menu_num_S == 2) {
                    if (VecSize < 3) menu_num_S = 0;
                    else menu_num_S = 3;
                }
                else if (menu_num_S == 3) {
                    if (VecSize < 4) menu_num_S = 0;
                    else menu_num_S = 4;
                }
                else if (menu_num_S == 4) {
                    if (VecSize < 5) menu_num_S = 0;
                    else menu_num_S = 5;
                }
                else if (menu_num_S == 5) {
                    if (VecSize < 6) menu_num_S = 0;
                    else menu_num_S = 6;
                }
                else if (menu_num_S == 6) {
                    if (VecSize < 7) menu_num_S = 0;
                    else menu_num_S = 7;
                }
                else if (menu_num_S == 7) {
                    menu_num_S = 0;
                }
            }
            else if (keybanngou == 0) {
                if (menu_num_S == VecSize) return -1;//●キャンセルが選ばれたので終了
                S2.pushScreWord(1, strvec[menu_num_S]);//●何かしらマップが選ばれた
                system("cls");
                S2.viewScreWord();
                while (1) {//----------------------------------
                    keybanngou = keyOnOff(false);
                    if (keybanngou == 1) {//上
                        if (menu_num_S2 == 0) menu_num_S2 = 2;
                        else if (menu_num_S2 == 1) menu_num_S2 = 0;
                        else menu_num_S2 = 1;
                    }
                    else if (keybanngou == 3) {//下
                        if (menu_num_S2 == 0) menu_num_S2 = 1;
                        else if (menu_num_S2 == 1) menu_num_S2 = 2;
                        else menu_num_S2 = 0;
                    }
                    else if (keybanngou == 0) {
                        if (menu_num_S2 == 0) {//●●決定
                            return menu_num_S;//Vecの何番目の要素に決定したかを返す
                        }
                        else if (menu_num_S2 == 1) {//●●削除
                            eraseVec_mapInfo(Vec, menu_num_S);
                            ///////////////////////////////消したらファイルに保存
                            ofstream outfile("mapData.txt");
                            for (int k = 0; k < Vec.end() - Vec.begin(); k++) {
                                outfile << Vec[k].getName() << "," << Vec[k].getTate() << "," << Vec[k].getYoko() << endl;
                                vector<int> vec = Vec[k].getMapvec();
                                for (int k2 = 0; k2 < vec.end() - vec.begin(); k2++) {
                                    outfile << vec[k2] << ",";
                                }
                            }
                            if (Vec.end() - Vec.begin() != 0) outfile << endl;//データを全て消した場合にはendlはしない→以降読み込まれなくなる
                            outfile.close();
                            ///////////////////////////////
                            breakOK = true;//この関数をはじめからやり直す
                            break;
                        }
                        else {//●●キャンセル
                            system("cls");
                            S.viewScreWord2();
                            break;
                        }
                    }
                    if (menu_num_S2 == 0) {
                        S2.pushScreWord(3, "→"); S2.pushScreWord(5, "　"); S2.pushScreWord(7, "　");
                    }
                    else if (menu_num_S2 == 1) {
                        S2.pushScreWord(3, "　"); S2.pushScreWord(5, "→"); S2.pushScreWord(7, "　");
                    }
                    else {
                        S2.pushScreWord(3, "　"); S2.pushScreWord(5, "　"); S2.pushScreWord(7, "→");
                    }
                    S2.viewScreWord();
                }//--------------------------------------------
            }
            if (menu_num_S == 0) {
                S.pushScreWord2(2, "→"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 1) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "→"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 2) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "→"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 3) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "→"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 4) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "→"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 5) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "→"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 6) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "→"); S.pushScreWord2(30, "　");
            }
            else if (menu_num_S == 7) {
                S.pushScreWord2(2, "　"); S.pushScreWord2(6, "　"); S.pushScreWord2(10, "　"); S.pushScreWord2(14, "　"); S.pushScreWord2(18, "　"); S.pushScreWord2(22, "　"); S.pushScreWord2(26, "　"); S.pushScreWord2(30, "→");
            }
            S.viewScreWord2();//更新
            if (breakOK == true) {//この関数をはじめからやり直す（マップの要素を削除した際にtrueになる）
                break;
            }
        }
    }
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------■メイン関数
int main() {
    constexpr CONSOLE_CURSOR_INFO cursor{ 1, FALSE };// カーソルを不可視化する
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor); // カーソルを不可視化する
    //変数宣言
    stageClass Stage;
    string writename;
    //マップデータ
    string mapname = "新迷路";
    vector<mapInfo> Vec;
    int tate = 11;
    int yoko = 11;
    ///////////////////////////////////////////////////////////
    screen S;//メニュー画面
    S.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
        "★ーーーー＜迷路ゲーム＞ーーーー★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S.pushScreWord(1, "→");
    S.pushScreWord(2, "　●　新規作成　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S.pushScreWord(3, "　");
    S.pushScreWord(4, "　●　マップを読み込む　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S.pushScreWord(5, "　");
    S.pushScreWord(6, "　●　終了　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★★★★★★★★★★★★★★★★★\n");
    ///////////////////////////////////////////////////////////
    screen S2;//メニュー画面２
    S2.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
        "★ーーーー＜新規作成＞ーーーーー★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S2.pushScreWord(1, "→");
    S2.pushScreWord(2, "　●　名前：");
    S2.pushScreWord(3, mapname);
    S2.pushScreWord(4, "\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S2.pushScreWord(5, "　");
    S2.pushScreWord(6, "　●　サイズ：");
    S2.pushScreWord(7, to_string(tate));
    S2.pushScreWord(8, "×");
    S2.pushScreWord(9, to_string(yoko));
    S2.pushScreWord(10, "\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S2.pushScreWord(11, "　");
    S2.pushScreWord(12, "　●　完了　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★　");
    S2.pushScreWord(13, "　");
    S2.pushScreWord(14, "　●　キャンセル　　　　　★\n"
        "★　　　　　　　　　　　　　　　★\n"
        "★★★★★★★★★★★★★★★★★\n");
    ///////////////////////////////////////////////////////////

    //最初のメニュー画面処理
    int keybanngou;
    int time = 0;
    bool toMap = false;//マップに移行するか
    bool kuria = false;
    int menu_num_S = 0;
    int menu_num_S2 = 0;
    bool fileDataOK = true;//ファイルを読み込めたか

    //データ読み込み
    fileDataOK = inFileMap(Vec);//最初にも読み込む必要あり→新規作成したときにVecのサイズがわからないため
    if (fileDataOK == false) {
        S.pushScreWord(4, "　　　データ読み込みエラー★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　　　　　　　　　　　　　　　★\n"
            "★　");
        Stage.setSaveOK(false);
    }

    //処理開始
    S.viewScreWord();
    while (1) {//////////////////////////////////////////////////////////////////////////////////////////////////////★(1)while　メインメニューループ

        time++;
        if (time >= 100000) {
            kuria = false;//初期化
            keybanngou = 0;//初期化
            time = 0;//初期化
            toMap = false;//初期化
            tate = 11;//初期化
            yoko = 11;//初期化
            mapname = "新迷路";//初期化
            S2.pushScreWord(3, mapname);//初期化
            S2.pushScreWord(7, "11");//初期化
            S2.pushScreWord(9, "11");//初期化

            keybanngou = keyOnOff(false);//キー受付
            if (keybanngou == 1) {//上
                if (fileDataOK == true) {
                    if (menu_num_S == 0) menu_num_S = 2;
                    else if (menu_num_S == 1) menu_num_S = 0;
                    else menu_num_S = 1;
                }
                else {//ファイルがない場合
                    if (menu_num_S == 0) menu_num_S = 2;
                    else menu_num_S = 0;
                }
            }
            else if (keybanngou == 3) {//下
                if (fileDataOK == true) {
                    if (menu_num_S == 0) menu_num_S = 1;
                    else if (menu_num_S == 1) menu_num_S = 2;
                    else menu_num_S = 0;
                }
                else {//ファイルがない場合
                    if (menu_num_S == 0) menu_num_S = 2;
                    else menu_num_S = 0;
                }
            }
            else if (keybanngou == 0) {//スペース→決定
                if (menu_num_S == 0) {//●新規作成
                    system("cls");//画面をまっさらにする
                    S2.viewScreWord();
                    menu_num_S2 = 0;
                    while (1) {////////////////////////////////////////////////★(2)while　新規作成ループ
                        keybanngou = 0;//初期化
                        keybanngou = keyOnOff(false);
                        if (keybanngou == 1) {//上
                            if (menu_num_S2 == 0) menu_num_S2 = 3;
                            else if (menu_num_S2 == 1) menu_num_S2 = 0;
                            else if (menu_num_S2 == 2) menu_num_S2 = 1;
                            else menu_num_S2 = 2;
                        }
                        else if (keybanngou == 3) {//下
                            if (menu_num_S2 == 0) menu_num_S2 = 1;
                            else if (menu_num_S2 == 1) menu_num_S2 = 2;
                            else if (menu_num_S2 == 2) menu_num_S2 = 3;
                            else menu_num_S2 = 0;
                        }
                        else if (keybanngou == 0) {//スペース→決定
                            if (menu_num_S2 == 0) {//●●名前
                                system("cls");//画面をまっさらにする
                                cout << "名前を入力してください　：";
                                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//cinバッファ削除
                                while (cin >> mapname) {
                                    if (mapname == "") cout << "名前を入力してください!!";
                                    else break;
                                }
                                system("cls");//画面をまっさらにする
                                S2.pushScreWord(3, mapname);//変更後の名前の更新
                            }
                            else if (menu_num_S2 == 1) {//●●サイズ
                                system("cls");//画面をまっさらにする
                                cout << "縦のサイズを入力してください(11以上)　：";
                                FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//cinバッファ削除
                                cin >> tate;
                                if (cin.fail()) {//整数以外エラー
                                    cin.clear();
                                    cin.ignore(1024, '\n');
                                    system("cls");//画面をまっさらにする
                                }
                                if (tate >= 11) {
                                    cout << "横のサイズを入力してください(11以上)　：";
                                    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));//cinバッファ削除
                                    cin >> yoko;
                                    if (cin.fail()) {//整数以外エラー
                                        cin.clear();
                                        cin.ignore(1024, '\n');
                                        system("cls");//画面をまっさらにする
                                    }
                                    if (yoko >= 11) {
                                        S2.pushScreWord(7, to_string(tate));//変更後の名前の更新
                                        S2.pushScreWord(9, to_string(yoko));
                                        system("cls");//画面をまっさらにする
                                    }
                                    else {//条件満たしてない
                                        system("cls");//画面をまっさらにする
                                    }
                                }
                                else {//条件満たしてない
                                    system("cls");//画面をまっさらにする
                                }
                            }
                            else if (menu_num_S2 == 2) {//●●完了
                                if (Vec.end() - Vec.begin() > 6) Stage.setSaveOK(false);
                                else Stage.setSaveOK(true);
                                Stage.setName(mapname);
                                Stage.setTate(to_string(tate));
                                Stage.setYoko(to_string(yoko));
                                //マップはStageクラスで自動生成する
                                toMap = true;
                                system("cls");//画面をまっさらにする
                                break;
                            }
                            else {//●●キャンセル
                                system("cls");//画面をまっさらにする
                                S.viewScreWord();//最初のメイン画面に戻る
                                break;//戻る
                            }
                        }
                        if (menu_num_S2 == 0) {
                            S2.pushScreWord(1, "→"); S2.pushScreWord(5, "　"); S2.pushScreWord(11, "　"), S2.pushScreWord(13, "　");
                        }
                        else if (menu_num_S2 == 1) {
                            S2.pushScreWord(1, "　"); S2.pushScreWord(5, "→"); S2.pushScreWord(11, "　"), S2.pushScreWord(13, "　");
                        }
                        else if (menu_num_S2 == 2) {
                            S2.pushScreWord(1, "　"); S2.pushScreWord(5, "　"); S2.pushScreWord(11, "→"), S2.pushScreWord(13, "　");
                        }
                        else {
                            S2.pushScreWord(1, "　"); S2.pushScreWord(5, "　"); S2.pushScreWord(11, "　"), S2.pushScreWord(13, "→");
                        }
                        S2.viewScreWord();
                    }////////////////////////////////////////////////////////////////★(2)while　新規作成ループ　ここまで
                }
                else if (menu_num_S == 1) {//●マップ読み込み
                    inFileMap(Vec);//マップ読み込み
                    int choosedMap = chooseMap(Vec);
                    if (choosedMap == -1) {//キャンセル
                        system("cls");//画面をまっさらにする
                        S.viewScreWord();
                        continue;//戻る
                    }
                    if (Vec.end() - Vec.begin() > 7) Stage.setSaveOK(false);
                    else Stage.setSaveOK(true);
                    Stage.setName(Vec[choosedMap].getName());
                    Stage.setTate(to_string(Vec[choosedMap].getTate()));
                    Stage.setYoko(to_string(Vec[choosedMap].getYoko()));
                    Stage.setMapvec(Vec[choosedMap].getMapvec());
                    toMap = true;
                    stopTime(1);//スペース長押しですぐにメニュー開くのを防ぐ
                }
                else {//●終了
                    break;//ここで終了（これ以降は読み込まない）
                }
            }
            if (menu_num_S == 0) {
                S.pushScreWord(1, "→"); S.pushScreWord(3, "　"); S.pushScreWord(5, "　");
            }
            else if (menu_num_S == 1) {
                S.pushScreWord(1, "　"); S.pushScreWord(3, "→"); S.pushScreWord(5, "　");
            }
            else {
                S.pushScreWord(1, "　"); S.pushScreWord(3, "　"); S.pushScreWord(5, "→");
            }


            //ゲームスタート時はここから
            if (toMap == true) {//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★ゲームスタート
                system("cls");//画面をまっさらにする
                kuria = Stage.gameStart();//ゲームスタート
                Stage.resetClass();//内容リセット→デストラクタは使わない
                tate = 0;//初期化
                yoko = 0;//初期化
                S2.pushScreWord(7, "11");//描画も初期化
                S2.pushScreWord(9, "11");//初期化
                if (kuria == true) {//ゲームをクリアしたときは...
                    S.pushScreWord(0, "☆★☆★☆★☆★☆★☆★☆★☆★☆\n"
                        "★ーーーー＜迷路ゲーム＞ーーーー★\n"
                        "☆　　　　　　　　　　　　　　　☆\n"
                        "★　");
                    S.pushScreWord(2, "　●　新規作成　　　　　　★\n"
                        "☆　　　　　　　　　　　　　　　☆\n"
                        "★　");
                    S.pushScreWord(4, "　●　マップを読み込む　　★\n"
                        "☆　　　　　　　　　　　　　　　☆\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "☆　　　　　　　　　　　　　　　☆\n"
                        "★　");
                    S.pushScreWord(6, "　●　終了　　　　　　　　★\n"
                        "☆　　　　　　　　　　　　　　　☆\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "☆★☆★☆★☆★☆★☆★☆★☆★☆\n");
                }
                else {
                    S.pushScreWord(0, "★★★★★★★★★★★★★★★★★\n"
                        "★ーーーー＜迷路ゲーム＞ーーーー★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　");
                    S.pushScreWord(2, "　●　新規作成　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　");
                    S.pushScreWord(4, "　●　マップを読み込む　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　");
                    S.pushScreWord(6, "　●　終了　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★　　　　　　　　　　　　　　　★\n"
                        "★★★★★★★★★★★★★★★★★\n");
                }
            }
            S.viewScreWord();//なにもなかったらループ繰り返し
        }
    }////////////////////////////////////////////////////////////////////////////////////////////////////////////////★(1)while　メインメニューループ　ここまで
}
//-------------------------------------------------------------------------------------------------


