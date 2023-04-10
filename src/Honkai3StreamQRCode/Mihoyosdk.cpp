#include "Mihoyosdk.h"
#include "HmacSha256.hpp"
#include "Json.h"
#include "Parser.h"
std::string Mihoyosdk::verify(const int uid, const std::string access_key)
{
	std::cout << "verify with uid="<<uid << std::endl;
	verifyData["uid"] = std::to_string(uid);
	verifyData["access_key"] = access_key;
	std::string ssd = "{\\\"access_key\\\":\\\"" + verifyData["access_key"]+ "\\\""",\\\"uid\\\":"+ verifyData["uid"]+"}";
	json::Json body;
	body.parse(verifyBody);
	body["data"] = ssd;
	const std::string sBody = body.str();
	body.clear();
	std::string s;
	u.PostRequest(s, loginV2Url, makeSign(sBody));
	std::cout << "����3��֤��ɣ���¼�ɹ�" << std::endl;
	return s;
}

std::string Mihoyosdk::getBHVer()
{
	return "6.5.0";
}

std::string Mihoyosdk::getOAServer()
{
	
	std::string bhVer = getBHVer();
	std::string oaMainUrl = "https://global2.bh3.com/query_dispatch?";
	std::string param = "version=" + bhVer+"_gf_android_bilibili&t="+ std::to_string(u.getCurrentUnixTime());
	std::string feedback;
	u.PostRequest(feedback, oaMainUrl + param, "");
	std::cout << u.UTF8_To_string(feedback)<< std::endl;
	param = "?version=" + bhVer + "_gf_android_bilibili&t=" + std::to_string(u.getCurrentUnixTime());
	json::Json j;
	j.parse(feedback);
	std::string dispatch_url = j["region_list"][0]["dispatch_url"];
	std::string dispatch;
	u.PostRequest(dispatch, dispatch_url+param,"");
	dispatch = u.UTF8_To_string(dispatch);
	std::cout << "���OA�������ɹ� : " <<dispatch << std::endl;
	j.clear();
	return dispatch;
}

std::string Mihoyosdk::makeSign(const std::string data)
{
	std::string sign;
	std::string	data2;
	json::Json p;
	p.parse(data);
	std::map<std::string,std::string> m= p.objToMap();
	for (auto &it: m)
	{
		if (it.first == "sign")
			continue;
		if(it.first=="data")
		{
			if (it.second.front() == '\"')
				it.second.erase(0, 1);
			if (it.second.back() == '\"')
				it.second.pop_back();
		}
		if(it.first =="device")
		{
			if (it.second.front() == '\"')
				it.second.erase(0, 1);
			if (it.second.back() == '\"')
				it.second.pop_back();
		}
		data2 += it.first + "=" + it.second + "&";
	}
	data2.erase(data2.length() - 1);
	std::cout <<"data2=" << data2 << std::endl;
	sign = bh3Sign(data2);
	p["sign"] = sign;
	sign = p.str();
	p.clear();
	return sign;
}

std::string Mihoyosdk::bh3Sign(std::string data)
{
	std::cout << data << std::endl;
	std::string key = "0ebc517adb1b62c6b408df153331f9aa";
	data.erase(std::remove(data.begin(), data.end(), '\\'), data.end());
	std::cout << data << std::endl;
	std::string sign = hmac_sha256(key, data);
	std::cout << "Hmac_Sha256ǩ�����" << std::endl;
	return sign;
}
