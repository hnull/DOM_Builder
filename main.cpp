#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#define file_input_name "example.html"

struct Tag
{
  int index;
  string declare;
  string content;
  vector<int> children;
};

void find_all_tags(string context, vector<string>& tags)
{
  for (int i = 0; i < context.length(); i++)
  {
    if (context[i] == '<')
    {
      string new_tag;
      while ((context[i] != '>'))
      {
          new_tag += context[i];
          i++;
      }
      new_tag += '>';
      tags.push_back(new_tag);
      continue;
    }
  }
}

void find_all_contens(string context, vector<string>& contents)
{
  for (int i = 0; i < context.length(); i++)
  {
    if (context[i] == '>')
    {
      string new_content;
      i++;
      while ((context[i] != '<'))
      {
          new_content += context[i];
          i++;
      }
      contents.push_back(new_content);
      continue;
    }
  }
}

int define_type_of_tag(string tag)
{
  if (tag[tag.size() - 2] == '/')
  {
    return 0;  //without close tag
  }
  else if(tag[1] == '/')
  {
    return 1; //close tag
  }
  else
  {
    return 2; //open tag
  }
}

string find_close_tag_from_open_tag(string open_tag)
{
  string close_tag = "</";
  int i = 1;
  while ((open_tag[i] != ' ') && (open_tag[i] != '>'))
  {
    close_tag += open_tag[i];
    i++;
  }
  close_tag += '>';
  return close_tag;
}

bool is_valid_child(vector<string> all_tags, int inner_index, int outer_index)
{
  int stack = 0;
  for (int z = outer_index + 1; z < inner_index; z++)
  {
    int temp_define_tag = define_type_of_tag(all_tags[z]);
    if (temp_define_tag == 2)
    {
      stack += 1;
    }
    else if (temp_define_tag == 1)
    {
      stack -= 1;
    }
  }
  if (stack == 0)
    return true;
  else
    return false;
}

void find_child_tags(vector<string> all_tags,  vector<string> all_contents, vector<Tag>& tags, int index)
{
  Tag tag;
  tag.index = index;
  vector<int> children;
  string close_tag = find_close_tag_from_open_tag(all_tags[index]);
  for (int j = index + 1; j < all_tags.size(); j++)
  {
    bool valid_child = is_valid_child(all_tags, j, index);
    if (all_tags[j] != close_tag)
    {
      if ((define_type_of_tag(all_tags[j]) != 1) && (valid_child == true))
      {
        children.push_back(j);
      }
    }
    else
    {
      break;
    }
  }
  tag.declare = all_tags[index];
  tag.children = children;
  tag.content = all_contents[index];
  tags.push_back(tag);
}

void build_tag_relatives(vector<string> all_tags,  vector<string> all_contents, vector<Tag>& tags)
{
  bool is_outer_tag_open = false;
  for (int i = 0; i < all_tags.size(); i++)
  {
    int tag_type = define_type_of_tag(all_tags[i]);
    switch (tag_type)
    {
      case 0:
      {
          Tag tag;
          tag.index = i;
          tag.declare = all_tags[i];
          tags.push_back(tag);
      }
      case 1:
      {
          continue;
      }
      case 2:
      {
        find_child_tags(all_tags, all_contents, tags, i);
      }
    }
  }

}

void split_input_with_comma(string input_string, vector<int>& inputs_number)
{
    stringstream ss(input_string);
    int i;
    while (ss >> i)
    {
        inputs_number.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
}

Tag find_tag_index_from_struct_of_tag(vector<Tag> struct_of_tags, int index)
{
  for (int j = 0; j < struct_of_tags.size(); j++)
  {
    if (struct_of_tags[j].index == index)
    {
      return struct_of_tags[j];
    }
  }
}

string show_complete_tag_info(vector<Tag> struct_of_tags, int index)
{
  string final_str;
  Tag tag = find_tag_index_from_struct_of_tag(struct_of_tags, index);
  if (tag.children.size() == 0)
  {
    if (define_type_of_tag(tag.declare) != 0)
      return tag.declare + ' ' + tag.content  + ' ' + find_close_tag_from_open_tag(tag.declare) + '\n';
    else
      return tag.declare + '\n';
  }
  else
  {
    final_str += tag.declare + ' ';
    for (int i = 0; i < tag.children.size(); i++)
    {
      final_str += show_complete_tag_info(struct_of_tags, tag.children[i]);
    }
    final_str += ' ' + find_close_tag_from_open_tag(tag.declare) + '\n';
  }
  return final_str;
}

void find_desire_tag(vector<int> inputs_number, vector<Tag> struct_of_tags)
{
  Tag tag = struct_of_tags[0];
  for (int i = 0; i < inputs_number.size(); i++)
  {
    int new_tag_index = tag.children[inputs_number[i]];
    tag = find_tag_index_from_struct_of_tag(struct_of_tags, new_tag_index);
  }
  if (tag.index == 0)
  {
    cout << "invalid sequence of numbers" << endl;
    return;
  }
  else
  {
    string output = show_complete_tag_info(struct_of_tags, tag.index);
    cout << output << endl;
  }
}

int main()
{
  ifstream input_file(file_input_name);
  if (input_file.is_open())
  {
    string file_content( (istreambuf_iterator<char>(input_file)),  (istreambuf_iterator<char>()));
    vector<string> all_tags;
    vector<string> all_contents;
    vector<Tag> struct_of_tags;
    find_all_tags(file_content, all_tags);
    find_all_contens(file_content, all_contents);
    build_tag_relatives(all_tags, all_contents, struct_of_tags);
    string input_string;
    vector<int> inputs_number;
    cout << "Please Enter Sequnece of Number:" << endl;
    getline(cin, input_string);
    split_input_with_comma(input_string, inputs_number);
    find_desire_tag(inputs_number, struct_of_tags);
  }
  else
  {
    cout << "Can not open the input file" << endl;
  }
}
