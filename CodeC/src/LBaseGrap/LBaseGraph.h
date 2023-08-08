/*
注: 使用顶点必须定义索引,如句柄Handle
*/

#ifndef LBaseGraph_H__20230601_lrl
#define LBaseGraph_H__20230601_lrl

#include "LBaseGraph_Global.h"

//图基类
template <class T1, class T2>
struct Vertex;
template <class T>
struct ArcSide;

template <class T1, class T2>
struct Vertex
{
    Vertex()
        : index(-1)
        , tail(nullptr)
        , head(nullptr)
    {
    }
    //顶点序号
    int index;
    //顶点数据
    T1 dataInfo;
    //指向以该顶点为弧尾的第一条弧

    ArcSide<T2> * tail;
    //指向以该顶点为弧头的第一条弧
    ArcSide<T2> * head;
};

template <class T>
struct ArcSide
{
    ArcSide()
        : tail_index(-1)
        , head_index(-1)
        , next_tail(nullptr)
        , next_head(nullptr)
    {
    }
    //弧尾顶点序号
    int tail_index;
    //弧头顶点序号
    int head_index;
    //指向弧尾相同的下一条弧
    ArcSide * next_tail;
    //指向弧头相同的下一条弧
    ArcSide * next_head;
    //弧数据
    T dataInfo;
};

class LBaseGraph
{
public:
    //创建十字链表
    template <class T1, class T2>
    std::vector<Vertex<T1, T2>> create_cross_list();
    //删除十字链表
    template <class T1, class T2>
    void delete_cross_list(std::vector<Vertex<T1, T2>> & vertices);
    //顶点操作
    //添加操作
    template <class T1, class T2>
    void add_vertex(std::vector<Vertex<T1, T2>> & vertices, int index, T1 data);
    //删除顶点
    template <class T1, class T2>
    void delete_vertex(std::vector<Vertex<T1, T2>> & vertices, int index);
    //返回顶点索引的数组下标
    template <class T1, class T2>
    int GetIndexNumber(std::vector<Vertex<T1, T2>> & vertices, int index);
    //修改顶点数据
    template <class T1, class T2>
    void modify_vertex(std::vector<Vertex<T1, T2>> & vertices, int index, T1 new_data);
    //查询顶点
    template <class T1, class T2>
    Vertex<T1, T2> query_vertex(std::vector<Vertex<T1, T2>> & vertices, int index);
    //弧操作
    //添加弧
    template <class T1, class T2>
    void add_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index, T2 new_data);
    //删除弧
    template <class T1, class T2>
    void delete_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index);
    //修改弧数据
    template <class T1, class T2>
    bool modify_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index, T2 new_data);
    //查询弧
    template <class T1, class T2>
    ArcSide<T2> query_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index);
    //获取所有弧信息
    template <class T1, class T2>
    std::vector<ArcSide<T2>> query_all_arc(std::vector<Vertex<T1, T2>> & vertices);
    //判断弧是否存在
    template <class T1, class T2>
    bool exist_arc(std::vector<Vertex<T1, T2>> & vertices, int head_index, int tail_index);
};

typedef std::shared_ptr<LBaseGraph> LBaseGraphPtr;

template <class T1, class T2>
std::vector<Vertex<T1, T2>> LBaseGraph::create_cross_list()
{
    std::vector<Vertex<T1, T2>> vec;
    return vec;
}

//删除十字链表
template <class T1, class T2>
void LBaseGraph::delete_cross_list(std::vector<Vertex<T1, T2>> & vertices)
{
    int len = vertices.size();
    for (int i = len - 1; i >= 0; i--)
    {
        delete_vertex<T1, T2>(vertices, i);
    }
    vertices.clear();
}

//顶点操作
//添加操作
template <class T1, class T2>
void LBaseGraph::add_vertex(std::vector<Vertex<T1, T2>> & vertices, int index, T1 dataInfo)
{
    Vertex<T1, T2> new_vertex;
    new_vertex.index    = index;
    new_vertex.dataInfo = dataInfo;
    new_vertex.tail     = nullptr;
    new_vertex.head     = nullptr;

    vertices.push_back(new_vertex);
}

//删除顶点
template <class T1, class T2>
void LBaseGraph::delete_vertex(std::vector<Vertex<T1, T2>> & vertices, int index)
{
    //删除以该顶点为弧头或者弧尾的所有弧
    int IndexNumber = GetIndexNumber(vertices, index);
    if (IndexNumber < 0)
    {
        return;
    }
    ArcSide<T2> * arc = vertices[IndexNumber].tail;
    while (arc != nullptr)
    {
        ArcSide<T2> * next = arc->next_tail;

        delete_arc(vertices, arc->tail_index, arc->head_index);
        arc = next;
    }
    arc = vertices[IndexNumber].head;
    while (arc != nullptr)
    {
        ArcSide<T2> * next = arc->next_head;
        delete_arc(vertices, arc->tail_index, arc->head_index);
        arc = next;
    }
    vertices.erase(vertices.begin() + IndexNumber);
}

//返回顶点索引的数组下标
template <class T1, class T2>
int LBaseGraph::GetIndexNumber(std::vector<Vertex<T1, T2>> & vertices, int index)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].index == index)
        {
            return i;
        }
    }
    return -1;
}

