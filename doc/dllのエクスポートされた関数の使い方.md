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
> 線分を追加します
>
> x1, y1, id1：始点
>
> x2, y2, id2：終点
>
> 返り値には必ず0が返ります
</details>

<details><summary>AddSegArc</summary>

```cpp
INT AddSegArc(DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE cx, DOUBLE cy, LONG id, DOUBLE r)
```
> 円弧データを線分に分解して線分として追加します
>
> x1, y1：始点
>
> x2, y2：終点
>
> cx, cy：円弧の中点
>
> id：円弧から線分に変換した時に線分の各頂点につけられる
>
> r：分割する角度
>
> エラーの場合-1を返します
</details>

<details><summary>GetSegCnt</summary>

```cpp
LONG GetSegCnt()
```
> segsの総数を返します
</details>

<details><summary>GetSeg</summary>

```cpp
INT GetSeg(LONG sn, DOUBLE* x1, DOUBLE* y1, LONG* id1, DOUBLE* x2, DOUBLE* y2, LONG* id2)
```
> 指定のsegsのインデックスのデータを取得
>
> sn：segsのインデックス
>
> x1, y1, id1：始点
>
> x2, y2, id2：終点
>
> エラーの場合-1を返します
</details>

<details><summary>GetSegS</summary>

```cpp
INT GetSegS(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
```
> 指定のsegsのインデックスの始点を取得
>
> sn：segsのインデックス
>
> x, y, id：始点
>
> エラーの場合-1を返します
</details>

<details><summary>GetSegE</summary>

```cpp
INT GetSegE(LONG sn, DOUBLE* x, DOUBLE* y, LONG* id)
```
> 指定のsegsのインデックスの終点を取得
>
> sn：segsのインデックス
>
> x, y, id：終点
>
> エラーの場合-1を返します
</details>

<details><summary>GetSegArc</summary>

```cpp
INT GetSegArc(LONG sn, DOUBLE r, DOUBLE rmargin, LONG minchain, LONG* en, DOUBLE* x1, DOUBLE* y1, DOUBLE* x2, DOUBLE* y2, DOUBLE* cx, DOUBLE* cy, LONG* id)
```
> 指定のsegsインデックスのデータが円弧かどうか、円弧だった場合は円弧データを返します
>
> sn：segsのインデックス
>
> r：分割時の角度
>
> rmargin：(+-)rの誤差
>
> minchain：対象の線分の最小連結数
>
> en：円弧の終点の次のインデックス
>
> x1, y1：始点
>
> x2, y2：終点
>
> cx, cy：円弧の中心
>
> id：線分の中心の始点id
>
> エラーの場合-1を返します
</details>

<details><summary>CnvSegs2Poly</summary>

```cpp
INT CnvSegs2Poly()
```
> segsのデータをpolysの末尾に追加します
>
> この時末尾に追加されるのは始点のみです
>
> エラーの場合-1を返します
</details>

---

### ポリゴンの追加、取得、変換

<details><summary>AddPoly</summary>

```cpp
INT AddPoly()
```
> polysのポリゴンを追加します
>
> エラーの場合-1を返します
</details>

<details><summary>AddPolyVec</summary>

```cpp
INT AddPolyVec(LONG pn, DOUBLE x, DOUBLE y, LONG id)
```
> 指定のポリゴンに頂点を追加します
>
> pn：ポリゴンインデックス
>
> x, y, id：頂点データ
>
> エラーの場合-1を返します
</details>

<details><summary>AddPolyArc</summary>

```cpp
INT AddPolyArc(LONG pn, DOUBLE x1, DOUBLE y1, DOUBLE x2, DOUBLE y2, DOUBLE cx, DOUBLE cy, LONG id, DOUBLE r)
```
> 円弧データを頂点分解して追加します
>
> pn：ポリゴンインデックス
>
> x1, y1：始点
>
> x2, y2：終点
>
> cx, cy：円弧の中点
>
> id：円弧から頂点に変換した時に各頂点に付けられる
>
> r：分割する角度
>
> エラーの場合-1を返します
</details>

<details><summary>GetPolyCnt</summary>

```cpp
LONG GetPolyCnt()
```
> polysの総数を返します
</details>

