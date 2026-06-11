/*
Author      : gongzhihao
Update Date : 2025/07/09
*/
#include "comm_layer/comm_json/comm_json.h"

// 读文件
std::string ReadFile(const std::string &abs_file_name)
{
    std::string file_data;
    std::ifstream ifs(abs_file_name, std::ios::binary | std::ios::ate);

    if (!ifs)
    {
        return file_data;
    }

    std::streamsize file_size = static_cast<std::streamsize>(ifs.tellg());
    if (file_size <= 0)
    {
        return file_data;
    }

    ifs.seekg(0);
    if (!ifs)
    {
        return file_data;
    }

    file_data.resize(static_cast<size_t>(file_size));
    ifs.read(&file_data[0], file_size);

    if (!ifs || ifs.gcount() != file_size)
    {
        file_data.clear();
    }

    return file_data;
}

// 写文件
bool WriteFile(const std::string &abs_file_name, const std::string &file_data)
{
    std::ofstream ofs(abs_file_name, std::ios::binary);

    if (!ofs)
    {
        return false;
    }

    ofs.write(file_data.data(), static_cast<std::streamsize>(file_data.size()));

    if (!ofs)
    {
        return false;
    }

    return true;
}

// 字符串->Json
Json::Value StrToJson(const std::string &str_data)
{
    Json::CharReaderBuilder builder;
    builder["collectComments"] = true;
    builder["emitUTF8"] = true;

    std::unique_ptr<Json::CharReader> p_reader(builder.newCharReader());

    const char *start = str_data.data();
    const char *end = start + str_data.size();
    Json::Value json_data;
    Json::String errs;

    try
    {
        if (!p_reader->parse(start, end, &json_data, &errs))
        {
            json_data = Json::Value();
        }
    }
    catch (...)
    {
        json_data = Json::Value();
    }

    return json_data;
}

// Json->字符串
std::string JsonToStr(const Json::Value &json_data)
{
    Json::StreamWriterBuilder builder;
    builder["emitUTF8"] = true;
    builder["indentation"] = "    ";
    builder["precision"] = 8;
    builder["precisionType"] = "decimal";

    std::unique_ptr<Json::StreamWriter> p_writer(builder.newStreamWriter());

    std::ostringstream oss;
    p_writer->write(json_data, &oss);
    return oss.str();
}
