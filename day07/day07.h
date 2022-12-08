#pragma once

#include <istream>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <memory>

// Forward
class Directory;
class File;

class Directory {
public:
    Directory(Directory* parent, std::string const name)
        : m_parent(parent)
        , m_name(name)
    {
    }

    Directory(const Directory&) = delete;
    Directory(Directory&&) = default;

    std::string const& name() const { return m_name; }

    std::vector<Directory>& child_dirs() { return m_child_dirs; }
    std::vector<File> const& files() { return m_files; }
    Directory* parent() const { return m_parent; }

    std::string const path() const;
    uint64_t size() const;
    Directory& traverse_dir_or_create(std::string name);
    void insert_file(File f);

private:
    Directory* m_parent = nullptr; // Should be still alive by ref (better way of managing? shared ptr?)
    std::string const m_name;
    std::vector<Directory> m_child_dirs;
    std::vector<File> m_files;
};

class File {
public:
    File(std::string const name, uint64_t size)
        : m_name(name)
        , m_size(size)
    {
    }

    std::string const& name() const { return m_name; }
    uint64_t size() const { return m_size; }

private:
    std::string const m_name;
    uint64_t m_size;
};

