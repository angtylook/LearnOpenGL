#ifndef BASE_FILESYSTEM_H_
#define BASE_FILESYSTEM_H_

#include <cstdint>
#include <string>

namespace engine {
namespace fs {

class Path {
public:
    Path();

    Path(const std::string& path);

    Path GetParentPath() const;

    Path Append(const Path& path) const;

    const char* data() const;

    void ReplaceExtension(const std::string& extension);

    bool IsValid() const;

    bool HasParentPath() const;

    bool HasExtension() const;

    std::string GetExtension() const;

    std::string GetFileName() const;

    std::string GetFileNameWithoutExtension() const;

    Path AddPrefix(const std::string& prefix);

    Path AddSubfix(const std::string& subfix);

    // private:
    // void Format();
    // size_t GetExtensionSeparatorPostion() const;
private:
    std::string path_;
};

bool Init(const std::string& base_directory);

bool Exist(const Path& path);

int64_t GetFileLength(const Path& path);

std::string GetFileContent(const Path& path);

Path GetRawPath(const Path& path);

// Ignore everything else that comes on the line, up to 'length' characters.
void IgnoreLine(std::istream& is, int64_t length);

}  // namespace fs
}  // namespace engine

#endif  // BASE_FILESYSTEM_H_
