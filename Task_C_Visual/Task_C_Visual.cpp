#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include <string>

using namespace std;

int get_expression_type(const string& exp, string& left_exp, string& right_exp, int& value)
{
    auto separator = exp.find('=', 0);
    left_exp = exp.substr(0, separator);
    string right = exp.substr(separator + 1);
    for (auto c : right)
    {
        if (isalpha(c))
        {
            right_exp = right;
            return 1;
        }
    }

    value = stoi(right);
    return 0;

} // возвращает 0, если тип {exp=val} и не ноль, если тип {exp=exp}, в аргументы записывает найденные значения

int main() {
    string expression;
    map<string, stack<int>> variables;
    stack<map<string, size_t>> blocks;
    queue<int> output;
    
    while (cin >> expression && !cin.eof())
    {
        string left_exp;
        string right_exp;
        int value;

        if (expression == "{") // начало нового блока
        {
            map<string, size_t> in_block_map;
            blocks.push(in_block_map);
        }
        else if (expression == "}") // конец блока
        {
            if (!blocks.empty())
            {
                for (auto block : blocks.top())
                {
                    auto find_var = variables.find(block.first);
                    for (size_t i = 0; i < block.second; ++i) {
                        find_var->second.pop();
                    }
                    if (find_var->second.empty())
                    {
                        variables.erase(block.first);
                    }
                }
                blocks.pop();
            }
        }
        else if (get_expression_type(expression, left_exp, right_exp, value) == 0) // <exp>=<val>, определяем тип выражения и запоминаем левую и правую его часть
        {
            auto find_variable = variables.find(left_exp);

            if (!blocks.empty())
            {
                auto find_in_block = blocks.top().find(left_exp);
                if (find_in_block == blocks.top().end())
                {
                    blocks.top().insert({ left_exp, 1 });
                }
                else
                {
                    find_in_block->second++;
                }
            }

            if (find_variable == variables.end()) // впервые встреченная переменная
            {
                stack<int> values_stack;
                values_stack.push(value);
                variables.insert({ left_exp, values_stack });
            }
            else // добавляем значение в стек переменной
            {
                find_variable->second.push(value);
            }
        }
        else if (get_expression_type(expression, left_exp, right_exp, value) == 1) // <exp>=<exp>
        {
            auto find_variable = variables.find(left_exp);
            auto find_value = variables.find(right_exp);
            if (find_value == variables.end())
            {
                value = 0;
            }
            else
            {
                value = find_value->second.top();
            }

            output.push(value);

            if (!blocks.empty())
            {
                auto find_in_block = blocks.top().find(left_exp);
                if (find_in_block == blocks.top().end())
                {
                    blocks.top().insert({ left_exp, 1 });
                }
                else
                {
                    find_in_block->second++;
                }
            }

            if (find_variable == variables.end()) // впервые встреченная переменная
            {
                stack<int> values_stack;
                values_stack.push(value);
                variables.insert({ left_exp, values_stack });
            }
            else // добавляем значение в стек переменной
            {
                find_variable->second.push(value);
            }
        }
    }
    while (!output.empty())
    {
        cout << output.front() << endl;
        output.pop();
    }
    return 0;
}
