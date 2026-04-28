#pragma execution_character_set("utf-8")
#include "DxLib.h"
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// =====================
// 構造体
// =====================
struct Building {
    bool exists;
    int  level;
    int  cost;
    int  upgCost;
    int  maintenance;
    int  studentUp;
    int  repUp;
    int  devUp;
    int  upgStu;
    int  upgRep;
    int  upgDev;
    const char* name;
    const char* effect;
};

struct GameEvent {
    const char* title;
    const char* desc;
    const char* opt1;
    const char* opt2;
    int m1, s1, r1, d1;
    int m2, s2, r2, d2;
};

// =====================
// イベント定義（10種）
// =====================
GameEvent eventList[] = {
    { "有名教授からのオファー",
      "著名な教授が本学への赴任を\n希望している。",
      "高給で採用する (-900万円)",
      "丁重にお断りする",
      -900, 0, 20, 15,
         0, 0,  5,  0 },
    { "台風による施設被害",
      "大型台風が直撃し、\n施設の一部が損傷した。",
      "すぐに修繕する (-1000万円)",
      "しばらく放置する",
      -1000,  0,  10, 0,
         0, -5, -15, 0 },
    { "スポーツ大会で全国優勝！",
      "本学のチームが全国大会で\n見事優勝を果たした！",
      "大々的に宣伝する (-200万円)",
      "普通に発表する",
      -200, 10, 30, 0,
         0,  5, 10, 0 },
    { "大手企業からの寄付申し出",
      "有力企業があなたの大学に\n多額の寄付を申し出た。",
      "寄付を受け取る (+1000万円)",
      "独立性を守るため断る",
      1000, 0,  5, 0,
         0, 0, 20, 0 },
    { "学生による抗議デモ",
      "学費値上げに反対する大規模な\nデモが構内で起きた。",
      "要求を受け入れる (-300万円)",
      "要求を無視する",
      -300, 15, 20,  0,
         0,-10,-25,  0 },
    { "海外大学との提携話",
      "海外の名門大学から\n学術提携の申し出があった。",
      "提携を結ぶ (-300万円)",
      "今は見送る",
      -300, 20, 25, 10,
         0,  0,  5,  0 },
    { "教員の不正が発覚！",
      "教員の論文不正が週刊誌に\nすっぱ抜かれてしまった…",
      "謝罪会見を開く (-200万円)",
      "「調査中」と言い続ける",
      -200,  0,-10, 0,
         0,-15,-35, 0 },
    { "研究が世界的に注目される",
      "本学の研究室が画期的な発見を\n成し遂げ、世界中で話題に！",
      "国際シンポジウム開催 (-600万円)",
      "論文発表のみにとどめる",
      -600, 5, 20, 20,
         0, 5, 30, 10 },
    { "近隣住民からのクレーム",
      "夜間の騒音について\n近隣住民から苦情が届いた。",
      "防音工事をする (-500万円)",
      "話し合いで解決を試みる",
      -500, 0, 10,  0,
         0, 0, -8,  0 },
    { "有名OBから支援の申し出",
      "卒業生の実業家が\n母校への支援を申し出た。",
      "支援を受け取る (+1000万円)",
      "感謝状だけ贈る",
      1000, 0,  5, 0,
        0, 0, 18, 0 },
};
const int EVENT_COUNT = 10;

// =====================
// ゲーム状態
// =====================
int gameState = 0; // 0=イントロ 1=名前入力 2=ゲーム 3=エンディング

int    introPage = 0;
string uniName = "";
int    year = 1;

int money = 5000; // 万円（5000万円）★難化
int students = 10;
int reputation = 50;
int deviation = 40;

