[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://github.com/sr-kentiku/Segs2Polys/blob/main/LICENSE)

# Segs2Polys

---

## 初めに

このリポジトリにはGPL v2.0ライセンスを適用しています。

詳しくはライセンスファイルなどを参照してください。

---

## 基本的な説明

2d空間上で複数の線分から最小のポリゴンを生成するプログラムです。

おまけ機能としてear clippingなどの機能や円弧関係の機能も付いています。

---

## コンパイル方法

__gccやcmakeなどは事前にインストールされている前提です。__

dllをコンパイル
```bash
make dll
```

テスト用のexeをコンパイル
```bash
make test
```
