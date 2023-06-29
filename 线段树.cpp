### 线段树

#### 问题1 区间询问

输入一个长度为 n 的整数序列。

接下来再输入 m 个询问，每个询问输入一对 $$[l,r]$$。

对于每个询问，输出原序列中从第 l 个数到第 r 个数的和。

预处理前缀和

```c++
int sum[N];

for (int i = 1; i <= n; i ++ )
{
    sum[i] = sum[i - 1] + a[i];
}

while (m -- )
{
    int l, r;
    cin >> l >> r;
    cout << s[r] - s[l - 1] << endl;
}
```





#### 问题2 区间修改

输入一个长度为 n 的整数序列。

接下来输入 m 个操作，每个操作包含三个整数 l, r, c，表示将序列中$$[l,r]$$ 之间的每个数加上 c。

请你输出进行完所有操作后的序列。

```c++
int b[N];
for (int i = 1; i <= n; i ++ )
{
    b[i] = a[i] - a[i - 1];
}

while (m -- )
{
    int l, r;
    cin >> l >> r;
    b[l] += c;
    b[r + 1] -= c;
}
```





#### 问题3  单点修改 区间查询

输入一个长度为 n 的整数序列。

接下来输入 m 个操作，操作分为两种：

C x v表示将序列中下标为x的数加上v。

Q l r表示询问序列中$$[l,r]$$ 的和。

期望复杂度：$$O(logn)$$



#### 问题4 区间修改 区间查询

输入一个长度为 n 的整数序列。

接下来输入 m 个操作，操作分为两种：

C l r d表示将序列中$$[l,r]$$的数加上d。

Q l r表示询问序列中$$[l,r]$$ 的和。

期望复杂度：$$O(logn)$$





#### 线段树的定义

线段树是一种[二叉搜索树](https://baike.baidu.com/item/二叉搜索树)，与[区间树](https://baike.baidu.com/item/区间树)相似，它将一个区间划分成一些单元区间，每个单元区间对应线段树中的一个叶结点。 [1] 

对于线段树中的每一个非[叶子节点](https://baike.baidu.com/item/叶子节点)[a,b]，它的左儿子表示的区间为[a,(a+b)/2]，右儿子表示的区间为[(a+b)/2+1,b]。因此线段树是[平衡二叉树](https://baike.baidu.com/item/平衡二叉树)，最后的子节点数目为N，即整个线段区间的长度。

使用线段树可以快速的查找某一个节点在若干条线段中出现的次数，[时间复杂度](https://baike.baidu.com/item/时间复杂度)为O(logN）。

![09MX5DLMOBN9ZOHXUG}LCFD](09MX5DLMOBN9ZOHXUG}LCFD.png)

#### 线段树的特点

- 一颗二叉树
- 树上的每个结点都对应一个区间$$[l,r]$$
- 同一层结点所代表的区间，不会重叠，并且加起来等于区间总长度
- 叶子结点的区间是单位长度$$[x,x]$$
- 对于每个内部结点$$[l,r]$$，他的左子结点是$$[l,mid]$$右子结点是$$[mid+1,r]，mid = l + r >> 1$$。
- 叶子结点的个数是N
- 存储形式是数组，假设当前结点编号为u，左儿子就是u * 2(u << 1)，右儿子就是u * 2 + 1(u << 1 | 1)。

#### 线段树支持的操作

1. $$pushup()$$：由子节点信息来更新父节点信息
2. $pushdown()$：由父节点信息来更新子节点信息
3. $$build()$$：将一段区间初始化为线段树
4. $$modify()$$：修改操作

   - 单点修改，区间查询  (easy)

   - 区间修改，区间查询  (hard)
5. $$query()$$：查询操作，$$logn$$的复杂度查询$$[l,r]$$区间。

上述四个操作的模板：

```c++
struct Node
{
	int l, r;
    int add; //懒标记
    int sum;
    int v;
}tr[N * 4];

void pushup(int u)
{
	tr[u].sum = tr[u << 1].sum + tr[u << 1 | 1].sum;
}

void pushdown(int u)
{
    Node &root = tr[u], &left = tr[u << 1], &right = tr[u << 1 | 1];
    if (root.add)
    {
        left.add += root.add, left.sum += (LL)left.add * (left.r - left.l + 1);
        right.add += root.add, right.sum += (LL)right.add * (right.r - right.l + 1);
        root.add = 0;
    }
}

void build(int u, int l, int r)
{
	if (l == r) tr[u] = {l, r, w[l]};
	else
	{
        tr[u] = {l, r};
		int mid = l + r >> 1;
		build(u << 1, l, mid);
		build(u << 1 | 1, mid + 1, r);
		pushup(u);
	}
}

void modify(int u, int x, int v) //单点修改
{
	if (tr[u].l == tr[u].r) tr[u].sum += v;
	else
	{
		int mid = tr[u].l + tr[u].r >> 1;
		if (x <= mid) modify(u << 1, x, v);
		else modify(u << 1 | 1, x, v);
		pushup(u);
	}
}

void modify(int u, int l, int r, int d) //区间修改
{
	if (tr[u].l >= l && tr[u].r <= r)
	{
		tr[u].sum += (tr[u].r - tr[u].l + 1) * d;
        tr[u].add += d;
	}
	else
	{
        pushdown(u);
		int mid = tr[u].l + tr[u].r >> 1;
		if (l <= mid) modify(u << 1, l, r, d);
		if (r > mid) modify(u << 1 | 1, l, r, d);
		pushup(u);
	}
}


LL query(int u, int l, int r)
{
	if (tr[u].l >= l && tr[u].r <= r)
	{
		return tr[u].sum;
	}
    pushdown(u);
	int mid = tr[u].l + tr[u].r >> 1;
	LL res = 0;
	if (l <= mid) res = query(u << 1, l, r);
	if (r > mid) res += query(u << 1 | 1, l, r);
	return res;
}
```



#### 例题

[线段树维护区间和](https://www.acwing.com/problem/content/description/1266/)

[线段树维护区间最大值](https://www.luogu.com.cn/problem/P1198)

[区间修改 区间查询](https://www.acwing.com/problem/content/244/)







