#include "OfficialApi.h"
#include "Json.h"
#include <random>
#include <sstream>
#include <iomanip>

std::string OfficialApi::generateUUID() 
{
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0, 15);

    std::stringstream ss;
    for (int i = 0; i < 32; ++i) 
    {
        int randomDigit = distribution(generator);
        ss << std::hex << randomDigit;
    }

    std::string uuid = ss.str();

    // ��ʽ��UUID������ָ���
    uuid.insert(8, "-");
    uuid.insert(13, "-");
    uuid.insert(18, "-");
    uuid.insert(23, "-");

    return uuid;
}

//ɨ������
void OfficialApi::request()
{
    json::Json payload;
    payload["app_id"] = 8;
    payload["device"] = generateUUID();
    payload["ticket"] = ticket;
    std::string s;
    PostRequest(s, hkrpgFirst,payload.str());
}

//ɨ��ȷ��
void OfficialApi::ConfirmRequest()
{
    std::string s;
    std::string getToken = "https://api-takumi.miyoushe.com/auth/api/getGameToken";
    std::map<std::string, std::string> headers = { {"cookie", ""} };
    GetRequest(s, getToken, headers);
    s = UTF8_To_string(s);
    json::Json j;
    j.parse(s);
}