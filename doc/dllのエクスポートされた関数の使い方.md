# dllのエクスポートされた関数の使い方

---

### 初めに

スタティックな内部バッファを2つ持っていて基本はバッファのインデックスを指定して任意の操作を行います。

内部バッファには2系統存在します。

- 線分バッファ

```cpp
  static std::vector<Line2> gSegs = std::vector<Line2>();
```
- ポリゴンバッファ

```cpp
  static std::vector<std::vector<Vec2>> gPolys = std::vector<std::vector<Vec2>>();
```

関数関係の説明では線分バッファは __"segs"__ ポリゴンバッファは __"polys"__ と省略しています。

---

### バッファクリア

<details><summary>Clear</summary>

```cpp
INT Clear()
```
> segsとpolysをクリアします
>
> 返り値には必ず0が返ります
</details>

<details><summary>ClearSegs</summary>

```cpp
INT ClearSegs()
```
> segsのみクリアします
>
> 返り値には必ず0が返ります
</details>

<details><summary>ClearPolys</summary>

```cpp
INT ClearPolys()
```
> polysのみクリアします
>
> 返り値には必ず0が返ります
</details>

---

### 線分の追加、取得、変換

<details><summary>AddSeg</summary>

```cpp
INT AddSeg(DOUBLE x1, DOUBLE y1, LONG id1, DOUBLE x2, DOUBLE y2, LONG id2)
```
> 線分の追加を行います
</details>

<details><summary>AddSegArc</summary>

```cpp
INT AddSegArc(DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE cx, DOUBLE cy, LONG id, DOUBLE r)
```
> 円弧データを線分に分解して線分として登録します
>
> rは分割する角度を入れてください
>
> エラーの場合-1が帰ります
</details>

<details><summary>GetSegCnt</summary>

```cpp
LONG GetSegCnt()
```
> segsの総数を返します
>
> 返り値は常に0が返ります
</details>

<details><summary>GetSeg</summary>

```cpp
INT GetSeg(LONG sn, DOUBLE* x1, DOUBLE* y1, LONG* id1, DOUBLE* x2, DOUBLE* y2, LONG* id2)
```
> snで指定されたインデックスのsegsの情報を返します
>
> 始点：x1,y1,id1
>
> 終点：x2,y2,id2
>
> エラーの場合-1が帰ります
</details>

<details><summary>GetSegS</summary>

```cpp
INT GetSegS(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
```
> snで指定されたインデックスのsegsの始点情報を返します
>
> エラーの場合-1が帰ります
</details>

<details><summary>GetSegE</summary>

```cpp
INT GetSegE(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
```
> snで指定されたインデックスのsegsの終点情報を返します
>
> エラーの場合-1が帰ります
</details>

<details><summary>GetSegArc</summary>

```cpp
INT GetSegArc(LONG sn, DOUBLE r, DOUBLE rmargin, LONG minchain, LONG* en, DOUBLE* x1, DOUBLE* y1, DOUBLE* x2, DOUBLE* y2, DOUBLE* cx, DOUBLE* cy, LONG* id)
```
> snで指定されたインデックスのsegsが円弧かどうかの判定と円弧だった場合に円弧データを返します
>
> r：分割時の角度
>
> rmargin：rの誤差(+-)
>
> minchain：対象の線分の最小連結数
>
> en：円弧の終点の次のインデックス
>
> エラーの場合-1が帰ります
</details>

<details><summary>CnvSegs2Poly</summary>

```cpp
INT CnvSegs2Poly()
```
> segsのデータをpolysの末尾に追加します
>
> この時末尾に追加されるのは始点のみです
>
> エラーの場合-1が帰ります
</details>

---

### ポリゴンの追加、取得、変換

