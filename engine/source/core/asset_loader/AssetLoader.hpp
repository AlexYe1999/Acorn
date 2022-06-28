#pragma once
#include"IRuntimeModule.hpp"
#include<vector>
#include<string>
namespace Acorn{

    class AssetLoader : public IRuntimeModule{
    public:
        AssetLoader(){}
        virtual ~AssetLoader(){}

        virtual void Initialize() override;
        virtual void Finalize() override;
        virtual void Tick() = 0;

    public:
        enum class AssetOpenMode : unsigned char{
            OPEN_TEXT   = 0, 
            OPEN_BINARY = 1, 
        };

        using AssetFilePtr = void*;

        bool AddSearchPath(const char *path);

        bool RemoveSearchPath(const char *path);

        bool FileExists(const char *filePath);

        AssetFilePtr OpenFile(const char* name, AssetOpenMode mode);

        Buffer SyncOpenAndReadText(const char *filePath);

        size_t SyncRead(const AssetFilePtr& fp, Buffer& buf);

        void CloseFile(AssetFilePtr& fp);

        size_t GetSize(const AssetFilePtr& fp);

        int32_t Seek(AssetFilePtr fp, long offset, AssetSeekBase where);

        inline std::string SyncOpenAndReadTextFileToString(const char* fileName)
        {
            std::string result;
            Buffer buffer = SyncOpenAndReadText(fileName);
            char* content = reinterpret_cast<char*>(buffer.m_pData);

            if (content)
            {
                result = std::string(std::move(content));
            }

            return result;
        }

    private:
        std::vector<std::string> m_strSearchPath;
    };


}