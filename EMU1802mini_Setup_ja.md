# EMU1802-mini セットアップマニュアル

## EMU1802-miniとは
EMU1802-miniはCOSMAC CDP1802 CPUとPIC18F27Q43だけで動作するシングルボードコンピュータです。   
PIC18F27Q43とPICに書き込むファームウェアで周辺デバイスであるROM, RAM, UARTの機能を実現しています。   
Tetsuya Suzuki氏が開発した[EMUZ80](https://vintagechips.wordpress.com/2022/03/05/emuz80_reference/)を参考にして製作しています。  
COSMAC CPUについては[COSMAC研究会](https://kanpapa.com/cosmac/cosmac-cpu.html)のサイトをご覧ください。
これまでの経緯をまとめた[EMU1802アーカイブ](https://kanpapa.com/cosmac/blog/emu1802/)もあります。

EMU1802-miniの完成写真  
![EMU1802-mini](/pictures/emu1802-mini.jpg)

## ハードウェアの製作
1. [回路図](/kicad/emu1802_mini_sch.pdf)と[BOM](/kicad/emu1802_mini_bom.md)を参考にしてパーツを入手してください。
1.  [EMU1802-mini専用プリント基板](https://store.shopping.yahoo.co.jp/orangepicoshop/pico-A-057.html)があればパーツを基板に実装してハンダ付けします。基板がない場合はブレッドボードを使って配線しても良いでしょう。

- [EMU1802-mini専用プリント基板](https://store.shopping.yahoo.co.jp/orangepicoshop/pico-A-057.html)について
   - [オレンジピコショップ](https://store.shopping.yahoo.co.jp/orangepicoshop/)さんで購入できます。
   - EMU1802-mini専用基板を使用する際の注意事項
      - 基板には最低限の情報しかシルク印刷されていません。作業開始前に回路図を確認することをお勧めします。
      - CON4は電源入力です。１番ピンがVccです。２番ピンと３番ピンはGNDです。誤ってショートしないように注意してください。
      - CON2, CON3はUSBシリアルを接続するコネクタです。１番ピンがGNDになります。
      - USBシリアルから電源を供給する場合は裏面のJP1またはJP2を半田でショートしてください。
      - 省スペースのために1/6W抵抗を使用していますが、1/4W抵抗の場合はリード線を曲げて縦に取り付けてください。

## 開発環境の準備
- [Microchip MPLAB X IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)が必要です。  
- ファームウェアの書き込みには[Microchip MPLAB SNAP](https://akizukidenshi.com/catalog/g/gM-13854)を使用します。
- ファームウェアのROM領域を変更する場合は[SB-Assembler 3](https://www.sbprojects.net/sbasm/)を使用します。Python3が動作する環境が必要です。私の場合はWSL2環境にインストールしています。  

## ファームウェアのビルドと書き込み手順
EMU1802-miniはCOSMACのプログラムをPICのファームウェアに組み込んで使います。好きなプログラムを動かすためにはファームウェアのビルドは必須です。
1. [emu1802mini.X](/emu1802.X)を適当な場所に展開します。
1. [Microchip MPLAB X IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)を起動します。
1. プロジェクトを開くで展開したemu1802mini.Xフォルダを指定して開きます。
1. MPLAB IDEでビルドを行いコンパイルができることを確認します。
1. EMU1802-miniのCON1にMPLAB SNAPを接続し、PCのUSBに接続します。
1. EMU1802-miniの電源を投入します。
1. MPLAB IDEでターゲットのPICに書き込みを行います。

## ビルド済みファームウェアの書き込み手順
[production](/production)にIDIOTモニタが含まれたビルド済のファームウェアのHEXファイルがありますので、これを利用することもできます。
1. 使用したいファームウェアをダウンロードしてください。詳しくは[README.md](/production/emu1802mini/README.md)を参照してください。
1. EMU1802-miniのCON1にMPLAB SNAPを接続し、PCのUSBに接続します。
1. EMU1802-miniの電源を投入します。
1. MPLAB IPEを起動します。
1. Device and Tool Selectionで、DeviceでPIC18F27Q43を選択し、ToolはSNAPを選択してください。
1. Hex FileのBrowseボタンを押して、ダウンロードしたファームウェアのHEXファイルを指定してください。
1. Erase→Blank Check→Program→Verifyを順に実行し、ターゲットのPICに書き込みます。

## EMU1802-miniの動作確認

ファームウェアV1.0の場合
1. USBシリアルをCON3（COSMACのQ/EF3）に接続します。
1. USBシリアルにPCを接続し、ターミナルプログラム(TeraTermなど）で300bps、スタート1ビット、データ長7ビット、パリティMark、ストップ1ビットに設定します。
1. EMU1802-miniの電源を投入します。
1. Enterキーを押すと、「IDIOT/4」起動メッセージとプロンプトの「*」が表示されます。

ファームウェアV1.1/V2.1Gの場合
1. USBシリアルをCON2（PICのUART）に接続します。
1. USBシリアルにPCを接続し、ターミナルプログラム(TeraTermなど）で9600bps、スタート1ビット、データ長8ビット、パリティ無し、ストップ1ビットに設定します。
1. EMU1802-miniの電源を投入します。
1. 「IDIOT/4」起動メッセージとプロンプトの「*」が表示されます。

## IDIOTモニタの使い方

### 基本的なコマンド
指定する値は16進数です。

| コマンド | 機能 |
----|----
|?Maaaa xx|アドレスaaaaからxxバイト分のメモリをダンプします |
|!Maaaa dd..dd;|アドレスaaaaから、ddの内容を書き込みます。<br>セミコロンをつけると次の行の内容も継続して書き込まれます。<br>これにより?Mコマンドでダンプした内容をキャプチャしておけば、キャプチャした内容をそのまま復元できます。|
|$Paaaa|割り込みを有効にして、PレジスタとXレジスタを0に設定した状態で、aaaaアドレスからのプログラムを実行します。|
|?R|最後の割り込み後に保存されたCPUのレジスタの値を表示します。|
|$Rxp|8FB8-8FDFのRAM領域の情報をCPUレジスタに設定します。<br>その後コマンドパラメタのxをXレジスタに、pをPレジスタに設定してプログラムの実行を行います。<br>つまり、Pレジスタで指定されたレジスタの内容が実行開始アドレスとなります。|

?Rコマンドの表示例
```
*?R
8FB8 00B2 0000 0000 0000;
8FC0 8000 00BB 8FE2 014E 0202 0224 FA7B 6BAD;
8FD0 002C BFFF 8004 5D6D 00EF 0000 0201 8080
```
レジスタとの対応は以下の通りです。
```
8FB8 ID T  DF D  IE Q  EF1234
8FC0 R0 R1 R2 R3 R4 R5 R6 R7
8FD0 R8 R9 RA RB RC RD RE RF
```
このレジスタの内容を使って$Rコマンドでレジスタの状態を設定したあとに実行することができます。


### コマンドの使用例

エコーバックをするプログラムを書き込んで実行した操作例です。  
ターミナルからテキストをアップロードする時は適切な送信遅延を入れてください。（後述の制限事項の項目を参照）
```
IDIOT/4
*!M8000 F880B4B5F817A4F80BA5D5F801B3F83EA3D354F89CA3D3003007
*?M8000 20
8000 F880 B4B5 F817 A4F8 0BA5 D5F8 01B3 F83E;
8010 A3D3 54F8 9CA3 D300 3007 0000 0000 0000
*$P8000
```

他にも有用なコマンドがあるかもしれません。詳細はソースファイルまたはIDIOTモニタのサイトを参照してください。  
- [COSMAC 1802 "IDIOT" monitor Software](https://www.retrotechnology.com/memship/mship_idiot.html)

## ファームウェアのカスタマイズ方法

### メモリマップの変更
main.cの以下の部分で設定できます。現在はROMエリアは$0000-$0400、RAMエリアは$8000-8FFFとしています。

```
#define ROM_SIZE 0x0400 // ROM size 1k bytes
#define RAM_SIZE 0x1000 // RAM size 4k bytes
#define RAM_TOP 0x8000 // RAM start address
```

### CPUクロックの変更
main.cの以下の部分の値を変更します。V2.1Gでは1.79MHzになっています。これ以上速くするとタイミングが合わなくなり動かなくなります。

```
#define Z80_CLK   1790000UL // CDP1802 clock frequency(2MHz)
```

### ROM領域の変更
ファームウェアのROM領域を変更する場合は[SB-Assemblers](https://www.sbprojects.net/sbasm/)でアセンブルしてバイナリファイルを生成したあとに、ファームウェアのソースファイルに組み込むためのHEXデータに変換します。  
[SB-Assemblers](https://www.sbprojects.net/sbasm/)のインストールと使用方法は公式サイトでご確認ください。  
インストールがうまくできていれば、以下のようにソースファイル名を指定するだけでカレントディレクトリにバイナリファイルとリストファイルができます。  
```
sbasm idiot_0000.asm
```
生成されたバイナリファイルを16進データ形式に変換するPythonプログラム [tools/bin2hexsrc.py](tools/bin2hexsrc.py) を用意していますので、これを使うとソースに組み込むデータを簡単に作成できます。  
使い方は以下の通りです。
```
python3 bin2hexsrc.py idiot_0000.bin > idiot_0000.hexsrc
```
生成された16進データを以下のrom配列の部分に組み込んでください。  
組み込むデータがROM_SIZEを超えてしまう場合はメモリマップを調整してください。
```
const unsigned char rom[ROM_SIZE] = {
   :
}
```

## 制限事項

EMU1802-miniはCPU以外の周辺デバイスをPICに置き換えています。このため以下の制限事項があります。
- 通常のRAMにくらべてPICからの読み込みは遅く、CPUクロックに上限があります。現在は1.79MHzが上限です。
- RESETを押すとRAM領域が初期化されます。通常のRAMですと電源が入っていればメモリの状態が保持されますが、PICがリセットされるのでRAMエリアは初期化されます。このため後述するIDIOTモニタの割り込みを活用してください。
- CPUでの処理速度がついていけず、ターミナルからの入力で文字を取りこぼす場合があります。特にテキストをアップロードする時は適切な送信遅延を入れてください。目安として20ミリ秒/字、500ミリ秒/行程度は必要です。

## EMU1802-miniでのCOSMAC CPUの楽しみ方

### BASICを動かす方法
Lee A. Hart.さんの[The 1802 Membership Card](http://www.sunrise-ev.com/1802.htm)用のMCBASIC3が動きます。  

[The 1802 Membership Card](http://www.sunrise-ev.com/1802.htm)のページにある[MCBASIC3.bin](http://www.sunrise-ev.com/MembershipCard/MCBASIC3.bin)を使用してください。バイナリのサイズは16Kbyteですので、ソースのROMサイズを以下のように変更してください。  
```
#define ROM_SIZE 0x4000 // ROM size 16k bytes
```
あとは、ダウンロードしたMCBASIC3.binをbin2hexsrc.pyでソースに組み込むデータを作成して、ROM領域に組み込んでください。  
これはソフトウェアシリアルを使用しますので、USBシリアルをCON3に接続してください。シリアル速度の設定は以下の通りです。  
- Speed 600bps
- Data 8bit
- Parity none
- Stop 1bit

正常に起動すると以下のように表示されます。
```
WELCOME TO THE 1802 BASIC3 V1.1
(C)1981 RCA
C/W?
```
C/W?とありますが、CはCOLD START、WはWARM START(*1)です。  
Cを入力するとREADYと表示され、入力待ちを表すコロン(:)が表示されます。この状態でBASICプログラムが入力できます。  
詳しい使い方はThe 1802 Membership CardのサイトにBASIC3 User Manualがありますのでそちらを参照ください。  
なお、少し速くタイピングしたり、ターミナルプログラムからファイルをアップロードしようとすると入力文字を取りこぼしますので適度な送信遅延をいれてください。  

*1 EMU1802-miniではリセット後にRAM領域がクリアされてしまうため、WARM STARTは利用できません。常にCOLD STARTで起動してください。

### PICのUARTにアクセスする方法

$E000, $E001がPICのUARTレジスタのアドレスになります。使用例は[サンプルプログラム](/test_programs/uart_echo/)で確認してください。

### ファームウェアV2.1Gについて

ファームウェアV2.1GはGazelle氏により大幅に高速化が図られたものです。ソースコードの開発および公開に感謝いたします。  
V1.x系とは処理方式が大きく変更されています。これまではTPBの立ち上がりでメモリーを読んでいましたが、V2.1Gでは割り込みを使わずにポーリングを使ってTPAの立下り直後のCLOCK立ち上がりで
アドレスを確定しています。さらにコンパイラの最適化レベルを2にしています。  
これによりCPUクロックの高速化が実現し、ソフトウェアシリアルでも9600bpsに対応でききるようになりました。ソフトウェアシリアルを使用している膨大な1802の資産が動かせそうです。  
V2.1Gに標準で組み込まれているIDIOTモニタはPIC内蔵のUART対応版ですが、ROM領域の#ifdef ～　#endif の場所を変更することでソフトウェアシリアル版のIDIOTモニタに切り替えることができます。  
暫定的なコーディングとなっていますが、これまでテストに使ったプログラムも残っているので参考にしてください。

### IDIOTモニタの割り込みを活用する

IDIOTモニタではハードウェア割り込みとソフトウェア割り込みをサポートしています。
- ハードウェア割り込み  
CPUのINTERRUPTピンをLOWにすると、IDIOTモニタの割り込みハンドラに制御が渡りTレジスタを除く全レジスタがRAM領域に保存され、IDIOTのコマンド入力待ちになります。
- ソフトウェア割り込み  
SEP R1命令（バイナリではD1）を実行すると、ハードウェア割り込みと同様の動きをします。PレジスタとXレジスタを除く全てのレジスタがRAM領域に保存され、IDIOTのコマンド入力待ちになります。
- レジスタ情報の確認  
割り込み発生後にIDIOTモニタで?Rコマンドを入力してRAM領域に保存されたレジスタの状態を確認できます。
- 割り込みで中断したプログラムの再開  
$Rコマンドを入力することで、RAM領域に保存されている情報がレジスタに書き戻され、中断したプログラムを再開することもできます。再開前にRAM領域を書き換えることで各レジスタを任意の値に設定してプログラムを再開することもできます。  

このようにプログラムのデバックやCOSMAC CPUの学習に活用できます。

## 参考ブログ

https://kanpapa.com/cosmac/blog/emu1802/
