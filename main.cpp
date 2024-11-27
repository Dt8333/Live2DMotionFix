#include <fstream>
#include <iostream>
#include <filesystem>

#include "nlohmann/json.hpp"

enum CubismMotionSegmentType
{
    CubismMotionSegmentType_Linear = 0,        // リニア
    CubismMotionSegmentType_Bezier = 1,        // ベジェ曲線
    CubismMotionSegmentType_Stepped = 2,       // ステップ
    CubismMotionSegmentType_InverseStepped = 3 // インバースステップ
};

void fixMotion(std::string path)
{
    if (std::filesystem::exists(path + ".bak"))
    {
        std::cout << "Backup file found, may have been fix, skipping:" << path << std::endl;
        return;
    }
    std::ifstream i(path);
    nlohmann::json j = nlohmann::json::parse(i);
    i.close();

    std::filesystem::rename(path, path + ".bak");

    // get the object array of the key name "curve"
    nlohmann::json curves = j["Curves"];
    // for each element in the array
    int curve_count = curves.size();
    int segment_count = 0;
    int point_count = 0;
    for (nlohmann::json::iterator it = curves.begin(); it != curves.end(); ++it)
    {
        // get the object array of the key name "Segments"
        nlohmann::json Segments = it.value()["Segments"];
        // for each element in the array
        segment_count += Segments.size();
        for (int segmentPosition = 0; segmentPosition < Segments.size();)
        {
            if (segmentPosition == 0)
            {
                point_count += 1;
                segmentPosition += 2;
            }
            enum CubismMotionSegmentType segmentType = (enum CubismMotionSegmentType)Segments[segmentPosition];
            switch (segmentType)
            {
            case CubismMotionSegmentType_Linear:
                segmentPosition += 3;
                point_count += 1;
                break;
            case CubismMotionSegmentType_Bezier:
                segmentPosition += 7;
                point_count += 3;
                break;
            case CubismMotionSegmentType_Stepped:
                segmentPosition += 3;
                point_count += 1;
                break;
            case CubismMotionSegmentType_InverseStepped:
                segmentPosition += 3;
                point_count += 1;
                break;
            default:
                break;
            }
        }
    }
    j["Meta"]["CurveCount"] = curve_count;
    j["Meta"]["TotalSegmentCount"] = segment_count;
    j["Meta"]["TotalPointCount"] = point_count;

    std::ofstream o(path);
    o << j.dump(4) << std::endl;
    o.close();
    std::cout << "Fixed:" << path << std::endl;
}

int main()
{
    std::filesystem::path path = std::filesystem::current_path() / "motions";
    if (!std::filesystem::is_directory(path))
    {
        std::cout << "No motions directory found, exiting" << std::endl;
        return 1;
    }
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension() == ".json")
        {
            fixMotion(entry.path().string());
        }
    }
    return 0;
}
