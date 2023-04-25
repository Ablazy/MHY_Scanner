#include "Download.h"

Download::Download()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

Download::~Download()
{
}

size_t Download::write_data(void* ptr, size_t size, size_t nmemb, void* stream)// ����ص���������curl���ص�����д�뻺����
{
    DWORD bytes_written = 0;
    BOOL result = WriteFile((HANDLE)stream, ptr, (DWORD)(size * nmemb), &bytes_written, NULL);
    if (result)
    {

        return bytes_written / size;
    }
    else
    {
        return 0;
    }
}

void Download::stopDownload()
{
    //std::this_thread::sleep_for(std::chrono::seconds(3)); // ֹͣ��������ĵȴ�ʱ��
    curl_easy_pause(curl, CURLPAUSE_ALL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    curl_easy_cleanup(curl);
    std::cout << "������ֹͣ" << std::endl;
}

void Download::curlDownlod(std::string url)
{
    // ʹ��curl����ֱ����
    if (curl)
    {
        //ռ������
        HANDLE fp = CreateFile(
            L".\\cache\\output.flv",            // �ļ���
            GENERIC_WRITE,                      // ����Ȩ��
            FILE_SHARE_READ | FILE_SHARE_WRITE, // ����ģʽ
            NULL,                               // ��ȫ����
            CREATE_ALWAYS,                      // ��ģʽ
            FILE_ATTRIBUTE_NORMAL,              // �ļ�����
            NULL                                // ģ���ļ����
        );
        if (fp)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36 Edg/110.0.1587.41");
            curl_easy_setopt(curl, CURLOPT_REFERER, "https://live.bilibili.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30000L);
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK)
            {
                std::cout << "Download completed successfully." << std::endl;
            }
            else
            {
                std::cout << "Error downloading: " << curl_easy_strerror(res) << std::endl;
            }
            // ��ͣ���ز���
            //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            //curl_easy_pause(curl, CURLPAUSE_ALL);
            //curl_easy_cleanup(curl);
            CloseHandle(fp);
        }
    }
#ifdef _DEBUG
    std::cout << "Exiting Down." << std::endl;
#endif
}
