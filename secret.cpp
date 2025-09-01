#include <bits/stdc++.h>
using namespace std;
using int128 = __int128_t; // for safe intermediate multiplications

struct Rat {
    long long num{0}, den{1};
    Rat()=default;
    Rat(long long n):num(n),den(1){}
    Rat(long long n,long long d):num(n),den(d){ normalize(); }

    static long long cgcd(long long a,long long b){
        if(a<0) a=-a; if(b<0) b=-b;
        while(b){ long long t=a%b; a=b; b=t; }
        return a;
    }
    void normalize(){
        if(den<0){ den=-den; num=-num; }
        long long g=cgcd(num,den);
        if(g) { num/=g; den/=g; }
    }
    bool isInt() const { return den==1; }
};
Rat operator+(const Rat&a,const Rat&b){ Rat r((long long)((int128)a.num*b.den+(int128)b.num*a.den), (long long)((int128)a.den*b.den)); r.normalize(); return r;}
Rat operator-(const Rat&a,const Rat&b){ Rat r((long long)((int128)a.num*b.den-(int128)b.num*a.den), (long long)((int128)a.den*b.den)); r.normalize(); return r;}
Rat operator*(const Rat&a,const Rat&b){ Rat r((long long)((int128)a.num*b.num), (long long)((int128)a.den*b.den)); r.normalize(); return r;}
Rat operator/(const Rat&a,const Rat&b){ Rat r((long long)((int128)a.num*b.den), (long long)((int128)a.den*b.num)); r.normalize(); return r;}

int digit_of(char c){
    if('0'<=c&&c<='9') return c-'0';
    if('a'<=c&&c<='z') return 10+(c-'a');
    if('A'<=c&&c<='Z') return 10+(c-'A');
    return -1;
}
long long parse_in_base(const string&s,int base){
    long long acc=0;
    for(char c:s){
        int d=digit_of(c);
        acc=acc*base+d;
    }
    return acc;
}
Rat lagrange(const vector<pair<int,long long>>&pts,int xq){
    Rat acc(0);
    int k=pts.size();
    for(int i=0;i<k;i++){
        int xi=pts[i].first;
        long long yi=pts[i].second;
        Rat num(1),den(1);
        for(int j=0;j<k;j++) if(i!=j){
            int xj=pts[j].first;
            num=num*Rat(xq-xj);
            den=den*Rat(xi-xj);
        }
        acc=acc+Rat(yi)*(num/den);
    }
    return acc;
}
bool next_comb(vector<int>&idx,int n){
    int k=idx.size();
    for(int i=k-1;i>=0;i--){
        if(idx[i]!=i+n-k){
            idx[i]++;
            for(int j=i+1;j<k;j++) idx[j]=idx[j-1]+1;
            return true;
        }
    }
    return false;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string input((istreambuf_iterator<char>(cin)), {});
    int n=0,k=0;
    {
        regex rn("\"n\"\\s*:\\s*([0-9]+)");
        regex rk("\"k\"\\s*:\\s*([0-9]+)");
        smatch m;
        regex_search(input,m,rn); n=stoi(m[1]);
        regex_search(input,m,rk); k=stoi(m[1]);
    }
    regex rblock("\"([0-9]+)\"\\s*:\\s*\\{([^}]*)\\}");
    regex rbase("\"base\"\\s*:\\s*\"([0-9]+)\"");
    regex rval("\"value\"\\s*:\\s*\"([^\"]+)\"");
    vector<pair<int,long long>> pts;
    auto it=input.cbegin(),end=input.cend(); smatch m;
    while(regex_search(it,end,m,rblock)){
        int idx=stoi(m[1]); string inner=m[2];
        smatch mb,mv;
        if(regex_search(inner,mb,rbase)&&regex_search(inner,mv,rval)){
            int base=stoi(mb[1]); string val=mv[1];
            long long y=parse_in_base(val,base);
            pts.push_back({idx,y});
        }
        it=m.suffix().first;
    }
    sort(pts.begin(),pts.end());

    int best=-1; long long secret=0;
    vector<int> comb(k); iota(comb.begin(),comb.end(),0);
    auto eval=[&](const vector<int>&c){
        vector<pair<int,long long>> sub; for(int id:c) sub.push_back(pts[id]);
        Rat sec=lagrange(sub,0);
        if(!sec.isInt()) return pair<int,long long>{-1,0};
        int match=0;
        for(auto&p:pts){
            Rat v=lagrange(sub,p.first);
            if(v.den==1&&v.num==p.second) match++;
        }
        return pair<int,long long>{match,(long long)sec.num};
    };
    while(true){
        auto [mch,sec]=eval(comb);
        if(mch>best){best=mch; secret=sec;}
        if(!next_comb(comb,n)) break;
    }
    cout<<secret<<"\n";
}