//                        exist lv cost  upgC  maint stu rep dev uStu uRep uDev  name      effect
Building lecture = { false, 1, 2000,  500,  300,  2,  0,  0,  2,   0,   0, "講義棟", "学生+2/年" };
Building library = { false, 1, 1500,  500,  250,  1,  0,  1,  1,   0,   1, "図書館", "学生+1 偏差値+1/年" };
Building cafeteria = { false, 1, 1000,  500,  180,  0,  5,  0,  0,   5,   0, "食堂",   "評判+5/年" };
Building gym = { false, 1,  200, 1000,  700,  0, 10,  0,  0,  10,   0, "体育館", "評判+10/年" };
Building research = { false, 1, 1500,  500,  450,  0,  5,  3,  0,   5,   3, "研究棟", "評判+5 偏差値+3/年" };
Building dormitory = { false, 1, 1400,  500,  220,  3,  2,  0,  3,   2,   0, "学生寮", "学生+3 評判+2/年" };

Building* allBldg[6] = { &lecture, &library, &cafeteria, &gym, &research, &dormitory };

// 広報
int adMode = 0;

// UI
bool  showInfo = false;
float fade = 0.0f;

// イベント
bool eventActive = false;
int  currentEventIdx = -1;
bool eventPrev1 = false;
bool eventPrev2 = false;

// 自動進行
float autoTimer = 0.0f;
float autoSpeed = 1.0f; // 秒/年
bool  autoPause = false;

// キー前フレーム
bool prevEnter = false;
bool prevEsc = false;
bool prevK[6] = {};

// エンディング
int endingType = 0;
string bankruptReason = "";

// イントロテキスト
const char* introLines[] = {
    "ある日、あなたに\n信じられない知らせが届いた。",
    "「おめでとうございます。\n宝くじで５千万円が当選しました。」",
    "あなたはずっと夢見ていた。\n\n「いつか、最高の大学を作りたい」と。",
    "今こそ、その夢を叶える時だ。\n施設を整え、学生を集め、\n世界に名を轟かせろ！",
};
const int INTRO_PAGES = 4;

// =====================
// リセット関数（大学名は保持）
// =====================
void ResetGame()
{
    year = 1;
    money = 5000;
    students = 10;
    reputation = 50;
    deviation = 40;
    adMode = 0;
    showInfo = false;
    fade = 0.0f;
    eventActive = false;
    currentEventIdx = -1;
    eventPrev1 = false;
    eventPrev2 = false;
    autoTimer = 0.0f;
    autoSpeed = 1.0f;
    autoPause = false;
    prevEnter = false;
    prevEsc = false;
    for (int i = 0; i < 6; i++) prevK[i] = false;
    endingType = 0;
    bankruptReason = "";

    lecture = { false,1,2000, 500,300,2,0,0,2,0,0,"講義棟","学生+2/年" };
    library = { false,1,1500, 500,250,1,0,1,1,0,1,"図書館","学生+1 偏差値+1/年" };
    cafeteria = { false,1,1000, 500,180,0,5,0,0,5,0,"食堂",  "評判+5/年" };
    gym = { false,1, 200,1000,700,0,10,0,0,10,0,"体育館","評判+10/年" };
    research = { false,1,1500, 500,450,0,5,3,0,5,3,"研究棟","評判+5 偏差値+3/年" };
    dormitory = { false,1,1400, 500,220,3,2,0,3,2,0,"学生寮","学生+3 評判+2/年" };

    gameState = 2; // 名前入力をスキップしてゲームへ
}

