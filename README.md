[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](/LICENSE)

# Segs2Polys

---

## 初めに

このリポジトリにはGPL v2.0ライセンスを適用しています。

詳しくはライセンスファイルなどを参照してください。

---

## 基本的な説明

2d空間上で複数の線分から最小のポリゴンを生成するプログラムです。

おまけ機能としてear clippingなどの機能や円弧関係の機能も付いています。

>リファレンスは[here](/doc/dllのエクスポートされた関数の使い方.md)
>
>アルゴリズムの考え方については[here](/doc/線分からポリゴンを生成するときの手順.md)

---

## コンパイル方法

__gccやcmakeなどは事前にインストールされている前提です。__

dllをコンパイル
```bash
make dll
```

テスト用のexeをコンパイル
```bash
make debug
```
