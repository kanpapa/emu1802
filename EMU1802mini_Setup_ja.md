# EMU1802-miniのセットアップ

## ハードウェアの製作

1. 回路図とBOMを参考にしてパーツを入手します。
1. 専用基板があれば基板に実装してハンダ付けします。基板がない場合はブレッドボードを使って配線しても良いでしょう。

## 開発環境の準備
- [Microchip MPLAB IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)が必要です。  
- ファームウェアのROM領域を変更する場合は[SB-Assemblers](https://www.sbprojects.net/sbasm/)を使用します。Python3が動作する環境が必要です。私の場合はWSL2環境にインストールしています。  

## ファームウェアの書き込み手順
1. emu1802mini.Xを適当な場所に展開します。
1. [Microchip MPLAB IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)を起動します。
1. プロジェクトを開くで上記フォルダを指定して開きます。
1. ビルドを行いコンパイルができることを確認します。
1. EMU1802-miniにSNAPを接続し、PCに接続します。
1. EMU1802-miniの電源を投入します。
1. ターゲットのPICに書き込みを行います。

## EMU1802-miniの動作確認
1. USBシリアルをCON3に接続します。（CON2はファームウェアがまだ未サポートです。）
1. EMU1802-miniの電源を投入します。
1. USBシリアルにPCを接続し、ターミナルプログラム(TeraTermなど）で300bps、スタート1ビット、パリティMark、ストップ1ビットに設定します。
1. Enterキーを押すと、IDIOT/4という起動メッセージが表示されます。

## IDIOTモニタの使い方
IDIOTモニタのサイトを参照してください。  
- [COSMAC 1802 "IDIOT" monitor Software](https://www.retrotechnology.com/memship/mship_idiot.html)


## ファームウェアのカスタマイズ方法

### メモリマップの変更
main.cの86〜88行で設定できます。現在はROMエリアは$0000-$0400、RAMエリアは$8000-8FFFとしています。

```
#define ROM_SIZE 0x0400 // ROM size 1k bytes
#define RAM_SIZE 0x1000 // RAM size 4k bytes
#define RAM_TOP 0x8000 // RAM start address
```

### CPUクロックの変更
main.cの77行目の値を変更します。現在は0.2MHzになっています。あまり大きくすると

```
#define Z80_CLK   200000UL // CDP1802 clock frequency(0.2MHz)
```

### ROM領域の変更
ファームウェアのROM領域を変更する場合は[SB-Assemblers](https://www.sbprojects.net/sbasm/)でアセンブルしてバイナリファイルを生成したあとに、ファームウェアのソースファイルに組み込むためのHEXデータに変換します。
[SB-Assemblers](https://www.sbprojects.net/sbasm/)のインストールと使用方法は公式サイトでご確認ください。
インストールがうまくできていれば、以下のようにソースファイル名を指定するだけでカレントディレクトリにバイナリファイルとリストファイルができます。
```
sbasm idiot_0000.asm
```
バイナリファイルを16進データ形式に変換するPythonプログラム tools/bin2hexsrc.py を用意していますので、これを使うと便利です。使い方は以下の通りです。
```
bin2hexsrc.py idiot_0000.bin > idiot_0000.hexsrc
```
生成された16進データを405行目ののrom配列に組み込んでください。
```
const unsigned char rom[ROM_SIZE] = {
   :
}
```