// =====================
// 年進行関数
// =====================
void AdvanceYear()
{
    year++;

    if (reputation < 0) reputation = 0;
    if (deviation < 0) deviation = 0;

    // =====================
    // 自然減少（強化）
    // =====================
    reputation -= 5;  // 6 → 5（少し緩和）
    deviation -= 3;  // 4 → 3（少し緩和）
    if (reputation < 0) reputation = 0;
    if (deviation < 0) deviation = 0;

    int stuChange = 0;  // ★ 自然増なし（1 → 0）
    if (adMode == 1) stuChange += 3;
    if (adMode == 2) reputation += 2;
    if (adMode == 3) money += 50;

    if (deviation > 80) deviation = 80;

    // =====================
    // 収入（弱体化）
    // =====================
    int income = students * 20 + reputation * 7; // ★ ×30→×20、×10→×7
    int expense = 150; // 200 → 150（少し緩和）

    for (int i = 0; i < 6; i++) {
        Building& b = *allBldg[i];
        if (b.exists) {
            expense += b.maintenance;
            stuChange += b.studentUp;
            reputation += b.repUp;
            deviation += b.devUp;
        }
    }

    // =====================
    // 評判低下ペナルティ（段階化）
    // =====================
    if (reputation < 30) stuChange -= 12; // ★ 閾値20→30、-10→-12
    else if (reputation < 50) stuChange -= 4;  // ★ 中間ペナルティ追加

    // =====================
    // 大規模化コスト（学生が多いと維持費増）
    // =====================
    if (students > 50)  expense += (students - 50) * 5;
    if (students > 100) expense += (students - 100) * 10;

    students += stuChange;
    money += income - expense;

    // =====================
    // 老化イベント（70年以降、確率UP）
    // =====================
    if (year >= 70 && rand() % 100 < 40) { // ★ 30% → 40%
        int aging = rand() % 4;
        if (aging == 0) money -= 800;  // ★ 500 → 800
        else if (aging == 1) students -= 8;    // ★ 5   → 8
        else if (aging == 2) reputation -= 15;   // ★ 10  → 15
        else                 deviation -= 8;    // ★ 5   → 8
    }

    // =====================
    // 通常イベント
    // =====================
    if (rand() % 9 == 0) {
        currentEventIdx = rand() % EVENT_COUNT;
        eventActive = true;
        eventPrev1 = true;
        eventPrev2 = true;
    }

    // =====================
    // 破綻チェック
    // =====================
    if (money < 0 || students <= 0) {
        endingType = 0;
        if (money < 0 && students <= 0)
            bankruptReason = "資金が底をつき、学生もいなくなってしまった…";
        else if (money < 0)
            bankruptReason = "資金が底をつき、運営を続けられなくなった…";
        else
            bankruptReason = "学生がいなくなり、大学としての体をなさなくなった…";
        gameState = 3;
        return;
    }

    // =====================
    // 寿命エンディング（100年）
    // =====================
    if (year >= 100) {
        int score = reputation * 3 + students * 2
            + deviation * 4 + money / 500;

        if (score >= 3000) endingType = 3;
        else if (score >= 1800) endingType = 2;
        else if (score >= 800)  endingType = 1;
        else                    endingType = 0;

        gameState = 3;
    }
}