//修改顶点数据
template <class T1, class T2>
void LBaseGraph::modify_vertex(std::vector<Vertex<T1, T2>> & vertices, int index, T1 new_dataInfo)
{
    int number                = GetIndexNumber(vertices, index);
    vertices[number].dataInfo = new_dataInfo;
}
//查询顶点
template <class T1, class T2>
Vertex<T1, T2> LBaseGraph::query_vertex(std::vector<Vertex<T1, T2>> & vertices, int index)
{
    int number = GetIndexNumber(vertices, index);
    return vertices[number];
}

//弧操作
//添加弧
template <class T1, class T2>
void LBaseGraph::add_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index, T2 new_dataInfo)
{
    //如果有,则修改数据
    if (exist_arc(vertices, tail_index, head_index))
    {
        modify_arc(vertices, tail_index, head_index, new_dataInfo);
        return;
    }
    ArcSide<T2> * arc              = new ArcSide<T2>();
    arc->tail_index            = tail_index;
    arc->head_index            = head_index;
    arc->dataInfo              = new_dataInfo;
    int number_tail            = GetIndexNumber(vertices, tail_index);
    arc->next_tail             = vertices[number_tail].tail;
    vertices[number_tail].tail = arc;
    int number_head            = GetIndexNumber(vertices, head_index);
    arc->next_head             = vertices[number_head].head;
    vertices[number_head].head = arc;
}

//删除弧
template <class T1, class T2>
void LBaseGraph::delete_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index)
{
    if (!exist_arc(vertices, tail_index, head_index))
    {
        return;
    }
    //删除入弧
    tail_index    = GetIndexNumber(vertices, tail_index);
    head_index    = GetIndexNumber(vertices, head_index);
    ArcSide<T2> * pre = nullptr;
    ArcSide<T2> * arc = vertices[tail_index].tail;
    while (arc != nullptr && arc->head_index != vertices[head_index].index)
    {
        //记录上一条弧
        pre = arc;
        //移动到下一条弧
        arc = arc->next_tail;
    }
    if (arc != nullptr)
    {
        if (pre != nullptr)
        {
            //如果有上一条弧,就将它的nwxt指向被删除弧的下一条弧
            pre->next_tail = arc->next_tail;
        }
        else
        {
            //如果这是第一条弧,就更新头指针
            vertices[tail_index].tail = arc->next_tail;
        }
    }
    //删除出弧
    pre = nullptr;
    arc = vertices[head_index].head;
    while (arc != nullptr && arc->tail_index != vertices[tail_index].index)
    {
        //记录上一条弧
        pre = arc;
        //移动到下一条弧
        arc = arc->next_head;
    }
    if (arc != nullptr)
    {
        if (pre != nullptr)
        {
            //如果有上一条弧,就将它的nwxt指向被删除弧的下一条弧
            pre->next_head = arc->next_head;
        }
        else
        {
            //如果这是第一条弧,就更新头指针
            vertices[head_index].head = arc->next_head;
        }
    }
    //删除这一条要删除的弧
    delete arc;
}

//修改弧数据
template <class T1, class T2>
bool LBaseGraph::modify_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index, T2 new_dataInfo)
{
    tail_index    = GetIndexNumber(vertices, tail_index);
    head_index    = GetIndexNumber(vertices, head_index);
    ArcSide<T2> * arc = vertices[tail_index].tail;
    while (arc != nullptr && arc->head_index != head_index)
    {
        arc = arc->next_tail;
    }
    if (arc != nullptr)
    {
        arc->dataInfo = new_dataInfo;
        return true;
    }
    else
    {
        return false;
    }
    return false;
}
//查询弧
template <class T1, class T2>
ArcSide<T2> LBaseGraph::query_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index)
{
    tail_index    = GetIndexNumber(vertices, tail_index);
    head_index    = GetIndexNumber(vertices, head_index);
    ArcSide<T2> * arc = vertices[tail_index].tail;
    while (arc != nullptr && arc->head_index != head_index)
    {
        arc = arc->next_tail;
    }
    if (arc)
    {
        return *arc;
    }
    else
    {
        ArcSide<T2> arc2;
        return arc2;
    }
}

//获取所有弧,只需要遍历所有节点的出弧或者入弧即可
template <class T1, class T2>
std::vector<ArcSide<T2>> LBaseGraph::query_all_arc(std::vector<Vertex<T1, T2>> & vertices)
{
    std::vector<ArcSide<T2>> arcs;
    for (auto vecs : vertices)
    {
        ArcSide<T2> * arc = vecs.tail;
        while (arc != nullptr)
        {
            arcs.push_back(*arc);
            arc = arc->next_tail;
        }
    }
    return arcs;
}

//判断弧是否存在
template <class T1, class T2>
bool LBaseGraph::exist_arc(std::vector<Vertex<T1, T2>> & vertices, int tail_index, int head_index)
{
    tail_index    = GetIndexNumber(vertices, tail_index);
    head_index    = GetIndexNumber(vertices, head_index);
    ArcSide<T2> * arc = vertices[tail_index].tail;
    while (arc != nullptr && arc->head_index != head_index)
    {
        arc = arc->next_tail;
    }
    if (arc)
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // LBaseGraph_H__20230601_lrl
