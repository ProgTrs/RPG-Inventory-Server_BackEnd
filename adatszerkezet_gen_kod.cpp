#include <vector>
#include <iostream>
using namespace std;

vector<string> kont_belso({"vector","list","map"});
vector<string> kont_utolso({"vector","list","map", "set"});
vector<string> kulcst({"int","char","string"});
vector<string> ertekt({"int","char","string","float","bool"});

string generate_from_hash(unsigned int h, int n)
{
    if (n>0)
    {
		vector<string> &kont = (n==1)?kont_utolso:kont_belso;
        int z=h%kont.size();
        if (z<2)
        {
            return kont[z]+"<"+generate_from_hash(h/17,n-1)+" > ";
        }
        else if (z==2)
        {
            int k=(h%5)%kulcst.size();
            return kont[z]+"<"+kulcst[k]+", "
                   +generate_from_hash(h/17,n-1)+" > ";
        } else {
            int k=(h%5)%kulcst.size();
            if (n==1) {
            	return kont[z]+"<"+kulcst[k]+">";
            }
            return kont[z]+"<"+generate_from_hash(h/17,n-1)+" > ";

        }
    }
    else
    {
        int z=h%ertekt.size();
        return ertekt[z];
    };
}

string generate(string s)
{
    string res="";
    unsigned int hash = 1;
    for (int i=0; i<s.length(); i++) hash=17*hash+s[i];

    res = generate_from_hash(hash,3);
    return res;
}

int main2()
{
    string s="";
    do
    {
        getline(cin,s);
        if (cin.good()) cout << s <<": " << generate(s) << endl;
    }
    while (cin.good());
}

//list<map<int, list<string>>>
