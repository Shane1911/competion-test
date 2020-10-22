#include <bits/stdc++.h>
using namespace std;

#define TEST

typedef unsigned int ui;
struct Path{
    int length;
    vector<ui> path;

    Path(int length, const vector<ui> &path) : length(length), path(path) {}

    bool operator<(const Path&rhs)const{
        if(length!=rhs.length) return length<rhs.length;
//        for(int i=0;i<length;i++){
            if(path[0]!=rhs.path[0])
                return path[0]<rhs.path[0];
//        }
    }
};

class Solution{
public:
    //maxN=560000
    //maxE=280000 ~avgN=26000
    //vector<int> *G;
    vector<vector<int>> G;
    vector<vector<int>> G_r;
    unordered_map<ui,int> idHash; //sorted id to 0...n
    vector<ui> ids; //0...n to sorted id
    vector<ui> inputs; //u-v pairs
    vector<bool> vis;
    vector<bool> vis7;
    vector<bool> visM;
    vector<Path> ans;
    int nodeCnt=0;

    void parseInput(string &testFile){
        FILE* file=fopen(testFile.c_str(),"r");
        ui u,v,c;
        while(fscanf(file,"%u,%u,%u",&u,&v,&c)!=EOF){
            inputs.push_back(u);
            inputs.push_back(v);
        }
//#ifdef TEST
//        printf("%d Records in Total\n",cnt);
//#endif
    }

    void constructGraph(){
        auto tmp=inputs;
        sort(tmp.begin(),tmp.end());
        tmp.erase(unique(tmp.begin(),tmp.end()),tmp.end());
        nodeCnt=tmp.size();
        ids=tmp;
        int i=0;
        for(ui &x:tmp){
            idHash[x]=i++;
        }
//#ifdef TEST
//        printf("%d Nodes in Total\n",nodeCnt);
//#endif
        int sz=inputs.size();
        //G=new vector<int>[nodeCnt];
        G=vector<vector<int>>(nodeCnt);
        G_r=vector<vector<int>>(nodeCnt);
        for(int i=0;i<sz;i+=2){
            int u=idHash[inputs[i]],v=idHash[inputs[i+1]];
            G[u].push_back(v);
            G_r[v].push_back(u);
        }
    }
    void search(int root){
        vector<int> node_s;
        node_s.push_back(root);
        for(int i=0;i<3;i++) {
            vector<int> new_nodes;
            for(int &v:node_s){
                if(G_r[v].empty()) continue;
                for(int&u:G_r[v]){
                    if(u>root){
                        if(i==0) vis7[u]=true;
                        visM[u]=true;
                        new_nodes.push_back(u);
                    }
                }
            }
            node_s = new_nodes;
        }
    }
    void dfs(int head,int cur,int depth,vector<int> &path){
        vis[cur]=true;
        for(int &v:G[cur]){
            if(vis[v]||depth>6||v<head) continue;
            path.push_back(v);
            if(vis7[v]&&depth>=2){
                vector<ui> tmp;
                for(int &x:path)
                    tmp.push_back(ids[x]);
                ans.emplace_back(Path(depth,tmp)); //可以引用path
                path.pop_back();
                continue;
            }
            if(!visM[v]&&depth>3){
                path.pop_back();
                continue;
            }
            dfs(head,v,depth+1,path);
            path.pop_back();
        }
        vis[cur]=false;
    }
//    void dfs(int head,int cur,int depth,vector<int> &path){
//        vis[cur]=true;
//        path.push_back(cur);
//        for(int &v:G[cur]){
//            if(v==head && depth>=3 && depth<=7){
//                vector<ui> tmp;
//                for(int &x:path)
//                    tmp.push_back(ids[x]);
//                ans.emplace_back(Path(depth,tmp));
//            }
//            if(v>head && !vis[v] && depth<7){
//                if(!visM[v] && depth>3) continue;
//                dfs(head,v,depth+1,path);
//            }
//        }
//        vis[cur]=false;
//        path.pop_back();
//    }
    void solve(){
        vis=vector<bool>(nodeCnt,false);
        vector<int> path;
        for(int i=0;i<nodeCnt;i++){
            if(G[i].empty()) continue;
            vis7=vector<bool>(nodeCnt,false);
            visM=vector<bool>(nodeCnt,false);
            search(i);
            path.push_back(i);
            dfs(i,i,1,path);
            path.clear();
        }
        sort(ans.begin(),ans.end());
    }

    void save(string &outputFile){
        FILE *fp;
        fp = fopen(outputFile.c_str(),"w");
        fprintf(fp,"%d\n",ans.size());
        for(auto &x:ans) {
            auto path = x.path;
            int sz = path.size();
            fprintf(fp, "%u", path[0]);
            for (int i = 1; i < sz; i++)
                fprintf(fp, ",%u", path[i]);
            fprintf(fp, "\n");
        }
    }
};

int main()
{
    string testFile = "test_data.txt";
//    string outputFile = "output.txt";
#ifdef TEST
    string outputFile = "result_my.txt";
#endif
    auto t=clock();
    Solution solution;
    solution.parseInput(testFile);
    solution.constructGraph();
    auto t1=clock();
    solution.solve();
    auto t2=clock();
    solution.save(outputFile);
    auto t3=clock();
    cout<<double(t3-t2)/CLOCKS_PER_SEC<<endl;
    cout<<double(t2-t1)/CLOCKS_PER_SEC<<endl;
    cout<<double(clock()-t)/CLOCKS_PER_SEC<<endl;
    return 0;
}

















