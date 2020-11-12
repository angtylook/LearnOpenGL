#include "filesystem.h"

#include <fstream>
#include <vector>

namespace engine {
namespace fs {

namespace detail {
Path root_;
}

const char kPathSeparator = '/';
const char kExtensionSeparator = '.';

// class Path
Path::Path() {}

Path::Path(const std::string& path) : path_(path) {}

Path Path::GetParentPath() const {
    if (!HasParentPath()) {
        return Path();
    }
    auto pos = path_.find_last_of(kPathSeparator);
    if (pos == path_.size() - 1) {
        pos = path_.find_last_of(kPathSeparator, pos);
    }

    return Path(std::string(path_, 0, pos));
}

Path Path::Append(const Path& path) const {
    std::string data = path_;
    // find first not sparator
    auto first = path.path_.begin();
    auto last = path.path_.end();
    while (first != last && *first == kPathSeparator) {
        ++first;
    }
    // append to end
    if (first != last) {
        if (data.back() != kPathSeparator) {
            data.push_back(kPathSeparator);
        }
        data.append(first, last);
    }
    return Path(data);
}

const char* Path::data() const {
    return path_.data();
}

void Path::ReplaceExtension(const std::string& extension) {
    if (extension.empty()) {
        return;
    }

    if (!HasExtension()) {
        if (path_.back() == kPathSeparator) {
            path_.pop_back();
        }
        path_.append(extension);
        return;
    }

    auto pos = path_.find_last_of(kExtensionSeparator);
    path_ = path_.substr(0, pos);
    path_.append(extension);
}

bool Path::IsValid() const {
    return !path_.empty();
}

bool Path::HasParentPath() const {
    // last sparator at begin or not founded
    auto pos = path_.find_last_of(kPathSeparator);
    return pos != 0 && pos != std::string::npos;
}

bool Path::HasExtension() const {
    auto first = path_.rbegin();
    auto last = path_.rend();

    for (; first != last; ++first) {
        if (*first == kPathSeparator) {
            return false;
        }
        if (*first == kExtensionSeparator) {
            return true;
        }
    }
    return false;
}

std::string Path::GetExtension() const {
    if (!HasExtension())
        return std::string();

    return path_.substr(path_.find_last_of(kExtensionSeparator));
}

std::string Path::GetFileName() const {
    if (path_.empty() || path_.back() == kPathSeparator)
        return std::string();
    return path_.substr(path_.find_last_of(kPathSeparator) + 1);
}

std::string Path::GetFileNameWithoutExtension() const {
    if (path_.empty() || path_.back() == kPathSeparator)
        return std::string();
    if (!HasExtension())
        return GetFileName();
    auto begin = path_.find_last_of(kPathSeparator);
    auto end = path_.find_last_of(kExtensionSeparator);
    return path_.substr(begin + 1, end - begin - 1);
}

Path Path::AddPrefix(const std::string& prefix) {
    std::string filename = prefix;
    filename.append(GetFileName());
    Path path = GetParentPath();
    return path.Append(Path(filename));
}

Path Path::AddSubfix(const std::string& subfix) {
    std::string filename = GetFileNameWithoutExtension();
    filename.append(subfix);
    filename.append(GetExtension());
    Path path = GetParentPath();
    return path.Append(Path(filename));
}

bool Init(const std::string& base_directory) {
    detail::root_ = Path(base_directory);
    return true;
}

bool Exist(const Path& path) {
    Path p = detail::root_.Append(path);
    if (FILE* file = fopen(p.data(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

int64_t GetFileLength(const Path& path) {
    std::ifstream is(GetRawPath(path).data(), std::ifstream::binary);
    int64_t length = -1;
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        length = is.tellg();
        is.seekg(0, is.beg);
        is.close();
    }
    return length;
}

std::string GetFileContent(const Path& path) {
    std::ifstream is(GetRawPath(path).data(), std::ifstream::binary);
    std::string buffer;
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        int64_t length = is.tellg();
        is.seekg(0, is.beg);

        buffer.resize(length, ' ');  // reserve space
        char* begin = &*buffer.begin();
        is.read(begin, length);
        is.close();
    }
    return buffer;
}

Path GetRawPath(const Path& path) {
    return detail::root_.Append(path);
}

void IgnoreLine(std::istream& is, int64_t length) {
    is.ignore(length, '\n');
}

}  // namespace fs
}  // namespace engine