<details><summary>GetPolyVecCnt</summary>

```cpp
LONG GetPolyVecCnt(LONG pn)
```
> 指定されたポリゴンインデックスの頂点数を返します
>
> pn：ポリゴンインデックス
>
> エラーの場合0を返します
</details>

<details><summary>GetPolyVec</summary>

```cpp
LONG GetPolyVec(LONG pn, LONG vn, DOUBLE* x, DOUBLE* y, LONG* id)
```
> 指定されたポリゴンインデックスの指定された頂点インデックスの頂点データを返します
>
> pn：ポリゴンインデックス
>
> vn：頂点インデックス
>
> x, y, id：頂点データ
>
> エラーの場合-1を返します
</details>

<details><summary>GetPolyArc</summary>

```cpp
INT GetPolyArc(LONG pn, LONG vn, DOUBLE r, DOUBLE rmargin, LONG minchain, LONG* en, DOUBLE* x1, DOUBLE* y1, DOUBLE* x2, DOUBLE* y2, DOUBLE* cx, DOUBLE* cy, LONG* id)
```
> 指定されたポリゴンインデックスの指定された頂点インデックスの頂点データが円弧かどうか、円弧だった場合は円弧データを返します
>
> pn：ポリゴンインデックス
>
> vn：頂点インデックス
>
> r：分割時の角度
>
> rmargin：(+-)rの誤差
>
> minchain：対象の線分の最小連結数
>
> en：円弧の終点の次のインデックス
>
> x1, y1：始点
>
> x2, y2：終点
>
> cx, cy：円弧の中心
>
> id：線分の中心の始点id
>
> エラーの場合-1を返します
</details>

<details><summary>CnvPoly2Segs</summary>

```cpp
INT CnvPoly2Segs(LONG pn)
```
> polysのデータをsegsの末尾に追加します
>
> エラーの場合-1を返します
</details>

---

### 各種演算

<details><summary>CalcExtendMarginIntersection</summary>

```cpp
INT CalcExtendMarginIntersection(DOUBLE dist)
```
> segsをマージンを持って交点計算を行います
>
> segsのデータを使用して処理後segsデータに返します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcSplitLines</summary>

```cpp
INT CalcSplitLines()
```
> segsの線分同士を交点で分解します
>
> segsのデータを使用して処理後segsデータに返します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcSegs2Polys</summary>

```cpp
INT CalcSegs2Polys()
```
> segsから最小のポリゴンを見つけpolysに追加します
>
> segsのデータを使用して処理後polysデータを生成します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcMergePolysLines</summary>

```cpp
INT CalcMergePolysLines()
```
> polysの頂点に交点などがなく頂点を削除しても問題ない頂点を削除します
>
> polysのデータを使用して処理後polysデータに返します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcEarClip</summary>

```cpp
LONG CalcEarClip(LONG pn, LONG safety)
```
> 任意のpolysを三角形に変換してpolysの末尾に追加します
>
> pn：ポリゴンインデックス
>
> safety：無限ループ防止用
>
> polysに追加された図形数を返します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcEarClipHoles</summary>

```cpp
LONG CalcEarClipHoles(LONG pn, LONG hpn, LONG* hp, LONG safety)
```
> 任意のpolysと任意の穴用polysを三角形に変換してpolysの末尾に追加します
>
> pn：ポリゴンインデックス
>
> hpn：穴の部分のポリゴンインデックスの数
>
> hp：穴の部分のポリゴンインデックス
>
> safety：無限ループ防止用
>
> polysに追加された図形数を返します
>
> エラーの場合-1を返します
</details>

<details><summary>CalcDirectionRotatePoly</summary>

```cpp
INT CalcDirectionRotatePoly(LONG pn, INT r)
```
> 指定のpolysを指定の回転方向に変更します
>
> pn：ポリゴンインデックス
>
> r：0=右回り 1=左回り
>
> エラーの場合-1を返します
</details>

<details><summary>CalcDirectionRotatePolys</summary>

```cpp
INT CalcDirectionRotatePolys(INT r)
```
> polysを指定の回転方向に変更します
>
> r：0=右回り 1=左回り
>
> エラーの場合-1を返します
</details>
