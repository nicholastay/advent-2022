#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

#include "day07.h"

// --- Directory Impl

Directory& Directory::traverse_dir_or_create(std::string name)
{
    for (auto& dir : m_child_dirs) {
        if (dir.name() == name)
            return dir;
    }

    m_child_dirs.emplace_back(this, name);
    return m_child_dirs.back();
}

void Directory::insert_file(File f)
{
    m_files.push_back(f);
}

std::string const Directory::path() const
{
    Directory const* current_dir = this;
    std::vector<std::string> dirs; // better way to do this?
    while (current_dir->parent()) {
        dirs.push_back(current_dir->name());
        current_dir = current_dir->parent();
    } 

    std::reverse(dirs.begin(), dirs.end());
    std::string path = "/";
    for (auto const& s : dirs)
        path += s + "/";
    return path;
}

uint64_t Directory::size() const
{
    // Could cache this?
    uint64_t size = 0;

    for (auto const& file : m_files)
        size += file.size();

    for (auto const& dir : m_child_dirs)
        size += dir.size();

    return size;
}

// --- Main

void parse_ls_lines(Directory& pwd, std::ifstream& file)
{
    std::string line;

    // Lets us peek ahead if we are done.
    auto old_pos = file.tellg();
    for (std::string line; std::getline(file, line); ) {
        if (line.starts_with("$ "))
            break;

        std::istringstream iss(line);

        std::string name;
        if (!getline(iss, name, ' ')) throw std::runtime_error("invalid ls format (1)");

        if (name == "dir") {
            if (!getline(iss, name, ' ')) throw std::runtime_error("invalid ls dir format");
            pwd.traverse_dir_or_create(name);
        } else {
            // size then name in format
            uint64_t size = std::atoll(name.c_str());
            if (!getline(iss, name, ' ')) throw std::runtime_error("invalid ls format (2)");
            pwd.insert_file({ name, size });
        }

        old_pos = file.tellg();
    }

    file.seekg(old_pos, std::ios_base::beg);
}

// Would prefer not having to pass line/file etc but can't think of better way for now.
void parse_line(Directory** pwd, std::string& line, std::ifstream& file)
{
    std::istringstream iss(line);

    std::string s;

    // Format: '$ cd a'
    if (!getline(iss, s, ' ')) throw std::runtime_error("invalid cmd format ($)");
    assert(s == "$");

    if (!getline(iss, s, ' ')) throw std::runtime_error("no cmd given");

    if (s == "cd") {
        if (!getline(iss, s, ' ')) throw std::runtime_error("no cd dirname given");
        if (s == "..") {
            *pwd = (*pwd)->parent();
            return;
        }
        if (s == "/") {
            // Go up to root
            // Probably use a better way instead lol
            while (auto parent = (*pwd)->parent())
                *pwd = parent;
            return;
        }
        *pwd = &((*pwd)->traverse_dir_or_create(s));
        return;
    }
    if (s == "ls") {
        parse_ls_lines(**pwd, file);
        return;
    }
    throw std::runtime_error("invalid cmd");
}

void dump(Directory& dir, int i = 0)
{
    std::cout << std::string(i*2, ' ') << "- " << dir.name() << " (dir, size=" << dir.size() << ")\n";

    auto padding = std::string((i+1)*2, ' ');

    for (auto const& file : dir.files())
        std::cout << padding << "- " << file.name() << " (file, size=" << file.size() << ")\n";

    for (auto& d : dir.child_dirs())
        dump(d, i + 1);
}

void part1_impl(Directory& dir, int& result)
{
    auto size = dir.size();
    if (size <= 100000)
        result += size;

    for (auto& d : dir.child_dirs())
        part1_impl(d, result);
}
void part1(Directory& root_dir)
{
    int r = 0;
    part1_impl(root_dir, r);
    std::cout << "Part 1 - Sum of <100,000 size dirs: " << r << "\n";
}

void part2_iterate(Directory& dir, uint64_t to_cleanup_space, uint64_t& smallest_cleanup_size)
{
    auto size = dir.size();
    if (size >= to_cleanup_space && size < smallest_cleanup_size)
        smallest_cleanup_size = size;

    for (auto& d : dir.child_dirs())
        part2_iterate(d, to_cleanup_space, smallest_cleanup_size);
}
void part2(Directory& root_dir, uint64_t total_space, uint64_t required_space)
{
    auto used_space = root_dir.size();
    assert(used_space < total_space);

    auto free_space = total_space - used_space;
    assert(required_space > free_space);
    uint64_t to_cleanup_space = required_space - free_space;

    uint64_t smallest_cleanup_size = used_space; // Biggest would be delete the whole root, lol
    part2_iterate(root_dir, to_cleanup_space, smallest_cleanup_size);
    std::cout << "Part 2 - Size of smallest candidate directory: " << smallest_cleanup_size << "\n";
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);

    Directory root_dir({}, "/");
    Directory* pwd = &root_dir;

    for (std::string line; std::getline(file, line); )
        parse_line(&pwd, line, file);

    dump(root_dir);
    part1(root_dir);
    part2(root_dir, 70000000, 30000000);
}