// =====================
// メイン
// =====================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    srand((unsigned)time(NULL));

    ChangeWindowMode(TRUE);
    SetGraphMode(1280, 720, 32);
    if (DxLib_Init() == -1) return -1;
    SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

    int imgs[6];
    imgs[0] = LoadGraph("lecture.png");
    imgs[1] = LoadGraph("library.png");
    imgs[2] = LoadGraph("cafeteria.png");
    imgs[3] = LoadGraph("gym.png");
    imgs[4] = LoadGraph("research.png");
    imgs[5] = LoadGraph("dormitory.png");

    int font = CreateFontToHandle("Meiryo", 22, 3);
    int fontSm = CreateFontToHandle("Meiryo", 16, 2);
    int fontLg = CreateFontToHandle("Meiryo", 36, 5);

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        bool escNow = CheckHitKey(KEY_INPUT_ESCAPE);
        bool enterNow = CheckHitKey(KEY_INPUT_RETURN);
        bool upNow = CheckHitKey(KEY_INPUT_UP);
        bool downNow = CheckHitKey(KEY_INPUT_DOWN);
        bool kNow[6] = {
            (bool)CheckHitKey(KEY_INPUT_1),
            (bool)CheckHitKey(KEY_INPUT_2),
            (bool)CheckHitKey(KEY_INPUT_3),
            (bool)CheckHitKey(KEY_INPUT_4),
            (bool)CheckHitKey(KEY_INPUT_5),
            (bool)CheckHitKey(KEY_INPUT_6),
        };

        if (escNow && !prevEsc && gameState == 2 && !eventActive)
            showInfo = !showInfo;
        prevEsc = escNow;

        if (showInfo) fade += 12; else fade -= 12;
        if (fade < 0)   fade = 0;
        if (fade > 200) fade = 200;

        // =====================================================
        // ■ イントロ
        // =====================================================
        if (gameState == 0)
        {
            DrawBox(0, 0, 1280, 720, GetColor(8, 8, 28), TRUE);
            DrawStringToHandle(380, 60, "大学100年経営シミュレーション", GetColor(255, 220, 0), font);
            DrawStringToHandle(160, 230, introLines[introPage], GetColor(255, 255, 255), fontLg);

            string pg = to_string(introPage + 1) + " / " + to_string(INTRO_PAGES);
            DrawStringToHandle(580, 630, pg.c_str(), GetColor(140, 140, 140), fontSm);
            DrawStringToHandle(420, 660, "ENTER でつぎへ", GetColor(200, 200, 100), fontSm);

            if (enterNow && !prevEnter) {
                introPage++;
                if (introPage >= INTRO_PAGES) { introPage = 0; gameState = 1; }
            }
            prevEnter = enterNow;
            ScreenFlip();
            continue;
        }

        // =====================================================
        // ■ 名前入力
        // =====================================================
        if (gameState == 1)
        {
            DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
            DrawStringToHandle(380, 180, "大学名を入力（5文字以内）", GetColor(255, 255, 0), font);
            DrawStringToHandle(420, 310, uniName.c_str(), GetColor(255, 255, 255), fontLg);
            DrawStringToHandle(500, 460, "ENTERで開始", GetColor(255, 255, 0), font);

            int key = GetInputChar(TRUE);
            if (key >= 32 && key <= 126 && (int)uniName.size() < 5) uniName += (char)key;
            if (CheckHitKey(KEY_INPUT_BACK) && !uniName.empty()) uniName.pop_back();
            if (enterNow && !prevEnter && uniName.size() > 0) gameState = 2;

            prevEnter = enterNow;
            ScreenFlip();
            continue;
        }

        // =====================================================
        // ■ エンディング
        // =====================================================
        if (gameState == 3)
        {
            showInfo = false;
            DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);

            DrawStringToHandle(250, 100, (uniName + "大学　最終報告").c_str(), GetColor(255, 255, 0), fontLg);
            DrawStringToHandle(300, 200, ("最終年度: " + to_string(year) + "年").c_str(), GetColor(200, 200, 200), font);
            DrawStringToHandle(300, 238, ("学生数:   " + to_string(students)).c_str(), GetColor(255, 255, 255), font);
            DrawStringToHandle(300, 276, ("評判:     " + to_string(reputation)).c_str(), GetColor(255, 255, 255), font);
            DrawStringToHandle(300, 314, ("偏差値:   " + to_string(deviation)).c_str(), GetColor(255, 255, 255), font);
            DrawStringToHandle(300, 352, ("残金:     " + to_string(money) + "万円").c_str(), GetColor(100, 255, 100), font);

            if (endingType == 3) {
                DrawStringToHandle(200, 440, "★  世界最高の大学  ★", GetColor(255, 220, 0), fontLg);
                DrawStringToHandle(200, 520, "あなたの大学は世界中から称えられる頂点に立った！", GetColor(255, 255, 255), font);
            }
            else if (endingType == 2) {
                DrawStringToHandle(200, 440, "【  有名大学として成功  】", GetColor(100, 200, 255), fontLg);
                DrawStringToHandle(200, 520, "多くの優秀な学生が集まる名門校になった。", GetColor(255, 255, 255), font);
            }
            else if (endingType == 1) {
                DrawStringToHandle(200, 440, "[  普通の大学  ]", GetColor(180, 180, 180), fontLg);
                DrawStringToHandle(200, 520, "地域に根ざした大学として存続した。", GetColor(255, 255, 255), font);
            }
            else {
                DrawStringToHandle(200, 440, "×  経営破綻  ×", GetColor(255, 50, 50), fontLg);
                // 破綻理由を表示
                DrawStringToHandle(200, 510, bankruptReason.c_str(), GetColor(255, 180, 180), font);
                DrawStringToHandle(200, 548, "５千万円の夢は虚しく散った…", GetColor(180, 180, 180), fontSm);
                // リスタート案内
                DrawBox(200, 590, 800, 635, GetColor(40, 10, 10), TRUE);
                DrawBox(200, 590, 800, 635, GetColor(200, 80, 80), FALSE);
                DrawStringToHandle(220, 604, "Rキー: 同じ大学名でやり直す", GetColor(255, 200, 100), font);
            }

            DrawStringToHandle(530, 660, "Mキーで終了", GetColor(200, 200, 200), fontSm);

            // リスタート（破綻時のみ）
            if (endingType == 0 && CheckHitKey(KEY_INPUT_R)) {
                ResetGame();
                ScreenFlip();
                continue;
            }
            if (CheckHitKey(KEY_INPUT_M)) break;

            ScreenFlip();
            continue;
        }

        // =====================================================
        // ■ ゲーム本体
        // =====================================================
        if (gameState == 2)
        {
            // -------------------------------------------------
            // イベントオーバーレイ
            // -------------------------------------------------
            if (eventActive)
            {
                GameEvent& ev = eventList[currentEventIdx];

                DrawBox(0, 0, 1280, 720, GetColor(0, 0, 18), TRUE);
                DrawBox(80, 80, 1200, 640, GetColor(18, 18, 48), TRUE);
                DrawBox(82, 82, 1198, 638, GetColor(90, 90, 200), FALSE);

                DrawStringToHandle(120, 110, "！  イベント発生  ！", GetColor(255, 220, 0), fontLg);
                DrawStringToHandle(120, 200, ev.title, GetColor(255, 255, 200), font);
                DrawStringToHandle(120, 245, ev.desc, GetColor(220, 220, 220), fontSm);

                DrawBox(100, 330, 590, 420, GetColor(20, 50, 100), TRUE);
                DrawBox(100, 330, 590, 420, GetColor(80, 140, 255), FALSE);
                DrawStringToHandle(120, 365, ("1: " + string(ev.opt1)).c_str(), GetColor(100, 200, 255), font);

                DrawBox(620, 330, 1160, 420, GetColor(80, 20, 20), TRUE);
                DrawBox(620, 330, 1160, 420, GetColor(255, 80, 80), FALSE);
                DrawStringToHandle(640, 365, ("2: " + string(ev.opt2)).c_str(), GetColor(255, 140, 140), font);

                DrawStringToHandle(420, 490, "1 か 2 のキーで選択", GetColor(200, 200, 100), font);

                if (kNow[0] && !eventPrev1) {
                    money += ev.m1; students += ev.s1;
                    reputation += ev.r1; deviation += ev.d1;
                    if (students < 0) students = 0;
                    if (reputation < 0) reputation = 0;
                    eventActive = false;
                }
                if (kNow[1] && !eventPrev2) {
                    money += ev.m2; students += ev.s2;
                    reputation += ev.r2; deviation += ev.d2;
                    if (students < 0) students = 0;
                    if (reputation < 0) reputation = 0;
                    eventActive = false;
                }

                eventPrev1 = kNow[0];
                eventPrev2 = kNow[1];
                ScreenFlip();
                continue;
            }

            // -------------------------------------------------
            // 通常入力
            // -------------------------------------------------
            if (!showInfo)
            {
                // 施設建設 / アップグレード
                for (int i = 0; i < 6; i++) {
                    Building& b = *allBldg[i];
                    if (kNow[i] && !prevK[i]) {
                        if (!b.exists && money >= b.cost) {
                            money -= b.cost;
                            b.exists = true;
                        }
                        else if (b.exists && b.level < 3 && money >= b.upgCost) {
                            money -= b.upgCost;
                            b.studentUp += b.upgStu;
                            b.repUp += b.upgRep;
                            b.devUp += b.upgDev;
                            b.maintenance += b.maintenance / 3;
                            b.upgCost = b.upgCost * 3 / 2;
                            b.level++;
                        }
                    }
                    prevK[i] = kNow[i];
                }

                // 広報
                if (CheckHitKey(KEY_INPUT_Z)) adMode = 0;
                if (CheckHitKey(KEY_INPUT_X)) adMode = 1;
                if (CheckHitKey(KEY_INPUT_C)) adMode = 2;
                if (CheckHitKey(KEY_INPUT_V)) adMode = 3;

                // 一時停止トグル
                if (enterNow && !prevEnter) autoPause = !autoPause;

                // 速度変更
                if (upNow)   autoSpeed -= 0.04f;
                if (downNow) autoSpeed += 0.04f;
                if (autoSpeed < 0.4f) autoSpeed = 0.4f;
                if (autoSpeed > 5.4f) autoSpeed = 5.4f;

                // 自動進行タイマー
                if (!autoPause) {
                    autoTimer += 1.0f / 60.0f;
                    if (autoTimer >= autoSpeed) {
                        autoTimer = 0.0f;
                        AdvanceYear();
                    }
                }
            }
            prevEnter = enterNow;

            // -------------------------------------------------
            // マップ描画
            // -------------------------------------------------
            DrawBox(50, 50, 800, 670, GetColor(155, 185, 155), TRUE);

            int bx1[6] = { 60, 310, 560,  60, 310, 560 };
            int by1[6] = { 60,  60,  60, 370, 370, 370 };
            int bx2[6] = { 300, 550, 795, 300, 550, 795 };
            int by2[6] = { 360, 360, 360, 660, 660, 660 };
            int defClr[6][3] = {
                { 100, 100, 255 }, { 100, 255, 100 }, { 255, 160,   0 },
                { 255,  60,  60 }, { 160,   0, 255 }, {   0, 210, 210 }
            };

            for (int i = 0; i < 6; i++) {
                Building& b = *allBldg[i];
                int cx = (bx1[i] + bx2[i]) / 2;
                int cy = (by1[i] + by2[i]) / 2;

                if (b.exists) {
                    if (imgs[i] != -1)
                        DrawExtendGraph(bx1[i], by1[i], bx2[i], by2[i], imgs[i], TRUE);
                    else
                        DrawBox(bx1[i], by1[i], bx2[i], by2[i],
                            GetColor(defClr[i][0], defClr[i][1], defClr[i][2]), TRUE);

                    // Lvバッジ
                    string lvStr = "Lv" + to_string(b.level);
                    DrawBox(bx2[i] - 40, by1[i] + 4, bx2[i] - 4, by1[i] + 26, GetColor(0, 0, 0), TRUE);
                    DrawStringToHandle(bx2[i] - 38, by1[i] + 6, lvStr.c_str(), GetColor(255, 220, 0), fontSm);

                    // 施設名
                    DrawStringToHandle(bx1[i] + 5, by2[i] - 22, b.name, GetColor(255, 255, 255), fontSm);

                    // 強化可能表示
                    if (b.level < 3 && money >= b.upgCost)
                        DrawStringToHandle(bx1[i] + 5, by1[i] + 6,
                            ("↑強化可 " + to_string(b.upgCost) + "万").c_str(),
                            GetColor(255, 255, 0), fontSm);
                }
                else {
                    DrawBox(bx1[i], by1[i], bx2[i], by2[i], GetColor(135, 135, 135), TRUE);
                    DrawBox(bx1[i] + 2, by1[i] + 2, bx2[i] - 2, by2[i] - 2, GetColor(100, 100, 100), FALSE);
                    DrawStringToHandle(cx - 60, cy - 40,
                        (to_string(i + 1) + ": " + string(b.name)).c_str(),
                        GetColor(50, 50, 50), font);
                    DrawStringToHandle(cx - 70, cy,
                        b.effect,
                        GetColor(70, 70, 70), fontSm);
                    DrawStringToHandle(cx - 70, cy + 24,
                        ("建設: " + to_string(b.cost) + "万円").c_str(),
                        money >= b.cost ? GetColor(80, 160, 80) : GetColor(160, 80, 80), fontSm);
                }
            }

            // -------------------------------------------------
            // 右側UI
            // -------------------------------------------------
            int rx = 820;

            DrawStringToHandle(rx, 30, ("年:     " + to_string(year)).c_str(), GetColor(255, 255, 255), font);
            DrawStringToHandle(rx, 65, (uniName + "大学").c_str(), GetColor(255, 255, 0), font);
            DrawStringToHandle(rx, 105, ("所持金: " + to_string(money) + "万円").c_str(), GetColor(100, 255, 100), font);
            DrawStringToHandle(rx, 140, ("学生数: " + to_string(students)).c_str(), GetColor(255, 255, 255), font);
            DrawStringToHandle(rx, 175, ("評判:   " + to_string(reputation)).c_str(), GetColor(255, 200, 100), font);
            DrawStringToHandle(rx, 210, ("偏差値: " + to_string(deviation)).c_str(), GetColor(100, 200, 255), font);

            int income = students * 20 + reputation * 7; // ★難化後の計算式
            int expense = 150;
            for (int i = 0; i < 6; i++) if (allBldg[i]->exists) expense += allBldg[i]->maintenance;
            if (students > 50)  expense += (students - 50) * 5;
            if (students > 100) expense += (students - 100) * 10;
            int balance = income - expense;

            DrawStringToHandle(rx, 255, ("収入: +" + to_string(income) + "万/年").c_str(), GetColor(150, 220, 150), fontSm);
            DrawStringToHandle(rx, 277, ("支出: -" + to_string(expense) + "万/年").c_str(), GetColor(220, 150, 150), fontSm);
            DrawStringToHandle(rx, 299,
                ("収支: " + string(balance >= 0 ? "+" : "") + to_string(balance) + "万/年").c_str(),
                balance >= 0 ? GetColor(100, 255, 100) : GetColor(255, 100, 100), fontSm);

            // 自動進行UI
            DrawStringToHandle(rx, 330, "─────────────────", GetColor(70, 70, 70), fontSm);
            DrawStringToHandle(rx, 353,
                autoPause ? "⏸  一時停止中" : "▶  自動進行中",
                autoPause ? GetColor(255, 100, 100) : GetColor(100, 255, 100), font);

            // 速度バー
            int barW = 200;
            int barX = rx;
            int barY = 395;
            DrawBox(barX, barY, barX + barW, barY + 16, GetColor(50, 50, 50), TRUE);
            int filled = (int)((5.0f - autoSpeed) / 4.8f * barW);
            if (filled < 0)    filled = 0;
            if (filled > barW) filled = barW;
            DrawBox(barX, barY, barX + filled, barY + 16, GetColor(80, 180, 255), TRUE);
            DrawStringToHandle(barX, barY + 18,
                ("速度: " + to_string((int)(1.0f / autoSpeed * 10) / 10) + "年/秒").c_str(),
                GetColor(200, 200, 200), fontSm);
            DrawStringToHandle(barX, barY + 38, "↑:速く  ↓:遅く  ENTER:停止", GetColor(150, 150, 150), fontSm);

            DrawStringToHandle(rx, 458, "─────────────────", GetColor(70, 70, 70), fontSm);
            DrawStringToHandle(rx, 478, "1〜6: 施設建設・強化", GetColor(180, 180, 180), fontSm);
            DrawStringToHandle(rx, 498, "同キー再押し: 強化（Lv3まで）", GetColor(180, 180, 180), fontSm);
            DrawStringToHandle(rx, 518, "ESC: ヘルプ", GetColor(180, 180, 180), fontSm);

            DrawStringToHandle(rx, 548, "─────────────────", GetColor(70, 70, 70), fontSm);
            DrawStringToHandle(rx, 568, "【 広報 】", GetColor(200, 200, 255), font);
            DrawStringToHandle(rx, 600, "Z: なし", GetColor(200, 200, 200), fontSm);
            DrawStringToHandle(rx, 620, "X: 理工（学生+3/年）", GetColor(100, 200, 255), fontSm);
            DrawStringToHandle(rx, 640, "C: 文学（評判+2/年）", GetColor(255, 200, 100), fontSm);
            DrawStringToHandle(rx, 660, "V: 経済（収入+50万/年）", GetColor(100, 255, 100), fontSm);

            const char* adTxt =
                adMode == 1 ? ">>> 理工広報中 <<<" :
                adMode == 2 ? ">>> 文学広報中 <<<" :
                adMode == 3 ? ">>> 経済広報中 <<<" : "広報なし";
            DrawStringToHandle(rx, 686, adTxt, GetColor(255, 255, 0), fontSm);
        }

        // =====================================================
        // ■ ESCヘルプオーバーレイ
        // =====================================================
        if (fade > 0 && gameState == 2)
        {
            DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
            DrawBox(60, 50, 1220, 670, GetColor(12, 12, 38), TRUE);
            DrawBox(62, 52, 1218, 668, GetColor(70, 70, 160), FALSE);

            DrawStringToHandle(100, 75, "【 ヘルプ 】", GetColor(255, 255, 0), fontLg);

            DrawStringToHandle(100, 165, "■ 施設一覧", GetColor(200, 200, 255), font);
            DrawStringToHandle(100, 200, "1:講義棟  建設2000万  維持300万  学生+2/年  強化で+2", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 222, "2:図書館  建設1500万  維持250万  学生+1 偏差値+1/年", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 244, "3:食堂    建設1000万  維持180万  評判+5/年  強化で+5", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 266, "4:体育館  建設 200万  維持700万  評判+10/年 強化で+10", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 288, "5:研究棟  建設1500万  維持450万  評判+5 偏差値+3/年", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 310, "6:学生寮  建設1400万  維持220万  学生+3 評判+2/年", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 332, "※ 施設はLv3まで強化可。同じキーをもう一度押す。", GetColor(255, 220, 80), fontSm);

            DrawStringToHandle(100, 375, "■ 収入の仕組み", GetColor(200, 200, 255), font);
            DrawStringToHandle(100, 410, "収入 = 学生数 × 20 + 評判 × 7  （万円/年）", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 432, "支出 = 200万（固定） + 建設した施設の維持費", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 454, "※ 学生数50超で追加維持費、100超でさらに増加", GetColor(255, 220, 80), fontSm);

            DrawStringToHandle(100, 492, "■ イベント", GetColor(200, 200, 255), font);
            DrawStringToHandle(100, 527, "年を進めると一定確率でランダムイベントが起きる。", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 549, "1か2のキーで選択肢を選ぶ。どちらを選ぶかで結果が変わる！", GetColor(255, 255, 255), fontSm);

            DrawStringToHandle(100, 588, "■ エンディング（100年後）", GetColor(200, 200, 255), font);
            DrawStringToHandle(100, 623, "スコア = 評判×3 + 学生×2 + 偏差値×4 + 所持金/500", GetColor(255, 255, 255), fontSm);
            DrawStringToHandle(100, 645, "3000↑:世界最高  1800↑:有名  800↑:普通  以下:破綻", GetColor(255, 255, 255), fontSm);

            DrawStringToHandle(500, 650, "ESCキーで閉じる", GetColor(140, 140, 140), fontSm);
        }

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}
