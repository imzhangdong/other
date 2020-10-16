#include<bits/stdc++.h>
using namespace std;
namespace Solution{
    void translate(stack<int>& s){
        const char * name[] = {"农夫","菜","羊","狼"};
        int curStatus= s.top(),nextStatus,pepStatus;
        s.pop();
        cout << "左岸: ";
        for(int i=0;i<4;++i){
            if(((curStatus&(i<<i))>>i)==0){
                cout << name[i]<<" ";
            }
        }
        cout << endl;
        while(!s.empty()){
            nextStatus = s.top();
            s.pop();
            pepStatus = nextStatus&1;
            cout << (pepStatus==1?"过岸: ":"回岸: ");
            for(int i=0;i<4;++i){
                if((((curStatus^nextStatus)>>i)&1)==1){
                    cout << name[i] <<" ";
                }
            }
            cout << endl;
            curStatus = nextStatus;
            cout << (pepStatus==0?"左岸: ":"右岸: ");
            for(int i=0;i<4;++i){
                if(((curStatus>>i)&1)==pepStatus){
                    cout << name[i] <<" ";
                }
            }
            cout << endl;     
        }
    }
    bool findPath(){
        /*
        狼 羊 菜 农夫 过河
        狼 羊 不能单独在同一侧
        羊 菜 不能单独在同一侧
        将这四个作为一个元祖视为一种状态，通过当前状态转换为另一种状态(合法状态)
        用4bit表示一种状态s表示，最低位为农夫时，通过最低位判断农夫的状态
        s&1 == 0时，农夫需要过河只能带一个东西或者空手，即s的高三位有一个需要翻转为1，农夫的状态位也翻转为1
            s | 0001 => s | 0001 | 1 => s | (1 << 0) | 1
            s | 0011 => s | 0010 | 1 => s | (1 << 1) | 1
            s | 0101 => s | 0100 | 1 => s | (1 << 2) | 1
            s | 1001 => s | 1000 | 1 => s | (1 << 3) | 1
        s&1 == 1时,情况与上面相反,高三位有一位需要翻转为0,农夫的状态翻转为0
            s & 1110 => s & 1110 & 1110 => s & (~0001) & 1110 => s & (~(1 << 0)) & 1110
            s & 1100 => s & 1101 & 1110 => s & (~0010) & 1110 => s & (~(1 << 1)) & 1110
            s & 1010 => s & 1011 & 1110 => s & (~0100) & 1110 => s & (~(1 << 2)) & 1110
            s & 0110 => s & 0111 & 1110 => s & (~1000) & 1110 => s & (~(1 << 3)) & 1110

        */
        // 初始状态 比特位由高到低分别表示[狼 羊 菜 农夫]，且0表示在左岸，1表示在右岸
        int curStatus = 0b0000;
        int result[16];
        // 将所有的节点置为-1，即所有的节点直接还没有路径
        memset(result,-1,sizeof(result));
        // 将起始节点的父节点置为自身，避免子状态回到父状态
        result[curStatus] = curStatus;
        queue<int> q;
        q.push(curStatus);
        while(!q.empty()){
            // 获取队头的状态
            curStatus = q.front();
            q.pop();
            // 获取农夫的状态(0在左岸 1在右岸)
            int pepStatus = curStatus&1;
            for(int i=0;i<4;++i){
                // 计算下一个状态
                int next = (pepStatus==0?(curStatus|(1<<i)|1):(curStatus&(~(1<<i))&0b1110));
                // 可以达下一个状态[下一个状态没有到达过 且 不能可以狼羊在同岸(0b0011\0b1100)、也不可以羊菜在同岸(0b0110\0b1001)]
                if(result[next]==-1&&next!=0b0011&next!=0b1100&&next!=0b0110&&next!=0b1001){
                    // 记录下一状态的父节点为当前状态
                    result[next] = curStatus;
                    // 如果还没有全部到达右岸(状态 0b1111 => 15)则将新节点加入到队列 ，即15号结点还没有父节点，游戏还没结束
                    if(result[0b1111]==-1){
                        q.push(next);
                    }
                }
            }
        }
        // 没有找到路径
        if(result[0b1111]==-1){
            return false;
        }
        stack<int> s;
        // 将终点状态 加入到栈
        s.push(0b1111);
        // 如果栈头不是 起始点状态
        while(s.top()!=0b0000){
            // 则将栈头的节点的父节点加入到栈
            s.push(result[s.top()]);
        }
        //翻译字节状态
        translate(s);
        return true;
    }
    template<typename T>
    void showStatus(const vector<T>& s,const string& name){
        cout << name << "[";
        if(s.size()>0){
            cout << s[0];
            for(size_t i=1;i<s.size();++i){
                cout << "," << s[i];
            } 
            cout << "]" <<endl;
        }else{
            cout << " NULL ]"<<endl;
        }
    }
    template<typename T>
    void hanoi(int n,
        vector<T>& a,const string& aName,
        vector<T>& b,const string& bName,
        vector<T>& c,const string& cName){
        if(n==1){
            cout << aName  << "->" << cName << " : " << a[0] << endl;
            c.insert(c.begin(),a[0]);
            a.erase(a.begin());
            showStatus(a,aName);
            showStatus(b,bName);
            showStatus(c,cName);
        }else{
            hanoi(n-1,a,aName,c,cName,b,bName);
            cout << aName  << "->" << cName << " : " << a[0] << endl;
            c.insert(c.begin(),a[0]);
            a.erase(a.begin());
            showStatus(a,aName);
            showStatus(b,bName);
            showStatus(c,cName);
            hanoi(n-1,b,bName,a,aName,c,cName);
        }
    }
    void hanoiRun(){
        vector<int> a({1,2,3}),b,c;
        hanoi(a.size(),a,"第一根柱子",b,"第二根柱子",c,"第三根柱子");
    }
    
