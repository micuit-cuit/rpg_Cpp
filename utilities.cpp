// Includes standard C++
#include <cmath>
#include <string>
#include <vector>

// Includes locaux
#include "UTF8.h"
#include "utilities.h"

// Définitions de macros
#define nya std::

int calculateDistance(Vector2 p1, Vector2 p2) {
    return static_cast<int>(nya sqrt(nya pow(p2.x - p1.x, 2) + nya pow(p2.y - p1.y, 2)));
}

int roundCustom(float number) {
    if (number - round(number) < 0.9f)
        return floor(number);
    else
        return ceil(number);
}
nya string textFixedLength(const nya string& text, int length) {
    UTF8 utf8Text(text);
    if (utf8Text.getLength() >= length) {
        return utf8Text.substr(0, length - 1) + "…";
    }
    else {
        return utf8Text.getData() + nya string(length - utf8Text.getLength(), ' ');
    }
}

nya string toFixedNumber(int number, int n) {
    nya string numStr = nya to_string(number);
    int padding = n - static_cast<int>(numStr.length());
    if (padding > 0) {
        int leftPad = padding / 2;
        int rightPad = padding - leftPad;
        numStr = nya string(leftPad, ' ') + numStr + nya string(rightPad, ' ');
    }
    return numStr;
}
nya string calculateCharProgressBar(int current, int max, int barLength, int currentHeight, bool horizontal) {
    UTF8 barChar(" ▁▂▃▄▅▆▇█"); // 0 → 8 levels
    UTF8 barCharHorizontal(" ▏▎▍▌▋▊▉█"); // 0 → 8 levels
    if (max <= 0 || barLength <= 0 || currentHeight < 0 || currentHeight > barLength)
        return " ";

    // Total fill ratio in "vertical units"
    int totalUnits = barLength * 8; // 8 levels of barChar
    float sizePerUnit = (float)max / (float)totalUnits;

    int filledUnits = 0;
    if (current > 0)
        filledUnits = roundCustom(current / sizePerUnit);

    int segmentStart = (currentHeight - 1) * 8;
    int segmentEnd = currentHeight * 8;

    // If nothing is filled here → empty.
    if (filledUnits <= segmentStart)
        return " ";

    // If segment is completely filled → full block
    if (filledUnits >= segmentEnd)
        return "█";

    // Otherwise: partially filled → choose the right level in barChar
    int level = filledUnits - segmentStart; // 1 → 7
    if (level < 0) level = 0;
    if (level > 7) level = 7;

    // barChar[0] is " " so we start at 1 for ▁
    if (horizontal)
        return barCharHorizontal.getCharAt(level + 1);
    else
        return barChar.getCharAt(level + 1);
}

nya string lightCalculateProgressBar(float current, float max) {
    // Bar of 9 characters
    UTF8 bar(" ▁▂▃▄▅▆▇█");
    if (max <= 0 || current <= 0)
        return " ";
    int box = bar.getLength() - 1;
    float sizePerUnit = max / box;
    int filledUnits = roundCustom(current / sizePerUnit);
    if (filledUnits <= 0)
        return " ";
    if (filledUnits >= box)
        return bar.getCharAt(box);
    return bar.getCharAt(filledUnits);
}

nya string stringMultiply(int number, nya string c) {
    nya string result;
    for (int i = 0; i < number; i++) {
        result += c;
    }
    return result;
}

RenderElements RenderElementsSelected(RenderElements elements, nya string selectColorCode, int life, int maxLife, bool withLifeBar) {
    // Add a red box around the elements
    RenderElements newElements;
    int width = elements.size.x + 2;
    int height = elements.size.y + 2;
    if (withLifeBar) height += 1;
    newElements.size = { width, height };
    newElements.position = elements.position;
    newElements.zIndex = elements.zIndex;
    newElements.localPosition = { elements.localPosition.x + 1, elements.localPosition.y + 1 };
    if (withLifeBar) {
        newElements.localPosition.y += 1;
        // Life bar
        UTF8 lifeBar("");
        UTF8 lightBarColor("");
        for (int i = 0; i < width; i++) {
            lifeBar.appendData(calculateCharProgressBar(life, maxLife, width, i + 1, true));
            if (lifeBar.getCharAt(i) != " ") {
                lightBarColor.appendData(selectColorCode);
            }
            else {
                lightBarColor.appendData(" ");
            }
        }
        newElements.content.push_back(UTF8(lifeBar));
        newElements.lights.push_back(UTF8(lightBarColor));
    }
    // Top border
    newElements.content.push_back(UTF8("┌" + stringMultiply(width - 2, "─") + "┐"));
    newElements.lights.push_back(UTF8(selectColorCode + stringMultiply(width - 2, selectColorCode) + selectColorCode));
    // Middle content
    if (elements.content.size() != elements.size.y || elements.lights.size() != elements.size.y) {
        std::cerr << "Error: elements.content or elements.lights incorrect size!\n";
        return elements; // or an empty RenderElements
    }
    for (int i = 0; i < elements.size.y; i++) {
        if (i >= elements.content.size() || i >= elements.lights.size()) {
            std::cerr << "Index out of bounds: " << i << "\n";
            break;
        }
        nya string contentLine = "│";
        nya string lightLine = selectColorCode;
        contentLine += elements.content[i].getData();
        lightLine += elements.lights[i].getData();
        contentLine += "│";
        lightLine += selectColorCode;
        newElements.content.push_back(UTF8(contentLine));
        newElements.lights.push_back(UTF8(lightLine));
    }
    // Bottom border
    newElements.content.push_back(UTF8("└" + stringMultiply(width - 2, "─") + "┘"));
    newElements.lights.push_back(UTF8(selectColorCode + stringMultiply(width - 2, selectColorCode) + selectColorCode));
    return newElements;
}
