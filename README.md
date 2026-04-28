# 大学100年経営シミュレーション

宝くじで当たった5000万円を元手に、100年かけて大学を育てる経営シミュレーションゲームです。  
施設を建設・強化し、学生を集め、評判と偏差値を高めて世界最高の大学を目指してください。

---

## 機能概要

| 機能 | 説明 |
|------|------|
| 施設建設・強化 | 講義棟・図書館・食堂・体育館・研究棟・学生寮の6種類を建設。各施設はLv3まで強化可能 |
| 広報選択 | 理工・文学・経済の3種類の広報戦略から選択。毎年の学生数・評判・収入に影響 |
| 自動進行 | 年が自動で経過。速度調整（↑↓キー）と一時停止（ENTERキー）に対応 |
| ランダムイベント | 毎年一定確率で10種類のイベントが発生。2択の選択肢によって結果が分岐 |
| 収支管理 | 収入＝学生数×20＋評判×7。支出＝固定費150万＋各施設維持費 |
| エンディング | 100年後のスコアで4段階評価（世界最高・有名・普通・破綻）。破綻時は原因を表示してリスタート可能 |

### 操作一覧

| キー | 操作 |
|------|------|
| `1`〜`6` | 施設の建設 / 同キー再押しで強化 |
| `Z` / `X` / `C` / `V` | 広報なし / 理工 / 文学 / 経済 |
| `↑` / `↓` | 進行速度を上げる / 下げる |
| `ENTER` | 一時停止・再開 |
| `ESC` | ヘルプ表示切替 |
| `R` | 破綻後に同じ大学名でリスタート |
| `M` | ゲーム終了 |

---

## 開発環境

| 項目 | 内容 |
|------|------|
| 言語 | C++17 |
| ライブラリ | DXライブラリ 3.x（Windows向け） |
| IDE | Visual Studio 2022（推奨） |
| 対応OS | Windows 10 / 11 (64bit) |
| 文字コード | UTF-8（`#pragma execution_character_set("utf-8")`） |
| フォント | Meiryo（システムフォント使用） |

---

## 運用方針

- 本プロジェクトは個人開発・学習目的で制作されています。
- バグ修正・パラメータ調整は `main.cpp` 内の各種定数を直接変更してください。
- 画像ファイル（`lecture.png` 等）がない場合、施設はカラーボックスで代替表示されます。ゲームの進行には影響しません。
- セーブ機能は未実装です。ゲームを終了すると進行状況はリセットされます。

---

## 環境の構築方法・インストール方法

### 1. DXライブラリの導入

1. [DXライブラリ公式サイト](https://dxlib.xsrv.jp/) から最新版をダウンロード
2. ZIP を解凍し、以下のファイルを Visual Studio プロジェクトフォルダにコピー

```
DxLib.h
DxLib.lib        （または DxLib_x64.lib）
DxLib.dll        （または DxLib_x64.dll）
```

3. Visual Studio のプロジェクト設定を変更

```
[プロジェクト] → [プロパティ]
  ├ 構成プロパティ → C/C++ → 全般 → 追加のインクルードディレクトリ: DXライブラリのパスを追加
  ├ リンカー → 入力 → 追加の依存ファイル: DxLib.lib を追加
  └ リンカー → システム → サブシステム: Windows (/SUBSYSTEM:WINDOWS)
```

### 2. プロジェクトのビルド

```bash
# Visual Studio の場合
# ソリューションを開いてビルド（Ctrl+Shift+B）

# または cl.exe を直接使う場合
cl /EHsc /utf-8 main.cpp DxLib.lib /link /SUBSYSTEM:WINDOWS
```

### 3. 実行ファイルと同じフォルダに配置するファイル

```
main.exe           # ビルド生成物
DxLib.dll          # DXライブラリ DLL
lecture.png        # 施設画像（任意）
library.png
cafeteria.png
gym.png
research.png
dormitory.png
```

> 画像ファイルは省略可能です。ない場合は色付きボックスで代替されます。

---

## ライセンス

本プロジェクトのソースコード（`main.cpp`）は **MIT License** のもとで公開しています。

```
MIT License

Copyright (c) 2025

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

> **注意:** DXライブラリ自体は独自のライセンスが適用されます。  
> 詳細は [DXライブラリ利用規約](https://dxlib.xsrv.jp/dxlicense.html) を確認してください。

---

## ディレクトリ構成

```
university-sim/
│
├── main.cpp              # ゲーム本体（全ソースコード）
├── README.md             # 本ドキュメント
│
├── DxLib.h               # DXライブラリ ヘッダ（別途取得）
├── DxLib.lib             # DXライブラリ スタティックライブラリ（別途取得）
├── DxLib.dll             # DXライブラリ DLL（実行時に必要）
│
├── lecture.png           # 講義棟の画像（任意）
├── library.png           # 図書館の画像（任意）
├── cafeteria.png         # 食堂の画像（任意）
├── gym.png               # 体育館の画像（任意）
├── research.png          # 研究棟の画像（任意）
└── dormitory.png         # 学生寮の画像（任意）
```

> ビルド後に `main.exe` が同フォルダに生成されます。  
> DXライブラリ関連ファイルは公式サイトから取得し、このフォルダに配置してください。
