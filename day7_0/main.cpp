#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include "../day7_input.h"

const int max_operand = 2;

struct StringView
{
    StringView() = default;
    StringView(const char* _str)
    {
        str = _str;
        len = (int)strlen(_str);
    }

    const char* str = nullptr;
    int len = 0;

    bool equal(const StringView& input) const
    {        
        return (len == input.len) && (strncmp(str, input.str, len) == 0);
    }

    bool equal(const char* input) const
    {
        return equal(StringView(input));
    }
};

struct LineInfo
{
    bool is_command;
    StringView op[max_operand];
};


struct File
{
    File() = delete;
    File(const StringView& _name, int _size) : name(_name), size(_size) {}

    StringView name;
    int size = 0;
};

struct Node
{
    Node() = delete;
    Node(Node* _parent, const StringView& _name) : parent(_parent), name(_name) {}
    ~Node()
    {
        for (auto&& node : directories) delete node;
        for (auto&& file : files) delete file;
    }

    Node* parent = nullptr;
    StringView name;
    std::vector<Node*> directories;
    std::vector<File*> files;

    File* findFile(const StringView& dir_name)
    {
        for (size_t i = 0; i < files.size(); ++i)
        {
            if (files[i]->name.equal(dir_name))
                return files[i];
        }

        return nullptr;
    }

    Node* findDir(const StringView& dir_name)
    {
        for (size_t i = 0; i < directories.size(); ++i)
        {
            if (directories[i]->name.equal(dir_name))
                return directories[i];
        }

        return nullptr;
    }

    Node* findDir(const char* dir_name)
    {
        return findDir(StringView(dir_name));
    }

    size_t byteSize()
    {
        size_t size = 0;
        for (File* file : files)
            size += file->size;

        for (Node* node : directories)
            size += node->byteSize();
        
        return size;
    }

    // Traverse the tree, if the action return false stop depth traverse abd continue to sibling    
    template <typename Lambda>
    void visit(Lambda&& Fn)
    {
        bool visit_children = Fn(this);
        if (visit_children)
            for (Node* child : directories)
                child->visit(Fn);
    }
    
};

const char* skipWhiteSpace(const char* input)
{
    while (*input == ' ')
        ++input;
    return input;
}

const char* readOperand(const char* input, StringView& op)
{
    // finish by '\n' '\0' or ' '
    const char* r = input;
    while (!(*r == '\n' || *r == '\0' || *r == ' '))
        ++r;

    if (r != input)
    {
        op.str = input;
        op.len = (int)(r - input);
    }

    return r; // skip '\n' or '\0' 
}

const char* parseLine(const char* input, LineInfo& info)
{
    info.is_command = input[0] == '$';
    if (info.is_command)
        input = skipWhiteSpace(input + 1);
    for (int i = 0; i < max_operand; ++i)
    {
        input = readOperand(input, info.op[i]);
        input = skipWhiteSpace(input);
    }

    input += 1; // next line, skip '\0' or '\n'
    return input;
}

Node* root = nullptr;
Node* pwd = nullptr;    // current dir

// execute the command and return pointer on next command
const char* executeCommand(const char* input, const LineInfo& info)
{
    assert(info.is_command);
    if (info.op[0].equal("cd"))
    {
        if (info.op[1].len == 1 && info.op[1].str[0] == '/')
        {
            // go root
            pwd = root;
        }
        else if ((info.op[1].len == 2 && info.op[1].equal("..")))
        {
            if (pwd->parent)
                pwd = pwd->parent;
        }
        else
        {
            if (info.op[1].len > 0)
            {
                Node* child = pwd->findDir(info.op[1]);
                if(child)
                    pwd = child;
            }
        }
    }
    else if (info.op[0].equal("ls"))
    {
        if (!(*input == '$' || *input == '\0')) // ls return nothing
        {
            // Read until next command or reach '\0'
            do
            {
                LineInfo resultInfo;
                input = parseLine(input, resultInfo);
                assert(!resultInfo.is_command);
                if (resultInfo.op[0].equal("dir"))
                {
                    // A new dir ?
                    Node* dir = pwd->findDir(resultInfo.op[1]);
                    if (!dir)
                    {
                        pwd->directories.push_back(new Node(pwd, resultInfo.op[1]));
                    }
                }
                else
                {
                    // new file?
                    File* file = pwd->findFile(resultInfo.op[1]);
                    if (!file)
                    {
                        pwd->files.push_back(new File(resultInfo.op[1], atoi(resultInfo.op[0].str)));
                    }

                }

            } while (!(*(input - 1) == '\0' || *(input) == '$'));
        }
    }
    else
    {
        assert(false && "unknown command");
    }

    return input;
}

int main()
{
    // Create tree
    const char* root_name = "/";    
    root = new Node(nullptr,StringView(root_name));
    const char* input = day7_input;
    do
    {
        LineInfo info;
        input = parseLine(input, info);
        input = executeCommand(input, info);
    } while (*(input - 1) != '\0');

    // Part 1
    {
        // Search dir <= 100000
        size_t totalsize = 0;
        auto step1_Fn = [&](Node* node)
        {
            if (node->byteSize() <= 100000)
            {
                totalsize += node->byteSize();
            }

            return true;
        };
        root->visit(step1_Fn);
        printf("Result = %zd\n", totalsize);
    }

    // Part2
    {
        size_t disk_size =    70000000;
        size_t require_size = 30000000;        
        size_t current_size = root->byteSize();
        size_t remaining_space = disk_size - current_size;

        size_t minimal_size_to_delete = ~0;
        auto step2_Fn = [&](Node* node)
        {
            size_t node_byte_size = node->byteSize();
            if (remaining_space + node_byte_size >= require_size
                && node_byte_size < minimal_size_to_delete)
            {
                minimal_size_to_delete = node_byte_size;
            }

            return true;
        };
        root->visit(step2_Fn);       
        printf("Result = %zd\n", minimal_size_to_delete);
    }

    delete root;
    return 0;
}