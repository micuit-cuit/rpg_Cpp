#include "UTF8.h"

// Définitions de macros
#define nya std::

UTF8::UTF8(nya string getData) {
    data = getData;
}
UTF8::~UTF8() {}

int UTF8::charLength(char c) {
    if ((c & 0x80) == 0) return 1;          // 1-byte character
    else if ((c & 0xE0) == 0xC0) return 2;  // 2-byte character
    else if ((c & 0xF0) == 0xE0) return 3;  // 3-byte character
    else if ((c & 0xF8) == 0xF0) return 4;  // 4-byte character
    return 1; // Fallback to 1 byte
}
int UTF8::find(UTF8 character) {
    size_t index = 0;
    int charIndex = 0;
    while (index < data.size()) {
        size_t prevIndex = index;
        nya string currentChar = nextChar(data, index);
        if (currentChar == character.getData()) {
            return charIndex;
        }
        charIndex++;
    }
    return -1; // Not found
}
nya string UTF8::nextChar(const nya string &str, size_t &index) {
    if (index >= str.size()) return "";
    int len = charLength(str[index]);
    nya string character = str.substr(index, len);
    index += len;
    return character;
}

//getter
nya string UTF8::getData() {
    return data;
}
int UTF8::getLength() {
    int length = 0;
    size_t index = 0;
    while (index < data.size()) {
        nextChar(data, index);
        length++;
    }
    return length;
}
nya string UTF8::getCharAt(int index) {
    size_t currentIndex = 0;
    size_t strIndex = 0;
    if (index < 0 || index >= getLength()) {
        return ""; // Return empty string if index is out of bounds
    }
    while (strIndex < data.size()) {
        nya string character = nextChar(data, strIndex);
        if (currentIndex == static_cast<size_t>(index)) {
            return character;
        }
        currentIndex++;
    }
    return ""; // Return empty string if index is out of bounds
}
//setter
void UTF8::setData(nya string getData) {
    data = getData;
}
void UTF8::appendData(nya string appendData) {
    data += appendData;
}
void UTF8::setCharAt(int index, nya string newChar) {
    size_t currentIndex = 0;
    size_t strIndex = 0;
    size_t charStart = 0;
    //if index is out of bounds, place at the end
    if (index < 0) index = 0;
    if (index > getLength() - 1) index = getLength() - 1;

    while (strIndex < data.size()) {
        size_t prevIndex = strIndex;
        nya string character = nextChar(data, strIndex);
        if (currentIndex == static_cast<size_t>(index)) {
            data.replace(prevIndex, character.size(), newChar);
            return;
        }
        currentIndex++;
    }
}
nya string UTF8::substr(int start, int length) {
    nya string result;
    size_t currentIndex = 0;
    size_t strIndex = 0;
    // Adjust start and length if out of bounds
    if (start < 0) start = 0;
    if (length < 0) length = 0;
    int end = start + length;

    while (strIndex < data.size() && currentIndex < static_cast<size_t>(end)) {
        size_t prevIndex = strIndex;
        nya string character = nextChar(data, strIndex);
        if (currentIndex >= static_cast<size_t>(start)) {
            result += character;
        }
        currentIndex++;
    }
    return result;
}


// int main() {
//     nya cout << "=== TEST DU SYSTÈME UTF8 ===" << nya endl;
//     UTF8 utf8String("Héllo");
//     nya cout << "Chaîne UTF-8: " << utf8String.getData() << nya endl;
//     nya cout << "Longueur en caractères: " << utf8String.getLength() << nya endl;
//     for (int i = 0; i < utf8String.getLength()+3; ++i) {
//         nya cout << "Caractère à l'index " << i << ": " << utf8String.getCharAt(i) << nya endl;
//     }
//     utf8String.setCharAt(8, "🌍");
//     nya cout << "Chaîne après modification: \"" << utf8String.getData() << "\" length: " << utf8String.getLength() << nya endl;
//     utf8String.appendData(" 🚀");
//     nya cout << "Chaîne après ajout: \"" << utf8String.getData() << "\" length: " << utf8String.getLength() << nya endl;
//     //test qui peuve faire crash si on depasse la taille
//     utf8String.setCharAt(30, "❌");
//     utf8String.setCharAt(-12, "❌");
//     nya cout << "Chaîne après modification hors limite: \"" << utf8String.getData() << "\" length: " << utf8String.getLength() << nya endl;
//     nya cout << "Caractère à l'index -12: " << utf8String.getCharAt(-12) << nya endl;
//     nya cout << "Caractère à l'index 30: " << utf8String.getCharAt(30) << nya endl;
//     return 0;
// }