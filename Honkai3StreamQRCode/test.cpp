#include"main.h"
#include"Bsgsdk.h"
using namespace json;
using namespace std;
int main()
{
	utils u;
	Bsgsdk b;
	Json j;
	string res;
	map<string, string>m;
	m.insert({ "pwd","����" });
	//res = "hello���";
	//int i;
	//u.PostRequest("http://httpbin.org/post", "", res);
	//i = u.getCurrentUnixTime();
	j = b.getUserInfo("1","2");
	//res = "hello";
	//res = b.setSign(m);
	//res = u.urlEncode(res);
	//j.parse("{ \"pwd\":\"����\" }");
	//res = b.setSign(j);
	cout << res << endl;

}