#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Helper function to skip whitespace characters
void skipWhitespace(istringstream& input) {
    while (isspace(input.peek())) {
        input.get();
    }
}

// Helper function to parse strings enclosed in double quotes
string parseString(istringstream& input) {
    char c;
    string value;
    input.get(); // Consume the opening double quote

    while (input.get(c)) {
        if (c == '"') {
            break;
        }
        value += c;
    }

    return value;
}

// Function to parse a JSON object
json parseObject(istringstream& input) {
    json obj;
    string key;

    while (input.peek() != '}') {
        skipWhitespace(input);
        if (input.peek() == '}') // Check for empty object
            return {};
        key = parseString(input); // Extract the value from the JsonValue object
        skipWhitespace(input);

        if (input.peek() == '{') {
            input.get(); // Consume the opening curly brace
            obj[key] = parseObject(input);
            skipWhitespace(input);
        }
        else if (input.peek() == '"') {
            obj[key] = parseString(input);
            skipWhitespace(input);
        }

        if (input.peek() == '}') {
            input.get(); // Consume the closing curly brace
            break;
        }
    }
    return obj;
}

json parseJson(istringstream& input) {
    // Skip first line
    string line;
    getline(input, line);

    // Consume the opening curly brace
    while (input.peek() != '{') {
		input.get();
	} input.get();

    json data;
    data = parseObject(input);

	return data;
}

int main()
{
    string example = R"(
"Manifest"
{
    "name"                 "Application"
    "id"                       "123090291"
    "moreData"
    {
        "extras"           "true"
    }
    "blanktest"
    {
    }
}
)";

    istringstream inputStream(example);
    json data = parseJson(inputStream);

    cout << data.dump(4) << endl;

    return 0;
}