    void hanoi2(int n,
        const string& aName,
        const string& bName,
        const string& cName){
            if(n >= 1){
                hanoi2(n-1,aName,cName,bName);
                cout << aName  << "->" << cName  << endl;
                hanoi2(n-1,bName,aName,cName);
            }
        }
    void hanoiRun2(){
        vector<int> a({1,2,3}),b,c;
        hanoi(a.size(),a,"第一根柱子",b,"第二根柱子",c,"第三根柱子");
    }
    bool checkLine(int y1,int x1,int y2,int x2){
        if(x1 == x2){
            return true;
        }
        // dy = y1-y2
        y1-=y2;
        // dx = x1-x2
        x1-=x2;
        if((y1+x1==0)||(y1==x1)){
            return true;
        }
        return false;
    }
    void nQueens(int n){
        int * m = new int[n];
        memset(m,0,sizeof(int)*n);
        // 保存找到结果的次数
        int resultCount = 0;
        int row = 0;
        while(true){
            //找到第row符合的列
            for(int i=0;i<row;){
                // 第 i 行与第 row 行比较
                // 不冲突
                if(false == checkLine(i,m[i],row,m[row])){
                    // 切换到 i+1 行
                    ++i;
                }else{
                    // 第 row 行 切换到下一列，已经在最右边不可以切换
                    if(m[row]<n-1){
                        ++m[row];
                    }
                    //否则的话 row 行列数变为0，并且返回上一行 
                    else{
                        // 回溯
                        while(m[row]==n-1&&row!=0){
                            m[row] = 0;
                            --row;
                        }
                        if(m[row]<n-1){
                            ++m[row];
                        }else{
                            // 无法回溯
                            break;
                        }
                    }
                    //i 重置为0,与新的row开始比较
                    i = 0;
                }
            }
            // n 行都满足条件则输出单次结果
            if(row==n-1){
                // 打印
                ++resultCount;
                for(int i=0;i<n;i++){
                    for(int j=0;j<m[i];++j){
                        cout<<"+ ";
                    }
                    cout<<"O ";
                    for(int j=m[i]+1;j<n;++j){
                        cout <<"+ ";
                    }
                    cout<<endl;
                }
                cout <<endl;
                //  回溯
                while(m[row]==n-1&&row!=0){
                    m[row] = 0;
                    --row;
                }
                if(m[row]<n-1){
                    ++m[row];
                }else{
                    // 无法回溯
                    break;
                }
                continue;
            }
            ++row;
            // 当遍历超过一半时退出(中心对称)
            if(m[0]>=(n>>1)){
                break;
            }
        }
        if(resultCount>0){
            cout << "count:"<<(n==1?1:resultCount*2)<<endl;
        }else{
            cout << "没有结果!"<<endl;
        }
        delete[] m;
    }
};

int  main(){
    Solution::hanoi2(2,"a","b","c");
    return 0;
};